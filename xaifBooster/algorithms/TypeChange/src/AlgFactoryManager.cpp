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

#include "xaifBooster/algorithms/TypeChange/inc/AlgFactoryManager.hpp"
#include "xaifBooster/algorithms/TypeChange/inc/AlgConfig.hpp"
#include "xaifBooster/algorithms/TypeChange/inc/ControlFlowGraphAlgFactory.hpp"
#include "xaifBooster/algorithms/TypeChange/inc/ConcreteArgumentAlgFactory.hpp"
#include "xaifBooster/algorithms/TypeChange/inc/SubroutineCallAlgFactory.hpp"
#include "xaifBooster/algorithms/TypeChange/inc/SymbolAlgFactory.hpp"

using namespace xaifBooster;

namespace xaifBoosterTypeChange { 

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
    resetControlFlowGraphAlgFactory(new ControlFlowGraphAlgFactory());
    resetConcreteArgumentAlgFactory(new ConcreteArgumentAlgFactory());
    resetSubroutineCallAlgFactory(new SubroutineCallAlgFactory());
    resetSymbolAlgFactory(new SymbolAlgFactory()); 
 }

  void AlgFactoryManager::init() {
    xaifBooster::AlgFactoryManager::init();
    xaifBoosterTypeChange::AlgFactoryManager::resets();
  }

  ALG_CONFIG_ACCESS_DEF_MACRO

}

