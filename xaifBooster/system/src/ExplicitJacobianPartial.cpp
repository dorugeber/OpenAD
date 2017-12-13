// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/utils/inc/LogicException.hpp"
#include "xaifBooster/system/inc/ExplicitJacobianPartial.hpp"

namespace xaifBooster { 

  const std::string ExplicitJacobianPartial::ourXAIFName("xaifnii:Partial");
  const std::string ExplicitJacobianPartial::our_myDep_XAIFName("dep");
  const std::string ExplicitJacobianPartial::our_myInDep_XAIFName("indep");
  const std::string ExplicitJacobianPartial::our_myPartial_XAIFName("partial");

  ExplicitJacobianPartial::ExplicitJacobianPartial() : 
    myDep(0),
    myInDep(0),
    myPartial(0) { 
  } 

  ExplicitJacobianPartial::~ExplicitJacobianPartial() { 
  } 

  void ExplicitJacobianPartial::setInfo(unsigned short dep,
					unsigned short indep,
					unsigned short partial) {
    if (myDep || myInDep || myPartial)
      THROW_LOGICEXCEPTION_MACRO("ExplicitJacobianPartial::setInfo: already set");
    myDep=dep;
    myInDep=indep;
    myPartial=partial;
  } 
    
  unsigned short ExplicitJacobianPartial::getDep() const { 
    if (! myDep)
      THROW_LOGICEXCEPTION_MACRO("ExplicitJacobianPartial::getDep: not set");
    return myDep;
  }
  
  unsigned short ExplicitJacobianPartial::getInDep() const { 
    if (! myInDep)
      THROW_LOGICEXCEPTION_MACRO("ExplicitJacobianPartial::getInDep: not set");
    return myInDep;
  }
  
  unsigned short ExplicitJacobianPartial::getPartial() const { 
    if (! myPartial)
      THROW_LOGICEXCEPTION_MACRO("ExplicitJacobianPartial::getPartial: not set");
    return myPartial;
  }

} 
