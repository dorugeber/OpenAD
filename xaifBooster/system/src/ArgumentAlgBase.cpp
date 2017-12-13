// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/system/inc/ArgumentAlgBase.hpp"
#include "xaifBooster/system/inc/Argument.hpp"

namespace xaifBooster {  
  
  ArgumentAlgBase::ArgumentAlgBase(const Argument& theContaining) : 
    ExpressionVertexAlgBase(theContaining) { 
  }

  ArgumentAlgBase::~ArgumentAlgBase(){}

} 
