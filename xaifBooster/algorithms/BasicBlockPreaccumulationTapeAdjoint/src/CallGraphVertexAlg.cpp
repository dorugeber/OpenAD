// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/algorithms/BasicBlockPreaccumulationTapeAdjoint/inc/CallGraphVertexAlg.hpp"

using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulationTapeAdjoint {

  CallGraphVertexAlg::CallGraphVertexAlg(CallGraphVertex& theContaining) : 
    xaifBooster::CallGraphVertexAlgBase(theContaining),
    xaifBoosterBasicBlockPreaccumulationTape::CallGraphVertexAlg(theContaining) {
  } // end CallGraphVertexAlg::CallGraphVertexAlg()

  CallGraphVertexAlg::~CallGraphVertexAlg() {
  } // end CallGraphVertexAlg::~CallGraphVertexAlg()

  void
  CallGraphVertexAlg::traverseToChildren(const GenericAction::GenericAction_E anAction_c) {
  } // end CallGraphVertexAlg::traverseToChildren()

} // end namespace xaifBoosterBasicBlockPreaccumulationTapeAdjoint

