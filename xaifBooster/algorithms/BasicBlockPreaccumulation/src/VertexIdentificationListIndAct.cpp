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
#include "xaifBooster/system/inc/Assignment.hpp"
#include "xaifBooster/system/inc/ArrayAccess.hpp"
#include "xaifBooster/system/inc/Argument.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/VertexIdentificationListIndAct.hpp"

using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulation {  

  VertexIdentificationListIndAct::VertexIdentificationListIndAct() { 
  } 

  bool 
  VertexIdentificationListIndAct::overwrittenBy(const Variable& theLHSVariable,
						const ObjectWithId::Id& aStatementId,
						const BasicBlock& theBasicBlock) const { 
    DBG_MACRO(DbgGroup::CALLSTACK,
	      "xaifBoosterBasicBlockPreaccumulation::VertexIdentificationListIndAct::overwrittenBy called for: "
	      << theLHSVariable.debug().c_str()
	      << " in statement "
	      << aStatementId.c_str()
	      << " in "
	      << theBasicBlock.debug().c_str()
	      << " on list "
	      << debug().c_str());
    // have to rely on alias information:
    AliasMap& theAliasMap(ConceptuallyStaticInstances::instance()->
			  getCallGraph().getAliasMap());
    AliasMap::AliasMapKeyPList anAliasMapKeyPList;
    getAliasMapKeyPList(anAliasMapKeyPList);
    if (!anAliasMapKeyPList.empty()) 
      return theAliasMap.subSet(theLHSVariable.getAliasMapKey(),
				anAliasMapKeyPList); 
    return false;
  } 

  void VertexIdentificationListIndAct::addElement(const Variable& theVariable,
						  const ObjectWithId::Id& aStatementId) { 
    DBG_MACRO(DbgGroup::CALLSTACK,
	      "xaifBoosterBasicBlockPreaccumulation::VertexIdentificationListIndAct::addElement called for: "
	      << theVariable.debug().c_str()
	      << " in statement "
	      << aStatementId.c_str());
    if (theVariable.hasArrayAccess()) {
      if (theVariable.getDuUdMapKey().getKind()!=InfoMapKey::NO_INFO) 
 	// if we ever encounter a usefull piece of duud information:
 	baseOnDuUdMap();
      const ArrayAccess::IndexTripletListType itl(theVariable.getArrayAccess().getIndexTripletList()); 
      for (ArrayAccess::IndexTripletListType::const_iterator itlIt=itl.begin();
	   itlIt!=itl.end();
	   ++itlIt) { 
	for (IndexTriplet::IndexPairList::const_iterator iplIt=(*itlIt)->getIndexPairList().begin();
	     iplIt!=(*itlIt)->getIndexPairList().end();
	     ++iplIt) { 
	  Expression::CArgumentPList cargpl;
	  (*iplIt).second->appendArguments(cargpl);
	  for(Expression::CArgumentPList::const_iterator cargplIt=cargpl.begin();
	      cargplIt!=cargpl.end();
	      ++cargplIt) { 
	    addElement((*cargplIt).first->getVariable(),
		       aStatementId); // recursive descent
	    // see if we have this definition already 
	    ListItemPList::const_iterator vilIt=myList.begin();
	    for(;
		vilIt!=myList.end();
		++vilIt) { 
	      if (ConceptuallyStaticInstances::instance()->
		  getCallGraph().getDuUdMap().sameDefinition((*cargplIt).first->getVariable().getDuUdMapKey(),
							     (*vilIt)->getDuUdMapKey()))
		break;
	    }
	    if (vilIt==myList.end()) { 
	      DBG_MACRO(DbgGroup::DATA,
			"xaifBoosterBasicBlockPreaccumulation::VertexIdentificationListIndAct::addElement adding: "
			<< (*cargplIt).first->getVariable().debug().c_str()
			<< " to "
			<< debug().c_str());
	      myList.push_back(new ListItem((*cargplIt).first->getVariable().getAliasMapKey(),
					    (*cargplIt).first->getVariable().getDuUdMapKey()));
	    }
	  }
	}
      } 
    }
  } 

  std::string VertexIdentificationListIndAct::debug () const { 
    std::ostringstream out;
    out << "VertexIdentificationListIndAct[" 
	<< this 
	<< VertexIdentificationList::debug().c_str()
	<< "]"
	<< std::ends;
    return out.str();
  } 

} // end of namespace 

