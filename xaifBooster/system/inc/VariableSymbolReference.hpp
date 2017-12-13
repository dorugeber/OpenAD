#ifndef _BASEVARIABLESYMBOLREFERENCE_INCLUDE_
#define _BASEVARIABLESYMBOLREFERENCE_INCLUDE_
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
#include "xaifBooster/system/inc/VariableVertex.hpp"
#include "xaifBooster/system/inc/SymbolReference.hpp"

namespace xaifBooster { 

  class VariableSymbolReference : public VariableVertex,
				  public SymbolReference,
				  public ObjectWithAnnotation {
  public:

    VariableSymbolReference (const Symbol& theSymbol,
			     const Scope& theScope);

    ~VariableSymbolReference(){};

    void printXMLHierarchy(std::ostream& os) const;

    std::string debug() const ;

    static const std::string ourXAIFName;

    static const std::string our_scopeId_XAIFName;

    static const std::string our_myId_XAIFName;

    virtual std::string equivalenceSignature() const;

    virtual VariableVertex& createCopyOfMyself() const ;

    virtual bool isSymbol() const;

  private:
    
    /**
     * no def
     */
    VariableSymbolReference();

  };
 
} // end of namespace xaifBooster
                                                                     
#endif
