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

#include "xaifBooster/algorithms/ControlFlowReversal/inc/ExitAlg.hpp"
#include "xaifBooster/system/inc/Exit.hpp"

using namespace xaifBooster;

namespace xaifBoosterControlFlowReversal { 

  ExitAlg::ExitAlg(Exit& theContaining) : ExitAlgBase(theContaining), ControlFlowGraphVertexAlg(theContaining) {}

  ExitAlg::~ExitAlg() {}

  std::string
  ExitAlg::debug() const {
    std::ostringstream out;
    out << "xaifBoosterControlFlowReversal::ExitAlg[" << this
        << "]" << std::ends;
    return out.str();
  }

} // end of namespace

