// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/system/inc/ExplicitJacobian.hpp"

namespace xaifBooster { 

  const std::string ExplicitJacobian::ourXAIFName("xaifnii:ExplicitJacobian");
  const std::string ExplicitJacobian::our_myJacobian_XAIFName("jacobian");

  ExplicitJacobian::ExplicitJacobian() { 
  } 

  ExplicitJacobian::~ExplicitJacobian() { 
    for(ExplicitJacobianPartialPList::iterator it= myExplicitJacobianPartialList.begin();
	it!=myExplicitJacobianPartialList.end();
	++it) 
      if (*it)
	delete (*it);
  } 

  ExplicitJacobianPartial& ExplicitJacobian::addExplicitJacobianPartial() { 
    myExplicitJacobianPartialList.push_back(new ExplicitJacobianPartial());
    return *(myExplicitJacobianPartialList.back());
  } 

} 
