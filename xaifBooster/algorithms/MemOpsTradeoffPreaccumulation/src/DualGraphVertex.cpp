// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/algorithms/MemOpsTradeoffPreaccumulation/inc/DualGraphVertex.hpp"

using namespace xaifBoosterCrossCountryInterface;

namespace MemOpsTradeoffPreaccumulation {

  void DualGraphVertex::setOriginalRef(const LinearizedComputationalGraphEdge& theOriginalEdge){
    originalRef = &theOriginalEdge;
    myDualReferenceType = TO_ORIGINAL_EDGE;
  }

  void DualGraphVertex::setAssumedInEdgeRef(const LinearizedComputationalGraphVertex& theAssumedInEdgeTarget){
    assumedRef = &theAssumedInEdgeTarget;
    myDualReferenceType = TO_ASSUMED_INEDGE;
  }

  void DualGraphVertex::setAssumedOutEdgeRef(const LinearizedComputationalGraphVertex& theAssumedOutEdgeSource){
    assumedRef = &theAssumedOutEdgeSource;
    myDualReferenceType = TO_ASSUMED_OUTEDGE;
  }

  void DualGraphVertex::setJacobianRef(MemOpsTradeoffPreaccumulation::JacobianAccumulationExpressionCopy* theJacobianExpression_pt){
    jacobianRef = theJacobianExpression_pt;
    myDualReferenceType = TO_INTERNAL_EXPRESSION;
  }

  const LinearizedComputationalGraphEdge& DualGraphVertex::getOriginalRef() const {
    return *originalRef;
  }
  
  const LinearizedComputationalGraphVertex& DualGraphVertex::getAssumedRef() const {
    return *assumedRef;
  }

  DualGraphVertex::DualVertexRefType DualGraphVertex::getRefType() const {
    return myDualReferenceType;
  }

  MemOpsTradeoffPreaccumulation::JacobianAccumulationExpressionCopy& DualGraphVertex::getJacobianRef() const {
    return *jacobianRef;
  }

} //end namespace
