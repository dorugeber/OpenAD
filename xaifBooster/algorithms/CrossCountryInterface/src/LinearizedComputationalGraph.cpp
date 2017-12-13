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

namespace xaifBoosterCrossCountryInterface {

  const LinearizedComputationalGraph::VertexPointerList&
  LinearizedComputationalGraph::getIndependentList() const { 
    return myIndependentList;
  }
  
  const LinearizedComputationalGraph::VertexPointerList& 
  LinearizedComputationalGraph::getDependentList() const {
    return myDependentList;
  }

} 
