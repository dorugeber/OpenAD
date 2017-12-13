#ifndef _XAIFBOOSTERBASICBLOCKPREACCUMULATIONREVERSE_SUBROUTINECALLALGFACTORY_INCLUDE_
#define _XAIFBOOSTERBASICBLOCKPREACCUMULATIONREVERSE_SUBROUTINECALLALGFACTORY_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/SubroutineCall.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulationTapeAdjoint/inc/SubroutineCallAlgFactory.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulationReverse/inc/SubroutineCallAlg.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulationReverse/inc/AlgFactoryManager.hpp"


  DERIVED_ALG_FACTORY_DECL_MACRO(SubroutineCall,
				 xaifBoosterBasicBlockPreaccumulationTapeAdjoint::SubroutineCallAlgFactory,
				 xaifBoosterBasicBlockPreaccumulationReverse)


#endif
