#ifndef _XAIFBOOSTERCONSTANTFOLDING_ENHANCEDPRIVATELINEARIZEDCOMPUTATIONALGRAPH_INCLUDE_
#define _XAIFBOOSTERCONSTANTFOLDING_ENHANCEDPRIVATELINEARIZEDCOMPUTATIONALGRAPH_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include <list>
#include "xaifBooster/boostWrapper/inc/GraphWrapper.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/PrivateLinearizedComputationalGraph.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/PrivateLinearizedComputationalGraphEdge.hpp"
#include "xaifBooster/algorithms/ConstantFolding/inc/EnhancedPrivateLinearizedComputationalGraphEdge.hpp"
#include "xaifBooster/algorithms/ConstantFolding/inc/EnhancedPrivateLinearizedComputationalGraphVertex.hpp"

using namespace xaifBooster; 

namespace xaifBoosterConstantFolding {  

  class EnhancedPrivateLinearizedComputationalGraph : public xaifBoosterBasicBlockPreaccumulation::PrivateLinearizedComputationalGraph {
    
  public:
    
    EnhancedPrivateLinearizedComputationalGraph() {};
    
    virtual ~EnhancedPrivateLinearizedComputationalGraph() {};
    
    void front_elim(xaifBoosterConstantFolding::EnhancedPrivateLinearizedComputationalGraphEdge& e);
    void back_elim(xaifBoosterConstantFolding::EnhancedPrivateLinearizedComputationalGraphEdge& e);
    void ConstantFolding();
    void CheckInEdgesForTriviality(xaifBoosterConstantFolding::EnhancedPrivateLinearizedComputationalGraphVertex& j);
    void CheckOutEdgesForTriviality(xaifBoosterConstantFolding::EnhancedPrivateLinearizedComputationalGraphVertex& j);
    bool CheckInEdgesForConstants(xaifBoosterConstantFolding::EnhancedPrivateLinearizedComputationalGraphVertex& j);
    bool CheckOutEdgesForConstants(xaifBoosterConstantFolding::EnhancedPrivateLinearizedComputationalGraphVertex& j);
    void RunConstantCheckForwardBackward();
    void RunTrivialCheckForward();
    void RunTrivialCheckBackwards();
    void RunParallelEdgeAddition();
    void RunTrivialCheckForwardAgain();
    void RunTrivialCheckBackwardsAgain();
    void CheckInEdgesForTrivialitySecondRun(xaifBoosterConstantFolding::EnhancedPrivateLinearizedComputationalGraphVertex& j);
    void CheckOutEdgesForTrivialitySecondRun(xaifBoosterConstantFolding::EnhancedPrivateLinearizedComputationalGraphVertex& j);
   
  };
  
} 

#endif
