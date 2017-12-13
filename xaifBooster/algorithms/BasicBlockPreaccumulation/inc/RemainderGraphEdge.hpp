#ifndef _REMAINDERGRAPHEDGE_INCLUDE_
#define _REMAINDERGRAPHEDGE_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include <set>

#include "xaifBooster/boostWrapper/inc/Edge.hpp"

#include "xaifBooster/algorithms/CrossCountryInterface/inc/AccumulationGraphVertex.hpp"

using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulation { 

  class RemainderGraphEdge : public Edge {
  public:

    RemainderGraphEdge(const xaifBoosterCrossCountryInterface::AccumulationGraphVertex&);

    ~RemainderGraphEdge(){};

    std::string debug() const;

    /// for GraphViz
    std::string
    getColorString() const;

    /// for GraphViz
    std::string
    getLabelString() const;

    /// for GraphViz
    std::string
    getStyleString() const;

    const xaifBoosterCrossCountryInterface::AccumulationGraphVertex&
    getAccumulationGraphVertex() const;

  private:

    /**
     * we do not own this
     */
    const xaifBoosterCrossCountryInterface::AccumulationGraphVertex* myAccumulationGraphVertex_p;

  }; 

  typedef std::set<const RemainderGraphEdge*> CRemainderGraphEdgePSet;
 
} 
                                                                     
#endif
