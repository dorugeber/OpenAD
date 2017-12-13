#ifndef _ENDLOOPALGBASE_INCLUDE_
#define _ENDLOOPALGBASE_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/ControlFlowGraphVertexAlgBase.hpp"

namespace xaifBooster {  
  
  class EndLoop;

  class EndLoopAlgBase: public ControlFlowGraphVertexAlgBase { 

  public: 

    EndLoopAlgBase(const EndLoop& theContaining);

    virtual ~EndLoopAlgBase();

  private: 

    /** 
     * not defined
     */
    EndLoopAlgBase();

    /** 
     * not defined
     */
    EndLoopAlgBase(const EndLoopAlgBase&);

    /** 
     * not defined
     */
    EndLoopAlgBase& operator=(const EndLoopAlgBase&);

  }; 

} // end of namespace 

#endif
