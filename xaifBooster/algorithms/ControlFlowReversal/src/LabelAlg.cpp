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

#include "xaifBooster/algorithms/ControlFlowReversal/inc/LabelAlg.hpp"
#include "xaifBooster/system/inc/Label.hpp"

using namespace xaifBooster;

namespace xaifBoosterControlFlowReversal { 

  LabelAlg::LabelAlg(Label& theContaining) : LabelAlgBase(theContaining), ControlFlowGraphVertexAlg(theContaining) {}

  LabelAlg::~LabelAlg() {}

  std::string
  LabelAlg::debug() const {
    std::ostringstream out;
    out << "xaifBoosterControlFlowReversal::LabelAlg[" << this
        << "]" << std::ends;
    return out.str();
  }

} // end of namespace

