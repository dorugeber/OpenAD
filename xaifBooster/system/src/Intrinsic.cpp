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
#include "xaifBooster/system/inc/ConceptuallyStaticInstances.hpp"
#include "xaifBooster/system/inc/Intrinsic.hpp"
#include "xaifBooster/system/inc/IntrinsicAlgFactory.hpp"

namespace xaifBooster { 

  const std::string Intrinsic::ourXAIFName("xaif:Intrinsic");
  const std::string Intrinsic::our_myName_XAIFName("name");
  const std::string Intrinsic::our_myId_XAIFName("vertex_id");

  Intrinsic::Intrinsic(const std::string& aName, bool makeAlgorithm) :
    myName(aName),
    myInlinableIntrinsicsCatalogueItem_p (0),
    myNonInlinableIntrinsicsCatalogueItem_p(0) {
    myExpressionVertexAlgBase_p=0;
    if (makeAlgorithm)
      myExpressionVertexAlgBase_p=IntrinsicAlgFactory::instance()->makeNewAlg(*this);
  }

  ExpressionVertex& Intrinsic::createCopyOfMyself(bool withAlgorithm) const { 
    Intrinsic* aNewIntrinsic_p=new Intrinsic(myName,withAlgorithm);
    aNewIntrinsic_p->setId(getId());
    if (annotationIsSet())
      aNewIntrinsic_p->setAnnotation(getAnnotation());
    return *aNewIntrinsic_p;
  } 

  std::string Intrinsic::equivalenceSignature() const { 
    std::ostringstream oss;
    oss << "Intrinsic::" << myName.c_str() << std::ends;
    return std::string(oss.str());
  }

  std::string Intrinsic::debug() const { 
    std::ostringstream oss;
    oss << "Intrinsic[" << ExpressionVertex::debug().c_str()
	<< ",myName=" << myName.c_str()
	<< "]" << std::ends;
    return std::string(oss.str());
  }

  const InlinableIntrinsicsCatalogueItem&
  Intrinsic::getInlinableIntrinsicsCatalogueItem() const {
    getInfo();
    if (!myInlinableIntrinsicsCatalogueItem_p) {
      THROW_LOGICEXCEPTION_MACRO("Intrinsic::getInlinableIntrinsicsCatalogueItem(): >" << myName.c_str() << "< is not an inlinable intrinsic.");
    }
    return *myInlinableIntrinsicsCatalogueItem_p;
  } 

  const NonInlinableIntrinsicsCatalogueItem&
  Intrinsic::getNonInlinableIntrinsicsCatalogueItem() const {
    getInfo();
    if (!myNonInlinableIntrinsicsCatalogueItem_p) {
      THROW_LOGICEXCEPTION_MACRO("Intrinsic::getNonInlinableIntrinsicsCatalogueItem(): >" << myName.c_str() << "< is not a non-inlinable intrinsic.");
    }
    return *myNonInlinableIntrinsicsCatalogueItem_p;
  }

  void
  Intrinsic::printXMLHierarchy(std::ostream& os) const {
    if (myExpressionVertexAlgBase_p
	&& 
	! ConceptuallyStaticInstances::instance()->getPrintVersion()==PrintVersion::SYSTEM_ONLY)
      getIntrinsicAlgBase().printXMLHierarchy(os);
    else
      printXMLHierarchyImpl(os);
  } // end of printXMLHierarchy

  void Intrinsic::printXMLHierarchyImpl(std::ostream& os) const { 
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent() 
       << "<"
       << ourXAIFName.c_str() 
       << " " 
       << our_myId_XAIFName.c_str() 
       << "=\"" 
       << getId().c_str()
       << "\" ";
    if (annotationIsSet())
      os << ObjectWithAnnotation::our_myAnnotation_XAIFName.c_str()
	 << "=\""
	 << getAnnotation().c_str()
	 << "\" "; 
    os << our_myName_XAIFName.c_str()
       << "=\""
       << myName.c_str()
       << "\""
       << "/>" 
       << std::endl; 
    pm.releaseInstance();
  } 

  IntrinsicAlgBase&
  Intrinsic::getIntrinsicAlgBase() const {
    if (!myExpressionVertexAlgBase_p)
      THROW_LOGICEXCEPTION_MACRO("Intrinsic::getIntrinsicAlgBase: not set");
    return dynamic_cast<IntrinsicAlgBase&>(*myExpressionVertexAlgBase_p);
  } 

  void Intrinsic::traverseToChildren(const GenericAction::GenericAction_E anAction_c) {
    getIntrinsicAlgBase().genericTraversal(anAction_c);
  } 

  const std::string& Intrinsic::getName() const { 
    return myName;
  } 

  void Intrinsic::getInfo() const {
    if (!myInlinableIntrinsicsCatalogueItem_p
        &&
        ! myNonInlinableIntrinsicsCatalogueItem_p) {
      try {
           myInlinableIntrinsicsCatalogueItem_p=&(ConceptuallyStaticInstances::instance()->getInlinableIntrinsicsCatalogue().getElement(myName));
      }
      catch (InlinableIntrinsicsCatalogue::HashTableType::NotFound& ei)  {
        try {
          myNonInlinableIntrinsicsCatalogueItem_p=&(ConceptuallyStaticInstances::instance()->getNonInlinableIntrinsicsCatalogue().getElement(myName));
        }
        catch (NonInlinableIntrinsicsCatalogue::HashTableType::NotFound& eni) {
          THROW_LOGICEXCEPTION_MACRO("Intrinsic::getInfo() cannot find catalogue information (inlinable or non-inlinable) for an intrinsic called >" << myName << "<");
        }
      }
    }
  }

  bool Intrinsic::isNonSmooth() const {
    getInfo();
    if (myInlinableIntrinsicsCatalogueItem_p)
      return myInlinableIntrinsicsCatalogueItem_p->isNonSmooth();
    return myNonInlinableIntrinsicsCatalogueItem_p->isNonSmooth();
  }

  bool Intrinsic::isIntrinsic() const {
    return true;
  }

  bool Intrinsic::isInlinable() const {
    getInfo();
    return (myInlinableIntrinsicsCatalogueItem_p!=0);
  }
} // end of namespace 
