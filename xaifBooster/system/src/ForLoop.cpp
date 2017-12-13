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
#include "xaifBooster/system/inc/FindDefinitionResult.hpp"
#include "xaifBooster/system/inc/ForLoop.hpp"
#include "xaifBooster/system/inc/ForLoopAlgFactory.hpp"

namespace xaifBooster { 

  const std::string ForLoop::ourXAIFName("xaif:ForLoop");

  const std::string ForLoop::our_myId_XAIFName("vertex_id");

  ForLoop::ForLoop(const ForLoopReversalType::ForLoopReversalType_E theUserReversalType) : 
    myInitialization_p(0),
    myUpdate_p(0) {
    myReversalType=theUserReversalType;
    myControlFlowGraphVertexAlgBase_p=ForLoopAlgFactory::instance()->makeNewAlg(*this);
  }
                                                                                
  ForLoop::~ForLoop() {
    if (myControlFlowGraphVertexAlgBase_p) delete myControlFlowGraphVertexAlgBase_p;
  }

  void
  ForLoop::printXMLHierarchy(std::ostream& os) const { 
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
       << ForLoopReversalType::our_attribute_XAIFName.c_str() 
       << "=\""
       << ForLoopReversalType::toString(myReversalType).c_str()
       << "\">" 
       << std::endl;
    getInitialization().printXMLHierarchy(os);
    getCondition().printXMLHierarchy(os);
    getUpdate().printXMLHierarchy(os);
    os << pm.indent() 
       << "</"
       << ourXAIFName.c_str() 
       << ">" 
       << std::endl;
    pm.releaseInstance();
  } // end of ForLoop::printXMLHierarchy

  std::string ForLoop::debug () const { 
    std::ostringstream out;
    out << "ForLoop[" << this 
	<< BaseLoop::debug().c_str()
	<< "]" << std::ends;  
    return out.str();
  } // end of ForLoop::debug

  Initialization& ForLoop::makeInitialization() {
     if (myInitialization_p) {
       THROW_LOGICEXCEPTION_MACRO("ForLoop::makeInitialization: already set");
     }
     myInitialization_p=new Initialization(true);
     return *myInitialization_p;
   }

  Initialization& ForLoop::getInitialization() { 
    if (!myInitialization_p) {
      THROW_LOGICEXCEPTION_MACRO("ForLoop::getInitialization: not set");
    }
    return *myInitialization_p;
  } 

  const Initialization& ForLoop::getInitialization() const { 
    if (!myInitialization_p) {
       THROW_LOGICEXCEPTION_MACRO("ForLoop::getInitialization: not set");
     }
    return *myInitialization_p;
  } 

  Update& ForLoop::makeUpdate() {
     if (myUpdate_p) {
       THROW_LOGICEXCEPTION_MACRO("ForLoop::makeUpdate: already set");
     }
     myUpdate_p=new Update(true);
     return *myUpdate_p;
   }

  Update& ForLoop::getUpdate() {
    if (!myUpdate_p) {
      THROW_LOGICEXCEPTION_MACRO("ForLoop::getUpdate: not set");
    }
    return *myUpdate_p;
  }

  const Update& ForLoop::getUpdate() const {
    if (!myUpdate_p) {
       THROW_LOGICEXCEPTION_MACRO("ForLoop::getUpdate: not set");
     }
    return *myUpdate_p;
  }

  ForLoopAlgBase&
  ForLoop::getForLoopAlgBase() const {
    if (!myControlFlowGraphVertexAlgBase_p)
      THROW_LOGICEXCEPTION_MACRO("ForLoop::getControlFlowGraphVertexAlgBase: not set");
    return dynamic_cast<ForLoopAlgBase&>(*myControlFlowGraphVertexAlgBase_p);
  }

  bool 
  ForLoop::hasStatement(const ObjectWithId::Id& aStatementId) const { 
    bool found=false;
    if (getInitialization().getAssignment().getId()==aStatementId)
      found=true;
    if (getUpdate().getAssignment().getId()==aStatementId) { 
      if (found)
	THROW_LOGICEXCEPTION_MACRO("ForLoop::hasStatement: duplicate id " << aStatementId.c_str() << " in loop with id " << getId().c_str());
      found=true;
    }
    return found; 
  } 

  void 
  ForLoop::addLoopVariable() { 
    myKnownLoopVariables.push_back(&(getInitialization().
				     getAssignment().
				     getLHS()));
  }

  FindDefinitionResult ForLoop::findDefinition(const ObjectWithId::Id& aStatementId) const { 
    if (aStatementId==getInitialization().getAssignment().getId())
      return FindDefinitionResult(&(getInitialization().getAssignment()));
    if (aStatementId==getUpdate().getAssignment().getId())
      return FindDefinitionResult(&(getUpdate().getAssignment()));
    return FindDefinitionResult(0);
  }

} // end of namespace xaifBooster 
