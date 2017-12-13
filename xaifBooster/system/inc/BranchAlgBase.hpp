#ifndef _BRANCHALGBASE_INCLUDE_
#define _BRANCHALGBASE_INCLUDE_
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
  
  class Branch;

  class BranchAlgBase: public ControlFlowGraphVertexAlgBase { 

  public: 

    BranchAlgBase(const Branch& theContaining);

    virtual ~BranchAlgBase();

  private: 

    /** 
     * not defined
     */
    BranchAlgBase();

    /** 
     * not defined
     */
    BranchAlgBase(const BranchAlgBase&);

    /** 
     * not defined
     */
    BranchAlgBase& operator=(const BranchAlgBase&);

  }; 

} // end of namespace 

#endif
