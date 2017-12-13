#ifndef  _BOOSTVERTEXCONTENTTYPE_INCLUDE_
#define  _BOOSTVERTEXCONTENTTYPE_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "boost/graph/adjacency_list.hpp"
#include "boost/property_map/property_map.hpp"         

namespace xaifBooster { 

  /** 
   * the following is required by boost
   * to give the vertex property map a unique ID
   * within the collection of the graph's maps
   */
  class BoostVertexContentType { 
  public: 
    enum { num=100 };
    typedef boost::vertex_property_tag kind;
  }; // end class

} // end of namespace

#endif
