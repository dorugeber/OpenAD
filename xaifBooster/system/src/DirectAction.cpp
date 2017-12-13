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
#include "xaifBooster/system/inc/DirectAction.hpp"

namespace xaifBooster { 

  const std::string DirectAction::ourXAIFName("xaifnii:DirectAction");
  const std::string DirectAction::our_myDerivAction_XAIFName("derivAction");

  DirectAction::DirectAction() { 
  } 

  DirectAction::~DirectAction() { 
  } 

  void DirectAction::setDerivAction(const std::string& aDerivAction) { 
    if (myDerivAction!="")
      THROW_LOGICEXCEPTION_MACRO("DirectAction::setDerivAction: already set");
    myDerivAction=aDerivAction;
  } 

  const std::string& DirectAction::getDerivAction() const { 
    if (myDerivAction=="")
      THROW_LOGICEXCEPTION_MACRO("DirectAction::getDerivAction: not set");
    return myDerivAction;
  } 

} 
