// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/utils/inc/PrintManager.hpp"

#include "xaifBooster/system/inc/BasicBlock.hpp"
#include "xaifBooster/system/inc/Assignment.hpp"
#include "xaifBooster/system/inc/Scope.hpp"
#include "xaifBooster/system/inc/SubroutineCall.hpp"
#include "xaifBooster/system/inc/VariableSymbolReference.hpp"
#include "xaifBooster/system/inc/ArrayAccess.hpp"
#include "xaifBooster/system/inc/Argument.hpp"
#include "xaifBooster/system/inc/CallGraph.hpp"

#include "xaifBooster/algorithms/InlinableXMLRepresentation/inc/InlinableSubroutineCall.hpp"

#include "xaifBooster/algorithms/TypeChange/inc/TemporariesHelper.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulationTapeAdjoint/inc/AdjointAssembly.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulationTapeAdjoint/inc/BasicBlockElementAlg.hpp"

using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulationTapeAdjoint { 

  xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall& 
  AdjointAssembly::addInlinableSubroutineCall(const std::string& aSubroutineName,
					      const ForLoopReversalType::ForLoopReversalType_E& aReversalType) { 
    xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall* aNewCall_p(new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall(aSubroutineName));
    getBasicBlockElementList(aReversalType).push_back(aNewCall_p);
    return *aNewCall_p;									     
  } 

  AdjointAssembly::~AdjointAssembly() { 
    for (PlainBasicBlock::BasicBlockElementList::const_iterator li=myBasicBlockElementListAnonymousReversal.begin();
         li!=myBasicBlockElementListAnonymousReversal.end();
         li++) { 
      if (*li)
	delete *li;
    } 
    for (PlainBasicBlock::BasicBlockElementList::const_iterator li=myBasicBlockElementListExplicitReversal.begin();
         li!=myBasicBlockElementListExplicitReversal.end();
         li++) { 
      if (*li)
	delete *li;
    } 
  }
  
  SubroutineCall& 
  AdjointAssembly::addSubroutineCall(const Symbol& aSubroutineNameSymbol,
				     const Scope& aSubroutineNameScope,
				     ActiveUseType::ActiveUseType_E anActiveUse,
				     const ForLoopReversalType::ForLoopReversalType_E& aReversalType,
				     unsigned short formalArgCount) { 
    SubroutineCall* aNewCall_p(new SubroutineCall(aSubroutineNameSymbol,
						  aSubroutineNameScope,
						  anActiveUse,
                                                  formalArgCount, 
						  true));
    aNewCall_p->setId("reverse_call");
    getBasicBlockElementList(aReversalType).push_back(aNewCall_p);
    return *aNewCall_p;									     
  } 

  Assignment& 
  AdjointAssembly::addAssignment(const ForLoopReversalType::ForLoopReversalType_E& aReversalType,bool withAlgorithm) {
    Assignment* aNewAssignment_p(new Assignment(withAlgorithm));
    aNewAssignment_p->setId("reverse_assignment");
    getBasicBlockElementList(aReversalType).push_back(aNewAssignment_p);
    return *aNewAssignment_p;									     
  } 
  
  void AdjointAssembly::reinterpretArrayAccess(const Variable& theOriginalVariable,
					       Variable& theNewVariable,
					       const ForLoopReversalType::ForLoopReversalType_E& aReversalType) { 
    DBG_MACRO(DbgGroup::CALLSTACK, "xaifBoosterBasicBlockPreaccumulationTapeAdjoint::AdjointAssembly::reinterpretArrayAccess");
    theOriginalVariable.copyMyselfInto(theNewVariable);
    if (!theOriginalVariable.hasArrayAccess() || aReversalType==ForLoopReversalType::EXPLICIT)
      return;
    // otherwise we will replace index expressions: 
    ArrayAccess::IndexTripletListType& theNewIndexTripletList(theNewVariable.getArrayAccess().getIndexTripletList());
    const ArrayAccess::IndexTripletListType& theOriginalIndexTripletList(theOriginalVariable.getArrayAccess().getIndexTripletList());
    ArrayAccess::IndexTripletListType::reverse_iterator aNewIndexTripletListTypeI=theNewIndexTripletList.rbegin() ;
    for (ArrayAccess::IndexTripletListType::const_reverse_iterator anOriginalIndexTripletListTypeCI=theOriginalIndexTripletList.rbegin();
	 anOriginalIndexTripletListTypeCI!=theOriginalIndexTripletList.rend();
	 ++anOriginalIndexTripletListTypeCI,
	   ++aNewIndexTripletListTypeI) { 
      for (IndexTriplet::IndexPairList::const_iterator anIndexPairListCI=(*anOriginalIndexTripletListTypeCI)->getIndexPairList().begin();
	   anIndexPairListCI!=(*anOriginalIndexTripletListTypeCI)->getIndexPairList().end();
	   ++anIndexPairListCI) { 
	// now we have two cases, essentially the expression is a single vertex with a constant 
	// (this discounts constant expressions, this is a todo which might be dealt with later or 
	// it may be completelt superceded by a TBR analysis)
	const Expression& theIndexExpression (*((*anIndexPairListCI).second));
        if (!theIndexExpression.isConstant()) {
	  // it is a variable or expression whose value we pushed and now want to pop and replace.
	  // First clear the old index expression that we copied into aNewIndexTripletListTypeI
	  (*aNewIndexTripletListTypeI)->getExpression((*anIndexPairListCI).first).clear();

          const Variable* thePoppedAddressVariable_p = NULL;
          if (theIndexExpression.numVertices() == 1) {
            // In this case, the address variable has already been popped, and so we must just find the variable it has been popped into
            const Variable& theAddressVariable (dynamic_cast<const Argument&>(theIndexExpression.getMaxVertex()).getVariable());
            // find the popped factor variable from the correlation list
            DBG_MACRO(DbgGroup::DATA,"BasicBlockPreaccumulationTapeAdjoint::AdjointAssembly::reinterpretArrayAccess: "
                                     << "Trying to find " << theAddressVariable.debug() << ":");
            VariablePPairList::const_iterator avCorI;
            for (avCorI = myAddressVariableCorList.begin(); avCorI != myAddressVariableCorList.end(); ++avCorI) {
              DBG_MACRO(DbgGroup::DATA,"BasicBlockPreaccumulationTapeAdjoint::AdjointAssembly::reinterpretArrayAccess: "
                                       << "      trying " << (*avCorI).first->debug());
              if (theAddressVariable.equivalentTo(*(*avCorI).first))
                break;
            }
            if (avCorI == myAddressVariableCorList.end())
              THROW_LOGICEXCEPTION_MACRO("xaifBoosterBasicBlockPreaccumulationTapeAdjoint::AdjointAssembly::reinterpretArrayAccess: "
                                         << "could not correlate pushed address variable with popped address variable");
            thePoppedAddressVariable_p = (*avCorI).second;
          } // end if expression has only 1 vertex
          else { // more than one vertex in index expression => an assignment was created in taping phase and only the LHS was pushed
	    xaifBoosterTypeChange::TemporariesHelper aTemporariesHelper("xaifBoosterBasicBlockPreaccumulationTapeAdjoint::AdjointAssembly::reinterpretArrayAccess",
									theIndexExpression,theIndexExpression.getMaxVertex());
            thePoppedAddressVariable_p = &addAddressPop(aReversalType,aTemporariesHelper);
          } // end if >1 vertex

          // create a copy of the variable in the indexExpression: 
          Argument& theNewArgument (*new Argument);
          // relinquish ownership to the index expression: 
          (*aNewIndexTripletListTypeI)->getExpression((*anIndexPairListCI).first).supplyAndAddVertexInstance(theNewArgument);
          theNewArgument.setId(1);
          thePoppedAddressVariable_p->copyMyselfInto(theNewArgument.getVariable());
        } // end if index expression is non-constant
      } // end for all index pairs
    } // end for all index triplets
  } // end AdjointAssembly::reinterpretArrayAccess()

  void AdjointAssembly::addZeroDeriv(const Variable& theTarget,
				     const ForLoopReversalType::ForLoopReversalType_E& aReversalType) { 
    xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall& theSubroutineCall(addInlinableSubroutineCall("ZeroDeriv",
														 aReversalType));
    theSubroutineCall.setId("inline_zeroderiv");
    theTarget.copyMyselfInto(theSubroutineCall.addConcreteArgument(1).getArgument().getVariable());
  } 

  void AdjointAssembly::addUnitFactor(const Variable& theSource,
				      const Variable& theTarget,
				      const ForLoopReversalType::ForLoopReversalType_E& aReversalType) { 
    xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall& theSetDerivCall(addInlinableSubroutineCall("IncDeriv",
                                                                                                               aReversalType));
    theSetDerivCall.setId("inline_IncDeriv");
    theTarget.copyMyselfInto(theSetDerivCall.addConcreteArgument(1).getArgument().getVariable());
    theSource.copyMyselfInto(theSetDerivCall.addConcreteArgument(2).getArgument().getVariable());
  } // end AdjointAssembly::addUnitFactor()

  void AdjointAssembly::addNegativeUnitFactor(Variable& theSource,
					      Variable& theTarget,
					      const ForLoopReversalType::ForLoopReversalType_E& aReversalType) { 
    xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall& theDecDerivCall (addInlinableSubroutineCall("DecDeriv",
                                                                                                                aReversalType));
    theDecDerivCall.setId("inline_DecDeriv");
    theTarget.copyMyselfInto(theDecDerivCall.addConcreteArgument(1).getArgument().getVariable());
    theSource.copyMyselfInto(theDecDerivCall.addConcreteArgument(2).getArgument().getVariable());
  } // end AdjointAssembly::addNegativeUnitFactor()

  const Variable& AdjointAssembly::addFactorPop(const Symbol& aTemporarySymbol,
						const ForLoopReversalType::ForLoopReversalType_E& aReversalType) { 
    xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall& theFactorPopCall (addInlinableSubroutineCall("pop_"+SymbolShape::toShortString(aTemporarySymbol.getSymbolShape()),
                                                                                                                 aReversalType));
    theFactorPopCall.setId("inline_pop");
    Variable& theInlineVariable(theFactorPopCall.addConcreteArgument(1).getArgument().getVariable());
    // give it a name etc.
    // create a new symbol and add a new VariableSymbolReference in the Variable
    VariableSymbolReference* theNewVariableSymbolReference_p=
      new VariableSymbolReference(aTemporarySymbol,
				  getScope());
    theNewVariableSymbolReference_p->setId("1");
    theNewVariableSymbolReference_p->setAnnotation("xaifBoosterBasicBlockPreaccumulationTapeAdjoint::AdjointAssembly::addFactorPop");
    // pass it on to the variable and relinquish ownership
    theInlineVariable.supplyAndAddVertexInstance(*theNewVariableSymbolReference_p);
    theInlineVariable.getAliasMapKey().setTemporary();
    theInlineVariable.getDuUdMapKey().setTemporary();
    return theInlineVariable;
  } 

  const Variable& AdjointAssembly::addAddressPop(const ForLoopReversalType::ForLoopReversalType_E& aReversalType,
						 xaifBoosterTypeChange::TemporariesHelper& aTemporariesHelper) {
    // pop the value
    xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall& 
      theAddressPopCall (addInlinableSubroutineCall("pop_i_"+SymbolShape::toShortString(aTemporariesHelper.getSymbolShape()),
						    aReversalType));
    theAddressPopCall.setId("inline_pop_i");
    Variable& thePoppedAddressVariable (theAddressPopCall.addConcreteArgument(1).getArgument().getVariable());
    // give it a name etc.
    // create a new symbol and add a new VariableSymbolReference in the Variable
    VariableSymbolReference* theNewVariableSymbolReference_p =
      new VariableSymbolReference(aTemporariesHelper.makeTempSymbol(getScope()),
                                  getScope());
    theNewVariableSymbolReference_p->setId("1");
    theNewVariableSymbolReference_p->setAnnotation("xaifBoosterBasicBlockPreaccumulationTapeAdjoint::AdjointAssembly::addAddressPop");
    // pass it on to the variable and relinquish ownership
    thePoppedAddressVariable.supplyAndAddVertexInstance(*theNewVariableSymbolReference_p);
    thePoppedAddressVariable.getAliasMapKey().setTemporary();
    thePoppedAddressVariable.getDuUdMapKey().setTemporary();
    return thePoppedAddressVariable;
  } // end AdjointAssembly::addAddressPop()

  void AdjointAssembly::addAllocation(const Symbol& aTemporarySymbol,
				      const Scope& theScope,
				      xaifBoosterTypeChange::TemporariesHelper & aHelper,
				      const ForLoopReversalType::ForLoopReversalType_E& aReversalType) { 
    std::list<Variable*> thePoppedVariablePList;
    // get shape from tape
    SymbolShape::SymbolShape_E theShape=aTemporarySymbol.getSymbolShape();
    for(unsigned short dim=theShape;dim>=1;--dim) {
      xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall& theShapePopCall (addInlinableSubroutineCall("pop_i_"+SymbolShape::toShortString(SymbolShape::SCALAR),
														    aReversalType));
      theShapePopCall.setId("xaifBoosterBasicBlockPreaccumulationTapeAdjoint::AdjointAssembly::addAllocation");
      Variable& thePoppedShapeVariable (theShapePopCall.addConcreteArgument(1).getArgument().getVariable());
      // give it a name etc.
      // create a new symbol and add a new VariableSymbolReference in the Variable
      VariableSymbolReference* theNewVariableSymbolReference_p =
	new VariableSymbolReference(getScope().getSymbolTable().addUniqueAuxSymbol(SymbolKind::VARIABLE,
										   SymbolType::INTEGER_STYPE,
										   SymbolShape::SCALAR,
										   false),
				    getScope());
      theNewVariableSymbolReference_p->setId("1");
      theNewVariableSymbolReference_p->setAnnotation("xaifBoosterBasicBlockPreaccumulationTapeAdjoint::AdjointAssembly::addAllocation");
      // pass it on to the variable and relinquish ownership
      thePoppedShapeVariable.supplyAndAddVertexInstance(*theNewVariableSymbolReference_p);
      thePoppedShapeVariable.getAliasMapKey().setTemporary();
      thePoppedShapeVariable.getDuUdMapKey().setTemporary();
      // note that thePoppedShapeVariable is a reference to the argument in the pop call
      thePoppedVariablePList.push_front(&thePoppedShapeVariable);
    }
    xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall& theSRCall(addInlinableSubroutineCall("oad_AllocateShape",
													 aReversalType)); 
    theSRCall.setId("xaifBoosterBasicBlockPreaccumulationTapeAdjoint::AdjointAssembly::addAllocation");
    // first argument
    Variable& toBeAllocatedVariable(theSRCall.addConcreteArgument(1).getArgument().getVariable());
    VariableSymbolReference* 
      tobeAllocateVariableSymbolReference_p(new VariableSymbolReference(aTemporarySymbol,
									theScope));
    tobeAllocateVariableSymbolReference_p->setId("1");
    tobeAllocateVariableSymbolReference_p->setAnnotation("xaifBoosterBasicBlockPreaccumulation::Sequence::addAllocation");
    toBeAllocatedVariable.supplyAndAddVertexInstance(*tobeAllocateVariableSymbolReference_p);
    toBeAllocatedVariable.getAliasMapKey().setTemporary();
    toBeAllocatedVariable.getDuUdMapKey().setTemporary();
    // put the shape arguments as 2nd and following into the allocation call
    int argPos=2;
    for (std::list<Variable*>::iterator it=thePoppedVariablePList.begin();
	 it!=thePoppedVariablePList.end();
	 ++it,++argPos) { 
      (*it)->copyMyselfInto(theSRCall.addConcreteArgument(argPos).getArgument().getVariable());
    }
  } 

  void AdjointAssembly::addSaxpy(const Variable& theSource,
				 const Variable& theTarget,
				 const Variable& theFactor,
				 const ForLoopReversalType::ForLoopReversalType_E& aReversalType) { 
    xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall& theSaxpyCall(addInlinableSubroutineCall("Saxpy",
													    aReversalType));
    theSaxpyCall.setId("inline_saxpy");
    theFactor.copyMyselfInto(theSaxpyCall.addConcreteArgument(1).getArgument().getVariable());
    theTarget.copyMyselfInto(theSaxpyCall.addConcreteArgument(2).getArgument().getVariable());
    theSource.copyMyselfInto(theSaxpyCall.addConcreteArgument(3).getArgument().getVariable());
  }

  void AdjointAssembly::addSaxpy(const Variable& theSource,
				 const Variable& theTarget,
				 const Constant& theFactor,
				 const ForLoopReversalType::ForLoopReversalType_E& aReversalType) { 
    xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall& theSaxpyCall (addInlinableSubroutineCall("Saxpy",
													     aReversalType));
    theSaxpyCall.setId("inline_saxpy");
    ConcreteArgument& theNewArgument (theSaxpyCall.addConcreteArgument(1));
    theNewArgument.makeConstant(theFactor.getType());
    theNewArgument.getConstant().setFromString(theFactor.toString());
    theTarget.copyMyselfInto(theSaxpyCall.addConcreteArgument(2).getArgument().getVariable());
    theSource.copyMyselfInto(theSaxpyCall.addConcreteArgument(3).getArgument().getVariable());
  }

  void AdjointAssembly::reinterpretDerivativePropagatorEntry(const xaifBoosterDerivativePropagator::DerivativePropagatorEntry& aDerivativePropagatorEntry) { 
    DBG_MACRO(DbgGroup::CALLSTACK, "xaifBoosterBasicBlockPreaccumulationTapeAdjoint::AdjointAssembly::reinterpretDerivativePropagatorEntry");
    // retrieve stored index values if needed starting with the target: 
    // take care of target address if needed: 
    const Variable& theOriginalTarget(aDerivativePropagatorEntry.getTarget());
    // this is either the original target
    // or it is the replaced target in case of ArrayAccesses
    Variable theActualTargetAnonymous,theActualTargetExplicit;
    reinterpretArrayAccess(theOriginalTarget,theActualTargetAnonymous,ForLoopReversalType::ANONYMOUS); 
    reinterpretArrayAccess(theOriginalTarget,theActualTargetExplicit,ForLoopReversalType::EXPLICIT); 
    xaifBoosterDerivativePropagator::DerivativePropagatorEntry::FactorList aFactorList;
    aDerivativePropagatorEntry.getFactors(aFactorList);
    for (xaifBoosterDerivativePropagator::DerivativePropagatorEntry::FactorList::reverse_iterator aFactorListI=aFactorList.rbegin();
	 aFactorListI!=aFactorList.rend();
	 ++aFactorListI) {
      // ZeroDerivs don't really have a factor and thus no source
      if ((*aFactorListI).getKind() != xaifBoosterDerivativePropagator::DerivativePropagatorEntry::Factor::ZERO_FACTOR) {
	const Variable& theOriginalSource((*aFactorListI).getSource());
	// this is either the original source
	// or it is the replaced target in case of ArrayAccesses
	Variable theActualSourceAnonymous,theActualSourceExplicit;
	reinterpretArrayAccess(theOriginalSource,theActualSourceAnonymous,ForLoopReversalType::ANONYMOUS); 
	reinterpretArrayAccess(theOriginalSource,theActualSourceExplicit,ForLoopReversalType::EXPLICIT); 
	// deal with the other cases: 
	switch((*aFactorListI).getKind()) {
        // SetDeriv or IncDeriv in tangent-linear mode
	case xaifBoosterDerivativePropagator::DerivativePropagatorEntry::Factor::UNIT_FACTOR: {
	    addUnitFactor(theActualSourceAnonymous,
                          theActualTargetAnonymous,
                          ForLoopReversalType::ANONYMOUS);
	    addUnitFactor(theActualSourceExplicit,
                          theActualTargetExplicit,
                          ForLoopReversalType::EXPLICIT);
	    break; 
	} 
        // SetNegDeriv or DecDeriv in tangent-linear mode
	case xaifBoosterDerivativePropagator::DerivativePropagatorEntry::Factor::NEGATIVE_UNIT_FACTOR: {
	    addNegativeUnitFactor(theActualSourceAnonymous,
                                  theActualTargetAnonymous,
                                  ForLoopReversalType::ANONYMOUS);
	    addNegativeUnitFactor(theActualSourceExplicit,
                                  theActualTargetExplicit,
                                  ForLoopReversalType::EXPLICIT);
	    break; 
	}
	case xaifBoosterDerivativePropagator::DerivativePropagatorEntry::Factor::CONSTANT_FACTOR: {
	  /**
	   * The inlinable call saxpy(a,x,y) is supposed to do y.d=y.d+a*x.d which in reverse means
	   * we have to switch arguments properly
	   */
          addSaxpy(theActualSourceAnonymous,
                   theActualTargetAnonymous,
                   (*aFactorListI).getConstant(),
                   ForLoopReversalType::ANONYMOUS);
          addSaxpy(theActualSourceExplicit,
                   theActualTargetExplicit,
                   (*aFactorListI).getConstant(),
                   ForLoopReversalType::EXPLICIT);
	  break;
	} // end case CONSTANT_FACTOR
	case xaifBoosterDerivativePropagator::DerivativePropagatorEntry::Factor::VARIABLE_FACTOR: // both represent some saxpy or sax 
	  {
            // find the popped factor variable from the correlation list
            VariablePPairList::const_iterator fvCorI;
            for (fvCorI = myFactorVariableCorList.begin(); fvCorI != myFactorVariableCorList.end(); ++fvCorI)
              if ((*aFactorListI).getVariable().equivalentTo(*(*fvCorI).first))
                break;
            if (fvCorI == myFactorVariableCorList.end())
              THROW_LOGICEXCEPTION_MACRO("xaifBoosterBasicBlockPreaccumulationTapeAdjoint::AdjointAssembly::reinterpretDerivativePropagatorEntry: "
                                         << "could not correlate pushed factor variable with popped factor variable");
            const Variable& thePoppedFactorVariable (*(*fvCorI).second);
	    /** 
	     * The inlinable call saxpy(a,x,y) is supposed to do y.d=y.d+a*x.d which in reverse means
	     * we have to switch arguments properly
	     */
	    addSaxpy(theActualSourceAnonymous,
		     theActualTargetAnonymous,
		     thePoppedFactorVariable,
		     ForLoopReversalType::ANONYMOUS);
	    addSaxpy(theActualSourceExplicit,
		     theActualTargetExplicit,
		     thePoppedFactorVariable,
		     ForLoopReversalType::EXPLICIT);
	    break; 
	  } 
	default: 
	  THROW_LOGICEXCEPTION_MACRO("AdjointAssembly::reinterpretDerivativePropagatorEntry: cannot handle factor kind " 
				     << (*aFactorListI).getKind()); 
	  break; 
	} // end switch 
      } // end if NOT ZeroDeriv
      // we follow all non-incremental propagations with a ZeroDeriv
      if (!aDerivativePropagatorEntry.isIncremental()) {
        addZeroDeriv(theActualTargetAnonymous,ForLoopReversalType::ANONYMOUS);
        addZeroDeriv(theActualTargetExplicit,ForLoopReversalType::EXPLICIT);
      } // end if non-incremental
    } // end for FactorList
  }

  const PlainBasicBlock::BasicBlockElementList& 
  AdjointAssembly::getBasicBlockElementList(const ForLoopReversalType::ForLoopReversalType_E& aReversalType) const { 
    switch(aReversalType) { 
    case ForLoopReversalType::ANONYMOUS : 
      return myBasicBlockElementListAnonymousReversal;
      break;
    case ForLoopReversalType::EXPLICIT : 
      return myBasicBlockElementListExplicitReversal;
      break;
    default: 
      THROW_LOGICEXCEPTION_MACRO("AdjointAssembly::getBasicBlockElementList: unknown reversal type "
				 << ForLoopReversalType::toString(aReversalType).c_str());
      break;
    }      
    // to make the compiler happy:
    return myBasicBlockElementListAnonymousReversal;
  }  

  PlainBasicBlock::BasicBlockElementList& 
  AdjointAssembly::getBasicBlockElementList(const ForLoopReversalType::ForLoopReversalType_E& aReversalType) { 
    switch(aReversalType) { 
    case ForLoopReversalType::ANONYMOUS : 
      return myBasicBlockElementListAnonymousReversal;
      break;
    case ForLoopReversalType::EXPLICIT : 
      return myBasicBlockElementListExplicitReversal;
      break;
    default: 
      THROW_LOGICEXCEPTION_MACRO("AdjointAssembly::getBasicBlockElementList: unknown reversal type "
				 << ForLoopReversalType::toString(aReversalType).c_str());
      break;
    }      
    // to make the compiler happy:
    return myBasicBlockElementListAnonymousReversal;
  }  

} // end namespace xaifBoosterBasicBlockPreaccumulationTapeAdjoint

