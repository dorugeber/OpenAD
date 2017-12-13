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

#include "xaifBooster/system/inc/BasicBlockElement.hpp"
#include "xaifBooster/system/inc/ConceptuallyStaticInstances.hpp"
#include "xaifBooster/system/inc/Assignment.hpp"
#include "xaifBooster/algorithms/TypeChange/inc/SymbolAlg.hpp"
#include "xaifBooster/algorithms/TypeChange/inc/ConcreteArgumentAlg.hpp"
#include "xaifBooster/algorithms/AddressArithmetic/inc/AssignmentAlg.hpp"
#include "xaifBooster/algorithms/CodeReplacement/inc/ConceptuallyStaticInstances.hpp"

using namespace xaifBooster;

namespace xaifBoosterAddressArithmetic { 

  AssignmentAlg::AssignmentAlg(Assignment& theContaining) : 
    xaifBoosterBasicBlockPreaccumulation::AssignmentAlg(theContaining),
    xaifBoosterBasicBlockPreaccumulationTapeAdjoint::AssignmentAlg(theContaining){ 
  }

  void
  AssignmentAlg::printXMLHierarchy(std::ostream& os) const { 
    switch(xaifBoosterCodeReplacement::ConceptuallyStaticInstances::instance()->getPrintVersion()) { 
    case xaifBoosterCodeReplacement::PrintVersion::ORIGINAL: { 
      getContainingAssignment().printXMLHierarchyImpl(os);
      break;
    }
    case xaifBoosterCodeReplacement::PrintVersion::AUGMENTED: 
      xaifBoosterBasicBlockPreaccumulationTape::AssignmentAlg::printXMLHierarchy(os);
      break;
    case xaifBoosterCodeReplacement::PrintVersion::ADJOINT: 
      xaifBoosterBasicBlockPreaccumulationTapeAdjoint::AssignmentAlg::printXMLHierarchy(os);
      break;
    default: 
      THROW_LOGICEXCEPTION_MACRO("AssignmentAlg::printXMLHierarchy: cannot handle PrintVersion "
				 << xaifBoosterCodeReplacement::PrintVersion::toString(xaifBoosterCodeReplacement::ConceptuallyStaticInstances::instance()->
										       getPrintVersion()).c_str());
      break;
    } // end switch
  } // end of AssignmentAlg::printXMLHierarchy
  
  std::string
  AssignmentAlg::debug() const {
    std::ostringstream out;
    out << "xaifBoosterAddressArithmetic::AssignmentAlg[" << xaifBoosterBasicBlockPreaccumulationTapeAdjoint::AssignmentAlg::debug().c_str()
        << "]" << std::ends;  
    return out.str();
  }

  void AssignmentAlg::traverseToChildren(const GenericAction::GenericAction_E anAction_c) { 
  } 

} // end of namespace
