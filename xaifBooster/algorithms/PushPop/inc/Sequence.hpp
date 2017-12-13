#ifndef _XAIFBOOSTERPUSHPOP_SEQUENCE_INCLUDE_
#define _XAIFBOOSTERPUSHPOP_SEQUENCE_INCLUDE_
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

#include "xaifBooster/system/inc/Expression.hpp"
#include "xaifBooster/system/inc/ExpressionVertex.hpp"

#include "xaifBooster/algorithms/RequiredValues/inc/RequiredValue.hpp"
#include "xaifBooster/algorithms/RequiredValues/inc/RequiredValueSet.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/PrivateLinearizedComputationalGraphVertex.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/RemainderGraphVertex.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulationTape/inc/Sequence.hpp"

#include "xaifBooster/algorithms/CrossCountryInterface/inc/AccumulationGraphVertex.hpp"

#include "xaifBooster/algorithms/PushPop/inc/CombinedGraph.hpp" 
#include "xaifBooster/algorithms/PushPop/inc/CombinedGraphEdge.hpp" 
#include "xaifBooster/algorithms/PushPop/inc/CombinedGraphVertex.hpp" 

using namespace xaifBooster;

namespace xaifBoosterPushPop {

  /**
   * Derived Sequence class for ...
   */
  class Sequence : public xaifBoosterBasicBlockPreaccumulationTape::Sequence {

  public:

    Sequence();

    virtual ~Sequence();

    virtual std::string debug() const;

    /**
     * check ... for \p anExpression
     */
    virtual bool hasExpression(const Expression& anExpression) const;

    xaifBoosterRequiredValues::RequiredValueSet::ComparisonResult_E
    compareExpressions(const Expression& firstExpression,
                       const Expression& secondExpression) const;

    void assignAndPushRequiredValueAfter(const xaifBoosterRequiredValues::RequiredValue& aRequiredValue);

    void pushRequiredValueAfter(const xaifBoosterRequiredValues::RequiredValue& aRequiredValue);

  protected:
    friend class BasicBlockAlg;

    void populateCombinedGraph();

  private:

    /// no def
    Sequence(const Sequence&);

    /// no def
    Sequence& operator= (const Sequence&);

    /// a list of pushes (owned by us) that occur after this sequence
    CBasicBlockElementPList myPushBlock;

    /** 
     * The combined graph, which includes
     * - the original computational graph for this sequence
     * - computation of the local partials (linearization)
     * - the accumulation graph
     */
    CombinedGraph myCombinedGraph;

    typedef std::map<const ExpressionEdge*,
                     CombinedGraphVertex*> CExpressionEdgeP2CombinedGraphVertexPMap;

    typedef std::map<const xaifBoosterBasicBlockPreaccumulation::PrivateLinearizedComputationalGraphVertex*,
                     CombinedGraphVertex*> PLCGVp2CGVpMap;
    /// maps each computational graph vertex to the corresponding CombinedGraphVertex
    PLCGVp2CGVpMap myPLCGVp2CGVpMap;

    typedef std::map<const ExpressionVertex*,
                     CombinedGraphVertex*> CExpressionVertexP2CombinedGraphVertexPMap;
    /**
     * maps original expression vertices to the corresp. CG vertex
     * (the original expression vertex associated with the corresp. PLCG vertex)
     * used to associate arguments to concrete partial expressions with the corresp. CGV
     */
    CExpressionVertexP2CombinedGraphVertexPMap myEVp2CGVpMap;

    typedef std::map<const xaifBoosterCrossCountryInterface::AccumulationGraphVertex*,
                     CombinedGraphVertex*> CAGVp2CGVpMap;
    /// maps each AccumulationGraphVertex to the corresponding CombinedGraphVertex
    CAGVp2CGVpMap myAGVp2CGVpMap;

    /**
     * duplicate a partial expression and affix it to the CombinedGraph
     * returns a reference to the CombinedGraphVertex that corresponds to the maximal linearization expression vertex
     */
    CombinedGraphVertex& affixLinearizationExpressionToCombinedGraph(const ExpressionEdge&);

    /// maps each RemainderGraphVertex to the corresponding CombinedGraphVertex
    CombinedGraph::CRGVp2CCGVpMap myRGVp2CGVpMap;

    /// maps each RemainderGraphEdge to the corresponding CombinedGraphVertex
    CombinedGraph::CRGEp2CCGVpMap myRGEp2CGVpMap;

  };

}

#endif
