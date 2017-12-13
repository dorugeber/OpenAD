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

#include "xaifBooster/system/inc/Argument.hpp"
#include "xaifBooster/algorithms/Linearization/inc/ArgumentAlg.hpp"

namespace xaifBoosterLinearization { 

  ArgumentAlg::ArgumentAlg(Argument& theContainingArgument) : 
    ExpressionVertexAlgBase(theContainingArgument),
    ArgumentAlgBase(theContainingArgument),
    ExpressionVertexAlg(theContainingArgument) {
  }

  ArgumentAlg::~ArgumentAlg() {}

  std::string ArgumentAlg::debug () const { 
    std::ostringstream out;
    out << "xaifBoosterLinearization::ArgumentAlg[" << ExpressionVertexAlg::debug().c_str()
 	<< "]" << std::ends;  
    return out.str();
  } // end of ArgumentAlg::debug

  void
  ArgumentAlg::printXMLHierarchy(std::ostream& os) const { 
    ArgumentAlgBase::getContaining().printXMLHierarchyImpl(os);
  }

  void ArgumentAlg::traverseToChildren(const GenericAction::GenericAction_E anAction_c) { 
  } 

}
