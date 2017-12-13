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

#include "xaifBooster/algorithms/ControlFlowReversal/inc/BranchAlg.hpp"
#include "xaifBooster/system/inc/Branch.hpp"

using namespace xaifBooster;

namespace xaifBoosterControlFlowReversal { 

  BranchAlg::BranchAlg(Branch& theContaining) : BranchAlgBase(theContaining), ControlFlowGraphVertexAlg(theContaining) {
  }

  BranchAlg::~BranchAlg() {}
  
  std::string
  BranchAlg::debug() const {
    std::ostringstream out;
    out << "xaifBoosterControlFlowReversal::BranchAlg[" << this
        << "]" << std::ends;
    return out.str();
  }

} // end of namespace

