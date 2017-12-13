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
#include "xaifBooster/system/inc/PartialDerivativeKind.hpp"

namespace xaifBooster { 

  const std::string PartialDerivativeKind::our_attribute_XAIFName("kind");

  std::string 
  PartialDerivativeKind::toString(const PartialDerivativeKind_E& aKind)
    throw (PrintingIntException) { 
    std::string returnString;
    switch(aKind) {
    case NOT_SET: 
      returnString="not_set";
      break;
    case PASSIVE:
      returnString="passive";
      break;
    case LINEAR_ONE: 
      returnString="linear_one";
      break;
    case LINEAR_MINUS_ONE: 
      returnString="linear_minus_one";
      break;
    case LINEAR: 
      returnString="linear";
      break;
    case NONLINEAR: 
      returnString="nonlinear";
      break;
    default: 
      throw PrintingIntException("PartialDerivativeKind::toString: unknown value",aKind);
      break;
    } // end switch
    return returnString;
  } // end of std::string PartialDerivativeKind::toString

  const PartialDerivativeKind::PartialDerivativeKind_E
  PartialDerivativeKind::fromString(const std::string& aName) { 
    PartialDerivativeKind_E returnValue;
    if (aName=="not_set")
      returnValue=NOT_SET;
    else if (aName=="passive")
      returnValue=PASSIVE;
    else if (aName=="linear_one")
      returnValue=LINEAR_ONE;
    else if (aName=="linear_minus_one")
      returnValue=LINEAR_MINUS_ONE;
    else if (aName=="linear")
      returnValue=LINEAR;
    else if (aName=="nonlinear")
      returnValue=NONLINEAR;
    else  
      THROW_LOGICEXCEPTION_MACRO("PartialDerivativeKind::fromString: unknown value >"
				 << aName.c_str() << "<");
    return returnValue;
  } // end of std::string PartialDerivativeKind::fromString

  PartialDerivativeKind::PartialDerivativeKind_E 
  PartialDerivativeKind::leastDependent(PartialDerivativeKind::PartialDerivativeKind_E aKind,
					PartialDerivativeKind::PartialDerivativeKind_E anotherKind) { 
    PartialDerivativeKind_E theResult;
    switch (aKind) { 
    case PASSIVE:
      theResult=PASSIVE;
    case LINEAR_ONE: 
      switch (anotherKind) { 
      case PASSIVE:
	theResult=PASSIVE;
      case LINEAR_MINUS_ONE: 
	THROW_LOGICEXCEPTION_MACRO("PartialDerivativeKind::leastDependent: cannot compare " 
				   << toString(LINEAR_ONE).c_str()
				   << " and " 
				   << toString(LINEAR_MINUS_ONE).c_str());
	break;
      case LINEAR_ONE: 
      case LINEAR: 
      case NONLINEAR: 
	theResult=LINEAR_ONE;
	break;
      default: 
	throw PrintingIntException("PartialDerivativeKind::leastDependent: unknown value",anotherKind);
	break;
      } 
      break;
    case LINEAR_MINUS_ONE: 
      switch (anotherKind) { 
      case PASSIVE:
	theResult=PASSIVE;
      case LINEAR_ONE: 
	THROW_LOGICEXCEPTION_MACRO("PartialDerivativeKind::leastDependent: cannot compare " 
				   << toString(LINEAR_ONE).c_str()
				   << " and " 
				   << toString(LINEAR_MINUS_ONE).c_str());
	break;
      case LINEAR_MINUS_ONE: 
      case LINEAR: 
      case NONLINEAR: 
	theResult=LINEAR_MINUS_ONE;
	break;
      default: 
	throw PrintingIntException("PartialDerivativeKind::leastDependent: unknown value",anotherKind);
	break;
      } 
      break;
    case LINEAR: 
      switch (anotherKind) { 
      case PASSIVE:
      case LINEAR_ONE: 
      case LINEAR_MINUS_ONE: 
	theResult=anotherKind;
	break;
      case LINEAR: 
      case NONLINEAR: 
	theResult=LINEAR;
	break;
      default: 
	throw PrintingIntException("PartialDerivativeKind::leastDependent: unknown value",anotherKind);
	break;
      } 
      break;
    case NONLINEAR: 
      theResult=anotherKind;
      break;
    default: 
      throw PrintingIntException("PartialDerivativeKind::toString: unknown value",aKind);
      break;
    } 
    return theResult;
  } 

  
} // end of namespace xaifBooster
