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
#include "xaifBooster/system/inc/Branch.hpp"
#include "xaifBooster/system/inc/BranchAlgFactory.hpp"

namespace xaifBooster { 

  const std::string Branch::ourXAIFName("xaif:Branch");
  const std::string Branch::our_myId_XAIFName("vertex_id");

  Branch::Branch() {
    myControlFlowGraphVertexAlgBase_p=BranchAlgFactory::instance()->makeNewAlg(*this);
  }

  Branch::~Branch() {
    if (myControlFlowGraphVertexAlgBase_p) delete myControlFlowGraphVertexAlgBase_p;
  }


  void
  Branch::printXMLHierarchy(std::ostream& os) const { 
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
    myCondition.printXMLHierarchy(os);
    os << pm.indent() 
       << "</"
       << ourXAIFName.c_str() 
       << ">" 
       << std::endl;
    pm.releaseInstance();
  } // end of Branch::printXMLHierarchy

  std::string Branch::debug () const { 
    std::ostringstream out;
    out << "Branch[" << this 
	<< "]" << std::ends;  
    return out.str();
  } // end of Branch::debug

  Condition& Branch::getCondition() { 
    return myCondition;
  } 

  const Condition& Branch::getCondition() const { 
    return myCondition;
  } 

  BranchAlgBase&
  Branch::getBranchAlgBase() const {
    if (!myControlFlowGraphVertexAlgBase_p)
      THROW_LOGICEXCEPTION_MACRO("Branch::getControlFlowGraphVertexAlgBase: not set");
    return dynamic_cast<BranchAlgBase&>(*myControlFlowGraphVertexAlgBase_p);
  }

} // end of namespace xaifBooster 
