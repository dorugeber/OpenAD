// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/algorithms/AddressArithmetic/inc/AlgConfig.hpp"
#include "xaifBooster/algorithms/AddressArithmetic/inc/AlgFactoryManager.hpp"
#include "xaifBooster/algorithms/AddressArithmetic/inc/CallGraphVertexAlg.hpp"

namespace xaifBoosterAddressArithmetic { 

  AlgConfig::AlgConfig(int argc, 
		       char** argv,
		       const std::string& buildStamp) :
    xaifBooster::AlgConfig(argc,argv,buildStamp),
    xaifBoosterBasicBlockPreaccumulationTapeAdjoint::AlgConfig(argc,argv,buildStamp),
    xaifBoosterControlFlowReversal::AlgConfig(argc,argv,buildStamp) {
    registerIt(&ourConfig,&ourUsage,"uUt");
  } 

  STATIC_ALG_CONFIG_FUNC_DEF_MACRO

  void AlgConfig::myConfig() { 
    if (isSet('u')) 
      CallGraphVertexAlg::setUserDecides();
    if (isSet('U')) 
      CallGraphVertexAlg::setIgnorance();
    if (isSet('t')) 
      CallGraphVertexAlg::setTopLevelRoutine(argAsString('t'));
  } 

  void AlgConfig::myUsage() { 
    std::cout << " AddressArithmetic options:" << std::endl
	      << "             [-u] user decides on all variables violating simple loop restrictions" << std::endl
	      << "             [-U] ignore all variables violating simple loop restrictions" << std::endl
	      << "             [-t <name> ]" << std::endl
	      << "                top level procedure <name> is checked for quasi-constant data" << std::endl;
  } 

} 
