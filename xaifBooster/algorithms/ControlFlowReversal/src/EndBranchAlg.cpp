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

#include "xaifBooster/algorithms/ControlFlowReversal/inc/EndBranchAlg.hpp"
#include "xaifBooster/system/inc/EndBranch.hpp"

using namespace xaifBooster;

namespace xaifBoosterControlFlowReversal { 

  EndBranchAlg::EndBranchAlg(EndBranch& theContaining) : EndBranchAlgBase(theContaining), ControlFlowGraphVertexAlg(theContaining) {}

  EndBranchAlg::~EndBranchAlg() {}

  std::string
  EndBranchAlg::debug() const {
    std::ostringstream out;
    out << "xaifBoosterControlFlowReversal::EndBranchAlg[" << this
        << "]" << std::ends;
    return out.str();
  }

} // end of namespace

