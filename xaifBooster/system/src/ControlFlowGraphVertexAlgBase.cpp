// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/ControlFlowGraphVertexAlgBase.hpp"
#include "xaifBooster/system/inc/ControlFlowGraphVertex.hpp"

namespace xaifBooster {

  ControlFlowGraphVertexAlgBase::ControlFlowGraphVertexAlgBase(const ControlFlowGraphVertex& theContaining) :
    AlgBase<ControlFlowGraphVertex>(theContaining) {
  } // end ControlFlowGraphVertexAlgBase::ControlFlowGraphVertexAlgBase()

  ControlFlowGraphVertexAlgBase::~ControlFlowGraphVertexAlgBase() {
  } // end ControlFlowGraphVertexAlgBase::~ControlFlowGraphVertexAlgBase()

  bool
  ControlFlowGraphVertexAlgBase::hasExpression(const Expression& anExpression) const {
    return false;
  } // end ControlFlowGraphVertexAlgBase::hasExpression()

} // end namespace xaifBooster

