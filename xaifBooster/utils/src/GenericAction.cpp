// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/utils/inc/GenericAction.hpp"
#include "xaifBooster/utils/inc/LogicException.hpp"

namespace xaifBooster { 
  
  const std::string GenericAction::our_attribute_XAIFName("shape");

  std::string GenericAction::toString(const GenericAction_E& anAction)
    throw (PrintingIntException) { 
    std::string returnString;
    switch(anAction) {
    case ALGORITHM_INIT:
      returnString="algorithm_init";
      break;
    case ALGORITHM_ACTION_1:
      returnString="algorithm_action_1";
      break;
    case ALGORITHM_ACTION_2:
      returnString="algorithm_action_2";
      break;
    case ALGORITHM_ACTION_3:
      returnString="algorithm_action_3";
      break;
    case ALGORITHM_ACTION_4:
      returnString="algorithm_action_4";
      break;
    case ALGORITHM_ACTION_5:
      returnString="algorithm_action_5";
      break;
    case ALGORITHM_ACTION_6:
      returnString="algorithm_action_6";
      break;
    case ALGORITHM_ACTION_7:
      returnString="algorithm_action_7";
      break;
    case ALGORITHM_ACTION_8:
      returnString="algorithm_action_8";
      break;
    default: 
      throw PrintingIntException("GenericAction::toString: unknown value",anAction);
      break;
    } // end switch
    return returnString;
  } // end of std::string GenericAction::toString

  const GenericAction::GenericAction_E
  GenericAction::fromString(const std::string& aName) { 
    GenericAction_E returnValue;
    if (aName=="algorithm_init")
      returnValue=ALGORITHM_INIT;
    else if (aName=="algorithm_action_1")
      returnValue=ALGORITHM_ACTION_1;
    else if (aName=="algorithm_action_2")
      returnValue=ALGORITHM_ACTION_2;
    else if (aName=="algorithm_action_3")
      returnValue=ALGORITHM_ACTION_3;
    else if (aName=="algorithm_action_4")
      returnValue=ALGORITHM_ACTION_4;
    else if (aName=="algorithm_action_5")
      returnValue=ALGORITHM_ACTION_5;
    else if (aName=="algorithm_action_6")
      returnValue=ALGORITHM_ACTION_6;
    else if (aName=="algorithm_action_7")
      returnValue=ALGORITHM_ACTION_7;
    else if (aName=="algorithm_action_8")
      returnValue=ALGORITHM_ACTION_8;
    else  
      THROW_LOGICEXCEPTION_MACRO("GenericAction::fromString: unknown value >" << aName.c_str() << "<");
    return returnValue;
  } // end of std::string GenericAction::fromString
  
} // end of namespace xaifBooster
