#ifndef  _EDGE_INCLUDE_
#define  _EDGE_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/boostWrapper/inc/GraphElement.hpp" 
#include "xaifBooster/boostWrapper/inc/WrapperTypeDefs.hpp"

namespace xaifBooster { 

  /** 
   * this is an edge to be 
   * used with the GraphWrapper class 
   * which doesn't have capacity for traversal
   */ 
  typedef GraphElement<WrapperTypeDefs::InternalBoostEdgeDescriptorType> Edge;

} // end of namespace xaifBooster

#endif
