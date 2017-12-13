// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/utils/inc/DbgLoggerManager.hpp"

#include "xaifBooster/system/inc/Marker.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulationTapeAdjoint/inc/MarkerAlg.hpp"

namespace xaifBoosterBasicBlockPreaccumulationTapeAdjoint {  

  MarkerAlg::MarkerAlg(const Marker& theContainingMarker) : 
    MarkerAlgBase(theContainingMarker),
    BasicBlockElementAlg(theContainingMarker) { 
  }

  void MarkerAlg::printXMLHierarchy(std::ostream& os) const { 
    MarkerAlgBase::printXMLHierarchy(os);
  }

  std::string 
  MarkerAlg::debug() const { 
    std::ostringstream out;
    out << "xaifBoosterBasicBlockPreaccumulationTapeAdjoint::MarkerAlg["
	<< this 
	<< ","
 	<< MarkerAlgBase::debug().c_str()
	<< "]" << std::ends;  
    return out.str();
  }

  void MarkerAlg::traverseToChildren(const GenericAction::GenericAction_E anAction_c) { 
  } 
  
} // end namespace xaifBoosterBasicBlockPreaccumulationTapeAdjoint

