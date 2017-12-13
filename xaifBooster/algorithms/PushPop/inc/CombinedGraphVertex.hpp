#ifndef _COMBINEDGRAPHVERTEX_INCLUDE_
#define _COMBINEDGRAPHVERTEX_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/boostWrapper/inc/Vertex.hpp"

#include "xaifBooster/system/inc/ExpressionVertex.hpp"
#include "xaifBooster/system/inc/ExpressionEdge.hpp"

#include "xaifBooster/algorithms/Linearization/inc/ExpressionEdgeAlg.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/PrivateLinearizedComputationalGraphVertex.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/RemainderGraphVertex.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/RemainderGraphEdge.hpp"

#include "xaifBooster/algorithms/CrossCountryInterface/inc/AccumulationGraphVertex.hpp"

using namespace xaifBooster;

namespace xaifBoosterPushPop {

  /**
   * Aggregates the original graph, the partial expressions, and the accumulation graph
   * each vertex is one of the following types:
   * - an original computational graph vertex (which may occur as an argument in linearization (partial) expressions and the accumulation graph)
   * - an argument to a partial expression (will be a passive variable if it's not alsoa LCG vertex), which may also be an accumulationgraph argument
   * - a constant in a partial expression (which may also be an argument in the accumulation graph)
   * - an intrinsic in a partial expression (which may also have a linearization variable and be an argument in the accumulation graph)
   */
  class CombinedGraphVertex : public Vertex {

  public:

    CombinedGraphVertex();

    ~CombinedGraphVertex(){};

    std::string
    debug() const;

    /// see myLHSCVariablePSet
    void
    addLHSVariable(const Variable&);

    /// see myLHSCVariablePSet
    const CVariablePSet&
    getLHSCVariablePSet() const;

    const bool
    hasComputationalGraphVertex() const;

    void
    setComputationalGraphVertex(const xaifBoosterBasicBlockPreaccumulation::PrivateLinearizedComputationalGraphVertex&);

    const xaifBoosterBasicBlockPreaccumulation::PrivateLinearizedComputationalGraphVertex&
    getComputationalGraphVertex() const;

    void
    associateLinearizationExpressionVertex(const ExpressionVertex&);

    /// see myLinearizationExpressionConstant_p
    const bool
    hasLinearizationExpressionConstant() const;

    /// see myLinearizationExpressionIntrinsic_p
    const bool
    hasLinearizationExpressionIntrinsic() const;

    /// see myLinearizationExpressionArgumentPSet
    const bool
    hasLinearizationExpressionArgument() const;

    /// returns true iff there is a linearization constant, intrinsic, or argument
    const bool
    hasLinearizationExpressionVertex() const;

    /// myLinearizedExpressionEdgeAlg_p
    const bool
    hasLinearizedExpressionEdgeAlg() const;

    /// see myLinearizedExpressionEdgeAlg_p
    void
    setLinearizedExpressionEdgeAlg(const xaifBoosterLinearization::ExpressionEdgeAlg&);

    /// see myLinearizedExpressionEdgeAlg_p
    const xaifBoosterLinearization::ExpressionEdgeAlg&
    getLinearizedExpressionEdgeAlg() const;

    const bool
    hasAccumulationGraphVertex() const;

    void
    associateAccumulationGraphVertex(const xaifBoosterCrossCountryInterface::AccumulationGraphVertex&);

    const xaifBoosterCrossCountryInterface::AccumulationGraphVertex&
    getAccumulationGraphVertex() const;

    const bool
    hasRemainderGraphVertex() const;

    /// shouldn't be needed???
    void
    setRemainderGraphVertex(const xaifBoosterBasicBlockPreaccumulation::RemainderGraphVertex&);

    const xaifBoosterBasicBlockPreaccumulation::RemainderGraphVertex&
    getRemainderGraphVertex() const;

    const bool
    hasRemainderGraphEdge() const;

    void
    associateRemainderGraphEdge(const xaifBoosterBasicBlockPreaccumulation::RemainderGraphEdge&);

    /// for graphViz
    std::string
    getShapeString() const;

    /// for graphViz
    std::string
    getColorString() const;

    /// for graphViz
    std::string
    getLabelString() const;

  private:

    /// no def
    CombinedGraphVertex(const CombinedGraphVertex&);
    /// no def
    CombinedGraphVertex& operator=(const CombinedGraphVertex&);

    /**
     * a CombinedGraphVertex may be associated with any of the following:
     * a propagation variable, an accumulation variable, a linearization variable,
     * an auxiliary variable, an original program variable
     * currently we only track linearization assignment LHSs and propagation variables
     */
    CVariablePSet myLHSCVariablePSet;

    /// points to the corresp. computational graph vertex (may not be set)
    const xaifBoosterBasicBlockPreaccumulation::PrivateLinearizedComputationalGraphVertex* myLCGVertex_p;

    /**
     * points to an intrinsic expression vertex in a partial expression.
     * if this is set, this should be the only associated linearization expression vertex
     * (should not be an original LCG vertex, but may be an accumulation vertex)
     */
    const Intrinsic* myLinearizationExpressionIntrinsic_p;

    /**
     * a set of arguments in partial linearization expressions.
     * if this is nonempty, then we shouldnt also have a constant or an intrinsic linearization expression vertex.
     * can be either a passive variable or an active variable (in the latter case this CGV should correspond to an original LCG vertex)
     */
    CArgumentPSet myLinearizationExpressionArgumentPSet;

    /**
     * points to a constant expression vertex in a partial expression.
     * if this is set, this should be the only associated linearization expression vertex
     * (should not be an original LCG vertex, but may be an accumulation vertex)
     */
    const Constant* myLinearizationExpressionConstant_p;

    /**
     * for trivial partial expressions exclusively (PDK LINEAR_ONE or LINEAR_MINUS_ONE).
     * If this is set, then we shouldnt correspond to anything else (all other pointers should be zero, etc.)
     * In particular, we shouldnt also have a linearization intrinsic or constant
     */
    const xaifBoosterLinearization::ExpressionEdgeAlg* myLinearizedExpressionEdgeAlg_p;

    /// a single CombinedGraphVertex could correspond to multiple AccumulationGraph vertices
    xaifBoosterCrossCountryInterface::CAccumulationGraphVertexPSet myAccumulationGraphVertexPSet;

    /// for now, every CombinedGraphVertex can correspond to at most one vertex in the RemainderGraph (may not be set)
    // \TODO \FIXME: we shouldn't need this?
    const xaifBoosterBasicBlockPreaccumulation::RemainderGraphVertex* myRemainderGraphVertex_p;

    /// a single CombinedGraphVertex may correspond to multiple RemainderGraph edges -- think of an independent that is multiplied with a number of different things (may be empty)
    xaifBoosterBasicBlockPreaccumulation::CRemainderGraphEdgePSet myRemainderGraphEdgePSet;

  };
 
} 

#endif

