#ifndef _XAIFBOOSTERBASICBLOCKPREACCUMULATIONTAPE_SEQUENCE_INCLUDE_
#define _XAIFBOOSTERBASICBLOCKPREACCUMULATIONTAPE_SEQUENCE_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include <list>

#include "xaifBooster/system/inc/BasicBlockElement.hpp"
#include "xaifBooster/system/inc/ForLoopReversalType.hpp"
#include "xaifBooster/system/inc/Variable.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/Sequence.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulationTape/inc/ReinterpretedDerivativePropagator.hpp"


using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulationTape {

  /**
   * Derived Sequence class for taping.
   * designed to facilitate the reinterpretation of propagations as tapings
   */
  class Sequence : public xaifBoosterBasicBlockPreaccumulation::Sequence {

  public:

    Sequence();

    virtual ~Sequence();

    virtual std::string debug() const;

    /**
     * check myReinterpretedDerivativePropagator for \p anExpression
     */
    virtual bool hasExpression(const Expression& anExpression) const;

    const CVariablePList& getPushedAddressVariablesPList() const;

    const CVariablePList& getPushedFactorVariablesPList() const;

    const ReinterpretedDerivativePropagator& getReinterpretedDerivativePropagator() const;

    void reinterpretPropagationsAsTapings();

  private:

    CVariablePList myPushedAddressVariablesPList;

    CVariablePList myPushedFactorVariablesPList;

    ReinterpretedDerivativePropagator myReinterpretedDerivativePropagator;

    /** 
     * some helper that deals with pushing computed indices
     */
    void reinterpretArrayAccess(const ArrayAccess& theArrayAccess);

    void pushDimensionsOf(const Variable& theFactorVariable,
                          ForLoopReversalType::ForLoopReversalType_E aReversalType);

    /// no def
    Sequence(const Sequence&);

    /// no def
    Sequence& operator= (const Sequence&);

  };

}

#endif
