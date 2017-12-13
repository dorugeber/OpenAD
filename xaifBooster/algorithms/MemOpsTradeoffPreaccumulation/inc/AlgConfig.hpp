#ifndef _XAIFBOOSTERMEMOPSTRADEOFFPREACCUMULATION_ALGCONFIG_INCLUDE_
#define _XAIFBOOSTERMEMOPSTRADEOFFPREACCUMULATION_ALGCONFIG_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/AlgConfig.hpp"

namespace xaifBoosterMemOpsTradeoffPreaccumulation { 

  /** 
   * configuration and usage for this transformation 
   */
  class AlgConfig : public xaifBoosterBasicBlockPreaccumulation::AlgConfig  { 

  public:

    AlgConfig(int argc, 
	      char** argv,
	      const std::string& buildStamp);

    virtual void usage();

    virtual void config();

  protected:
    
    virtual std::string getSwitches();

  }; 
  
} // end of namespace xaifBooster
                                                                     
#endif
