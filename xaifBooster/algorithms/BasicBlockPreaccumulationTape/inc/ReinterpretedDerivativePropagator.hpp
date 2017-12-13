#ifndef _XAIFBOOSTERBASICBLOCKPREACCUMULATIONTAPE_REINTERPRETEDDERIVATIVEPROPAGATOR_INCLUDE_
#define _XAIFBOOSTERBASICBLOCKPREACCUMULATIONTAPE_REINTERPRETEDDERIVATIVEPROPAGATOR_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/ForLoopReversalType.hpp"
#include "xaifBooster/system/inc/PlainBasicBlock.hpp"

#include "xaifBooster/algorithms/DerivativePropagator/inc/DerivativePropagator.hpp"

using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulationTape {

  /** 
   * here we keep the reinterpreted statements
   * the association with the respective DerivativePropagator
   * comes about by virtue of the fact that they are both a part of a sequence
   */
  class ReinterpretedDerivativePropagator {

  public:

    ReinterpretedDerivativePropagator();

    ~ReinterpretedDerivativePropagator();

    /**
     * adding a reinterpretation element to our list
     */
    void
    supplyAndAddBasicBlockElementInstance(BasicBlockElement& theBasicBlockElement,
                                          const ForLoopReversalType::ForLoopReversalType_E& aReversalType);

    const PlainBasicBlock::BasicBlockElementList&
    getBasicBlockElementList(const ForLoopReversalType::ForLoopReversalType_E& aReversalType) const;

    /**
     * check myBasicBlockElementListAnonymousReversal and myBasicBlockElementListExplicitReversal for \p anExpression
     */
    bool
    hasExpression(const Expression& anExpression) const;

  private:

    /// no def
    ReinterpretedDerivativePropagator(const ReinterpretedDerivativePropagator&);

    /// no def
    ReinterpretedDerivativePropagator& operator=(const ReinterpretedDerivativePropagator&);

    /** 
     * the elements that the reinterpretation consists of
     * which assumes an anonymous reversal (no knowledge about the original 
     * loop variables)
     */
    PlainBasicBlock::BasicBlockElementList myBasicBlockElementListAnonymousReversal;

    /** 
     * the elements that the reinterpretation consists of
     * under an explicit reversal (loop constructs are reversed explicitly
     * and we assume all index expressions can be recalculated explcitly 
     * at reversal time from explicitly reversed loops)
     */
    PlainBasicBlock::BasicBlockElementList myBasicBlockElementListExplicitReversal;

  }; // end of class ReinterpretedDerivativePropagator

}

#endif

