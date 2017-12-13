#ifndef _ELIMINATION_INCLUDE_
#define _ELIMINATION_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include <map>
#include "xaifBooster/utils/inc/GuardedMap.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/PreaccumulationCounter.hpp"

#include "xaifBooster/algorithms/CrossCountryInterface/inc/AccumulationGraph.hpp"
#include "xaifBooster/algorithms/CrossCountryInterface/inc/AwarenessLevel.hpp"
#include "xaifBooster/algorithms/CrossCountryInterface/inc/LinearizedComputationalGraph.hpp"
#include "xaifBooster/algorithms/CrossCountryInterface/inc/LinearizedComputationalGraphVertex.hpp"
#include "xaifBooster/algorithms/CrossCountryInterface/inc/JacobianAccumulationExpressionList.hpp"
#include "xaifBooster/algorithms/CrossCountryInterface/inc/GraphCorrelations.hpp"

namespace xaifBoosterCrossCountryInterface { 

  /**
   * class for configuring and carrying out an elimination sequence
   */
  class Elimination {

    public:
    
    enum EliminationType_E {
      UNSET_ELIMTYPE,
      OPS_ELIMTYPE,
      OPS_RANDOM_ELIMTYPE,
      OPS_LSA_VERTEX_ELIMTYPE,
      OPS_LSA_FACE_ELIMTYPE,
      SCARCE_ELIMTYPE,
      SCARCE_RANDOM_ELIMTYPE,
      SCARCE_TRANSFORMATIONTYPE,
      SCARCE_RANDOM_TRANSFORMATIONTYPE};

    /// just sets graph.  Use initAs* methods for more specific things
    Elimination (LinearizedComputationalGraph& lcg);
    ~Elimination(){};

    // init functions allow for a generic constructor for all elimination types
    void initAsOperations();
    void initAsOperationsRandom();
    void initAsLSAVertex(int i, double g);
    void initAsLSAFace(int i, double g);
    void initAsScarceElimination();
    void initAsScarceRandomElimination();
    void initAsScarceTransformation();
    void initAsScarceRandomTransformation();
    
    void eliminate();

    static void
    setAwarenessLevel(AwarenessLevel::AwarenessLevel_E anAwarenessLevel);
   
    static void setAllowMaintainingFlag();
    
    std::string getDescription() const;

    const int getNumIterations () const {
      return myNumIterations;
    }
    
    const double getGamma () const {
      return myGamma;
    }
    
    const LinearizedComputationalGraph& getRemainderLCG() const;
    LinearizedComputationalGraph& getRemainderLCG();

    const AccumulationGraph& getAccumulationGraph() const;
    AccumulationGraph& getAccumulationGraph();

    /**
     * used for propagation
     */
    typedef GuardedMap<const LinearizedComputationalGraphEdge*,
                     const AccumulationGraphVertex*> RemainderEdge2AccumulationVertexMap;

    const RemainderEdge2AccumulationVertexMap& getRemainderEdge2AccumulationVertexMap() const;

    const PreaccumulationCounter& getCounter() const;

    unsigned int getNumReroutings() const;

    /**
     * returns a reference to the original LCG vertex that corresponds to the passed remainder graph vertex
     */
    const LinearizedComputationalGraphVertex&
    rVertex2oVertex (const LinearizedComputationalGraphVertex& theRemainderVertex) const;

    /**
     * Flattens the JAEs into a single Accumulation graph and populates a mapping from remainder graph edges to
     * accumulation graph vertices, which will be used in the generation of propagators. 
     */
    void buildAccumulationGraph();

  private:

    Elimination(){};
    
    EliminationType_E myType;
   
    std::string myDescription;
    
    LinearizedComputationalGraph* myLCG_p;

    static AwarenessLevel::AwarenessLevel_E ourAwarenessLevel;
    static bool ourAllowMaintainingFlag;
    
    int myNumIterations;
    double myGamma;

    /// this will be populated by angel during the transformation process
    JacobianAccumulationExpressionList myJAEList;

    /// this is populated by angel after the elimination process
    LinearizedComputationalGraph myRemainderLCG;

    /// maps vertices in the remainder graph to vertices in the original graph
    VertexCorrelationList myVertexCorrelationList;

    /// maps edges in the remainder graph to edges in the original graph
    EdgeCorrelationList myEdgeCorrelationList;

    /// built subsequent to the angel transformation
    AccumulationGraph myAccumulationGraph;

    /// maps edges in the remainder graph to thjeiur corresponding vertices in myAccumulationGraph.  used in propagation
    RemainderEdge2AccumulationVertexMap myRemainderEdge2AccumulationVertexMap;

    /// contains the results of performing the transformation (in terms of ops and remainder graph edges)
    mutable PreaccumulationCounter myCounter;

    /// for scarcity with rerouting, holds the number of reroutings
    unsigned int myNumReroutings;

    typedef GuardedMap<const LinearizedComputationalGraphEdge*,
                     AccumulationGraphVertex*> LCGe_to_ACCv_map;

    typedef GuardedMap<const JacobianAccumulationExpressionVertex*,
                     AccumulationGraphVertex*> JAEv_to_ACCv_map;

    /**
     * have we counted the elimination operations
     */
    mutable bool myCountedFlag;

    /// used for evaluating preaccumulation cost
    typedef std::map<const AccumulationGraphVertex*,
                     bool> AccVertexIsUnitMap;

    /**
     * \brief recursively visits Accumulation graph vertices and counts nontrivial operations (cost)
     * should be called exactly once for every accumulation graph vertex
     */
    void evaluateCostRecursively(const AccumulationGraphVertex& rootAccvertex,
                                 AccVertexIsUnitMap& theAccVertexIsUnitMap) const;

    /// counts all nontrivial operations in myAccumulationGraph
    void countPreaccumulationOperations() const;

    /// sets the three counters that measure scarcity in the remainder graph
    void countRemainderGraphEdges() const;

  }; // end of class Elimination

} // end namespace xaifBoosterCrossCountryInterface

#endif
