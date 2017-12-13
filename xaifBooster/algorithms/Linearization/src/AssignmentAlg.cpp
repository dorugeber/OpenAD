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

#include "xaifBooster/utils/inc/DbgLoggerManager.hpp"

#include "xaifBooster/system/inc/Assignment.hpp"
#include "xaifBooster/system/inc/Argument.hpp"
#include "xaifBooster/system/inc/AliasMap.hpp"
#include "xaifBooster/system/inc/ConceptuallyStaticInstances.hpp"
#include "xaifBooster/system/inc/CallGraph.hpp"
#include "xaifBooster/system/inc/VariableSymbolReference.hpp"
#include "xaifBooster/system/inc/InlinableIntrinsicsExpression.hpp"
#include "xaifBooster/system/inc/InlinableIntrinsicsExpressionVertex.hpp"
#include "xaifBooster/system/inc/GraphVizDisplay.hpp"

#include "xaifBooster/algorithms/TypeChange/inc/TemporariesHelper.hpp"

#include "xaifBooster/algorithms/Linearization/inc/AssignmentAlg.hpp"
#include "xaifBooster/algorithms/Linearization/inc/ExpressionAlg.hpp"
#include "xaifBooster/algorithms/Linearization/inc/ExpressionEdgeAlg.hpp"
#include "xaifBooster/algorithms/Linearization/inc/ExpressionVertexAlg.hpp"

namespace xaifBoosterLinearization { 

  AssignmentAlg::AssignmentAlg(Assignment& theContainingAssignment) : 
    AssignmentAlgBase(theContainingAssignment),
    myHaveLinearizedRHSFlag(false),
    myActiveFlag(true), // note: delayed initialization
    myActiveFlagInit(false) {
  }

  std::string AssignmentAlg::debug() const { 
    std::ostringstream out;
    out << "xaifBoosterLinearization::AssignmentAlg[" << AssignmentAlgBase::debug().c_str()
        << ",myHaveLinearizedRHSFlag=" << myHaveLinearizedRHSFlag
        << ",myActiveFlag=" << myActiveFlag
	<< "]" << std::ends;  
    return out.str();
  } // end of AssignmentAlg::debug

  void
  AssignmentAlg::printXMLHierarchy(std::ostream& os) const { 
    DBG_MACRO(DbgGroup::CALLSTACK,"xaifBoosterLinearization::AssignmentAlg::printXMLHierarchy: entered for " << debug().c_str());
    // New print order
    //---------------------------------
    // There is some question as to how the aux and lin assignments should be ordered.
    //   if we can't print a partial assignment, then it must be because we are waiting on some aux assignment.
    //   We can think of it as greedy bottom-up (post-order) traversal, where
    //   partial assignments are printed as we move up the edge
    // - for a lowest-level subexpression that has an aux variable
    // - allocations (SSA and partial?)
    // - ExpressionAlg (which does auxiliary and partial assignments) inline
    // - the final assignment to be printed is the assignment to the LHS itself

    // we can either
    // (1) - do all of the aux assignments first (in an aux block) followed by all of the linearizations (in a partial eval block)
    // (2) - interleave the aux assignments with the partials (perhaps in a single traversal of the expression tree)
    //       even here we do have a small choice: if some EV has an aux (and thus an aux assignment) that
    //       isn't used for the partial expressions for any of the inedges, then do we print it before the linearization?

    // as of right now, it seems we prefer (1), because the aux assignments will always occur in the forward sweep in full,
    // whereas the partial expressions might be pushed to the adjoint (no pun intended)

    const ExpressionAlg& theRHSExpressionAlg(
     dynamic_cast<const ExpressionAlg&>(getContainingAssignment().getRHS().getExpressionAlgBase())
    );
    // print the extraction assignments to the aux vars and their allocations, bottom-up
    if (theRHSExpressionAlg.needsAuxiliaryExtraction())
      theRHSExpressionAlg.printAuxiliaryAssignmentsSSA(getContaining().getId(),
                                                       os);
                                                       
    // print the local partial assignments and their allocations, bottom-up
    if (hasLinearizedRHS()) // passivated assignments and noninlinable intrinsics, for example, have no local partial assignments
      theRHSExpressionAlg.printLocalPartialAssignments(os);
    // print the assignment itself
    if (theRHSExpressionAlg.needsAuxiliaryExtraction())
      printReplacementAssignmentSSA(os);
    else
      getContaining().printXMLHierarchyImpl(os);
  }

  void
  AssignmentAlg::printReplacementAssignmentSSA(std::ostream& os) const {
    Assignment theReplacementAssignment(false);
    theReplacementAssignment.setId(getContainingAssignment().getId()+": delayed LHS assignment for top level replacement");
    // the LHS is the same as the original Assignment
    getContainingAssignment().getLHS().copyMyselfInto(theReplacementAssignment.getLHS());
    // set the RHS
    const ExpressionVertex& theMaxEV(getContainingAssignment().getRHS().getMaxVertex());
    const ExpressionVertexAlg& theMaxEVAlg(dynamic_cast<const ExpressionVertexAlg&>(theMaxEV.getExpressionVertexAlgBase()));
    if (theMaxEVAlg.hasAuxiliaryVariable()) { // an aux assignment must have already been created
      Argument& theAuxiliaryArgument(*new Argument());
      theMaxEVAlg.copyAuxiliaryInto(theAuxiliaryArgument);
      theReplacementAssignment.getRHS().supplyAndAddVertexInstance(theAuxiliaryArgument);
    }
    else { // recursively construct RHS
      ExpressionVertex& theNewMaxEV(theMaxEV.createCopyOfMyself());
      theReplacementAssignment.getRHS().supplyAndAddVertexInstance(theNewMaxEV);
      const ExpressionAlg& theRHSExpressionAlg(dynamic_cast<const ExpressionAlg&>(getContainingAssignment().getRHS().getExpressionAlgBase()));
      Expression::ConstInEdgeIteratorPair iep(getContainingAssignment().getRHS().getInEdgesOf(theMaxEV));
      for (Expression::ConstInEdgeIterator aEEi(iep.first); aEEi != iep.second; ++aEEi)
        theRHSExpressionAlg.populateSSASubexpressionRecursively(*aEEi,
                                                                theReplacementAssignment.getRHS(),
                                                                theNewMaxEV);
    }
    theReplacementAssignment.printXMLHierarchy(os);
  }

  void AssignmentAlg::traverseToChildren(const GenericAction::GenericAction_E anAction_c) { 
  } 

  // local writer definition: 
  class VertexLabelWriter {
  public:
    VertexLabelWriter(const Expression& e) : myE(e) {};
    template <class BoostIntenalVertexDescriptor>
    void operator()(std::ostream& out, const BoostIntenalVertexDescriptor& v) const {
      ExpressionVertexAlg& va(dynamic_cast<ExpressionVertexAlg&>((*(boost::get(boost::get(BoostVertexContentType(),
											  myE.getInternalBoostGraph()),
									       v))).getExpressionVertexAlgBase()));
      out << "[label=\"" << va.isActive() << "\"]";
    }
    const Expression& myE;
  };

  void AssignmentAlg::algorithm_action_1() { 
    DBG_MACRO(DbgGroup::CALLSTACK, 
	      "xaifBoosterLinearization::AssignmentAlg::algorithm_action_1(analyze) called for: "
	      << debug().c_str());
    if (myHaveLinearizedRHSFlag)
      THROW_LOGICEXCEPTION_MACRO("xaifBoosterLinearization::AssignmentAlg::algorithm_action_1(analyze): cannot be called twice");
    if (!getContainingAssignment().getLHS().getActiveType()) { 
      //getContainingAssignment().getRHS().show("Inactive_RHS");
      // the LHS is an inactive type
      // passivate the entire statement.
      // note that the 'active'=true may just 
      // be the default set by the schema...
      myActiveFlag=false;
      myActiveFlagInit=true;
      // skip the rest
      return;
    }
    //getContainingAssignment().getRHS().show("AssignmentRHS_BeforeAnalysis");
    //GraphVizDisplay::show(getContainingAssignment().getRHS(),
    //                      "AssignmentRHS",
    //                      VertexLabelWriter(getContainingAssignment().getRHS()));
    // perform the activity analysis on the original right hand side
    if (!getContainingAssignment().isNonInlinable())
      dynamic_cast<ExpressionAlg&>(getContainingAssignment().getRHS().getExpressionAlgBase()).activityAnalysis();
    //getContainingAssignment().getRHS().show("AssignmentRHS_AfterAnalysis");
    //GraphVizDisplay::show(getContainingAssignment().getRHS(),
    //                      "originalAfter",
    //                      VertexLabelWriter(getContainingAssignment().getRHS()));

    // has the maximal node become passive?
    if (!dynamic_cast<ExpressionVertexAlg&>(getContainingAssignment().getRHS().getMaxVertex().getExpressionVertexAlgBase()).isActive()) { 
      // make the entire assignment passive
      myActiveFlag=false;
      myActiveFlagInit=true;
    }
    if (!myActiveFlag)
      return;

    if (getContainingAssignment().isNonInlinable())
      return;

    // set the flag
    myHaveLinearizedRHSFlag = true;
  } // end of AssignmentAlg::algorithm_action_1(analyze/copy)

  void AssignmentAlg::activityAnalysis() {
    if (!myActiveFlag)
      return;
    dynamic_cast<ExpressionAlg&>(getContainingAssignment().getRHS().getExpressionAlgBase()).activityAnalysis();
    // has the maximal node become passive?
    if (!dynamic_cast<ExpressionVertexAlg&>(getContainingAssignment().getRHS().getMaxVertex().getExpressionVertexAlgBase()).isActive()
	||
	(getContainingAssignment().getRHS().numVertices()==1 // we have only one vertex which must
	 // be an argument, otherwise it would be a constant and we would have hit the previous condition
	 // and not execute this one
	 &&
	 !dynamic_cast<const Argument&>(getContainingAssignment().getRHS().getMaxVertex()).getVariable().getActiveType())) {
      // make the entire assignment passive
      myActiveFlag=false;
      myActiveFlagInit=true;
      myHaveLinearizedRHSFlag = false;
    } // end if 
  } // end of AssignmentAlg::activityAnalysis

  void AssignmentAlg::algorithm_action_2() { 
    DBG_MACRO(DbgGroup::CALLSTACK, "xaifBoosterLinearization::AssignmentAlg::algorithm_action_2(code generation) called for: " << debug().c_str());
    if (!myActiveFlag
     || getContainingAssignment().isNonInlinable())
      return; // nothing to be done here
    // attach partial expressions
    dynamic_cast<ExpressionAlg&>(getContainingAssignment().getRHS().getExpressionAlgBase()).createPartialExpressions();
  } // end of AssignmentAlg::algorithm_action_2(code generation)

  const Expression& AssignmentAlg::getLinearizedRHS() const { 
    if (!myHaveLinearizedRHSFlag)
      THROW_LOGICEXCEPTION_MACRO("xaifBoosterLinearization::AssignmentAlg::getLinearizedRHS: this has not been linearized");
    return getContainingAssignment().getRHS(); //myLinearizedRightHandSide;
  }

  bool AssignmentAlg::hasLinearizedRHS() const { 
    return myHaveLinearizedRHSFlag;
  }

  bool AssignmentAlg::getActiveFlag() const { 
    if (!myActiveFlagInit) { 
      myActiveFlag=getContainingAssignment().getLHS().getActiveFlag();
      myActiveFlagInit=true;
    }
    return myActiveFlag;
  } 

}
