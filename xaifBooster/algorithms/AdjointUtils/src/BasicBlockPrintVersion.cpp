// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/algorithms/AdjointUtils/inc/BasicBlockPrintVersion.hpp"

using namespace xaifBooster;

namespace xaifBoosterAdjointUtils { 

  ForLoopReversalType::ForLoopReversalType_E BasicBlockPrintVersion::ourPrintVersion=ForLoopReversalType::ANONYMOUS;
  
  void BasicBlockPrintVersion::set(ForLoopReversalType::ForLoopReversalType_E aReversalType) {
    ourPrintVersion=aReversalType;
  }

  ForLoopReversalType::ForLoopReversalType_E BasicBlockPrintVersion::get() { 
    return ourPrintVersion;
  } 

} // end of namespace xaifBoosterAngelInterfaceAlgorithms 
