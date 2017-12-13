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
#include "xaifBooster/algorithms/TraceDiff/inc/BasicBlockAlgFactory.hpp"
#include "xaifBooster/algorithms/TraceDiff/inc/BasicBlockAlg.hpp"


namespace xaifBoosterTraceDiff { 
  BasicBlockAlgBase* BasicBlockAlgFactory::makeNewAlg(BasicBlock& theContaining) { 
    return dynamic_cast<BasicBlockAlg*>(new BasicBlockAlg(theContaining)); 
  } 
  xaifBooster :: BasicBlockAlgFactory* BasicBlockAlgFactory::instance() { 
    return AlgFactoryManager::instance()->getBasicBlockAlgFactory(); 
  } 
  std::string BasicBlockAlgFactory::debug() const { 
    std::ostringstream out; 
    out << "xaifBoosterTraceDiff" 
	<< "::" 
	<< "BasicBlock" 
	<< "AlgFactory["
	<< this 
	<< "]" 
	<< std::ends; 
    return out.str(); 
  } 
}

