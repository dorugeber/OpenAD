#ifndef _CONSTANTALGBASE_INCLUDE_
#define _CONSTANTALGBASE_INCLUDE_
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
  
  class Constant;

  class ConstantAlgBase: public virtual ExpressionVertexAlgBase { 

  public: 

    ConstantAlgBase(const Constant& theContaining);

    virtual ~ConstantAlgBase();

  private: 

    /** 
     * not defined
     */
    ConstantAlgBase();

    /** 
     * not defined
     */
    ConstantAlgBase(const ConstantAlgBase&);

    /** 
     * not defined
     */
    ConstantAlgBase& operator=(const ConstantAlgBase&);

  }; 

} // end of namespace 

#endif
