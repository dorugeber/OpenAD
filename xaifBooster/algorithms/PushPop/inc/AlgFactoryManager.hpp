#ifndef _XAIFBOOSTERPUSHPOP_ALGFACTORYMANAGER_INCLUDE_
#define _XAIFBOOSTERPUSHPOP_ALGFACTORYMANAGER_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/algorithms/AddressArithmetic/inc/AlgFactoryManager.hpp"

namespace xaifBoosterPushPop {

  class AlgConfig; 

  /** 
   * the singleton class for 
   * setting algorithm factory pointers
   */
  class AlgFactoryManager : public xaifBoosterAddressArithmetic::AlgFactoryManager {

  public: 

    static AlgFactoryManager* instance();

    virtual void resets(); 

    virtual void init(); 

    ALG_CONFIG_ACCESS_DECL_MACRO

  }; // end of class AlgFactoryManager

}

#endif
