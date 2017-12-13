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
#include "xaifBooster/system/inc/Exit.hpp"
#include "xaifBooster/system/inc/ExitAlgFactory.hpp"

namespace xaifBooster { 

  const std::string Exit::ourXAIFName("xaif:Exit");

  const std::string Exit::our_myId_XAIFName("vertex_id");

  Exit::Exit() {
        myControlFlowGraphVertexAlgBase_p=ExitAlgFactory::instance()->makeNewAlg(*this);
  }
                                                                                
  Exit::~Exit() {
    if (myControlFlowGraphVertexAlgBase_p) delete myControlFlowGraphVertexAlgBase_p;
  }

  void
  Exit::printXMLHierarchy(std::ostream& os) const { 
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
  } // end of Exit::printXMLHierarchy

  std::string Exit::debug () const { 
    std::ostringstream out;
    out << "Exit[" << this 
	<< ControlFlowGraphVertex::debug()
	<< "]" << std::ends;  
    return out.str();
  } // end of Exit::debug

} // end of namespace xaifBooster 
