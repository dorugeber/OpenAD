#ifndef _INTRINSICALGBASE_INCLUDE_
#define _INTRINSICALGBASE_INCLUDE_
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
  
  class Intrinsic;

  class IntrinsicAlgBase: public virtual ExpressionVertexAlgBase { 

  public: 

    IntrinsicAlgBase(const Intrinsic& theContaining);

    virtual ~IntrinsicAlgBase();

  private: 

    /** 
     * not defined
     */
    IntrinsicAlgBase();

    /** 
     * not defined
     */
    IntrinsicAlgBase(const IntrinsicAlgBase&);

    /** 
     * not defined
     */
    IntrinsicAlgBase& operator=(const IntrinsicAlgBase&);

  }; 

} // end of namespace 

#endif
