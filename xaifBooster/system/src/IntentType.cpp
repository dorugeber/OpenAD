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
#include "xaifBooster/system/inc/IntentType.hpp"

namespace xaifBooster { 
  
  const std::string IntentType::our_attribute_XAIFName("type");

  std::string IntentType::toString(const IntentType_E& aType)
    throw (PrintingIntException) { 
    std::string returnString;
    switch(aType) {
    case IN_ITYPE:
      returnString="in";
      break;
    case OUT_ITYPE: 
      returnString="out";
      break;
    case INOUT_ITYPE:
      returnString="inout";
      break;
    default: 
      throw PrintingIntException("IntentType::toString: unknown value",aType);
      break;
    } // end switch
    return returnString;
  } // end of std::string IntentType::toString

  const IntentType::IntentType_E
  IntentType::fromString(const std::string& aName) { 
    IntentType_E returnValue;
    if (aName=="in")
      returnValue=IN_ITYPE;
    else if (aName=="out")
      returnValue=OUT_ITYPE;
    else if (aName=="inout")
      returnValue=INOUT_ITYPE;
    else  
      THROW_LOGICEXCEPTION_MACRO("IntentType::fromString: unknown value >"
				 << aName.c_str() << "<");
    return returnValue;
  } // end of std::string IntentType::fromString
  
} // end of namespace xaifBooster
