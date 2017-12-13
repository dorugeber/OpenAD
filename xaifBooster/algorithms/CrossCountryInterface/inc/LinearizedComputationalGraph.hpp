#ifndef _LINEARIZEDCOMPUTATIONALGRAPH_INCLUDE_
#define _LINEARIZEDCOMPUTATIONALGRAPH_INCLUDE_
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

#include "xaifBooster/algorithms/CrossCountryInterface/inc/LinearizedComputationalGraphVertex.hpp"
#include "xaifBooster/algorithms/CrossCountryInterface/inc/LinearizedComputationalGraphEdge.hpp"

using namespace xaifBooster;

namespace xaifBoosterCrossCountryInterface { 

  /**
   * the default representation for LinearizedComputationalGraph
   */
  class LinearizedComputationalGraph : public GraphWrapper<LinearizedComputationalGraphVertex,
				       LinearizedComputationalGraphEdge>{
  public:
    
    typedef std::list<const LinearizedComputationalGraphVertex*> VertexPointerList;
    const VertexPointerList& getIndependentList() const;
    const VertexPointerList& getDependentList() const;

  protected: 

    VertexPointerList myIndependentList;
    VertexPointerList myDependentList;

  }; // end of class LinearizedComputationalGraph 

} 
                                                                     
#endif
