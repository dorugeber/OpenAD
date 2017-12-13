// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/algorithms/MemOpsTradeoffPreaccumulation/inc/JacobianAccumulationExpressionCopy.hpp"

namespace MemOpsTradeoffPreaccumulation {

  void JacobianAccumulationExpressionCopy::setMaximal(const xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionVertex& theMaximal){
    myMaximal = const_cast<xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionVertex*>(&theMaximal);
  }

  xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionVertex& JacobianAccumulationExpressionCopy::getMaximal() const{
    return *myMaximal;
  } 

}//end of namespace MemOpsTradeoffPreaccumulation
