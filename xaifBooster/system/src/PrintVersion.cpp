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

#include "xaifBooster/system/inc/PrintVersion.hpp"

namespace xaifBooster { 

  std::string 
  PrintVersion::toString(const PrintVersion_E& aKind)
    throw (PrintingIntException) { 
    std::string returnString;
    switch(aKind) {
    case VIRTUAL:
      returnString="VIRTUAL";
      break;
    case SYSTEM_ONLY: 
      returnString="SYSTEM_ONLY";
      break;
    default: 
      throw PrintingIntException("PrintVersion::toString: unknown value",aKind);
      break;
    } // end switch
    return returnString;
  } // end of std::string PrintVersion::toString

  const PrintVersion::PrintVersion_E
  PrintVersion::fromString(const std::string& aName) { 
    PrintVersion_E returnValue;
    if (aName=="VIRTUAL")
      returnValue=VIRTUAL;
    else if (aName=="SYSTEM_ONLY")
      returnValue=SYSTEM_ONLY;
    else  
      THROW_LOGICEXCEPTION_MACRO("PrintVersion::fromString: unknown value >"
				 << aName.c_str() << "<");
    return returnValue;
  } // end of std::string PrintVersion::fromString
  
} 
