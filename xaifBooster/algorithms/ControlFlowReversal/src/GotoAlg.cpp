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

#include "xaifBooster/algorithms/ControlFlowReversal/inc/GotoAlg.hpp"
#include "xaifBooster/system/inc/Goto.hpp"

using namespace xaifBooster;

namespace xaifBoosterControlFlowReversal { 

  GotoAlg::GotoAlg(Goto& theContaining) : GotoAlgBase(theContaining), ControlFlowGraphVertexAlg(theContaining) {}

  GotoAlg::~GotoAlg() {}

  std::string
  GotoAlg::debug() const {
    std::ostringstream out;
    out << "xaifBoosterControlFlowReversal::GotoAlg[" << this
        << "]" << std::ends;
    return out.str();
  }

} // end of namespace

