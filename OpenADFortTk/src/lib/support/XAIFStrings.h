// ##########################################################
// # This file is part of OpenADFortTk.                     #
// # The full COPYRIGHT notice can be found in the top      #
// # level directory of the OpenADFortTk source tree.       #
// # For more information visit                             #
// # http://www.mcs.anl.gov/openad                          #
// ##########################################################

#ifndef XAIFStrings_INCLUDED_h
#define XAIFStrings_INCLUDED_h

#include <iostream>

#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/util/XMLString.hpp>

XERCES_CPP_NAMESPACE_USE

// A class containing strings for XAIF.  
class XAIFStringRepository
{
public: 
  enum {
    // -----------------------------------------------------

    // Common attributes
    ATTR_annot = 0, // annotation
    ATTR_Vid,       // vertex_id
    ATTR_Eid,       // edge_id
    ATTR_scopeId,   // scope_id
    ATTR_symId,     // symbol_id

    ATTR_active,    // active
    ATTR_deriv,     // deriv

    ATTR_source,    // source
    ATTR_target,    // target

    ATTR_lineNumber,// line number
    
    // -----------------------------------------------------
    // top level
    ATTR_prefix,    // specific prefix
    
    // -----------------------------------------------------
    // xaif:CallGraph
    ELEM_CallGraph,

    // -----------------------------------------------------
    // xaif:ScopeHierarchy, xaif:Scope, xaif:SymbolTable
    ELEM_ScopeHierarchy,
    ELEM_Scope,
    ELEM_SymTab,

    // xaif:Symbol
    ELEM_Symbol,
    ATTR_kind,
    ATTR_type,
    ATTR_feType,
    ATTR_shape,
    ATTR_temp,

    // xaif:DimensionBounds
    ELEM_DimensionBounds,
    ATTR_lower,
    ATTR_upper,
    
    // -----------------------------------------------------
    
    // xaif:ControlFlowGraph, xaif:ReplacementList, xaif:Replacement
    ELEM_CFG,
    ELEM_ReplaceList,
    
    ELEM_ArgList,
    ELEM_ArgSymRef,
    ATTR_intent,
    ATTR_structured,
    
    ELEM_Replacement,
    ATTR_placeholder,
    
    ELEM_CFGEdge,
    ATTR_hasCondval,
    ATTR_condval,
    
    // xaif:BasicBlock
    ELEM_BB,
    ELEM_BBEntry,
    ELEM_BBExit,
    ELEM_BBBranch,
    ELEM_BBForLoop,
    ELEM_BBPreLoop,
    ELEM_BBPostLoop,

    ELEM_BBEndBranch,
    ELEM_BBEndLoop,
    
    ELEM_LpInit,
    ELEM_Condition,
    ELEM_LpUpdate,

    // Statements
    ELEM_Assign,
    ELEM_SubCall,
    ELEM_InlinableSubCall,
    ELEM_Marker,

    ELEM_AssignLHS,
    ELEM_AssignRHS,

    ELEM_Argument,

    ATTR_subname,
    ATTR_formalArgCount,

    // Expressions
    ELEM_VarRef,
    ELEM_Constant,
    ELEM_Intrinsic,
    ELEM_FuncCall,
    ELEM_BoolOp,

    ELEM_ExprEdge,

    ATTR_name,
    ATTR_value,
    ATTR_position,
    
    // Variable Reference Expressions
    ELEM_SymRef,
    ELEM_ArrayElemRef,
    
    ELEM_IndexTriplet,
    ELEM_Index,
    ELEM_Bound,
    ELEM_Stride,
    
    ELEM_VarRefEdge,
    
    // DerivativePropagator
    ELEM_DerivProp,  // block

    ELEM_SetDeriv,   // stmt
    ELEM_SetNegDeriv,
    ELEM_IncDeriv,
    ELEM_DecDeriv,
    ELEM_ZeroDeriv,
    ELEM_Sax,
    ELEM_Saxpy,

    ELEM_Tgt,        // expr
    ELEM_Src,
    ELEM_AX,
    ELEM_A,
    ELEM_X,
    ELEM_Y,

    // -----------------------------------------------------
    
    // Special tags
    TAG_SymTabId, // Tags a colon-separated list of symbol table (ST_TAB) ids
    TAG_SymId,    // Tags a colon-separated list of symbol (ST) ids
    
    TAG_PUId,     // Tags a colon-separated list of PU ids
    TAG_WHIRLId,  // Tags a colon-separated list of WHIRL (WN) ids
    
    TAG_StmtGoto,   // Tags a goto statement
    TAG_StmtLabel,  // Tags a label statement
    TAG_StmtReturn, // Tags a return statement
    
    TAG_IntrinsicKey, // Tags an intrinsic to disambiguate translation
    TAG_PregId,       // Tags a SymbolReference to a preg
    
    TAG_End,      // End of tag information
    
    // -----------------------------------------------------

    NUM_STRINGS // Not a valid string!
  };

public:
  XAIFStringRepository();
  ~XAIFStringRepository();
  
  // -------------------------------------------------------
  // Must be called after Xerces is initialized.  All XML strings are
  // unavailable until this has been called.
  // -------------------------------------------------------
  void XMLInitialize();

  // -------------------------------------------------------
  // General access to strings
  // -------------------------------------------------------
  
  const char* c(int i) const
  {
    if (0 <= i && i < NUM_STRINGS) { return c_strTbl[i]; }
    else { return NULL; }
  }

  XMLCh* xml(int i) const
  {
    if (0 <= i && i < NUM_STRINGS) { return x_strTbl[i]; }
    else { return NULL; }
  }

  // -------------------------------------------------------
  // Quick access to strings
  // -------------------------------------------------------
  
  // Common attributes
  const char* attr_annot() const   { return c_strTbl[ATTR_annot]; }
  XMLCh*      attr_annot_x() const { return x_strTbl[ATTR_annot]; }
  const char* attr_Vid() const { return c_strTbl[ATTR_Vid]; }
  XMLCh*      attr_Vid_x() const { return x_strTbl[ATTR_Vid]; }
  const char* attr_Eid() const { return c_strTbl[ATTR_Eid]; }
  XMLCh*      attr_Eid_x() const { return x_strTbl[ATTR_Eid]; }
  const char* attr_scopeId() const { return c_strTbl[ATTR_scopeId]; }
  XMLCh*      attr_scopeId_x() const { return x_strTbl[ATTR_scopeId]; }
  const char* attr_symId() const { return c_strTbl[ATTR_symId]; }
  XMLCh*      attr_symId_x() const { return x_strTbl[ATTR_symId]; }
  
  const char* attr_active() const { return c_strTbl[ATTR_active]; }
  XMLCh*      attr_active_x() const { return x_strTbl[ATTR_active]; }
  const char* attr_deriv() const { return c_strTbl[ATTR_deriv]; }
  XMLCh*      attr_deriv_x() const { return x_strTbl[ATTR_deriv]; }

  const char* attr_source() const { return c_strTbl[ATTR_source]; }
  XMLCh*      attr_source_x() const { return x_strTbl[ATTR_source]; }
  const char* attr_target() const { return c_strTbl[ATTR_target]; }
  XMLCh*      attr_target_x() const { return x_strTbl[ATTR_target]; }

  const char* attr_lineNumber() const { return c_strTbl[ATTR_lineNumber]; }
  XMLCh*      attr_lineNumber_x() const { return x_strTbl[ATTR_lineNumber]; }
  
  // -----------------------------------------------------
  // top level  
  const char* attr_prefix() const   { return c_strTbl[ATTR_prefix]; }
  XMLCh*      attr_prefix_x() const { return x_strTbl[ATTR_prefix]; }

  // -----------------------------------------------------
  // xaif:CallGraph
  const char* elem_CallGraph() const { return c_strTbl[ELEM_CallGraph]; }
  XMLCh*      elem_CallGraph_x() const { return x_strTbl[ELEM_CallGraph]; }

  // -----------------------------------------------------
  // xaif:ScopeHierarchy, xaif:Scope, xaif:SymbolTable
  const char* elem_ScopeHierarchy() const { return c_strTbl[ELEM_ScopeHierarchy]; }
  XMLCh*      elem_ScopeHierarchy_x() const { return x_strTbl[ELEM_ScopeHierarchy]; }
  const char* elem_Scope() const { return c_strTbl[ELEM_Scope]; }
  XMLCh*      elem_Scope_x() const { return x_strTbl[ELEM_Scope]; }
  const char* elem_SymTab() const { return c_strTbl[ELEM_SymTab]; }
  XMLCh*      elem_SymTab_x() const { return x_strTbl[ELEM_SymTab]; }
    
  // xaif:Symbol
  const char* elem_Symbol() const { return c_strTbl[ELEM_Symbol]; }
  XMLCh*      elem_Symbol_x() const { return x_strTbl[ELEM_Symbol]; }
  
  const char* attr_kind() const { return c_strTbl[ATTR_kind]; }
  XMLCh*      attr_kind_x() const { return x_strTbl[ATTR_kind]; }
  const char* attr_type() const { return c_strTbl[ATTR_type]; }
  XMLCh*      attr_type_x() const { return x_strTbl[ATTR_type]; }
  const char* attr_feType() const { return c_strTbl[ATTR_feType]; }
  XMLCh*      attr_feType_x() const { return x_strTbl[ATTR_feType]; }
  const char* attr_shape() const { return c_strTbl[ATTR_shape]; }
  XMLCh*      attr_shape_x() const { return x_strTbl[ATTR_shape]; }
  const char* attr_temp() const { return c_strTbl[ATTR_temp]; }
  XMLCh*      attr_temp_x() const { return x_strTbl[ATTR_temp]; }

  // xaif:DimensionBounds
  const char* elem_DimensionBounds() const { return c_strTbl[ELEM_DimensionBounds]; }
  XMLCh*      elem_DimensionBounds_x() const { return x_strTbl[ELEM_DimensionBounds]; }
  
  const char* attr_lower() const { return c_strTbl[ATTR_lower]; }
  XMLCh*      attr_lower_x() const { return x_strTbl[ATTR_lower]; }
  const char* attr_upper() const { return c_strTbl[ATTR_upper]; }
  XMLCh*      attr_upper_x() const { return x_strTbl[ATTR_upper]; }
  
  // -----------------------------------------------------
  
  // xaif:ControlFlowGraph
  const char* elem_CFG() const { return c_strTbl[ELEM_CFG]; }
  XMLCh*      elem_CFG_x() const { return x_strTbl[ELEM_CFG]; }
  const char* elem_ReplaceList() const { return c_strTbl[ELEM_ReplaceList]; }
  XMLCh*      elem_ReplaceList_x() const { return x_strTbl[ELEM_ReplaceList]; }

  const char* elem_ArgList() const { return c_strTbl[ELEM_ArgList]; }
  XMLCh*      elem_ArgList_x() const { return x_strTbl[ELEM_ArgList]; }
  const char* elem_ArgSymRef() const { return c_strTbl[ELEM_ArgSymRef]; }
  XMLCh*      elem_ArgSymRef_x() const { return x_strTbl[ELEM_ArgSymRef]; }
  const char* attr_intent() const { return c_strTbl[ATTR_intent]; }
  XMLCh*      attr_intent_x() const { return x_strTbl[ATTR_intent]; }
  const char* attr_structured() const { return c_strTbl[ATTR_structured]; }
  XMLCh*      attr_structured_x() const { return x_strTbl[ATTR_structured]; }
  
  const char* elem_Replacement() const { return c_strTbl[ELEM_Replacement]; }
  XMLCh*      elem_Replacement_x() const { return x_strTbl[ELEM_Replacement]; }
  const char* attr_placeholder() const { return c_strTbl[ATTR_placeholder]; }
  XMLCh*      attr_placeholder_x() const { return x_strTbl[ATTR_placeholder]; }

  const char* elem_CFGEdge() const { return c_strTbl[ELEM_CFGEdge]; }
  XMLCh*      elem_CFGEdge_x() const { return x_strTbl[ELEM_CFGEdge]; }
  const char* attr_hasCondval() const { return c_strTbl[ATTR_hasCondval]; }
  XMLCh*      attr_hasCondval_x() const { return x_strTbl[ATTR_hasCondval]; }
  const char* attr_condval() const { return c_strTbl[ATTR_condval]; }
  XMLCh*      attr_condval_x() const { return x_strTbl[ATTR_condval]; }

  // xaif:BasicBlock
  const char* elem_BB() const { return c_strTbl[ELEM_BB]; }
  XMLCh*      elem_BB_x() const { return x_strTbl[ELEM_BB]; }
  const char* elem_BBEntry() const { return c_strTbl[ELEM_BBEntry]; }
  XMLCh*      elem_BBEntry_x() const { return x_strTbl[ELEM_BBEntry]; }
  const char* elem_BBExit() const { return c_strTbl[ELEM_BBExit]; }
  XMLCh*      elem_BBExit_x() const { return x_strTbl[ELEM_BBExit]; }
  const char* elem_BBBranch() const { return c_strTbl[ELEM_BBBranch]; }
  XMLCh*      elem_BBBranch_x() const { return x_strTbl[ELEM_BBBranch]; }
  const char* elem_BBForLoop() const { return c_strTbl[ELEM_BBForLoop]; }
  XMLCh*      elem_BBForLoop_x() const { return x_strTbl[ELEM_BBForLoop]; }
  const char* elem_BBPreLoop() const { return c_strTbl[ELEM_BBPreLoop]; }
  XMLCh*      elem_BBPreLoop_x() const { return x_strTbl[ELEM_BBPreLoop]; }
  const char* elem_BBPostLoop() const { return c_strTbl[ELEM_BBPostLoop]; }
  XMLCh*      elem_BBPostLoop_x() const { return x_strTbl[ELEM_BBPostLoop]; }
  
  const char* elem_BBEndBranch() const { return c_strTbl[ELEM_BBEndBranch]; }
  XMLCh*      elem_BBEndBranch_x() const { return x_strTbl[ELEM_BBEndBranch]; }
  const char* elem_BBEndLoop() const { return c_strTbl[ELEM_BBEndLoop]; }
  XMLCh*      elem_BBEndLoop_x() const { return x_strTbl[ELEM_BBEndLoop]; }

  const char* elem_LpInit() const { return c_strTbl[ELEM_LpInit]; }
  XMLCh*      elem_LpInit_x() const { return x_strTbl[ELEM_LpInit]; }
  const char* elem_Condition() const { return c_strTbl[ELEM_Condition]; }
  XMLCh*      elem_Condition_x() const { return x_strTbl[ELEM_Condition]; }
  const char* elem_LpUpdate() const { return c_strTbl[ELEM_LpUpdate]; }
  XMLCh*      elem_LpUpdate_x() const { return x_strTbl[ELEM_LpUpdate]; }

  // Statements
  const char* elem_Assign() const { return c_strTbl[ELEM_Assign]; }
  XMLCh*      elem_Assign_x() const { return x_strTbl[ELEM_Assign]; }
  const char* elem_SubCall() const { return c_strTbl[ELEM_SubCall]; }
  XMLCh*      elem_SubCall_x() const { return x_strTbl[ELEM_SubCall]; }
  const char* elem_InlinableSubCall() const { return c_strTbl[ELEM_InlinableSubCall]; }
  XMLCh*      elem_InlinableSubCall_x() const { return x_strTbl[ELEM_InlinableSubCall]; }
  const char* elem_Marker() const { return c_strTbl[ELEM_Marker]; }
  XMLCh*      elem_Marker_x() const { return x_strTbl[ELEM_Marker]; }

  const char* elem_AssignLHS() const { return c_strTbl[ELEM_AssignLHS]; }
  XMLCh*      elem_AssignLHS_x() const { return x_strTbl[ELEM_AssignLHS]; }
  const char* elem_AssignRHS() const { return c_strTbl[ELEM_AssignRHS]; }
  XMLCh*      elem_AssignRHS_x() const { return x_strTbl[ELEM_AssignRHS]; }

  const char* elem_Argument() const { return c_strTbl[ELEM_Argument]; }
  XMLCh*      elem_Argument_x() const { return x_strTbl[ELEM_Argument]; }

  const char* attr_subname() const { return c_strTbl[ATTR_subname]; }
  XMLCh*      attr_subname_x() const { return x_strTbl[ATTR_subname]; }
  
  const char* attr_formalArgCount() const { return c_strTbl[ATTR_formalArgCount]; }
  XMLCh*      attr_formalArgCount_x() const { return x_strTbl[ATTR_formalArgCount]; }

  // Expressions
  const char* elem_VarRef() const { return c_strTbl[ELEM_VarRef]; }
  XMLCh*      elem_VarRef_x() const { return x_strTbl[ELEM_VarRef]; }
  const char* elem_Constant() const { return c_strTbl[ELEM_Constant]; }
  XMLCh*      elem_Constant_x() const { return x_strTbl[ELEM_Constant]; }
  const char* elem_Intrinsic() const { return c_strTbl[ELEM_Intrinsic]; }
  XMLCh*      elem_Intrinsic_x() const { return x_strTbl[ELEM_Intrinsic]; }
  const char* elem_FuncCall() const { return c_strTbl[ELEM_FuncCall]; }
  XMLCh*      elem_FuncCall_x() const { return x_strTbl[ELEM_FuncCall]; }
  const char* elem_BoolOp() const { return c_strTbl[ELEM_BoolOp]; }
  XMLCh*      elem_BoolOp_x() const { return x_strTbl[ELEM_BoolOp]; }
  
  const char* elem_ExprEdge() const { return c_strTbl[ELEM_ExprEdge]; }
  XMLCh*      elem_ExprEdge_x() const { return x_strTbl[ELEM_ExprEdge]; }

  const char* attr_name() const { return c_strTbl[ATTR_name]; }
  XMLCh*      attr_name_x() const { return x_strTbl[ATTR_name]; }
  const char* attr_value() const { return c_strTbl[ATTR_value]; }
  XMLCh*      attr_value_x() const { return x_strTbl[ATTR_value]; }
  const char* attr_position() const { return c_strTbl[ATTR_position]; }
  XMLCh*      attr_position_x() const { return x_strTbl[ATTR_position]; }

  // Variable Reference Expressions
  const char* elem_SymRef() const { return c_strTbl[ELEM_SymRef]; }
  XMLCh*      elem_SymRef_x() const { return x_strTbl[ELEM_SymRef]; }
  const char* elem_ArrayElemRef() const { return c_strTbl[ELEM_ArrayElemRef]; }
  XMLCh*      elem_ArrayElemRef_x() const { return x_strTbl[ELEM_ArrayElemRef]; }
  
  const char* elem_IndexTriplet() const { return c_strTbl[ELEM_IndexTriplet]; }
  XMLCh*      elem_IndexTriplet_x() const { return x_strTbl[ELEM_IndexTriplet]; }
  const char* elem_Index() const { return c_strTbl[ELEM_Index]; }
  XMLCh*      elem_Index_x() const { return x_strTbl[ELEM_Index]; }
  const char* elem_Bound() const { return c_strTbl[ELEM_Bound]; }
  XMLCh*      elem_Bound_x() const { return x_strTbl[ELEM_Bound]; }
  const char* elem_Stride() const { return c_strTbl[ELEM_Stride]; }
  XMLCh*      elem_Stride_x() const { return x_strTbl[ELEM_Stride]; }
  
  const char* elem_VarRefEdge() const { return c_strTbl[ELEM_VarRefEdge]; }
  XMLCh*      elem_VarRefEdge_x() const { return x_strTbl[ELEM_VarRefEdge]; }
  
  // DerivativePropagator
  XMLCh*      elem_DerivProp_x() const { return x_strTbl[ELEM_DerivProp]; }
  
  XMLCh*      elem_SetDeriv_x() const { return x_strTbl[ELEM_SetDeriv]; }
  XMLCh*      elem_SetNegDeriv_x() const { return x_strTbl[ELEM_SetNegDeriv]; }
  XMLCh*      elem_IncDeriv_x() const { return x_strTbl[ELEM_IncDeriv]; }
  XMLCh*      elem_DecDeriv_x() const { return x_strTbl[ELEM_DecDeriv]; }
  XMLCh*      elem_ZeroDeriv_x() const { return x_strTbl[ELEM_ZeroDeriv]; }
  XMLCh*      elem_Sax_x() const { return x_strTbl[ELEM_Sax]; }
  XMLCh*      elem_Saxpy_x() const { return x_strTbl[ELEM_Saxpy]; }

  XMLCh*      elem_Tgt_x() const { return x_strTbl[ELEM_Tgt]; }
  XMLCh*      elem_Src_x() const { return x_strTbl[ELEM_Src]; }
  XMLCh*      elem_AX_x() const { return x_strTbl[ELEM_AX]; }
  XMLCh*      elem_A_x() const { return x_strTbl[ELEM_A]; }
  XMLCh*      elem_X_x() const { return x_strTbl[ELEM_X]; }
  XMLCh*      elem_Y_x() const { return x_strTbl[ELEM_Y]; }
  
  // -----------------------------------------------------
  
  // Special tags
  const char* tag_SymTabId() const   { return c_strTbl[TAG_SymTabId]; }
  XMLCh*      tag_SymTabId_x() const { return x_strTbl[TAG_SymTabId]; }
  
  const char* tag_SymId() const   { return c_strTbl[TAG_SymId]; }
  XMLCh*      tag_SymId_x() const { return x_strTbl[TAG_SymId]; }
  
  const char* tag_PUId() const   { return c_strTbl[TAG_PUId]; }
  XMLCh*      tag_PUId_x() const { return x_strTbl[TAG_PUId]; }
  
  const char* tag_WHIRLId() const   { return c_strTbl[TAG_WHIRLId]; }
  XMLCh*      tag_WHIRLId_x() const { return x_strTbl[TAG_WHIRLId]; }
  
  const char* tag_StmtGoto() const   { return c_strTbl[TAG_StmtGoto]; }
  XMLCh*      tag_StmtGoto_x() const { return x_strTbl[TAG_StmtGoto]; }
  
  const char* tag_StmtLabel() const   { return c_strTbl[TAG_StmtLabel]; }
  XMLCh*      tag_StmtLabel_x() const { return x_strTbl[TAG_StmtLabel]; }
  
  const char* tag_StmtReturn() const   { return c_strTbl[TAG_StmtReturn]; }
  XMLCh*      tag_StmtReturn_x() const { return x_strTbl[TAG_StmtReturn]; }
  
  const char* tag_IntrinsicKey() const   { return c_strTbl[TAG_IntrinsicKey]; }
  XMLCh*      tag_IntrinsicKey_x() const { return x_strTbl[TAG_IntrinsicKey]; }

  const char* tag_PregId() const   { return c_strTbl[TAG_PregId]; }
  XMLCh*      tag_PregId_x() const { return x_strTbl[TAG_PregId]; }
  
  const char* tag_End() const { return c_strTbl[TAG_End]; }
  XMLCh*      tag_End_x() const { return x_strTbl[TAG_End]; }
  
private:
  static const char* c_strTbl[];
  XMLCh** x_strTbl;
};

// A global instance providing everyone access to the above info
extern XAIFStringRepository XAIFStrings;

#endif // XAIFStrings_INCLUDED_h
