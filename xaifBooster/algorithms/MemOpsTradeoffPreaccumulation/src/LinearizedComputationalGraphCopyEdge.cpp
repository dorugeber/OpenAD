// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/algorithms/MemOpsTradeoffPreaccumulation/inc/LinearizedComputationalGraphCopyEdge.hpp"

namespace MemOpsTradeoffPreaccumulation {

  void LinearizedComputationalGraphCopyEdge::setOriginalRef(const xaifBoosterCrossCountryInterface::LinearizedComputationalGraphEdge& theOriginalEdge){
    originalRef = &theOriginalEdge;
    myCopyReferenceType = TO_ORIGINAL_EDGE;
  }

  void LinearizedComputationalGraphCopyEdge::setJacobianRef(MemOpsTradeoffPreaccumulation::JacobianAccumulationExpressionCopy* theJacobianExpression_pt){
    jacobianRef = theJacobianExpression_pt;
    myCopyReferenceType = TO_INTERNAL_EXPRESSION;
  }

  const xaifBoosterCrossCountryInterface::LinearizedComputationalGraphEdge& LinearizedComputationalGraphCopyEdge::getOriginalRef() const {
    return *originalRef;
  }
  
  MemOpsTradeoffPreaccumulation::JacobianAccumulationExpressionCopy& LinearizedComputationalGraphCopyEdge::getJacobianRef() const {
    return *jacobianRef;
  }

  LinearizedComputationalGraphCopyEdge::EdgeCopyRefType LinearizedComputationalGraphCopyEdge::getRefType() const {
    return myCopyReferenceType;
  }

} //end namespace MemOpsTradeoffPreaccumulation
