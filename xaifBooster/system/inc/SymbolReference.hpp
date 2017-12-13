#ifndef _SYMBOLREFERENCE_INCLUDE_
#define _SYMBOLREFERENCE_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/utils/inc/Debuggable.hpp"

#include "xaifBooster/system/inc/Symbol.hpp"
#include "xaifBooster/system/inc/Scope.hpp"

namespace xaifBooster { 

  class SymbolReference {
  public:

    SymbolReference (const Symbol& theSymbol,
		     const Scope& theScope);

    virtual ~SymbolReference(){};

    std::string debug() const ;

    const Symbol& getSymbol() const;

    const Scope& getScope() const;
    
    bool refersToSameSymbolAs(const SymbolReference& anotherSymbolReference) const;

  protected:
    
    /**
     * no def
     */
    SymbolReference ();

    const Symbol& mySymbol_r;

    const Scope& myScope_r;
    
  };
 
} // end of namespace xaifBooster
                                                                     
#endif
