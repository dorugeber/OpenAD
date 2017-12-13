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

#include "xaifBooster/algorithms/TraceDiff/inc/AlgFactoryManager.hpp"
#include "xaifBooster/algorithms/TraceDiff/inc/AlgConfig.hpp"
#include "xaifBooster/algorithms/TraceDiff/inc/AssignmentAlgFactory.hpp"
#include "xaifBooster/algorithms/TraceDiff/inc/BasicBlockAlgFactory.hpp"
#include "xaifBooster/algorithms/TraceDiff/inc/IntrinsicAlgFactory.hpp"
#include "xaifBooster/algorithms/TraceDiff/inc/CallGraphVertexAlgFactory.hpp"
#include "xaifBooster/algorithms/TraceDiff/inc/SubroutineCallAlgFactory.hpp"

using namespace xaifBooster;

namespace xaifBoosterTraceDiff { 

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
    resetBasicBlockAlgFactory(new BasicBlockAlgFactory());
    resetAssignmentAlgFactory(new AssignmentAlgFactory());
    resetCallGraphVertexAlgFactory(new CallGraphVertexAlgFactory());
    resetSubroutineCallAlgFactory(new SubroutineCallAlgFactory());
    resetIntrinsicAlgFactory(new IntrinsicAlgFactory());
    DBG_MACRO(DbgGroup::CALLSTACK,
	      "at the end of xaifBoosterTraceDiff::AlgFactoryManager::resets: " 
	      << debug().c_str());
  }

  void AlgFactoryManager::init() {
    // need to pick a path
    dynamic_cast<xaifBoosterTypeChange::AlgFactoryManager*>(this)->xaifBooster::AlgFactoryManager::init();
    xaifBoosterTypeChange::AlgFactoryManager::resets();
    xaifBoosterControlFlowReversal::AlgFactoryManager::resets();
    xaifBoosterTraceDiff::AlgFactoryManager::resets();
  }

  ALG_CONFIG_ACCESS_DEF_MACRO

}

