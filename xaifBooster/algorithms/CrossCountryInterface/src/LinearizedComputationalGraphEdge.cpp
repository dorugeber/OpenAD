// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/algorithms/CrossCountryInterface/inc/LinearizedComputationalGraphEdge.hpp"

namespace xaifBoosterCrossCountryInterface {

  LinearizedComputationalGraphEdge::LinearizedComputationalGraphEdge() :
    myEdgeLabelType(VARIABLE_LABEL) { 
  } 

  void LinearizedComputationalGraphEdge::setEdgeLabelType(LinearizedComputationalGraphEdge::LCG_Edge_Label_Type_E anEdgeLabelType) { 
    myEdgeLabelType = anEdgeLabelType; 
  }

  LinearizedComputationalGraphEdge::LCG_Edge_Label_Type_E LinearizedComputationalGraphEdge::getEdgeLabelType() const { 
    return myEdgeLabelType;
  }

} 
