#ifndef _COMBINEDGRAPHEDGE_INCLUDE_
#define _COMBINEDGRAPHEDGE_INCLUDE_
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

#include "xaifBooster/system/inc/ExpressionEdge.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/PrivateLinearizedComputationalGraphEdge.hpp"

#include "xaifBooster/algorithms/CrossCountryInterface/inc/AccumulationGraphEdge.hpp"

using namespace xaifBooster;

namespace xaifBoosterPushPop { 

  class CombinedGraphEdge : public Edge {
  public:

    CombinedGraphEdge(const xaifBoosterBasicBlockPreaccumulation::PrivateLinearizedComputationalGraphEdge&);

    /// constructor for linearization expressions
    CombinedGraphEdge(const ExpressionEdge&);

    CombinedGraphEdge(const xaifBoosterCrossCountryInterface::AccumulationGraphEdge&);

    ~CombinedGraphEdge(){};

    std::string debug() const;

    /// for GraphViz
    std::string
    getStyleString() const;

    /// for GraphViz
    std::string
    getColorString() const;

    enum EdgeOriginType_E {ORIGINAL_EDGE,
                           LINEARIZATION_EDGE,
                           ACCUMULATION_EDGE};
    
    EdgeOriginType_E getOriginType() const;

    const xaifBoosterBasicBlockPreaccumulation::PrivateLinearizedComputationalGraphEdge&
    getPrivateLinearizedComputationalGraphEdge() const;

    const xaifBoosterCrossCountryInterface::AccumulationGraphEdge&
    getAccumulationGraphEdge() const;

  private:

    /// no def
    CombinedGraphEdge();
    /// no def
    CombinedGraphEdge(const CombinedGraphEdge&);
    /// no def
    CombinedGraphEdge& operator=(const CombinedGraphEdge&);

    /**
     * 
     */
    const union Origin {
      const xaifBoosterBasicBlockPreaccumulation::PrivateLinearizedComputationalGraphEdge* myOriginalPLCGEdge_p;
      const ExpressionEdge* myLinearizationExpressionEdge_p;
      const xaifBoosterCrossCountryInterface::AccumulationGraphEdge* myAccumulationGraphEdge_p;

      Origin(const xaifBoosterBasicBlockPreaccumulation::PrivateLinearizedComputationalGraphEdge& aPLCGEdge) :
        myOriginalPLCGEdge_p(&aPLCGEdge){}
      Origin(const ExpressionEdge& aLinearizationExpressionEdge) :
        myLinearizationExpressionEdge_p(&aLinearizationExpressionEdge){}
      Origin(const xaifBoosterCrossCountryInterface::AccumulationGraphEdge& aAccumulationGraphEdge) :
        myAccumulationGraphEdge_p(&aAccumulationGraphEdge){}
    } myOrigin;

    const EdgeOriginType_E  myOriginType;

  }; 
 
} 
                                                                     
#endif
