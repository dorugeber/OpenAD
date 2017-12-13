#ifndef _XAIFBOOSTERDERIVATIVEPROPAGATORENTRY_INCLUDE_
#define _XAIFBOOSTERDERIVATIVEPROPAGATORENTRY_INCLUDE_
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

#include "xaifBooster/algorithms/InlinableXMLRepresentation/inc/InlinableSubroutineCall.hpp"

using namespace xaifBooster;

namespace xaifBoosterDerivativePropagator { 

  /** 
   * class representing different 
   * versions for y+=[a*x]
   * \todo: The problem with this class is that 
   * it should probably be a union rather than 
   * a base class for the other versions, since 
   * we need to recollect all the information at some 
   * point. The Factor class sort of represents it already.
   */
  class DerivativePropagatorEntry : public XMLPrintable {
  public:

    DerivativePropagatorEntry(const Variable& theTarget);

    ~DerivativePropagatorEntry();

    typedef std::list<const Variable*> VariablePList;

    /** 
     * Elements 'a' and 'x' in a*x
     */
    class Factor { 
    public: 
      /**
       * the kind of factor a 
       * in y+=a*x
       */
      enum FactorKind_E {  ZERO_FACTOR,
			   UNIT_FACTOR,
			   NEGATIVE_UNIT_FACTOR,
			   CONSTANT_FACTOR,
			   VARIABLE_FACTOR, 
			   NOT_SET};

      Factor() : 
	myFactorKind(NOT_SET),
	mySource_p(0) {
      };

      /** 
       * fix it to ZERO_FACTOR
       */
      void setZero(); 
 
      /** 
       * fix it to UNIT_FACTOR
       */
      void setUnit(); 

      /** 
       * fix it to NEGATIVE_UNIT_FACTOR
       */
      void setNegativeUnit(); 

      /** 
       * fix it to CONSTANT_FACTOR
       */
      void setConstant(const Constant& theConstant); 

      /** 
       * fix it to VARIABLE_FACTOR
       */
      void setVariable(const Variable& theVariable); 

      FactorKind_E getKind() const;
      const Variable& getVariable() const;
      const Constant& getConstant() const;
      void  setSource(const Variable& theVariable);
      const Variable& getSource() const;

    private: 
      /**
       * this represents the factor 'a'
       * which can be either variable or constant
       */
      union { 
	const Variable* myVariable_p;
	const Constant* myConstant_p;
      } myFactor;
      
      /** 
       * used also as discriminator for myFactor
       */
      FactorKind_E myFactorKind;
      
      /** 
       * this represents the source 'x'
       */
      const Variable* mySource_p;
    };

    typedef std::list<Factor> FactorList;

    /**
     * fill in the list of all a*x 
     * in y+=a*x
     */
    virtual void getFactors(FactorList& theFactorList) const =0; 

    /**
     * returns y from y+=a*x
     */
    const Variable& getTarget() const;

    virtual bool isIncremental() const { return false;}; 

    std::string debug() const;

    /**
     * checks the target for \p anExpression (intended to be redefined by subclasses)
     */
    virtual bool hasExpression(const Expression& anExpression) const;

    virtual const xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall& asInlinableSubroutineCall() const =0 ;

  private:

    /**
     * no def
     */
    DerivativePropagatorEntry ();

    /**
     * no def
     */
    DerivativePropagatorEntry (const DerivativePropagatorEntry&);

    /**
     * no def
     */
    DerivativePropagatorEntry& operator=(const DerivativePropagatorEntry&);

  protected: 

    /**
     * all versions have a single target
     * but we need this to be a copy
     * \todo: does it really need to be a copy? AL: probably not
     */
    Variable myTarget;

    /**
     * represent this as an InlinableSubroutineCall
     */
    mutable xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall* myInlinableSubroutineCall_p;

    /**
     * helper method for the entries that have a source and a target
     */
    const xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall&
    asSourceTargetInlinableSubroutineCall(const std::string& name, const Variable& theSource) const;

  };
 
}
                                                                     
#endif
