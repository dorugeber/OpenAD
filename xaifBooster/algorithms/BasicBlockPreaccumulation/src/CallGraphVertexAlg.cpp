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
#include "xaifBooster/system/inc/SideEffectListType.hpp"
#include "xaifBooster/algorithms/TypeChange/inc/SymbolAlg.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/CallGraphVertexAlg.hpp"

using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulation {

  CallGraphVertexAlg::CallGraphVertexAlg(const CallGraphVertex& theContaining) :
    CallGraphVertexAlgBase(theContaining),
    myBasicControlFlowGraph_p(NULL) {
  }

  CallGraphVertexAlg::~CallGraphVertexAlg() {
    if (myBasicControlFlowGraph_p) { 
      delete myBasicControlFlowGraph_p;
    }
  }

  void
  CallGraphVertexAlg::printXMLHierarchy(std::ostream& os) const {
    PrintManager& pm=PrintManager::getInstance();
    const xaifBoosterTypeChange::SymbolAlg& theSymbolAlg(dynamic_cast<const xaifBoosterTypeChange::SymbolAlg&>(getContaining().getControlFlowGraph().
													       getSymbolReference().
													       getSymbol().
													       getSymbolAlgBase()));
    if (theSymbolAlg.hasReplacementSymbolReference())
      getContaining().getControlFlowGraph().printXMLHierarchyImplHead(os,
								      theSymbolAlg.getReplacementSymbolReference(),
								      pm);
    else 
      getContaining().getControlFlowGraph().printXMLHierarchyImplHead(os,
								      getContaining().getControlFlowGraph().getSymbolReference(),
								      pm);
    myBasicControlFlowGraph_p->printXMLHierarchy(os);
    getContaining().getControlFlowGraph().printXMLHierarchyImplTail(os,
								    pm);
    pm.releaseInstance();
  }

  void 
  CallGraphVertexAlg::algorithm_action_1() {
    if (myBasicControlFlowGraph_p)
      THROW_LOGICEXCEPTION_MACRO("CallGraphVertexAlg::algorithm_action_1: already run");
    myBasicControlFlowGraph_p = new BasicControlFlowGraph(getContaining().getControlFlowGraph());
    myBasicControlFlowGraph_p->makeThisACopyOfOriginalControlFlowGraph();
    if (getContaining().getInitializeDerivativeComponentsFlag()) {
      // add inert basic block
      myBasicControlFlowGraph_p->insertBasicBlock();
    }
  }

  std::string
  CallGraphVertexAlg::debug() const {
    std::ostringstream out;
    out << "xaifBoosterBasicBlockPreaccumulation::CallGraphVertexAlg["
        << this
	<< ", containing="
	<< getContaining().debug().c_str()
        << "]" << std::ends;
    return out.str();
  }

  void CallGraphVertexAlg::traverseToChildren(const GenericAction::GenericAction_E anAction_c) {
  }
  
} // end namespace xaifBoosterBasicBlockPreaccumulation
