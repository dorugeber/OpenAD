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
#include "xaifBooster/system/inc/Scope.hpp"

namespace xaifBooster { 

  const std::string Scope::ourXAIFName("xaif:Scope");

  const std::string Scope::our_myId_XAIFName("vertex_id");

  void
  Scope::printXMLHierarchy(std::ostream& os) const { 
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent() 
       << "<"
       << ourXAIFName
       << " "
       << our_myId_XAIFName
       << "=\""
       << getId().c_str()
       << "\" " 
       << ObjectWithAnnotation::our_myAnnotation_XAIFName.c_str() 
       << "=\""
       << getAnnotation().c_str()
       << "\">"
       << std::endl;
    mySymbolTable.printXMLHierarchy(os);
    os << pm.indent()
       << "</"
       << ourXAIFName
       << ">"
       << std::endl;
    pm.releaseInstance();
  } // end of Scope::printXMLHierarchy

  std::string Scope::debug () const { 
    std::ostringstream out;
    out << "Scope[" << this 
	<< Vertex::debug()
	<< ",mySymbolTable=" << mySymbolTable.debug()
	<< "]" << std::ends;  
    return out.str();
  } // end of Scope::debug

  SymbolTable&
  Scope::getSymbolTable() { 
    return mySymbolTable;
  } // end of Scope::getControlFlowGraph

  const SymbolTable&
  Scope::getSymbolTable() const { 
    return mySymbolTable;
  } // end of Scope::getControlFlowGraph

} // end of namespace xaifBooster 
