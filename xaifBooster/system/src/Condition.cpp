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
#include "xaifBooster/utils/inc/PrintManager.hpp"
#include "xaifBooster/system/inc/Condition.hpp"

namespace xaifBooster { 

  const std::string Condition::ourXAIFName("xaif:Condition");

  void
  Condition::printXMLHierarchy(std::ostream& os) const { 
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent() 
       << "<"
       << ourXAIFName.c_str() 
       << ">" 
       << std::endl;
    myExpression.printXMLHierarchy(os);
    os << pm.indent() 
       << "</"
       << ourXAIFName.c_str() 
       << ">" 
       << std::endl;
    pm.releaseInstance();
  } // end of Condition::printXMLHierarchy

  std::string Condition::debug () const { 
    std::ostringstream out;
    out << "Condition[" << this 
	<< "]" << std::ends;  
    return out.str();
  } // end of Condition::debug

  Expression& Condition::getExpression() { 
    return myExpression;
  } 

  const Expression& Condition::getExpression() const { 
    return myExpression;
  } 

} // end of namespace xaifBooster 
