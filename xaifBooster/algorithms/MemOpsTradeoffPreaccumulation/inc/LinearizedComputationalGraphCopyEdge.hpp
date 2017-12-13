#ifndef _LINEARIZEDCOMPUTATIONALGRAPHCOPYEDGE_INCLUDE_
#define _LINEARIZEDCOMPUTATIONALGRAPHCOPYEDGE_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/boostWrapper/inc/Edge.hpp"
#include "xaifBooster/algorithms/CrossCountryInterface/inc/LinearizedComputationalGraphEdge.hpp"
#include "xaifBooster/algorithms/MemOpsTradeoffPreaccumulation/inc/JacobianAccumulationExpressionCopy.hpp"

using namespace xaifBoosterCrossCountryInterface;

namespace MemOpsTradeoffPreaccumulation {
 
  class LinearizedComputationalGraphCopyEdge : public Edge {
  
  public:
    /*
     * constructor sets the jacobianref to null, so that when the destructor executes, jacobianref will
     * only be deleted if it has been set in the first place.
     */
    LinearizedComputationalGraphCopyEdge():jacobianRef(NULL) {};

    ~LinearizedComputationalGraphCopyEdge(){
      if(jacobianRef){
	delete jacobianRef;
      }
    };

    enum EdgeCopyRefType { TO_ORIGINAL_EDGE,
			   TO_INTERNAL_EXPRESSION};

    /**
     * these functions set either the jacobianref or original ref pointers.  an edge can only point to one or the other.
     * these functions also set mycopyreferencetype to the corrresponding type.
     */
    void setOriginalRef(const LinearizedComputationalGraphEdge& theOriginalEdge);
    void setJacobianRef(MemOpsTradeoffPreaccumulation::JacobianAccumulationExpressionCopy* theJacobianExpression_pt);

    /**
     * these functions return the original ref, the jacobian ref, or the ref type respectively.
     */
    const LinearizedComputationalGraphEdge& getOriginalRef() const;
    MemOpsTradeoffPreaccumulation::JacobianAccumulationExpressionCopy& getJacobianRef() const;
    EdgeCopyRefType getRefType() const;

  private:
    
    const LinearizedComputationalGraphEdge* originalRef;
    MemOpsTradeoffPreaccumulation::JacobianAccumulationExpressionCopy* jacobianRef;

    EdgeCopyRefType myCopyReferenceType;

  }; // end of class LinearizedComputationalGraphCopyEdge
 
} //end of namespace MemOpsTradeoffPreaccumulation
                                                                     
#endif
