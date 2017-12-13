#ifndef _DUALGRAPH_INCLUDE_
#define _DUALGRAPH_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/boostWrapper/inc/GraphWrapper.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/BasicBlockAlg.hpp"
#include "xaifBooster/algorithms/MemOpsTradeoffPreaccumulation/inc/DualGraphEdge.hpp"
#include "xaifBooster/algorithms/MemOpsTradeoffPreaccumulation/inc/DualGraphVertex.hpp"
#include "xaifBooster/algorithms/MemOpsTradeoffPreaccumulation/inc/DualGraphPath.hpp"
#include "xaifBooster/algorithms/MemOpsTradeoffPreaccumulation/inc/JacobianAccumulationExpressionCopy.hpp"

using namespace MemOpsTradeoffPreaccumulation;

namespace MemOpsTradeoffPreaccumulation { 

  class DualGraph : public GraphWrapper<DualGraphVertex, DualGraphEdge>{

  public:

    unsigned int absum, opsum;

    DualGraph(const LinearizedComputationalGraph& theOriginal);
    ~DualGraph();

    typedef std::list<DualGraphEdge*> FacePointerList;
    typedef std::list<DualGraphVertex*> VertexPointerList;
    typedef std::list<DualGraphPath*> PathList;  

    const DualGraphVertex& getDualVertex(const xaifBoosterCrossCountryInterface::LinearizedComputationalGraphEdge& theEdge) const;
    
    void populatePathList();
    void copyPath(DualGraphPath* thePath);
    void clearPathList();
    
    FacePointerList populateElimList();
    bool isFinal(DualGraphVertex& theVertex) const;

    DualGraphVertex* elim_face(
      DualGraphEdge& theFace,
      const DualGraph::VertexPointerList& thePredList,
      const DualGraph::VertexPointerList& theSuccList,
      xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionList& theJacobianAccumulationExpressionList);

    PathList myPathList;

  private:

  }; // end of class DualGraph 

} // end of namespace MemOpsTradeoffPreaccumulation
                                                                     
#endif
