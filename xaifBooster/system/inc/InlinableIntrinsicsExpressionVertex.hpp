#ifndef _INLINABLEINTRINSICSEXPRESSIONVERTEX_INCLUDE_
#define _INLINABLEINTRINSICSEXPRESSIONVERTEX_INCLUDE_
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
#include "xaifBooster/utils/inc/ObjectWithId.hpp"

namespace xaifBooster { 

  class ExpressionVertex;

  /**
   * \todo: inherit from ID once it is factored out of 
   * XMLPrintableWIthId
   */
  class InlinableIntrinsicsExpressionVertex : public Vertex, 
					      public ObjectWithId { 

  public:

    InlinableIntrinsicsExpressionVertex (){};
    ~InlinableIntrinsicsExpressionVertex(){};

    virtual std::string debug() const;
    
    /**
     * \todo switch to using ID rather than descriptor
     */
    virtual ExpressionVertex& createCopyOfMyself() const =0;

    }; // end of class InlinableIntrinsicsExpressionVertex
 
} // end of namespace xaifBooster
                                                                     
#endif
