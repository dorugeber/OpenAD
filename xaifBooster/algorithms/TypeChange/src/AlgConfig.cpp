// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/algorithms/TypeChange/inc/AlgConfig.hpp"
#include "xaifBooster/algorithms/TypeChange/inc/SubroutineCallAlg.hpp"
#include "xaifBooster/algorithms/TypeChange/inc/ControlFlowGraphAlg.hpp"
#include "xaifBooster/algorithms/TypeChange/inc/AlgFactoryManager.hpp"

namespace xaifBoosterTypeChange { 

  AlgConfig::AlgConfig(int argc, 
		       char** argv,
		       const std::string& buildStamp) :
    xaifBooster::AlgConfig(argc,argv,buildStamp) {
    registerIt(&ourConfig,&ourUsage,"wr");
  } 

  STATIC_ALG_CONFIG_FUNC_DEF_MACRO

  void AlgConfig::myConfig() { 
    if (isSet('w')) 
      xaifBoosterTypeChange::SubroutineCallAlg::addWrapperNames(argAsString('w'));
    if (isSet('r')) 
      xaifBoosterTypeChange::ControlFlowGraphAlg::setForceNonExternalRenames();
  } 

  void AlgConfig::myUsage() { 
    std::cout << " TypeChange options: " << std::endl
	      << "             [-w \"<list of subroutines with wrappers\"> ]" << std::endl
	      << "                 space separated list enclosed in double quotes" << std::endl
	      << "             [-r] force renaming of all non-external routines" << std::endl;
  } 

} 
