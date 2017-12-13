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
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/VertexIdentificationListActiveLHS.hpp"

using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulation {  

  VertexIdentificationListActiveLHS::ListItem::ListItem(const Assignment& aAssignment) : 
    VertexIdentificationListActive::ListItem(aAssignment.getLHS().getAliasMapKey(),
                                             aAssignment.getLHS().getDuUdMapKey(),
                                             aAssignment,
                                             aAssignment.getRHS().getMaxVertex()),
    myStatementId(aAssignment.getId()) {
  }

  VertexIdentificationListActiveLHS::IdentificationResult 
  VertexIdentificationListActiveLHS::canIdentify(const Variable& theVariable,
						 const ObjectWithId::Id& statementId) const { 
    if (isDuUdMapBased() 
	&& 
	theVariable.getDuUdMapKey().getKind()!=InfoMapKey::NO_INFO) { 
      StatementIdList aStatementIdList;
      getStatementIdList(aStatementIdList);
      DuUdMapDefinitionResult theResult(ConceptuallyStaticInstances::instance()->
					getCallGraph().getDuUdMap().definition(theVariable.getDuUdMapKey(),
									       statementId,
									       aStatementIdList));
      if (theResult.myAnswer==DuUdMapDefinitionResult::UNIQUE_INSIDE) {
	for (ListItemPList::const_iterator aListIterator=myList.begin();
	     aListIterator!=myList.end(); 
	     ++aListIterator) { 
	  ListItem& theItem(dynamic_cast<ListItem&>(**aListIterator));
	  if (theResult.myStatementId==theItem.myStatementId) 
	    return IdentificationResult(UNIQUELY_IDENTIFIED,
                                        &theItem.getAssignment(),
                                        &theItem.getExpressionVertex());
	} // end for
	// we are in trouble 
	THROW_LOGICEXCEPTION_MACRO("VertexIdentificationListActiveLHS::canIdentify: cannot match statement for "
				   << theResult.myStatementId 
				   << " in " 
				   << debug().c_str());
      } // end if
      else if (theResult.myAnswer==DuUdMapDefinitionResult::UNIQUE_OUTSIDE
	       ||
	       theResult.myAnswer==DuUdMapDefinitionResult::AMBIGUOUS_OUTSIDE) {
	return IdentificationResult(NOT_IDENTIFIED,NULL,NULL);
      }
      else if (theResult.myAnswer==DuUdMapDefinitionResult::AMBIGUOUS_INSIDE
	       ||
	       theResult.myAnswer==DuUdMapDefinitionResult::AMBIGUOUS_BOTHSIDES) {
	return IdentificationResult(AMBIGUOUSLY_IDENTIFIED,NULL,NULL);
      }
    }
    if (!isDuUdMapBased()){ 
      return VertexIdentificationListActive::canIdentify(theVariable);
    }
    return IdentificationResult(AMBIGUOUSLY_IDENTIFIED,NULL,NULL);
  } 

  void VertexIdentificationListActiveLHS::addElement(const Assignment& aAssignment) { 
    if (aAssignment.getLHS().getDuUdMapKey().getKind()!=InfoMapKey::NO_INFO)
      // if we ever encounter a usefull piece of duud information:
      baseOnDuUdMap();
    if (!isDuUdMapBased() 
	&& 
	canIdentify(aAssignment.getLHS(),
		    aAssignment.getId()).getAnswer()!=NOT_IDENTIFIED) 
      THROW_LOGICEXCEPTION_MACRO("VertexIdentificationListActiveLHS::addElement: new element must have a unique address");
    myList.push_back(new ListItem(aAssignment));
  } 

  std::string VertexIdentificationListActiveLHS::ListItem::debug() const { 
    std::ostringstream out;
    out << "VertexIdentificationListActiveLHS::ListItem[" << VertexIdentificationListActive::ListItem::debug().c_str()
	<< ",myStatementId="
	<< myStatementId
	<< "]" 
	<< std::ends;
    return out.str();
  }

  std::string VertexIdentificationListActiveLHS::debug () const { 
    std::ostringstream out;
    out << "VertexIdentificationListActiveLHS[" << VertexIdentificationListActive::debug().c_str()
	<< "]" 
	<< std::ends;
    return out.str();
  } 

  void VertexIdentificationListActiveLHS::getStatementIdList(StatementIdList& aStatementIdList)const { 
    for (ListItemPList::const_iterator aListIterator=myList.begin();
	 aListIterator!=myList.end(); 
	 ++aListIterator) { 
      aStatementIdList.push_back((dynamic_cast<ListItem&>(**aListIterator)).myStatementId);
    }
  } 

} // end of namespace 
