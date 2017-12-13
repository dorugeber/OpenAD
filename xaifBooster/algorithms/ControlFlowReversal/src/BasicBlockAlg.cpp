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

#include "xaifBooster/algorithms/ControlFlowReversal/inc/BasicBlockAlg.hpp"
#include "xaifBooster/system/inc/BasicBlock.hpp"

using namespace xaifBooster;

namespace xaifBoosterControlFlowReversal { 

  BasicBlockAlg::BasicBlockAlg(BasicBlock& theContaining) : 
    BasicBlockAlgBase(theContaining),
    ControlFlowGraphVertexAlg(theContaining),
    myEnumVal(0) {
  }

  BasicBlockAlg::~BasicBlockAlg() {
  }
  
  std::string
  BasicBlockAlg::debug() const {
    std::ostringstream out;
    out << "xaifBoosterControlFlowReversal::BasicBlockAlg[" << this
        << "]" << std::ends;
    return out.str();
  }

  void BasicBlockAlg::setEnumVal(unsigned short anEnumVal) { 
    myEnumVal=anEnumVal;
  }

  unsigned short BasicBlockAlg::getEnumVal() const { 
    return myEnumVal;
  }

} // end of namespace

