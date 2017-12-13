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
#include "xaifBooster/system/inc/EndLoop.hpp"
#include "xaifBooster/system/inc/EndLoopAlgFactory.hpp"

namespace xaifBooster { 

  const std::string EndLoop::ourXAIFName("xaif:EndLoop");

  const std::string EndLoop::our_myId_XAIFName("vertex_id");

  EndLoop::EndLoop() {
        myControlFlowGraphVertexAlgBase_p=EndLoopAlgFactory::instance()->makeNewAlg(*this);
  }
                                                                                
  EndLoop::~EndLoop() {
    if (myControlFlowGraphVertexAlgBase_p) delete myControlFlowGraphVertexAlgBase_p;
  }

  void
  EndLoop::printXMLHierarchy(std::ostream& os) const { 
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent() 
       << "<"
       << ourXAIFName 
       << " " 
       << our_myId_XAIFName 
       << "=\"" 
       << getId().c_str()
       << "\" " 
       << ObjectWithAnnotation::our_myAnnotation_XAIFName.c_str() 
       << "=\""
       << getAnnotation().c_str()
       << "\"/>" 
       << std::endl;
    pm.releaseInstance();
  } // end of EndLoop::printXMLHierarchy

  std::string EndLoop::debug () const { 
    std::ostringstream out;
    out << "EndLoop[" << this 
	<< ControlFlowGraphVertex::debug()
	<< "]" << std::ends;  
    return out.str();
  } // end of EndLoop::debug

} // end of namespace xaifBooster 
