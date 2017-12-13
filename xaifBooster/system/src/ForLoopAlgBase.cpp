// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/system/inc/ForLoopAlgBase.hpp"
#include "xaifBooster/system/inc/ForLoop.hpp"

namespace xaifBooster {  
  
  ForLoopAlgBase::ForLoopAlgBase(const ForLoop& theContaining) : 
    ControlFlowGraphVertexAlgBase(theContaining) { 
  }

  ForLoopAlgBase::~ForLoopAlgBase(){}

} 
