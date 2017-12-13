// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include <sstream>

#include "xaifBooster/utils/inc/PrintManager.hpp"
#include "xaifBooster/utils/inc/DbgLoggerManager.hpp"

#include "xaifBooster/system/inc/ActiveUseType.hpp"
#include "xaifBooster/system/inc/GraphVizDisplay.hpp"
#include "xaifBooster/system/inc/Assignment.hpp"
#include "xaifBooster/system/inc/BasicBlock.hpp"
#include "xaifBooster/system/inc/VariableSymbolReference.hpp"
#include "xaifBooster/system/inc/Argument.hpp"
#include "xaifBooster/system/inc/Intrinsic.hpp"
#include "xaifBooster/system/inc/ConceptuallyStaticInstances.hpp"
#include "xaifBooster/system/inc/CallGraph.hpp"
#include "xaifBooster/system/inc/Constant.hpp"

#include "xaifBooster/algorithms/TypeChange/inc/TemporariesHelper.hpp"

#include "xaifBooster/algorithms/Linearization/inc/ExpressionEdgeAlg.hpp"
#include "xaifBooster/algorithms/Linearization/inc/ExpressionVertexAlg.hpp"

#include "xaifBooster/algorithms/CrossCountryInterface/inc/AccumulationGraph.hpp"
#include "xaifBooster/algorithms/CrossCountryInterface/inc/EliminationException.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/AssignmentAlg.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/PrivateLinearizedComputationalGraph.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/PrivateLinearizedComputationalGraphEdge.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/PrivateLinearizedComputationalGraphVertex.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/PrivateLinearizedComputationalGraphAlgFactory.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/PrivateLinearizedComputationalGraphEdgeAlgFactory.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/PrivateLinearizedComputationalGraphVertexAlgFactory.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/PreaccumulationCounter.hpp" 
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/BasicBlockAlg.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/RemainderGraphWriters.hpp"


using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulation {

  bool Sequence::ourPermitAliasedLHSsFlag = false;

  PreaccumulationMetric::PreaccumulationMetric_E Sequence::ourPreaccumulationMetric = PreaccumulationMetric::SCARCITY_METRIC;

  bool Sequence::ourUseRandomizedHeuristicsFlag = false;

  /// for simulated annealing
  int Sequence::ourIterationsParameter=5000;

  /// for simulated annealing
  double Sequence::ourGamma=5.0;

  bool Sequence::ourUseReroutingsFlag = false;

  bool Sequence::ourPermitNarySaxFlag = false;

  Sequence::Sequence() : myBestElimination_p(0),
                         myBestRemainderGraph_p(0) {
    myComputationalGraph_p = BasicBlockAlg::getPrivateLinearizedComputationalGraphAlgFactory()->makeNewPrivateLinearizedComputationalGraph();
  }
  
  Sequence::~Sequence() { 
    for (InlinableSubroutineCallPList::iterator i(myPreaccumulationAllocationList.begin()); i != myPreaccumulationAllocationList.end(); ++i)
      if (*i)
        delete *i;
    for (AssignmentPList::iterator i(myFrontAssignmentList.begin()); i != myFrontAssignmentList.end(); ++i)
      if (*i)
        delete *i;
    for (AssignmentPList::iterator i(myEndAssignmentList.begin()); i != myEndAssignmentList.end(); ++i) 
      if (*i)
        delete *i;
    for (EliminationPList::iterator i(myEliminationPList.begin()); i != myEliminationPList.end(); ++i)
      if (*i)
        delete *i;
    if (myComputationalGraph_p)
      delete myComputationalGraph_p;
  }

  void
  Sequence::permitAliasedLHSs() {
    ourPermitAliasedLHSsFlag = true;
  }

  bool
  Sequence::doesPermitAliasedLHSs() {
    return ourPermitAliasedLHSsFlag;
  }

  void
  Sequence::useRandomizedHeuristics() {
    ourUseRandomizedHeuristicsFlag = true;
  }

  void
  Sequence::setPreaccumulationMetric(PreaccumulationMetric::PreaccumulationMetric_E aMetric) {
    ourPreaccumulationMetric = aMetric;
  } // end BasicBlockAlg::setPreaccumulationMetric()

  void
  Sequence::useReroutings() { 
    ourUseReroutingsFlag = true;
  } // end BasicBlockAlg::useReroutings()

  void
  Sequence::permitNarySax() {
    ourPermitNarySaxFlag=true;
  }

  bool
  Sequence::doesPermitNarySax() {
    return ourPermitNarySaxFlag;
  }

  void
  Sequence::printXMLHierarchyImpl(std::ostream& os) const {
    DBG_MACRO(DbgGroup::CALLSTACK,"xaifBoosterBasicBlockPreaccumulation::Sequence::printXMLHierarchyImpl: invoked for " << debug().c_str());
    // print all the stuff before the first element
    for(AssignmentPList::const_iterator fli(getFrontAssignmentList().begin()); fli != getFrontAssignmentList().end(); ++fli)
      (*fli)->printXMLHierarchy(os);

    // print the assignments themselves
    for (CAssignmentPList::const_iterator ai(myAssignmentPList.begin()); ai != myAssignmentPList.end(); ++ai)
      (*ai)->printXMLHierarchy(os);

    // print all the stuff after the last element
    // allocations
    for (InlinableSubroutineCallPList::const_iterator ali(getPreaccumulationAllocationList().begin()); ali != getPreaccumulationAllocationList().end(); ++ali)
      (*ali)->printXMLHierarchy(os);
    // assignments
    for(AssignmentPList::const_iterator eli(getEndAssignmentList().begin()); eli != getEndAssignmentList().end(); ++eli)
      (*eli)->printXMLHierarchy(os);
  }

  const PrivateLinearizedComputationalGraph&
  Sequence::getLCG() const {
    return *myComputationalGraph_p;
  }

  xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall& 
  Sequence::addAllocation(const VariableSymbolReference& toBeAllocated,
			  const Variable& variableToMatch,
	                  bool forPreaccmulation) {
    xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall* theSRCall_p=new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall("oad_AllocateMatching"); 
    theSRCall_p->setId("xaifBoosterBasicBlockPreaccumulation::Sequence::addAllocation");
    if (forPreaccmulation)
      myPreaccumulationAllocationList.push_back(theSRCall_p);
    else
      myDerivativePropagator.getPropagationAllocationList().push_back(theSRCall_p);
    // first argument
    Variable& toBeAllocatedVariable(theSRCall_p->addConcreteArgument(1).getArgument().getVariable());
    VariableSymbolReference* 
      tobeAllocateVariableSymbolReference_p(new VariableSymbolReference(toBeAllocated.getSymbol(),
									toBeAllocated.getScope()));
    tobeAllocateVariableSymbolReference_p->setId("1");
    tobeAllocateVariableSymbolReference_p->setAnnotation("xaifBoosterBasicBlockPreaccumulation::Sequence::addAllocation");
    toBeAllocatedVariable.supplyAndAddVertexInstance(*tobeAllocateVariableSymbolReference_p);
    toBeAllocatedVariable.getAliasMapKey().setTemporary();
    toBeAllocatedVariable.getDuUdMapKey().setTemporary();
    // second argument
    variableToMatch.copyMyselfInto(theSRCall_p->addConcreteArgument(2).getArgument().getVariable());
    return *theSRCall_p;
  } 

  Assignment& Sequence::appendFrontAssignment(bool withAlgorithm) { 
    Assignment* theAssignment_p=new Assignment(withAlgorithm);
    myFrontAssignmentList.push_back(theAssignment_p);
    return *theAssignment_p;
  }

  Assignment& Sequence::appendEndAssignment(bool withAlgorithm) { 
    Assignment* theAssignment_p=new Assignment(withAlgorithm);
    myEndAssignmentList.push_back(theAssignment_p);
    return *theAssignment_p;
  }

  const Sequence::InlinableSubroutineCallPList& Sequence::getPreaccumulationAllocationList() const { 
    return myPreaccumulationAllocationList;
  }

  const AssignmentPList& Sequence::getFrontAssignmentList() const { 
    return myFrontAssignmentList;
  }

  const AssignmentPList& Sequence::getEndAssignmentList() const { 
    return myEndAssignmentList;
  }

  std::string Sequence::debug() const {
    std::ostringstream out;
    out << "xaifBoosterBasicBlockPreaccumulation::Sequence[" << this
        << ",myAssignmentPList.size():" << myAssignmentPList.size()
        << ",myComputationalGraph_p->" << myComputationalGraph_p->debug().c_str()
        << ",myPreaccumulationAllocationList.size():" << myPreaccumulationAllocationList.size()
        << ",myFrontAssignmentList.size():" << myFrontAssignmentList.size()
        << ",myEndAssignmentList.size():" << myEndAssignmentList.size()
        << ",myEliminationPList.size():" << myEliminationPList.size()
        << ",myBestElimination_p=" << myBestElimination_p
        << ",myBestRemainderGraph_p=" << myBestRemainderGraph_p
        << "]" << std::ends;
    return out.str();
  }

  bool
  Sequence::canIncorporate(const Assignment& aAssignment,
                           const BasicBlock& theBasicBlock) const {
    if (aAssignment.isNonInlinable())
      THROW_LOGICEXCEPTION_MACRO("xaifBoosterBasicBlockPreaccumulation::Sequence::canIncorporate:"
                                 << " called on noninlinable assignment " << aAssignment.debug().c_str());
    AssignmentAlg& theAssignmentAlg(dynamic_cast<AssignmentAlg&>(aAssignment.getAssignmentAlgBase()));
    if (theAssignmentAlg.getActiveFlag()) {
      // check for ambiguous identification among the arguments
      Expression::ConstVertexIteratorPair vip(theAssignmentAlg.getLinearizedRHS().vertices());
      for (Expression::ConstVertexIterator ExpressionVertexI(vip.first); ExpressionVertexI != vip.second; ++ExpressionVertexI) {
        if ((*ExpressionVertexI).isArgument()) {
          const Argument& theArgument(dynamic_cast<const Argument&>(*ExpressionVertexI));

          VertexIdentificationListActive::IdentificationResult
           theLHSIdResult(myVertexIdentificationListActiveLHS.canIdentify(theArgument.getVariable(),
                                                                          aAssignment.getId())),
           theRHSIdResult(myVertexIdentificationListActiveRHS.canIdentify(theArgument.getVariable()));
          // passivate if all possibly identified LHSs are passive
          VertexIdentificationList::IdentificationResult_E thePassiveIdResult(
           myVertexIdentificationListPassive.canIdentify(theArgument.getVariable(),
                                                         aAssignment.getId())
          );
          if (theLHSIdResult.getAnswer()==VertexIdentificationList::UNIQUELY_IDENTIFIED
           || theRHSIdResult.getAnswer()==VertexIdentificationList::UNIQUELY_IDENTIFIED) { 
            // do nothing,  push later
          } // end if 
          else if (thePassiveIdResult==VertexIdentificationList::UNIQUELY_IDENTIFIED) { 
            // note, that for the passive identification we have a misnomer here but 
            // uniquely identified means positively passive identfied regardless if 
            // the actual identification is unique to a particular LHS or not as long 
            // as all possibly identified LHSs are passive.
            // passivate this: 
            dynamic_cast<const xaifBoosterLinearization::ExpressionVertexAlg&>((*ExpressionVertexI).getExpressionVertexAlgBase()).passivate();
          } // end if 
          else { // the vertex cannot be uniquely identified
            if (theLHSIdResult.getAnswer()==VertexIdentificationList::NOT_IDENTIFIED) {
              // the RHS identification doesn't really matter since we cannot uniquely identify within the RHSs
              // it is only important that we don't alias a preceding LHS
              // don't need to do anything else at this point, just continue
            } // end if NOT_IDENTIFIED
            else { // there is an ambiguity
              // don't continue here. 
              // Note that this is the point where we cut off somewhat arbitrarily in 
              // that we decide to break the flattening here because it is convenient
              // but we could continue to flatten other not connected unambiguous portions 
              // into this graph at the cost of more maintenance. see the basic block 
              // flattening paper
              DBG_MACRO(DbgGroup::DATA,
                        "xaifBoosterBasicBlockPreaccumulation::Sequence::canIncorporate:"
                        << " forcing split into new Sequence because RHS argument " << dynamic_cast<const Argument&>(*ExpressionVertexI).debug().c_str()
                        << "of " << theAssignmentAlg.debug().c_str()
                        << " is ambiguously identified (may alias a preceeding LHS) according to " << myVertexIdentificationListActiveLHS.debug().c_str());
              return false;
            } // end else (theLHSIdResult.getAnswer()==AMBIGUOUSLY_IDENTIFIED)
          } // end else (no unique identification)
        } // end if Argument
      } // end for all vertices in this RHS

      // check whether the LHS possibly overlaps with a previous LHS in this graph
      if (!doesPermitAliasedLHSs()) {
        // We must split into a new computational graph when a LHS is determined to possibly alias with a previous LHS.
        // If we didn't, we would have a graph with two vertices that may correspond to the same variable,
        // and thus the graph would not properly represent the dependences in the code.
        // Consider the following case, where there is a possibility that p and q point to the same memory location:
        //   *q = a*b
        //   *p = b*c
        // When it comes time to propagate, both *p and *q will get a SAX along one inedge and a SAXPY along the other.
        // In the case where p = q, the second SAX applied will erase the contribution from the first SAX (along with any SAXPY applied in between).
        // As a computational graph encapsulates precedence information exclusively, the type of complication described above is not representable as part of the graph.
        // This is our motivation for stipulating that a new graph must be created.
        if (myVertexIdentificationListActiveLHS.aliasIdentify(aAssignment.getLHS()).getAnswer() != VertexIdentificationList::NOT_IDENTIFIED) {
          DBG_MACRO(DbgGroup::DATA,"xaifBoosterBasicBlockPreaccumulation::Sequence::canIncorporate:"
                                   << " forcing split into new Sequence because the LHS of " << theAssignmentAlg.debug().c_str()
                                   << " may alias a preceeding LHS according to : " << myVertexIdentificationListActiveLHS.debug().c_str());
          return false;
        }
      }
      return true;
    } // end if aAssignment is active
    else {
    //// if it's inactive but its LHS overwrites an indirectly active variable
      if (myVertexIdentificationListIndAct.overwrittenBy(aAssignment.getLHS(),
                                                            aAssignment.getId(),
                                                            theBasicBlock)) {
        DBG_MACRO(DbgGroup::DATA,"xaifBoosterBasicBlockPreaccumulation::Sequence::canIncorporate:"
                                 << " forcing split into new Sequence because the LHS of inactive " << theAssignmentAlg.debug().c_str()
                                 << " overwrites an IndAct according to " << myVertexIdentificationListIndAct.debug().c_str());
        return false;
      } // end if the LHS overwrites an IndAct
      return true;
    } // end else (the assignment is not active)
  }

  void
  Sequence::incorporateAssignment(const Assignment& aAssignment,
                                  const StatementIdList& theKnownAssignmentsIdList) {
    DBG_MACRO(DbgGroup::DATA,"xaifBoosterBasicBlockPreaccumulation::Sequence::incorporateAssignment:"
                             << " incorporating " << aAssignment.debug().c_str()
                             << " into " << debug().c_str());
    // replace this with an association (map?) between assignments and LCG (or merged graph) vertices
    myAssignmentPList.push_back(&aAssignment);

    // build myFlatteningMap (flatten arguments into previous expression vertices)
    Expression::ConstVertexIteratorPair vip(aAssignment.getRHS().vertices());
    for (Expression::ConstVertexIterator evi(vip.first); evi != vip.second; ++evi) {
      if ((*evi).isArgument()) { 
        const Argument& theArgument(dynamic_cast<const Argument&>(*evi));
        VertexIdentificationListActive::IdentificationResult theLHSIdResult(
         myVertexIdentificationListActiveLHS.canIdentify(theArgument.getVariable(),
                                                         aAssignment.getId())
        );
        VertexIdentificationListActive::IdentificationResult theRHSIdResult(
         myVertexIdentificationListActiveRHS.canIdentify(theArgument.getVariable())
        );
        if (theLHSIdResult.getAnswer() == VertexIdentificationList::UNIQUELY_IDENTIFIED) {
          myLHSFlatteningMap.addElement(&theArgument,&theLHSIdResult.getAssignment());
        }
        else if (theRHSIdResult.getAnswer() == VertexIdentificationList::UNIQUELY_IDENTIFIED) {
          myFlatteningMap.addElement(&theArgument,&theRHSIdResult.getExpressionVertex());
        }
        else { // cannot be uniquely identified
          myFlatteningMap.addElement(&theArgument,&theArgument);
          myLHSFlatteningMap.addElement(&theArgument,&aAssignment); // to indicate that no LHS identification was possible within this sequence
          if (theRHSIdResult.getAnswer() == VertexIdentificationList::NOT_IDENTIFIED) { 
            myVertexIdentificationListActiveRHS.addElement(theArgument,
                                                               aAssignment,
                                                               theKnownAssignmentsIdList);
            myVertexIdentificationListIndAct.addElement(theArgument.getVariable(),
                                                        aAssignment.getId());
            DBG_MACRO(DbgGroup::DATA,"xaifBoosterBasicBlockPreaccumulation::Sequence::incorporateAssignment(flatten):"
                                     << " could not identify " << theArgument.debug().c_str()
                                     << ", so added to RHS: " << myVertexIdentificationListActiveRHS.debug().c_str());
          }
        } // end else (cannot be uniquely identified)
      } // end if argument
    } // end for all expression vertices

    AssignmentAlg& theAssignmentAlg(dynamic_cast<AssignmentAlg&>(aAssignment.getAssignmentAlgBase()));
    if (theAssignmentAlg.getActiveFlag()) {
      //incorporateAssignmentIntoPLCG(aAssignment);
      // we need to keep the lists mutually exclusive. a left hand side cannot occur in the right hand side list
      myVertexIdentificationListActiveRHS.removeIfIdentifiable(aAssignment.getLHS());
      // an overwritten LHS needs to refer to the respective last definition
      myVertexIdentificationListActiveLHS.removeIfIdentifiable(aAssignment.getLHS());
      myVertexIdentificationListActiveLHS.addElement(aAssignment);
      // a known active lhs cannot have a passive identification
      myVertexIdentificationListPassive.removeIfIdentifiable(aAssignment.getLHS(),
                                                             aAssignment.getId());
    }
    else { // inactive (passive) assignments
      if (aAssignment.getLHS().getActiveType()) { // but the LHS has active type
        myVertexIdentificationListPassive.addElement(aAssignment.getLHS(),
                                                     aAssignment.getId());
        if (aAssignment.getLHS().getActiveFlag()) // this means the LHS has been passivated 
          myDerivativePropagator.addZeroDerivToEntryPList(aAssignment.getLHS());
      } // end if LHS is active type
    } // end else (the assignment is not active)
    myVertexIdentificationListIndAct.addElement(aAssignment.getLHS(),
                                                aAssignment.getId());
    // debug output
    if (DbgLoggerManager::instance()->isSelected(DbgGroup::GRAPHICS) && DbgLoggerManager::instance()->wantTag("cg"))
      getLCG().show("PartialSequencePLCG");
    DBG_MACRO(DbgGroup::DATA,
              "xaifBoosterBasicBlockPreaccumulation::Sequence::incorporateAssignment(flatten):"
              << " passive: " << myVertexIdentificationListPassive.debug().c_str()
              << " LHSIdList " << myVertexIdentificationListActiveLHS.debug().c_str()
              << " RHSIdList " << myVertexIdentificationListActiveRHS.debug().c_str()
              << " IndAct " << myVertexIdentificationListIndAct.debug().c_str()
             );
  }

  void
  Sequence::buildLinearizedComputationalGraph() {
    PrivateLinearizedComputationalGraph& theLCG(*myComputationalGraph_p);
    theLCG.clear();
    EVp2LCGVpMap theEVp2LCGVpMap; // local map to facilitate copy
    for (CAssignmentPList::const_iterator ai(myAssignmentPList.begin()); ai != myAssignmentPList.end(); ++ai) {
      const Assignment& aAssignment(**ai);
      if (dynamic_cast<const AssignmentAlg&>(aAssignment.getAssignmentAlgBase()).getActiveFlag()) {
        const PrivateLinearizedComputationalGraphVertex* theLHSLCGV_p(NULL);
        if (aAssignment.getRHS().getMaxVertex().isArgument()) {
          theLHSLCGV_p = &affixActiveDirectCopyAssignmentToComputationalGraph(aAssignment,
                                                                              theEVp2LCGVpMap);
        }
        else {
          theLHSLCGV_p = &affixActiveAssignmentToComputationalGraph(aAssignment,
                                                                    theEVp2LCGVpMap);
        }
        // associate the assignment with the LCGV that correspons to the LHS
        theLCG.mapAssignmentLHS2PLCGV(aAssignment,
                                      *theLHSLCGV_p);
        // as we step through the assignments we add all the left hand sides as dependendents
        // and when we are done with one flattening section we remove the ones not needed
        // in the future, done on the fly during the PLCG assignment2PLCGVertex mapping?
        theLCG.addToDependentList(*theLHSLCGV_p,
                                  aAssignment.getId());
      } // end if the assignment is active
      else {
        theLCG.addToPassiveStatementIdList(aAssignment.getId());
      }
    } // end for all assignments in this sequence

    // fill independents/dependents lists (dependents list is built on the fly in the future?)
    fillLCGIndependentsList();
    fillLCGDependentsList();

    if (DbgLoggerManager::instance()->isSelected(DbgGroup::GRAPHICS) && DbgLoggerManager::instance()->wantTag("cg")) {
      theLCG.show("flattenedPLCG");
    }
  }

  const PrivateLinearizedComputationalGraphVertex&
  Sequence::affixActiveAssignmentToComputationalGraph(const Assignment& aAssignment,
                                                      EVp2LCGVpMap& theEVp2LCGVpMap) {
    if (aAssignment.getRHS().getMaxVertex().isArgument()) {
      THROW_LOGICEXCEPTION_MACRO("Sequence::affixActiveAssignmentToComputationalGraph: called on " << aAssignment.debug().c_str()
                              << " where the max vertex of the RHS is an argument (i.e., this is a direct copy assignment)");
    }
    PrivateLinearizedComputationalGraph& theLCG(*myComputationalGraph_p);
    // copy RHS expression vertices
    Expression::ConstVertexIteratorPair vip(aAssignment.getRHS().vertices());
    for (Expression::ConstVertexIterator evi(vip.first); evi != vip.second; ++evi) {
      const ExpressionVertex& aExpressionVertex(*evi);
      if (!dynamic_cast<const xaifBoosterLinearization::ExpressionVertexAlg&>(aExpressionVertex.getExpressionVertexAlgBase()).isActive())
        continue; // skip passive expression vertices

      // find an existing corresp. LCGV or make a new one
      const PrivateLinearizedComputationalGraphVertex* correspLCGV_p;
      if (aExpressionVertex.isArgument()
       && myLHSFlatteningMap.hasElement(dynamic_cast<const Argument*>(&aExpressionVertex))
       && myLHSFlatteningMap.getElement(dynamic_cast<const Argument*>(&aExpressionVertex)) != &aAssignment) {
	const Assignment& theAssignment(dynamic_cast<const Assignment&>(*myLHSFlatteningMap.getElement(dynamic_cast<const Argument*>(&aExpressionVertex))));
        correspLCGV_p = &theLCG.getLCGVertexForAssignmentLHS(theAssignment);
      }
      else if (aExpressionVertex.isArgument() // an Argument that can be uniquely identifed to an existing LCGV
       && myFlatteningMap.hasElement(dynamic_cast<const Argument*>(&aExpressionVertex))
       && myFlatteningMap.getElement(dynamic_cast<const Argument*>(&aExpressionVertex)) != &aExpressionVertex
       && theEVp2LCGVpMap.hasElement(myFlatteningMap.getElement(dynamic_cast<const Argument*>(&aExpressionVertex)))) {
	const ExpressionVertex& theEV(dynamic_cast<const ExpressionVertex&>(*myFlatteningMap.getElement(dynamic_cast<const Argument*>(&aExpressionVertex))));
        correspLCGV_p = theEVp2LCGVpMap.getElement(&theEV);
      }
      else { // we must make a new vertex
        PrivateLinearizedComputationalGraphVertex& theNewLCGV(
         *BasicBlockAlg::getPrivateLinearizedComputationalGraphVertexAlgFactory()->makeNewPrivateLinearizedComputationalGraphVertex()
        );
        theLCG.supplyAndAddVertexInstance(theNewLCGV);
        // simple rule: if the origin vertex is the maximal one, then we corresp. to the LHS variable (no issue for direct copy edges)
        if (&aExpressionVertex != &aAssignment.getRHS().getMaxVertex())
          theNewLCGV.setOrigin(aExpressionVertex,
                               aAssignment);
        else // the max expression vertex corresp. to the LHS (we assume we are NOT in a direct copy assignment)
          theNewLCGV.setOrigin(aAssignment);
        correspLCGV_p = &theNewLCGV;
      } // end else (new PLCG vertex)
      theEVp2LCGVpMap.addElement(&aExpressionVertex,correspLCGV_p);
      //if (!aExpressionVertex.isArgument())
      //  THROW_LOGICEXCEPTION_MACRO("Sequence::buildLinearizedComputationalGraph:"
      //                             << " non-argument " << aExpressionVertex.debug().c_str()
      //                             << " is not mapped to itself in the flattening map");
    } // end iterate over RHS expression vertices

    // copy RHS expression edges
    Expression::ConstEdgeIteratorPair eip(aAssignment.getRHS().edges());
    for (Expression::ConstEdgeIterator eei(eip.first); eei != eip.second; ++eei) {
      const ExpressionVertex
       &theSourceEV(aAssignment.getRHS().getSourceOf(*eei)),
       &theTargetEV(aAssignment.getRHS().getTargetOf(*eei));
      if (!dynamic_cast<const xaifBoosterLinearization::ExpressionVertexAlg&>(theSourceEV.getExpressionVertexAlgBase()).isActive()
       || !dynamic_cast<const xaifBoosterLinearization::ExpressionVertexAlg&>(theTargetEV.getExpressionVertexAlgBase()).isActive())
        continue;
      const xaifBoosterLinearization::ExpressionEdgeAlg& theEEAlg(
        dynamic_cast<const xaifBoosterLinearization::ExpressionEdgeAlg&>((*eei).getExpressionEdgeAlgBase())
      );
      PartialDerivativeKind::PartialDerivativeKind_E thePartialDerivativeKind(theEEAlg.getPartialDerivativeKind());
      if (thePartialDerivativeKind == PartialDerivativeKind::PASSIVE) 
        continue;
      const PrivateLinearizedComputationalGraphVertex
       &theSourceLCGV(*theEVp2LCGVpMap.getElement(&theSourceEV)),
       &theTargetLCGV(*theEVp2LCGVpMap.getElement(&theTargetEV));
      // filter out parallel edges:
      PrivateLinearizedComputationalGraph::OutEdgeIteratorPair anOutEdgeItPair(theLCG.getOutEdgesOf(theSourceLCGV));
      PrivateLinearizedComputationalGraph::OutEdgeIterator aPrivLinCompGEdgeI(anOutEdgeItPair.first), aPrivLinCompGEdgeIEnd(anOutEdgeItPair.second);
      for (; aPrivLinCompGEdgeI != aPrivLinCompGEdgeIEnd; ++aPrivLinCompGEdgeI) { 
        if (&theTargetLCGV == &(theLCG.getTargetOf(*aPrivLinCompGEdgeI)))
          break; // already have such an edge in here
      } // end for 
      if (aPrivLinCompGEdgeI != aPrivLinCompGEdgeIEnd) {  // this is an edge parallel to an existing  edge
        dynamic_cast<PrivateLinearizedComputationalGraphEdge&>(*aPrivLinCompGEdgeI).addParallel(*eei);
        continue; // we can skip the rest
      }
      PrivateLinearizedComputationalGraphEdge& theNewPLCGEdge(
       *(BasicBlockAlg::getPrivateLinearizedComputationalGraphEdgeAlgFactory())->makeNewPrivateLinearizedComputationalGraphEdge()
      );
      // set the back reference
      theNewPLCGEdge.setLinearizedExpressionEdge(*eei);
      if (thePartialDerivativeKind == PartialDerivativeKind::LINEAR_ONE || thePartialDerivativeKind == PartialDerivativeKind::LINEAR_MINUS_ONE)
        theNewPLCGEdge.setEdgeLabelType(xaifBoosterCrossCountryInterface::LinearizedComputationalGraphEdge::UNIT_LABEL);
      else if (thePartialDerivativeKind == PartialDerivativeKind::LINEAR)
        theNewPLCGEdge.setEdgeLabelType(xaifBoosterCrossCountryInterface::LinearizedComputationalGraphEdge::CONSTANT_LABEL);
      theLCG.supplyAndAddEdgeInstance(theNewPLCGEdge,
                                      theSourceLCGV,
                                      theTargetLCGV);
    //DBG_MACRO(DbgGroup::DATA,"xaifBoosterBasicBlockPreaccumulation::Sequence::incorporateAssignmentIntoPLCG(flatten)"
    //                         << " Edge source:" << theSourceLCGV.debug().c_str()
    //                         << " target " << theTargetLCGV.debug().c_str());
    }  // end for all expression edges
    return *theEVp2LCGVpMap.getElement(&aAssignment.getRHS().getMaxVertex());
  }

  const PrivateLinearizedComputationalGraphVertex&
  Sequence::affixActiveDirectCopyAssignmentToComputationalGraph(const Assignment& aAssignment,
                                                                EVp2LCGVpMap& theEVp2LCGVpMap) {
    PrivateLinearizedComputationalGraph& theLCG(*myComputationalGraph_p);
    const ExpressionVertex& theMaxEV(aAssignment.getRHS().getMaxVertex());
    if (!theMaxEV.isArgument()) {
      THROW_LOGICEXCEPTION_MACRO("Sequence::affixActiveDirectCopyAssignmentToComputationalGraph:"
                              << " called on " << aAssignment.debug().c_str()
                              << " where the max vertex of the RHS is not an argument (i.e., this is not a direct copy assignment)");
    }
    if (!dynamic_cast<const xaifBoosterLinearization::ExpressionVertexAlg&>(theMaxEV.getExpressionVertexAlgBase()).isActive()) {
      THROW_LOGICEXCEPTION_MACRO("Sequence::affixActiveDirectCopyAssignmentToComputationalGraph:"
                              << " called on with inactive maximal RHS vertex "
                              << dynamic_cast<const xaifBoosterLinearization::ExpressionVertexAlg&>(theMaxEV).debug().c_str());
    }

    // make a new LCGVertex for the TARGET of the direct copy (in the LCG)
    PrivateLinearizedComputationalGraphVertex& theTargetLCGV(
     *(BasicBlockAlg::getPrivateLinearizedComputationalGraphVertexAlgFactory()->makeNewPrivateLinearizedComputationalGraphVertex())
    );
    theLCG.supplyAndAddVertexInstance(theTargetLCGV);
    theTargetLCGV.setOrigin(aAssignment);
    //theLHSLCGVertex_p->associateExpressionVertex(aAssignment.getRHS().getMaxVertex());

    // determine the LCGVertex for the SOURCE of the direct copy (find an existing corresp. LCGV or make a new one)
    const Argument& theRHSArgument(dynamic_cast<const Argument&>(theMaxEV));
    const PrivateLinearizedComputationalGraphVertex* theSourceLCGV_p(NULL);
    if (myLHSFlatteningMap.hasElement(&theRHSArgument)
       && myLHSFlatteningMap.getElement(&theRHSArgument) != &aAssignment) {
      const Assignment& theAssignment(dynamic_cast<const Assignment&>(*myLHSFlatteningMap.getElement(&theRHSArgument)));
      theSourceLCGV_p = &theLCG.getLCGVertexForAssignmentLHS(theAssignment);
    }
    else if (myLHSFlatteningMap.hasElement(&theRHSArgument)
      && myFlatteningMap.getElement(&theRHSArgument) != &theRHSArgument
      && theEVp2LCGVpMap.hasElement(myFlatteningMap.getElement(&theRHSArgument))) { // the RHS EV flattens to another EV (which should be an existing LCGV)
      const ExpressionVertex& theEV(dynamic_cast<const ExpressionVertex&>(*myFlatteningMap.getElement(&theRHSArgument)));
      theSourceLCGV_p = theEVp2LCGVpMap.getElement(&theEV);
    }
    else { // we must make a new vertex
      PrivateLinearizedComputationalGraphVertex& theNewLCGV(
       *BasicBlockAlg::getPrivateLinearizedComputationalGraphVertexAlgFactory()->makeNewPrivateLinearizedComputationalGraphVertex()
      );
      theLCG.supplyAndAddVertexInstance(theNewLCGV);
      // simple rule: if the origin vertex is the maximal one, then we corresp. to the LHS variable (no issue for direct copy edges)
      theNewLCGV.setOrigin(theMaxEV,
                           aAssignment);
      theSourceLCGV_p = &theNewLCGV;
    } // end else (new PLCG vertex)
    theEVp2LCGVpMap.addElement(&theMaxEV, theSourceLCGV_p);

    // we need to add the direct copy edge, we can't set a back reference because there is none
    PrivateLinearizedComputationalGraphEdge& theNewDCEdge(
     *(BasicBlockAlg::getPrivateLinearizedComputationalGraphEdgeAlgFactory()->makeNewPrivateLinearizedComputationalGraphEdge())
    );
    theNewDCEdge.setDirectCopyEdge();
    theLCG.supplyAndAddEdgeInstance(theNewDCEdge,
                                    *theSourceLCGV_p,
                                    theTargetLCGV);
    return theTargetLCGV;
  }

  void
  Sequence::fillLCGIndependentsList() {
    PrivateLinearizedComputationalGraph::VertexIteratorPair p(myComputationalGraph_p->vertices());
    for (PrivateLinearizedComputationalGraph::VertexIterator it(p.first); it != p.second; ++it)
      // here we should have constants etc. already removed
      // JU: this is temporary until we have r/w analysis
      if (!myComputationalGraph_p->numInEdgesOf(*it))
	myComputationalGraph_p->addToIndependentList(*it);
  }

  void
  Sequence::fillLCGDependentsList() {
    // now look at all the vertices in the dependent list 
    // and remove the ones not needed as indicated by 
    // the duud information:
    const xaifBoosterCrossCountryInterface::LinearizedComputationalGraph::VertexPointerList& theDepVertexPList(myComputationalGraph_p->getDependentList());
    for (xaifBoosterCrossCountryInterface::LinearizedComputationalGraph::VertexPointerList::const_iterator aDepVertexPListI(theDepVertexPList.begin());
	 aDepVertexPListI!=theDepVertexPList.end();) { 
      // cast it first
      const PrivateLinearizedComputationalGraphVertex& myPrivateVertex(dynamic_cast<const PrivateLinearizedComputationalGraphVertex&>(**aDepVertexPListI));
      // advance the iterator before we delete anything:
      ++aDepVertexPListI;
      // all the dependent ones should have the LHS set
      const StatementIdSetMapKey& aDuUdMapKey(myPrivateVertex.getOriginalVariable().getDuUdMapKey()); 
      if (aDuUdMapKey.getKind()==InfoMapKey::TEMP_VAR) { 
        // now the assumption is that temporaries are local to the flattened Sequence
        // and we can remove: 
        myComputationalGraph_p->removeFromDependentList(myPrivateVertex);
        continue;
      }
      DuUdMapUseResult theDuUdMapUseResult(
       ConceptuallyStaticInstances::instance()->getCallGraph().getDuUdMap().use(aDuUdMapKey,
                                                                                myPrivateVertex.getStatementId(),
                                                                                myComputationalGraph_p->getStatementIdLists())
      );
      if (theDuUdMapUseResult.myAnswer==DuUdMapUseResult::AMBIGUOUS_INSIDE 
	  || 
	  theDuUdMapUseResult.myAnswer==DuUdMapUseResult::UNIQUE_INSIDE) { 
	if (!myComputationalGraph_p->numOutEdgesOf(myPrivateVertex)) { 
	  if (theDuUdMapUseResult.myActiveUse!=ActiveUseType::PASSIVEUSE) { 
	    // if the use is not strictly passive then in case of UNIQUE_INSIDE this vertex 
	    // should not be maximal and in case of AMBIGUOUS_INSIDE there should have 
	    // been a split. 
	    THROW_LOGICEXCEPTION_MACRO("Sequence::fillLCGDependentsList: attempting to remove a maximal vertex " << myPrivateVertex.debug().c_str()
                                       << " key " << aDuUdMapKey.debug().c_str() << " from the dependent list");
	  }
	  // else do nothing, just leave it. This may indicate some inconsistency in 
	  // the activity results
	} // end if maximal
	else { // has outedges
          // we only use it in the scope of this flattened sequence, therefore remove it
          DBG_TAG_MACRO(DbgGroup::DATA,"depList","Sequence::fillLCGDependentsList:"
                        << " removing dependent " << myPrivateVertex.getOriginalVariable().debug().c_str()
                        << " list size: " << theDepVertexPList.size()); 
          myComputationalGraph_p->removeFromDependentList(myPrivateVertex);
          continue;
	}
      } // end if unique or ambiguous (inside) identification
      else { 
        if (DbgLoggerManager::instance()->isSelected(DbgGroup::DATA)) { 
          if (myComputationalGraph_p->numOutEdgesOf(myPrivateVertex)) { 
            DBG_TAG_MACRO(DbgGroup::DATA,"depList","Sequence::fillLCGDependentsList:"
                          << " keeping non-maximal dependent " << myPrivateVertex.getOriginalVariable().debug().c_str()
                          << " key is " << aDuUdMapKey.debug().c_str()
                          << " use result is " << theDuUdMapUseResult.myAnswer
                          << " lists are " << myComputationalGraph_p->getStatementIdLists().debug().c_str()); 
          }
          else {
            DBG_TAG_MACRO(DbgGroup::DATA,"depList","Sequence::fillLCGDependentsList:"
                          << " keeping regular dependent " << myPrivateVertex.getOriginalVariable().debug().c_str()
                          << " key is " << aDuUdMapKey.debug().c_str());
          }
        }
      } // end else (neither ambiguous nor UNIQUE_INSIDE)
    } // end for all vertices
  }

  void Sequence::runElimination() { 
    // initialize the graph transformation(s)
    switch (ourPreaccumulationMetric) {
    case PreaccumulationMetric::OPERATIONS_METRIC: {
      addNewElimination().initAsOperations();
      if (ourUseRandomizedHeuristicsFlag) {
        addNewElimination().initAsOperationsRandom();
        addNewElimination().initAsLSAVertex(ourIterationsParameter,ourGamma);
        //addNewElimination().initAsLSAFace(ourIterationsParameter,ourGamma);
      } // end randomized heuristics
      break;
    } // end OPERATIONS
    case PreaccumulationMetric::SCARCITY_METRIC: {
      addNewElimination().initAsScarceElimination();
      if (ourUseRandomizedHeuristicsFlag)
        addNewElimination().initAsScarceRandomElimination();
      if (ourUseReroutingsFlag) {
        addNewElimination().initAsScarceTransformation();
        if (ourUseRandomizedHeuristicsFlag)
          addNewElimination().initAsScarceRandomTransformation();
      }
      break;
    } // end SCARCITY
    default: {
      THROW_LOGICEXCEPTION_MACRO("Sequence::runElimination: unknown preaccumulation metric "
                                 << PreaccumulationMetric::toString(ourPreaccumulationMetric));
      break;
    } // end default
    } // end switch (ourPreaccumulationMetric)

    // perform the transformations and build the accumulation graph
    for (EliminationPList::iterator elim_i = getEliminationPList().begin(); elim_i != getEliminationPList().end(); ++elim_i) {
      try {
        (*elim_i)->eliminate();	
      }
      catch(xaifBoosterCrossCountryInterface::EliminationException e) { 
	THROW_LOGICEXCEPTION_MACRO("BasicBlockAlg::runElimination: " << (*elim_i)->getDescription()
                                   << ": " << e.getReason().c_str());
      }
      (*elim_i)->buildAccumulationGraph();
      DBG_MACRO(DbgGroup::METRIC, " Sequence " << this
                               << " by " << (*elim_i)->getDescription()
                               << ": " << (*elim_i)->getCounter().debug().c_str()
                               << " with " << (*elim_i)->getNumReroutings() << " reroutings");
    } // end iterate over all Eliminations for this Sequence
    determineBestElimination(ourPreaccumulationMetric);
    DBG_MACRO(DbgGroup::METRIC, " Sequence " << this
                             << " best is " << getBestElimination().getDescription()
                             << ": " << getBestElimination().getCounter().debug().c_str()
                             << " with " << getBestElimination().getNumReroutings() << " reroutings");
  }

  xaifBoosterCrossCountryInterface::Elimination&
  Sequence::addNewElimination() {
    xaifBoosterCrossCountryInterface::Elimination* theElimination_p = new xaifBoosterCrossCountryInterface::Elimination(*myComputationalGraph_p);
    myEliminationPList.push_back(theElimination_p);
    return *theElimination_p;
  }

  void Sequence::determineBestElimination(PreaccumulationMetric::PreaccumulationMetric_E aMetric) {
    if (myEliminationPList.empty())
      THROW_LOGICEXCEPTION_MACRO("Sequence::determineBestElimination() : no eliminations, thus no results");
    if (myBestElimination_p)
      THROW_LOGICEXCEPTION_MACRO("Sequence::determineBestElimination() : myBestElimination_p already set");
    myBestElimination_p = *(myEliminationPList.begin());
    for (EliminationPList::iterator i = ++(myEliminationPList.begin()); i != myEliminationPList.end(); ++i) { 
      if ((*i)->getCounter() < myBestElimination_p->getCounter())
        myBestElimination_p = *i;
    } // end iterate over all Eliminations
  } 

  const xaifBoosterCrossCountryInterface::Elimination& Sequence::getBestElimination() const {
    if (!myBestElimination_p)
      THROW_LOGICEXCEPTION_MACRO("Sequence::getBestElimination: myBestElimination_p not set");
    return *myBestElimination_p;
  }

  xaifBoosterCrossCountryInterface::Elimination&
  Sequence::getBestElimination() {
    if (!myBestElimination_p)
      THROW_LOGICEXCEPTION_MACRO("Sequence::getBestElimination: myBestElimination_p not set");
    return *myBestElimination_p;
  } 

  RemainderGraph& 
  Sequence::getBestRemainderGraph() {
    if (!myBestElimination_p)
      THROW_LOGICEXCEPTION_MACRO("Sequence::getBestRemainderGraph: myBestElimination_p not set");
    if (!myBestRemainderGraph_p) { 
      myBestRemainderGraph_p=new RemainderGraph();
      myBestRemainderGraph_p->initFrom(getBestElimination());
    }
    return *myBestRemainderGraph_p;
  } 

  Sequence::EliminationPList& Sequence::getEliminationPList() {
    if (myEliminationPList.empty())
      THROW_LOGICEXCEPTION_MACRO("Sequence::getEliminationPList: myEliminationP:List is empty");
    return myEliminationPList;
  }

  bool
  Sequence::hasExpression(const Expression& anExpression) const {
    // NOTE: this currently does not check the original basic block elements.
    // check the front assignment list
    for (AssignmentPList::const_iterator frontAssI = myFrontAssignmentList.begin();
         frontAssI != myFrontAssignmentList.end(); ++frontAssI)
      if ((*frontAssI)->hasExpression(anExpression))
        return true;
    // check the end assignment list
    for (AssignmentPList::const_iterator endAssI = myEndAssignmentList.begin();
         endAssI != myEndAssignmentList.end(); ++endAssI)
      if ((*endAssI)->hasExpression(anExpression))
        return true;
    // check the derivative propagator
    return myDerivativePropagator.hasExpression(anExpression);
  }

  const CAssignmentPList&
  Sequence::getAssignmentPList() const {
    return myAssignmentPList;
  }

  void
  Sequence::generateRemainderGraphPropagators() { 
    const RemainderGraph& theRemainderGraph(getBestRemainderGraph());
    getBestRemainderGraph().initVisit();
    bool done = false;
    while(!done) {
      done = true;
      RemainderGraph::ConstVertexIteratorPair aVertexIP(theRemainderGraph.vertices());
      for(RemainderGraph::ConstVertexIterator anLCGVertI(aVertexIP.first),anLCGvertEndI(aVertexIP.second);
	  anLCGVertI != anLCGvertEndI; ++anLCGVertI) {
	const RemainderGraphVertex& theRemainderTargetV(*anLCGVertI);
	// skip visited vertices
	if (theRemainderTargetV.wasVisited()) continue;
	// skip minimal vertices
	if (!theRemainderGraph.numInEdgesOf(theRemainderTargetV)) {
	  theRemainderTargetV.setVisited();
	  continue;
	}
	// check whether predecessors have been visited
	RemainderGraph::ConstInEdgeIteratorPair inEdgeIP (theRemainderGraph.getInEdgesOf(theRemainderTargetV));
	RemainderGraph::ConstInEdgeIterator iei (inEdgeIP.first), ie_end (inEdgeIP.second);
	for (; iei != ie_end; ++iei) // break on unvisited predecessor
	  if (!theRemainderGraph.getSourceOf(*iei).wasVisited()) break;
	if (iei != ie_end) // skip this vertex if a predecessor hasn't been visited
	  done = false;
	else { // all preds visited, so visit this vertex
	  theRemainderTargetV.setVisited();
          if (doesPermitNarySax())
            propagateToRemainderVertex_narySax(theRemainderTargetV);
          else 
            propagateToRemainderVertex(theRemainderTargetV);
	} // end visit
      } // end iterate over all vertices
    } // end while(!done)
    getBestRemainderGraph().finishVisit();
  }

  void
  Sequence::propagateToRemainderVertex(const RemainderGraphVertex& theRemainderTargetV) {
    const RemainderGraph& theRemainderGraph(getBestRemainderGraph());
    std::list<const RemainderGraphEdge*> passiveInEdges,linearOneInEdges,linearMinusOneInEdges,linearInEdges,nonlinearInEdges;

    // first iterate over inedges to build up lists of different types of partial derivative kinds
    RemainderGraph::ConstInEdgeIteratorPair inEdgeIP (theRemainderGraph.getInEdgesOf(theRemainderTargetV));
    for (RemainderGraph::ConstInEdgeIterator iei (inEdgeIP.first), ie_end (inEdgeIP.second);
         iei != ie_end; ++iei) {
      const RemainderGraphEdge& theRemainderGraphEdge(*iei);
      switch (theRemainderGraphEdge.getAccumulationGraphVertex().getPartialDerivativeKind()) {
      case PartialDerivativeKind::PASSIVE:
	passiveInEdges.push_back(&theRemainderGraphEdge);
	break;
      case PartialDerivativeKind::LINEAR_ONE:
	linearOneInEdges.push_back(&theRemainderGraphEdge);
	break;
      case PartialDerivativeKind::LINEAR_MINUS_ONE:
	linearMinusOneInEdges.push_back(&theRemainderGraphEdge);
	break;
      case PartialDerivativeKind::LINEAR:
	linearInEdges.push_back(&theRemainderGraphEdge);
	break;
      case PartialDerivativeKind::NONLINEAR:
	nonlinearInEdges.push_back(&theRemainderGraphEdge);
	break;
      default:
	THROW_LOGICEXCEPTION_MACRO("BasicBlockPreaccumulation::Sequence::propagateToRemainderVertex:"
				   << " invalid PDK (" << PartialDerivativeKind::toString(theRemainderGraphEdge.getAccumulationGraphVertex().getPartialDerivativeKind()) << ") for saxpy factor");
	break;
      } // end switch on PDK
    } // end for all inedges

    bool isZero = true;

    // LINEAR_ONE: SetDeriv and IncDeriv
    for (std::list<const RemainderGraphEdge*>::const_iterator loi = linearOneInEdges.begin();
         loi != linearOneInEdges.end(); ++loi) {
      const RemainderGraphVertex& theSourceVertex(theRemainderGraph.getSourceOf(**loi));
      if (isZero) // SetDeriv
        myDerivativePropagator.addSetDerivToEntryPList(theRemainderTargetV.getPropagationVariable(),
                                                       theSourceVertex.getPropagationVariable());
      else // IncDeriv
        myDerivativePropagator.addIncDerivToEntryPList(theRemainderTargetV.getPropagationVariable(),
                                                       theSourceVertex.getPropagationVariable());
      isZero = false;
    }
    // LINEAR_MINUS_ONE: SetNegDeriv and DecDeriv
    for (std::list<const RemainderGraphEdge*>::const_iterator lmoi = linearMinusOneInEdges.begin();
         lmoi != linearMinusOneInEdges.end(); ++lmoi) {
      const RemainderGraphVertex& theSourceVertex(theRemainderGraph.getSourceOf(**lmoi));
      if (isZero) // SetNegDeriv
        myDerivativePropagator.addSetNegDerivToEntryPList(theRemainderTargetV.getPropagationVariable(),
                                                          theSourceVertex.getPropagationVariable());
      else // DecDeriv
        myDerivativePropagator.addDecDerivToEntryPList(theRemainderTargetV.getPropagationVariable(),
                                                       theSourceVertex.getPropagationVariable());
      isZero = false;
    }
    // LINEAR
    for (std::list<const RemainderGraphEdge*>::const_iterator li = linearInEdges.begin();
         li != linearInEdges.end(); ++li) {
      const RemainderGraphEdge& currentRGE(**li);
      Constant theConstantFactor (SymbolType::REAL_STYPE, false);
      theConstantFactor.setId(1);
      theConstantFactor.setdouble(currentRGE.getAccumulationGraphVertex().getValue());
      xaifBoosterDerivativePropagator::DerivativePropagatorSaxpy& theNewSaxpy
	(myDerivativePropagator.addSaxpyToEntryPList(theConstantFactor,
                                                     (theRemainderGraph.getSourceOf(currentRGE)).getPropagationVariable(),
                                                     theRemainderTargetV.getPropagationVariable()));
      if (isZero)
        theNewSaxpy.useAsSax();
      isZero = false;
    }
    // NONLINEAR - sax/saxpy
    for (std::list<const RemainderGraphEdge*>::const_iterator nli = nonlinearInEdges.begin();
         nli != nonlinearInEdges.end(); ++nli) {
      const RemainderGraphEdge& currentRGE(**nli);
      xaifBoosterDerivativePropagator::DerivativePropagatorSaxpy& theNewSaxpy
	(myDerivativePropagator.addSaxpyToEntryPList(currentRGE.getAccumulationGraphVertex().getLHSVariable(),
                                                     (theRemainderGraph.getSourceOf(currentRGE)).getPropagationVariable(),
                                                     theRemainderTargetV.getPropagationVariable()));
      if (isZero)
        theNewSaxpy.useAsSax();
      isZero = false;
    }
  } 

  void
  Sequence::propagateToRemainderVertex_narySax(const RemainderGraphVertex& theRemainderTargetV) {
    const RemainderGraph& theRemainderGraph(getBestRemainderGraph());
    xaifBoosterDerivativePropagator::DerivativePropagatorSaxpy* theSax_p (NULL);
    // we will create a single SAX operation that encompasses the derivative components from all the inedges
    bool allPassive = true;
    RemainderGraph::ConstInEdgeIteratorPair inEdgeIP (theRemainderGraph.getInEdgesOf(theRemainderTargetV));
    for (RemainderGraph::ConstInEdgeIterator iei (inEdgeIP.first), ie_end (inEdgeIP.second);
         iei != ie_end; ++iei) {
      const RemainderGraphEdge& currentRGE(*iei);
      const RemainderGraphVertex& theRemainderSourceV(theRemainderGraph.getSourceOf(currentRGE));
      const xaifBoosterCrossCountryInterface::AccumulationGraphVertex& theAccVertex(currentRGE.getAccumulationGraphVertex());
      if (theAccVertex.getPartialDerivativeKind() == PartialDerivativeKind::PASSIVE)
        continue; // skip PASSIVE inedges
      allPassive = false;
      switch (theAccVertex.getPartialDerivativeKind()) {
      case PartialDerivativeKind::LINEAR_ONE: {
	//linearOneInEdges.push_back(&*iei);
	Constant theTempConstant (SymbolType::INTEGER_STYPE, false);
	theTempConstant.setId(1);
	theTempConstant.setint(1);
	if (theSax_p)
	  theSax_p->addAX(theTempConstant,
			  theRemainderSourceV.getPropagationVariable());
	else
	  theSax_p = &(myDerivativePropagator.addSaxpyToEntryPList(theTempConstant,
                                                                   theRemainderSourceV.getPropagationVariable(),
                                                                   theRemainderTargetV.getPropagationVariable()));
	break;
      }
      case PartialDerivativeKind::LINEAR_MINUS_ONE: {
	Constant theTempConstant (SymbolType::INTEGER_STYPE, false);
	theTempConstant.setId(1);
	theTempConstant.setint(-1);
	if (theSax_p)
	  theSax_p->addAX(theTempConstant,
			  theRemainderSourceV.getPropagationVariable());
	else
	  theSax_p = &(myDerivativePropagator.addSaxpyToEntryPList(theTempConstant,
                                                                   theRemainderSourceV.getPropagationVariable(),
                                                                   theRemainderTargetV.getPropagationVariable()));
	break;
      }
      case PartialDerivativeKind::LINEAR: {
	Constant theTempConstant (SymbolType::REAL_STYPE, false);
	theTempConstant.setId(1);
	theTempConstant.setdouble(theAccVertex.getValue());
	if (theSax_p)
	  theSax_p->addAX(theTempConstant,
			  theRemainderSourceV.getPropagationVariable());
	else
	  theSax_p = &(myDerivativePropagator.addSaxpyToEntryPList(theTempConstant,
                                                                   theRemainderSourceV.getPropagationVariable(),
                                                                   theRemainderTargetV.getPropagationVariable()));
	break;
      }
      case PartialDerivativeKind::NONLINEAR: {
	if (theSax_p)
	  theSax_p->addAX(theAccVertex.getLHSVariable(),
			  theRemainderSourceV.getPropagationVariable());
	else
	  theSax_p = &(myDerivativePropagator.addSaxpyToEntryPList(theAccVertex.getLHSVariable(),
                                                                   theRemainderSourceV.getPropagationVariable(),
                                                                   theRemainderTargetV.getPropagationVariable()));
	break;
      }
      default:
	THROW_LOGICEXCEPTION_MACRO("BasicBlockPreaccumulation::Sequence::propagateToRemainderVertex_narySax:"
				   << " invalid PDK (" << PartialDerivativeKind::toString(theAccVertex.getPartialDerivativeKind()) << ") for saxpy factor");
	break;
      } // end switch on PDK
    } // end for all inedges
    if (!theSax_p)
      THROW_LOGICEXCEPTION_MACRO("BasicBlockPreaccumulation::Sequence::propagateToRemainderVertex_narySax:"
                                 << " remainder vertex " << theRemainderTargetV.debug() << " has no inedges with non-passive partial derivative kind");
    // this will be the sole propagation entry for theRemainderTargetV
    theSax_p->useAsSax();
  }

}

