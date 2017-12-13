#ifndef _XAIFBOOSTERDERIVATIVEPROPAGATOR_INCLUDE_
#define _XAIFBOOSTERDERIVATIVEPROPAGATOR_INCLUDE_
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

#include "xaifBooster/utils/inc/XMLPrintable.hpp"

#include "xaifBooster/system/inc/Constant.hpp"
#include "xaifBooster/system/inc/Variable.hpp"

#include "xaifBooster/algorithms/DerivativePropagator/inc/DerivativePropagatorEntry.hpp"
#include "xaifBooster/algorithms/DerivativePropagator/inc/DerivativePropagatorDecDeriv.hpp"
#include "xaifBooster/algorithms/DerivativePropagator/inc/DerivativePropagatorIncDeriv.hpp"
#include "xaifBooster/algorithms/DerivativePropagator/inc/DerivativePropagatorSaxpy.hpp"
#include "xaifBooster/algorithms/DerivativePropagator/inc/DerivativePropagatorSetDeriv.hpp"
#include "xaifBooster/algorithms/DerivativePropagator/inc/DerivativePropagatorSetNegDeriv.hpp"

using namespace xaifBooster;

namespace xaifBoosterDerivativePropagator { 

  class DerivativePropagator : public XMLPrintable {
  public:

    DerivativePropagator (){};

    ~DerivativePropagator ();

    void printXMLHierarchy(std::ostream& os) const;

    /**
     * the actual printing is in here 
     * so we can take a function pointer and 
     * replace this 
     */
    static void printXMLHierarchyImpl(std::ostream& os,
				      const DerivativePropagator& aDerivativePropagator);

    std::string debug() const ;

    /**
     * name for this class as represented in XAIF schema
     */
    static const std::string ourXAIFName;
    
    typedef std::list<DerivativePropagatorEntry*> EntryPList;

    const DerivativePropagatorSetDeriv& addSetDerivToEntryPList(const Variable& theTarget,
								const Variable& theSource);

    const DerivativePropagatorSetNegDeriv& addSetNegDerivToEntryPList(const Variable& theTarget,
                                                                      const Variable& theSource);

    const DerivativePropagatorIncDeriv& addIncDerivToEntryPList(const Variable& theTarget,
                                                                const Variable& theSource);

    const DerivativePropagatorDecDeriv& addDecDerivToEntryPList(const Variable& theTarget,
                                                                const Variable& theSource);

    DerivativePropagatorSaxpy& addSaxpyToEntryPList(const Constant& thePartial,
						    const Variable& theIndependent,
						    const Variable& theDependent);

    DerivativePropagatorSaxpy& addSaxpyToEntryPList(const Variable& thePartial,
						    const Variable& theIndependent,
						    const Variable& theDependent);

    void addZeroDerivToEntryPList(const Variable& theTarget);

    const EntryPList& getEntryPList() const;

    /** 
     * now that we want to have control over the insertion point
     * we need to supply a non-const list
     */
    EntryPList& getEntryPList();

    /**
     * check for \p anExpression in every entry
     */
    virtual bool hasExpression(const Expression& anExpression) const;

    typedef std::list<xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall*> InlinableSubroutineCallPList;

    InlinableSubroutineCallPList& getPropagationAllocationList();

    const InlinableSubroutineCallPList& getPropagationAllocationList() const;

    void printPropagationAllocationList(std::ostream&) const;

  private:

    /**
     * this class owns the DerivativePropagatorEntry
     * instances pointed to in myEntryPList
     */
    EntryPList myEntryPList;

    /**
      * list to hold allocation calls to be added for
      * the propagation phase
      */
    InlinableSubroutineCallPList myPropagationAllocationList;

  }; // end of class DerivativePropagator
 
} // end of namespace
                                                                     
#endif
