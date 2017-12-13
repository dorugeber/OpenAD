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
#include "xaifBooster/system/inc/Goto.hpp"
#include "xaifBooster/system/inc/GotoAlgFactory.hpp"

namespace xaifBooster { 

  const std::string Goto::ourXAIFName("xaif:Goto");

  const std::string Goto::our_myId_XAIFName("vertex_id");

  Goto::Goto() {
        myControlFlowGraphVertexAlgBase_p=GotoAlgFactory::instance()->makeNewAlg(*this);
  }
                                                                                
  Goto::~Goto() {
    if (myControlFlowGraphVertexAlgBase_p) delete myControlFlowGraphVertexAlgBase_p;
  }

  void
  Goto::printXMLHierarchy(std::ostream& os) const { 
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent() 
       << "<"
       << ourXAIFName 
       << " " 
       << our_myId_XAIFName 
       << "=\"" 
       << getId()
       << "\" " 
       << ObjectWithAnnotation::our_myAnnotation_XAIFName.c_str() 
       << "=\""
       << getAnnotation().c_str()
       << "\"/>" 
       << std::endl;
    pm.releaseInstance();
  } // end of Goto::printXMLHierarchy

  std::string Goto::debug () const { 
    std::ostringstream out;
    out << "Goto[" << this 
	<< ControlFlowGraphVertex::debug()
	<< "]" << std::ends;  
    return out.str();
  } // end of Goto::debug

} // end of namespace xaifBooster 
