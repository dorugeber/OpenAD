// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include <sstream>

#include "xaifBooster/system/inc/BasicBlockElement.hpp"
#include "xaifBooster/system/inc/Assignment.hpp"
#include "xaifBooster/system/inc/VariableSymbolReference.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulationTapeAdjoint/inc/BasicBlockElementAlg.hpp"

namespace xaifBoosterBasicBlockPreaccumulationTapeAdjoint { 

  BasicBlockElementAlg::BasicBlockElementAlg(const BasicBlockElement& theContainingBasicBlockElement) : 
    BasicBlockElementAlgBase(theContainingBasicBlockElement) { 
  }

  BasicBlockElementAlg::~BasicBlockElementAlg() { 
  }

  std::string 
  BasicBlockElementAlg::debug() const { 
    std::ostringstream out;
    out << "xaifBoosterBasicBlockPreaccumulationTapeAdjoint::BasicBlockElementAlg["
        << BasicBlockElementAlgBase::debug().c_str()
	<< "]" << std::ends;  
    return out.str();
  }

  void
  BasicBlockElementAlg::printXMLHierarchy(std::ostream& os) const { 
    getContaining().printXMLHierarchyImpl(os);
  }

  void BasicBlockElementAlg::traverseToChildren(const GenericAction::GenericAction_E anAction_c) { 
  } 

} // end namespace xaifBoosterBasicBlockPreaccumulationTapeAdjoint

