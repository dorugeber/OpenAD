// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/utils/inc/LogicException.hpp"

#include "xaifBooster/system/inc/BasicBlock.hpp"

#include "xaifBooster/algorithms/AdjointUtils/inc/BasicBlockPrintVersion.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulationTape/inc/BasicBlockAlg.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulationTape/inc/Sequence.hpp"

using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulationTape { 

  BasicBlockAlg::BasicBlockAlg(BasicBlock& theContaining) : 
    xaifBooster::BasicBlockAlgBase(theContaining),
    xaifBoosterBasicBlockPreaccumulation::BasicBlockAlg(theContaining) { 
  }

  BasicBlockAlg::~BasicBlockAlg() {}

  void
  BasicBlockAlg::printXMLHierarchy(std::ostream& os) const { 
    xaifBoosterBasicBlockPreaccumulation::BasicBlockAlg::printXMLHierarchyImpl(os,
									       printDerivativePropagatorAsTape);
  } // end of BasicBlockAlg::printXMLHierarchy
  
  void BasicBlockAlg::printDerivativePropagatorAsTape(std::ostream& os,
						      const BasicBlockAlgBase& aBasicBlockAlg, 
						      const xaifBoosterDerivativePropagator::DerivativePropagator& aPropagator) { 
    const BasicBlockAlg& ourAlgorithm(dynamic_cast<const BasicBlockAlg&>(aBasicBlockAlg));
    SequencePList::const_iterator aSequencePListI;
    for (aSequencePListI = ourAlgorithm.myUniqueSequencePList.begin(); aSequencePListI != ourAlgorithm.myUniqueSequencePList.end(); ++aSequencePListI) {
      Sequence& theSequence(dynamic_cast<Sequence&>(**aSequencePListI));
      if (&theSequence.myDerivativePropagator == &aPropagator) { // this is the right one: 
	for (PlainBasicBlock::BasicBlockElementList::const_iterator aBasicBlockElementListI=
	       theSequence.getReinterpretedDerivativePropagator().getBasicBlockElementList(xaifBoosterAdjointUtils::BasicBlockPrintVersion::get()).begin();
	     aBasicBlockElementListI != theSequence.getReinterpretedDerivativePropagator().getBasicBlockElementList(xaifBoosterAdjointUtils::BasicBlockPrintVersion::get()).end();
	     ++aBasicBlockElementListI)
	  (*(aBasicBlockElementListI))->printXMLHierarchy(os);
	break; 
      } // end if 
    }
    if (aSequencePListI == ourAlgorithm.myUniqueSequencePList.end()) // we didn't find it...
      THROW_LOGICEXCEPTION_MACRO("BasicBlockAlg::printDerivativePropagatorAsTape: didn't find proper ReinterpretedDerivativePropagator");
  } // end BasicBlockAlg::printDerivativePropagatorAsTape()

  bool
  BasicBlockAlg::hasExpression(const Expression& anExpression) const {
    for (SequencePList::const_iterator aSequencePListI = myUniqueSequencePList.begin(); aSequencePListI != myUniqueSequencePList.end(); ++aSequencePListI)
      if ((*aSequencePListI)->hasExpression(anExpression))
        return true;
    return xaifBoosterBasicBlockPreaccumulation::BasicBlockAlg::hasExpression(anExpression);
  } // end BasicBlockAlg::hasExpression()

  void BasicBlockAlg::algorithm_action_4() { 
    DBG_MACRO(DbgGroup::CALLSTACK, "xaifBoosterBasicBlockPreaccumulationTape::BasicBlockAlg::algorithm_action_4(reinterpret DerivativePropagators as tapings)");
    // for each propagator:
    // create an InlinableSubroutinecall for each Variable in each saxpy element in the propagator
    // and also one for each index of target or source vertices
    // The order is fixed as follows:
    // - pairs of : 
    //   - JacobianEntry value (if variable)
    //   - Source index value (if variable array index)
    // - Target index value (if variable array index) 
    // obviously this order has to be matched by the reverse sweep reading this tape.
    for (SequencePList::const_iterator aSequencePListI = myUniqueSequencePList.begin(); aSequencePListI != myUniqueSequencePList.end(); ++aSequencePListI) {
      Sequence& theSequence(dynamic_cast<Sequence&>(**aSequencePListI));
      theSequence.reinterpretPropagationsAsTapings();
    }
  } // end BasicBlockAlg::algorithm_action_4()

  std::string BasicBlockAlg::debug () const { 
    std::ostringstream out;
    out << "xaifBoosterBasicBlockPreaccumulationTape::BasicBlockAlg[" << xaifBoosterBasicBlockPreaccumulation::BasicBlockAlg::debug().c_str()
 	<< "]" << std::ends;  
    return out.str();
  } // end of BasicBlockAlg::debug

  void BasicBlockAlg::traverseToChildren(const GenericAction::GenericAction_E anAction_c) { 
  } 

} // end namespace xaifBoosterBasicBlockPreaccumulationTape

