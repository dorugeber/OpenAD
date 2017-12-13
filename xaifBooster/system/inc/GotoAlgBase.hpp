#ifndef _GOTOALGBASE_INCLUDE_
#define _GOTOALGBASE_INCLUDE_
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
  
  class Goto;

  class GotoAlgBase: public ControlFlowGraphVertexAlgBase { 

  public: 

    GotoAlgBase(const Goto& theContaining);

    virtual ~GotoAlgBase();

  private: 

    /** 
     * not defined
     */
    GotoAlgBase();

    /** 
     * not defined
     */
    GotoAlgBase(const GotoAlgBase&);

    /** 
     * not defined
     */
    GotoAlgBase& operator=(const GotoAlgBase&);

  }; 

} // end of namespace 

#endif
