#ifndef _JACOBIANACCUMULATIONEXPRESSIONVERTEX_INCLUDE_
#define _JACOBIANACCUMULATIONEXPRESSIONVERTEX_INCLUDE_
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
#include "xaifBooster/algorithms/CrossCountryInterface/inc/LinearizedComputationalGraphEdge.hpp"

using namespace xaifBooster;

namespace xaifBoosterCrossCountryInterface { 

  class JacobianAccumulationExpressionVertex : public Vertex { 
    
  public: 

    JacobianAccumulationExpressionVertex();
    ~JacobianAccumulationExpressionVertex(){};

    void setExternalReference(const LinearizedComputationalGraphEdge& theExternalEdge);
    void setInternalReference(const JacobianAccumulationExpressionVertex& theInternalVertex);

    const LinearizedComputationalGraphEdge&
    getExternalReference() const ;

    const JacobianAccumulationExpressionVertex&
    getInternalReference() const;
    
    enum Operation_E { ADD_OP,
		       MULT_OP,
		       DIV_OP };
      
    void setOperation(Operation_E);

    Operation_E getOperation() const;

    enum ReferenceType_E { NOTHING,
			   INTERNAL_REF,
			   EXTERNAL_REF,
			   OPERATION}; 
    
    ReferenceType_E getReferenceType() const;

  private: 


    /**
     * Any vertex in the list of graphs held in 
     * JacobianAccumulationExpressionList
     * can possibly point to a vertex of another 
     * graph in the list  (internal) in which case it 
     * has to be the maximal vertex in that graph or 
     * to an edge in the LinearizedComputationalGraph
     * or to nothing at all
     */
    union Reference { 
      JacobianAccumulationExpressionVertex* myInternal_p;
      LinearizedComputationalGraphEdge* myExternal_p;
      Operation_E myOperation;      
    } myReferenceUnion;

    ReferenceType_E  myReferenceUnionType;

  }; // end of class  JacobianAccumulationExpressionVertex

} 

#endif 
