#ifndef _EXITALGBASE_INCLUDE_
#define _EXITALGBASE_INCLUDE_
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
  
  class Exit;

  class ExitAlgBase: public ControlFlowGraphVertexAlgBase { 

  public: 

    ExitAlgBase(const Exit& theContaining);

    virtual ~ExitAlgBase();

  private: 

    /** 
     * not defined
     */
    ExitAlgBase();

    /** 
     * not defined
     */
    ExitAlgBase(const ExitAlgBase&);

    /** 
     * not defined
     */
    ExitAlgBase& operator=(const ExitAlgBase&);

  }; 

} // end of namespace 

#endif
