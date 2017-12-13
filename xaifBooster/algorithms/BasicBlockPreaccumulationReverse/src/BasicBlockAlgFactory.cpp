// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/system/inc/BasicBlockAlgBase.hpp"
#include "xaifBooster/system/inc/BasicBlock.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulationReverse/inc/BasicBlockAlgFactory.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulationReverse/inc/BasicBlockAlg.hpp"


namespace xaifBoosterBasicBlockPreaccumulationReverse { 
  BasicBlockAlgBase* BasicBlockAlgFactory::makeNewAlg(BasicBlock& theContaining) { 
    return dynamic_cast<xaifBoosterControlFlowReversal::BasicBlockAlg*>(new BasicBlockAlg(theContaining)); 
  } 
  xaifBooster :: BasicBlockAlgFactory* BasicBlockAlgFactory::instance() { 
    return AlgFactoryManager::instance()->getBasicBlockAlgFactory(); 
  } 
  std::string BasicBlockAlgFactory::debug() const { 
    std::ostringstream out; 
    out << "xaifBoosterBasicBlockPreaccumulationReverse" 
	<< "::" 
	<< "BasicBlock" 
	<< "AlgFactory["
	<< this 
	<< "]" 
	<< std::ends; 
    return out.str(); 
  } 
}

// because of the extra cast in makeNewAlg needed to resolve base class ambiguity 
// we cannot use the following macro:
//  DERIVED_ALG_FACTORY_DEF_MACRO(BasicBlock,xaifBoosterBasicBlockPreaccumulationReverse);


