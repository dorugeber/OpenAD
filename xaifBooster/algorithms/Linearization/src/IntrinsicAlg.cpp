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

#include "xaifBooster/system/inc/Intrinsic.hpp"
#include "xaifBooster/algorithms/Linearization/inc/IntrinsicAlg.hpp"

namespace xaifBoosterLinearization { 

  IntrinsicAlg::IntrinsicAlg(Intrinsic& theContainingIntrinsic) : 
    ExpressionVertexAlgBase(theContainingIntrinsic),
    IntrinsicAlgBase(theContainingIntrinsic),
    ExpressionVertexAlg(theContainingIntrinsic) {
  }

  IntrinsicAlg::~IntrinsicAlg() {}

  std::string IntrinsicAlg::debug () const { 
    std::ostringstream out;
    out << "xaifBoosterLinearization::IntrinsicAlg[" << ExpressionVertexAlg::debug().c_str()
 	<< "]" << std::ends;  
    return out.str();
  } // end of IntrinsicAlg::debug

  void
  IntrinsicAlg::printXMLHierarchy(std::ostream& os) const { 
    IntrinsicAlgBase::getContaining().printXMLHierarchyImpl(os);
  }

  void IntrinsicAlg::traverseToChildren(const GenericAction::GenericAction_E anAction_c) { 
  } 

}
