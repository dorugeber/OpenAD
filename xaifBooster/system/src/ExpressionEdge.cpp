
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

#include "xaifBooster/utils/inc/DbgLoggerManager.hpp"
#include "xaifBooster/utils/inc/LogicException.hpp"
#include "xaifBooster/utils/inc/PrintManager.hpp"
#include "xaifBooster/utils/inc/StringConversions.hpp"

#include "xaifBooster/system/inc/ConceptuallyStaticInstances.hpp"
#include "xaifBooster/system/inc/Expression.hpp"
#include "xaifBooster/system/inc/ExpressionEdge.hpp"
#include "xaifBooster/system/inc/ExpressionEdgeAlgFactory.hpp"

namespace xaifBooster { 

  const std::string ExpressionEdge::ourXAIFName("xaif:ExpressionEdge");
  const std::string ExpressionEdge::our_myId_XAIFName("edge_id");
  const std::string ExpressionEdge::our_source_XAIFName("source");
  const std::string ExpressionEdge::our_target_XAIFName("target");
  const std::string ExpressionEdge::our_myPosition_XAIFName("position");

  ExpressionEdge::ExpressionEdge(bool makeAlgorithm): 
    myPosition(0) {
    myExpressionEdgeAlgBase_p=0;
    if (makeAlgorithm)
      myExpressionEdgeAlgBase_p=ExpressionEdgeAlgFactory::instance()->makeNewAlg(*this);
  } 

  ExpressionEdge::~ExpressionEdge(){
    if (myExpressionEdgeAlgBase_p) delete myExpressionEdgeAlgBase_p;
  }

  /*
   * if there is an algorithm, then let it decide what to print;
   * otherwise call the local implementation
   */
  void
  ExpressionEdge::printXMLHierarchy(std::ostream& os,
				    const Expression& theExpression) const { 
    if (myExpressionEdgeAlgBase_p
	&& 
	! ConceptuallyStaticInstances::instance()->getPrintVersion()==PrintVersion::SYSTEM_ONLY)
      myExpressionEdgeAlgBase_p->printXMLHierarchy(os,theExpression);
    else
      printXMLHierarchyImpl(os,theExpression);
  } // end of printXMLHierarchy

  void 
  ExpressionEdge::printXMLHierarchyImpl(std::ostream& os, 
					const Expression& theExpression) const { 
    PrintManager& pm=PrintManager::getInstance(); 
    os << pm.indent() 
       << "<" 
       << ourXAIFName 
       << " " 
       << our_myId_XAIFName 
       << "=\"" 
       << getId().c_str() 
       << "\" " 
       << our_source_XAIFName 
       << "=\"" 
       << theExpression.getSourceOf(*this).getId().c_str() 
       << "\" " 
       << our_target_XAIFName 
       << "=\"" 
       << theExpression.getTargetOf(*this).getId().c_str() 
       << "\" " 
       << our_myPosition_XAIFName 
       << "=\"" 
       << myPosition 
       << "\"/>" 
       << std::endl; 
    pm.releaseInstance();
  } // end printXMLHierarchyImpl

  std::string ExpressionEdge::debug () const { 
    std::ostringstream out;
    out << "ExpressionEdge[" << Edge::debug().c_str()
        << ",myPosition=" << myPosition
        << "]" << std::ends;
    return out.str();
  } // end debug

  void 
  ExpressionEdge::setPosition(const std::string& i) {
    if (myPosition)                              
      THROW_LOGICEXCEPTION_MACRO("ExpressionEdge::setPosition: already set");
    myPosition=StringConversions::convertToUInt(i);
  } 

  void 
  ExpressionEdge::setPosition(unsigned int i) { 
    if (myPosition)
      THROW_LOGICEXCEPTION_MACRO("ExpressionEdge::setPosition: already set");
    myPosition=i;
  } 

  unsigned int 
  ExpressionEdge::getPosition() const { 
    if (!myPosition)
      THROW_LOGICEXCEPTION_MACRO("ExpressionEdge::getPosition: not set");
    return myPosition;
  }     

  void ExpressionEdge::copyMyselfInto(ExpressionEdge& theCopy) const {
    theCopy.myPosition=myPosition;
    theCopy.setId(getId());
  } // end copyMyselfInto

  ExpressionEdge& ExpressionEdge::createCopyOfMyself(bool withAlgorithm) const {
    ExpressionEdge* theCopy;
    theCopy=new ExpressionEdge(withAlgorithm);
    theCopy->myPosition=myPosition;
    theCopy->setId(getId());
    return *theCopy;
  } // end createCopyOfMyself

  ExpressionEdgeAlgBase&
  ExpressionEdge::getExpressionEdgeAlgBase() const {
    if (!myExpressionEdgeAlgBase_p)
      THROW_LOGICEXCEPTION_MACRO("ExpressionEdge::getExpressionEdgeAlgBase: not set");
    return *myExpressionEdgeAlgBase_p;
  } // end getExpressionEdgeAlgBase

  void ExpressionEdge::traverseToChildren(const GenericAction::GenericAction_E anAction_c) {        
    getExpressionEdgeAlgBase().genericTraversal(anAction_c);
  } // end traversalToChildren 

} // end of namespace xaifBooster 
