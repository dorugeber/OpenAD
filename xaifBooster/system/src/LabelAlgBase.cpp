// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/system/inc/LabelAlgBase.hpp"
#include "xaifBooster/system/inc/Label.hpp"

namespace xaifBooster {  
  
  LabelAlgBase::LabelAlgBase(const Label& theContaining) : 
    ControlFlowGraphVertexAlgBase(theContaining) { 
  }

  LabelAlgBase::~LabelAlgBase(){}

} 
