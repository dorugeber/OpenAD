#ifndef _XAIFBOOSTERDERIVATIVEPROPAGATORSAXPY_INCLUDE_
#define _XAIFBOOSTERDERIVATIVEPROPAGATORSAXPY_INCLUDE_
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

#include "xaifBooster/system/inc/Constant.hpp"
#include "xaifBooster/system/inc/Expression.hpp"

#include "xaifBooster/algorithms/DerivativePropagator/inc/DerivativePropagatorEntry.hpp"

using namespace xaifBooster;

namespace xaifBoosterDerivativePropagator { 

  class DerivativePropagatorSaxpy : public DerivativePropagatorEntry {
  public:

    DerivativePropagatorSaxpy(const Variable& theA,
			      const Variable& theX,
			      const Variable& theY);

    DerivativePropagatorSaxpy(const Constant& theA,
			      const Variable& theX,
			      const Variable& theY);

    ~DerivativePropagatorSaxpy();

    void printXMLHierarchy(std::ostream& os) const;

    std::string debug() const;

    void useAsSax();

    /**
     * name for this class as represented in XAIF schema
     */
    static const std::string ourXAIFName;

    /**
     * name for this class as represented in XAIF schema
     */
    static const std::string our_SAX_XAIFName;

    static const std::string our_myA_XAIFName;
    
    static const std::string our_myX_XAIFName;

    static const std::string our_myTarget_XAIFName;

    static const std::string our_myAX_XAIFName;

    void addAX(const Variable& theA,
	       const Variable& theX);

    void addAX(const Constant& theA,
	       const Variable& theX);
    
    virtual void  getFactors(FactorList& theFactorList) const ; 

    virtual bool isIncremental()const; 

    /**
     * redefined from DerivativePropagatorEntry::hasExpression to also check
     * the factor and source for each AX
     */
    virtual bool hasExpression(const Expression& anExpression) const;

    virtual const xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall& asInlinableSubroutineCall() const;

  private:

    /**
     * no def
     */
    DerivativePropagatorSaxpy ();

    /**
     * no def
     */
    DerivativePropagatorSaxpy (const DerivativePropagatorSaxpy&);

    /**
     * no def
     */
    DerivativePropagatorSaxpy& operator=(const DerivativePropagatorSaxpy&);

    struct AX { 
      Expression myA;
      Variable myX;
    }; 
    typedef std::list <AX*> AXPList;

    AXPList myAXPList;

    bool useAsSaxFlag;

    void printMemberXMLHierarchy(const Variable& theVariable,
				 const std::string& aName,
				 std::ostream& os) const;

    void printMemberXMLHierarchy(const Expression& thePartial,
				 const std::string& aName,
				 std::ostream& os) const;

    void printAXMemberXMLHierarchy(const AX&,
				   std::ostream& os) const;

  }; // end of class DerivativePropagatorSaxpy
 
} // end of namespace 
                                                                     
#endif
