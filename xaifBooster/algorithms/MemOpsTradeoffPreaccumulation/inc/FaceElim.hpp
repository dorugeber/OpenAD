#ifndef _XAIFBOOSTERMEMOPSTRADEOFFPREACCUMULATION_FACEELIM_INCLUDE_
#define _XAIFBOOSTERMEMOPSTRADEOFFPREACCUMULATION_FACEELIM_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/algorithms/MemOpsTradeoffPreaccumulation/inc/DualGraph.hpp"
#include "xaifBooster/algorithms/MemOpsTradeoffPreaccumulation/inc/JacobianAccumulationExpressionCopy.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/BasicBlockAlg.hpp"

using namespace MemOpsTradeoffPreaccumulation;

namespace xaifBoosterMemOpsTradeoffPreaccumulation {  

  class FaceElim {

  public:

    /**
     */
    static void absorbMode_f(
      DualGraph& theDual,
      DualGraph::FacePointerList& theOldFaceList,
      const DualGraph::VertexPointerList& thePredList,
      const DualGraph::VertexPointerList& theSuccList,
      DualGraphVertex* newOrAbsorb);

  };  // end of class

} // end of namespace 
                                                                     
#endif
