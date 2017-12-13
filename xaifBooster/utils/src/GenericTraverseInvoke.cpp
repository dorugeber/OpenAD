// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/utils/inc/GenericTraverseInvoke.hpp"
#include "xaifBooster/utils/inc/LogicException.hpp"

namespace xaifBooster { 

  void
  GenericTraverseInvoke::genericTraversal(const GenericAction::GenericAction_E anAction_c) {
    genericInvocation(anAction_c); 
    traverseToChildren(anAction_c);
  } // end of GenericTraverseInvoke::genericTraversal

  void
  GenericTraverseInvoke::genericInvocation(const GenericAction::GenericAction_E anAction_c) { 
    switch(anAction_c) { 
    case GenericAction::ALGORITHM_INIT :
      algorithm_init();
      break; 
    case GenericAction::ALGORITHM_ACTION_1 :
      algorithm_action_1();
      break; 
    case GenericAction::ALGORITHM_ACTION_2 :
      algorithm_action_2();
      break; 
    case GenericAction::ALGORITHM_ACTION_3 :
      algorithm_action_3();
      break; 
    case GenericAction::ALGORITHM_ACTION_4 :
      algorithm_action_4();
      break; 
    case GenericAction::ALGORITHM_ACTION_5 :
      algorithm_action_5();
      break; 
    case GenericAction::ALGORITHM_ACTION_6 :
      algorithm_action_6();
      break; 
    case GenericAction::ALGORITHM_ACTION_7 :
      algorithm_action_7();
      break; 
    case GenericAction::ALGORITHM_ACTION_8 :
      algorithm_action_8();
      break; 
    default:
      THROW_LOGICEXCEPTION_MACRO("GenericTraverseInvoke::genericInvocation: unknown action");
      break; 
    } // end switch 
  } // end of GenericTraverseInvoke::genericInvocation

}

