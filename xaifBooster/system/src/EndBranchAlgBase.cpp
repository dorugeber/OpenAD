// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/system/inc/EndBranchAlgBase.hpp"
#include "xaifBooster/system/inc/EndBranch.hpp"

namespace xaifBooster {  
  
  EndBranchAlgBase::EndBranchAlgBase(const EndBranch& theContaining) : 
    ControlFlowGraphVertexAlgBase(theContaining) { 
  }

  EndBranchAlgBase::~EndBranchAlgBase(){}

} 
