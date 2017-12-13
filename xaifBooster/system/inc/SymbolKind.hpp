#ifndef _SYMBOLKIND_INCLUDE_
#define _SYMBOLKIND_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include <string>
#include "xaifBooster/utils/inc/PrintingIntException.hpp"

namespace xaifBooster { 
  class SymbolKind { 
public:
    enum SymbolKind_E {VARIABLE,
		       SUBROUTINE,
                       POINTER};
    static std::string toString(const SymbolKind_E& aKind) throw (PrintingIntException);

    static const SymbolKind_E fromString(const std::string& aName);
    
    static const std::string our_attribute_XAIFName;

  }; // end of class SymbolKind

} // end of namespace xaifBooster
                                                                     
#endif
