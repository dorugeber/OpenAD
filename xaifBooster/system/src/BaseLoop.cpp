// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include <sstream>
#include "xaifBooster/system/inc/BaseLoop.hpp"

namespace xaifBooster { 

  BaseLoop::BaseLoop() :
        myCondition_p(0){
  }

  BaseLoop::~BaseLoop() { }

  std::string BaseLoop::debug () const {
    std::ostringstream out;
    out << "BaseLoop[" << this
                     << ControlFlowGraphVertex::debug().c_str()
                     << ObjectWithLineNumber::debug().c_str()
                     << "]" << std::ends;
    return out.str();
  } // end of BaseLoop::debug

  Condition& BaseLoop::makeCondition() {
    if (myCondition_p) {
      THROW_LOGICEXCEPTION_MACRO("BaseLoop::makeCondition: already set");
    }
    myCondition_p=new Condition();
    return *myCondition_p;
  }

  Condition& BaseLoop::getCondition() {
    if (!myCondition_p) {
      THROW_LOGICEXCEPTION_MACRO("BaseLoop::getCondition: not set");
    }
    return *myCondition_p;
  }

  const Condition& BaseLoop::getCondition() const {
    if (!myCondition_p) {
      THROW_LOGICEXCEPTION_MACRO("BaseLoop::getCondition: not set");
    }
    return *myCondition_p;
  }

}
