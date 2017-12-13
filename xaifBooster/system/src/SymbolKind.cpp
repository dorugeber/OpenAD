// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/utils/inc/LogicException.hpp"
#include "xaifBooster/system/inc/SymbolKind.hpp"

namespace xaifBooster { 

  const std::string SymbolKind::our_attribute_XAIFName("kind");

  std::string 
  SymbolKind::toString(const SymbolKind_E& aKind)
    throw (PrintingIntException) { 
    std::string returnString;
    switch(aKind) {
    case VARIABLE:
      returnString="variable";
      break;
    case SUBROUTINE: 
      returnString="subroutine";
      break;
    case POINTER:
      returnString="pointer";
      break;
    default: 
      throw PrintingIntException("SymbolKind::toString: unknown value",aKind);
      break;
    } // end switch
    return returnString;
  } // end of std::string SymbolKind::toString

  const SymbolKind::SymbolKind_E
  SymbolKind::fromString(const std::string& aName) { 
    SymbolKind_E returnValue;
    if (aName=="variable")
      returnValue=VARIABLE;
    else if (aName=="subroutine")
      returnValue=SUBROUTINE;
    else if (aName=="pointer")
      returnValue=POINTER;
    else  
      THROW_LOGICEXCEPTION_MACRO("SymbolKind::fromString: unknown value >"
			   << aName.c_str() << "<");
    return returnValue;
  } // end of std::string SymbolKind::fromString
  
} // end of namespace xaifBooster
