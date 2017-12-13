#ifndef _REMAINDERGRAPH_INCLUDE_
#define _REMAINDERGRAPH_INCLUDE_
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

#include "xaifBooster/algorithms/CrossCountryInterface/inc/Elimination.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/RemainderGraphVertex.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/RemainderGraphEdge.hpp"

using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulation { 

  /**
   * the default representation for the RemainderGraph
   */
  class RemainderGraph : public GraphWrapper<RemainderGraphVertex,
                                             RemainderGraphEdge> {
  public:

    RemainderGraph(){};

    void initFrom(const xaifBoosterCrossCountryInterface::Elimination& anElimination);

    virtual ~RemainderGraph(){};
    
    /**
     * \todo JU implementation incomplete
     */
    std::string debug() const { return std::string("RemainderGraph");};

    /// calls GraphvizDisplay::show
    void
    show(const std::string& outputName) const;

  private:

  }; 

} 
                                                                     
#endif
