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
#include "xaifBooster/system/inc/PreLoop.hpp"
#include "xaifBooster/system/inc/PreLoopAlgFactory.hpp"

namespace xaifBooster { 

  const std::string PreLoop::ourXAIFName("xaif:PreLoop");
  const std::string PreLoop::our_myId_XAIFName("vertex_id");

  PreLoop::PreLoop() {
   myControlFlowGraphVertexAlgBase_p=PreLoopAlgFactory::instance()->makeNewAlg(*this);
  }
 
  PreLoop::~PreLoop() {
    if (myControlFlowGraphVertexAlgBase_p) delete myControlFlowGraphVertexAlgBase_p;
  }

  void
  PreLoop::printXMLHierarchy(std::ostream& os) const { 
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent() 
       << "<"
       << ourXAIFName.c_str() 
       << " " 
       << our_myId_XAIFName 
       << "=\"" 
       << getId().c_str()
       << "\" " 
       << ObjectWithAnnotation::our_myAnnotation_XAIFName.c_str() 
       << "=\""
       << getAnnotation().c_str()
       << "\">" 
       << std::endl;
    getCondition().printXMLHierarchy(os);
    os << pm.indent() 
       << "</"
       << ourXAIFName.c_str() 
       << ">" 
       << std::endl;
    pm.releaseInstance();
  } // end of PreLoop::printXMLHierarchy

  std::string PreLoop::debug () const { 
    std::ostringstream out;
    out << "PreLoop[" << this 
        << BaseLoop::debug().c_str()
	<< "]" << std::ends;  
    return out.str();
  } // end of PreLoop::debug

  PreLoopAlgBase&
  PreLoop::getPreLoopAlgBase() const {
    if (!myControlFlowGraphVertexAlgBase_p)
      THROW_LOGICEXCEPTION_MACRO("PreLoop::getControlFlowGraphVertexAlgBase: not set");
    return dynamic_cast<PreLoopAlgBase&>(*myControlFlowGraphVertexAlgBase_p);
  }

} // end of namespace xaifBooster 
