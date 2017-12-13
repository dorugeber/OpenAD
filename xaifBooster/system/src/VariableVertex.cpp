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
#include "xaifBooster/utils/inc/LogicException.hpp"
#include "xaifBooster/system/inc/VariableVertex.hpp"

namespace xaifBooster { 

  void VariableVertex::printXMLHierarchy(std::ostream& os) const {
  } // end if VariableVertex::printXMLHierarchy
  
  std::string VariableVertex::debug () const { 
    std::ostringstream out;
    out << "VariableVertex[" << this 
	<< "]" << std::ends;  
    return out.str();
  } // end of VariableVertex::debug


  bool VariableVertex::isSymbol() const { 
    return false;
  } 

} // end of namespace xaifBooster 
