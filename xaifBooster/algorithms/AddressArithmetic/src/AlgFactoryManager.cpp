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

#include "xaifBooster/algorithms/Linearization/inc/AlgFactoryManager.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/AlgFactoryManager.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulationTape/inc/AlgFactoryManager.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulationTapeAdjoint/inc/AlgFactoryManager.hpp"
#include "xaifBooster/algorithms/ControlFlowReversal/inc/AlgFactoryManager.hpp"

#include "xaifBooster/algorithms/AddressArithmetic/inc/AlgFactoryManager.hpp"
#include "xaifBooster/algorithms/AddressArithmetic/inc/AlgConfig.hpp"
#include "xaifBooster/algorithms/AddressArithmetic/inc/CallGraphVertexAlgFactory.hpp"
#include "xaifBooster/algorithms/AddressArithmetic/inc/BasicBlockAlgFactory.hpp"
#include "xaifBooster/algorithms/AddressArithmetic/inc/AssignmentAlgFactory.hpp"

using namespace xaifBooster;

namespace xaifBoosterAddressArithmetic { 

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
    // we have to pick a path
    xaifBoosterBasicBlockPreaccumulationTapeAdjoint::AlgFactoryManager::resetCallGraphVertexAlgFactory(new CallGraphVertexAlgFactory());
    xaifBoosterBasicBlockPreaccumulationTapeAdjoint::AlgFactoryManager::resetBasicBlockAlgFactory(new BasicBlockAlgFactory());
    resetAssignmentAlgFactory(new AssignmentAlgFactory());
  }

  void AlgFactoryManager::init() {
    // need to pick a path
    dynamic_cast<xaifBoosterBasicBlockPreaccumulationTapeAdjoint::AlgFactoryManager*>(this)->xaifBooster::AlgFactoryManager::init();
    dynamic_cast<xaifBoosterBasicBlockPreaccumulationTapeAdjoint::AlgFactoryManager*>(this)->xaifBoosterTypeChange::AlgFactoryManager::resets();
    dynamic_cast<xaifBoosterBasicBlockPreaccumulationTapeAdjoint::AlgFactoryManager*>(this)->xaifBoosterLinearization::AlgFactoryManager::resets();
    xaifBoosterBasicBlockPreaccumulation::AlgFactoryManager::resets();
    xaifBoosterBasicBlockPreaccumulationTape::AlgFactoryManager::resets();
    xaifBoosterBasicBlockPreaccumulationTapeAdjoint::AlgFactoryManager::resets();
    xaifBoosterControlFlowReversal::AlgFactoryManager::resets();
    xaifBoosterAddressArithmetic::AlgFactoryManager::resets();
  }

  ALG_CONFIG_ACCESS_DEF_MACRO

}

