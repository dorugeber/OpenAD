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

#include "xaifBooster/algorithms/ControlFlowReversal/inc/AlgFactoryManager.hpp"
#include "xaifBooster/algorithms/ControlFlowReversal/inc/AlgConfig.hpp"
#include "xaifBooster/algorithms/ControlFlowReversal/inc/CallGraphAlgFactory.hpp"
#include "xaifBooster/algorithms/ControlFlowReversal/inc/CallGraphVertexAlgFactory.hpp"
#include "xaifBooster/algorithms/ControlFlowReversal/inc/ControlFlowGraphVertexAlgFactory.hpp"
#include "xaifBooster/algorithms/ControlFlowReversal/inc/IfStatementAlgFactory.hpp"
#include "xaifBooster/algorithms/ControlFlowReversal/inc/PreLoopAlgFactory.hpp"
#include "xaifBooster/algorithms/ControlFlowReversal/inc/ForLoopAlgFactory.hpp"
#include "xaifBooster/algorithms/ControlFlowReversal/inc/BasicBlockAlgFactory.hpp"
#include "xaifBooster/algorithms/ControlFlowReversal/inc/EntryAlgFactory.hpp"
#include "xaifBooster/algorithms/ControlFlowReversal/inc/ExitAlgFactory.hpp"
#include "xaifBooster/algorithms/ControlFlowReversal/inc/EndLoopAlgFactory.hpp"
#include "xaifBooster/algorithms/ControlFlowReversal/inc/EndBranchAlgFactory.hpp"
#include "xaifBooster/algorithms/ControlFlowReversal/inc/BranchAlgFactory.hpp"
#include "xaifBooster/algorithms/ControlFlowReversal/inc/LabelAlgFactory.hpp"
#include "xaifBooster/algorithms/ControlFlowReversal/inc/GotoAlgFactory.hpp"

using namespace xaifBooster;

namespace xaifBoosterControlFlowReversal { 

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
    resetCallGraphAlgFactory(new CallGraphAlgFactory());
    resetCallGraphVertexAlgFactory(new CallGraphVertexAlgFactory());
    resetControlFlowGraphVertexAlgFactory(new ControlFlowGraphVertexAlgFactory());
    resetIfStatementAlgFactory(new IfStatementAlgFactory());
    resetPreLoopAlgFactory(new PreLoopAlgFactory());
    resetForLoopAlgFactory(new ForLoopAlgFactory());
    resetEntryAlgFactory(new EntryAlgFactory());
    resetExitAlgFactory(new ExitAlgFactory());
    resetEndLoopAlgFactory(new EndLoopAlgFactory());
    resetEndBranchAlgFactory(new EndBranchAlgFactory());
    resetBasicBlockAlgFactory(new BasicBlockAlgFactory());
    resetBranchAlgFactory(new BranchAlgFactory());
    resetGotoAlgFactory(new GotoAlgFactory());
    resetLabelAlgFactory(new LabelAlgFactory());
  }

  void AlgFactoryManager::init() {
    xaifBooster::AlgFactoryManager::init();
    xaifBoosterControlFlowReversal::AlgFactoryManager::resets();
  }

  ALG_CONFIG_ACCESS_DEF_MACRO

}

