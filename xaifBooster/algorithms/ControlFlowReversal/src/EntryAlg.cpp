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

#include "xaifBooster/algorithms/ControlFlowReversal/inc/EntryAlg.hpp"
#include "xaifBooster/system/inc/Entry.hpp"

using namespace xaifBooster;

namespace xaifBoosterControlFlowReversal { 

  EntryAlg::EntryAlg(Entry& theContaining) : EntryAlgBase(theContaining), ControlFlowGraphVertexAlg(theContaining) {}

  EntryAlg::~EntryAlg() {}

  std::string
  EntryAlg::debug() const {
    std::ostringstream out;
    out << "xaifBoosterControlFlowReversal::EntryAlg[" << this
        << "]" << std::ends;
    return out.str();
  }

} // end of namespace

