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

#include "xaifBooster/system/inc/VariableSymbolReference.hpp"
#include "xaifBooster/system/inc/ConceptuallyStaticInstances.hpp"

#include "xaifBooster/algorithms/TypeChange/inc/TemporariesHelper.hpp"

#include "xaifBooster/algorithms/AdjointUtils/inc/BasicBlockPrintVersion.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulationTapeAdjoint/inc/AssignmentAlg.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulationTapeAdjoint/inc/BasicBlockAlg.hpp"

namespace xaifBoosterBasicBlockPreaccumulationTapeAdjoint {  

  AssignmentAlg::AssignmentAlg(Assignment& theContainingAssignment) : 
    xaifBoosterBasicBlockPreaccumulation::AssignmentAlg(theContainingAssignment),
    xaifBoosterBasicBlockPreaccumulationTape::AssignmentAlg(theContainingAssignment),
    xaifBoosterBasicBlockPreaccumulationTapeAdjoint::BasicBlockElementAlg(theContainingAssignment),
    myPlaceholderDummyAnonymous_p(0),
    myPlaceholderDummyExplicit_p(0) {
  }

  AssignmentAlg::~AssignmentAlg() { }

  void AssignmentAlg::printXMLHierarchy(std::ostream& os) const { 
    const PlainBasicBlock::BasicBlockElementList& aBasicBlockElementList(getBasicBlockElementList(xaifBoosterAdjointUtils::BasicBlockPrintVersion::get()));
    for (PlainBasicBlock::BasicBlockElementList::const_iterator li=aBasicBlockElementList.begin();
	 li!=aBasicBlockElementList.end();
	 li++)
      (*(li))->printXMLHierarchy(os);
  }

  std::string 
  AssignmentAlg::debug() const { 
    std::ostringstream out;
    out << "xaifBoosterBasicBlockPreaccumulationTapeAdjoint::AssignmentAlg[" << xaifBoosterBasicBlockPreaccumulationTape::AssignmentAlg::debug().c_str()
        << "]" << std::ends;
    return out.str();
  }

  void AssignmentAlg::traverseToChildren(const GenericAction::GenericAction_E anAction_c) { 
  } 
  
  void AssignmentAlg::insertYourself(const BasicBlock& theBasicBlock) { 
    if (!getActiveFlag() || !getContainingAssignment().isNonInlinable())
      return; 
    insertYourself(theBasicBlock, ForLoopReversalType::ANONYMOUS);
    insertYourself(theBasicBlock, ForLoopReversalType::EXPLICIT);
  } 

  void AssignmentAlg::insertYourself(const BasicBlock& theBasicBlock,
				     ForLoopReversalType::ForLoopReversalType_E aReversalType) {
    Assignment **theDummy_pp=0;
    switch (aReversalType) {
    case ForLoopReversalType::ANONYMOUS:
      theDummy_pp=&myPlaceholderDummyAnonymous_p;
      break;
    case ForLoopReversalType::EXPLICIT:
      theDummy_pp=&myPlaceholderDummyExplicit_p;
      break;
    default: {
      THROW_LOGICEXCEPTION_MACRO("AssignmentAlg::insertYourself: unimplemented logic for reversal type " << ForLoopReversalType::toString(aReversalType).c_str());
      break;
    }
    }
    if (*theDummy_pp) {
      THROW_LOGICEXCEPTION_MACRO("AssignmentAlg::insertYourself: dummy placeholder already set");
    }
    BasicBlockAlg& theBasicBlockAlg(dynamic_cast<BasicBlockAlg&>(theBasicBlock.getBasicBlockAlgBase()));
    // make the placeholder:
    *theDummy_pp=&(theBasicBlockAlg.addAssignment(aReversalType,true)); // here we want the algorithm object to attach things to
    // do the adjoint assembly on the algorithm of the dummy
    dynamic_cast<AssignmentAlg&>((*theDummy_pp)->getAssignmentAlgBase()).adjointAssembly(*this,
											 theBasicBlock,
											 aReversalType);
  }

  void AssignmentAlg::adjointAssembly(const AssignmentAlg& theOrigAlg, const BasicBlock& theBasicBlock, ForLoopReversalType::ForLoopReversalType_E aReversalType) {
    BasicBlockAlg& theBasicBlockAlg(dynamic_cast<BasicBlockAlg&>(theBasicBlock.getBasicBlockAlgBase()));
    const Variable& theSource(theOrigAlg.getContainingAssignment().getLHS());
    const Expression& theOrigRHS(theOrigAlg.getContainingAssignment().getRHS());
    const ExpressionVertex& theOrigIntrinsic(theOrigRHS.getMaxVertex());
    const NonInlinableIntrinsicsCatalogueItem& theNonInlinableIntrinsicsCatalogueItem(theOrigIntrinsic.getNonInlinableIntrinsicsCatalogueItem());
    if (theNonInlinableIntrinsicsCatalogueItem.isExplicitJacobian()) {
      THROW_LOGICEXCEPTION_MACRO("AssignmentAlg::algorithm_action_3: not implemented for explicit Jacobian")
	}
    else if (theNonInlinableIntrinsicsCatalogueItem.isDirectAction()) {
      Expression::ConstInEdgeIteratorPair p(theOrigRHS.getInEdgesOf(theOrigRHS.getMaxVertex()));
      Expression::ConstInEdgeIterator ieIt(p.first),endIt(p.second);
      for (; ieIt!=endIt; ++ieIt) {
	if (theOrigRHS.getSourceOf(*ieIt).isArgument()) {
	  // get the deriv action for this argument
	  Assignment& derivActionAsgnmt(addAssignment(aReversalType,false)); // here we don't want the algorithm object, it is just an assignment
	  Intrinsic* newIntrinsic_p=new Intrinsic(theNonInlinableIntrinsicsCatalogueItem.getDirectAction().getDerivAction(),false);
	  derivActionAsgnmt.getRHS().supplyAndAddVertexInstance(*newIntrinsic_p);
	  newIntrinsic_p->setId(derivActionAsgnmt.getRHS().getNextVertexId());
	  Expression::ConstInEdgeIteratorPair p1(theOrigRHS.getInEdgesOf(theOrigIntrinsic));
	  Expression::ConstInEdgeIterator ieIt1(p1.first),endIt1(p1.second);
	  for (; ieIt1!=endIt1; ++ieIt1) {
	    // make the argument
            Argument* newArgument_p=new Argument(false);
            theSource.copyMyselfInto(newArgument_p->getVariable());
	    if (newArgument_p->getVariable().getActiveType())
	      newArgument_p->getVariable().setDerivFlag();
	    derivActionAsgnmt.getRHS().supplyAndAddVertexInstance(*newArgument_p);
            newArgument_p->setId(derivActionAsgnmt.getRHS().getNextVertexId());
	    ExpressionEdge* newEdge_p=new ExpressionEdge(false);
	    derivActionAsgnmt.getRHS().supplyAndAddEdgeInstance(*newEdge_p,*newArgument_p,*newIntrinsic_p);
	    newEdge_p->setPosition((*ieIt).getPosition());
	    newEdge_p->setId(derivActionAsgnmt.getRHS().getNextEdgeId());
	  }
	  // temporary LHS modeled after the argument
          const Argument& theOrigArg(dynamic_cast<const Argument&>(theOrigRHS.getSourceOf(*ieIt)));
	  xaifBoosterTypeChange::TemporariesHelper aTemporariesHelper("AssignmentAlg::insertYourself",
								      theOrigArg.getVariable());
	  VariableSymbolReference* theNewVariableSymbolReference_p=
	    new VariableSymbolReference(aTemporariesHelper.makeTempSymbol(theBasicBlockAlg.getContaining().getScope(),
                                                                          true),
					theBasicBlockAlg.getContaining().getScope());
	  theNewVariableSymbolReference_p->setId("1");
          theNewVariableSymbolReference_p->setAnnotation("AssignmentAlg::insertYourself");
	  derivActionAsgnmt.getLHS().supplyAndAddVertexInstance(*theNewVariableSymbolReference_p);
          derivActionAsgnmt.getLHS().markTemporary();
          derivActionAsgnmt.getLHS().setDerivFlag();
	  addUnitFactor(theOrigArg.getVariable(),
                        derivActionAsgnmt.getLHS(),
			aReversalType); 
	  addZeroDeriv(theSource,aReversalType);
	}
      }
    }
    handleArrayAccessIndices(*this,aReversalType);
  } 

  void AssignmentAlg::handleArrayAccessIndices(AssignmentAlg& orignalCallAlg,
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
	thePopCall_p=(new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall("push_"+SymbolShape::toShortString((*pairIt).first->getEffectiveShape())));
      else
	THROW_LOGICEXCEPTION_MACRO("AssignmentAlg::handleArrayAccessIndices: no logic to pop things of type " 
				   << SymbolType::toString((*pairIt).first->getVariableSymbolReference().getSymbol().getSymbolType()).c_str()
				   << " and shape "
				   << SymbolShape::toString((*pairIt).first->getEffectiveShape()).c_str())

	  getBasicBlockElementList(ForLoopReversalType::EXPLICIT).push_back(thePopCall_p);
      thePopCall_p->setId("AssignmentAlg::handleArrayAccessIndices");
      (*pairIt).first->copyMyselfInto(thePopCall_p->addConcreteArgument(1).getArgument().getVariable());
    }
  }

  Scope&  
  AssignmentAlg::getScope() { 
    return ConceptuallyStaticInstances::instance()->getTraversalStack().getCurrentBasicBlockInstance().getScope();
  } 

} // end namespace xaifBoosterBasicBlockPreaccumulationTapeAdjoint
