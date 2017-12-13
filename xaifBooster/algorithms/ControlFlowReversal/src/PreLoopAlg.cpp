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

#include "xaifBooster/algorithms/ControlFlowReversal/inc/PreLoopAlg.hpp"
#include "xaifBooster/system/inc/PreLoop.hpp"

using namespace xaifBooster;

namespace xaifBoosterControlFlowReversal { 

  PreLoopAlg::PreLoopAlg(PreLoop& theContaining) : PreLoopAlgBase(theContaining), ControlFlowGraphVertexAlg(theContaining) {}

  PreLoopAlg::~PreLoopAlg() {}

  std::string
  PreLoopAlg::debug() const {
    std::ostringstream out;
    out << "xaifBoosterControlFlowReversal::PreLoopAlg[" << this
        << "]" << std::ends;
    return out.str();
  }

} // end of namespace

