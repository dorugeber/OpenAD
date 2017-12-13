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

#include "xaifBooster/algorithms/BasicBlockPreaccumulationTape/inc/CallGraphVertexAlg.hpp"

using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulationTape {

  CallGraphVertexAlg::CallGraphVertexAlg(CallGraphVertex& theContaining) : 
    CallGraphVertexAlgBase(theContaining) {
  }

  CallGraphVertexAlg::~CallGraphVertexAlg() {
  }

  std::string
  CallGraphVertexAlg::debug() const {
    std::ostringstream out;
    out << "xaifBoosterBasicBlockPreaccumulationTape::CallGraphVertexAlg[" << this
        << ",myContaining=" << getContaining().debug().c_str()
        << ",myRequiredValueSet=" << myRequiredValueSet.debug().c_str()
        << "]" << std::ends;
    return out.str();
  } // end CallGraphVertexAlg::debug()

  void
  CallGraphVertexAlg::traverseToChildren(const GenericAction::GenericAction_E anAction_c) {
  } // end CallGraphVertexAlg::traverseToChildren()

  void
  CallGraphVertexAlg::markRequiredValue(const Expression& theExpression,
                                        const ControlFlowGraphVertex& theControlFlowGraphVertex,
                                        const std::string theOriginStr) {
    myRequiredValueSet.addValueToRequiredSet(theExpression,
                                             theControlFlowGraphVertex,
                                             theOriginStr);
  } // end CallGraphVertexAlg::markRequiredValue()

} // end namespace xaifBoosterBasicBlockPreaccumulationTape

