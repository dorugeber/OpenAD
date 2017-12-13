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

#include "xaifBooster/system/inc/Argument.hpp"
#include "xaifBooster/system/inc/ArrayAccess.hpp"
#include "xaifBooster/system/inc/Assignment.hpp"
#include "xaifBooster/system/inc/ConceptuallyStaticInstances.hpp"
#include "xaifBooster/system/inc/Scope.hpp"
#include "xaifBooster/system/inc/TraversalStack.hpp"
#include "xaifBooster/system/inc/Variable.hpp"
#include "xaifBooster/system/inc/VariableSymbolReference.hpp"

#include "xaifBooster/algorithms/InlinableXMLRepresentation/inc/InlinableSubroutineCall.hpp"

#include "xaifBooster/algorithms/TypeChange/inc/SymbolAlg.hpp"
#include "xaifBooster/algorithms/TypeChange/inc/TemporariesHelper.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulationTape/inc/CallGraphVertexAlg.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulationTape/inc/Sequence.hpp"

using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulationTape {

  // \TODO: is this needed?
  Sequence::Sequence() : xaifBoosterBasicBlockPreaccumulation::Sequence() {
  }

  Sequence::~Sequence() {
  }

  std::string
  Sequence::debug() const {
    std::ostringstream out;
    out << "xaifBoosterBasicBlockPreaccumulationTape::Sequence[" << xaifBoosterBasicBlockPreaccumulation::Sequence::debug().c_str()
        << ",myPushedAddressVariablesPList.size():" << myPushedAddressVariablesPList.size()
        << ",myPushedFactorVariablesPList.size():" << myPushedFactorVariablesPList.size()
        << ",myReinterpretedDerivativePropagator=" << &myReinterpretedDerivativePropagator
	<< "]" << std::ends;  
    return out.str();
  }

  bool
  Sequence::hasExpression(const Expression& anExpression) const {
    if (myReinterpretedDerivativePropagator.hasExpression(anExpression))
      return true;
    return xaifBoosterBasicBlockPreaccumulation::Sequence::hasExpression(anExpression);
  }

  const CVariablePList&
  Sequence::getPushedAddressVariablesPList() const {
    return myPushedAddressVariablesPList;
  }

  const CVariablePList&
  Sequence::getPushedFactorVariablesPList() const {
    return myPushedFactorVariablesPList;
  }

  const ReinterpretedDerivativePropagator&
  Sequence::getReinterpretedDerivativePropagator() const {
    return myReinterpretedDerivativePropagator;
  }

  void
  Sequence::reinterpretPropagationsAsTapings() {
    // create a push for each (unique) factor variable and save it in the list
    const xaifBoosterDerivativePropagator::DerivativePropagator::EntryPList& theEntryPList (myDerivativePropagator.getEntryPList());
    for (xaifBoosterDerivativePropagator::DerivativePropagator::EntryPList::const_iterator entryPListI = theEntryPList.begin();
         entryPListI != theEntryPList.end(); ++entryPListI) {
      xaifBoosterDerivativePropagator::DerivativePropagatorEntry::FactorList aFactorList;
      (*entryPListI)->getFactors(aFactorList);
      for (xaifBoosterDerivativePropagator::DerivativePropagatorEntry::FactorList::iterator aFactorListI = aFactorList.begin();
           aFactorListI != aFactorList.end(); ++aFactorListI) {
        if ((*aFactorListI).getKind()==xaifBoosterDerivativePropagator::DerivativePropagatorEntry::Factor::VARIABLE_FACTOR) {
          const Variable& theFactorVariable ((*aFactorListI).getVariable());
          // check whether this factor variable has already been pushed
          CVariablePList::const_iterator pushedFacVarPI;
          for (pushedFacVarPI = myPushedFactorVariablesPList.begin(); pushedFacVarPI != myPushedFactorVariablesPList.end(); ++pushedFacVarPI)
            if (theFactorVariable.equivalentTo(**pushedFacVarPI))
              break;
          if (pushedFacVarPI == myPushedFactorVariablesPList.end()) { // this variable has not yet been pushed
            if (dynamic_cast<xaifBoosterTypeChange::SymbolAlg&>(theFactorVariable.getVariableSymbolReference().getSymbol().getSymbolAlgBase()).needsAllocation()) {
      	      pushDimensionsOf(theFactorVariable,ForLoopReversalType::ANONYMOUS);
      	      pushDimensionsOf(theFactorVariable,ForLoopReversalType::EXPLICIT);
            }
            // ANONYMOUS version
            xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall* theSubroutineCall_p(new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall("push_"+SymbolShape::toShortString(theFactorVariable.getEffectiveShape())));
            theSubroutineCall_p->setId("inline_push");
            theFactorVariable.copyMyselfInto(theSubroutineCall_p->addConcreteArgument(1).getArgument().getVariable());
            myReinterpretedDerivativePropagator.supplyAndAddBasicBlockElementInstance(*theSubroutineCall_p,
                                                                                      ForLoopReversalType::ANONYMOUS);
            // EXPLICIT version
            theSubroutineCall_p=new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall("push_"+SymbolShape::toShortString(theFactorVariable.getEffectiveShape()));
            theSubroutineCall_p->setId("inline_push");
            theFactorVariable.copyMyselfInto(theSubroutineCall_p->addConcreteArgument(1).getArgument().getVariable());
            myReinterpretedDerivativePropagator.supplyAndAddBasicBlockElementInstance(*theSubroutineCall_p,
                                                                                      ForLoopReversalType::EXPLICIT);
            myPushedFactorVariablesPList.push_back(&theFactorVariable);
          }
        } // end if VARIABLE_FACTOR
        if ((*aFactorListI).getKind() != xaifBoosterDerivativePropagator::DerivativePropagatorEntry::Factor::ZERO_FACTOR) { 
          if ((*aFactorListI).getSource().hasArrayAccess()) // push addresses for source
            reinterpretArrayAccess((*aFactorListI).getSource().getArrayAccess());
        } // end if not ZERO_FACTOR
      } // end for all factors
      if ((*entryPListI)->getTarget().hasArrayAccess()) // push addresses for target
        reinterpretArrayAccess((*entryPListI)->getTarget().getArrayAccess());
    } // end for all entries
    // now push all of the single-variable address variables (so they aren't intermixed with the other ones for which an assignment was created)
    for (CVariablePList::const_iterator pushedAddVarPI = myPushedAddressVariablesPList.begin();
         pushedAddVarPI != myPushedAddressVariablesPList.end(); ++pushedAddVarPI) {
      // make the push and save it in the list
      xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall* theSubroutineCall_p = new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall("push_i_"+SymbolShape::toShortString((*pushedAddVarPI)->getEffectiveShape()));
      myReinterpretedDerivativePropagator.supplyAndAddBasicBlockElementInstance(*theSubroutineCall_p,
                                                                                ForLoopReversalType::ANONYMOUS);
      theSubroutineCall_p->setId("BasicBlockPreaccumulationTape::Sequence::reinterpretPropagationsAsTapings:inline_push_i");
      (*pushedAddVarPI)->copyMyselfInto(theSubroutineCall_p->addConcreteArgument(1).getArgument().getVariable());
    } // end for all address variables to be pushed
  }

  void
  Sequence::reinterpretArrayAccess(const ArrayAccess& theArrayAccess) {
    const BasicBlock& theCurrentBasicBlock(ConceptuallyStaticInstances::instance()->getTraversalStack().getCurrentBasicBlockInstance());
    const ArrayAccess::IndexTripletListType& theIndexTripletList(theArrayAccess.getIndexTripletList());
    for (ArrayAccess::IndexTripletListType::const_iterator anIndexTripletListTypeCI=theIndexTripletList.begin();
	 anIndexTripletListTypeCI!=theIndexTripletList.end();
	 ++anIndexTripletListTypeCI) { 
      for (IndexTriplet::IndexPairList::const_iterator anIndexPairListCI=(*anIndexTripletListTypeCI)->getIndexPairList().begin();
	   anIndexPairListCI!=(*anIndexTripletListTypeCI)->getIndexPairList().end();
	   ++anIndexPairListCI) { 
	// now we have two cases, essentially the expression is a single vertex with a constant 
	// (this discounts constant expressions, this is a todo which might be dealt with later or 
	// it may be completely superceded by a TBR analysis)
	const Expression& theIndexExpression(*((*anIndexPairListCI).second));
        if (!theIndexExpression.isConstant()) {
          CallGraphVertexAlg& theCallerCallGraphVertexAlg (dynamic_cast<CallGraphVertexAlg&>(ConceptuallyStaticInstances::instance()->getTraversalStack().getCurrentCallGraphVertexInstance().getCallGraphVertexAlgBase()));
          theCallerCallGraphVertexAlg.markRequiredValue(theIndexExpression,
                                                        theCurrentBasicBlock,
                                                        "xaifBoosterBasicBlockPreaccumulationTape::Sequence::reinterpretArrayAccess");
          xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall* theSubroutineCall_p;
          if (theIndexExpression.numVertices() == 1) { // only one argument, non-const => push its value
            const Variable& theAddressVariable (dynamic_cast<const Argument&>(theIndexExpression.getMaxVertex()).getVariable());
            // check whether we've already pushed this variable
            CVariablePList::const_iterator pushedAddVarPI;
            for (pushedAddVarPI = myPushedAddressVariablesPList.begin(); pushedAddVarPI != myPushedAddressVariablesPList.end(); ++pushedAddVarPI)
              if (theAddressVariable.equivalentTo(**pushedAddVarPI)) {
                DBG_MACRO(DbgGroup::DATA,"BasicBlockPreaccumulationTape::Sequence::reinterpretArrayAccess: "
                                         << "address variable " << theAddressVariable.debug() << " already pushed => skipping");
                break;
              }
            if (pushedAddVarPI == myPushedAddressVariablesPList.end()) { // this variable has not yet been pushed
              DBG_MACRO(DbgGroup::DATA,"BasicBlockPreaccumulationTape::Sequence::reinterpretArrayAccess: "
                                       << "pushing address variable " << theAddressVariable.debug());
              myPushedAddressVariablesPList.push_back(&theAddressVariable);
            }
          } // end one non-const argument
          else { // more than one argument, and at least one is non-const
            // \todo FIXME: for now we push EVERY address assignment variable.
            // we don't yet hope to identify things like x(i+j) and x(i+j) this will be fixed by the proper push/pop framework
            // make the assignment it and save it in the list
            Assignment* theIndexExpressionAssignment_p(new Assignment(false));
            myReinterpretedDerivativePropagator.supplyAndAddBasicBlockElementInstance(*theIndexExpressionAssignment_p,
                                                                                      ForLoopReversalType::ANONYMOUS);
            theIndexExpressionAssignment_p->setId("index_expression_assignment_for_taping");
            // create a new symbol and add a new VariableSymbolReference in the Variable
	    xaifBoosterTypeChange::TemporariesHelper 
	      aTemporariesHelper("xaifBoosterBasicBlockPreaccumulationTape::Sequence::reinterpretArrayAccess",
				 theIndexExpression,
				 theIndexExpression.getMaxVertex());
            VariableSymbolReference* theNewVariableSymbolReference_p =
	      new VariableSymbolReference(aTemporariesHelper.makeTempSymbol(theCurrentBasicBlock.getScope()),
					  theCurrentBasicBlock.getScope());
            theNewVariableSymbolReference_p->setId("1");
            theNewVariableSymbolReference_p->setAnnotation("xaifBoosterBasicBlockPreaccumulationTape::Sequence::reinterpretArrayAccess");
            // pass it on to the LHS and relinquish ownership
            theIndexExpressionAssignment_p->getLHS().supplyAndAddVertexInstance(*theNewVariableSymbolReference_p);
            theIndexExpressionAssignment_p->getLHS().getAliasMapKey().setTemporary();
            theIndexExpressionAssignment_p->getLHS().getDuUdMapKey().setTemporary();
            // set the RHS
            theIndexExpression.copyMyselfInto(theIndexExpressionAssignment_p->getRHS(),false,false);
            // make the subroutine call: 
            theSubroutineCall_p = new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall("push_i_"+SymbolShape::toShortString(theNewVariableSymbolReference_p->getSymbol().getSymbolShape()));
            // save it in the list
            myReinterpretedDerivativePropagator.supplyAndAddBasicBlockElementInstance(*theSubroutineCall_p,
                                                                                      ForLoopReversalType::ANONYMOUS);
            theSubroutineCall_p->setId("xaifBoosterBasicBlockPreaccumulationTape::Sequence::reinterpretArrayAccess:inline_push_i");
            theIndexExpressionAssignment_p->getLHS().copyMyselfInto(theSubroutineCall_p->addConcreteArgument(1).getArgument().getVariable());
          } // end >1 argument
        } // end if index expression is non-constant
      } // loop for index pairs
    } // end for i
  }

  void
  Sequence::pushDimensionsOf(const Variable& theFactorVariable,
                             ForLoopReversalType::ForLoopReversalType_E aReversalType) { 
    const BasicBlock& theCurrentBasicBlock(ConceptuallyStaticInstances::instance()->getTraversalStack().getCurrentBasicBlockInstance());
    SymbolShape::SymbolShape_E theShape=theFactorVariable.getEffectiveShape();
    for(unsigned short dim=1;dim<=theShape;++dim) {
      // save the size info
      Assignment* theSizeAssignment_p(new Assignment(false));
      myReinterpretedDerivativePropagator.supplyAndAddBasicBlockElementInstance(*theSizeAssignment_p,
                                                                                aReversalType);
      theSizeAssignment_p->setId("size_assignment_for_taping");
      // create a new symbol and add a new VariableSymbolReference in the Variable
      VariableSymbolReference* theNewVariableSymbolReference_p =
	new VariableSymbolReference(theCurrentBasicBlock.getScope().getSymbolTable().addUniqueAuxSymbol(SymbolKind::VARIABLE,
                                                                                                        SymbolType::INTEGER_STYPE,
                                                                                                        SymbolShape::SCALAR,
                                                                                                        false),
                                    theCurrentBasicBlock.getScope());
      theNewVariableSymbolReference_p->setId("1");
      theNewVariableSymbolReference_p->setAnnotation("xaifBoosterBasicBlockPreaccumulationTape::Sequence::pushDimensionsOf");
      // pass it on to the LHS and relinquish ownership
      theSizeAssignment_p->getLHS().supplyAndAddVertexInstance(*theNewVariableSymbolReference_p);
      theSizeAssignment_p->getLHS().getAliasMapKey().setTemporary();
      theSizeAssignment_p->getLHS().getDuUdMapKey().setTemporary();
      // make the RHS
      Expression& theSizeExpression(theSizeAssignment_p->getRHS());
      Argument& theNewArgument(*(new Argument()));
      theNewArgument.setId(theSizeExpression.getNextVertexId());
      theSizeExpression.supplyAndAddVertexInstance(theNewArgument);
      theFactorVariable.copyMyselfInto(theNewArgument.getVariable());
      Constant& theConstant(*new Constant(dim));
      theConstant.setId(theSizeExpression.getNextVertexId());
      theSizeExpression.supplyAndAddVertexInstance(theConstant);
      ExpressionVertex& theSizeIntrinsic=theSizeExpression.addBinaryOpByName("size",
                                                                             theNewArgument,
                                                                             theConstant);
      theSizeIntrinsic.setId(theSizeExpression.getNextVertexId());
      // now make the subroutine call: 
      xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall* theSubroutineCall_p = 
	new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall("push_i_"+SymbolShape::toShortString(theSizeAssignment_p->getLHS().getEffectiveShape()));
      // save it in the list
      myReinterpretedDerivativePropagator.supplyAndAddBasicBlockElementInstance(*theSubroutineCall_p,
                                                                                aReversalType);
      theSubroutineCall_p->setId("xaifBoosterBasicBlockPreaccumulationTape::Sequence::pushDimensionsOf");
      theSizeAssignment_p->getLHS().copyMyselfInto(theSubroutineCall_p->addConcreteArgument(1).getArgument().getVariable());
    }
  }

}
 
