// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/system/inc/ConstantAlgBase.hpp"
#include "xaifBooster/system/inc/Constant.hpp"

namespace xaifBooster {  
  
  ConstantAlgBase::ConstantAlgBase(const Constant& theContaining) : 
    ExpressionVertexAlgBase(theContaining) { 
  }

  ConstantAlgBase::~ConstantAlgBase(){}

} 
