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
#include "xaifBooster/system/inc/Entry.hpp"
#include "xaifBooster/system/inc/EntryAlgFactory.hpp"

namespace xaifBooster { 

  const std::string Entry::ourXAIFName("xaif:Entry");

  const std::string Entry::our_myId_XAIFName("vertex_id");

  Entry::Entry() { 
        myControlFlowGraphVertexAlgBase_p=EntryAlgFactory::instance()->makeNewAlg(*this);
  }

  Entry::~Entry() {
    if (myControlFlowGraphVertexAlgBase_p) delete myControlFlowGraphVertexAlgBase_p;
  }

  void
  Entry::printXMLHierarchy(std::ostream& os) const { 
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
  } // end of Entry::printXMLHierarchy

  std::string Entry::debug () const { 
    std::ostringstream out;
    out << "Entry[" << this 
	<< ControlFlowGraphVertex::debug()
	<< "]" << std::ends;  
    return out.str();
  } // end of Entry::debug

} // end of namespace xaifBooster 
