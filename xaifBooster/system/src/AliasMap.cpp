// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/utils/inc/LogicException.hpp"
#include "xaifBooster/utils/inc/PrintManager.hpp"
#include "xaifBooster/utils/inc/StringConversions.hpp"
#include "xaifBooster/system/inc/AliasMap.hpp"
#include "xaifBooster/system/inc/AliasMapKey.hpp"
#include "xaifBooster/system/inc/AliasMapEntry.hpp"
#include "xaifBooster/system/inc/AliasRange.hpp"

namespace xaifBooster { 

  const std::string AliasMap::ourXAIFName("xaif:AliasSetMap");
  
  AliasMap::~AliasMap() {
    for(AAVector::const_iterator it=myAAVector.begin();
	it!=myAAVector.end();
	it++)
      if (*it)
	delete (*it);
  } 

  AliasMapEntry& 
  AliasMap::addAliasMapEntry(const std::string& aKey) { 
    unsigned int intKey(StringConversions::convertToUInt(aKey));
    if (intKey>=myAAVector.size())
      // resize and initialize to 0
      myAAVector.resize(intKey>myAAVector.size()+256?intKey:myAAVector.size()+256,0);
    AliasMapEntry* aNewAliasMapEntry_p=new AliasMapEntry(intKey);
    myAAVector[intKey]=aNewAliasMapEntry_p;
    return (*aNewAliasMapEntry_p);
  } // end of AliasMap::addAliasMapEntry

  std::string AliasMap::debug() const {
    std::ostringstream out;
    out << "AliasMap[" << this 
	<< "]" << std::ends; 
    return out.str();
  } // end of AliasMap::debug
  
  void AliasMap::printXMLHierarchy(std::ostream& os) const {
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent() 
       << "<" 
       << ourXAIFName 
       << ">" 
       << std::endl; 
    if (PrintManager::isVerbose()) { 
      for(AAVector::const_iterator it=myAAVector.begin();
	  it!=myAAVector.end();
	  it++)
	if (*it)
	  (*it)->printXMLHierarchy(os);       
    }
    else { // print only the first one for referential integrity:
      if (myAAVector.begin()!=myAAVector.end()) 
	(*(myAAVector.begin()))->printXMLHierarchy(os);
    }  
    os << pm.indent() 
       << "</"
       << ourXAIFName
       << ">" << std::endl;
    pm.releaseInstance();
  } // end of  AliasMap::printXMLHierarchy

  bool AliasMap::mayAlias(const AliasMapKey& theKey, 
			  const AliasMap::AliasMapKeyPList& theList) const { 
    // some obvious things first
    // nothing to do if theList is empty
    if (theList.empty()) 
      return false; 
    // if the list isn't empty but we have no info
    if (theKey.getKind()==AliasMapKey::NO_INFO)
      return true; 
    // if the list isn't empty and it is a temporary variable
    if (theKey.getKind()==AliasMapKey::TEMP_VAR)
      // by agreed usage patterns, i.e. a single 
      // relevant assignment within a given scope
      return false; 
    // else
    AliasMapKeyPList::const_iterator i=theList.begin();
    for (;i!=theList.end();
	 ++i) { 
      if (haveNonEmptyIntersection(theKey,**i))
	break;
    } 
    return (i!=theList.end());
  }

  bool AliasMap::mayAlias(const AliasMapKey& theKey, 
			  const AliasMapKey& theOtherKey) const { 
    if (theKey.getKind()==AliasMapKey::TEMP_VAR)
      // by agreed usage patterns, i.e. a single 
      // relevant assignment within a given scope
      return false; 
    if (theKey.getKind()!=AliasMapKey::NO_INFO) 
      return haveNonEmptyIntersection(theKey,theOtherKey);
    return true;
  }

  bool AliasMap::mustAlias(const AliasMapKey& theKey, 
			   const AliasMapKey& theOtherKey) const { 
    // some obvious things first
    if (theKey.getKind()==AliasMapKey::TEMP_VAR 
	|| 
	theOtherKey.getKind()==AliasMapKey::TEMP_VAR)
      THROW_LOGICEXCEPTION_MACRO("AliasMap::mustAlias: not supported for temporaries");
    if (theKey.getKind()==AliasMapKey::NO_INFO
	||
	theOtherKey.getKind()==AliasMapKey::NO_INFO) 
      return false;
    if (theKey.getKey()<0 
	|| 
	theKey.getKey()>=myAAVector.size()
	||
	theOtherKey.getKey()<0 
	|| 
	theOtherKey.getKey()>=myAAVector.size()) 
      // have an explicit check here rather than using 'at' which 
      // wouldn't hint where the problem is...
      THROW_LOGICEXCEPTION_MACRO("AliasMap::mustAlias: key >" 
				 << theKey.getKey() 
				 << "< or >"
				 << theOtherKey.getKey()
				 << "< out of range");
    if (myAAVector[theKey.getKey()]->mustAlias(*myAAVector[theOtherKey.getKey()]))
      return true;
    return false;
  }

  bool AliasMap::subSet(const AliasMapKey& theKey, 
			const AliasMap::AliasMapKeyPList& theList) const { 
    // some obvious things first
    // nothing to do if theList is empty
    // or we have no info
    if (theList.empty() 
	||
	theKey.getKind()==AliasMapKey::NO_INFO) 
      return false; 
    // if the list isn't empty and it is a temporary variable
    if (theKey.getKind()==AliasMapKey::TEMP_VAR)
      // by agreed usage patterns, we should never ask 
      // this question
      THROW_LOGICEXCEPTION_MACRO("AliasMap::subSet: theKey has kind TEMP_VAR");
    // else
    // make a temporary union of all AliasSets in theList
    AliasSet aTemporaryUnionAliasSet;
    for (AliasMapKeyPList::const_iterator anAliasMapKeyPListI=theList.begin();
	 anAliasMapKeyPListI!=theList.end();
	 ++anAliasMapKeyPListI) {
      if ((*anAliasMapKeyPListI)->getKey()<0 
	  || 
	  (*anAliasMapKeyPListI)->getKey()>=myAAVector.size())
	THROW_LOGICEXCEPTION_MACRO("AliasMap::subSet: key from theList >" 
				     << (*anAliasMapKeyPListI)->getKey() 
				     << "< is out of range");
      const AliasSet::AliasRangePList& theAliasRangePList(myAAVector[(*anAliasMapKeyPListI)->getKey()]->getAliasSet().getAliasRangePList());
      for (AliasSet::AliasRangePList::const_iterator anAliasRangePListI=theAliasRangePList.begin();
	   anAliasRangePListI!=theAliasRangePList.end();
	   ++anAliasRangePListI) {
	aTemporaryUnionAliasSet.addAlias((*anAliasRangePListI)->min(),
					 (*anAliasRangePListI)->max(),
					 true);
      }
    }
    // now check this union against the addreses in theKey
    if (theKey.getKey()<0 
	|| 
	theKey.getKey()>=myAAVector.size())
      THROW_LOGICEXCEPTION_MACRO("AliasMap::subSet: theKey >" 
				 << theKey.getKey() 
				 << "< is out of range");
    return myAAVector[theKey.getKey()]->getAliasSet().subSetOf(aTemporaryUnionAliasSet);
  }

  bool AliasMap::haveNonEmptyIntersection(const AliasMapKey& theKey,
					  const AliasMapKey& theOtherKey) const { 
    if (theOtherKey.getKind()==AliasMapKey::TEMP_VAR)
      // by agreed usage patterns, i.e. a single 
      // relevant assignment within a given scope
      return false; 
    if (theOtherKey.getKind()!=AliasMapKey::NO_INFO) {
      if (theKey.getKey()!=theOtherKey.getKey()) { 
	if (theKey.getKey()<0 
	    || 
	    theKey.getKey()>=myAAVector.size()
	    ||
	    theOtherKey.getKey()<0 
	    || 
	    theOtherKey.getKey()>=myAAVector.size()) 
	  // have an explicit check here rather than using 'at' which 
	  // wouldn't hint where the problem is...
	  THROW_LOGICEXCEPTION_MACRO("AliasMap::haveNonEmptyIntersection: key >" 
				     << theKey.getKey() 
				     << "< or >"
				     << theOtherKey.getKey()
				     << "< out of range");
	if (myAAVector[theKey.getKey()]->disjointFrom(*myAAVector[theOtherKey.getKey()]))
	  return false;
      }
    } 
    return true;
  } 

} // end of namespace  
