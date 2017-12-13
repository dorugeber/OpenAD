// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include <sstream>
#include "xaifBooster/utils/inc/PrintManager.hpp"
#include "xaifBooster/utils/inc/LogicException.hpp"
#include "xaifBooster/system/inc/SymbolReference.hpp"

namespace xaifBooster { 

  SymbolReference::SymbolReference (const Symbol& theSymbol,
				    const Scope& theScope) :
    mySymbol_r(theSymbol) ,
    myScope_r(theScope){
  }

  const Symbol& 
  SymbolReference::getSymbol() const { 
    return mySymbol_r;
  } // end of SymbolReference::getSymbol

  const Scope& 
  SymbolReference::getScope() const { 
    return myScope_r;
  } // end of SymbolReference::getScope

  std::string SymbolReference::debug () const { 
    std::ostringstream out;
    out << "SymbolReference[" << this 
	<< ",mySymbol_r=" << mySymbol_r.debug().c_str()
	<< ",myScope_r=" << myScope_r.debug().c_str()
	<< "]" << std::ends;  
    return out.str();
  } // end of Symbol::debug

  bool SymbolReference::refersToSameSymbolAs(const SymbolReference& anotherSymbolReference) const { 
    // we must point to the same scope instance and symbol within the scope. 
    // in practice no Symbol instance is shared between scopes so it 
    // should be sufficient to test for same symbol instance but anyway....
    return (&(getScope())==&(anotherSymbolReference.getScope()) && &(getSymbol())==&(anotherSymbolReference.getSymbol()));
  } 

} // end of namespace xaifBooster 
