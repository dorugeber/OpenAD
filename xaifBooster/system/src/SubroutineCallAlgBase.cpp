// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/system/inc/SubroutineCallAlgBase.hpp"
#include "xaifBooster/system/inc/SubroutineCall.hpp"

namespace xaifBooster {  
  
  SubroutineCallAlgBase::SubroutineCallAlgBase(const SubroutineCall& theContaining) : 
    BasicBlockElementAlgBase(theContaining) { 
  }

  SubroutineCallAlgBase::~SubroutineCallAlgBase(){}

  const SubroutineCall& SubroutineCallAlgBase::getContainingSubroutineCall() const { 
    return dynamic_cast<const SubroutineCall&>(getContaining());
  }

} 
