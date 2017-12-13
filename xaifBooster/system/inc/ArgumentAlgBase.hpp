#ifndef _ARGUMENTALGBASE_INCLUDE_
#define _ARGUMENTALGBASE_INCLUDE_
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
  
  class Argument;

  class ArgumentAlgBase: public virtual ExpressionVertexAlgBase { 

  public: 

    ArgumentAlgBase(const Argument& theContaining);

    virtual ~ArgumentAlgBase();

  private: 

    /** 
     * not defined
     */
    ArgumentAlgBase();

    /** 
     * not defined
     */
    ArgumentAlgBase(const ArgumentAlgBase&);

    /** 
     * not defined
     */
    ArgumentAlgBase& operator=(const ArgumentAlgBase&);

  }; 

} // end of namespace 

#endif
