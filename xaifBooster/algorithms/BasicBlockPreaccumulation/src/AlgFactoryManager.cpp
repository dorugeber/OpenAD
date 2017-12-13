// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/utils/inc/LogicException.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/AlgFactoryManager.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/AlgConfig.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/AssignmentAlgFactory.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/BasicBlockAlgFactory.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/CallGraphVertexAlgFactory.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/SequenceFactory.hpp"

using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulation { 

  AlgFactoryManager::AlgFactoryManager() : ourSequenceFactory_p(0) {
  }

  AlgFactoryManager::~AlgFactoryManager() {
    if (ourSequenceFactory_p) delete ourSequenceFactory_p;
  }

  AlgFactoryManager* 
  AlgFactoryManager::instance() { 
    if (ourInstance_p)
      return dynamic_cast<AlgFactoryManager*>(ourInstance_p);
    ourInstanceMutex.lock();
    try { 
      if (!ourInstance_p)
	ourInstance_p=new AlgFactoryManager();
      if (!ourInstance_p) { 
	THROW_LOGICEXCEPTION_MACRO("AlgFactoryManager::instance");
      } // end if 
    } // end try 
    catch (...) { 
      ourInstanceMutex.unlock();
      throw;
    } // end catch
    ourInstanceMutex.unlock();
    return dynamic_cast<AlgFactoryManager*>(ourInstance_p);
  } // end of AlgFactoryManager::instance

  void AlgFactoryManager::resets() {
    resetAssignmentAlgFactory(new AssignmentAlgFactory());
    resetBasicBlockAlgFactory(new BasicBlockAlgFactory());
    resetCallGraphVertexAlgFactory(new CallGraphVertexAlgFactory());
    resetSequenceFactory(new SequenceFactory());
  }

  void AlgFactoryManager::init() {
    xaifBoosterLinearization::AlgFactoryManager::init();
    AlgFactoryManager::resets();
  }

  ALG_CONFIG_ACCESS_DEF_MACRO

  SequenceFactory*
  AlgFactoryManager::getSequenceFactory() const {
    if (!ourSequenceFactory_p)
      THROW_LOGICEXCEPTION_MACRO("AlgFactoryManager::getSequenceFactory: not set");
    return ourSequenceFactory_p;
  }

  void
  AlgFactoryManager::resetSequenceFactory(SequenceFactory* anotherSequenceFactory_p) {
    if(ourSequenceFactory_p)
      delete ourSequenceFactory_p;
    ourSequenceFactory_p = anotherSequenceFactory_p;
  }

}

