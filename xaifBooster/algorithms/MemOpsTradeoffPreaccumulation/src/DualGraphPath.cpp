// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/algorithms/MemOpsTradeoffPreaccumulation/inc/DualGraphPath.hpp"

namespace MemOpsTradeoffPreaccumulation {

  DualGraphPath::DualGraphPath() {
    complete = false;
  }

  void DualGraphPath::setComplete(){
    complete = true;
  }

  bool DualGraphPath::isComplete() const {
    return complete;
  }

} 
