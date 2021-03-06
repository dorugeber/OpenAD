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
#include "xaifBooster/algorithms/ConstantFolding/inc/AlgFactoryManager.hpp"
#include "xaifBooster/algorithms/ConstantFolding/inc/AssignmentAlgFactory.hpp"
#include "xaifBooster/algorithms/ConstantFolding/inc/BasicBlockAlgFactory.hpp"

using namespace xaifBooster;

namespace xaifBoosterConstantFolding { 

  xaifBooster::AlgFactoryManager* 
  AlgFactoryManager::instance() { 
    if (ourInstance_p)
      return ourInstance_p;
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
    return ourInstance_p;
  } // end of AlgFactoryManager::instance

  void AlgFactoryManager::resets() {
    resetAssignmentAlgFactory(new AssignmentAlgFactory());
    resetBasicBlockAlgFactory(new BasicBlockAlgFactory());
  }

  void AlgFactoryManager::init() {
    xaifBoosterBasicBlockPreaccumulation::AlgFactoryManager::init();
    xaifBoosterConstantFolding::AlgFactoryManager::resets();
  }

}

