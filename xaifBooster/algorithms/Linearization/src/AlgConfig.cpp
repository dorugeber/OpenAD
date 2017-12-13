// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/algorithms/Linearization/inc/AlgConfig.hpp"
#include "xaifBooster/algorithms/Linearization/inc/AlgFactoryManager.hpp"

namespace xaifBoosterLinearization { 

  AlgConfig::AlgConfig(int argc, 
		       char** argv,
		       const std::string& buildStamp) :
    xaifBooster::AlgConfig(argc,argv,buildStamp),
    xaifBoosterTypeChange::AlgConfig(argc,argv,buildStamp) {
    registerIt(&ourConfig,&ourUsage,"");
  } 

  STATIC_ALG_CONFIG_FUNC_DEF_MACRO

  void AlgConfig::myConfig() { 
  } 

  void AlgConfig::myUsage() { 
    std::cout << " Linearization options: no specific options here " << std::endl;
  } 

} // end of namespace xaifBooster
                                                                     


