#ifndef _JACOBIANACCUMULATIONEXPRESSIONCOPY_INCLUDE_
#define _JACOBIANACCUMULATIONEXPRESSIONCOPY_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/algorithms/CrossCountryInterface/inc/JacobianAccumulationExpression.hpp"

using namespace xaifBoosterCrossCountryInterface;

namespace MemOpsTradeoffPreaccumulation {

  class JacobianAccumulationExpressionCopy {
  
  public:

    /**
     * Constructor sets the expression member of the class upon creation
     */
    JacobianAccumulationExpressionCopy(JacobianAccumulationExpression& theNewExpression): myExpression(theNewExpression){};
    
    /**
     * sets the myMaximal pointer to the maximal vertex of the expression
     */
    void setMaximal(const JacobianAccumulationExpressionVertex& theMaximal);

    /**
     * returns a reference to the maximal vertex of the expression
     */
    JacobianAccumulationExpressionVertex& getMaximal() const;

    JacobianAccumulationExpression& myExpression;

  private: 

    JacobianAccumulationExpressionVertex* myMaximal;

  }; // end of class JacobianAccumulationExpressionCopy

} // end of namespace MemOpsTradeoffPreaccumulation
                                                                     
#endif
