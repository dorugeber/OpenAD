#ifndef _ARGUMENTSYMBOLREFERENCE_INCLUDE_
#define _ARGUMENTSYMBOLREFERENCE_INCLUDE_
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
#include "xaifBooster/utils/inc/GenericTraverseInvoke.hpp"
#include "xaifBooster/system/inc/SymbolReference.hpp"
#include "xaifBooster/system/inc/ObjectWithAnnotation.hpp"
#include "xaifBooster/system/inc/IntentType.hpp"
#include "xaifBooster/system/inc/ArgumentSymbolReferenceAlgBase.hpp"


namespace xaifBooster { 
  class ArgumentSymbolReference: public SymbolReference, 
				 public XMLPrintable, 
				 public ObjectWithAnnotation,
				 public GenericTraverseInvoke { 
  public:
    
    ArgumentSymbolReference (const Symbol& theSymbol,
			     const Scope& theScope,
			     unsigned int thePosition,
			     IntentType::IntentType_E theIntent,
			     bool makeAlgorithm=true);

    ~ArgumentSymbolReference();

    /** 
     * algorithm access where the ArgumentSymbolReference may 
     * be const but in difference to the 
     * internal representation (wich is always 
     * const for the algorithms) the algorithm 
     * instances will always be modifiable.
     */
    ArgumentSymbolReferenceAlgBase& getArgumentSymbolReferenceAlgBase()const;

    void printXMLHierarchy(std::ostream& os) const;

    virtual void traverseToChildren(const GenericAction::GenericAction_E anAction_c);

    /**
     * actual implementation for printing xaif
     */
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
     * name for symbol id used for SymbolReference member as represented in XAIF schema
     */
    static const std::string our_symbolId_XAIFName;

    /**
     * name for symbol id used for SymbolReference member as represented in XAIF schema
     */
    static const std::string our_scopeId_XAIFName;

    /**
     * name for member myIntent as represented in XAIF schema
     */
    static const std::string our_myIntent_XAIFName;

    unsigned int getPosition() const;
    
    IntentType::IntentType_E getIntent() const;

  private:
    
    /**
     * no def
     */
    ArgumentSymbolReference();

    /**
     * position of this argument in the argument list of 
     * a subroutine
     */
    unsigned int myPosition;

    /**
     * the flag indicating the intent of this argument
     */
    const IntentType::IntentType_E myIntent;

    /** 
     * this will be set to point a dynamically instance
     * during construction and deleted during 
     * destruction
     */
    ArgumentSymbolReferenceAlgBase* myArgumentSymbolReferenceAlgBase_p;

  }; // end of class ArgumentSymbolReference

} // end of namespace xaifBooster
                                                                     
#endif
