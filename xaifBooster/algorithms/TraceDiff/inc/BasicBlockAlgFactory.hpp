#ifndef _XAIFBOOSTERTRACEDIFF_BASICBLOCKALGFACTORY_INCLUDE_
#define _XAIFBOOSTERTRACEDIFF_BASICBLOCKALGFACTORY_INCLUDE_
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
#include "xaifBooster/algorithms/ControlFlowReversal/inc/BasicBlockAlgFactory.hpp"
#include "xaifBooster/algorithms/TraceDiff/inc/BasicBlockAlg.hpp"
#include "xaifBooster/algorithms/TraceDiff/inc/AlgFactoryManager.hpp"


  DERIVED_ALG_FACTORY_DECL_MACRO(BasicBlock, xaifBoosterControlFlowReversal::BasicBlockAlgFactory, xaifBoosterTraceDiff)


#endif
