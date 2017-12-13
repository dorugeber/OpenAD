// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/algorithms/CrossCountryInterface/inc/JacobianAccumulationExpressionList.hpp"

namespace xaifBoosterCrossCountryInterface {

  JacobianAccumulationExpressionList::~JacobianAccumulationExpressionList() {
    for (GraphList::iterator it=myGraphList.begin();
	 it!=myGraphList.end();
	 it++) { 
      if (*it)
	delete (*it);
    } 
  } 

  JacobianAccumulationExpression& 
  JacobianAccumulationExpressionList::addExpression() { 
    // deallocated in dtor
    JacobianAccumulationExpression* theGraph_p=new JacobianAccumulationExpression();
    myGraphList.push_back(theGraph_p);
    return *theGraph_p;
  }

  const JacobianAccumulationExpressionList::GraphList& 
  JacobianAccumulationExpressionList::getGraphList() const { 
    return myGraphList;
  } 

} 
