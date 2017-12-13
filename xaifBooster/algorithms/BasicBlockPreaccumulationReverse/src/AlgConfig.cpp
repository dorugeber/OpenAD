// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/algorithms/BasicBlockPreaccumulationReverse/inc/AlgConfig.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulationReverse/inc/AlgFactoryManager.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulationReverse/inc/CallGraphVertexAlg.hpp"

namespace xaifBoosterBasicBlockPreaccumulationReverse { 

  AlgConfig::AlgConfig(int argc, 
		       char** argv,
		       const std::string& buildStamp) :
    xaifBooster::AlgConfig(argc,argv,buildStamp),
    xaifBoosterPushPop::AlgConfig(argc,argv,buildStamp) {
    registerIt(&ourConfig,&ourUsage,"fP");
  } 

  STATIC_ALG_CONFIG_FUNC_DEF_MACRO

  void AlgConfig::myConfig() { 
    if (isSet('f')) 
      CallGraphVertexAlg::checkPointToFiles();
    if (isSet('P')) 
      CallGraphVertexAlg::forceAllArgumentCheckpoints();
  } 

  void AlgConfig::myUsage() { 
    std::cout << " BasicBlockPreaccumulationReverse options:" << std::endl
	    << "             [-P] force checkpoints even on formal parameters with constant actual arguments or no calls" << std::endl
	    << "             [-f] checkpoint write order for individual files instead of a memory stack" << std::endl; 
  } 

} 

