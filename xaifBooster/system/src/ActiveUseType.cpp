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
#include "xaifBooster/system/inc/ActiveUseType.hpp"

namespace xaifBooster { 

  const std::string ActiveUseType::our_attribute_XAIFName("active");

  std::string 
  ActiveUseType::toString(const ActiveUseType_E& aKind)
    throw (PrintingIntException) { 
    std::string returnString;
    switch(aKind) {
    case ACTIVEUSE:
      returnString="true";
      break;
    case PASSIVEUSE: 
      returnString="false";
      break;
    case UNDEFINEDUSE: 
      returnString="undefined";
      break;
    default: 
      throw PrintingIntException("ActiveUseType::toString: unknown value",aKind);
      break;
    } // end switch
    return returnString;
  } // end of std::string ActiveUseType::toString

  const ActiveUseType::ActiveUseType_E
  ActiveUseType::fromString(const std::string& aName) { 
    ActiveUseType_E returnValue;
    if (aName=="true")
      returnValue=ACTIVEUSE;
    else if (aName=="false")
      returnValue=PASSIVEUSE;
    else if (aName=="undefined")
      returnValue=UNDEFINEDUSE;
    else  
      THROW_LOGICEXCEPTION_MACRO("ActiveUseType::fromString: unknown value >"
			   << aName.c_str() << "<");
    return returnValue;
  } // end of std::string ActiveUseType::fromString
  
} // end of namespace xaifBooster
