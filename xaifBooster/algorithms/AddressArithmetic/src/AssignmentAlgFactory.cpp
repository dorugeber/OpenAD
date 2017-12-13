// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/system/inc/AssignmentAlgBase.hpp"
#include "xaifBooster/system/inc/Assignment.hpp"
#include "xaifBooster/algorithms/AddressArithmetic/inc/AssignmentAlgFactory.hpp"
#include "xaifBooster/algorithms/AddressArithmetic/inc/AssignmentAlg.hpp"


namespace xaifBoosterAddressArithmetic { 
  AssignmentAlgBase* AssignmentAlgFactory::makeNewAlg(Assignment& theContaining) { 
    return dynamic_cast<xaifBoosterBasicBlockPreaccumulationTapeAdjoint::AssignmentAlg*>(new AssignmentAlg(theContaining)); 
  }
 
  xaifBooster :: AssignmentAlgFactory* AssignmentAlgFactory::instance() { 
    return AlgFactoryManager::instance()->getAssignmentAlgFactory(); 
  } 

  std::string AssignmentAlgFactory::debug() const { 
    std::ostringstream out; 
    out << "xaifBoosterAddressArithmetic" 
	<< "::" 
	<< "Assignment" 
	<< "AlgFactory["
	<< this 
	<< "]" 
	<< std::ends; 
    return out.str(); 
  } 
}

