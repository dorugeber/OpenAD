// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/system/inc/SubroutineCallAlgBase.hpp"
#include "xaifBooster/system/inc/SubroutineCall.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulationReverse/inc/SubroutineCallAlgFactory.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulationReverse/inc/SubroutineCallAlg.hpp"


namespace xaifBoosterBasicBlockPreaccumulationReverse { 
  SubroutineCallAlgBase* SubroutineCallAlgFactory::makeNewAlg(SubroutineCall& theContaining) { 
    return dynamic_cast<xaifBoosterBasicBlockPreaccumulationTapeAdjoint::SubroutineCallAlg*>(new SubroutineCallAlg(theContaining)); 
  }
 
  xaifBooster :: SubroutineCallAlgFactory* SubroutineCallAlgFactory::instance() { 
    return AlgFactoryManager::instance()->getSubroutineCallAlgFactory(); 
  } 

  std::string SubroutineCallAlgFactory::debug() const { 
    std::ostringstream out; 
    out << "xaifBoosterBasicBlockPreaccumulationReverse" 
	<< "::" 
	<< "SubroutineCall" 
	<< "AlgFactory["
	<< this 
	<< "]" 
	<< std::ends; 
    return out.str(); 
  } 
}

// because of the extra cast in makeNewAlg needed to resolve base class ambiguity 
// we cannot use the following macro:
//  DERIVED_ALG_FACTORY_DEF_MACRO(SubroutineCall,xaifBoosterBasicBlockPreaccumulationReverse);


