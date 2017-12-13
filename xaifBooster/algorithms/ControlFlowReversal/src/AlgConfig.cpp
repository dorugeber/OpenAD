// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/algorithms/ControlFlowReversal/inc/AlgConfig.hpp"
#include "xaifBooster/algorithms/ControlFlowReversal/inc/AlgFactoryManager.hpp"
#include "xaifBooster/algorithms/ControlFlowReversal/inc/CallGraphVertexAlg.hpp"

namespace xaifBoosterControlFlowReversal { 

  AlgConfig::AlgConfig(int argc, 
		       char** argv,
		       const std::string& buildStamp) :
    xaifBooster::AlgConfig(argc,argv,buildStamp) {
    registerIt(&ourConfig,&ourUsage,"I");
  } 

  STATIC_ALG_CONFIG_FUNC_DEF_MACRO

  void AlgConfig::myConfig() { 
    if (isSet('I')) 
      CallGraphVertexAlg::changeIntent();
  } 

  void AlgConfig::myUsage() { 
    std::cout << " ControlFlowReversal options:" << std::endl
	      << "             [-I] change all argument INTENTs for checkpoints and adjoint propagation" << std::endl;
  } 

} 
