#ifndef _XAIFBASEPARSERHANDLERS_INCLUDE_
#define _XAIFBASEPARSERHANDLERS_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/utils/inc/XAIFBaseParserHelper.hpp"
#include "xaifBooster/utils/inc/LogicException.hpp"
#include "xaifBooster/utils/inc/DbgLoggerManager.hpp"

#include "xaifBooster/system/inc/AliasMapEntry.hpp"
#include "xaifBooster/system/inc/AliasRange.hpp"
#include "xaifBooster/system/inc/AliasSet.hpp"
#include "xaifBooster/system/inc/Argument.hpp"
#include "xaifBooster/system/inc/ArgumentList.hpp"
#include "xaifBooster/system/inc/ArrayAccess.hpp"
#include "xaifBooster/system/inc/Assignment.hpp"
#include "xaifBooster/system/inc/BasicBlock.hpp"
#include "xaifBooster/system/inc/BooleanOperation.hpp"
#include "xaifBooster/system/inc/Branch.hpp"
#include "xaifBooster/system/inc/ConceptuallyStaticInstances.hpp"
#include "xaifBooster/system/inc/ConcreteArgument.hpp"
#include "xaifBooster/system/inc/Constant.hpp"
#include "xaifBooster/system/inc/ControlFlowGraph.hpp"
#include "xaifBooster/system/inc/DoMap.hpp"
#include "xaifBooster/system/inc/EndBranch.hpp"
#include "xaifBooster/system/inc/EndLoop.hpp"
#include "xaifBooster/system/inc/Entry.hpp"
#include "xaifBooster/system/inc/Exit.hpp"
#include "xaifBooster/system/inc/ExpressionEdge.hpp"
#include "xaifBooster/system/inc/ForLoop.hpp"
#include "xaifBooster/system/inc/Goto.hpp"
#include "xaifBooster/system/inc/IfStatement.hpp"
#include "xaifBooster/system/inc/Intrinsic.hpp"
#include "xaifBooster/system/inc/Label.hpp"
#include "xaifBooster/system/inc/Marker.hpp"
#include "xaifBooster/system/inc/PostLoop.hpp"
#include "xaifBooster/system/inc/PreLoop.hpp"
#include "xaifBooster/system/inc/ScopeContainment.hpp"
#include "xaifBooster/system/inc/SideEffectList.hpp"
#include "xaifBooster/system/inc/SubroutineCall.hpp"
#include "xaifBooster/system/inc/Symbol.hpp"
#include "xaifBooster/system/inc/SymbolReferenceProperty.hpp"
#include "xaifBooster/system/inc/Variable.hpp"
#include "xaifBooster/system/inc/VariableSymbolReference.hpp"

namespace xaifBooster {

  class XAIFBaseParserHandlers {
  public: 

    // Here are all the parsing methods: 

#include "xaifBooster/tools/codegen/code/parse_method_decl.inc"

void onControlFlowGraphEnd(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut);

  protected:
    XAIFBaseParserHandlers(){};
    ~XAIFBaseParserHandlers(){};

  }; // end of class XMLParser

} // end of namespace 

#endif
