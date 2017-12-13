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
#include "xaifBooster/system/inc/BooleanOperation.hpp"
#include "xaifBooster/system/inc/BooleanOperationAlgFactory.hpp"

namespace xaifBooster { 

  const std::string BooleanOperation::ourXAIFName("xaif:BooleanOperation");
  const std::string BooleanOperation::our_myType_XAIFName("name");
  const std::string BooleanOperation::our_myId_XAIFName("vertex_id");

  BooleanOperation::BooleanOperation(BooleanOperationType::BooleanOperationType_E theType,
				     bool makeAlgorithm) :
    myType(theType) {
    if (makeAlgorithm)
      myExpressionVertexAlgBase_p=BooleanOperationAlgFactory::instance()->makeNewAlg(*this); 
  }

  ExpressionVertex& BooleanOperation::createCopyOfMyself(bool withAlgorithm) const { 
    BooleanOperation* aNewBooleanOperation_p=new BooleanOperation(myType);
    aNewBooleanOperation_p->setId(getId());
    //     aNewBooleanOperation_p->setId(getId());
    //     if (!isActive())
    //       aNewBooleanOperation_p->passivate();
    return *aNewBooleanOperation_p;
  } 

  std::string BooleanOperation::equivalenceSignature() const { 
    std::ostringstream oss;
    oss << "BooleanOperation::" 
	<< BooleanOperationType::toString(myType).c_str() 
	<< std::ends;
    return std::string(oss.str());
  }

  std::string BooleanOperation::debug() const { 
    std::ostringstream oss;
    oss << "BooleanOperation[" << this << "," 
	<< ExpressionVertex::debug().c_str()
	<< "myType=" << BooleanOperationType::toString(myType).c_str()
	<< "]" << std::ends;
    return std::string(oss.str());
  }

  void
  BooleanOperation::printXMLHierarchy(std::ostream& os) const {
    if (myExpressionVertexAlgBase_p
	&& 
	! ConceptuallyStaticInstances::instance()->getPrintVersion()==PrintVersion::SYSTEM_ONLY)
      getBooleanOperationAlgBase().printXMLHierarchy(os);
    else
      printXMLHierarchyImpl(os);
  } // end of printXMLHierarchy

  void BooleanOperation::printXMLHierarchyImpl(std::ostream& os) const { 
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent() 
       << "<"
       << ourXAIFName 
       << " " 
       << our_myId_XAIFName 
       << "=\"" 
       << getId().c_str()
       << "\" " 
       << our_myType_XAIFName
       << "=\""
       << BooleanOperationType::toString(myType).c_str()
       << "\"/>" 
       << std::endl; 
    pm.releaseInstance();
  } 

  BooleanOperationAlgBase&
  BooleanOperation::getBooleanOperationAlgBase() const {
    if (!myExpressionVertexAlgBase_p)
      THROW_LOGICEXCEPTION_MACRO("BooleanOperation::getBooleanOperationAlgBase: not set");
    return dynamic_cast<BooleanOperationAlgBase&>(*myExpressionVertexAlgBase_p);
  } 

  void BooleanOperation::traverseToChildren(const GenericAction::GenericAction_E anAction_c) {
    getBooleanOperationAlgBase().genericTraversal(anAction_c);
  } 

  BooleanOperationType::BooleanOperationType_E BooleanOperation::getType() const { 
    return myType;
  } 

} // end of namespace 
