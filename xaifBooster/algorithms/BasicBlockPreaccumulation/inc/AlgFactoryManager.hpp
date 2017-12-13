#ifndef _XAIFBOOSTERBASICBLOCKPREACCUMULATION_ALGFACTORYMANAGER_INCLUDE_
#define _XAIFBOOSTERBASICBLOCKPREACCUMULATION_ALGFACTORYMANAGER_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/algorithms/Linearization/inc/AlgFactoryManager.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/SequenceFactory.hpp"

namespace xaifBoosterBasicBlockPreaccumulation { 

  class AlgConfig; 

  /** 
   * the singleton class for 
   * setting algorithm factory pointers
   */
  class AlgFactoryManager : public xaifBoosterLinearization::AlgFactoryManager { 

  public: 

    AlgFactoryManager();

    virtual ~AlgFactoryManager();

    static AlgFactoryManager* instance();

    virtual void resets(); 

    virtual void init(); 

    ALG_CONFIG_ACCESS_DECL_MACRO

    SequenceFactory* getSequenceFactory() const;

  protected:

    void resetSequenceFactory(SequenceFactory*);

  private:

    SequenceFactory* ourSequenceFactory_p;

  }; // end of class AlgFactoryManager

}

#endif
