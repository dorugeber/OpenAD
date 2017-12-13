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
#include "xaifBooster/utils/inc/LogicException.hpp"
#include "xaifBooster/utils/inc/DbgLoggerManager.hpp"

#include "xaifBooster/system/inc/StatementIdSetMapEntry.hpp"

namespace xaifBooster { 

  const std::string StatementIdSetMapEntry::ourXAIFName("xaif:StmtIdSet");

  const std::string StatementIdSetMapEntry::our_myKey_XAIFName("key");

  const std::string StatementIdSetMapEntry::our_StatementId_XAIFName("xaif:StatementId");

  const std::string StatementIdSetMapEntry::our_IdRef_XAIFName("idRef");

  StatementIdSetMapEntry::StatementIdSetMapEntry(unsigned int theKey) : 
    myKey(theKey) {} 
 
  StatementIdSetMapEntry::~StatementIdSetMapEntry(){}

  std::string StatementIdSetMapEntry::debug() const {
    std::ostringstream out;
    out << "StatementIdSetMapEntry[" << this 
	<< "]" << std::ends; 
    return out.str();
  } 
  
  void StatementIdSetMapEntry::printXMLHierarchy(std::ostream& os) const {
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent() 
       << "<" 
       << ourXAIFName.c_str() 
       << " "
       << our_myKey_XAIFName.c_str()
       << "=\""
       << myKey
       << "\""
       << ">" 
       << std::endl; 
    for(StatementIdSet::const_iterator it=myStatementIdSet.begin();
	it!=myStatementIdSet.end();
	it++) { 
      os << pm.indent(2) 
	 << "<" 
	 << our_StatementId_XAIFName.c_str()
	 << " "
	 << our_IdRef_XAIFName.c_str()
	 << "=\""
	 << (*it).c_str()
	 << "\""
	 << "/>"
	 << std::endl; 
    } 
    os << pm.indent() 
       << "</"
       << ourXAIFName
       << ">" << std::endl;
    pm.releaseInstance();
  } 

  void StatementIdSetMapEntry::insert(const std::string& anId) { 
    myStatementIdSet.insert(anId);
  } 

  const StatementIdSet& StatementIdSetMapEntry::getStatementIdSet() const { 
    return myStatementIdSet;
  } 

} // end of namespace  
