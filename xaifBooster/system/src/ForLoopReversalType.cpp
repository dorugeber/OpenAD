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
#include "xaifBooster/system/inc/ForLoopReversalType.hpp"

namespace xaifBooster { 
  
  const std::string ForLoopReversalType::our_attribute_XAIFName("reversal");

  static const ForLoopReversalType::ForLoopReversalType_E typeList[]={
      ForLoopReversalType::EXPLICIT,
      ForLoopReversalType::ANONYMOUS,
      ForLoopReversalType::HEURISTIC
  };

  const ForLoopReversalType::TypeList ForLoopReversalType::ourTypeList(typeList,typeList+sizeof(typeList)/sizeof(ForLoopReversalType::ForLoopReversalType_E));

  std::string ForLoopReversalType::toString(const ForLoopReversalType_E& aType)
    throw (PrintingIntException) { 
    std::string returnString;
    switch(aType) {
    case EXPLICIT:
      returnString="explicit";
      break;
    case ANONYMOUS: 
      returnString="anonymous";
      break;
    case HEURISTIC:
      returnString="heuristic";
      break;
    default: 
      throw PrintingIntException("ForLoopReversalType::toString: unknown value",aType);
      break;
    } // end switch
    return returnString;
  } // end of std::string ForLoopReversalType::toString

  const ForLoopReversalType::ForLoopReversalType_E
  ForLoopReversalType::fromString(const std::string& aName) { 
    ForLoopReversalType_E returnValue;
    if (aName=="explicit")
      returnValue=EXPLICIT;
    else if (aName=="anonymous")
      returnValue=ANONYMOUS;
    else if (aName=="heuristic")
       returnValue=HEURISTIC;
    else  
      THROW_LOGICEXCEPTION_MACRO("ForLoopReversalType::fromString: unknown value >"
				 << aName.c_str() << "<");
    return returnValue;
  } // end of std::string ForLoopReversalType::fromString
  
} // end of namespace xaifBooster
