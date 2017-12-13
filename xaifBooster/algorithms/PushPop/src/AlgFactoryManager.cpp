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
#include "xaifBooster/utils/inc/DbgLoggerManager.hpp"

#include "xaifBooster/algorithms/PushPop/inc/AlgFactoryManager.hpp"
#include "xaifBooster/algorithms/PushPop/inc/AlgConfig.hpp"
#include "xaifBooster/algorithms/PushPop/inc/BasicBlockAlgFactory.hpp"
#include "xaifBooster/algorithms/PushPop/inc/CallGraphVertexAlgFactory.hpp"
#include "xaifBooster/algorithms/PushPop/inc/SequenceFactory.hpp"

using namespace xaifBooster;

namespace xaifBoosterPushPop {

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
    resetBasicBlockAlgFactory(new BasicBlockAlgFactory());
    resetCallGraphVertexAlgFactory(new CallGraphVertexAlgFactory());
    resetSequenceFactory(new SequenceFactory());
  }

  void AlgFactoryManager::init() {
    xaifBoosterAddressArithmetic::AlgFactoryManager::init();
    AlgFactoryManager::resets();
  } // end AlgFactoryManager::init()

  ALG_CONFIG_ACCESS_DEF_MACRO

}
