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

#include "xaifBooster/system/inc/BasicBlockAlgBase.hpp"
#include "xaifBooster/system/inc/BasicBlock.hpp"

namespace xaifBooster {  
  
  BasicBlockAlgBase::BasicBlockAlgBase(const BasicBlock& theContaining) : 
    ControlFlowGraphVertexAlgBase(theContaining) { 
  }

  BasicBlockAlgBase::~BasicBlockAlgBase(){}

  std::string
  BasicBlockAlgBase::debug() const {
    std::ostringstream out;
    out << "BasicBlockAlgBase[" << this
        << ",myContaining=" << dynamic_cast<const BasicBlock&>(myContaining).debug().c_str()
        << "]" << std::ends;  
    return out.str();
  }

} 
