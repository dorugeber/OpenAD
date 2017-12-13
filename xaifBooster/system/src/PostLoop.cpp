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
#include "xaifBooster/system/inc/PostLoop.hpp"

namespace xaifBooster { 

  const std::string PostLoop::ourXAIFName("xaif:PostLoop");

  const std::string PostLoop::our_myId_XAIFName("vertex_id");

  void
  PostLoop::printXMLHierarchy(std::ostream& os) const { 
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent() 
       << "<"
       << ourXAIFName.c_str() 
       << " " 
       << our_myId_XAIFName 
       << "=\"" 
       << getId()
       << "\" " 
       << ObjectWithAnnotation::our_myAnnotation_XAIFName.c_str() 
       << "=\""
       << getAnnotation().c_str()
       << "\">" 
       << std::endl;
    getCondition().printXMLHierarchy(os);
    os << pm.indent() 
       << "</"
       << ourXAIFName.c_str() 
       << ">" 
       << std::endl;
    pm.releaseInstance();
  } // end of PostLoop::printXMLHierarchy

  std::string PostLoop::debug () const { 
    std::ostringstream out;
    out << "PostLoop[" << this 
        << BaseLoop::debug().c_str()
	<< "]" << std::ends;  
    return out.str();
  } // end of PostLoop::debug

} // end of namespace xaifBooster 
