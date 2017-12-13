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
#include "xaifBooster/system/inc/EndBranch.hpp"
#include "xaifBooster/system/inc/EndBranchAlgFactory.hpp"

namespace xaifBooster { 

  const std::string EndBranch::ourXAIFName("xaif:EndBranch");

  const std::string EndBranch::our_myId_XAIFName("vertex_id");

  EndBranch::EndBranch() {
        myControlFlowGraphVertexAlgBase_p=EndBranchAlgFactory::instance()->makeNewAlg(*this);
  }
                                                                                
  EndBranch::~EndBranch() {
    if (myControlFlowGraphVertexAlgBase_p) delete myControlFlowGraphVertexAlgBase_p;
  }

  void
  EndBranch::printXMLHierarchy(std::ostream& os) const { 
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
  } // end of EndBranch::printXMLHierarchy

  std::string EndBranch::debug () const { 
    std::ostringstream out;
    out << "EndBranch[" << this 
	<< ControlFlowGraphVertex::debug()
	<< "]" << std::ends;  
    return out.str();
  } // end of EndBranch::debug

} // end of namespace xaifBooster 
