#ifndef _FORLOOPALGBASE_INCLUDE_
#define _FORLOOPALGBASE_INCLUDE_
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
  
  class ForLoop;

  class ForLoopAlgBase: public ControlFlowGraphVertexAlgBase { 

  public: 

    ForLoopAlgBase(const ForLoop& theContaining);

    virtual ~ForLoopAlgBase();

  private: 

    /** 
     * not defined
     */
    ForLoopAlgBase();

    /** 
     * not defined
     */
    ForLoopAlgBase(const ForLoopAlgBase&);

    /** 
     * not defined
     */
    ForLoopAlgBase& operator=(const ForLoopAlgBase&);

  }; 

} // end of namespace 

#endif
