// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/system/inc/MarkerAlgBase.hpp"
#include "xaifBooster/system/inc/Marker.hpp"

namespace xaifBooster {  
  
  MarkerAlgBase::MarkerAlgBase(const Marker& theContaining) : 
    BasicBlockElementAlgBase(theContaining) { 
  }

  MarkerAlgBase::~MarkerAlgBase(){}

} 
