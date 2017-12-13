#ifndef  _GRAPHWRAPPER_INCLUDE_
#define  _GRAPHWRAPPER_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/boostWrapper/inc/WrapperTypeDefs.hpp"
#include "xaifBooster/boostWrapper/inc/BoostVertexContentType.hpp"
#include "xaifBooster/boostWrapper/inc/BoostEdgeContentType.hpp"
#include "xaifBooster/boostWrapper/inc/IteratorWrapper.hpp"


namespace xaifBooster { 

  /**
   * generic wrapper class for boost graphs
   * with vertex and edge template parameters
   * which denote the content of vertices and 
   * edges respectively
   */
  template <class Vertex, class Edge> 
  class GraphWrapper {
  public:

    GraphWrapper();
    
    /**
     * deletes all the vertices and edges
     */    
    virtual ~GraphWrapper();

    unsigned int numEdges()const {return boost::num_edges(myBoostGraph);};

    /**
     * returns true if there are no vertices
     * (as opposed to "empty" graph indicating 
     * no edges but possibly isolated vertices)
     */
    bool isNull()const; 

    unsigned int numOutEdgesOf(const Vertex& aVertex)const; 
    unsigned int numInEdgesOf(const Vertex& aVertex)const; 
    unsigned int numVertices()const {return boost::num_vertices(myBoostGraph);};

    /**
     * adds a vertex 
     * returns a reference 
     */
    Vertex& addVertex();

    /**
     * adds a vertex to the graph,
     * however the Vertex instance has to 
     * be DYNAMICALLY ALLOCATED and the 
     * OWNERSHIP IS RELINQUISHED to the graph.
     * This interface should only be used for 
     * instances of Vertices which are subclasses
     * of the 'Vertex' type used for the 
     * GraphWrapper template instantiation
     */
    Vertex& supplyAndAddVertexInstance(Vertex& theVertex);

    /** 
     * this removes a vertex and all incident 
     * edges from the graph. 
     * NOTE: this will invalidate the Vertex instance
     */
    void removeAndDeleteVertex(const Vertex& aVertex);

    /**
     * adds a edge
     * returns a reference 
     */
    Edge& addEdge(const Vertex& theSource_cr,
		  const Vertex& theTarget_cr);

    /**
     * adds a edge to the graph,
     * however the Edge instance has to 
     * be DYNAMICALLY ALLOCATED and the 
     * OWNERSHIP IS RELINQUISHED to the graph.
     * This interface should only be used for 
     * instances of Edges which are subclasses
     * of the 'Edge' type used for the 
     * GraphWrapper template instantiation
     */
    Edge& supplyAndAddEdgeInstance(Edge& theEdge,
				  const Vertex& theSource_cr,
				  const Vertex& theTarget_cr);

    /** 
     * this removes an edge
     * NOTE: this will invalidate the Edge instance
     */
    void removeAndDeleteEdge(const Edge& anEdge);

    /** 
     * get the source vertex of the edge supplied as parameter
     */
    Vertex& getSourceOf(const Edge& anEdge);
    
    const Vertex& getSourceOf(const Edge& anEdge) const;

    /** 
     * get the target vertex of the edge supplied as parameter
     */
    Vertex& getTargetOf(const Edge& anEdge);

    const Vertex& getTargetOf(const Edge& anEdge) const;

    /** 
     * get a maximal vertex of the graph if it exists
     */
    Vertex& getMaxVertex();

    const Vertex& getMaxVertex() const;

    /** 
     * remove all edges and vertices from a graph
     */
    void clear();

    /** 
     * tests if aDominatedVertex is strictly dominated 
     * by aDominatorVertex, i.e. all paths to maximal vertices
     * in the graph have to pass through aDominatorVertex
     * direction indicates if the path should follow the 
     * edge direction or its reverse
     */
    bool firstDominatedBySecond(const Vertex& aDominatedVertex,
				const Vertex& aDominatorVertex,
				bool sameDirection) const;

    /**
     * tries to find \param anEdge by address
     */
    bool has(const Edge& anEdge) const;

    /**
     * tries to find \param aVertex by address
     */
    bool has(const Vertex& aVertex) const;

  protected:

    /** 
     * our internal representation of the boost graph 
     */
    typedef typename 
    boost::adjacency_list<WrapperTypeDefs::OutEdgeListType,
			  WrapperTypeDefs::VertexListType,
			  WrapperTypeDefs::DiretionType,
			  // we have to use Vertex and Edge pointers
			  // if we used value maps and returned references
			  // the internal resizing of the maps invalidates 
			  // these references, therefore we have to use pointers 
			  // to circumvent the invalidation. 
			  boost::property<BoostVertexContentType, Vertex*>,
			  boost::property<BoostEdgeContentType, Edge*> > InternalBoostGraphType;
    InternalBoostGraphType myBoostGraph;

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
    GraphWrapper(const GraphWrapper&);

    /**
     * disallow assignment, same rationale as with copy constructor
     */
    GraphWrapper& operator=(const GraphWrapper&);

    typedef typename boost::graph_traits<InternalBoostGraphType>::edge_iterator InternalBoostEdgeIteratorType;
    typedef typename boost::graph_traits<InternalBoostGraphType>::out_edge_iterator InternalBoostOutEdgeIteratorType;
    typedef typename boost::graph_traits<InternalBoostGraphType>::in_edge_iterator InternalBoostInEdgeIteratorType;
    typedef typename boost::graph_traits<InternalBoostGraphType>::vertex_iterator InternalBoostVertexIteratorType;
    
    /** 
     * simple counter to provide Ids 
     * to vertices
     */
    unsigned int myNextVertexId;

    /** 
     * simple counter to provide Ids 
     * to edges
     */
    unsigned int myNextEdgeId;

  public: 

    const InternalBoostGraphType& getInternalBoostGraph() const { return myBoostGraph;};
    
    typedef IteratorWrapper<InternalBoostGraphType,
			    InternalBoostEdgeIteratorType,
			    BoostEdgeContentType,
			    Edge> EdgeIterator;

    typedef typename 
    std::pair<EdgeIterator, EdgeIterator >  EdgeIteratorPair;

    /**
     * returns a pair of vertex iterators pointing 
     * to the equivalent of STL's begin() and end() respectively.
     * is equivalent to boost's vertices() method.
     */
    EdgeIteratorPair edges();

    typedef IteratorWrapper<const InternalBoostGraphType,
			    InternalBoostEdgeIteratorType,
			    BoostEdgeContentType,
			    const Edge> ConstEdgeIterator;

    typedef typename 
    std::pair<ConstEdgeIterator, ConstEdgeIterator>  ConstEdgeIteratorPair;

    /** 
     * the const equivalent to edges()
     */
    ConstEdgeIteratorPair edges() const;

    typedef IteratorWrapper<InternalBoostGraphType,
			    InternalBoostVertexIteratorType,
			    BoostVertexContentType,
			    Vertex> VertexIterator;

    typedef typename 
    std::pair<VertexIterator, VertexIterator >  VertexIteratorPair;

    /**
     * returns a pair of vertex iterators pointing 
     * to the equivalent of STL's begin() and end() respectively.
     * is equivalent to boost's vertices() method.
     */
    VertexIteratorPair vertices();

    typedef IteratorWrapper<const InternalBoostGraphType,
			    InternalBoostVertexIteratorType,
			    BoostVertexContentType,
			    const Vertex> ConstVertexIterator;

    typedef typename 
    std::pair<ConstVertexIterator, ConstVertexIterator>  ConstVertexIteratorPair;

    /** 
     * the const equivalent to vertices()
     */
    ConstVertexIteratorPair vertices() const;

    typedef IteratorWrapper<InternalBoostGraphType,
			    InternalBoostOutEdgeIteratorType,
			    BoostEdgeContentType,
			    Edge> OutEdgeIterator;
    typedef typename 
    std::pair<OutEdgeIterator, OutEdgeIterator>  OutEdgeIteratorPair;

    /** 
     * get the out edges of a vertex
     */
    OutEdgeIteratorPair getOutEdgesOf(const Vertex& aVertex_cr); 

    typedef IteratorWrapper<const InternalBoostGraphType,
			    InternalBoostOutEdgeIteratorType,
			    BoostEdgeContentType,
			    const Edge> ConstOutEdgeIterator;
    typedef typename 
    std::pair<ConstOutEdgeIterator, ConstOutEdgeIterator>  ConstOutEdgeIteratorPair;

    /** 
     * get the out edges of a vertex
     */
    ConstOutEdgeIteratorPair getOutEdgesOf(const Vertex& aVertex_cr) const; 

    typedef IteratorWrapper<InternalBoostGraphType,
			    InternalBoostInEdgeIteratorType,
			    BoostEdgeContentType,
			    Edge> InEdgeIterator;
    typedef typename 
    std::pair<InEdgeIterator, InEdgeIterator>  InEdgeIteratorPair;

    /** 
     * get the in edges of a vertex
     */
    InEdgeIteratorPair getInEdgesOf(const Vertex& aVertex_cr); 

    typedef IteratorWrapper<const InternalBoostGraphType,
			    InternalBoostInEdgeIteratorType,
			    BoostEdgeContentType,
			    const Edge> ConstInEdgeIterator;
    typedef typename 
    std::pair<ConstInEdgeIterator, ConstInEdgeIterator>  ConstInEdgeIteratorPair;

    /** 
     * get the in edges of a vertex
     */
    ConstInEdgeIteratorPair getInEdgesOf(const Vertex& aVertex_cr) const; 

    /** 
     * gets the next usable vertex id
     */
    unsigned int getNextVertexId();

    /** 
     * gets the next usable edge id
     */
    unsigned int getNextEdgeId();


    /** 
     * indicates if we perform a visitor algorithm on the graph
     */
    mutable bool myVisitInProgressFlag;

    void initVisit() const;

    void finishVisit() const;

    /** 
     * strict domination in edge direction
     */
    bool dominates_f(const Vertex& aDominatedVertex,
		     const Vertex& aDominatorVertex) const;

    /** 
     * strict domination in reverse edge direction
     */
    bool dominates_r(const Vertex& aDominatedVertex,
		     const Vertex& aDominatorVertex) const;

  }; // end of class GraphWrapper
  
} // end of namespace

#include "xaifBooster/boostWrapper/inc/TemplateImpl/GraphWrapper.cpp"
                                                                     
#endif
