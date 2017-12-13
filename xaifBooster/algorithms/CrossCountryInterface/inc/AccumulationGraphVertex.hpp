#ifndef _XAIFBOOSTERCROSSCOUNTRYINTERFACE_ACCUMULATIONGRAPHVERTEX_INCLUDE_
#define _XAIFBOOSTERCROSSCOUNTRYINTERFACE_ACCUMULATIONGRAPHVERTEX_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include <set>

#include "xaifBooster/boostWrapper/inc/Vertex.hpp"

#include "xaifBooster/system/inc/Constant.hpp"
#include "xaifBooster/system/inc/ExpressionEdge.hpp"
#include "xaifBooster/system/inc/ExpressionEdgeAlgBase.hpp"
#include "xaifBooster/system/inc/Intrinsic.hpp"
#include "xaifBooster/system/inc/PartialDerivativeKind.hpp"
#include "xaifBooster/system/inc/Variable.hpp"

#include "xaifBooster/algorithms/CrossCountryInterface/inc/JacobianAccumulationExpressionVertex.hpp"
#include "xaifBooster/algorithms/CrossCountryInterface/inc/LinearizedComputationalGraphEdge.hpp"

using namespace xaifBooster;

namespace xaifBoosterCrossCountryInterface { 

  class AccumulationGraphVertex : public Vertex {

  public:

    /**
     * Constructor for leaves that correspond to direct copy edges in the LCG;
     * sets only the PDK (which is all that's needed).
     */
    AccumulationGraphVertex();
 
    /**
     * Constructor for leaves (those that don't correspond to direct copy edges in the LCG).
     * Sets the PDK and also the value (when PDK is LINEAR), or the LHSVariable (when PDK is NONLINEAR).
     * AL: currently there is a HACK in place that sets the PDK to NONLINEAR if
     * it's linear but is some constant expression instead of a particular known constant
     * (in other words, if the RHS of the partial expression consists of more than one (Constant) vertex.)
     */
    AccumulationGraphVertex(const ExpressionEdge&);
 
    /**
     * Constructor for vertices for which there is an operation (all non-leaves).
     */
    AccumulationGraphVertex(const JacobianAccumulationExpressionVertex::Operation_E&);

    ~AccumulationGraphVertex(){};

    const bool hasExpressionEdge() const;
    const ExpressionEdge& getExpressionEdge() const;

    void setRemainderGraphEdge(const LinearizedComputationalGraphEdge& anLCGEdge);
    const LinearizedComputationalGraphEdge& getRemainderGraphEdge() const;
    const bool hasRemainderGraphEdge() const;

    const bool hasOperation() const;
    JacobianAccumulationExpressionVertex::Operation_E getOperation() const;

    void setPartialDerivativeKind(const PartialDerivativeKind::PartialDerivativeKind_E aPartialDerivativeKind);
    const PartialDerivativeKind::PartialDerivativeKind_E getPartialDerivativeKind() const;

    void setValue(double aValue);
    double getValue() const;
    const bool hasValue() const;

    /// true iff it's a constant input node (1, -1, or linear) or an operation node with all (both) arguments constant
    bool isConstant() const;

    void setLHSVariable(const Variable& aVariable);
    const Variable& getLHSVariable() const;
    const bool hasLHSVariable() const;
 
    std::string debug() const;

    /// in human-readable format, for GraphViz.
    std::string
    getColorString() const;

    /// in human-readable format, for GraphViz.  returns an empty string if it's not const
    std::string
    getValueString() const;

    /// in human-readable format, for GraphViz.
    std::string
    getOperationString() const;

    /// in human-readable format, for GraphViz.
    std::string
    getRHSString() const;

    /// in human-readable format, for GraphViz.
    std::string
    getLHSString() const;

    /// in human-readable format, for GraphViz.
    std::string
    getLabelString() const;

  private:

    AccumulationGraphVertex (const AccumulationGraphVertex&);
    AccumulationGraphVertex& operator=(const AccumulationGraphVertex&);

    /**
     * for accumulation graph inputs that correspond to (computed) edge labels in the computational graph,
     * this points to the corresponding original ExpressionEdge.  May or may not be set.
     */
    const ExpressionEdge* myExpressionEdge_p;

    /**
     * Pointer to corresponding edge in the remainder graph.  May or may not be set.
     * When set, this Acc graph vertex must have some LHS set, which is used in propagation.
     */
    const LinearizedComputationalGraphEdge* myRemainderGraphEdge_p;

    const bool myHasOperationFlag;

    /**
     * currently either ADD or MULT, only for non-leaf vertices.
     */
    JacobianAccumulationExpressionVertex::Operation_E myOperationType;

    /**
     *  Set either during the constructor (for leaves)
     *  or during the topological traversal (for non-leaves)
     */
    PartialDerivativeKind::PartialDerivativeKind_E myPartialDerivativeKind;

    /**
     * stores the value for vertices whose value can be computed
     * (those with LINEAR PDK, for all other PDKs the value is implicit)
     */
    double myValue;

    /**
     * Flag that indicates whether the value was set to something in particular (its initialized to zero,
     * but the flag tells us whether or not it was set to zero specifically).
     */
    bool ValueHasBeenSet;

    /**
     * MUST be set for accumulation graph vertices that either
     * - correspond to an edge in the remainder graph or
     * - are non-leaves with >1 outedge or
     * - are NONLINEAR leaves
     * May be set for non-leaves with one outedge whose LHSVARIABLE is identical to that of a child
     * (like if it's NONLINEAR+PASSIVE or NONLINEAR*LINEAR_ONE).
     * We do not own this variable.
     */
    const Variable* myLHSVariable_p;

  }; // end of class AccumulationGraphVertex

  typedef std::set<const AccumulationGraphVertex*> CAccumulationGraphVertexPSet;

} // end namespace xaifBoosterCrossCountryInterface
                       
#endif

