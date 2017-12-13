#ifndef _INLINABLEINTRINSICSARGUMENTREFERENCE_INCLUDE_
#define _INLINABLEINTRINSICSARGUMENTREFERENCE_INCLUDE_
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

namespace xaifBooster { 

  /**
   * a class for a vertex representing an argument in an 
   * intrinsic
   */
  class InlinableIntrinsicsArgumentReference : public InlinableIntrinsicsExpressionVertex  { 
  
  public:

    InlinableIntrinsicsArgumentReference (unsigned int aPosition) : 
      myPosition(aPosition) {};
    ~InlinableIntrinsicsArgumentReference(){};

    /** 
     * this will always throw an exception since
     * we expect to make an explicit VariableReference
     * and cannot just plainly copy this
     */
    virtual ExpressionVertex& createCopyOfMyself() const;

    static const std::string ourXAIFName;
    static const std::string our_myArgRef_XAIFName;
    static const std::string our_myId_XAIFName;

  private:
    
    /**
     * the position in the argument list
     * counting starts with 1
     */
    unsigned int myPosition;

  }; // end of class InlinableIntrinsicsArgumentReference
 
} // end of namespace xaifBooster
                                                                     
#endif
