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

#include "xaifBooster/system/inc/DoMap.hpp"

namespace xaifBooster { 

  const std::string DoMap::ourXAIFName("xaif:DOSetMap");

  void DoMap::printXMLHierarchy(std::ostream& os) const {
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent() 
       << "<" 
       << ourXAIFName 
       << ">" 
       << std::endl; 
    for(StatementIdSetMapEntryPVector::const_iterator it=getEntries().begin();
	it!=getEntries().end();
	it++)
      if (*it)
	(*it)->printXMLHierarchy(os);       
    os << pm.indent() 
       << "</"
       << ourXAIFName
       << ">" << std::endl;
    pm.releaseInstance();
  } // end of  DoMap::printXMLHierarchy

} // end of namespace  
