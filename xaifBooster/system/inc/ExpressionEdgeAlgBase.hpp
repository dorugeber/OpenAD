#ifndef _EXPRESSIONEDGEALGBASE_INCLUDE_
#define _EXPRESSIONEDGEALGBASE_INCLUDE_
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
  class ExpressionEdge;
  
  typedef AlgBaseGraphElement<ExpressionEdge,Expression> ExpressionEdgeAlgBase;

} // end of namespace 

#endif
