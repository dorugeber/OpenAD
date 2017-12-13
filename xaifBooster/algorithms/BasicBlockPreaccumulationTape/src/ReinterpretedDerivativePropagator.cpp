// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/algorithms/BasicBlockPreaccumulationTape/inc/ReinterpretedDerivativePropagator.hpp"

using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulationTape {

  ReinterpretedDerivativePropagator::ReinterpretedDerivativePropagator() {}

  ReinterpretedDerivativePropagator::~ReinterpretedDerivativePropagator() {
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

  void
  ReinterpretedDerivativePropagator::supplyAndAddBasicBlockElementInstance(BasicBlockElement& theBasicBlockElement,
                                                                           const ForLoopReversalType::ForLoopReversalType_E& aReversalType) {
    switch(aReversalType) {
    case ForLoopReversalType::ANONYMOUS :
      myBasicBlockElementListAnonymousReversal.push_back(&theBasicBlockElement);
      break;
    case ForLoopReversalType::EXPLICIT :
      myBasicBlockElementListExplicitReversal.push_back(&theBasicBlockElement);
      break;
    default:
      THROW_LOGICEXCEPTION_MACRO("ReinterpretedDerivativePropagator::supplyAndAddBasicBlockElementInstance: unknown reversal type "
				 << ForLoopReversalType::toString(aReversalType).c_str());
      break;
    } // end switch on aReversalType
  } // end ReinterpretedDerivativePropagator::supplyAndAddBasicBlockElementInstance()

  const PlainBasicBlock::BasicBlockElementList&
  ReinterpretedDerivativePropagator::getBasicBlockElementList(const ForLoopReversalType::ForLoopReversalType_E& aReversalType) const {
    switch(aReversalType) {
    case ForLoopReversalType::ANONYMOUS :
      return myBasicBlockElementListAnonymousReversal;
      break;
    case ForLoopReversalType::EXPLICIT :
      return myBasicBlockElementListExplicitReversal;
      break;
    default:
      THROW_LOGICEXCEPTION_MACRO("ReinterpretedDerivativePropagator::getBasicBlockElementList: unknown reversal type "
				 << ForLoopReversalType::toString(aReversalType).c_str());
      break;
    }
    // to make the compiler happy:
    return myBasicBlockElementListAnonymousReversal;
  }

  bool
  ReinterpretedDerivativePropagator::hasExpression(const Expression& anExpression) const {
    // check myBasicBlockElementListAnonymousReversal
    for (PlainBasicBlock::BasicBlockElementList::const_iterator anonI = myBasicBlockElementListAnonymousReversal.begin();
         anonI != myBasicBlockElementListAnonymousReversal.end(); ++anonI)
      if ((*anonI)->hasExpression(anExpression))
        return true;
    // check myBasicBlockElementListExplicitReversal
    for (PlainBasicBlock::BasicBlockElementList::const_iterator explicitI = myBasicBlockElementListExplicitReversal.begin();
         explicitI != myBasicBlockElementListExplicitReversal.end(); ++explicitI)
      if ((*explicitI)->hasExpression(anExpression))
        return true;
    return false;
  } // end ReinterpretedDerivativePropagator::hasExpression()

}

