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

#include "xaifBooster/algorithms/AdjointUtils/inc/BasicBlockPrintVersion.hpp"

#include "xaifBooster/algorithms/TypeChange/inc/TemporariesHelper.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulationTape/inc/Sequence.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulationTapeAdjoint/inc/BasicBlockAlg.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulationTapeAdjoint/inc/BasicBlockElementAlg.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulationTapeAdjoint/inc/MarkerAlg.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulationTapeAdjoint/inc/SubroutineCallAlg.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulationTapeAdjoint/inc/AssignmentAlg.hpp"

using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulationTapeAdjoint { 

  BasicBlockAlg::BasicBlockAlg(BasicBlock& theContaining) : 
    xaifBooster::BasicBlockAlgBase(theContaining),
    xaifBoosterBasicBlockPreaccumulationTape::BasicBlockAlg(theContaining) { 
  }

  BasicBlockAlg::~BasicBlockAlg() { } 

  void
  BasicBlockAlg::printXMLHierarchy(std::ostream& os) const { 
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent() 
       << "<"
       << PlainBasicBlock::ourXAIFName.c_str()
       << " " 
       << PlainBasicBlock::our_myId_XAIFName.c_str()
       << "=\"" 
       << getContaining().getId().c_str()
       << "\" " 
       << ObjectWithAnnotation::our_myAnnotation_XAIFName.c_str() 
       << "=\""
       << getContaining().getAnnotation().c_str()
       << "\" " 
       << PlainBasicBlock::our_myScopeId_XAIFName.c_str() 
       << "=\""
       << getContaining().getScope().getId().c_str()
       << "\">" 
       << std::endl;
    const PlainBasicBlock::BasicBlockElementList& aBasicBlockElementList(getBasicBlockElementList(xaifBoosterAdjointUtils::BasicBlockPrintVersion::get()));
    for (PlainBasicBlock::BasicBlockElementList::const_iterator li=aBasicBlockElementList.begin();
	 li!=aBasicBlockElementList.end();
	 li++)
      (*(li))->printXMLHierarchy(os);
    os << pm.indent()
       << "</"
       << PlainBasicBlock::ourXAIFName.c_str()
       << ">"
       << std::endl;
    pm.releaseInstance();
  } // end of BasicBlockAlg::printXMLHierarchy
  
  std::string
  BasicBlockAlg::debug () const {
    std::ostringstream out;
    out << "xaifBoosterBasicBlockPreaccumulationTapeAdjoint::BasicBlockAlg[" << xaifBoosterBasicBlockPreaccumulationTape::BasicBlockAlg::debug().c_str()
        << "]" << std::ends;  
    return out.str();
  }

  void BasicBlockAlg::traverseToChildren(const GenericAction::GenericAction_E anAction_c) { 
  } 

  void BasicBlockAlg::replicateAllocationWrapper(const xaifBoosterBasicBlockPreaccumulation::Sequence& currentSequence,
                                                 ForLoopReversalType::ForLoopReversalType_E aReversalType) {
    for (xaifBoosterDerivativePropagator::DerivativePropagator::InlinableSubroutineCallPList::const_iterator ali=currentSequence.myDerivativePropagator.getPropagationAllocationList().begin();
        ali!=currentSequence.myDerivativePropagator.getPropagationAllocationList().end();
        ++ali) {
      xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall& theCopy=addInlinableSubroutineCall((*ali)->getSubroutineName(),aReversalType);
      theCopy.setId((*ali)->getId());
      for (SubroutineCall::ConcreteArgumentPList::const_iterator aali=(*ali)->getArgumentList().begin();
          aali!=(*ali)->getArgumentList().end();
          ++aali) {
        (*aali)->copyMyselfInto(theCopy.addConcreteArgument((*aali)->getPosition()));
      }
    }
  }

  void BasicBlockAlg::algorithm_action_5() {
    DBG_MACRO(DbgGroup::CALLSTACK, "xaifBoosterBasicBlockPreaccumulationTapeAdjoint::BasicBlockAlg::algorithm_action_5(adjoin propagators)");
    for (xaifBoosterBasicBlockPreaccumulation::CFlattenedBasicBlockElementPList::const_reverse_iterator aFBBEri(myFlattenedBasicBlockElementPList.rbegin());
         aFBBEri != myFlattenedBasicBlockElementPList.rend(); ++aFBBEri) {
      const xaifBoosterBasicBlockPreaccumulation::FlattenedBasicBlockElement& theFBBE(**aFBBEri);
      switch(theFBBE.myType) {
        case xaifBoosterBasicBlockPreaccumulation::MARKER:
	  dynamic_cast<MarkerAlg&>(theFBBE.myRef.myMarker_p->getMarkerAlgBase()).insertYourself(getContaining());
          break;
        case xaifBoosterBasicBlockPreaccumulation::SUBROUTINE_CALL:
	  dynamic_cast<SubroutineCallAlg&>(theFBBE.myRef.mySubroutineCall_p->getSubroutineCallAlgBase()).insertYourself(getContaining());
          break;
        case xaifBoosterBasicBlockPreaccumulation::NONINLINABLE_ASSIGNMENT:
	  dynamic_cast<AssignmentAlg&>(theFBBE.myRef.myNonInlinableAssignment_p->getAssignmentAlgBase()).insertYourself(getContaining());
          break;
        case xaifBoosterBasicBlockPreaccumulation::SEQUENCE: {
          const xaifBoosterBasicBlockPreaccumulationTape::Sequence& currentSequence(
           dynamic_cast<const xaifBoosterBasicBlockPreaccumulationTape::Sequence&>(*theFBBE.myRef.mySequence_p)
          );
          // pop all of the address variables
          for (CVariablePList::const_reverse_iterator pushedAddVarPrI = currentSequence.getPushedAddressVariablesPList().rbegin();
               pushedAddVarPrI != currentSequence.getPushedAddressVariablesPList().rend(); ++pushedAddVarPrI) {
            xaifBoosterTypeChange::TemporariesHelper aTemporariesHelper("xaifBoosterBasicBlockPreaccumulationTapeAdjoint::BasicBlockAlg::algorithm_action_5",
                                                                        **pushedAddVarPrI);
            const Variable& thePoppedAddressVariable (addAddressPop(ForLoopReversalType::ANONYMOUS,aTemporariesHelper));
            DBG_MACRO(DbgGroup::DATA,"BasicBlockPreaccumulationTapeAdjoint::BasicBlockAlg::algorithm_action_5: "
                                  << "Popping address into variable " << thePoppedAddressVariable.debug());
            //addAddressPop(ForLoopReversalType::EXPLICIT);
            // we push to the front so that when we search from the beginning we find the most recent addition,
            // which should be the appropriate one for the occurance of this variable in this sequence
            myAddressVariableCorList.push_front(std::make_pair(*pushedAddVarPrI,&thePoppedAddressVariable));
          }
          // pop all of the factor variables
          for (CVariablePList::const_reverse_iterator pushedFacVarPrI = currentSequence.getPushedFactorVariablesPList().rbegin();
               pushedFacVarPrI != currentSequence.getPushedFactorVariablesPList().rend(); ++pushedFacVarPrI) {
            xaifBoosterTypeChange::TemporariesHelper aTemporariesHelper("BasicBlockPreaccumulationTapeAdjoint::BasicBlockAlg::algorithm_action_5",
                                                                        **pushedFacVarPrI);
            const Symbol& aTemporarySymbol (aTemporariesHelper.makeTempSymbol(getContaining().getScope()));
            // allocation needed?
            if (aTemporariesHelper.needsAllocation()) { 
              addAllocation(aTemporarySymbol,
                            getContaining().getScope(),
                            aTemporariesHelper,
                            ForLoopReversalType::ANONYMOUS);
              addAllocation(aTemporarySymbol,
                            getContaining().getScope(),
                            aTemporariesHelper,
                            ForLoopReversalType::EXPLICIT);
            }
            // Anonymous version
            const Variable& thePoppedFactorVariable(addFactorPop(aTemporarySymbol,
                                                                 ForLoopReversalType::ANONYMOUS));
            // Explicit version (we can use the same symbol for the popped factor variable)
            addFactorPop(aTemporarySymbol,
                         ForLoopReversalType::EXPLICIT);
            myFactorVariableCorList.push_back(std::make_pair(*pushedFacVarPrI,&thePoppedFactorVariable));
          } // end for all pushed factor variables
          // replicate derivative propagator allocationshere
          replicateAllocationWrapper(currentSequence,ForLoopReversalType::ANONYMOUS);
          replicateAllocationWrapper(currentSequence,ForLoopReversalType::EXPLICIT);
          const xaifBoosterDerivativePropagator::DerivativePropagator& aDerivativePropagator(currentSequence.myDerivativePropagator);
          for(xaifBoosterDerivativePropagator::DerivativePropagator::EntryPList::const_reverse_iterator entryPListI=
               aDerivativePropagator.getEntryPList().rbegin();
              entryPListI!= aDerivativePropagator.getEntryPList().rend();
              ++entryPListI) {
            reinterpretDerivativePropagatorEntry(**entryPListI);
          } // end for DerivativePropagatorEntry list
          break;
        }
        default:
          THROW_LOGICEXCEPTION_MACRO("xaifBoosterBasicBlockPreaccumulationTapeAdjoint::algorithm_action_5: unexpected FBBE type")
          break;
      } // end switch
    }
  } // end BasicBlockAlg::algorithm_action_5()

  Scope&  
  BasicBlockAlg::getScope() { 
    return getContaining().getScope();
  } 

} // end namespace xaifBoosterBasicBlockPreaccumulationTapeAdjoint

