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
#include "xaifBooster/system/inc/IndexTriplet.hpp"

namespace xaifBooster { 

  const std::string IndexTriplet::ourXAIFName("xaif:IndexTriplet");
  const std::string IndexTriplet::our_myIndex_XAIFName("xaif:Index");
  const std::string IndexTriplet::our_myBound_XAIFName("xaif:Bound");
  const std::string IndexTriplet::our_myStride_XAIFName("xaif:Stride");

  IndexTriplet::IndexTriplet (){
  }
  
  IndexTriplet::~IndexTriplet(){
    for (IndexPairList::const_iterator it=myIndexPairList.begin();
	 it!=myIndexPairList.end();
	 ++it)
      if ((*it).second)
	delete (*it).second;
  }

  std::string IndexTriplet::debug() const {
    std::ostringstream out;
    out << "IndexTriplet[" << this;
    if (hasExpression(IT_INDEX))
      out << "index";
    else
      out << ".";
    if (hasExpression(IT_BOUND))
      out << "bound";
    else
      out << ".";
    if (hasExpression(IT_STRIDE))
      out << "stride";
    else
      out << ".";
    out << "]" << std::ends;
    return out.str();
    return std::string("IndexTriplet");
  }

  void
  IndexTriplet::copyMyselfInto(IndexTriplet& anotherIndexTriplet) const { 
    for (IndexPairList::const_iterator i=myIndexPairList.begin();
	 i!=myIndexPairList.end();
	 ++i) { 
      Expression& theOtherExpression=anotherIndexTriplet.addExpression((*i).first);
      (*i).second->copyMyselfInto(theOtherExpression,false,false);
    }
  }

  std::string IndexTriplet::equivalenceSignature() const { 
    std::ostringstream oss;
    // JU: incomplete: 
    oss << "IndexTriplet::" << std::ends;
    return std::string(oss.str());
  } 

  void IndexTriplet::printXMLHierarchy(std::ostream& os) const {
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent() 
       << "<" 
       << ourXAIFName.c_str() 
       << ">" 
       << std::endl;
    if (hasExpression(IT_INDEX))
      printXMLHierarchyIndexTripletElement(os,getExpression(IT_INDEX),our_myIndex_XAIFName);
    if (hasExpression(IT_BOUND))
      printXMLHierarchyIndexTripletElement(os,getExpression(IT_BOUND),our_myBound_XAIFName);
    if (hasExpression(IT_STRIDE))
      printXMLHierarchyIndexTripletElement(os,getExpression(IT_STRIDE),our_myStride_XAIFName);
    os << pm.indent()
       << "</"
       << ourXAIFName.c_str()
       << ">"
       << std::endl;
    pm.releaseInstance(); 
  } // end IndexTriplet::printXMLHierarchy

  void IndexTriplet::printXMLHierarchyIndexTripletElement(std::ostream& os,
							const Expression& theIndexElement,
							const std::string& theXaifName) const { 
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent() 
       << "<" 
       << theXaifName.c_str() 
       << ">" 
       << std::endl;
    theIndexElement.printXMLHierarchyImpl(os);
    os << pm.indent() 
       << "</"
       << theXaifName.c_str()
       << ">"
       << std::endl;
    pm.releaseInstance(); 
  } // end of IndexTriplet::printXMLHierarchyIndex

  Expression& IndexTriplet::addExpression(const IndexTriplet::IndexTriplet_E& discriminator) { 
    if (hasExpression(discriminator))
      THROW_LOGICEXCEPTION_MACRO("IndexTriplet::addExpression: already have this"); 
    Expression* anExpression_p=new Expression();
    myIndexPairList.push_back(IndexPair(discriminator,anExpression_p));
    return *anExpression_p;
  } 

  const Expression& IndexTriplet::getExpression(const IndexTriplet::IndexTriplet_E& discriminator) const { 
    IndexPairList::const_iterator i=myIndexPairList.begin();
    for (;
	 i!=myIndexPairList.end();
	 ++i)
      if ((*i).first==discriminator)
	break; 
    if (i==myIndexPairList.end())
      THROW_LOGICEXCEPTION_MACRO("IndexTriplet::getExpression: do not have this");
    return *((*i).second);
  } 

  Expression& IndexTriplet::getExpression(const IndexTriplet::IndexTriplet_E& discriminator) { 
    IndexPairList::iterator i=myIndexPairList.begin();
    for (;
	 i!=myIndexPairList.end();
	 ++i)
      if ((*i).first==discriminator)
	break; 
    if (i==myIndexPairList.end())
      THROW_LOGICEXCEPTION_MACRO("IndexTriplet::getExpression: do not have this");
    return *((*i).second);
  } 
  
  bool IndexTriplet::hasExpression(const IndexTriplet::IndexTriplet_E& discriminator) const { 
    IndexPairList::const_iterator i=myIndexPairList.begin();
    for (;
	 i!=myIndexPairList.end();
	 ++i)
      if ((*i).first==discriminator)
	break; 
    return (i!=myIndexPairList.end());
  } 

  const IndexTriplet::IndexPairList& IndexTriplet::getIndexPairList() const { 
    return myIndexPairList;
  }

  IndexTriplet::IndexPairList& IndexTriplet::getIndexPairList() { 
    return myIndexPairList;
  }

  bool IndexTriplet::isScalarDeref() const {
    return (!hasExpression(IT_BOUND) && !hasExpression(IT_STRIDE));
  } 

} 
