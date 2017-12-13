#ifndef _ENDBRANCHALGBASE_INCLUDE_
#define _ENDBRANCHALGBASE_INCLUDE_
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
  
  class EndBranch;

  class EndBranchAlgBase: public ControlFlowGraphVertexAlgBase { 

  public: 

    EndBranchAlgBase(const EndBranch& theContaining);

    virtual ~EndBranchAlgBase();

  private: 

    /** 
     * not defined
     */
    EndBranchAlgBase();

    /** 
     * not defined
     */
    EndBranchAlgBase(const EndBranchAlgBase&);

    /** 
     * not defined
     */
    EndBranchAlgBase& operator=(const EndBranchAlgBase&);

  }; 

} // end of namespace 

#endif
