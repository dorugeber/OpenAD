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
#include "xaifBooster/algorithms/ControlFlowReversal/inc/ForLoopDirection.hpp"

namespace xaifBoosterControlFlowReversal { 
  
  const std::string ForLoopDirection::our_attribute_XAIFName("reversal");

  std::string ForLoopDirection::toString(const ForLoopDirection_E& aType)
    throw (PrintingIntException) { 
    std::string returnString;
    switch(aType) {
    case COUNT_UP:
      returnString="up";
      break;
    case COUNT_DOWN: 
      returnString="down";
      break;
    case COUNT_UNDECIDED: 
      returnString="undecided";
      break;
    default: 
      throw PrintingIntException("ForLoopDirection::toString: unknown value",aType);
      break;
    } // end switch
    return returnString;
  }

  const ForLoopDirection::ForLoopDirection_E
  ForLoopDirection::fromString(const std::string& aName) { 
    ForLoopDirection_E returnValue;
    if (aName=="up")
      returnValue=COUNT_UP;
    else if (aName=="down")
      returnValue=COUNT_DOWN;
    else if (aName=="undecided")
      returnValue=COUNT_UNDECIDED;
    else  
      THROW_LOGICEXCEPTION_MACRO("ForLoopDirection::fromString: unknown value >"
				 << aName.c_str() << "<");
    return returnValue;
  } 
  
} 
