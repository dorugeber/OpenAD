#ifndef _XAIFBOOSTERADDRESSARITHMETIC_ALGCONFIG_INCLUDE_
#define _XAIFBOOSTERADDRESSARITHMETIC_ALGCONFIG_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/algorithms/BasicBlockPreaccumulationTapeAdjoint/inc/AlgConfig.hpp"
#include "xaifBooster/algorithms/ControlFlowReversal/inc/AlgConfig.hpp"

namespace xaifBoosterAddressArithmetic { 

  /** 
   * configuration and usage for this transformation 
   */
  class AlgConfig : public xaifBoosterBasicBlockPreaccumulationTapeAdjoint::AlgConfig,
		    public xaifBoosterControlFlowReversal::AlgConfig { 

  public:

    static void ourUsage(); 
    void myUsage();

    static void ourConfig();
    void myConfig();

  protected:
    
    friend class AlgFactoryManager; 

    AlgConfig(int argc, 
	      char** argv,
	      const std::string& buildStamp);

  }; 
  
}

#endif

