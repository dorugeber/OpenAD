#ifndef _CONTROLFLOWGRAPHVERTEXKIND_INCLUDE_
#define _CONTROLFLOWGRAPHVERTEXKIND_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include <string>
#include "xaifBooster/utils/inc/PrintingIntException.hpp"

namespace xaifBooster { 
  class ControlFlowGraphVertexKind { 
public:
    enum ControlFlowGraphVertexKind_E {UNDEF_VKIND,
				       ENTRY_VKIND,
				       EXIT_VKIND,
				       IF_VKIND,
				       FORLOOP_VKIND,
				       PRELOOP_VKIND,
				       POSTLOOP_VKIND,
				       BASICBLOCK_VKIND,
				       ENDLOOP_VKIND,
				       ENDBRANCH_VKIND,
				       BRANCH_VKIND,
				       LABEL_VKIND,
				       GOTO_VKIND};

    static std::string toString(const ControlFlowGraphVertexKind_E& aKind) throw (PrintingIntException);

    static const ControlFlowGraphVertexKind_E fromString(const std::string& aName);
    
    static const std::string our_attribute_XAIFName;

  }; // end of class ControlFlowGraphVertexKind

} // end of namespace xaifBooster
                                                                     
#endif
