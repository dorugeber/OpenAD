// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/utils/inc/PrintManager.hpp"

#include "xaifBooster/system/inc/PrintVersion.hpp"
#include "xaifBooster/system/inc/ConceptuallyStaticInstances.hpp"
#include "xaifBooster/algorithms/CodeReplacement/inc/ConceptuallyStaticInstances.hpp"
#include "xaifBooster/algorithms/AddressArithmetic/inc/BasicBlockAlg.hpp"

using namespace xaifBooster;

namespace xaifBoosterAddressArithmetic { 

  BasicBlockAlg::BasicBlockAlg(BasicBlock& theContaining) : 
    BasicBlockAlgBase(theContaining),
    xaifBoosterControlFlowReversal::BasicBlockAlg(theContaining),
    xaifBoosterBasicBlockPreaccumulationTapeAdjoint::BasicBlockAlg(theContaining){ 
  }

  std::string
  BasicBlockAlg::debug() const {
    std::ostringstream out;
    out << "xaifBoosterAddressArithmetic::BasicBlockAlg[" << xaifBoosterControlFlowReversal::BasicBlockAlg::debug().c_str()
                                                   << "," << xaifBoosterBasicBlockPreaccumulationTapeAdjoint::BasicBlockAlg::debug().c_str()
        << "]" << std::ends;
    return out.str();
  }

  void BasicBlockAlg::printXMLHierarchy(std::ostream& os) const { 
    // this is for testing only!
    switch(xaifBoosterCodeReplacement::ConceptuallyStaticInstances::instance()->getPrintVersion()) { 
    case xaifBoosterCodeReplacement::PrintVersion::AUGMENTED: 
      xaifBoosterBasicBlockPreaccumulationTape::BasicBlockAlg::printXMLHierarchy(os);
      break;
    case xaifBoosterCodeReplacement::PrintVersion::ADJOINT: 
      xaifBoosterBasicBlockPreaccumulationTapeAdjoint::BasicBlockAlg::printXMLHierarchy(os);
      break;
    default: 
      THROW_LOGICEXCEPTION_MACRO("BasicBlockAlg::printXMLHierarchy: cannot handle PrintVersion "
				 << xaifBoosterCodeReplacement::PrintVersion::toString(xaifBoosterCodeReplacement::ConceptuallyStaticInstances::instance()->
							   getPrintVersion()).c_str());
      break;
    } // end switch
  } 

  void BasicBlockAlg::traverseToChildren(const GenericAction::GenericAction_E anAction_c) { 
  } 

  void BasicBlockAlg::algorithm_action_2() { 
    DBG_MACRO(DbgGroup::CALLSTACK,"xaifBoosterAddressArithmetic::BasicBlockAlg::algorithm_action_2() called for: " << debug().c_str());
    xaifBoosterBasicBlockPreaccumulation::BasicBlockAlg::algorithm_action_2();
  }

  void BasicBlockAlg::algorithm_action_3() { 
    DBG_MACRO(DbgGroup::CALLSTACK,"xaifBoosterAddressArithmetic::BasicBlockAlg::algorithm_action_3() called for: " << debug().c_str());
    xaifBoosterBasicBlockPreaccumulation::BasicBlockAlg::algorithm_action_3();
  }

  void BasicBlockAlg::algorithm_action_4() { 
    DBG_MACRO(DbgGroup::CALLSTACK,"xaifBoosterAddressArithmetic::BasicBlockAlg::algorithm_action_4() called for: " << debug().c_str());
    xaifBoosterBasicBlockPreaccumulationTape::BasicBlockAlg::algorithm_action_4();
  } 

  void BasicBlockAlg::algorithm_action_5() { 
    DBG_MACRO(DbgGroup::CALLSTACK,"xaifBoosterAddressArithmetic::BasicBlockAlg::algorithm_action_5() called for: " << debug().c_str());
    xaifBoosterBasicBlockPreaccumulationTapeAdjoint::BasicBlockAlg::algorithm_action_5();
  }
  
} // end namespace xaifBoosterAddressArithmetic

