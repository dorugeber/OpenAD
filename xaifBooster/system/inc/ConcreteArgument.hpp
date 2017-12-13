#ifndef _CONCRETEARGUMENT_INCLUDE_
#define _CONCRETEARGUMENT_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/utils/inc/XMLPrintable.hpp"

#include "xaifBooster/system/inc/ObjectWithAnnotation.hpp"
#include "xaifBooster/system/inc/Argument.hpp"
#include "xaifBooster/system/inc/Constant.hpp"
#include "xaifBooster/system/inc/ConcreteArgumentAlgBase.hpp"

namespace xaifBooster { 

  /**
   * this class represents a concrete 
   * argument in a SubroutineCall.
   * Note the restriction that 
   * concrete arguments are always 
   * lvalues or literal constants.
   * The front-end achieves this through 
   * canonicalization.
   */
  class ConcreteArgument: public XMLPrintable, 
			  public ObjectWithAnnotation { 
  public:
    
    ConcreteArgument (unsigned int thePosition,
		      bool makeAlgorithm=true);

    ~ConcreteArgument();

    virtual void printXMLHierarchy(std::ostream& os) const;
    void printXMLHierarchyImpl(std::ostream& os) const;

    std::string debug() const ;

    /**
     * name for this class as represented in XAIF schema
     */
    static const std::string ourXAIFName;

    /**
     * name for member myPosition as represented in XAIF schema
     */
    static const std::string our_myPosition_XAIFName;

    /**
     * get algorithm
     */
    ConcreteArgumentAlgBase& getConcreteArgumentAlgBase() const;
                                                                                
    unsigned int getPosition() const;
    
    bool isArgument() const;

    Argument& getArgument();

    const Argument& getArgument() const ;

    bool isConstant() const;

    Constant& getConstant();

    const Constant& getConstant() const ;

    Constant& makeConstant(const SymbolType::SymbolType_E aType);

    /** 
     * copy
     * the 'deep' argument refers to the AST  
     * i.e. deep=true means 'a[i]' is copied as 'a[i]'
     * while deep=false means 'a[i]' is copied as 'a'
     */
    void copyMyselfInto(ConcreteArgument& theTarget, 
			bool deep=true) const;

  private:
    
    /**
     * no def
     */
    ConcreteArgument();

    /**
     * position of this argument in the argument list of 
     * a subroutine
     */
    unsigned int myPosition;

    enum ConcreteArgumentKind_E{ UNDEFINED_KIND=0,
				 VARIABLE_KIND=1,
				 CONSTANT_KIND=2 };

    /** 
     * the kind of this argument
     * determined by the first call 
     * to the non-const version of 
     * getArgument or getConstant
     */
    ConcreteArgumentKind_E myKind;

    /**
     * the actual concrete argument is a variable
     */
    Argument* myArgument_p;

    /**
     * the actual concrete argument is a constant
     */
    Constant* myConstant_p;

    /**
     * this will be set to point a dynamically instance
     * during construction and deleted during
     * destruction
     */
    ConcreteArgumentAlgBase* myConcreteArgumentAlgBase_p;

  }; // end of class ConcreteArgument

} // end of namespace xaifBooster
                                                                     
#endif
