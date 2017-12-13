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
#include "xaifBooster/utils/inc/DbgLoggerManager.hpp"

#include "xaifBooster/system/inc/DuUdMap.hpp"
#include "xaifBooster/system/inc/StatementIdSetMapKey.hpp"

namespace xaifBooster { 

  const std::string DuUdMap::ourXAIFName("xaif:DUUDSetMap");

  void DuUdMap::printXMLHierarchy(std::ostream& os) const {
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent() 
       << "<" 
       << ourXAIFName 
       << ">" 
       << std::endl; 
    if (PrintManager::isVerbose()) { 
      for(StatementIdSetMapEntryPVector::const_iterator it=getEntries().begin();
	  it!=getEntries().end();
	  it++)
	if (*it)
	  (*it)->printXMLHierarchy(os);       
    }
    else { 
      if (getEntries().begin()!=getEntries().end()) { 
	(*(getEntries().begin()))->printXMLHierarchy(os);
      }
    }
    os << pm.indent() 
       << "</"
       << ourXAIFName
       << ">" << std::endl;
    pm.releaseInstance();
  } // end of  DuUdMap::printXMLHierarchy

  const DuUdMapDefinitionResult DuUdMap::definition(const StatementIdSetMapKey& aKey,
						    const ObjectWithId::Id& statementId,
						    const StatementIdList& anIdList) const {
    DuUdMapDefinitionResult theResult;
    if (aKey.getKind()==InfoMapKey::TEMP_VAR)
      // obviously because the map doesn't contain any info on temporaries, 
      // the calling context should figure it out itself
      THROW_LOGICEXCEPTION_MACRO("DuUdMap::definition: not supported for temporaries");
    if (aKey.getKind()!=InfoMapKey::NO_INFO) { 
      // we get the entry:
      if (aKey.getKey()<0 
	  || 
	  aKey.getKey()>=getEntries().size())  
	// have an explicit check here rather than using 'at' which 
	// wouldn't hint where the problem is...
	THROW_LOGICEXCEPTION_MACRO("DuUdMap::definition: key >" 
				   << aKey.getKey() 
				   << "< out of range");
      const StatementIdSetMapEntry& theEntry(getEntry(aKey));
      if (theEntry.getStatementIdSet().empty()) 
	THROW_LOGICEXCEPTION_MACRO("DuUdMap::definition: empty StatementIdSet implies use of a variable that has not been defined");
      unsigned int aSize(theEntry.getStatementIdSet().size());
      unsigned int matchNumber=0;
      bool hasOutOfScope=false;
      bool passedUse=false;
      bool loopCarried=false;
      for(StatementIdSet::const_iterator chainI=theEntry.getStatementIdSet().begin();
	  chainI!=getEntry(aKey).getStatementIdSet().end();
	  ++chainI) {
	for(StatementIdList::const_iterator it=anIdList.begin();
	    it!=anIdList.end();
	    ++it) { 
	  if (*it=="")
	    THROW_LOGICEXCEPTION_MACRO("DuUdMap::definition: all StatementIds in anIdSet are supposed to be for regular statements and therefore cannot be empty");
	  if (*it==*chainI) { 
	    matchNumber++;
	    if (passedUse) 
	      loopCarried=true;
	  }
	  if (matchNumber==1)
	    theResult.myStatementId=*chainI;
	  if (statementId==*it)
	    passedUse=true;
	}
	if (*chainI=="") 
	  hasOutOfScope=true;
      }
      if (loopCarried)
	hasOutOfScope=true;
      if ((matchNumber==0 
	   &&
	   (hasOutOfScope
	    || 
	    (!hasOutOfScope 
	     && 
	     aSize>1))))
	theResult.myAnswer=DuUdMapDefinitionResult::AMBIGUOUS_OUTSIDE;
      else if (matchNumber==0 
	       && 
	       !hasOutOfScope
	       && 
	       aSize==1)
	theResult.myAnswer=DuUdMapDefinitionResult::UNIQUE_OUTSIDE;
      else if (matchNumber>0 
	       && 
	       (hasOutOfScope
		|| 
		aSize>matchNumber))
	theResult.myAnswer=DuUdMapDefinitionResult::AMBIGUOUS_BOTHSIDES;
      else if (matchNumber==1 
	       && 
	       aSize==1)
	theResult.myAnswer=DuUdMapDefinitionResult::UNIQUE_INSIDE;
      else if (matchNumber>1 
	       && 
	       aSize==matchNumber)
	theResult.myAnswer=DuUdMapDefinitionResult::AMBIGUOUS_INSIDE;
      else 
	THROW_LOGICEXCEPTION_MACRO("DuUdMapEntry::definition: missing case");
      return theResult;
    } 
    return theResult;
  } 

  const DuUdMapUseResult DuUdMap::use(const StatementIdSetMapKey& aKey,
				      const ObjectWithId::Id& statementId,
				      const DuUdMapUseResult::StatementIdLists& idLists) const {
    DuUdMapUseResult theResult;
    if (aKey.getKind()==InfoMapKey::TEMP_VAR)
      // obviously because the map doesn't contain any info on temporaries, 
      // the calling context should figure it out itself
      THROW_LOGICEXCEPTION_MACRO("DuUdMap::use: not supported for temporaries");
    if (aKey.getKind()!=InfoMapKey::NO_INFO) { 
      // we get the entry:
      if (aKey.getKey()<0 
	  || 
	  aKey.getKey()>=getEntries().size())  
	// have an explicit check here rather than using 'at' which 
	// wouldn't hint where the problem is...
	THROW_LOGICEXCEPTION_MACRO("DuUdMap::use: key >" 
				   << aKey.getKey() 
				   << "< out of range");
      const StatementIdSetMapEntry& theEntry(getEntry(aKey));
      if (theEntry.getStatementIdSet().empty()) { 
	DBG_MACRO(DbgGroup::ERROR,"DuUdMapEntry::use: an empty StatementIdSet implies dead code, the subsequent transformations may fail");
	return theResult;
      }
      unsigned int aSize(theEntry.getStatementIdSet().size());
      unsigned int matchNumber=0;
      bool hasOutOfScope=false;
      bool passedDef=false; // did we come across the definition?
      bool loopCarried=false;
      ActiveUseType::ActiveUseType_E thisUse(ActiveUseType::ACTIVEUSE);
      for(StatementIdSet::const_iterator chainI=theEntry.getStatementIdSet().begin();
	  chainI!=theEntry.getStatementIdSet().end();
	  ++chainI) { // for each chain entry:
	for(StatementIdList::const_iterator statementIdListI=idLists.myStatementIdList.begin();
	    statementIdListI!=idLists.myStatementIdList.end();
	    ++statementIdListI) { 
	  if (*statementIdListI=="")
	    THROW_LOGICEXCEPTION_MACRO("DuUdMapEntry::use: StatementIds in the active statement id list cannot be empty");
	  if (*statementIdListI==*chainI) { // found a use
	    matchNumber++;
	    if (!passedDef) // we didn't come across a definition but it is used
	      loopCarried=true; //so it may be loop carried
	    // if it is a passive use if we find it in the passive list...
	    // note, we do not look at the depdentList bc by now some active statements may have been removed and 
	    // we wouldn't find it there and so the default remains "active" use
	    for(StatementIdList::const_iterator passiveStatementIdListI=idLists.myPassiveStatementIdList.begin();
		passiveStatementIdListI!=idLists.myPassiveStatementIdList.end();
		++passiveStatementIdListI) {
	      if (*passiveStatementIdListI==*chainI) { 
		thisUse=ActiveUseType::PASSIVEUSE;
		break;
	      }
	    }
	  }
	  if (matchNumber==1) { 
	    theResult.myStatementId=*chainI; // the (first) use
	    theResult.myActiveUse=thisUse;
	  }
	  else { 
	    if (theResult.myActiveUse!=ActiveUseType::UNDEFINEDUSE 
		&& 
		theResult.myActiveUse!=thisUse) { 
	      // so the use was active or passive but now it is not the same: 
	      theResult.myActiveUse=ActiveUseType::UNDEFINEDUSE;
	    }
	  } 
	  if (*statementIdListI==statementId) // we reached the definition statement
	    passedDef=true;
	}
	if (*chainI=="") 
	  hasOutOfScope=true;
      }
      if (loopCarried)
	hasOutOfScope=true;
      if ((matchNumber==0 
	   &&
	   (hasOutOfScope
	    || 
	    (!hasOutOfScope 
	     && 
	     aSize>1))))
	theResult.myAnswer=DuUdMapDefinitionResult::AMBIGUOUS_OUTSIDE;
      else if (matchNumber==0 
	       && 
	       !hasOutOfScope
	       && 
	       aSize==1)
	theResult.myAnswer=DuUdMapDefinitionResult::UNIQUE_OUTSIDE;
      else if (matchNumber>0 
	       && 
	       (hasOutOfScope
		|| 
		aSize>matchNumber))
	theResult.myAnswer=DuUdMapDefinitionResult::AMBIGUOUS_BOTHSIDES;
      else if (matchNumber==1 
	       && 
	       aSize==1)
	theResult.myAnswer=DuUdMapDefinitionResult::UNIQUE_INSIDE;
      else if (matchNumber>1 
	       && 
	       aSize==matchNumber)
	theResult.myAnswer=DuUdMapDefinitionResult::AMBIGUOUS_INSIDE;
      else 
	THROW_LOGICEXCEPTION_MACRO("DuUdMap::use: missing case");
      return theResult;
    } 
    return theResult;
  } 

  bool DuUdMap::sameDefinition(const StatementIdSetMapKey& aKey,
			       const StatementIdSetMapKey& anotherKey) const {
    if (aKey.getKind()==InfoMapKey::TEMP_VAR
	|| 
	anotherKey.getKind()==InfoMapKey::TEMP_VAR)
      THROW_LOGICEXCEPTION_MACRO("DuUdMap::sameDefinition: not supported for temporaries");
    if (aKey.getKind()!=InfoMapKey::NO_INFO
	&&
	anotherKey.getKind()!=InfoMapKey::NO_INFO) { 
      if (aKey.getKey()<0 
	  || 
	  aKey.getKey()>=getEntries().size()
	  ||
	  anotherKey.getKey()<0 
	  || 
	  anotherKey.getKey()>=getEntries().size()) { 
	THROW_LOGICEXCEPTION_MACRO("DuUdMap::sameDefinition: key >" 
				   << aKey.getKey() 
				   << "< or key >"
				   << anotherKey.getKey()
				   << "< out of range");
      }
      const StatementIdSetMapEntry& theEntry(getEntry(aKey));
      const StatementIdSetMapEntry& anotherEntry(getEntry(anotherKey));
      unsigned int aSize(theEntry.getStatementIdSet().size());
      unsigned int anotherSize(anotherEntry.getStatementIdSet().size());
      if (!aSize 
	  ||
	  !anotherSize) { 
	THROW_LOGICEXCEPTION_MACRO("DuUdMap::sameDefinitionAs: empty chain(s)");
      }
      return (aSize==1 && anotherSize==1 &&
	      !(*(theEntry.getStatementIdSet().begin())).empty()
	      &&
	      *(theEntry.getStatementIdSet().begin())==*(anotherEntry.getStatementIdSet().begin()));
    }
    return false;
  } 

  bool DuUdMap::disjointDefinition(const StatementIdSetMapKey& aKey,
				   const StatementIdSetMapKey& anotherKey) const {
    if (aKey.getKind()==InfoMapKey::TEMP_VAR
	|| 
	anotherKey.getKind()==InfoMapKey::TEMP_VAR)
      THROW_LOGICEXCEPTION_MACRO("DuUdMap::disjointDefinition: not supported for temporaries");
    if (aKey.getKind()!=InfoMapKey::NO_INFO
	&&
	anotherKey.getKind()!=InfoMapKey::NO_INFO) { 
      if (aKey.getKey()<0 
	  || 
	  aKey.getKey()>=getEntries().size()
	  ||
	  anotherKey.getKey()<0 
	  || 
	  anotherKey.getKey()>=getEntries().size()) { 
	THROW_LOGICEXCEPTION_MACRO("DuUdMap::sameDefinition: key >" 
				   << aKey.getKey() 
				   << "< or key >"
				   << anotherKey.getKey()
				   << "< out of range");
      }
      const StatementIdSetMapEntry& theEntry(getEntry(aKey));
      const StatementIdSetMapEntry& anotherEntry(getEntry(anotherKey));
      if (theEntry.getStatementIdSet().empty()
	  ||
	  anotherEntry.getStatementIdSet().empty()) { 
	THROW_LOGICEXCEPTION_MACRO("DuUdMap::disjointDefinitionFrom: empty chain(s)");
      }
      for(StatementIdSet::const_iterator chainI=theEntry.getStatementIdSet().begin();
	  chainI!=theEntry.getStatementIdSet().end();
	  ++chainI) {
	for(StatementIdSet::const_iterator anotherChainI=anotherEntry.getStatementIdSet().begin();
	    anotherChainI!=anotherEntry.getStatementIdSet().end();
	    ++anotherChainI) {
	  if (*anotherChainI==*chainI) { 
	    return false; 
	  }
	}
      }
      return true;
    }
    return false;
  } 

}
