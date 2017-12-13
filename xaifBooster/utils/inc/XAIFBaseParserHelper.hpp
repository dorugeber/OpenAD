#ifndef _XAIFBASEPARSERHELPER_INCLUDE_
#define _XAIFBASEPARSERHELPER_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/utils/inc/XMLParserHelper.hpp"

namespace xaifBooster { 

  class AliasMap;
  class AliasMapEntry;
  class ArgumentList;
  class ArrayAccess;
  class Assignment;
  class BaseLoop;
  class BasicBlock;
  class Branch;
  class CallGraph;
  class ConcreteArgument;
  class Condition;
  class ControlFlowGraph;
  class DoMap;
  class DuUdMap;
  class Expression;
  class ExpressionEdge;
  class ForLoop;
  class Goto;
  class IfStatement;
  class IndexTriplet;
  class Initialization;
  class Label;
  class PostLoop;
  class PreLoop;
  class Scope;
  class Scopes;
  class SideEffectList;
  class StatementIdSetMap;
  class StatementIdSetMapEntry;
  class SubroutineCall;
  class Symbol;
  class SymbolReferenceProperty;
  class SymbolTable;
  class Update;
  class Variable;
  class VariableEdge;
  
  class XAIFBaseParserHelper : public XMLParserHelper { 
  public: 
    XAIFBaseParserHelper();
    ~XAIFBaseParserHelper(){};

#include "xaifBooster/tools/codegen/code/helper_declaration.inc"

  }; // end of class XAIFBaseParserHelper

} // end of namespace

#endif
