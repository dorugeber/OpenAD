// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/system/inc/BranchAlgBase.hpp"
#include "xaifBooster/system/inc/Branch.hpp"

namespace xaifBooster {  
  
  BranchAlgBase::BranchAlgBase(const Branch& theContaining) : 
    ControlFlowGraphVertexAlgBase(theContaining) { 
  }

  BranchAlgBase::~BranchAlgBase(){}

} 
