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
#include "xaifBooster/system/inc/VariableSymbolReference.hpp"

#include "xaifBooster/algorithms/InlinableXMLRepresentation/inc/InlinableSubroutineCall.hpp"

#include "xaifBooster/algorithms/TypeChange/inc/ConcreteArgumentAlg.hpp"
#include "xaifBooster/algorithms/TypeChange/inc/SymbolAlg.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulationTapeAdjoint/inc/SubroutineCallAlg.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulationTapeAdjoint/inc/BasicBlockAlg.hpp"

namespace xaifBoosterBasicBlockPreaccumulationTapeAdjoint {  

  SubroutineCallAlg::SubroutineCallAlg(const SubroutineCall& theContainingSubroutineCall) : 
    xaifBoosterTypeChange::SubroutineCallAlg(theContainingSubroutineCall),
    xaifBoosterBasicBlockPreaccumulationTape::SubroutineCallAlg(theContainingSubroutineCall),
    xaifBoosterBasicBlockPreaccumulationTapeAdjoint::BasicBlockElementAlg(theContainingSubroutineCall),
    myAdjointCounterpart_p(NULL),
    myOriginalCounterpart_p(NULL) {
  }

  SubroutineCallAlg::~SubroutineCallAlg() { 
    for (PlainBasicBlock::BasicBlockElementList::iterator aBasicBlockElementListI=
	   myPops.begin();
	 aBasicBlockElementListI!=myPops.end();
	 ++aBasicBlockElementListI) {
      if (*aBasicBlockElementListI)
	delete *aBasicBlockElementListI;
    }
  } 

  void
  SubroutineCallAlg::printXMLHierarchy(std::ostream& os) const { 
    for (PlainBasicBlock::BasicBlockElementList::const_iterator aBasicBlockElementListI
	   =myPops.begin();
	 aBasicBlockElementListI!=myPops.end();
	 ++aBasicBlockElementListI) {
      if (*aBasicBlockElementListI) { 
	(*aBasicBlockElementListI)->printXMLHierarchy(os);
      } 
    }
    // only print the adjustments (not the assignments)
    xaifBoosterTypeChange::SubroutineCallAlg::printXMLHierarchyImplWithAdjustments(os);
  } // end of BasicBlockAlg::printXMLHierarchy

  std::string 
  SubroutineCallAlg::debug() const { 
    std::ostringstream out;
    out << "xaifBoosterBasicBlockPreaccumulationTapeAdjoint::SubroutineCallAlg["
	<< this 
	<< ","
 	<< SubroutineCallAlgBase::debug().c_str()
	<< "]" << std::ends;  
    return out.str();
  }

  void SubroutineCallAlg::traverseToChildren(const GenericAction::GenericAction_E anAction_c) { 
  } 

  bool SubroutineCallAlg::isExternal() const {
    return (dynamic_cast<xaifBoosterTypeChange::SymbolAlg&>
    (getContainingSubroutineCall().
        getSymbolReference().getSymbol().getSymbolAlgBase())).isExternal();
  }

  void SubroutineCallAlg::insertYourself(const BasicBlock& theBasicBlock) { 
    // we don't do this for external calls: 
    if(isExternal())
      return;
    insertYourself(theBasicBlock, ForLoopReversalType::ANONYMOUS);
    insertYourself(theBasicBlock, ForLoopReversalType::EXPLICIT);
  } 

  void SubroutineCallAlg::insertYourself(const BasicBlock& theBasicBlock,
					 ForLoopReversalType::ForLoopReversalType_E aReversalType) { 
    // we don't do this for external calls: 
    if(isExternal())
      return;
    BasicBlockAlg& theBasicBlockAlg(dynamic_cast<BasicBlockAlg&>(theBasicBlock.getBasicBlockAlgBase()));
    SubroutineCall& theNewSubroutineCall(theBasicBlockAlg.addSubroutineCall(getContainingSubroutineCall().getSymbolReference().getSymbol(),
									    getContainingSubroutineCall().getSymbolReference().getScope(),
									    getContainingSubroutineCall().getActiveUse(),
									    aReversalType,
                                                                            getContainingSubroutineCall().getFormalArgCount()));
    const SubroutineCall::ConcreteArgumentPList& theOldConcreteArgumentPList(getContainingSubroutineCall().getConcreteArgumentPList());
    SubroutineCall::ConcreteArgumentPList& theNewConcreteArgumentPList(theNewSubroutineCall.getConcreteArgumentPList());
    for (SubroutineCall::ConcreteArgumentPList::const_iterator theOldConcreteArgumentPListI=theOldConcreteArgumentPList.begin();
	 theOldConcreteArgumentPListI!=theOldConcreteArgumentPList.end();
	 ++theOldConcreteArgumentPListI) { 
      ConcreteArgument* theNewConcreteArgument_p(new ConcreteArgument((*theOldConcreteArgumentPListI)->getPosition()));
      theNewConcreteArgumentPList.push_back(theNewConcreteArgument_p);
      (*theOldConcreteArgumentPListI)->copyMyselfInto(*theNewConcreteArgument_p);
    } // end for
    // reapply any argument changes we may need
    // but for the adjoint we can skip the copy calls
    SubroutineCallAlg& theNewSubroutineCallAlg(dynamic_cast<SubroutineCallAlg&>(theNewSubroutineCall.getSubroutineCallAlgBase()));
    theNewSubroutineCallAlg.myOriginalCounterpart_p=this;
    myAdjointCounterpart_p=&(theNewSubroutineCallAlg);
    theNewSubroutineCallAlg.replaceArguments(false);
    if (aReversalType==ForLoopReversalType::ANONYMOUS) {
      theNewSubroutineCallAlg.handleArrayAccessIndices(*this,aReversalType);
    }
  } 

  void SubroutineCallAlg::handleArrayAccessIndices(SubroutineCallAlg& orignalCallAlg,
                                                   ForLoopReversalType::ForLoopReversalType_E aReversalType) {
    // pop all the indices: 
    const Expression::VariablePVariableSRPPairList& theTypeChangePairs(orignalCallAlg.getVariablesPushed(aReversalType));
    for (Expression::VariablePVariableSRPPairList::const_reverse_iterator pairIt=theTypeChangePairs.rbegin();
	 pairIt!=theTypeChangePairs.rend();
	 ++pairIt) { 
      // make the subroutine call:    
      xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall* thePopCall_p;
      if ((*pairIt).first->getVariableSymbolReference().getSymbol().getSymbolType()==SymbolType::INTEGER_STYPE)
	thePopCall_p=new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall("pop_i_"+SymbolShape::toShortString((*pairIt).first->getEffectiveShape()));
      else if ((*pairIt).first->getVariableSymbolReference().getSymbol().getSymbolType()==SymbolType::REAL_STYPE)
	thePopCall_p=(new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall("pop_"+SymbolShape::toShortString((*pairIt).first->getEffectiveShape())));
      else
	THROW_LOGICEXCEPTION_MACRO("SubroutineCallAlg::handleArrayAccessIndices: no logic to pop things of type " 
				   << SymbolType::toString((*pairIt).first->getVariableSymbolReference().getSymbol().getSymbolType()).c_str()
				   << " and shape "
				   << SymbolShape::toString((*pairIt).first->getEffectiveShape()).c_str())

      myPops.push_back(thePopCall_p);
      thePopCall_p->setId("SubroutineCallAlg::handleArrayAccessIndices");
      (*pairIt).first->copyMyselfInto(thePopCall_p->addConcreteArgument(1).getArgument().getVariable());
    }
  }

  SubroutineCallAlg& SubroutineCallAlg::getAdjointCounterPart() {
    if (!myAdjointCounterpart_p) {
      THROW_LOGICEXCEPTION_MACRO("SubroutineCallAlg::getAdjointCounterPart: not set for " << debug().c_str());
    }
    return *myAdjointCounterpart_p;
  }

  SubroutineCallAlg& SubroutineCallAlg::getOriginalCounterPart() {
    if (!myOriginalCounterpart_p) {
      THROW_LOGICEXCEPTION_MACRO("SubroutineCallAlg::getOriginalCounterPart: not set for " << debug().c_str());
    }
    return *myOriginalCounterpart_p;
  }

}

