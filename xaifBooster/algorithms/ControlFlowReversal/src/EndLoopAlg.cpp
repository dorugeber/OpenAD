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

#include "xaifBooster/algorithms/ControlFlowReversal/inc/EndLoopAlg.hpp"
#include "xaifBooster/system/inc/EndLoop.hpp"

using namespace xaifBooster;

namespace xaifBoosterControlFlowReversal { 

  EndLoopAlg::EndLoopAlg(EndLoop& theContaining) : EndLoopAlgBase(theContaining), ControlFlowGraphVertexAlg(theContaining) {}

  EndLoopAlg::~EndLoopAlg() {}

  std::string
  EndLoopAlg::debug() const {
    std::ostringstream out;
    out << "xaifBoosterControlFlowReversal::EndLoopAlg[" << this
        << "]" << std::ends;
    return out.str();
  }

} // end of namespace

