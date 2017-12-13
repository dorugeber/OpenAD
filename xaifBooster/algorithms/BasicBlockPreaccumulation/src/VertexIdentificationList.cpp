// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/VertexIdentificationList.hpp"

using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulation {  

  VertexIdentificationList::VertexIdentificationList() :
    myBasedOnDuUdMapFlag(false) { 
  } 

  VertexIdentificationList::~VertexIdentificationList() {
    for (ListItemPList::iterator i=myList.begin();
	 i!=myList.end();
	 ++i) { 
      if (*i)
	delete *i;
    }
  } 

  std::string VertexIdentificationList::debug () const { 
    std::ostringstream out;
    out << "VertexIdentificationList[" 
	<< this
	<< ","
	<< "myList=";
    for (ListItemPList::const_iterator aListIterator=myList.begin();
	 aListIterator!=myList.end(); 
	 ++aListIterator)
      out << "("
	  << (*aListIterator)->debug().c_str()
	  << ")";
    out << "]" 
	<< std::ends;
    return out.str();
  } 

  VertexIdentificationList::ListItem::ListItem(const AliasMapKey& anAliasMapKey,
					       const StatementIdSetMapKey& aDuUdMapKey) : 
    myAliasMapKey(anAliasMapKey),
    myDuUdMapKey(aDuUdMapKey) {
  }
  
  const AliasMapKey& VertexIdentificationList::ListItem::getAliasMapKey() const {
    return myAliasMapKey;
  }
   
  const StatementIdSetMapKey& VertexIdentificationList::ListItem::getDuUdMapKey() const { 
    return myDuUdMapKey;
  }

  std::string VertexIdentificationList::ListItem::debug() const { 
    std::ostringstream out;
    out << "VertexIdentificationList::ListItem[" 
	<< this
	<< ","
	<< "myAliasMapKey="
	<< myAliasMapKey.debug().c_str()
	<< ","
	<< "myDuUdMapKey="
	<< myDuUdMapKey.debug().c_str()
	<< "]" 
	<< std::ends;
    return out.str();
  }

  void VertexIdentificationList::getAliasMapKeyPList(AliasMap::AliasMapKeyPList& anAliasMapKeyPList) const { 
    for (ListItemPList::const_iterator aListIterator=myList.begin();
	 aListIterator!=myList.end(); 
	 ++aListIterator) { 
      anAliasMapKeyPList.push_back(&((*aListIterator)->getAliasMapKey()));
    } 
  } 

  void VertexIdentificationList::baseOnDuUdMap()const {
    myBasedOnDuUdMapFlag=true;
  } 

  bool VertexIdentificationList::isDuUdMapBased() const { 
    return myBasedOnDuUdMapFlag;
  } 


} // end of namespace 
