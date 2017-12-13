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
#include "xaifBooster/system/inc/Update.hpp"

namespace xaifBooster { 

  const std::string Update::ourXAIFName("xaif:Update");

  Update::Update(bool theActiveFlag) : 
    myAssignment(theActiveFlag) { 
  } 

  void
  Update::printXMLHierarchy(std::ostream& os) const { 
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent() 
       << "<"
       << ourXAIFName.c_str() 
       << " " 
       << Assignment::our_myId_XAIFName.c_str() 
       << "=\"" 
       << myAssignment.getId().c_str()
       << "\""
       << ">" 
       << std::endl;
    myAssignment.printXMLHierarchyLHS(os);
    myAssignment.printXMLHierarchyRHS(os);
    os << pm.indent() 
       << "</"
       << ourXAIFName.c_str() 
       << ">" 
       << std::endl;
    pm.releaseInstance();
  } // end of Update::printXMLHierarchy

  std::string Update::debug () const { 
    std::ostringstream out;
    out << "Update[" << this 
	<< "]" << std::ends;  
    return out.str();
  } // end of Update::debug

  Assignment& Update::getAssignment() { 
    return myAssignment;
  } 

  const Assignment& Update::getAssignment() const { 
    return myAssignment;
  } 

} // end of namespace xaifBooster 
