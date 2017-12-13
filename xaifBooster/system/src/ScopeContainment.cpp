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
#include "xaifBooster/system/inc/Scopes.hpp"
#include "xaifBooster/system/inc/ScopeContainment.hpp"

namespace xaifBooster { 

  const std::string ScopeContainment::ourXAIFName("xaif:ScopeEdge");
  const std::string ScopeContainment::our_myId_XAIFName("edge_id");
  const std::string ScopeContainment::our_source_XAIFName("source");
  const std::string ScopeContainment::our_target_XAIFName("target");

  void
  ScopeContainment::printXMLHierarchy(std::ostream& os,
				      const Scopes& theScopes) const { 
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent()
       << "<"
       << ourXAIFName 
       << " " 
       << our_myId_XAIFName 
       << "=\"" 
       << getId().c_str()
       << "\" " 
       << our_source_XAIFName 
       << "=\"" 
       << theScopes.getSourceOf(*this).getId().c_str()
       << "\" " 
       << our_target_XAIFName 
       << "=\"" 
       << theScopes.getTargetOf(*this).getId().c_str()
       << "\"/>" 
       << std::endl; 
    pm.releaseInstance();
  } // end of ScopeContainment::printXMLHierarchy

  std::string ScopeContainment::debug () const { 
    std::ostringstream out;
    out << "ScopeContainment[" << this 
	<< Edge::debug()
	<< "]" << std::ends;  
    return out.str();
  } // end of ScopeContainment::debug

} // end of namespace xaifBooster 
