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
#include "xaifBooster/system/inc/ShapeChange.hpp"

namespace xaifBooster { 

  const std::string ShapeChange::our_attribute_XAIFName("kind");

  std::string 
  ShapeChange::toString(const ShapeChange_E& aKind)
    throw (PrintingIntException) { 
    std::string returnString;
    switch(aKind) {
    case NO_CHANGE: 
      returnString="no_change";
      break;
    case RANK:
      returnString="rank";
      break;
    case SCALAR:
      returnString="scalar";
      break;
    default: 
      throw PrintingIntException("ShapeChange::toString: unknown value",aKind);
      break;
    } // end switch
    return returnString;
  } // end of std::string ShapeChange::toString

  const ShapeChange::ShapeChange_E
  ShapeChange::fromString(const std::string& aName) { 
    ShapeChange_E returnValue;
    if (aName=="no_change")
      returnValue=NO_CHANGE;
    else if (aName=="rank")
      returnValue=RANK;
    else if (aName=="scalar")
      returnValue=SCALAR;
    else  
      THROW_LOGICEXCEPTION_MACRO("ShapeChange::fromString: unknown value >"
				 << aName.c_str() << "<");
    return returnValue;
  } // end of std::string ShapeChange::fromString

  
} // end of namespace xaifBooster
