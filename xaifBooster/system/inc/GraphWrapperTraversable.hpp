#ifndef  _GRAPHWRAPPERTRAVERSABLE_INCLUDE_
#define  _GRAPHWRAPPERTRAVERSABLE_INCLUDE_
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
#include "xaifBooster/utils/inc/GenericTraverseInvoke.hpp"

namespace xaifBooster { 

  /**
   * generic wrapper class for boost graphs
   * with vertex and edge template parameters
   * which denote the content of vertices and 
   * edges respectively
   */
  template <class Vertex, class Edge> 
  class GraphWrapperTraversable : public GraphWrapper<Vertex,Edge>,
				  public GenericTraverseInvoke {
  public:

    GraphWrapperTraversable(){};

    virtual ~GraphWrapperTraversable(){};

    void traverseToChildren(const GenericAction::GenericAction_E anAction_c); 

  private:

    /**
     * disallow copy ctor for now
     * JU: we can refine this to only allow 
     * copies of empty graphs (i.e. 0 vertices and edges)
     * to avoid copying Edge and Vertex pointers an getting into 
     * confusion who owns the edges/vertices and attempt to delete 
     * them twice. 
     * Alternatively we could perform a true copy but it
     * will be very expensive, i.e. should be avoided. 
     */
    GraphWrapperTraversable(const GraphWrapperTraversable&);

  }; // end of class BoostGraphWrapper
  
} // end of namespace

#include "TemplateImpl/GraphWrapperTraversable.cpp"
                                                                     
#endif
