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

#include "xaifBooster/algorithms/Linearization/inc/AlgFactoryManager.hpp"
#include "xaifBooster/algorithms/Linearization/inc/AlgConfig.hpp"
#include "xaifBooster/algorithms/Linearization/inc/ArgumentAlgFactory.hpp"
#include "xaifBooster/algorithms/Linearization/inc/AssignmentAlgFactory.hpp"
#include "xaifBooster/algorithms/Linearization/inc/BooleanOperationAlgFactory.hpp"
#include "xaifBooster/algorithms/Linearization/inc/ConstantAlgFactory.hpp"
#include "xaifBooster/algorithms/Linearization/inc/ExpressionAlgFactory.hpp"
#include "xaifBooster/algorithms/Linearization/inc/ExpressionEdgeAlgFactory.hpp"
#include "xaifBooster/algorithms/Linearization/inc/IntrinsicAlgFactory.hpp"

using namespace xaifBooster;

namespace xaifBoosterLinearization { 

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
    resetArgumentAlgFactory(new ArgumentAlgFactory());
    resetAssignmentAlgFactory(new AssignmentAlgFactory());
    resetBooleanOperationAlgFactory(new BooleanOperationAlgFactory());
    resetConstantAlgFactory(new ConstantAlgFactory());
    resetExpressionAlgFactory(new ExpressionAlgFactory());
    resetExpressionEdgeAlgFactory(new ExpressionEdgeAlgFactory());
    resetIntrinsicAlgFactory(new IntrinsicAlgFactory());
  }

  void AlgFactoryManager::init() {
    xaifBoosterTypeChange::AlgFactoryManager::init();
    xaifBoosterLinearization::AlgFactoryManager::resets();
  }

  ALG_CONFIG_ACCESS_DEF_MACRO

}

