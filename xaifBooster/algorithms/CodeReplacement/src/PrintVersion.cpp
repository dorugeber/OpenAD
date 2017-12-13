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

#include "xaifBooster/algorithms/CodeReplacement/inc/PrintVersion.hpp"

namespace xaifBoosterCodeReplacement { 

  std::string 
  PrintVersion::toString(const PrintVersion_E& aKind)
    throw (PrintingIntException) { 
    std::string returnString;
    switch(aKind) {
    case ORIGINAL:
      returnString="ORIGINAL";
      break;
    case AUGMENTED: 
      returnString="AUGMENTED";
      break;
    case ADJOINT: 
      returnString="ADJOINT";
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
    if (aName=="ORIGINAL")
      returnValue=ORIGINAL;
    else if (aName=="AUGMENTED")
      returnValue=AUGMENTED;
    else if (aName=="ADJOINT")
      returnValue=ADJOINT;
    else  
      THROW_LOGICEXCEPTION_MACRO("PrintVersion::fromString: unknown value >"
				 << aName.c_str() << "<");
    return returnValue;
  } // end of std::string PrintVersion::fromString
  
} 
