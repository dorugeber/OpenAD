#ifndef _BOOLEANOPERATIONALGBASE_INCLUDE_
#define _BOOLEANOPERATIONALGBASE_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/ExpressionVertexAlgBase.hpp"

namespace xaifBooster {  
  
  class BooleanOperation;

  class BooleanOperationAlgBase: public virtual ExpressionVertexAlgBase { 

  public: 

    BooleanOperationAlgBase(const BooleanOperation& theContaining);

    virtual ~BooleanOperationAlgBase();

  private: 

    /** 
     * not defined
     */
    BooleanOperationAlgBase();

    /** 
     * not defined
     */
    BooleanOperationAlgBase(const BooleanOperationAlgBase&);

    /** 
     * not defined
     */
    BooleanOperationAlgBase& operator=(const BooleanOperationAlgBase&);

  }; 

} // end of namespace 

#endif
