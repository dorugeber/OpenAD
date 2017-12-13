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

#include "xaifBooster/system/inc/ConceptuallyStaticInstances.hpp"
#include "xaifBooster/system/inc/CallGraph.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/VertexIdentificationList.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/VertexIdentificationListActive.hpp"

using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulation {  

  VertexIdentificationListActive::VertexIdentificationListActive() { 
  } 

  VertexIdentificationListActive::IdentificationResult::IdentificationResult(IdentificationResult_E anAnswer,
                                                                                 const Assignment* aAssignment_p,
                                                                                 const ExpressionVertex* aExpressionVertex_p) :
    myAnswer(anAnswer),
    myAssignment_p(aAssignment_p),
    myExpressionVertex_p(aExpressionVertex_p) { 
  }

  VertexIdentificationList::IdentificationResult_E
  VertexIdentificationListActive::IdentificationResult::getAnswer() const { 
    return myAnswer;
  } 

  const ExpressionVertex&
  VertexIdentificationListActive::IdentificationResult::getExpressionVertex() const { 
    if (myAnswer==NOT_IDENTIFIED ||
        !myExpressionVertex_p) 
      THROW_LOGICEXCEPTION_MACRO("VertexIdentificationListActive::getExpressionVertex: vertex not uniquely identified");
    return *myExpressionVertex_p;
  } 

  const Assignment&
  VertexIdentificationListActive::IdentificationResult::getAssignment() const { 
    if (myAnswer==NOT_IDENTIFIED ||
        !myAssignment_p) 
      THROW_LOGICEXCEPTION_MACRO("VertexIdentificationListActive::getAssignment: vertex not uniquely identified");
    return *myAssignment_p;
  } 

  VertexIdentificationListActive::ListItem::ListItem(const AliasMapKey& anAliasMapKey,
						     const StatementIdSetMapKey& aDuUdMapKey,
                                                         const Assignment& aAssignment,
                                                         const ExpressionVertex& aExpressionVertex) : 
    VertexIdentificationList::ListItem(anAliasMapKey,
				       aDuUdMapKey),
    myAssignment_p(&aAssignment),
    myExpressionVertex_p(&aExpressionVertex) {
  }

  VertexIdentificationListActive::IdentificationResult 
  VertexIdentificationListActive::aliasIdentify(const Variable& theVariable) const { 
    AliasMap& theAliasMap(ConceptuallyStaticInstances::instance()->
			  getCallGraph().getAliasMap());
    AliasMap::AliasMapKeyPList anAliasMapKeyPList;
    getAliasMapKeyPList(anAliasMapKeyPList);
    if (theAliasMap.mayAlias(theVariable.getAliasMapKey(),
			     anAliasMapKeyPList)) {
      // so there is potential 
      // try to find an exact match: 
      for (ListItemPList::const_iterator aListIterator=myList.begin();
	   aListIterator!=myList.end(); 
	   ++aListIterator) { 
	ListItem& theItem(dynamic_cast<ListItem&>(**aListIterator));
	if (theAliasMap.mustAlias(theVariable.getAliasMapKey(),
				  theItem.getAliasMapKey())) 
	  return IdentificationResult(UNIQUELY_IDENTIFIED,
                                      &theItem.getAssignment(),
                                      &theItem.getExpressionVertex());
	if (theAliasMap.mayAlias(theVariable.getAliasMapKey(),
				 theItem.getAliasMapKey())) 
	  return IdentificationResult(AMBIGUOUSLY_IDENTIFIED,
                                      &theItem.getAssignment(),
                                      &theItem.getExpressionVertex());
      } // end for 
    } // end if aliased
    return IdentificationResult(NOT_IDENTIFIED,NULL,NULL);
  } // end VertexIdentificationListActive::aliasIdentify()

  VertexIdentificationListActive::IdentificationResult 
  VertexIdentificationListActive::canIdentify(const Variable& theVariable) const { 
    if (isDuUdMapBased())
      THROW_LOGICEXCEPTION_MACRO("VertexIdentificationListActive::canIdentify: should not be invoked for DuUd map based lists");
    return aliasIdentify(theVariable);
  } // end VertexIdentificationListActive::canIdentify()

  void VertexIdentificationListActive::removeIfIdentifiable(const Variable& theVariable) { 
    if (myList.empty())
      return;
    if (isDuUdMapBased())
      return;
    IdentificationResult idResult(canIdentify(theVariable));
    while(idResult.getAnswer()!=NOT_IDENTIFIED) { 
      for (ListItemPList::iterator aListIterator=myList.begin(); aListIterator!=myList.end(); ++aListIterator) {
	if (&dynamic_cast<VertexIdentificationListActive::ListItem&>(**aListIterator).getExpressionVertex()==&idResult.getExpressionVertex()) { 
	  myList.erase(aListIterator);
	  break;
	} // end if 
      } // end for
      idResult=canIdentify(theVariable);
    } // end while 
  } 

  std::string VertexIdentificationListActive::ListItem::debug() const { 
    std::ostringstream out;
    out << "VertexIdentificationListActive::ListItem[" << VertexIdentificationList::ListItem::debug().c_str()
	<< ",myAssignment_p=" << myAssignment_p
	<< ",myExpressionVertex_p=" << myExpressionVertex_p
	<< "]" << std::ends;
    return out.str();
  }

  const ExpressionVertex&
  VertexIdentificationListActive::ListItem::getExpressionVertex() const {
    return *myExpressionVertex_p;
  }

  const Assignment&
  VertexIdentificationListActive::ListItem::getAssignment() const {
    return *myAssignment_p;
  }

  std::string VertexIdentificationListActive::debug () const { 
    std::ostringstream out;
    out << "VertexIdentificationListActive[" << VertexIdentificationList::debug().c_str()
	<< "]" << std::ends;
    return out.str();
  }

} // end of namespace 

