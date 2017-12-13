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

#include "xaifBooster/algorithms/ControlFlowReversal/inc/ControlFlowGraphVertexAlg.hpp"

using namespace xaifBooster;

namespace xaifBoosterControlFlowReversal { 

  ControlFlowGraphVertexAlg::ControlFlowGraphVertexAlg(ControlFlowGraphVertex& theContaining) : 
    ControlFlowGraphVertexAlgBase(theContaining) {
  }

  ControlFlowGraphVertexAlg::~ControlFlowGraphVertexAlg() {
  }

  std::string ControlFlowGraphVertexAlg::kindToString(ControlFlowGraphVertexAlg::ControlFlowGraphVertexKind_E aKind) { 
    switch(aKind) {
      case ENTRY : { return std::string("ENTRY"); }
      case EXIT : { return std::string("EXIT"); }
      case BASICBLOCK : { return std::string("BASICBLOCK"); }
      case ENDBRANCH : { return std::string("ENDBRANCH"); }
      case ENDLOOP : { return std::string("ENDLOOP"); }
      case IF : { return std::string("IF"); }
      case PRELOOP : { return std::string("PRELOOP"); }
      case FORLOOP : { return std::string("FORLOOP"); }
      case BRANCH : { return std::string("BRANCH"); }
      case LABEL : { return std::string("LABEL"); }
      case GOTO : { return std::string("GOTO"); }
      default : { break; }
    }
    return std::string("UNDEF"); 
  } 

  std::string
  ControlFlowGraphVertexAlg::kindToString() const {
    return kindToString(getKind());
  }

} // end of namespace

