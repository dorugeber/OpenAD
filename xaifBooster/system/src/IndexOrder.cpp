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
#include "xaifBooster/system/inc/IndexOrder.hpp"

namespace xaifBooster { 
  
  std::string IndexOrder::toString(const IndexOrder_E& anOrder)
    throw (PrintingIntException) { 
    std::string returnString;
    switch(anOrder) {
    case ROWMAJOR:
      returnString="rowmajor";
      break;
    case COLUMNMAJOR:
      returnString="columnmajor";
      break;
    default: 
      throw PrintingIntException("IndexOrder::toString: unknown value",anOrder);
      break;
    } // end switch
    return returnString;
  } // end of std::string IndexOrder::toString

  const IndexOrder::IndexOrder_E
  IndexOrder::fromString(const std::string& aName) { 
    IndexOrder_E returnValue;
    if (aName=="rowmajor")
      returnValue=ROWMAJOR;
    else if (aName=="columnmajor")
      returnValue=COLUMNMAJOR;
    else  
      THROW_LOGICEXCEPTION_MACRO("IndexOrder::fromString: unknown value >"
			   << aName.c_str() << "<");
    return returnValue;
  } // end of std::string IndexOrder::fromString
  
} // end of namespace xaifBooster
