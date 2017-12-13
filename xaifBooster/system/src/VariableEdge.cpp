// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/utils/inc/PrintManager.hpp"
#include "xaifBooster/system/inc/VariableEdge.hpp"
#include "xaifBooster/system/inc/Variable.hpp"

namespace xaifBooster { 

  const std::string VariableEdge::ourXAIFName("xaif:VariableReferenceEdge");
  const std::string VariableEdge::our_myId_XAIFName("edge_id");
  const std::string VariableEdge::our_source_XAIFName("source");
  const std::string VariableEdge::our_target_XAIFName("target");

  void 
  VariableEdge::printXMLHierarchy(std::ostream& os,
					       const Variable& theGraph) const { 
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent()
       << "<" 
       << ourXAIFName 
       << " " 
       << our_myId_XAIFName 
       << "=\"" 
       << getId().c_str() 
       << "\" " 
       << our_source_XAIFName 
       << "=\"" 
       << theGraph.getSourceOf(*this).getId().c_str() 
       << "\" " 
       << our_target_XAIFName 
       << "=\"" 
       << theGraph.getTargetOf(*this).getId().c_str() 
       << "\"/>"
       << std::endl; 
    pm.releaseInstance();
  } 

   std::string VariableEdge::debug () const {
     std::ostringstream out;
     out << "VariableEdge[" << this
         << Edge::debug()
         << "]" << std::ends;
     return out.str();
   } // end of VariableEdge::debug

}
