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
#include "xaifBooster/utils/inc/PrintManager.hpp"
#include "xaifBooster/utils/inc/LogicException.hpp"
#include "xaifBooster/system/inc/ConceptuallyStaticInstances.hpp"
#include "xaifBooster/system/inc/BasicBlock.hpp"
#include "xaifBooster/system/inc/BasicBlockAlgBase.hpp"
#include "xaifBooster/system/inc/BasicBlockAlgFactory.hpp"
#include "xaifBooster/system/inc/BasicBlockElement.hpp"
#include "xaifBooster/system/inc/Scope.hpp"
#include "xaifBooster/system/inc/Assignment.hpp"
#include "xaifBooster/system/inc/Marker.hpp"

namespace xaifBooster { 

  BasicBlock::BasicBlock(Scope& theScope,
			 bool makeAlgorithm) : 
    PlainBasicBlock(theScope) { 
    if (makeAlgorithm)
      myControlFlowGraphVertexAlgBase_p=BasicBlockAlgFactory::instance()->makeNewAlg(*this);
  }

  BasicBlock::~BasicBlock() {
    if (myControlFlowGraphVertexAlgBase_p) delete myControlFlowGraphVertexAlgBase_p;
  }

  BasicBlockAlgBase&
  BasicBlock::getBasicBlockAlgBase() const {
    if (!myControlFlowGraphVertexAlgBase_p)
      THROW_LOGICEXCEPTION_MACRO("BasicBlock::getControlFlowGraphVertexAlgBase: not set");
    return dynamic_cast<BasicBlockAlgBase&>(*myControlFlowGraphVertexAlgBase_p);
  }

  void
  BasicBlock::printXMLHierarchy(std::ostream& os) const { 
    if (myControlFlowGraphVertexAlgBase_p
	&& 
	! ConceptuallyStaticInstances::instance()->getPrintVersion()==PrintVersion::SYSTEM_ONLY)
      getBasicBlockAlgBase().printXMLHierarchy(os);
    else
      printXMLHierarchyImpl(os);
  } // end of BasicBlock::printXMLHierarchy

  void
  BasicBlock::printXMLHierarchyImpl(std::ostream& os) const { 
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent()
       << "<"
       << ourXAIFName.c_str()
       << " "
       << our_myId_XAIFName.c_str()
       << "=\""
       << getId().c_str()
       << "\" "
       << ObjectWithAnnotation::our_myAnnotation_XAIFName.c_str()
       << "=\""
       << getAnnotation().c_str()
       << "\" "
       << PlainBasicBlock::our_myScopeId_XAIFName.c_str()
       << "=\""
       << PlainBasicBlock::myScope_r.getId().c_str()
       << "\">"
       << std::endl;
    for (PlainBasicBlock::BasicBlockElementList::const_iterator li=myElementList.begin();
         li!=myElementList.end();
         li++)
      (*(li))->printXMLHierarchy(os);
    os << pm.indent()
       << "</"
       << ourXAIFName
       << ">"
       << std::endl;
    pm.releaseInstance();
  } // end of BasicBlock::printXMLHierarchyImpl

  std::string
  BasicBlock::debug() const {
    std::ostringstream out;
    out << "BasicBlock[" << PlainBasicBlock::debug().c_str()
                  << "," << ControlFlowGraphVertex::debug().c_str()
        << "]" << std::ends;
    return out.str();
  }

  void BasicBlock::traverseToChildren(const GenericAction::GenericAction_E anAction_c) {
    ConceptuallyStaticInstances::instance()->getTraversalStack().setCurrentBasicBlockInstance(*this);
    getBasicBlockAlgBase().genericTraversal(anAction_c);
    std::list<BasicBlockElement*>::iterator li=myElementList.begin();
    for (;li!=myElementList.end();li++)
      (*(li))->genericTraversal(anAction_c);
    ConceptuallyStaticInstances::instance()->getTraversalStack().resetCurrentBasicBlockInstance();
  } 

  bool BasicBlock::hasStatement(const ObjectWithId::Id& aStatementId) const { 
    for (PlainBasicBlock::BasicBlockElementList::const_iterator li=myElementList.begin();
         li!=myElementList.end();
         li++) { 
      if ((*li)->getId()==aStatementId)
	return true; 
    }
    return false; 
  } 
  
  bool
  BasicBlock::hasExpression(const Expression& anExpression) const {
    if (PlainBasicBlock::hasExpression(anExpression))
      return true;
    if (myControlFlowGraphVertexAlgBase_p)
      return myControlFlowGraphVertexAlgBase_p->hasExpression(anExpression);
    else
      return false;
  } // end BasicBlock::hasExpression()
 
  FindDefinitionResult BasicBlock::findDefinition(const ObjectWithId::Id& aStatementId) const { 
    for (PlainBasicBlock::BasicBlockElementList::const_iterator li=myElementList.begin();
         li!=myElementList.end();
         ++li) {
      if ((*li)->getId()==aStatementId) { 
	const Assignment* anAssignment_p=dynamic_cast<const Assignment*>(*li);
	if (anAssignment_p)
	  return FindDefinitionResult(anAssignment_p);
	const Marker* aMarker_p=dynamic_cast<const Marker*>(*li);
	if (aMarker_p)
	  return FindDefinitionResult(aMarker_p);
      }
    }
    return FindDefinitionResult(0); 
  } 
  
} // end of namespace xaifBooster 
