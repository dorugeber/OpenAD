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
#include "xaifBooster/system/inc/ControlFlowGraphVertexKind.hpp"

namespace xaifBooster { 

  std::string 
  ControlFlowGraphVertexKind::toString(const ControlFlowGraphVertexKind_E& aKind)
    throw (PrintingIntException) { 
    std::string returnString;
    switch(aKind) {
    case ENTRY_VKIND:
      returnString="entry";
      break;
    case EXIT_VKIND:
      returnString="exit";
      break;
    case BASICBLOCK_VKIND:
      returnString="basicblock";
      break;
    case ENDBRANCH_VKIND:
      returnString="endbranch";
      break;
    case ENDLOOP_VKIND:
      returnString="endloop";
      break;
    case IF_VKIND:
      returnString="if";
      break;
    case PRELOOP_VKIND:
      returnString="preloop";
      break;
    case POSTLOOP_VKIND:
      returnString="postloop";
      break;
    case FORLOOP_VKIND:
      returnString="forloop";
      break;
    case BRANCH_VKIND:
      returnString="branch";
      break;
    case LABEL_VKIND:
      returnString="label";
      break;
    case GOTO_VKIND:
      returnString="goto";
      break;
    default: 
      throw PrintingIntException("ControlFlowGraphVertexKind::toString: unknown value",aKind);
      break;
    } // end switch
    return returnString;
  } // end of std::string ControlFlowGraphVertexKind::toString

  const ControlFlowGraphVertexKind::ControlFlowGraphVertexKind_E
  ControlFlowGraphVertexKind::fromString(const std::string& aName) { 
    ControlFlowGraphVertexKind_E returnValue;
    if (aName=="entry")
      returnValue=ENTRY_VKIND;
    else if (aName=="exit")
      returnValue=EXIT_VKIND;
    else if (aName=="basicblock")
      returnValue=BASICBLOCK_VKIND;
    else if (aName=="endbranch")
      returnValue=ENDBRANCH_VKIND;
    else if (aName=="endloop")
      returnValue=ENDLOOP_VKIND;
    else if (aName=="if")
      returnValue=IF_VKIND;
    else if (aName=="preloop")
      returnValue=PRELOOP_VKIND;
    else if (aName=="postloop")
      returnValue=POSTLOOP_VKIND;
    else if (aName=="forloop")
      returnValue=FORLOOP_VKIND;
    else if (aName=="branch")
      returnValue=BRANCH_VKIND;
    else if (aName=="label")
      returnValue=LABEL_VKIND;
    else if (aName=="goto")
      returnValue=GOTO_VKIND;
    else  
      THROW_LOGICEXCEPTION_MACRO("ControlFlowGraphVertexKind::fromString: unknown value >"
			   << aName.c_str() << "<");
    return returnValue;
  } // end of std::string ControlFlowGraphVertexKind::fromString
  
} // end of namespace xaifBooster
