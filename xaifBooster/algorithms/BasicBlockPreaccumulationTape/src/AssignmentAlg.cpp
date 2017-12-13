// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/utils/inc/DbgLoggerManager.hpp"

#include "xaifBooster/system/inc/Assignment.hpp"
#include "xaifBooster/system/inc/BasicBlock.hpp"
#include "xaifBooster/system/inc/ArrayAccess.hpp"
#include "xaifBooster/system/inc/Assignment.hpp"
#include "xaifBooster/system/inc/VariableSymbolReference.hpp"
#include "xaifBooster/system/inc/CallGraph.hpp"
#include "xaifBooster/system/inc/ControlFlowGraph.hpp"
#include "xaifBooster/system/inc/ConceptuallyStaticInstances.hpp"

#include "xaifBooster/algorithms/InlinableXMLRepresentation/inc/InlinableSubroutineCall.hpp"

#include "xaifBooster/algorithms/TypeChange/inc/ConcreteArgumentAlg.hpp"
#include "xaifBooster/algorithms/TypeChange/inc/SymbolAlg.hpp"

#include "xaifBooster/algorithms/AdjointUtils/inc/BasicBlockPrintVersion.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulationTape/inc/AssignmentAlg.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulationTape/inc/BasicBlockAlg.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulationTape/inc/CallGraphVertexAlg.hpp"

#include "xaifBooster/algorithms/SaveValuesAcross/inc/SaveValuesAcross.hpp"

namespace xaifBoosterBasicBlockPreaccumulationTape {  

  AssignmentAlg::AssignmentAlg(Assignment& theContainingAssignment) : 
    xaifBoosterBasicBlockPreaccumulation::AssignmentAlg(theContainingAssignment),
    BasicBlockElementAlg(theContainingAssignment) { 
  }

  AssignmentAlg::~AssignmentAlg() { 
  }

  void
  AssignmentAlg::printXMLHierarchy(std::ostream& os) const {
    if (getContainingAssignment().isNonInlinable()) { 
      // the assignment
      getContainingAssignment().printXMLHierarchyImpl(os);
      const PushContainer& thePushContainer(myPushContainerMap.find(xaifBoosterAdjointUtils::BasicBlockPrintVersion::get())->second);
      for (std::list<const BasicBlockElement*>::const_iterator assignI = thePushContainer.myAssignmentsforPush.begin();
          assignI != thePushContainer.myAssignmentsforPush.end(); ++assignI)
        (*assignI)->printXMLHierarchy(os);
      for (PlainBasicBlock::BasicBlockElementList::const_iterator aBasicBlockElementListI = thePushContainer.myPostStatementPushList.begin();
          aBasicBlockElementListI != thePushContainer.myPostStatementPushList.end(); ++aBasicBlockElementListI)
        (*aBasicBlockElementListI)->printXMLHierarchy(os);
      const PushContainer& thePushContainer2(myPushContainerMap.find(ForLoopReversalType::HEURISTIC)->second);
      for (std::list<BasicBlockElement*>::const_iterator pushI = thePushContainer2.myPostStatementPushList.begin();
          pushI != thePushContainer2.myPostStatementPushList.end(); ++pushI)
        (*pushI)->printXMLHierarchy(os);
    }
    else 
      xaifBoosterBasicBlockPreaccumulation::AssignmentAlg::printXMLHierarchy(os);
  }
  
  std::string 
  AssignmentAlg::debug() const {
    std::ostringstream out;
    out << "xaifBoosterBasicBlockPreaccumulationTape::AssignmentAlg[" << xaifBoosterBasicBlockPreaccumulation::AssignmentAlg::debug().c_str()
                                                               << "," << BasicBlockElementAlg::debug().c_str()
        << "]" << std::ends;  
    return out.str();
  }

  void AssignmentAlg::traverseToChildren(const GenericAction::GenericAction_E anAction_c) { 
  } 

  void AssignmentAlg::checkAndPush(const Variable& theVariable) { 
    // has it been pushed already? 
    bool pushedAlready=false; 
    PushContainer& thePushContainer(myPushContainerMap[ForLoopReversalType::ANONYMOUS]);
    for (Expression::VariablePVariableSRPPairList::iterator it=thePushContainer.myVariablesPushed.begin();
        it!=thePushContainer.myVariablesPushed.end();
        ++it) {
      DBG_MACRO(DbgGroup::DATA, "comparing " << theVariable.debug().c_str() << " to " << ((*it).first)->debug().c_str()); 
      if (theVariable.equivalentTo(*((*it).first))) { 
        pushedAlready=true;
        break;
      }
    }
    if (!pushedAlready) { 
      // make the subroutine call:    
      xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall* theAssignment_p;
      if (theVariable.getVariableSymbolReference().getSymbol().getSymbolType()==SymbolType::INTEGER_STYPE)
        theAssignment_p=new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall("push_i_"+SymbolShape::toShortString(theVariable.getEffectiveShape()));
      else if (theVariable.getVariableSymbolReference().getSymbol().getSymbolType()==SymbolType::REAL_STYPE)
        theAssignment_p=(new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall("push_"+SymbolShape::toShortString(theVariable.getEffectiveShape())));
      else
        THROW_LOGICEXCEPTION_MACRO("AssignmentAlg::checkAndPush: no logic to push things of type "
            << SymbolType::toString(theVariable.getVariableSymbolReference().getSymbol().getSymbolType()).c_str()
            << " and shape "
            << SymbolShape::toString(theVariable.getEffectiveShape()).c_str())
            // save it in the list
            thePushContainer.myPostStatementPushList.push_back(theAssignment_p);
      theAssignment_p->setId("AssignmentAlg::checkAndPush");
      theVariable.copyMyselfInto(theAssignment_p->addConcreteArgument(1).getArgument().getVariable());
      thePushContainer.myVariablesPushed.push_back(Expression::VariablePVariableSRPPair(&theVariable,0));
    }
  }

  void AssignmentAlg::algorithm_action_4() { 
    DBG_MACRO(DbgGroup::CALLSTACK, "xaifBoosterBasicBlockPreaccumulationTape::AssignmentAlg::algorithm_action_4(tape array access in argument)");
    // we do this only for noninlinables:
    if(getContainingAssignment().isNonInlinable()) {
      Expression::CArgumentPList argList;
      getContainingAssignment().getRHS().appendArguments(argList);
      for (Expression::CArgumentPList::const_iterator lIt=argList.begin();
           lIt!=argList.end();
           ++lIt) {
        if ((*lIt).first->getVariable().hasArrayAccess())
	  handleArrayAccessIndices((*lIt).first->getVariable());
      }
    } 
  } 

  bool
  AssignmentAlg::hasExpression(const Expression& anExpression) const {
    // check myIndexPushes
    const PushContainer& thePushContainer(myPushContainerMap.find(ForLoopReversalType::ANONYMOUS)->second);
    for (PlainBasicBlock::BasicBlockElementList::const_iterator pushI = thePushContainer.myPostStatementPushList.begin();
         pushI != thePushContainer.myPostStatementPushList.end(); ++pushI)
      if ((*pushI)->hasExpression(anExpression))
        return true;
    // pass on to the typechange version of this routine, which will look through the list of saveacross values
    return xaifBoosterBasicBlockPreaccumulation::AssignmentAlg::hasExpression(anExpression);
  } // end AssignmentAlg::hasExpression()

  void AssignmentAlg::handleArrayAccessIndices(const Variable& theVariable) {
    // get the CallGraphVertexAlg and BasicBlock from the traversal stack
    CallGraphVertexAlg& theCallerCallGraphVertexAlg
      (dynamic_cast<CallGraphVertexAlg&>(ConceptuallyStaticInstances::instance()->getTraversalStack().getCurrentCallGraphVertexInstance().getCallGraphVertexAlgBase()));
    const BasicBlock& theCallerBasicBlock (dynamic_cast<const BasicBlock&>(ConceptuallyStaticInstances::instance()->getTraversalStack().getCurrentBasicBlockInstance()));

    const ArrayAccess::IndexTripletListType& theIndexTripletList(theVariable.getArrayAccess().getIndexTripletList());
    for (ArrayAccess::IndexTripletListType::const_iterator anIndexTripletListTypeCI=theIndexTripletList.begin();
	 anIndexTripletListTypeCI!=theIndexTripletList.end();
	 ++anIndexTripletListTypeCI) { 
      for (IndexTriplet::IndexPairList::const_iterator anIndexPairListCI=(*anIndexTripletListTypeCI)->getIndexPairList().begin();
	   anIndexPairListCI!=(*anIndexTripletListTypeCI)->getIndexPairList().end();
	   ++anIndexPairListCI) { 
	// now we have two cases, essentially the expression is a single vertex with a constant 
	// (this discounts constant expressions, this is a todo which might be dealt with later or 
	// it may be completly superceded by a TBR analysis)
	const Expression& theIndexExpression(*((*anIndexPairListCI).second));
	if (!theIndexExpression.isConstant()) {
	  Expression::CArgumentPList listToBeAppended;
	  theIndexExpression.appendArguments(listToBeAppended);
	  for (Expression::CArgumentPList::const_iterator argumentI=listToBeAppended.begin(); argumentI!=listToBeAppended.end(); ++argumentI) {
	    checkAndPush((*argumentI).first->getVariable());
	  }
          theCallerCallGraphVertexAlg.markRequiredValue(theIndexExpression,
                                                        theCallerBasicBlock,
                                                        "xaifBoosterBasicBlockPreaccumulationTape::AssignmentAlg::handleArrayAccessIndices");
	} // end if the index expression is non-const
      }
    }
  } // end AssignmentAlg::handleArrayAccessIndices()

} // end namespace xaifBoosterBasicBlockPreaccumulationTape

