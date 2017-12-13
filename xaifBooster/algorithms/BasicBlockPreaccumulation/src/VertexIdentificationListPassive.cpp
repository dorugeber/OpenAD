// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/system/inc/ConceptuallyStaticInstances.hpp"
#include "xaifBooster/system/inc/CallGraph.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/VertexIdentificationListPassive.hpp"

using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulation {  

  VertexIdentificationListPassive::VertexIdentificationListPassive() { 
  } 

  VertexIdentificationListPassive::ListItem::ListItem(const AliasMapKey& anAliasMapKey,
						      const StatementIdSetMapKey& aDuUdMapKey,
						      const ObjectWithId::Id& aStatementId) : 
    VertexIdentificationList::ListItem(anAliasMapKey,
				       aDuUdMapKey),
    myStatementId(aStatementId) { 
  }

  std::string VertexIdentificationListPassive::ListItem::debug() const { 
    std::ostringstream out;
    out << "VertexIdentificationListPassive::ListItem[" 
	<< this
	<< ","
	<< VertexIdentificationList::ListItem::debug().c_str()
	<< "myStatementId="
	<< myStatementId.c_str()
	<< "]" 
	<< std::ends;
    return out.str();
  }

  VertexIdentificationList::IdentificationResult_E 
  VertexIdentificationListPassive::canIdentify(const Variable& theVariable,
					       const ObjectWithId::Id& statementId) const { 
    IdentificationResult_E result=NOT_IDENTIFIED;
    if (isDuUdMapBased() 
	&& 
	theVariable.getDuUdMapKey().getKind()!=InfoMapKey::NO_INFO) { 
      // if we ever encounter a bit of DuUd information:
      baseOnDuUdMap();
      StatementIdList aStatementIdList;
      getStatementIdList(aStatementIdList);
      DuUdMapDefinitionResult theResult(ConceptuallyStaticInstances::instance()->
					getCallGraph().getDuUdMap().definition(theVariable.getDuUdMapKey(),
									       statementId,
									       aStatementIdList));
      // either one or more LHSs of passive statements that we have in the list
      // means this one is guaranteed to be passive too
      if (theResult.myAnswer==DuUdMapDefinitionResult::UNIQUE_INSIDE
	  ||
	  theResult.myAnswer==DuUdMapDefinitionResult::AMBIGUOUS_INSIDE) {
	result=UNIQUELY_IDENTIFIED;
      }
      // else we can't be sure 
      else { 
	result=AMBIGUOUSLY_IDENTIFIED;
      }
    }
    // have to rely on alias information:
    if (!isDuUdMapBased()) { 
      AliasMap& theAliasMap(ConceptuallyStaticInstances::instance()->
			    getCallGraph().getAliasMap());
      AliasMap::AliasMapKeyPList anAliasMapKeyPList;
      getAliasMapKeyPList(anAliasMapKeyPList);
      if (anAliasMapKeyPList.empty()) 
	result=NOT_IDENTIFIED;
      else if (theAliasMap.subSet(theVariable.getAliasMapKey(),
				  anAliasMapKeyPList)) 
	result=UNIQUELY_IDENTIFIED;
      else 
	result=AMBIGUOUSLY_IDENTIFIED;
    }
    return result;
  } 

  void VertexIdentificationListPassive::addElement(const Variable& theVariable,
						   const ObjectWithId::Id& aStatementId) { 
    if (theVariable.getDuUdMapKey().getKind()!=InfoMapKey::NO_INFO) 
      // if we ever encounter a usefull piece of duud information:
      baseOnDuUdMap();
    if (!isDuUdMapBased() 
	&& 
	canIdentify(theVariable,
		    aStatementId)==UNIQUELY_IDENTIFIED) 
      return; 
    myList.push_back(new ListItem(theVariable.getAliasMapKey(),
				  theVariable.getDuUdMapKey(),
				  aStatementId));
  } 

  void VertexIdentificationListPassive::removeIfIdentifiable(const Variable& theVariable,
							     const ObjectWithId::Id& statementId) { 
    if (isDuUdMapBased())
      return;
    if (myList.empty())
      return;
    AliasMap& theAliasMap(ConceptuallyStaticInstances::instance()->
			  getCallGraph().getAliasMap());
    IdentificationResult_E idResult(canIdentify(theVariable,
						statementId));
    while(idResult!=NOT_IDENTIFIED) { 
      for (ListItemPList::iterator aListIterator(myList.begin());
	   aListIterator!=myList.end(); 
	   ++aListIterator) { 
	if (theAliasMap.mayAlias(theVariable.getAliasMapKey(),
				 (*aListIterator)->getAliasMapKey())) { 
	  myList.erase(aListIterator);
	  break;
	}
      } // end for 
      idResult=canIdentify(theVariable,
			   statementId);
    } // end while 
  } 

  std::string VertexIdentificationListPassive::debug () const { 
    std::ostringstream out;
    out << "VertexIdentificationListPassive[" 
	<< this 
	<< VertexIdentificationList::debug().c_str()
	<< "]"
	<< std::ends;
    return out.str();
  } 

  void VertexIdentificationListPassive::getStatementIdList(StatementIdList& aStatementIdList)const { 
    for (ListItemPList::const_iterator aListIterator=myList.begin();
	 aListIterator!=myList.end(); 
	 ++aListIterator) { 
      aStatementIdList.push_back((dynamic_cast<ListItem&>(**aListIterator)).myStatementId);
    }
  } 

} // end of namespace 

