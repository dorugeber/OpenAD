// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#ifndef  _GRAPHWRAPPERTRAVERSABLE_INCLUDE_
#include "xaifBooster/system/inc/GraphWrapperTraversable.hpp"
#endif

namespace xaifBooster { 

  template <class Vertex, class Edge>
  void
  GraphWrapperTraversable<Vertex,Edge>::traverseToChildren(const GenericAction::GenericAction_E anAction_c) { 
    typename GraphWrapper<Vertex,Edge>::VertexIteratorPair pv=GraphWrapper<Vertex,Edge>::vertices();
    typename GraphWrapper<Vertex,Edge>::VertexIterator beginItv(pv.first),endItv(pv.second);
    for (; beginItv!=endItv ;++beginItv) { 
      (*beginItv).genericTraversal(anAction_c);
    } // end for 
    typename GraphWrapper<Vertex,Edge>::EdgeIteratorPair pe=GraphWrapper<Vertex,Edge>::edges();
    typename GraphWrapper<Vertex,Edge>::EdgeIterator beginIte(pe.first),endIte(pe.second);
    for (; beginIte!=endIte ;++beginIte) { 
      (*beginIte).genericTraversal(anAction_c);
    } // end for 
  }  

} // end of namespace
