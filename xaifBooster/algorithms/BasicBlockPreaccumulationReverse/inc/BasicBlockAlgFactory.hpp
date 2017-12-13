#ifndef _XAIFBOOSTERBASICBLOCKPREACCUMULATIONREVERSE_BASICBLOCKALGFACTORY_INCLUDE_
#define _XAIFBOOSTERBASICBLOCKPREACCUMULATIONREVERSE_BASICBLOCKALGFACTORY_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/BasicBlock.hpp"
#include "xaifBooster/algorithms/PushPop/inc/BasicBlockAlgFactory.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulationReverse/inc/BasicBlockAlg.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulationReverse/inc/AlgFactoryManager.hpp"


  DERIVED_ALG_FACTORY_DECL_MACRO(BasicBlock,
                                 xaifBoosterPushPop::BasicBlockAlgFactory,
				 xaifBoosterBasicBlockPreaccumulationReverse)


#endif
