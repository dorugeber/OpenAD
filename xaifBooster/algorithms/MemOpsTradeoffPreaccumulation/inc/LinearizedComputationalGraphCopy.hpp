#ifndef _LINEARIZEDCOMPUTATIONALGRAPHCOPY_INCLUDE_
#define _LINEARIZEDCOMPUTATIONALGRAPHCOPY_INCLUDE_
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
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/BasicBlockAlg.hpp"
#include "xaifBooster/algorithms/CrossCountryInterface/inc/LinearizedComputationalGraphVertex.hpp"
#include "xaifBooster/algorithms/MemOpsTradeoffPreaccumulation/inc/LinearizedComputationalGraphCopyEdge.hpp"

using namespace MemOpsTradeoffPreaccumulation;

namespace MemOpsTradeoffPreaccumulation { 

  class LinearizedComputationalGraphCopy : public GraphWrapper<xaifBoosterCrossCountryInterface::LinearizedComputationalGraphVertex,
				       LinearizedComputationalGraphCopyEdge>{
  public:

    unsigned int sdsum, spsum, opsum;

    /**
     * each edge in an edge list has a direction associated with it, so an edge can be in the list twice, one for each direction
     * this allows the heuristics to be mroe efficient
     */
    enum edgeElimDirection {FRONT, BACK};
    struct edgeEntry{
      MemOpsTradeoffPreaccumulation::LinearizedComputationalGraphCopyEdge* edge_p;
      edgeElimDirection direction;
    };

    typedef std::list<const xaifBoosterCrossCountryInterface::LinearizedComputationalGraphVertex*> ConstVertexPointerList;
    typedef std::list<xaifBoosterCrossCountryInterface::LinearizedComputationalGraphVertex*> VertexPointerList;
    typedef std::list<edgeEntry> EdgePointerList;

    /**
     * these functions facilitate the elimination of vertices and edges in an LCG.  the corresponding
     * new edges are created, and the relevant accumulation expressions are constructed and added to the list.
     */
    void front_elim_edge(LinearizedComputationalGraphCopyEdge& theEdge,
			 xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionList& theJacobianAccumulationExpressionList);
    void back_elim_edge(LinearizedComputationalGraphCopyEdge& theEdge,
			xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionList& theJacobianAccumulationExpressionList);
    void elim_vertex(xaifBoosterCrossCountryInterface::LinearizedComputationalGraphVertex& theVertex,
		     xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionList& theJacobianAccumulationExpressionList);

    /**
     * these functions return the independent and dependent lists associated with the graph.
     */
    const ConstVertexPointerList& getIndependentList() const;
    const ConstVertexPointerList& getDependentList() const;

    bool isDep(xaifBoosterCrossCountryInterface::LinearizedComputationalGraphVertex& theVertex) const;

    /**
     * these functions return the number of independent and dependent vertices, respectively
     */
    const unsigned int numIndeps() const;
    const unsigned int numDeps() const;

    /**
     * returns the vertex list associated with the graph
     */
    VertexPointerList getVertexList() const;

    /**
     * these functions facilitate the population of the dependent and independent lists when the graph copy is made
     */
    void addToIndependentList(const xaifBoosterCrossCountryInterface::LinearizedComputationalGraphVertex& theIndepdentVertex);
    void addToDependentList(const xaifBoosterCrossCountryInterface::LinearizedComputationalGraphVertex& theDepdentVertex);

    /**
     * vertexlist is a topologically sorted list of all the vertices in the graph, the vertex list stays the same throughout
     * the elimination process, except that when a vertex is eliminated it is removed from the list.  the edgelist is
     * cleared and recreated based on the vertex list before every edge elimination decision is made.
     */
    void addToVertexList(xaifBoosterCrossCountryInterface::LinearizedComputationalGraphVertex& theVertex);
    void removeFromVertexList(xaifBoosterCrossCountryInterface::LinearizedComputationalGraphVertex& theVertex);
    EdgePointerList populateEdgeList();
    void clearEdgeList();

  private: 

    ConstVertexPointerList myIndependentList;
    ConstVertexPointerList myDependentList;
    VertexPointerList myVertexList;
    EdgePointerList myEdgeList;

  }; // end of class LinearizedComputationalGraphCopy 

} // end of namespace MemOpsTradeoffPreaccumulation
                                                                     
#endif
