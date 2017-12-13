#ifndef _PRELOOPALGBASE_INCLUDE_
#define _PRELOOPALGBASE_INCLUDE_
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
  
  class PreLoop;

  class PreLoopAlgBase: public ControlFlowGraphVertexAlgBase { 

  public: 

    PreLoopAlgBase(const PreLoop& theContaining);

    virtual ~PreLoopAlgBase();

  private: 

    /** 
     * not defined
     */
    PreLoopAlgBase();

    /** 
     * not defined
     */
    PreLoopAlgBase(const PreLoopAlgBase&);

    /** 
     * not defined
     */
    PreLoopAlgBase& operator=(const PreLoopAlgBase&);

  }; 

} // end of namespace 

#endif
