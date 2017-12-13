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
#include "xaifBooster/utils/inc/DbgLoggerManager.hpp"

#include "xaifBooster/algorithms/TypeChange/inc/ControlFlowGraphAlg.hpp"
#include "xaifBooster/algorithms/TypeChange/inc/SymbolAlg.hpp"

using namespace xaifBooster;

namespace xaifBoosterTypeChange { 

  bool ControlFlowGraphAlg::ourForceNonExternalRenamesFlag(false);

  ControlFlowGraphAlg::ControlFlowGraphAlg(const ControlFlowGraph& theContaining) : 
    ControlFlowGraphAlgBase(theContaining){
  }

  ControlFlowGraphAlg::~ControlFlowGraphAlg() {
  }

  void
  ControlFlowGraphAlg::printXMLHierarchy(std::ostream& os) const {
    const SymbolAlg& theSymbolAlg(dynamic_cast<const SymbolAlg&>(getContaining().
								 getSymbolReference().
								 getSymbol().
								 getSymbolAlgBase()));
    if (theSymbolAlg.hasReplacementSymbolReference())
      getContaining().printXMLHierarchyImpl(os,theSymbolAlg.getReplacementSymbolReference());
    else 
      getContaining().printXMLHierarchyImpl(os);
  }

  void 
  ControlFlowGraphAlg::algorithm_action_1() { 
    SymbolAlg& theSymbolAlg(dynamic_cast<SymbolAlg&>(getContaining().
						     getSymbolReference().
						     getSymbol().
						     getSymbolAlgBase()));
    if (forceNonExternalRenames() && !theSymbolAlg.hasReplacementSymbolReference())
      theSymbolAlg.internalRename(getContaining().getSymbolReference());
  }

  std::string
  ControlFlowGraphAlg::debug() const {
    std::ostringstream out;
    out << "xaifBoosterTypeChange::ControlFlowGraphAlg["
        << this
	<< ", containing="
	<< getContaining().debug().c_str()
        << "]" << std::ends;
    return out.str();
  }

  void ControlFlowGraphAlg::traverseToChildren(const GenericAction::GenericAction_E anAction_c) {
  }

  void ControlFlowGraphAlg::setForceNonExternalRenames() { 
    if (ourForceNonExternalRenamesFlag)
      THROW_LOGICEXCEPTION_MACRO("SubroutineCallAlg::setForceNonExternalRenames: already set");
    ourForceNonExternalRenamesFlag=true;
  } 
  
  bool ControlFlowGraphAlg::forceNonExternalRenames() const { 
    return ourForceNonExternalRenamesFlag;
  }

  SignaturePattern& ControlFlowGraphAlg::getSomewhereConstPattern() { 
    if (!mySomewhereConstPattern.initialized()) 
      mySomewhereConstPattern.setSize(getContaining().
				      getArgumentList().
				      getArgumentSymbolReferencePList().
				      size());
    return mySomewhereConstPattern;
  } 

  SignaturePattern& ControlFlowGraphAlg::getSomewhereVariablePattern() { 
    if (!mySomewhereVariablePattern.initialized()) 
      mySomewhereVariablePattern.setSize(getContaining().
					 getArgumentList().
					 getArgumentSymbolReferencePList().
					 size());
    return mySomewhereVariablePattern;
  } 
  
}


