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
#include "xaifBooster/system/inc/ArrayAccess.hpp"

namespace xaifBooster { 

  VariableVertex& 
  ArrayAccess::createCopyOfMyself() const { 
    ArrayAccess* theCopy_p=new ArrayAccess();
    for (IndexTripletListType::const_iterator i=myIndexTripletList.begin();
	 i!=myIndexTripletList.end();
	 ++i) { 
      (*i)->copyMyselfInto(theCopy_p->addIndexTriplet());
    } 
    theCopy_p->setId(getId());
    return *theCopy_p;
  }

  std::string ArrayAccess::equivalenceSignature() const { 
    std::ostringstream oss;
    // JU: incomplete: 
    oss << "ArrayAccess::" << std::ends;
    return std::string(oss.str());
  } 

  const std::string ArrayAccess::ourXAIFName("xaif:ArrayElementReference");
  const std::string ArrayAccess::our_myId_XAIFName("vertex_id");

  void ArrayAccess::printXMLHierarchy(std::ostream& os) const {
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent() 
       << "<" 
       << ourXAIFName.c_str() 
       << " " 
       << our_myId_XAIFName.c_str() 
       << "=\"" 
       << getId().c_str() 
       << "\">" 
       << std::endl;
    for (IndexTripletListType::const_iterator i=myIndexTripletList.begin();
	 i!=myIndexTripletList.end();
	 ++i) 
      (*i)->printXMLHierarchy(os);
    os << pm.indent()
       << "</"
       << ourXAIFName.c_str()
       << ">"
       << std::endl;
    pm.releaseInstance(); 
  } // end ArrayAccess::printXMLHierarchy

  const ArrayAccess::IndexTripletListType& ArrayAccess::getIndexTripletList() const {
    return myIndexTripletList;
  }
  
  ArrayAccess::IndexTripletListType& ArrayAccess::getIndexTripletList() {
    return myIndexTripletList;
  }

  IndexTriplet& ArrayAccess::addIndexTriplet() {
    IndexTriplet* anIndexTriplet_p=new IndexTriplet();
    myIndexTripletList.push_back(anIndexTriplet_p);
    return *anIndexTriplet_p;
  }

  bool
  ArrayAccess::hasExpression(const Expression& anExpression) const {
    for (IndexTripletListType::const_iterator iTripListCI = myIndexTripletList.begin();
         iTripListCI != myIndexTripletList.end(); ++iTripListCI) {
      for (IndexTriplet::IndexPairList::const_iterator anIndexPairListCI = (*iTripListCI)->getIndexPairList().begin();
           anIndexPairListCI != (*iTripListCI)->getIndexPairList().end(); ++anIndexPairListCI) {
        const Expression& theIndexExpression(*((*anIndexPairListCI).second));
        if (theIndexExpression.hasExpression(anExpression))
          return true;
      } // end for all index pairs
    } // end for all index triplets
    return false;
  } // end ArrayAccess::hasExpression()

} // end namespace xaifBooster

