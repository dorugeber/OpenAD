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
#include "xaifBooster/system/inc/SubroutineCall.hpp"
#include "xaifBooster/algorithms/TypeChange/inc/SymbolAlg.hpp"
#include "xaifBooster/algorithms/TypeChange/inc/ConcreteArgumentAlg.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulationReverse/inc/SubroutineCallAlg.hpp"
#include "xaifBooster/algorithms/CodeReplacement/inc/ConceptuallyStaticInstances.hpp"

using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulationReverse { 

  SubroutineCallAlg::SubroutineCallAlg(SubroutineCall& theContaining) : 
    xaifBoosterTypeChange::SubroutineCallAlg(theContaining),
    xaifBoosterBasicBlockPreaccumulationTape::SubroutineCallAlg(theContaining),
    xaifBoosterBasicBlockPreaccumulationTapeAdjoint::SubroutineCallAlg(theContaining){ 
  }

  void
  SubroutineCallAlg::printXMLHierarchy(std::ostream& os) const { 
    switch(xaifBoosterCodeReplacement::ConceptuallyStaticInstances::instance()->getPrintVersion()) { 
    case xaifBoosterCodeReplacement::PrintVersion::ORIGINAL: { 
      // pick a path:
      dynamic_cast<const xaifBoosterBasicBlockPreaccumulationTapeAdjoint::SubroutineCallAlg*>(this)->
	xaifBoosterTypeChange::SubroutineCallAlg::printXMLHierarchy(os);
      break;
    }
    case xaifBoosterCodeReplacement::PrintVersion::AUGMENTED: 
      xaifBoosterBasicBlockPreaccumulationTape::SubroutineCallAlg::printXMLHierarchy(os);
      break;
    case xaifBoosterCodeReplacement::PrintVersion::ADJOINT: 
      xaifBoosterBasicBlockPreaccumulationTapeAdjoint::SubroutineCallAlg::printXMLHierarchy(os);
      break;
    default: 
      THROW_LOGICEXCEPTION_MACRO("SubroutineCallAlg::printXMLHierarchy: cannot handle PrintVersion "
				 << xaifBoosterCodeReplacement::PrintVersion::toString(xaifBoosterCodeReplacement::ConceptuallyStaticInstances::instance()->
							   getPrintVersion()).c_str());
      break;
    } // end switch
  } // end of SubroutineCallAlg::printXMLHierarchy
  
  std::string SubroutineCallAlg::debug () const { 
    std::ostringstream out;
    out << "xaifBoosterBasicBlockPreaccumulationReverse::SubroutineCallAlg[" 
	<< this
	<< ", calling "
 	<< getContainingSubroutineCall().getSymbolReference().getSymbol().getId().c_str()
 	<< "]" << std::ends;  
    return out.str();
  } // end of SubroutineCallAlg::debug

  void SubroutineCallAlg::traverseToChildren(const GenericAction::GenericAction_E anAction_c) { 
  } 

  void SubroutineCallAlg::algorithm_action_1() { 
    // pick a path
    dynamic_cast<xaifBoosterBasicBlockPreaccumulationTapeAdjoint::SubroutineCallAlg*>(this)->xaifBoosterTypeChange::SubroutineCallAlg::algorithm_action_1();
  }

} // end of namespace
