// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/system/inc/EntryAlgBase.hpp"
#include "xaifBooster/system/inc/Entry.hpp"

namespace xaifBooster {  
  
  EntryAlgBase::EntryAlgBase(const Entry& theContaining) : 
    ControlFlowGraphVertexAlgBase(theContaining) { 
  }

  EntryAlgBase::~EntryAlgBase(){}

} 
