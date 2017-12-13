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
#include "xaifBooster/system/inc/ControlFlowGraphEdge.hpp"
#include "xaifBooster/system/inc/ControlFlowGraphBase.hpp"

namespace xaifBooster { 

  const std::string ControlFlowGraphEdge::ourXAIFName("xaif:ControlFlowEdge");
  const std::string ControlFlowGraphEdge::our_myId_XAIFName("edge_id");
  const std::string ControlFlowGraphEdge::our_source_XAIFName("source");
  const std::string ControlFlowGraphEdge::our_target_XAIFName("target");
  const std::string ControlFlowGraphEdge::our_myConditionValueFlag_XAIFName("has_condition_value");
  const std::string ControlFlowGraphEdge::our_myConditionValue_XAIFName("condition_value");

  bool ControlFlowGraphEdge::hasConditionValue() const {
    return myConditionValueFlag;
  }
                                                                                
  void ControlFlowGraphEdge::setConditionValue(int cv) {
    if (myConditionValueFlag)
      THROW_LOGICEXCEPTION_MACRO("ControlFlowGraphEdge::setConditionValue: already set");
    myConditionValue=cv;
    myConditionValueFlag=true;
  }

  int ControlFlowGraphEdge::getConditionValue() const {
    if (!myConditionValueFlag)
      THROW_LOGICEXCEPTION_MACRO("ControlFlowGraphEdge::getConditionValue: not set");
    return myConditionValue;
  }

  void
  ControlFlowGraphEdge::printXMLHierarchy(std::ostream& os,
					  const ControlFlowGraphBase& theGraph) const { 
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent()
       << "<"
       << ourXAIFName.c_str() 
       << " " 
       << our_myId_XAIFName.c_str()
       << "=\"" 
       << getId().c_str()
       << "\" " 
       << our_source_XAIFName.c_str()
       << "=\"" 
       << theGraph.getSourceOf(*this).getId().c_str()
       << "\" " 
       << our_target_XAIFName.c_str() 
       << "=\"" 
       << theGraph.getTargetOf(*this).getId().c_str()
       << "\" " 
       << our_myConditionValueFlag_XAIFName.c_str() 
       << "=\"" 
       << hasConditionValue()
       << "\"";
    if (hasConditionValue()) {
      os << " "
	 << our_myConditionValue_XAIFName.c_str() 
	 << "=\"" 
	 << getConditionValue()
	 << "\"";
    }
    os << "/>" 
       << std::endl; 
    pm.releaseInstance();
  } // end of ControlFlowGraphEdge::printXMLHierarchy

  std::string ControlFlowGraphEdge::debug () const { 
    std::ostringstream out;
    out << "ControlFlowGraphEdge[" << this 
	<< Edge::debug()
	<< "]" << std::ends;  
    return out.str();
  } // end of ControlFlowGraphEdge::debug

  bool ControlFlowGraphEdge::leadsToLoopBody() const {
    return (myConditionValueFlag && myConditionValue==1);
  }

} // end of namespace xaifBooster 
