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

#include "xaifBooster/system/inc/Argument.hpp"
#include "xaifBooster/system/inc/ConceptuallyStaticInstances.hpp"
#include "xaifBooster/system/inc/Expression.hpp"
#include "xaifBooster/system/inc/ExpressionVertex.hpp"
#include "xaifBooster/system/inc/GraphVizDisplay.hpp"
#include "xaifBooster/system/inc/PartialDerivativeKind.hpp"
#include "xaifBooster/system/inc/TraversalStack.hpp"
#include "xaifBooster/system/inc/VariableSymbolReference.hpp"

#include "xaifBooster/algorithms/Linearization/inc/ExpressionEdgeAlg.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/PrivateLinearizedComputationalGraph.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/RemainderGraph.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/RemainderGraphWriters.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulationTape/inc/AssignmentAlg.hpp"

#include "xaifBooster/algorithms/CrossCountryInterface/inc/AccumulationGraph.hpp"
#include "xaifBooster/algorithms/CrossCountryInterface/inc/JacobianAccumulationExpressionVertex.hpp"

#include "xaifBooster/algorithms/RequiredValues/inc/RequiredValue.hpp"

#include "xaifBooster/algorithms/PushPop/inc/Sequence.hpp"

using namespace xaifBooster;

namespace xaifBoosterPushPop {

  // \TODO: is this needed?
  Sequence::Sequence() : xaifBoosterBasicBlockPreaccumulationTape::Sequence() {
  }

  Sequence::~Sequence() {
    // delete all the push calls in the push block
    for (CBasicBlockElementPList::const_iterator pushBlockI = myPushBlock.begin();
         pushBlockI != myPushBlock.end(); ++pushBlockI)
      if (*pushBlockI)
        delete *pushBlockI;
  }

  std::string
  Sequence::debug() const {
    std::ostringstream out;    
    out << "xaifBoosterPushPop::Sequence[" << xaifBoosterBasicBlockPreaccumulationTape::Sequence::debug()
        << ",myCombinedGraph=" << myCombinedGraph.debug()
        << "]" << std::ends;
    return out.str();
  }

  bool
  Sequence::hasExpression(const Expression& anExpression) const {
    return xaifBoosterBasicBlockPreaccumulationTape::Sequence::hasExpression(anExpression);
  }

  xaifBoosterRequiredValues::RequiredValueSet::ComparisonResult_E
  Sequence::compareExpressions(const Expression& firstExpression,
                               const Expression& secondExpression) const {
    DBG_MACRO(DbgGroup::CALLSTACK, "xaifBoosterPushPop::Sequence::compareExpressions");
    if (!hasExpression(firstExpression))
      THROW_LOGICEXCEPTION_MACRO("xaifBoosterPushPop::Sequence::compareExpressions: we don't have " << firstExpression.debug());
    if (!hasExpression(secondExpression))
      THROW_LOGICEXCEPTION_MACRO("xaifBoosterPushPop::Sequence::compareExpressions: we don't have " << secondExpression.debug());
    // check the assignments themselves
    for (CAssignmentPList::const_iterator ai(getAssignmentPList().begin()); ai != getAssignmentPList().end(); ++ai) {
      const xaifBoosterBasicBlockPreaccumulationTape::AssignmentAlg& theAssignmentAlg(
       dynamic_cast<const xaifBoosterBasicBlockPreaccumulationTape::AssignmentAlg&>(
        (*ai)->getAssignmentAlgBase()
       )
      );
      // check the original assignments
      bool foundFirst = theAssignmentAlg.hasExpression(firstExpression);
      bool foundSecond = theAssignmentAlg.hasExpression(secondExpression);
      if (foundFirst && foundSecond) return xaifBoosterRequiredValues::RequiredValueSet::EQUAL;
      else if (foundFirst) return xaifBoosterRequiredValues::RequiredValueSet::LESSTHAN;
      else if (foundSecond) return xaifBoosterRequiredValues::RequiredValueSet::GREATERTHAN;
    }

    // check the stuff that comes after the sequence
    // (for now we can consider it to all occur at the same time)
    // meaning that everything in accumulation and propagation is considered to occur simultaneously
    // for the same reason, we can just do all the pushes at the end
    return xaifBoosterRequiredValues::RequiredValueSet::EQUAL;
  }

  void
  Sequence::assignAndPushRequiredValueAfter(const xaifBoosterRequiredValues::RequiredValue& aRequiredValue) {
    DBG_MACRO(DbgGroup::CALLSTACK, "xaifBoosterPushPop::Sequence::assignAndPushRequiredValueAfter");
    const BasicBlock& theCurrentBasicBlock(ConceptuallyStaticInstances::instance()->getTraversalStack().getCurrentBasicBlockInstance());
    Assignment* theNewExpressionAssignment_p (new Assignment(false));
    theNewExpressionAssignment_p->setId("index_expression_assignment_for_taping");
    // create a new symbol and add a new VariableSymbolReference in the Variable
    //ConceptuallyStaticInstances::instance()->getTapingVariableNameCreator(),
    VariableSymbolReference* theNewVariableSymbolReference_p
      (new VariableSymbolReference(theCurrentBasicBlock.getScope().getSymbolTable().addUniqueAuxSymbol(SymbolKind::VARIABLE,
                                                                                                       SymbolType::INTEGER_STYPE,
                                                                                                       SymbolShape::SCALAR,
                                                                                                       false),
                                   theCurrentBasicBlock.getScope())
      );
    theNewVariableSymbolReference_p->setId("1");
    theNewVariableSymbolReference_p->setAnnotation("xaifBoosterPushPop::Sequence::assignAndPushRequiredValueAfter");
    // pass it on to the LHS and relinquish ownership
    theNewExpressionAssignment_p->getLHS().supplyAndAddVertexInstance(*theNewVariableSymbolReference_p);
    theNewExpressionAssignment_p->getLHS().getAliasMapKey().setTemporary();
    theNewExpressionAssignment_p->getLHS().getDuUdMapKey().setTemporary();
    // set the RHS
    aRequiredValue.getExpression().copyMyselfInto(theNewExpressionAssignment_p->getRHS(),
                                                  false,
                                                  false);
    myPushBlock.push_back(theNewExpressionAssignment_p);
    // now create the push
    xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall* theNewPushSubroutineCall_p
     (new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall("push_i_"+SymbolShape::toShortString(theNewExpressionAssignment_p->getLHS().getEffectiveShape())));
    theNewPushSubroutineCall_p->setId("xaifBoosterPushPop::Sequence::assignAndPushRequiredValueAfter:inline_push_i");
    theNewExpressionAssignment_p->getLHS().copyMyselfInto(theNewPushSubroutineCall_p->addConcreteArgument(1).getArgument().getVariable());
    myPushBlock.push_back(theNewPushSubroutineCall_p);
  }

  void
  Sequence::pushRequiredValueAfter(const xaifBoosterRequiredValues::RequiredValue& aRequiredValue) {
    DBG_MACRO(DbgGroup::CALLSTACK,"xaifBoosterPushPop::Sequence::pushRequiredValueAfter");
    if (!aRequiredValue.isArgument())
      THROW_LOGICEXCEPTION_MACRO("xaifBoosterPushPop::Sequence::pushRequiredValueAfter:"
                                 << " required value " << aRequiredValue.debug() << " is not an argument (it's some expression)");
    xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall* theNewPushSubroutineCall_p
      (new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall("push_i_"+SymbolShape::toShortString(aRequiredValue.getArgument().getVariable().getEffectiveShape())));
    theNewPushSubroutineCall_p->setId("xaifBoosterPushPop::Sequence::pushRequiredValueAfter:inline_push_i");
    aRequiredValue.getArgument().getVariable().copyMyselfInto(theNewPushSubroutineCall_p->addConcreteArgument(1).getArgument().getVariable());
    myPushBlock.push_back(theNewPushSubroutineCall_p);
  }

  void
  Sequence::populateCombinedGraph() {
    myCombinedGraph.clear();

    const xaifBoosterBasicBlockPreaccumulation::PrivateLinearizedComputationalGraph& theOriginalPLCG(getLCG());
    // copy the PLCG vertices
    xaifBoosterBasicBlockPreaccumulation::PrivateLinearizedComputationalGraph::ConstVertexIteratorPair aPLCGVpair(theOriginalPLCG.vertices());
    for (xaifBoosterBasicBlockPreaccumulation::PrivateLinearizedComputationalGraph::ConstVertexIterator aPLCGVi(aPLCGVpair.first); aPLCGVi != aPLCGVpair.second; ++aPLCGVi) {
      const xaifBoosterBasicBlockPreaccumulation::PrivateLinearizedComputationalGraphVertex& theCurrentPLCGV(
        dynamic_cast<const xaifBoosterBasicBlockPreaccumulation::PrivateLinearizedComputationalGraphVertex&>(*aPLCGVi)
      );
      CombinedGraphVertex* theNewCGV_p = new CombinedGraphVertex();
      theNewCGV_p->setComputationalGraphVertex(theCurrentPLCGV);
      myCombinedGraph.supplyAndAddVertexInstance(*theNewCGV_p);
      myPLCGVp2CGVpMap[&theCurrentPLCGV] = theNewCGV_p;
      // map each associated expressionvertex to the PLCGV (for handling the linearization expressions)
      const CExpressionVertexPSet& theAssociatedExpressionVertexPSet(theCurrentPLCGV.getAssociatedExpressionVertexPSet());
      for (CExpressionVertexPSet::const_iterator evI = theAssociatedExpressionVertexPSet.begin(); evI != theAssociatedExpressionVertexPSet.end(); ++evI)
        myEVp2CGVpMap[*evI] = theNewCGV_p;
    }

    // local map to correlate results of linearization expressions with AccumulationGraph inputs
    CExpressionEdgeP2CombinedGraphVertexPMap theEEp2CGVpMap;
    // add PLCG edges
    xaifBoosterBasicBlockPreaccumulation::PrivateLinearizedComputationalGraph::ConstEdgeIteratorPair aPLCGEpair(theOriginalPLCG.edges());
    for (xaifBoosterBasicBlockPreaccumulation::PrivateLinearizedComputationalGraph::ConstEdgeIterator aPLCGEi(aPLCGEpair.first), aPLCGEi_end(aPLCGEpair.second);
         aPLCGEi != aPLCGEi_end;
         ++aPLCGEi) {
      const xaifBoosterBasicBlockPreaccumulation::PrivateLinearizedComputationalGraphEdge& theCurrentPLCGE(
       dynamic_cast<const xaifBoosterBasicBlockPreaccumulation::PrivateLinearizedComputationalGraphEdge&>(*aPLCGEi)
      );
      // add the corresp. edge to the combined graph
      myCombinedGraph.supplyAndAddEdgeInstance(*(new CombinedGraphEdge(theCurrentPLCGE)),
                                               *myPLCGVp2CGVpMap[dynamic_cast<const xaifBoosterBasicBlockPreaccumulation::PrivateLinearizedComputationalGraphVertex*>(&theOriginalPLCG.getSourceOf(*aPLCGEi))],
                                               *myPLCGVp2CGVpMap[dynamic_cast<const xaifBoosterBasicBlockPreaccumulation::PrivateLinearizedComputationalGraphVertex*>(&theOriginalPLCG.getTargetOf(*aPLCGEi))]);
      // affix the linearization expression graph(s)
      if (theCurrentPLCGE.hasLinearizedExpressionEdge()) {
        theEEp2CGVpMap[&theCurrentPLCGE.getLinearizedExpressionEdge()] = &affixLinearizationExpressionToCombinedGraph(theCurrentPLCGE.getLinearizedExpressionEdge());
        // handle any parallel expression edges
        if (theCurrentPLCGE.getParallels().size()) {
          for (xaifBoosterBasicBlockPreaccumulation::PrivateLinearizedComputationalGraphEdge::ExpressionEdgePList::const_iterator parallelI = theCurrentPLCGE.getParallels().begin();
               parallelI != theCurrentPLCGE.getParallels().end(); ++parallelI) {
            theEEp2CGVpMap[*parallelI] = &affixLinearizationExpressionToCombinedGraph(**parallelI);
          }
        }
      } // end if there is a linearized expression edge associated with this PLCGE
      else if (!theCurrentPLCGE.isDirectCopyEdge()) {
        THROW_LOGICEXCEPTION_MACRO("xaifBoosterPushPop::Sequence::populateCombinedGraph:"
                                   << "theCurrentPLCGE " << theCurrentPLCGE.debug().c_str()
                                   << " has no LinearizedExpressionEdge and is not a DirectCopyEdge");
      }
    }

    // include the AccumulationGraph vertices
    const xaifBoosterCrossCountryInterface::AccumulationGraph& theAccumulationGraph(getBestElimination().getAccumulationGraph());
    xaifBoosterCrossCountryInterface::AccumulationGraph::ConstVertexIteratorPair aAGVpair(theAccumulationGraph.vertices());
    for (xaifBoosterCrossCountryInterface::AccumulationGraph::ConstVertexIterator aAGVi(aAGVpair.first); aAGVi != aAGVpair.second; ++aAGVi) {
      const xaifBoosterCrossCountryInterface::AccumulationGraphVertex& theCurrentAGV(*aAGVi);
      CombinedGraphVertex* theCorrespCGV_p; // either new or pre-existing
      if (theAccumulationGraph.numInEdgesOf(theCurrentAGV) == 0) { // if it has no children, then it's a local partial
        if (theCurrentAGV.hasExpressionEdge()) {
          // in this case, a vertex should already exist (linearization expression vertex/ PLCG edge)
          const ExpressionEdge& theLinearizedExpressionEdge(theCurrentAGV.getExpressionEdge());
          CExpressionEdgeP2CombinedGraphVertexPMap::const_iterator EEp2CGVpMapI = theEEp2CGVpMap.find(&theLinearizedExpressionEdge);
          if (EEp2CGVpMapI == theEEp2CGVpMap.end())
            THROW_LOGICEXCEPTION_MACRO("xaifBoosterPushPop::Sequence::populateCombinedGraph: could not find CGV corresp. to " << theLinearizedExpressionEdge.debug().c_str());
          theCorrespCGV_p = theEEp2CGVpMap[&theLinearizedExpressionEdge];
        }
        else { // should ONLY happen in cases where this AGV corresp. to a direct copy edge (riiight??)
          //THROW_LOGICEXCEPTION_MACRO("xaifBoosterPushPop::Sequence::populateCombinedGraph: no ExpressionEdge for " << theCurrentAGV.debug().c_str());
          theCorrespCGV_p = new CombinedGraphVertex();
          myCombinedGraph.supplyAndAddVertexInstance(*theCorrespCGV_p);
        }
      }
      else {// if it has children, then it's either an ADD or MULT vertex
        theCorrespCGV_p = new CombinedGraphVertex();
        myCombinedGraph.supplyAndAddVertexInstance(*theCorrespCGV_p);
      }
      myAGVp2CGVpMap[&theCurrentAGV] = theCorrespCGV_p;
      theCorrespCGV_p->associateAccumulationGraphVertex(theCurrentAGV);
    } // end iterate over AccumulationGraphVertices
    // incorporate AccumulationGraph edges
    xaifBoosterCrossCountryInterface::AccumulationGraph::ConstEdgeIteratorPair aAGEpair(theAccumulationGraph.edges());
    for (xaifBoosterCrossCountryInterface::AccumulationGraph::ConstEdgeIterator aAGEi(aAGEpair.first); aAGEi != aAGEpair.second; ++aAGEi) {
      const xaifBoosterCrossCountryInterface::AccumulationGraphVertex
       &theAccSource(theAccumulationGraph.getSourceOf(*aAGEi)),
       &theAccTarget(theAccumulationGraph.getTargetOf(*aAGEi));
      const CombinedGraphVertex& theCGSource(*myAGVp2CGVpMap[&theAccSource]);
      const CombinedGraphVertex& theCGTarget(*myAGVp2CGVpMap[&theAccTarget]);
      myCombinedGraph.supplyAndAddEdgeInstance(*(new CombinedGraphEdge(*aAGEi)),
                                               theCGSource,
                                               theCGTarget);
    }

    const xaifBoosterBasicBlockPreaccumulation::RemainderGraph& theRemainderGraph(getBestRemainderGraph());
    // RemainderGraph vertices
    xaifBoosterBasicBlockPreaccumulation::RemainderGraph::ConstVertexIteratorPair aRGVpair(theRemainderGraph.vertices());
    for (xaifBoosterBasicBlockPreaccumulation::RemainderGraph::ConstVertexIterator aRGVi(aRGVpair.first); aRGVi != aRGVpair.second; ++aRGVi) {
      const xaifBoosterBasicBlockPreaccumulation::RemainderGraphVertex& currentRemainderGraphVertex(*aRGVi);
      CombinedGraphVertex& correspCGV(*myPLCGVp2CGVpMap[&currentRemainderGraphVertex.getOriginalVertex()]);
      correspCGV.setRemainderGraphVertex(currentRemainderGraphVertex);
      myRGVp2CGVpMap[&currentRemainderGraphVertex] = &correspCGV;
    }
    // RemainderGraph edges
    xaifBoosterBasicBlockPreaccumulation::RemainderGraph::ConstEdgeIteratorPair aRGEpair(theRemainderGraph.edges());
    for (xaifBoosterBasicBlockPreaccumulation::RemainderGraph::ConstEdgeIterator aRGEi(aRGEpair.first); aRGEi != aRGEpair.second; ++aRGEi) {
      const xaifBoosterBasicBlockPreaccumulation::RemainderGraphEdge& currentRGEdge(*aRGEi);
      CombinedGraphVertex& correspCGV(*myAGVp2CGVpMap[&currentRGEdge.getAccumulationGraphVertex()]);
      correspCGV.associateRemainderGraphEdge(currentRGEdge);
      myRGEp2CGVpMap[&currentRGEdge] = &correspCGV;
    }

    if (DbgLoggerManager::instance()->isSelected(DbgGroup::GRAPHICS) && DbgLoggerManager::instance()->wantTag("cg")) {
      std::stringstream aCombinedGraphName;
      aCombinedGraphName << "CombinedGraph" << &myCombinedGraph;
      GraphVizDisplay::show(myCombinedGraph,
                            aCombinedGraphName.str(),
                            CombinedGraphVertexLabelWriter(myCombinedGraph),
                            CombinedGraphEdgeLabelWriter(myCombinedGraph),
                            CombinedGraphPropertiesWriter(myCombinedGraph));
    }

  }

  CombinedGraphVertex&
  Sequence::affixLinearizationExpressionToCombinedGraph(const ExpressionEdge& aLinearizedExpressionEdge) {
    // the CGV to be returned (corresp. to the maximal partial ExpressionVertex)
    // cannot be const because we set the LHS variable in this part
    CombinedGraphVertex* theReturnedMaximalCGVertex_p(NULL);

    const xaifBoosterLinearization::ExpressionEdgeAlg& theLinearizedExpressionEdgeAlg(
     dynamic_cast<const xaifBoosterLinearization::ExpressionEdgeAlg&>(aLinearizedExpressionEdge.getExpressionEdgeAlgBase())
    );
    if (!theLinearizedExpressionEdgeAlg.hasPartial())
      THROW_LOGICEXCEPTION_MACRO("xaifBoosterPushPop::Sequence::affixLinearizationExpressionToCombinedGraph: no abstract partial expression is set for " << theLinearizedExpressionEdgeAlg.debug().c_str());
    if (theLinearizedExpressionEdgeAlg.hasConcretePartialAssignment()) {
      const Expression& theConcretePartialRHSExpression(theLinearizedExpressionEdgeAlg.getConcretePartialAssignment().getRHS());
    //GraphVizDisplay::show(theConcretePartialRHSExpression,"theConcretePartialRHSExpression");
      // temporary local map for reconstructing the partial assignment expression
      CExpressionVertexP2CombinedGraphVertexPMap aEVp2CGVpMap;
      // copy the partial expression vertices
      Expression::ConstVertexIteratorPair aEVpair(theConcretePartialRHSExpression.vertices());
      for (Expression::ConstVertexIterator aEVi(aEVpair.first); aEVi != aEVpair.second; ++aEVi) {
        const ExpressionVertex& aPartialExpressionVertex(*aEVi);
        CombinedGraphVertex* theCorrespCGV_p(NULL);
        if (aPartialExpressionVertex.isArgument()) {// the expression vertex is an argument, and corresponds to an existing PLCGV
          const ExpressionVertex& theOriginalExpressionVertex(
           theLinearizedExpressionEdgeAlg.getOriginalExpressionVertex4ConcretePartialArgument(aPartialExpressionVertex)
          );
          CExpressionVertexP2CombinedGraphVertexPMap::const_iterator mapI = myEVp2CGVpMap.find(&theOriginalExpressionVertex);
          // does it corresp. to an existing PLCG vertex?
          if (mapI != myEVp2CGVpMap.end()) {
            theCorrespCGV_p = mapI->second;
          }
          else {
            theCorrespCGV_p = new CombinedGraphVertex();
            myCombinedGraph.supplyAndAddVertexInstance(*theCorrespCGV_p);
          }
        } // end if aPartialExpressionVertex is an argument
        else {
          // if it's not an argument, then it's an intrinsic or a constant,
          // so we need to make a new vertex
          theCorrespCGV_p = new CombinedGraphVertex();
          myCombinedGraph.supplyAndAddVertexInstance(*theCorrespCGV_p);
        }
        aEVp2CGVpMap[&aPartialExpressionVertex] = theCorrespCGV_p; // store the association in the local map
        theCorrespCGV_p->associateLinearizationExpressionVertex(aPartialExpressionVertex);
      } // end iterate over partial assignment expression vertices

      // add the edges from the linearization assignment expression
      Expression::ConstEdgeIteratorPair aEEpair(theConcretePartialRHSExpression.edges());
      for (Expression::ConstEdgeIterator aEEi(aEEpair.first); aEEi != aEEpair.second; ++aEEi)
        myCombinedGraph.supplyAndAddEdgeInstance(*(new CombinedGraphEdge(*aEEi)),
                                                 *aEVp2CGVpMap[&theConcretePartialRHSExpression.getSourceOf(*aEEi)],
                                                 *aEVp2CGVpMap[&theConcretePartialRHSExpression.getTargetOf(*aEEi)]);
      theReturnedMaximalCGVertex_p = aEVp2CGVpMap[&theConcretePartialRHSExpression.getMaxVertex()];
      theReturnedMaximalCGVertex_p->addLHSVariable(theLinearizedExpressionEdgeAlg.getConcretePartialAssignment().getLHS());
    } // end if there is a concrete partial assignment
    else {
      switch (theLinearizedExpressionEdgeAlg.getPartialDerivativeKind()) {
        case PartialDerivativeKind::LINEAR_ONE:
        case PartialDerivativeKind::LINEAR_MINUS_ONE: {
          CombinedGraphVertex* theNewCGV_p = new CombinedGraphVertex();
          myCombinedGraph.supplyAndAddVertexInstance(*theNewCGV_p);
          theNewCGV_p->setLinearizedExpressionEdgeAlg(theLinearizedExpressionEdgeAlg);
          theReturnedMaximalCGVertex_p = theNewCGV_p;
          break;
        }
        case PartialDerivativeKind::LINEAR: {
          CombinedGraphVertex* theNewConcreteConstantCGV_p = new CombinedGraphVertex();
          myCombinedGraph.supplyAndAddVertexInstance(*theNewConcreteConstantCGV_p);
          theNewConcreteConstantCGV_p->associateLinearizationExpressionVertex(theLinearizedExpressionEdgeAlg.getConcreteConstant());
          theReturnedMaximalCGVertex_p = theNewConcreteConstantCGV_p;
          break;
        }
        case PartialDerivativeKind::NONLINEAR:
          THROW_LOGICEXCEPTION_MACRO("xaifBoosterPushPop::Sequence::affixLinearizationExpressionToCombinedGraph: "
                                       << "theLinearizedExpressionEdgeAlg has PartialDerivativeKind::NONLINEAR yet no ConcretePartialAssignment: " << theLinearizedExpressionEdgeAlg.debug().c_str());
          break;
        case PartialDerivativeKind::NOT_SET:
        case PartialDerivativeKind::PASSIVE:
        default:
          THROW_LOGICEXCEPTION_MACRO("xaifBoosterPushPop::Sequence::affixLinearizationExpressionToCombinedGraph:"
                                     << "invalid PDK for theLinearizedExpressionEdgeAlg " << theLinearizedExpressionEdgeAlg.debug().c_str());
          break;
      } // end switch PDK
    }
    if (!theReturnedMaximalCGVertex_p)
      THROW_LOGICEXCEPTION_MACRO("xaifBoosterPushPop::Sequence::affixLinearizationExpressionToCombinedGraph:"
                                 << "could not determine which CombinedGraphVertex corresp. to maximal expression vertex for " << theLinearizedExpressionEdgeAlg.debug().c_str());
    return *theReturnedMaximalCGVertex_p;
  }

}
 
