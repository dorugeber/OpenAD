#ifndef _EXPRESSIONVERTEXALGBASE_INCLUDE_
#define _EXPRESSIONVERTEXALGBASE_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/AlgBaseGraphElement.hpp"

namespace xaifBooster {  
  
  class Expression;
  class ExpressionVertex;
  
  typedef AlgBaseGraphElement<ExpressionVertex,Expression> ExpressionVertexAlgBase;

} // end of namespace 

#endif
