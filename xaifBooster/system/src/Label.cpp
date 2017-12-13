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
#include "xaifBooster/system/inc/Label.hpp"
#include "xaifBooster/system/inc/LabelAlgFactory.hpp"

namespace xaifBooster { 

  const std::string Label::ourXAIFName("xaif:Label");

  const std::string Label::our_myId_XAIFName("vertex_id");

  Label::Label() {
        myControlFlowGraphVertexAlgBase_p=LabelAlgFactory::instance()->makeNewAlg(*this);
  }
                                                                                
  Label::~Label() {
    if (myControlFlowGraphVertexAlgBase_p) delete myControlFlowGraphVertexAlgBase_p;
  }

  void
  Label::printXMLHierarchy(std::ostream& os) const { 
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
  } // end of Label::printXMLHierarchy

  std::string Label::debug () const { 
    std::ostringstream out;
    out << "Label[" << this 
	<< ControlFlowGraphVertex::debug()
	<< "]" << std::ends;  
    return out.str();
  } // end of Label::debug

} // end of namespace xaifBooster 
