#ifndef _GRAPHCORRELATIONS_INCLUDE_
#define _GRAPHCORRELATIONS_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/algorithms/CrossCountryInterface/inc/LinearizedComputationalGraph.hpp"
#include "xaifBooster/algorithms/CrossCountryInterface/inc/JacobianAccumulationExpressionList.hpp"

namespace xaifBoosterCrossCountryInterface {

  /** 
   * a vertex in the remainder graph 
   * should have a counter part in the original
   * linearized computational graph
   */
  struct VertexCorrelationEntry {
    const LinearizedComputationalGraphVertex* myOriginalVertex_p;
    LinearizedComputationalGraphVertex* myRemainderVertex_p;
    VertexCorrelationEntry(): 
      myOriginalVertex_p(0),
      myRemainderVertex_p(0) {
    };
  };
  
  /** 
   * an edge in the remainder graph 
   * should reference either an edge in the original
   * linearized computational graph
   * or a top level vertex in an JacobianAccumulationExpression
   */
  struct EdgeCorrelationEntry {
    
    union { 
      const LinearizedComputationalGraphEdge* myOriginalEdge_p;
      JacobianAccumulationExpressionVertex* myJAEVertex_p;
    } myEliminationReference;

    enum RemainderEdgeType_E {LCG_EDGE, 
			      JAE_VERT,
			      UNDEFINED};
    /** 
     * the discriminator for myEliminationReference
     */
    RemainderEdgeType_E myType;
    LinearizedComputationalGraphEdge* myRemainderGraphEdge_p;
    EdgeCorrelationEntry() : 
      myType(UNDEFINED),
      myRemainderGraphEdge_p(0) {
    };
  };
  
  typedef std::list<VertexCorrelationEntry> VertexCorrelationList;
  typedef std::list<EdgeCorrelationEntry> EdgeCorrelationList;
  
} // namespace xaifBoosterCrossCountryInterface

#endif  
