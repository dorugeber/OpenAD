#ifndef _DUALGRAPHVERTEX_INCLUDE_
#define _DUALGRAPHVERTEX_INCLUDE_
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
#include "xaifBooster/algorithms/CrossCountryInterface/inc/LinearizedComputationalGraph.hpp"
#include "xaifBooster/algorithms/MemOpsTradeoffPreaccumulation/inc/JacobianAccumulationExpressionCopy.hpp"

using namespace xaifBoosterCrossCountryInterface;

namespace MemOpsTradeoffPreaccumulation {
 
  class DualGraphVertex : public Vertex {
  
  public:
    /*
     * constructor sets the jacobianref to null, so that when the destructor executes, jacobianref will
     * only be deleted if it has been set in the first place.
     */
    DualGraphVertex(){
      final = false;
      jacobianRef = NULL;
    };

    ~DualGraphVertex(){
      if(jacobianRef){
	delete jacobianRef;
      }
    };

    bool final;

    enum DualVertexRefType { TO_ORIGINAL_EDGE,
			     TO_ASSUMED_INEDGE,
			     TO_ASSUMED_OUTEDGE,
			     TO_INTERNAL_EXPRESSION};

    /**
     * these functions set either the jacobianref, assumed ref or original ref pointers.
     * these functions also set mycopyreferencetype to the corrresponding type.
     */
    void setOriginalRef(const LinearizedComputationalGraphEdge& theOriginalEdge);
    void setAssumedInEdgeRef(const LinearizedComputationalGraphVertex& theAssumedInEdgeTarget);
    void setAssumedOutEdgeRef(const LinearizedComputationalGraphVertex& theAssumedOutEdgeSource);
    void setJacobianRef(MemOpsTradeoffPreaccumulation::JacobianAccumulationExpressionCopy* theJacobianExpression_pt);

    /**
     * these functions return the original ref, the jacobian ref, an assumed ref, or the ref type respectively.
     */
    const LinearizedComputationalGraphEdge& getOriginalRef() const;
    const LinearizedComputationalGraphVertex& getAssumedRef() const;
    MemOpsTradeoffPreaccumulation::JacobianAccumulationExpressionCopy& getJacobianRef() const;

    DualVertexRefType getRefType() const;

  private:
    
    const LinearizedComputationalGraphEdge* originalRef;
    const LinearizedComputationalGraphVertex* assumedRef;
    MemOpsTradeoffPreaccumulation::JacobianAccumulationExpressionCopy* jacobianRef;

    DualVertexRefType myDualReferenceType;

  }; // end of class
 
} //end of namespace MemOpsTradeoffPreaccumulation
                                                                     
#endif
