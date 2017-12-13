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

#include "xaifBooster/system/inc/SubroutineCall.hpp"
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

#include "xaifBooster/algorithms/BasicBlockPreaccumulationTape/inc/SubroutineCallAlg.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulationTape/inc/BasicBlockAlg.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulationTape/inc/CallGraphVertexAlg.hpp"

#include "xaifBooster/algorithms/SaveValuesAcross/inc/SaveValuesAcross.hpp"

namespace xaifBoosterBasicBlockPreaccumulationTape {  

  SubroutineCallAlg::SubroutineCallAlg(const SubroutineCall& theContainingSubroutineCall) : 
    xaifBoosterTypeChange::SubroutineCallAlg(theContainingSubroutineCall),
    BasicBlockElementAlg(theContainingSubroutineCall) { 
  }

  SubroutineCallAlg::~SubroutineCallAlg() { 
    for (std::list<const Expression*>::iterator expListI = myOnEntryFormalExpressionPList.begin();
         expListI != myOnEntryFormalExpressionPList.end(); ++expListI) {
      if (*expListI)
        delete *expListI;
    }
    for (std::list<const Expression*>::iterator expListI = myOnEntryNonFormalExpressionPList.begin();
         expListI != myOnEntryNonFormalExpressionPList.end(); ++expListI) {
      if (*expListI)
        delete *expListI;
    }
  }

  void
  SubroutineCallAlg::printXMLHierarchy(std::ostream& os) const { 
    // the call
    xaifBoosterTypeChange::SubroutineCallAlg::printXMLHierarchy(os);
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
  
  std::string 
  SubroutineCallAlg::debug() const { 
    std::ostringstream out;
    out << "xaifBoosterBasicBlockPreaccumulationTape::SubroutineCallAlg[" << this 
	<< "," << SubroutineCallAlgBase::debug().c_str()
	<< "]" << std::ends;  
    return out.str();
  }

  void SubroutineCallAlg::traverseToChildren(const GenericAction::GenericAction_E anAction_c) { 
  } 

  void SubroutineCallAlg::checkAndPush(const Variable& theVariable,
                                       ForLoopReversalType::ForLoopReversalType_E aReversalType) {
    // has it been pushed already? 
    bool pushedAlready=false; 
    PushContainer& aPushContainer(myPushContainerMap[aReversalType]);
    for (Expression::VariablePVariableSRPPairList::iterator it=aPushContainer.myVariablesPushed.begin();
        it!=aPushContainer.myVariablesPushed.end();
        ++it) {
      DBG_MACRO(DbgGroup::DATA, "for call to " << getContainingSubroutineCall().getSymbolReference().getSymbol().getId().c_str()  << " comparing " << theVariable.debug().c_str() << " to " << ((*it).first)->debug().c_str());
      if (theVariable.equivalentTo(*((*it).first))) { 
        pushedAlready=true;
        break;
      }
    }
    if (!pushedAlready) { 
      // make the subroutine call:    
      xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall* theSubroutineCall_p;
      if (theVariable.getVariableSymbolReference().getSymbol().getSymbolType()==SymbolType::INTEGER_STYPE)
        theSubroutineCall_p=new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall("push_i_"+SymbolShape::toShortString(theVariable.getEffectiveShape()));
      else if (theVariable.getVariableSymbolReference().getSymbol().getSymbolType()==SymbolType::REAL_STYPE)
        theSubroutineCall_p=(new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall("push_"+SymbolShape::toShortString(theVariable.getEffectiveShape())));
      else
        THROW_LOGICEXCEPTION_MACRO("SubroutineCallAlg::checkAndPush: no logic to push things of type "
            << SymbolType::toString(theVariable.getVariableSymbolReference().getSymbol().getSymbolType()).c_str()
            << " and shape "
            << SymbolShape::toString(theVariable.getEffectiveShape()).c_str())
            // save it in the list
            aPushContainer.myPostStatementPushList.push_back(theSubroutineCall_p);
      theSubroutineCall_p->setId("SubroutineCallAlg::checkAndPush");
      theVariable.copyMyselfInto(theSubroutineCall_p->addConcreteArgument(1).getArgument().getVariable());
      aPushContainer.myVariablesPushed.push_back(Expression::VariablePVariableSRPPair(&theVariable,0));
    }
  }

  void SubroutineCallAlg::algorithm_action_4() { 
    DBG_MACRO(DbgGroup::CALLSTACK, "xaifBoosterBasicBlockPreaccumulationTape::SubroutineCallAlg::algorithm_action_4(tape array access in argument)");
    xaifBoosterTypeChange::SymbolAlg& theSymbolAlg(dynamic_cast<xaifBoosterTypeChange::SymbolAlg&>
						   (getContainingSubroutineCall().getSymbolReference().getSymbol().getSymbolAlgBase()));
    // we don't do this for external calls: 
    if(!theSymbolAlg.isExternal()) {
      // for each subroutinecall argument
      // store the index variables via 
      // an InlinableSubroutinecall to push
      // note that here we don't use the fact that we are or are not in an explicit loop
      // NOTE: we actually DONT have to consider those values that have already been saved by
      // the conversion caused by TypeChange, because they're for passive arguments exclusively.

      CallGraphVertexAlg& theCallerCallGraphVertexAlg
	(dynamic_cast<CallGraphVertexAlg&>(ConceptuallyStaticInstances::instance()->getTraversalStack().getCurrentCallGraphVertexInstance().getCallGraphVertexAlgBase()));
      const BasicBlock& theCallerBasicBlock (dynamic_cast<const BasicBlock&>(ConceptuallyStaticInstances::instance()->getTraversalStack().getCurrentBasicBlockInstance()));

      // mark array access indices for all arguments that are
      // (1) arguments
      // (2) active
      // (3) array accesses
      for (SubroutineCall::ConcreteArgumentPList::const_iterator aConcreteArgumentPListI = getContainingSubroutineCall().getConcreteArgumentPList().begin();
           aConcreteArgumentPListI != getContainingSubroutineCall().getConcreteArgumentPList().end();
           ++aConcreteArgumentPListI) {
        DBG_MACRO(DbgGroup::DATA, "xaifBoosterBasicBlockPreaccumulationTape::SubroutineCallAlg::algorithm_action_4 on argument " << (*aConcreteArgumentPListI)->getArgument().getVariable().getVariableSymbolReference().getSymbol().getId().c_str() << " for " << getContainingSubroutineCall().getSymbolReference().getSymbol().getId().c_str() << " which is " << (*aConcreteArgumentPListI)->debug().c_str());
        if ((*aConcreteArgumentPListI)->isArgument()
            &&
            (*aConcreteArgumentPListI)->getArgument().getVariable().hasArrayAccess()) {
          handleArrayAccessIndices(**aConcreteArgumentPListI); 
        }
      } // end for
      // now check if any values are required on entry: 
      const ControlFlowGraph& theCalleeCFG (ConceptuallyStaticInstances::instance()->getCallGraph().getSubroutineBySymbolReference(getContainingSubroutineCall().getSymbolReference()).getControlFlowGraph());
      const SideEffectList& theOnEntryList(theCalleeCFG.getSideEffectList(SideEffectListType::ON_ENTRY_LIST));
      for (SideEffectList::VariablePList::const_iterator i = theOnEntryList.getVariablePList().begin(); i != theOnEntryList.getVariablePList().end(); ++i) {
        Expression* theDummyExpression_p (NULL); // dummy expression for marking as required
        std::string theOriginStr;
	ControlFlowGraph::FormalResult theResult(theCalleeCFG.hasFormal((*i)->getVariableSymbolReference()));
	if (theResult.first) { // is a formal
	  const ConcreteArgument& theConcreteArgument(getContainingSubroutineCall().getConcreteArgument(theResult.second));
	  if (theConcreteArgument.isConstant())
	    continue; 
	  checkAndPush(theConcreteArgument.getArgument().getVariable(), ForLoopReversalType::ANONYMOUS);
          theDummyExpression_p = new Expression (false);
          Argument* theNewArgument_p = new Argument();
          theConcreteArgument.getArgument().getVariable().copyMyselfInto(theNewArgument_p->getVariable());
          theNewArgument_p->setId(1);
          theDummyExpression_p->supplyAndAddVertexInstance(*theNewArgument_p);
          myOnEntryFormalExpressionPList.push_back(theDummyExpression_p);
          theOriginStr = "BasicBlockPreaccumulationTape::SubroutineCallAlg::algorithm_action_4: on entry formal";
	}
	else { // not a formal
	  checkAndPush(**i, ForLoopReversalType::ANONYMOUS);
          // make a dummy expression for marking as required. The sole ExpressionVertex is an argument
          theDummyExpression_p = new Expression (false);
          Argument* theNewArgument_p = new Argument();
          (*i)->copyMyselfInto(theNewArgument_p->getVariable());
          theNewArgument_p->setId(1);
          theDummyExpression_p->supplyAndAddVertexInstance(*theNewArgument_p);
          myOnEntryNonFormalExpressionPList.push_back(theDummyExpression_p);
          theOriginStr = "BasicBlockPreaccumulationTape::SubroutineCallAlg::algorithm_action_4: on entry non formal";
        } // end if/else formal
        theCallerCallGraphVertexAlg.markRequiredValue(*theDummyExpression_p,
                                                      theCallerBasicBlock,
                                                      theOriginStr);
      } // end iterate over theOnEntryList
    } // end if not external
  } // end SubroutineCallAlg::algorithm_action_4()

  bool
  SubroutineCallAlg::hasExpression(const Expression& anExpression) const {
    // check on entry formals
    for (std::list<const Expression*>::const_iterator expPListI = myOnEntryFormalExpressionPList.begin();
         expPListI != myOnEntryFormalExpressionPList.end(); ++expPListI)
      if ((*expPListI)->hasExpression(anExpression))
        return true;
    // check on entry non-formals
    for (std::list<const Expression*>::const_iterator expPListI = myOnEntryNonFormalExpressionPList.begin();
         expPListI != myOnEntryNonFormalExpressionPList.end(); ++expPListI)
      if ((*expPListI)->hasExpression(anExpression))
        return true;
    const PushContainer& thePushContainer(myPushContainerMap.find(ForLoopReversalType::ANONYMOUS)->second);
    for (PlainBasicBlock::BasicBlockElementList::const_iterator pushI = thePushContainer.myPostStatementPushList.begin();
         pushI != thePushContainer.myPostStatementPushList.end(); ++pushI)
      if ((*pushI)->hasExpression(anExpression))
        return true;
    // pass on to the typechange version of this routine, which will look through the list of saved-across values
    return xaifBoosterTypeChange::SubroutineCallAlg::hasExpression(anExpression);
  } // end SubroutineCallAlg::hasExpression()

  void SubroutineCallAlg::handleArrayAccessIndices(ConcreteArgument& theConcreteArgument) {
    // get the CallGraphVertexAlg and BasicBlock from the traversal stack
    DBG_MACRO(DbgGroup::DATA, "xaifBoosterBasicBlockPreaccumulationTape::SubroutineCallAlg::handleArrayAccessIndices on argument " << theConcreteArgument.getArgument().getVariable().getVariableSymbolReference().getSymbol().getId().c_str() << " for " << getContainingSubroutineCall().getSymbolReference().getSymbol().getId().c_str());
    CallGraphVertexAlg& theCallerCallGraphVertexAlg
      (dynamic_cast<CallGraphVertexAlg&>(ConceptuallyStaticInstances::instance()->getTraversalStack().getCurrentCallGraphVertexInstance().getCallGraphVertexAlgBase()));
    const BasicBlock& theCallerBasicBlock (dynamic_cast<const BasicBlock&>(ConceptuallyStaticInstances::instance()->getTraversalStack().getCurrentBasicBlockInstance()));

    const ArrayAccess::IndexTripletListType& theIndexTripletList(theConcreteArgument.getArgument().getVariable().getArrayAccess().getIndexTripletList());
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
	    checkAndPush((*argumentI).first->getVariable(), ForLoopReversalType::ANONYMOUS);
	  }
          theCallerCallGraphVertexAlg.markRequiredValue(theIndexExpression,
                                                        theCallerBasicBlock,
                                                        "xaifBoosterBasicBlockPreaccumulationTape::SubroutineCallAlg::handleArrayAccessIndices");
	} // end if the index expression is non-const
      }
    }
  } // end SubroutineCallAlg::handleArrayAccessIndices()

} // end namespace xaifBoosterBasicBlockPreaccumulationTape

