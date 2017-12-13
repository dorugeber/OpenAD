// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include <algorithm>
#include "xaifBooster/utils/inc/FrontEndDecorations.hpp"
#include "xaifBooster/utils/inc/LogicException.hpp"

namespace xaifBooster { 

  std::string 
  FrontEndDecorations::toString(const FrontEndDecorations_E& aGroup)
    throw (PrintingIntException) { 
    std::string returnString;
    switch(aGroup) {
    case OPEN64_STYLE:
      returnString="OPEN64_STYLE";
      break;
    case NO_STYLE:
      returnString="NO_STYLE";
      break;
    default: 
      throw PrintingIntException("FrontEndDecorations::toString: unknown value",aGroup);
      break;
    } // end switch
    return returnString;
  } // end of std::string FrontEndDecorations::toString

  const FrontEndDecorations::FrontEndDecorations_E
  FrontEndDecorations::fromString(const std::string& aName) {
    std::string  capitalName;
    capitalName.reserve(aName.size());
    std::transform (aName.begin(), aName.end(), capitalName.begin(), toupper); 
    FrontEndDecorations_E returnValue;
    if (capitalName=="OPEN64_STYLE")
      returnValue=OPEN64_STYLE;
    else if (aName=="NO_STYLE")
      returnValue=NO_STYLE;
    else  
      THROW_LOGICEXCEPTION_MACRO("FrontEndDecorations::fromString: unknown value >"
			   << aName.c_str() << "<");
    return returnValue;
  } // end of std::string FrontEndDecorations::fromString

  std::string 
  FrontEndDecorations::printAll() throw (PrintingIntException) {
    std::ostringstream os;
    os << "OPEN64_STYLE,"
       << "NO_STYLE"
       << std::ends; 
    return os.str();
  }

} // end of namespace xaifBooster
