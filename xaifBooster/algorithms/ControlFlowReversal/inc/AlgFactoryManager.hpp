#ifndef _XAIFBOOSTERCONTROLFLOWREVERSAL_ALGFACTORYMANAGER_INCLUDE_
#define _XAIFBOOSTERCONTROLFLOWREVERSAL_ALGFACTORYMANAGER_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/AlgFactoryManager.hpp"

namespace xaifBoosterControlFlowReversal { 

  class AlgConfig; 

  /** 
   * the singleton class for 
   * setting algorithm factory pointers
   */
  class AlgFactoryManager : virtual public xaifBooster::AlgFactoryManager { 

  public: 

    static xaifBooster::AlgFactoryManager* instance();

    virtual void resets(); 

    virtual void init(); 

    ALG_CONFIG_ACCESS_DECL_MACRO

  }; // end of class AlgFactoryManager

}

#endif
