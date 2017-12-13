#ifndef _LINEARIZEDCOMPUTATIONALGRAPHEDGE_INCLUDE_
#define _LINEARIZEDCOMPUTATIONALGRAPHEDGE_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/boostWrapper/inc/Edge.hpp"

using namespace xaifBooster;

namespace xaifBoosterCrossCountryInterface { 

  class LinearizedComputationalGraphEdge : public Edge {
  public:

    LinearizedComputationalGraphEdge();

    ~LinearizedComputationalGraphEdge(){};

    enum LCG_Edge_Label_Type_E { VARIABLE_LABEL,
				 UNIT_LABEL,
				 CONSTANT_LABEL };
    
    void setEdgeLabelType(LCG_Edge_Label_Type_E anEdgeLabelType);

    LCG_Edge_Label_Type_E getEdgeLabelType() const;

  private: 

    /**
     * Indicator of edge label type.
     * Every edge is either variable, unit (+/- 1), or constant. 
     * Set to VARIABLE_EDGE by default
     */
    LCG_Edge_Label_Type_E myEdgeLabelType;

 
  }; // end of class LinearizedComputationalGraphEdge
 
} 
                                                                     
#endif
