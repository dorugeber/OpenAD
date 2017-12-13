#ifndef _XAIFBOOSTERDERIVATIVEPROPAGATORSETDERIV_INCLUDE_
#define _XAIFBOOSTERDERIVATIVEPROPAGATORSETDERIV_INCLUDE_
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

  class DerivativePropagatorSetDeriv : public DerivativePropagatorEntry {
  public:

    DerivativePropagatorSetDeriv(const Variable& theTarget,
				 const Variable& theSource);

    ~DerivativePropagatorSetDeriv(){};

    void printXMLHierarchy(std::ostream& os) const;

    void printMemberXMLHierarchy(const Variable& theVariable,
				 const std::string& aName,
				 std::ostream& os) const;

    std::string debug() const ;

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

    /**
     * no def
     */
    DerivativePropagatorSetDeriv ();

    /**
     * no def
     */
    DerivativePropagatorSetDeriv (const DerivativePropagatorSetDeriv&);

    /**
     * no def
     */
    DerivativePropagatorSetDeriv& operator=(const DerivativePropagatorSetDeriv&);

    /**
     * this version only has a single source
     */
    Variable mySource;

  }; // end of class DerivativePropagatorSetDeriv
 
} // end of namespace 
                                                                     
#endif
