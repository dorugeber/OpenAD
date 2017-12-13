#ifndef _INLINABLEINTRINSICSINTERMEDIATEREFERENCE_INCLUDE_
#define _INLINABLEINTRINSICSINTERMEDIATEREFERENCE_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/InlinableIntrinsicsExpressionVertex.hpp"
#include "xaifBooster/system/inc/InlinableIntrinsicsExpression.hpp"

namespace xaifBooster { 

  /**
   * a class for a vertex representing an Intermediate in an 
   * intrinsic, i.e. any vertex in an Expression graph 
   * this class takes care of 
   * xaifii:IntermediateFunctionReferenceType
   * xaifii:IntermediatePartialReferenceType
   * xaifii:PartialReferenceType
   * xaifii:ResultReferenceType
   */
  class InlinableIntrinsicsIntermediateReference : public InlinableIntrinsicsExpressionVertex  { 
  
  public:

    InlinableIntrinsicsIntermediateReference (){};
    ~InlinableIntrinsicsIntermediateReference(){};

  private:
    
    /**
     * intially for readout from the XML file only, 
     * refers to the index into 
     * InlinableIntrinsicsCatalogueItem::myExpressionList
     */
    unsigned int myRefExpressionIndex;

    /** 
     * intially for readout from the XML file only, 
     * the id of the vertex we are referring to
     * it may not be set for references to the 
     * maximal node in the referred to Expression
     */
    unsigned int myRefVertexId;

    /** 
     * a separate traversal is need to set up these pointers
     * once all Expressions for this intrinsic have been read
     * in order to deal with forward references etc.
     */
    InlinableIntrinsicsExpression* myExpressionReference_p;

    /** 
     * a separate traversal is need to set up these pointers
     * once all Expressions for this intrinsic have been read
     * in order to deal with forward references etc.
     */
    InlinableIntrinsicsExpressionVertex* myVertex_p;

  }; // end of class InlinableIntrinsicsIntermediateReference
 
} // end of namespace xaifBooster
                                                                     
#endif
