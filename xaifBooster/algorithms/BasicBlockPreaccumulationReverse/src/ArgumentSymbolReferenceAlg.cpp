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

#include "xaifBooster/system/inc/ConceptuallyStaticInstances.hpp"
#include "xaifBooster/system/inc/ArgumentSymbolReference.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulationReverse/inc/ArgumentSymbolReferenceAlg.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulationReverse/inc/CallGraphVertexAlg.hpp"
#include "xaifBooster/algorithms/CodeReplacement/inc/ConceptuallyStaticInstances.hpp"

using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulationReverse { 

  ArgumentSymbolReferenceAlg::ArgumentSymbolReferenceAlg(ArgumentSymbolReference& theContaining):
    ArgumentSymbolReferenceAlgBase(theContaining) { 
  }

  void
  ArgumentSymbolReferenceAlg::printXMLHierarchy(std::ostream& os) const { 
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent() 
       << "<"
       << ArgumentSymbolReference::ourXAIFName.c_str() 
       << " "
       << ArgumentSymbolReference::our_myPosition_XAIFName.c_str()
       << "=\""
       << getContaining().getPosition()
       << "\" " 
       << ArgumentSymbolReference::our_symbolId_XAIFName.c_str()
       << "=\"" 
       << getContaining().getSymbol().getId().c_str()
       << "\" " 
       << ArgumentSymbolReference::our_scopeId_XAIFName.c_str() 
       << "=\"" 
       << getContaining().getScope().getId().c_str()
       << "\" " 
       << ObjectWithAnnotation::our_myAnnotation_XAIFName.c_str() 
       << "=\""
       << getContaining().getAnnotation().c_str()
       << "\" " 
       << ArgumentSymbolReference::our_myIntent_XAIFName.c_str() 
       << "=\"" 
       << IntentType::toString(myIntent).c_str()
       << "\"/>" 
       << std::endl; 
    pm.releaseInstance();
  } // end of ArgumentSymbolReferenceAlg::printXMLHierarchy
  
  std::string ArgumentSymbolReferenceAlg::debug () const { 
    std::ostringstream out;
    out << "xaifBoosterBasicBlockPreaccumulationTapeAdjoint::ArgumentSymbolReferenceAlg[" << this
 	<< "]" << std::ends;  
    return out.str();
  } // end of ArgumentSymbolReferenceAlg::debug

  void ArgumentSymbolReferenceAlg::traverseToChildren(const GenericAction::GenericAction_E anAction_c) { 
  } 

  void ArgumentSymbolReferenceAlg::algorithm_action_1() {
    myIntent=getContaining().getIntent();
    if (getContaining().getSymbol().getActiveTypeFlag()
	|| 
	CallGraphVertexAlg::changesIntent())
      // all active parameters should become inout
      myIntent=IntentType::INOUT_ITYPE;
  }

} // end of namespace
