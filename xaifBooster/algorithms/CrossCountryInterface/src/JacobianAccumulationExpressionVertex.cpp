// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/utils/inc/LogicException.hpp"
#include "xaifBooster/algorithms/CrossCountryInterface/inc/JacobianAccumulationExpressionVertex.hpp"

namespace xaifBoosterCrossCountryInterface {

  JacobianAccumulationExpressionVertex::JacobianAccumulationExpressionVertex() : 
    myReferenceUnionType(NOTHING) { 
  } 

  void 
  JacobianAccumulationExpressionVertex::setExternalReference(const LinearizedComputationalGraphEdge& theExternalEdge) { 
    if (myReferenceUnionType!=NOTHING) 
      THROW_LOGICEXCEPTION_MACRO("JacobianAccumulationExpressionVertex::setExternalReference: already set");
    myReferenceUnionType=EXTERNAL_REF;
    // conceptually const 
    myReferenceUnion.myExternal_p=const_cast<LinearizedComputationalGraphEdge*>(&theExternalEdge);
  }

  void
  JacobianAccumulationExpressionVertex::setInternalReference(const JacobianAccumulationExpressionVertex& theInternalVertex) { 
    if (myReferenceUnionType!=NOTHING) 
      THROW_LOGICEXCEPTION_MACRO("JacobianAccumulationExpressionVertex::setInternalReference: already set");
    myReferenceUnionType=INTERNAL_REF;
    // conceptually const 
    myReferenceUnion.myInternal_p=const_cast<JacobianAccumulationExpressionVertex*>(&theInternalVertex);
  }
    
  const LinearizedComputationalGraphEdge&
  JacobianAccumulationExpressionVertex::getExternalReference() const { 
    if (myReferenceUnionType!=EXTERNAL_REF ) 
      THROW_LOGICEXCEPTION_MACRO("JacobianAccumulationExpressionVertex::getExternalReference: not set");
    return *(myReferenceUnion.myExternal_p);
  }

  const JacobianAccumulationExpressionVertex&
  JacobianAccumulationExpressionVertex::getInternalReference() const {
    if (myReferenceUnionType!=INTERNAL_REF ) 
      THROW_LOGICEXCEPTION_MACRO("JacobianAccumulationExpressionVertex::getInternalReference: not set");
    return *(myReferenceUnion.myInternal_p);
  }
    
  JacobianAccumulationExpressionVertex::Operation_E 
  JacobianAccumulationExpressionVertex::getOperation() const {
    if (myReferenceUnionType!=OPERATION ) 
      THROW_LOGICEXCEPTION_MACRO("JacobianAccumulationExpressionVertex::getOperation: not set");
    return myReferenceUnion.myOperation;
  }

  void  
  JacobianAccumulationExpressionVertex::setOperation(JacobianAccumulationExpressionVertex::Operation_E anOperation) {
    if (myReferenceUnionType!=NOTHING) 
      THROW_LOGICEXCEPTION_MACRO("JacobianAccumulationExpressionVertex::setOperation: already set");
    myReferenceUnionType=OPERATION;
    myReferenceUnion.myOperation=anOperation;
  }

  JacobianAccumulationExpressionVertex::ReferenceType_E 
  JacobianAccumulationExpressionVertex::getReferenceType() const {
    return myReferenceUnionType;
  }

} 
