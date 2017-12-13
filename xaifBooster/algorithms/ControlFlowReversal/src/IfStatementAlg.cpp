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

#include "xaifBooster/algorithms/ControlFlowReversal/inc/IfStatementAlg.hpp"
#include "xaifBooster/system/inc/IfStatement.hpp"

using namespace xaifBooster;

namespace xaifBoosterControlFlowReversal { 

  IfStatementAlg::IfStatementAlg(IfStatement& theContaining) : IfStatementAlgBase(theContaining), ControlFlowGraphVertexAlg(theContaining) {
  }

  IfStatementAlg::~IfStatementAlg() {}
  
  std::string
  IfStatementAlg::debug() const {
    std::ostringstream out;
    out << "xaifBoosterControlFlowReversal::IfStatementAlg[" << this
        << "]" << std::ends;
    return out.str();
  }

} // end of namespace

