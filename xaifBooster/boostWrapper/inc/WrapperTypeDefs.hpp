#ifndef  _WRAPPERTYPEDEFS_INCLUDE_
#define  _WRAPPERTYPEDEFS_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#define BOOST_NO_HASH // gets rid of hash_set deprecation warnings until boost fixes its code
#include "boost/graph/adjacency_list.hpp"
#include "boost/property_map/property_map.hpp"         

namespace xaifBooster { 

  struct WrapperTypeDefs { 
    /**
     * \todo JU make this a parameter of GraphWrapper
     */
    typedef boost::listS OutEdgeListType;
    typedef boost::listS VertexListType;
    typedef boost::bidirectionalS DiretionType;
    
    typedef boost::adjacency_list_traits<
      OutEdgeListType,
      VertexListType,
      DiretionType
      > ::vertex_descriptor 
    InternalBoostVertexDescriptorType;

    typedef boost::adjacency_list_traits<
      OutEdgeListType,
      VertexListType,
      DiretionType
      > ::edge_descriptor 
    InternalBoostEdgeDescriptorType;

 }; // end of struct 

} // end of namespace

#endif 
