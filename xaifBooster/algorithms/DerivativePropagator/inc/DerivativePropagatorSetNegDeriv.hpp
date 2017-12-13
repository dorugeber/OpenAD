#ifndef _XAIFBOOSTERDERIVATIVEPROPAGATORSETNEGDERIV_INCLUDE_
#define _XAIFBOOSTERDERIVATIVEPROPAGATORSETNEGDERIV_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/algorithms/DerivativePropagator/inc/DerivativePropagatorEntry.hpp"

using namespace xaifBooster;

namespace xaifBoosterDerivativePropagator {

  class DerivativePropagatorSetNegDeriv : public DerivativePropagatorEntry {
  public:

    DerivativePropagatorSetNegDeriv(const Variable& theTarget,
                                    const Variable& theSource);

    ~DerivativePropagatorSetNegDeriv(){};

    void printXMLHierarchy(std::ostream& os) const;

    void printMemberXMLHierarchy(const Variable& theVariable,
                                 const std::string& aName,
                                 std::ostream& os) const;

    std::string debug() const;

    /**
     * name for this class as represented in XAIF schema
     */
    static const std::string ourXAIFName;

    static const std::string our_myTarget_XAIFName;

    static const std::string our_mySource_XAIFName;

    virtual void  getFactors(FactorList& theFactorList) const ; 

    /// redefined from DerivativePropagatorEntry::hasExpression to also check the source
    virtual bool hasExpression(const Expression& anExpression) const;

    virtual const xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall& asInlinableSubroutineCall() const;

  private:

    /// no def
    DerivativePropagatorSetNegDeriv();

    /// no def
    DerivativePropagatorSetNegDeriv(const DerivativePropagatorSetNegDeriv&);

    /// no def
    DerivativePropagatorSetNegDeriv& operator=(const DerivativePropagatorSetNegDeriv&);

    /// this version only has a single source
    Variable mySource;

  }; // end class DerivativePropagatorSetNegDeriv
 
} // end namespace xaifBoosterDerivativePropagator

#endif

