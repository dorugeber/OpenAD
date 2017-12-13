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

#include "xaifBooster/system/inc/StatementIdSetMap.hpp"
#include "xaifBooster/system/inc/StatementIdSetMapKey.hpp"

namespace xaifBooster { 

  StatementIdSetMap::~StatementIdSetMap() {
    for(StatementIdSetMapEntryPVector::iterator it=myStatementIdSetMapEntryPVector.begin();
	it!=myStatementIdSetMapEntryPVector.end();
	it++)
      if (*it)
	delete (*it);
  } 

  std::string StatementIdSetMap::debug() const {
    std::ostringstream out;
    out << "StatementIdSetMap[" << this 
	<< "]" << std::ends; 
    return out.str();
  }

  StatementIdSetMapEntry& StatementIdSetMap::addEntry(unsigned int theKey) { 
    if (theKey>=myStatementIdSetMapEntryPVector.size())
      // resize and initialize to 0
      myStatementIdSetMapEntryPVector.resize(theKey>myStatementIdSetMapEntryPVector.size()+256?theKey:myStatementIdSetMapEntryPVector.size()+256,0);
    StatementIdSetMapEntry* aNewStatementIdSetMapEntry_p=new StatementIdSetMapEntry(theKey);
    myStatementIdSetMapEntryPVector[theKey]=aNewStatementIdSetMapEntry_p;
    return (*aNewStatementIdSetMapEntry_p);
  } 

  const StatementIdSetMapEntry& StatementIdSetMap::getEntry(const StatementIdSetMapKey& aKey) const { 
    if (aKey.getKind()==StatementIdSetMapKey::TEMP_VAR)
      // obviously because the map doesn't contain any info on temporaries, 
      // the calling context should figure it out itself
      THROW_LOGICEXCEPTION_MACRO("StatementIdSetMap::getStatementIdSetMapEntry: not supported for temporaries");
    if (aKey.getKind()==StatementIdSetMapKey::NO_INFO) 
      THROW_LOGICEXCEPTION_MACRO("StatementIdSetMap::getStatementIdSetMapEntry: no Do information");
    if (aKey.getKey()<0 
	|| 
	aKey.getKey()>=myStatementIdSetMapEntryPVector.size())  
      // have an explicit check here rather than using 'at' which 
      // wouldn't hint where the problem is...
      THROW_LOGICEXCEPTION_MACRO("StatementIdSetMap::getStatementIdSetMapEntry: key >" 
				 << aKey.getKey() 
				 << "< out of range");
    if (!myStatementIdSetMapEntryPVector[aKey.getKey()])
      THROW_LOGICEXCEPTION_MACRO("StatementIdSetMap::getStatementIdSetMapEntry: null pointer for key >" 
				 << aKey.getKey() 
				 << "<");
    return *myStatementIdSetMapEntryPVector[aKey.getKey()];
  }

  const StatementIdSetMap::StatementIdSetMapEntryPVector& StatementIdSetMap::getEntries() const { 
    return myStatementIdSetMapEntryPVector;
  }

} // end of namespace  
