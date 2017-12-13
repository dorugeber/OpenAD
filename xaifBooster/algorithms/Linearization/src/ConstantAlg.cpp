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

#include "xaifBooster/system/inc/Constant.hpp"
#include "xaifBooster/algorithms/Linearization/inc/ConstantAlg.hpp"

namespace xaifBoosterLinearization { 

  ConstantAlg::ConstantAlg(Constant& theContainingConstant) : 
    ExpressionVertexAlgBase(theContainingConstant),
    ConstantAlgBase(theContainingConstant),
    ExpressionVertexAlg(theContainingConstant) {
    passivate();
  }

  ConstantAlg::~ConstantAlg() {}

  std::string 
  ConstantAlg::debug() const { 
    std::ostringstream out;
    out << "xaifBoosterLinearization::ConstantAlg[" << ExpressionVertexAlg::debug().c_str()
	<< "]" << std::ends;  
    return out.str();
  }

  void
  ConstantAlg::printXMLHierarchy(std::ostream& os) const { 
    ConstantAlgBase::getContaining().printXMLHierarchyImpl(os);
  }

  void ConstantAlg::traverseToChildren(const GenericAction::GenericAction_E anAction_c) { 
  } 

}
