// -*-Mode: C++;-*-
// $Header: /Volumes/cvsrep/developer/OpenADFortTk/src/sexp2whirl/sexp2wn.cxx,v 1.10 2006/05/12 16:12:22 utke Exp $

#include <stdlib.h>

#include <sexp.h>

#include "Open64IRInterface/Open64BasicTypes.h"

#include "Diagnostics.h"
#include "sexp2wn.i"
#include "sexp2wn.h"

#include "SexpTags.h"
#include "sexputil.h"

using namespace sexp2whirl;


//***************************************************************************
// Translate any S-expression AST node
//***************************************************************************

// TranslateWN: see header. 
WN* 
sexp2whirl::TranslateWN(sexp_t* sx)
{   
  using namespace sexp;

  // The task of translation is dispatched to the appropriate handler
  // registered in the handler table.
  static WNXlationTable handlerTable;
  
  if (!sx || is_null_list(sx)) { 
    return NULL; 
  }
  
  OPERATOR opr = GetWhirlOpr(sx);
  FORTTK_DEVMSG(3, "Translating " << OPERATOR_name(opr));
  
  // Dispatch to the appropriate handler for this construct.
  return handlerTable[opr](sx);
}


// TranslateWNChildren: see header.
std::vector<WN*>
sexp2whirl::TranslateWNChildren(sexp_t* sx)
{   
  using namespace sexp;
  
  // Sanity check
  FORTTK_ASSERT(is_list(sx), fortTkSupport::Diagnostics::UnexpectedInput);  

  // Translate all children
  std::vector<WN*> vecWN;
  
  INT nkids = 0;
  for (sexp_t* kid_sx = get_wnast_kid0(sx); kid_sx; 
       kid_sx = get_next(kid_sx)) {
    WN* kidwn = TranslateWN(kid_sx);
    vecWN.push_back(kidwn);
  }
  
  return vecWN;
}


//***************************************************************************
// Commonly used routines for extracting information from S-expressions
//***************************************************************************

// GetWhirlSym
ST_IDX 
sexp2whirl::GetWhirlSym(sexp_t* sx)
{
  using namespace sexp;
  
  // Sanity check
  FORTTK_ASSERT(is_list(sx), fortTkSupport::Diagnostics::UnexpectedInput);  
  
  sexp_t* tag_sx = get_elem0(sx);
  const char* tagstr = get_value(tag_sx);
  FORTTK_ASSERT(tag_sx && strcmp(tagstr, SexpTags::ST) == 0,
		fortTkSupport::Diagnostics::UnexpectedInput);
  
  // Convert st_idx
  sexp_t* lvl_st = get_elem1(sx);
  sexp_t* idx_st = get_elem2(sx);
  SYMTAB_IDX lvl = (SYMTAB_IDX)get_value_ui32(lvl_st);
  UINT32 idx = get_value_ui32(idx_st);
  ST_IDX st_idx = make_ST_IDX(idx, lvl);  
  return st_idx;
}


// GetWhirlTy
TY_IDX
sexp2whirl::GetWhirlTy(sexp_t* sx)
{
  using namespace sexp;
  
  // Sanity check
  FORTTK_ASSERT(is_list(sx), fortTkSupport::Diagnostics::UnexpectedInput);  
  
  sexp_t* tag_sx = get_elem0(sx);
  const char* tagstr = get_value(tag_sx);
  FORTTK_ASSERT(tag_sx && strcmp(tagstr, SexpTags::TY) == 0,
		fortTkSupport::Diagnostics::UnexpectedInput);
  
  // Convert ty_idx
  // - ignore name
  sexp_t* idx_sx = get_elem2(sx);
  sexp_t* algn_sx = get_elem3(sx);
  UINT32 idx = get_value_ui32(idx_sx);
  UINT32 algn = get_value_ui32(algn_sx);
  
  TY_IDX ty_idx = make_TY_IDX(idx);
  Set_TY_align(ty_idx, algn);
  
  return ty_idx;
}


// GetWhirlOpr
OPERATOR
sexp2whirl::GetWhirlOpr(sexp_t* sx)
{
  // FIXME: this could benefit from a one-element cache
  using namespace sexp;

  static const int SZ = 28;
  static char OPRNM_full[SZ+4] = "OPR_"; // prepend skipped "OPR_"
  static char* OPRNM = OPRNM_full+4;
  
  // Sanity check
  FORTTK_ASSERT(is_list(sx), fortTkSupport::Diagnostics::UnexpectedInput);  
  
  // Convert operator
  sexp_t* opr_sx = get_elem0(sx);
  const char* opr_nm = get_value(opr_sx);
  strncpy(OPRNM, opr_nm, SZ); OPRNM[SZ-1] = '\0';
  OPERATOR opr = Name_To_OPERATOR(OPRNM_full);
  
  return opr;
}


// GetWhirlOpcode
OPCODE
sexp2whirl::GetWhirlOpc(sexp_t* sx)
{
  using namespace sexp;
  
  OPERATOR opr = GetWhirlOpr(sx);

  sexp_t* rty_sx = get_elem1(sx);
  const char* rty_nm = get_value(rty_sx);
  TYPE_ID rty = Name_To_Mtype(rty_nm);
  
  sexp_t* dty_sx = get_elem2(sx);
  const char* dty_nm = get_value(dty_sx);
  TYPE_ID dty = Name_To_Mtype(dty_nm);
  
  OPCODE opc = OPCODE_make_op(opr, rty, dty);
  return opc;
}


// GetWhirlSymRef
ST_IDX
sexp2whirl::GetWhirlSymRef(sexp_t* sx)
{
  using namespace sexp;

  // Sanity check
  FORTTK_ASSERT(is_list(sx), fortTkSupport::Diagnostics::UnexpectedInput);  
  
  sexp_t* tag_sx = get_elem0(sx);
  const char* tagstr = get_value(tag_sx);
  FORTTK_ASSERT(tag_sx && strcmp(tagstr, SexpTags::ST) == 0,
		fortTkSupport::Diagnostics::UnexpectedInput);

  // Convert st_idx
  // - ignore name
  sexp_t* lvl_sx = get_elem2(sx);
  sexp_t* idx_sx = get_elem3(sx);
  SYMTAB_IDX lvl = (SYMTAB_IDX)get_value_ui32(lvl_sx);
  UINT32 idx = get_value_ui32(idx_sx);
  ST_IDX st_idx = make_ST_IDX(idx, lvl);  
  return st_idx;
}


// GetWhirlTyUse
TY_IDX
sexp2whirl::GetWhirlTyUse(sexp_t* sx)
{
  using namespace sexp;

  // Sanity check
  FORTTK_ASSERT(is_list(sx), fortTkSupport::Diagnostics::UnexpectedInput);  
  
  sexp_t* tag_sx = get_elem0(sx);
  const char* tagstr = get_value(tag_sx);
  FORTTK_ASSERT(tag_sx && strcmp(tagstr, SexpTags::TY) == 0,
		fortTkSupport::Diagnostics::UnexpectedInput);

  // Convert ty_idx
  // - ignore name
  sexp_t* idx_sx = get_elem2(sx);
  sexp_t* algn_sx = get_elem3(sx);
  UINT32 idx = get_value_ui32(idx_sx);
  UINT32 algn = get_value_ui32(algn_sx);
  
  TY_IDX ty_idx = make_TY_IDX(idx);
  Set_TY_align(ty_idx, algn);
  
  return ty_idx;
}


// GetWhirlFlg
const char*
sexp2whirl::GetWhirlFlg(sexp_t* sx)
{
  using namespace sexp;
  
  // Sanity check
  FORTTK_ASSERT(is_list(sx), fortTkSupport::Diagnostics::UnexpectedInput);
  
  sexp_t* tag_sx = get_elem0(sx);
  const char* tagstr = get_value(tag_sx);
  FORTTK_ASSERT(tag_sx && strcmp(tagstr, SexpTags::FLG) == 0,
		fortTkSupport::Diagnostics::UnexpectedInput);
  
  // Get the flag string
  sexp_t* flg_sx = get_elem1(sx);
  FORTTK_ASSERT(flg_sx, fortTkSupport::Diagnostics::UnexpectedInput);
  
  const char* flg = get_value(flg_sx);
  return flg;
}


// GetWhirlOpaqueFlg
UINT64
sexp2whirl::GetWhirlOpaqueFlg(sexp_t* sx)
{
  using namespace sexp;
  
  // Sanity check
  FORTTK_ASSERT(is_list(sx), fortTkSupport::Diagnostics::UnexpectedInput);
  
  sexp_t* tag_sx = get_elem0(sx);
  const char* tagstr = get_value(tag_sx);
  FORTTK_ASSERT(tag_sx && strcmp(tagstr, SexpTags::OFLG) == 0,
		fortTkSupport::Diagnostics::UnexpectedInput);
  
  // Get the flag string
  sexp_t* flg_sx = get_elem1(sx);
  FORTTK_ASSERT(flg_sx, fortTkSupport::Diagnostics::UnexpectedInput);
  
  UINT64 flg = get_value_ui64(flg_sx);
  return flg;
}


//***************************************************************************
// Structured Control Flow Statements: translation of these is
//   superceded by construction of the control flow graph.
//***************************************************************************

WN*
sexp2whirl::xlate_FUNC_ENTRY(sexp_t* sx)
{
  using namespace sexp;
  
  OPCODE opc = GetWhirlOpc(sx); // WN_OPR

  FORTTK_ASSERT(OPCODE_operator(opc) == OPR_FUNC_ENTRY, 
		fortTkSupport::Diagnostics::UnexpectedInput);
  
  sexp_t* attrs_sx = get_wnast_attrs(sx); // WN_ATTRS
  sexp_t* st_idx_sx = get_elem0(attrs_sx);
  ST_IDX st_idx = GetWhirlSymRef(st_idx_sx);
  
  std::vector<WN*> kids = TranslateWNChildren(sx); // KIDs

  INT16 nkids = (INT16)kids.size();
  WN* pragmasWN = kids[nkids-3];
  WN* varrefsWN = kids[nkids-2];
  WN* bodyWN    = kids[nkids-1];

  WN* wn = WN_CreateEntry(nkids - 3, st_idx, bodyWN, pragmasWN, varrefsWN);
  for (INT16 i = 0; i < (nkids - 3); ++i) {
    WN_kid(wn,i) = kids[i];
  }
  
  return wn;
}


WN* 
sexp2whirl::xlate_BLOCK(sexp_t* sx)
{
  using namespace sexp;
  
  OPCODE opc = GetWhirlOpc(sx); // WN_OPR

  FORTTK_ASSERT(OPCODE_operator(opc) == OPR_BLOCK, fortTkSupport::Diagnostics::UnexpectedInput);
  
  sexp_t* attrs_sx = get_wnast_attrs(sx); // WN_ATTRS

  WN* blkWN = WN_CreateBlock();
  for (sexp_t* kid_sx = get_wnast_kid0(sx); kid_sx; // KIDs
       kid_sx = get_next(kid_sx)) {
    WN* wn = TranslateWN(kid_sx);
    WN_INSERT_BlockLast(blkWN, wn);
  }

  return blkWN;
}


WN* 
sexp2whirl::xlate_REGION(sexp_t* sx)
{
  FORTTK_DIE(fortTkSupport::Diagnostics::Unimplemented);
  return NULL;
}


WN* 
sexp2whirl::xlate_structured_cf(sexp_t* sx)
{
  using namespace sexp;
  
  OPCODE opc = GetWhirlOpc(sx); // WN_OPR

  OPERATOR opr = OPCODE_operator(opc);
  FORTTK_ASSERT(opr == OPR_DO_LOOP || opr == OPR_DO_WHILE || 
		opr == OPR_WHILE_DO || opr == OPR_IF, 
		fortTkSupport::Diagnostics::UnexpectedInput); 
  
  std::vector<WN*> kids = TranslateWNChildren(sx); // KIDs
  WN* wn = WN_Create(opc, kids.size());
  for (INT i = 0; i < kids.size(); ++i) {
    WN_kid(wn,i) = kids[i];
  }
  
  sexp_t* attrs_sx = get_wnast_attrs(sx); // WN_ATTRS
  if (opr == OPR_IF) {
    sexp_t* flags_sx = get_elem0(attrs_sx); 
    UINT32 flg = (UINT32)GetWhirlOpaqueFlg(flags_sx);
    WN_if_flag(wn) = flg;
  }
  
  return wn;
}


//***************************************************************************
// Unstructured Control Flow Statements
//***************************************************************************

WN*
sexp2whirl::xlate_IMPLIED_DO(sexp_t* sx)
{
  FORTTK_DIE(fortTkSupport::Diagnostics::Unimplemented);
  return NULL;
}


WN* 
sexp2whirl::xlate_GOTOx_LABEL(sexp_t* sx)
{  
  using namespace sexp;
  
  OPCODE opc = GetWhirlOpc(sx); // WN_OPR
  
  OPERATOR opr = OPCODE_operator(opc);
  FORTTK_ASSERT(opr == OPR_GOTO || opr == OPR_GOTO_OUTER_BLOCK ||
		opr == OPR_REGION_EXIT || opr == OPR_LABEL, 
		fortTkSupport::Diagnostics::UnexpectedInput); 
  
  sexp_t* attrs_sx = get_wnast_attrs(sx); // WN_ATTRS
  sexp_t* lbl_sx = get_elem0(attrs_sx); 
  INT32 lbl = get_value_i32(lbl_sx);

  WN* wn = NULL;
  if (opr == OPR_GOTO) {
    wn = WN_CreateGoto(lbl);
  }
  else if (opr == OPR_GOTO_OUTER_BLOCK) {
    sexp_t* lbl_lvl_sx = get_elem1(attrs_sx); 
    UINT32 lbl_lvl = get_value_ui32(lbl_lvl_sx);
    wn = WN_CreateGotoOuterBlock(lbl, lbl_lvl);
  }
  else if (opr == OPR_REGION_EXIT) {  
    wn = WN_CreateRegionExit(lbl);
  }
  else if (opr == OPR_LABEL) {
    sexp_t* flags_sx = get_elem1(attrs_sx); 
    UINT32 flg = (UINT32)GetWhirlOpaqueFlg(flags_sx);
    
    WN* kidWN = TranslateWN(get_wnast_kid0(sx)); // KID 0

    wn = WN_CreateLabel(lbl, flg, kidWN);
  }
  
  return wn;
}


WN*
sexp2whirl::xlate_multiBR(sexp_t* sx)
{
  using namespace sexp;
  
  OPCODE opc = GetWhirlOpc(sx); // WN_OPR
  
  OPERATOR opr = OPCODE_operator(opc);
  FORTTK_ASSERT(opr == OPR_SWITCH || opr == OPR_COMPGOTO, 
		fortTkSupport::Diagnostics::UnexpectedInput); 
  
  sexp_t* attrs_sx = get_wnast_attrs(sx); // WN_ATTRS
  sexp_t* nentries_sx = get_elem0(attrs_sx); 
  INT32 nentries = get_value_i32(nentries_sx);

  sexp_t* llbl_sx = get_elem1(attrs_sx); 
  INT32 llbl = get_value_i32(llbl_sx);

  std::vector<WN*> kids = TranslateWNChildren(sx); // KIDs
  WN* wn = WN_Create(opc, kids.size());
  for (INT i = 0; i < kids.size(); ++i) {
    WN_kid(wn,i) = kids[i];
  }
  WN_num_entries(wn) = nentries;
  WN_last_label(wn) = llbl;
  
  return wn;
}


WN*
sexp2whirl::xlate_CASEGOTO(sexp_t* sx)
{
  using namespace sexp;
  
  OPCODE opc = GetWhirlOpc(sx); // WN_OPR

  FORTTK_ASSERT(OPCODE_operator(opc) == OPR_CASEGOTO, fortTkSupport::Diagnostics::UnexpectedInput);
  
  sexp_t* attrs_sx = get_wnast_attrs(sx); // WN_ATTRS
  sexp_t* cval_sx = get_elem0(attrs_sx); 
  INT64 cval = get_value_i64(cval_sx);

  sexp_t* lbl_sx = get_elem1(attrs_sx); 
  INT32 lbl = get_value_i32(lbl_sx);
  
  WN* wn = WN_CreateCasegoto(cval, lbl);
  return wn;
}


WN* 
sexp2whirl::xlate_AGOTO(sexp_t* sx)
{
  FORTTK_DIE(fortTkSupport::Diagnostics::Unimplemented);
  return NULL;
}


WN*
sexp2whirl::xlate_ALTENTRY(sexp_t* sx)
{
  FORTTK_DIE(fortTkSupport::Diagnostics::Unimplemented);  
  return NULL;
}


WN* 
sexp2whirl::xlate_condBR(sexp_t* sx)
{
  using namespace sexp;
  
  OPCODE opc = GetWhirlOpc(sx); // WN_OPR

  OPERATOR opr = OPCODE_operator(opc);
  FORTTK_ASSERT(opr == OPR_TRUEBR || opr == OPR_FALSEBR,
		fortTkSupport::Diagnostics::UnexpectedInput);

  sexp_t* attrs_sx = get_wnast_attrs(sx); // WN_ATTRS
  sexp_t* lbl_sx = get_elem0(attrs_sx); 
  INT32 lbl = get_value_i32(lbl_sx);
  
  WN* kidWN = TranslateWN(get_wnast_kid0(sx)); // KID 0

  WN* wn = WN_Create(opc, 1);
  WN_label_number(wn) = lbl;
  WN_kid0(wn) = kidWN;
  return wn;
}


WN* 
sexp2whirl::xlate_RETURNx(sexp_t* sx)
{
  using namespace sexp;
  
  OPCODE opc = GetWhirlOpc(sx); // WN_OPR
  
  OPERATOR opr = OPCODE_operator(opc);
  FORTTK_ASSERT(opr == OPR_RETURN || opr == OPR_RETURN_VAL,
		fortTkSupport::Diagnostics::UnexpectedInput); 
  
  sexp_t* attrs_sx = get_wnast_attrs(sx); // WN_ATTRS
  
  WN* wn = NULL;
  if (opr == OPR_RETURN_VAL) {
    WN* kidWN = TranslateWN(get_wnast_kid0(sx)); // KID 0
    wn = WN_CreateReturn_Val(opc, kidWN);
  }
  else {
    wn = WN_CreateReturn();
  }

  return wn;
}


//***************************************************************************
// Calls
//***************************************************************************

WN* 
sexp2whirl::xlate_xCALL(sexp_t* sx)
{
  using namespace sexp;
  
  OPCODE opc = GetWhirlOpc(sx); // WN_OPR
  
  OPERATOR opr = OPCODE_operator(opc);
  FORTTK_ASSERT(opr == OPR_CALL || opr == OPR_ICALL || 
		opr == OPR_VFCALL || opr == OPR_PICCALL ||
		opr == OPR_INTRINSIC_CALL || opr == OPR_INTRINSIC_OP, 
		fortTkSupport::Diagnostics::UnexpectedInput); 
  
  std::vector<WN*> kids = TranslateWNChildren(sx); // KIDs
  WN* wn = WN_Create(opc, kids.size());
  for (INT i = 0; i < kids.size(); ++i) {
    WN_kid(wn,i) = kids[i];
  }
  
  sexp_t* attrs_sx = get_wnast_attrs(sx); // WN_ATTRS
  if (opr == OPR_CALL || opr == OPR_PICCALL) {
    sexp_t* st_idx_sx = get_elem0(attrs_sx); 
    ST_IDX st_idx = GetWhirlSymRef(st_idx_sx);
    WN_st_idx(wn) = st_idx;
  } 
  else if (opr == OPR_ICALL || opr == OPR_VFCALL) {
    sexp_t* ty_idx_sx = get_elem0(attrs_sx); 
    TY_IDX ty_idx = GetWhirlTyUse(ty_idx_sx);
    WN_set_ty(wn, ty_idx);
  } 
  else {
    sexp_t* intrn_sx = get_elem0(attrs_sx);
    const char* intrn_nm = get_value(intrn_sx);
    INTRINSIC intrn = Name_To_INTRINSIC(intrn_nm);
    WN_intrinsic(wn) = intrn;
  }
  if (opr != OPR_VFCALL) {
    sexp_t* flags_sx = get_elem1(attrs_sx); 
    UINT32 flg = (UINT32)GetWhirlOpaqueFlg(flags_sx);
    WN_call_flag(wn) = flg;
  }

  return wn;
}


WN* 
sexp2whirl::xlate_IO(sexp_t* sx)
{
  using namespace sexp;
  
  OPCODE opc = GetWhirlOpc(sx); // WN_OPR

  FORTTK_ASSERT(OPCODE_operator(opc) == OPR_IO, fortTkSupport::Diagnostics::UnexpectedInput);
  
  sexp_t* attrs_sx = get_wnast_attrs(sx); // WN_ATTRS
  sexp_t* ios_sx = get_elem0(attrs_sx); 
  const char* ios_nm = get_value(ios_sx);
  IOSTATEMENT ios = Name_To_IOSTATEMENT(ios_nm);

  sexp_t* flags_sx = get_elem1(attrs_sx);
  UINT32 flg = (UINT32)GetWhirlOpaqueFlg(flags_sx);

  std::vector<WN*> kids = TranslateWNChildren(sx); // KIDs
  WN* wn = WN_CreateIo(ios, kids.size());
  for (INT i = 0; i < kids.size(); ++i) {
    WN_kid(wn,i) = kids[i];
  }
  WN_io_flag(wn) = flg;

  return wn;
}


WN* 
sexp2whirl::xlate_IO_ITEM(sexp_t* sx)
{
  using namespace sexp;
  
  OPCODE opc = GetWhirlOpc(sx); // WN_OPR

  FORTTK_ASSERT(OPCODE_operator(opc) == OPR_IO_ITEM, fortTkSupport::Diagnostics::UnexpectedInput);
  
  sexp_t* attrs_sx = get_wnast_attrs(sx); // WN_ATTRS
  sexp_t* ioi_sx = get_elem0(attrs_sx); 
  const char* ioi_nm = get_value(ioi_sx);
  IOITEM ioi = Name_To_IOITEM(ioi_nm);
  
  sexp_t* ty_idx_sx = get_elem1(attrs_sx);
  TY_IDX ty_idx = GetWhirlTyUse(ty_idx_sx);
  
  std::vector<WN*> kids = TranslateWNChildren(sx); // KIDs
  WN* wn = WN_CreateIoItemN(ioi, kids.size(), ty_idx);
  for (INT i = 0; i < kids.size(); ++i) {
    WN_kid(wn,i) = kids[i];
  }

  return wn;
}

WN*
sexp2whirl::xlate_INTERFACE(sexp_t* sx) {
  using namespace sexp;
  OPCODE opc = GetWhirlOpc(sx); // WN_OPR
  OPERATOR opr = OPCODE_operator(opc);
  FORTTK_ASSERT(opr == OPR_INTERFACE, fortTkSupport::Diagnostics::UnexpectedInput);
  // FUNC_ENTRY nodes under interface look different!
  std::vector<WN*> interfaceKids;
  for (sexp_t* kid_sx = get_wnast_kid0(sx); 
       kid_sx; 
       kid_sx = get_next(kid_sx)) {
    OPERATOR kid_opr = GetWhirlOpr(kid_sx);
    WN* kidwn=0; 
    // see if it is a FUNC_ENTRY: 
    if (kid_opr == OPR_FUNC_ENTRY) { 
      std::vector<WN*> formalArguments = TranslateWNChildren(kid_sx);
      // make the FUNC_ENTRY: 
      kidwn = WN_Create (OPC_FUNC_ENTRY, formalArguments.size());
      sexp_t* attrs_sx = get_wnast_attrs(kid_sx); // WN_ATTRS
      sexp_t* st_idx_sx = get_elem0(attrs_sx);
      ST_IDX st_idx = GetWhirlSymRef(st_idx_sx);
      WN_entry_name(kidwn) = st_idx;
      for (INT i = 0; i < formalArguments.size(); ++i) {
	WN_kid(kidwn,i) = formalArguments[i];
      }
    } 
    else { 
      kidwn = TranslateWN(kid_sx);
    }
    interfaceKids.push_back(kidwn);
  }
  WN* wn = WN_Create(opc, interfaceKids.size());
  for (INT i = 0; i < interfaceKids.size(); ++i) {
    WN_kid(wn,i) = interfaceKids[i];
  }
  sexp_t* attrs_sx = get_wnast_attrs(sx); // WN_ATTRS
  sexp_t* cur_sx = get_elem0(attrs_sx);
  ST_IDX st_idx = GetWhirlSymRef(cur_sx);
  WN_st_idx(wn) = st_idx;
  return wn;
}

//***************************************************************************
// Other Statements
//***************************************************************************

WN*
sexp2whirl::xlate_misc_stmt(sexp_t* sx)
{
  using namespace sexp;
  
  OPCODE opc = GetWhirlOpc(sx); // WN_OPR
  
  OPERATOR opr = OPCODE_operator(opc);
  FORTTK_ASSERT(opr == OPR_EVAL ||
		opr == OPR_PREFETCH || opr == OPR_PREFETCHX ||
		opr == OPR_COMMENT || 
		opr == OPR_TRAP || opr == OPR_ASSERT || opr == OPR_AFFIRM ||
		opr == OPR_FORWARD_BARRIER || opr == OPR_BACKWARD_BARRIER ||
		opr == OPR_DEALLOCA ||
		opr == OPR_USE || opr == OPR_NAMELIST || 
		opr == OPR_IMPLICIT_BND || opr == OPR_NULLIFY || 
		opr == OPR_INTERFACE || opr == OPR_ARRAY_CONSTRUCT,
		fortTkSupport::Diagnostics::UnexpectedInput);
  
  std::vector<WN*> kids = TranslateWNChildren(sx); // KIDs
  WN* wn = WN_Create(opc, kids.size());
  for (INT i = 0; i < kids.size(); ++i) {
    WN_kid(wn,i) = kids[i];
  }
  
  sexp_t* attrs_sx = get_wnast_attrs(sx); // WN_ATTRS
  sexp_t* cur_sx = get_elem0(attrs_sx);
  if (OPERATOR_has_sym(opr)) {
    ST_IDX st_idx = GetWhirlSymRef(cur_sx);
    WN_st_idx(wn) = st_idx;
    cur_sx = get_next(cur_sx);
  }
  if (OPERATOR_has_offset(opr)) {
    WN_OFFSET ofst = get_value_ui32(cur_sx);
    WN_offset(wn) = ofst;
    cur_sx = get_next(cur_sx);
  }
  if (OPERATOR_has_flags(opr)) {
    UINT32 flg = (UINT32)GetWhirlOpaqueFlg(cur_sx);
    WN_set_flag(wn, flg);
    cur_sx = get_next(cur_sx);
  }
  
  return wn;
}


WN*
sexp2whirl::xlate_xPRAGMA(sexp_t* sx)
{
  using namespace sexp;
  
  OPCODE opc = GetWhirlOpc(sx); // WN_OPR
  
  OPERATOR opr = OPCODE_operator(opc);
  FORTTK_ASSERT(opr == OPR_PRAGMA || opr == OPR_XPRAGMA, 
		fortTkSupport::Diagnostics::UnexpectedInput); 
  
  sexp_t* attrs_sx = get_wnast_attrs(sx); // WN_ATTRS
  
  sexp_t* prag_sx = get_elem0(attrs_sx);
  WN_PRAGMA_ID prag = (WN_PRAGMA_ID)get_value_ui32(prag_sx);
  
  sexp_t* st_idx_sx = get_elem1(attrs_sx);
  ST_IDX st_idx = GetWhirlSymRef(st_idx_sx);
  
  sexp_t* flags_sx = get_elem2(attrs_sx);
  UINT16 flg = (UINT16)GetWhirlOpaqueFlg(flags_sx);

  WN* wn = NULL;
  if (opr == OPR_PRAGMA) {
    sexp_t* cval_sx = get_elem3(attrs_sx); // end WN_ATTRS
    INT64 cval = get_value_i64(cval_sx);
    
    wn = WN_CreatePragma(prag, st_idx, 0, 0);
    WN_const_val(wn) = cval;
  } 
  else {
    WN* kidWN = TranslateWN(get_wnast_kid0(sx)); // KID 0
    wn = WN_CreateXpragma(prag, st_idx, 1 /*kid_count*/);
    WN_kid0(wn) = kidWN;
  }
  
  return wn;
}


//***************************************************************************
// Loads (In WHIRL, loads are expressions.)
// Stores (In WHIRL, stores are statements.)
//***************************************************************************

WN* 
sexp2whirl::xlate_LDA_LDMA(sexp_t* sx)
{
  using namespace sexp;
  
  OPCODE opc = GetWhirlOpc(sx); // WN_OPR
  
  OPERATOR opr = OPCODE_operator(opc);
  FORTTK_ASSERT(opr == OPR_LDA || opr == OPR_LDMA, fortTkSupport::Diagnostics::UnexpectedInput);

  sexp_t* attrs_sx = get_wnast_attrs(sx); // WN_ATTRS
  sexp_t* st_idx_sx = get_elem0(attrs_sx);
  ST_IDX st_idx = GetWhirlSymRef(st_idx_sx);
  
  sexp_t* ofst_sx = get_elem1(attrs_sx);
  WN_OFFSET ofst = get_value_ui32(ofst_sx);

  sexp_t* ty_idx_sx = get_elem2(attrs_sx);
  TY_IDX ty_idx = GetWhirlTyUse(ty_idx_sx);

  sexp_t* fldid_sx = get_elem3(attrs_sx);
  UINT fldid = get_value_ui32(ofst_sx);

  WN* wn = NULL;
  if (opr == OPR_LDA) {
    wn = WN_CreateLda(opc, ofst, ty_idx, st_idx, fldid);
  }
  else {
    wn = WN_CreateLdma(opc, ofst, ty_idx, st_idx, fldid);
  }
  
  return wn;
}


WN* 
sexp2whirl::xlate_LDID_STID(sexp_t* sx)
{
  using namespace sexp;
  
  OPCODE opc = GetWhirlOpc(sx); // WN_OPR

  OPERATOR opr = OPCODE_operator(opc);
  FORTTK_ASSERT(opr == OPR_LDID 
		|| 
		opr == OPR_STID
		|| 
		opr == OPR_PSTID, 
		fortTkSupport::Diagnostics::UnexpectedInput);
  
  sexp_t* attrs_sx = get_wnast_attrs(sx); // WN_ATTRS
  sexp_t* st_idx_sx = get_elem0(attrs_sx);
  ST_IDX st_idx = GetWhirlSymRef(st_idx_sx);
  
  sexp_t* ofst_sx = get_elem1(attrs_sx);
  WN_OFFSET ofst = get_value_ui32(ofst_sx);
  
  sexp_t* ty_idx_sx = get_elem2(attrs_sx);
  TY_IDX ty_idx = GetWhirlTyUse(ty_idx_sx);
 
  sexp_t* fldid_sx = get_elem3(attrs_sx);
  UINT fldid = get_value_ui32(fldid_sx);
  
  WN* wn = NULL;
  if (opr == OPR_STID) {
    WN* kidWN = TranslateWN(get_wnast_kid0(sx)); // KID 0
    wn = WN_CreateStid(opc, ofst, st_idx, ty_idx, kidWN, fldid);
  }
  else if (opr == OPR_PSTID) { 
    WN* kidWN = TranslateWN(get_wnast_kid0(sx)); // KID 0
    wn = WN_CreatePStid(opc, ofst, st_idx, ty_idx, kidWN, fldid);
  }
  else {
    wn = WN_CreateLdid(opc, ofst, st_idx, ty_idx, fldid);
  }
  
  return wn;
} 


WN* 
sexp2whirl::xlate_IDNAME(sexp_t* sx)
{
  using namespace sexp;
  
  OPCODE opc = GetWhirlOpc(sx); // WN_OPR

  FORTTK_ASSERT(OPCODE_operator(opc) == OPR_IDNAME, fortTkSupport::Diagnostics::UnexpectedInput);
  
  sexp_t* attrs_sx = get_wnast_attrs(sx); // WN_ATTRS
  sexp_t* st_idx_sx = get_elem0(attrs_sx);
  ST_IDX st_idx = GetWhirlSymRef(st_idx_sx);
  
  sexp_t* ofst_sx = get_elem1(attrs_sx);
  WN_OFFSET ofst = get_value_ui32(ofst_sx);
  
  WN* wn = WN_CreateIdname(ofst, st_idx);
  return wn;
}


WN* 
sexp2whirl::xlate_xLOADx_xSTOREx(sexp_t* sx)
{
  using namespace sexp;
  
  OPCODE opc = GetWhirlOpc(sx); // WN_OPR

  OPERATOR opr = OPCODE_operator(opc);
  FORTTK_ASSERT(opr == OPR_ILOAD || opr == OPR_MLOAD || opr == OPR_ILOADX ||
		opr == OPR_ISTORE || opr == OPR_MSTORE || opr == OPR_ISTOREX ||
		opr == OPR_PSTORE ,
		fortTkSupport::Diagnostics::UnexpectedInput);

  std::vector<WN*> kids = TranslateWNChildren(sx); // KIDs
  WN* wn = WN_Create(opc, kids.size());
  for (INT i = 0; i < kids.size(); ++i) {
    WN_kid(wn,i) = kids[i];
  }

  sexp_t* attrs_sx = get_wnast_attrs(sx); // WN_ATTRS
  sexp_t* cur_sx = get_elem0(attrs_sx);
  if (opr == OPR_ILOAD || opr == OPR_MLOAD || 
      opr == OPR_ISTORE || opr == OPR_MSTORE ||
      opr == OPR_PSTORE) {
    sexp_t* ofst_sx = cur_sx;
    WN_OFFSET ofst = get_value_ui32(ofst_sx);
    WN_offset(wn) = ofst;
    
    sexp_t* fldid_sx = get_next(ofst_sx);
    UINT fldid = get_value_ui32(fldid_sx);
    WN_set_field_id(wn, fldid);
    
    cur_sx = get_next(fldid_sx);
  }
  
  TY_IDX ty_idx1 = GetWhirlTyUse(cur_sx);
  WN_set_ty(wn, ty_idx1);
  cur_sx = get_next(cur_sx);
  
  if (opr == OPR_ILOAD || opr == OPR_ILOADX) {
    TY_IDX ty_idx2 = GetWhirlTyUse(cur_sx);
    WN_set_load_addr_ty(wn, ty_idx2);
  }
  
  return wn;
}

//***************************************************************************
// Array Operators (N-ary Operations)
//***************************************************************************

WN*
sexp2whirl::xlate_ARRAYx(sexp_t* sx)
{
  using namespace sexp;
  
  OPCODE opc = GetWhirlOpc(sx); // WN_OPR
  
  OPERATOR opr = OPCODE_operator(opc);
  FORTTK_ASSERT(opr == OPR_ARRAY || opr == OPR_ARRAYEXP || 
		opr == OPR_ARRSECTION, fortTkSupport::Diagnostics::UnexpectedInput);
  
  std::vector<WN*> kids = TranslateWNChildren(sx); // KIDs
  WN* wn = WN_Create(opc, kids.size());
  for (INT i = 0; i < kids.size(); ++i) {
    WN_kid(wn,i) = kids[i];
  }

  sexp_t* attrs_sx = get_wnast_attrs(sx); // WN_ATTRS
  if (opr == OPR_ARRAY || opr == OPR_ARRSECTION) {
    sexp_t* sz_sx = get_elem0(attrs_sx);
    WN_ESIZE sz = get_value_i64(sz_sx);
    WN_element_size(wn) = sz;
  }
  
  return wn;
}


//***************************************************************************
// Type Conversion
//***************************************************************************

WN* 
sexp2whirl::xlate_CVT_CVTL(sexp_t* sx)
{
  using namespace sexp;
  
  OPCODE opc = GetWhirlOpc(sx); // WN_OPR
  
  OPERATOR opr = OPCODE_operator(opc);
  FORTTK_ASSERT(opr == OPR_CVT || opr == OPR_CVTL, fortTkSupport::Diagnostics::UnexpectedInput);
  
  WN* wn = WN_Create(opc, 1);
  WN* kidWN = TranslateWN(get_wnast_kid0(sx)); // KID 0
  WN_kid0(wn) = kidWN;

  sexp_t* attrs_sx = get_wnast_attrs(sx); // WN_ATTRS
  if (opr == OPR_CVTL) {
    sexp_t* cvtlbits_sx = get_elem0(attrs_sx);
    INT16 cvtlbits = (INT16)get_value_i32(cvtlbits_sx);
    WN_cvtl_bits(wn) = cvtlbits;
  }
  
  return wn;
}


WN* 
sexp2whirl::xlate_TAS(sexp_t* sx)
{
  using namespace sexp;
  
  OPCODE opc = GetWhirlOpc(sx); // WN_OPR
  
  FORTTK_ASSERT(OPCODE_operator(opc) == OPR_BLOCK, fortTkSupport::Diagnostics::UnexpectedInput);
  
  sexp_t* attrs_sx = get_wnast_attrs(sx); // WN_ATTRS
  sexp_t* ty_idx_sx = get_elem0(attrs_sx);
  TY_IDX ty_idx = GetWhirlTyUse(ty_idx_sx);
  
  WN* kidWN = TranslateWN(get_wnast_kid0(sx)); // KID 0
  
  WN* wn = WN_Tas(OPCODE_rtype(opc), ty_idx, kidWN);
  return wn;
}


//***************************************************************************
// Leaf (Other)
//***************************************************************************

WN* 
sexp2whirl::xlate_INTCONST(sexp_t* sx)
{
  using namespace sexp;
  
  OPCODE opc = GetWhirlOpc(sx); // WN_OPR
  
  FORTTK_ASSERT(OPCODE_operator(opc) == OPR_INTCONST, 
		fortTkSupport::Diagnostics::UnexpectedInput); 
  
  sexp_t* attrs_sx = get_wnast_attrs(sx); // WN_ATTRS
  sexp_t* val_sx = get_elem0(attrs_sx);
  INT64 val = get_value_i64(val_sx);
  
  WN* wn = WN_CreateIntconst(opc, val);
  return wn;
}


WN* 
sexp2whirl::xlate_CONST(sexp_t* sx)
{
  using namespace sexp;
  
  OPCODE opc = GetWhirlOpc(sx); // WN_OPR
  
  FORTTK_ASSERT(OPCODE_operator(opc) == OPR_CONST, fortTkSupport::Diagnostics::UnexpectedInput); 
  
  sexp_t* attrs_sx = get_wnast_attrs(sx); // WN_ATTRS
  sexp_t* st_idx_sx = get_elem0(attrs_sx);
  ST_IDX st_idx = GetWhirlSymRef(st_idx_sx);

  WN* wn = WN_CreateConst(opc, st_idx);
  return wn;
}


//***************************************************************************
// Expression Operators: Unary Operations
//***************************************************************************

WN*
sexp2whirl::xlate_UnaryOp(sexp_t* sx)
{
  using namespace sexp;
  
  OPCODE opc = GetWhirlOpc(sx); // WN_OPR
  
  FORTTK_ASSERT(OPCODE_nkids(opc) == 1, 
		fortTkSupport::Diagnostics::UnexpectedInput << OPCODE_name(opc));
  
  sexp_t* attrs_sx = get_wnast_attrs(sx); // WN_ATTRS
  WN* kidWN = TranslateWN(get_wnast_kid0(sx)); // KID 0
  
  // Use WN_Create() instead of distinguishing between WN_Unary() and
  // WN_Trunc(), etc.
  WN* wn = WN_Create(opc, 1);
  WN_kid0(wn) = kidWN;
  return wn;
}

WN* 
sexp2whirl::xlate_STRCTFLD(sexp_t* sx)
{
  using namespace sexp;
  
  OPCODE opc = GetWhirlOpc(sx); // WN_OPR
  
  FORTTK_ASSERT(OPCODE_operator(opc) == OPR_STRCTFLD, fortTkSupport::Diagnostics::UnexpectedInput);

  std::vector<WN*> kids = TranslateWNChildren(sx); // KIDs
  WN* wn = WN_Create(opc, kids.size());
  for (INT i = 0; i < kids.size(); ++i) {
    WN_kid(wn,i) = kids[i];
  }

  sexp_t* attrs_sx = get_wnast_attrs(sx); // WN_ATTRS
  sexp_t* cur_sx = get_elem0(attrs_sx);

  TY_IDX ty_idx1 = GetWhirlTyUse(cur_sx);
  WN_set_load_addr_ty(wn, ty_idx1);
  cur_sx = get_next(cur_sx);

  TY_IDX ty_idx2 = GetWhirlTyUse(cur_sx);
  WN_set_ty(wn, ty_idx2);
  cur_sx = get_next(cur_sx);

  UINT fldid = get_value_ui32(cur_sx);
  WN_set_field_id(wn, fldid);
    
  return wn;
}

WN* 
sexp2whirl::xlate_PARM(sexp_t* sx)
{
  using namespace sexp;
  
  OPCODE opc = GetWhirlOpc(sx); // WN_OPR
  
  FORTTK_ASSERT(OPCODE_operator(opc) == OPR_PARM, fortTkSupport::Diagnostics::UnexpectedInput);
  
  sexp_t* attrs_sx = get_wnast_attrs(sx); // WN_ATTRS
  sexp_t* flags_sx = get_elem0(attrs_sx); 
  UINT32 flg = (UINT32)GetWhirlOpaqueFlg(flags_sx);

  sexp_t* ty_idx_sx = get_elem1(attrs_sx);
  TY_IDX ty_idx = GetWhirlTyUse(ty_idx_sx);
  
  WN* kidWN = TranslateWN(get_wnast_kid0(sx)); // KID 0
  
  WN* wn = WN_CreateParm(OPCODE_rtype(opc), kidWN, ty_idx, flg);
  return wn;
}


WN* 
sexp2whirl::xlate_ALLOCA(sexp_t* sx)
{
  FORTTK_DIE(fortTkSupport::Diagnostics::Unimplemented);
  return NULL;
}


//***************************************************************************
// Expression Operators: Binary Operations
//***************************************************************************

WN*
sexp2whirl::xlate_BinaryOp(sexp_t* sx)
{
  using namespace sexp;
  
  OPCODE opc = GetWhirlOpc(sx); // WN_OPR
  
  FORTTK_ASSERT(OPCODE_nkids(opc) == 2, 
		fortTkSupport::Diagnostics::UnexpectedInput << OPCODE_name(opc));
  
  sexp_t* attrs_sx = get_wnast_attrs(sx); // WN_ATTRS
  std::vector<WN*> kids = TranslateWNChildren(sx); // KIDs
  FORTTK_ASSERT(kids.size() == 2, fortTkSupport::Diagnostics::UnexpectedInput);

  // Use WN_Create() instead of distinguishing between WN_Binary() and
  // WN_Relational()
  WN* wn = WN_Create(opc, 2);
  WN_kid0(wn) = kids[0];
  WN_kid1(wn) = kids[1];
  
  return wn;
}


//***************************************************************************
// Expression Operators: Ternary Operations; N-ary Operations
//***************************************************************************

WN*
sexp2whirl::xlate_TernaryOp(sexp_t* sx)
{
  using namespace sexp;
  
  OPCODE opc = GetWhirlOpc(sx); // WN_OPR
  
  FORTTK_ASSERT(OPCODE_nkids(opc) == 3, 
		fortTkSupport::Diagnostics::UnexpectedInput << OPCODE_name(opc));
  
  sexp_t* attrs_sx = get_wnast_attrs(sx); // WN_ATTRS
  std::vector<WN*> kids = TranslateWNChildren(sx); // KIDs
  FORTTK_ASSERT(kids.size() == 3, fortTkSupport::Diagnostics::UnexpectedInput);

  WN* wn = WN_Create(opc, 3);
  WN_kid0(wn) = kids[0];
  WN_kid1(wn) = kids[1];
  WN_kid2(wn) = kids[2];

  return wn;
}


//***************************************************************************
// WNXlationTable
//***************************************************************************

WN*
sexp2whirl::xlate_unknown(sexp_t* sx)
{
  // Warn about opcodes we cannot translate, but keep translating.
  OPERATOR opr = GetWhirlOpr(sx);
  
  FORTTK_DEVMSG(0, fortTkSupport::Diagnostics::UnexpectedOpr << OPERATOR_name(opr));
  return NULL;
}


#define TABLE_SZ (OPERATOR_LAST + 1)
#define INIT_TABLE_SZ \
  (sizeof(WNXlationTable::initTable) / sizeof(WNXlationTable::InitEntry))


bool WNXlationTable::initialized = false;

WNXlationTable::Handler WNXlationTable::table[TABLE_SZ];
unsigned int WNXlationTable::tableSz = TABLE_SZ;

WNXlationTable::InitEntry WNXlationTable::initTable[] = {
  
  // Note: Organization generally corresponds to that in
  // Open64/documentation/whirl.tex
  
  // Structured control flow
  { OPR_FUNC_ENTRY,           &xlate_FUNC_ENTRY },
  { OPR_BLOCK,                &xlate_BLOCK },
  { OPR_REGION,               &xlate_REGION },
  { OPR_DO_LOOP,              &xlate_structured_cf },
  { OPR_DO_WHILE,             &xlate_structured_cf },
  { OPR_WHILE_DO,             &xlate_structured_cf },
  { OPR_IF,                   &xlate_structured_cf },
  
  // Other control flow
  { OPR_IMPLIED_DO,           &xlate_IMPLIED_DO },
  { OPR_GOTO,                 &xlate_GOTOx_LABEL },
  { OPR_GOTO_OUTER_BLOCK,     &xlate_GOTOx_LABEL },
  { OPR_SWITCH,               &xlate_multiBR },
  { OPR_CASEGOTO,             &xlate_CASEGOTO },
  { OPR_COMPGOTO,             &xlate_multiBR },
  { OPR_AGOTO,                &xlate_AGOTO },
  { OPR_REGION_EXIT,          &xlate_GOTOx_LABEL },
  { OPR_ALTENTRY,             &xlate_ALTENTRY },
  { OPR_TRUEBR,               &xlate_condBR },
  { OPR_FALSEBR,              &xlate_condBR },
  { OPR_RETURN,               &xlate_RETURNx },
  { OPR_RETURN_VAL,           &xlate_RETURNx },
  { OPR_LABEL,                &xlate_GOTOx_LABEL },

  // Statements: Calls
  { OPR_CALL,                 &xlate_xCALL },
  { OPR_ICALL,                &xlate_xCALL },
  { OPR_VFCALL,               &xlate_xCALL },
  { OPR_PICCALL,              &xlate_xCALL },
  { OPR_INTRINSIC_CALL,       &xlate_xCALL },
  { OPR_IO,                   &xlate_IO },

  // Statements: Other
  { OPR_EVAL,                 &xlate_misc_stmt },
  { OPR_PRAGMA,               &xlate_xPRAGMA },
  { OPR_XPRAGMA,              &xlate_xPRAGMA },
  { OPR_PREFETCH,             &xlate_misc_stmt },
  { OPR_PREFETCHX,            &xlate_misc_stmt },
  { OPR_COMMENT,              &xlate_misc_stmt },
  { OPR_TRAP,                 &xlate_misc_stmt },
  { OPR_ASSERT,               &xlate_misc_stmt },
  { OPR_AFFIRM,               &xlate_misc_stmt },
  { OPR_FORWARD_BARRIER,      &xlate_misc_stmt },
  { OPR_BACKWARD_BARRIER,     &xlate_misc_stmt },
  { OPR_DEALLOCA,             &xlate_misc_stmt },

  { OPR_USE,                  &xlate_misc_stmt },
  { OPR_NAMELIST,             &xlate_misc_stmt },
  { OPR_IMPLICIT_BND,         &xlate_misc_stmt },  
  { OPR_NULLIFY,              &xlate_misc_stmt },
  { OPR_INTERFACE,            &xlate_INTERFACE },
  { OPR_ARRAY_CONSTRUCT,      &xlate_misc_stmt },
  
  // Memory Access (or assignment and variable references)
  { OPR_LDA,                  &xlate_LDA_LDMA },  // Leaf
  { OPR_LDMA,                 &xlate_LDA_LDMA },  // Leaf
  { OPR_IDNAME,               &xlate_IDNAME },    // Leaf; like a mem-ref
  { OPR_LDID,                 &xlate_LDID_STID },
  { OPR_STID,                 &xlate_LDID_STID },
  { OPR_ILOAD,                &xlate_xLOADx_xSTOREx },
  { OPR_ILOADX,               &xlate_xLOADx_xSTOREx },
  { OPR_MLOAD,                &xlate_xLOADx_xSTOREx },

  { OPR_ISTORE,               &xlate_xLOADx_xSTOREx },
  { OPR_ISTOREX,              &xlate_xLOADx_xSTOREx },
  { OPR_MSTORE,               &xlate_xLOADx_xSTOREx },

  { OPR_PSTID,                &xlate_LDID_STID }, // pointer version of STID 
  { OPR_PSTORE,               &xlate_xLOADx_xSTOREx }, // pointer version of store

  // Type conversion
  { OPR_CVT,                  &xlate_CVT_CVTL },
  { OPR_CVTL,                 &xlate_CVT_CVTL },
  { OPR_TAS,                  &xlate_TAS },
  
  // Expressions: Unary operations
  { OPR_INTCONST,             &xlate_INTCONST }, // Leaf
  { OPR_CONST,                &xlate_CONST },    // Leaf

  { OPR_NEG,                  &xlate_UnaryOp },
  { OPR_ABS,                  &xlate_UnaryOp },
  { OPR_SQRT,                 &xlate_UnaryOp },
  { OPR_RSQRT,                &xlate_UnaryOp },
  { OPR_RECIP,                &xlate_UnaryOp },
  { OPR_REALPART,             &xlate_UnaryOp }, // OPR_FIRSTPART
  { OPR_IMAGPART,             &xlate_UnaryOp }, // OPR_SECONDPART
  { OPR_PAREN,                &xlate_UnaryOp },
  { OPR_RND,                  &xlate_UnaryOp },
  { OPR_TRUNC,                &xlate_UnaryOp },
  { OPR_CEIL,                 &xlate_UnaryOp },
  { OPR_FLOOR,                &xlate_UnaryOp },
  { OPR_BNOT,                 &xlate_UnaryOp },
  { OPR_LNOT,                 &xlate_UnaryOp },
  // FIXME: LOWPART, HIGHPART, MINPART, MAXPART, ILDA, EXTRACT_BITS
  // structure field access
  { OPR_STRCTFLD,             &xlate_STRCTFLD },
  { OPR_PARM,                 &xlate_PARM },
  // FIXME: ASM_INPUT
  { OPR_ALLOCA,               &xlate_ALLOCA },

  // Expressions: Binary operations
  { OPR_COMPLEX,              &xlate_BinaryOp }, // OPR_PAIR
  { OPR_ADD,                  &xlate_BinaryOp },
  { OPR_SUB,                  &xlate_BinaryOp },
  { OPR_MPY,                  &xlate_BinaryOp },
  // FIXME: HIGHMPY, XMPY
  { OPR_DIV,                  &xlate_BinaryOp },
  { OPR_MOD,                  &xlate_BinaryOp },
  { OPR_REM,                  &xlate_BinaryOp },
  // FIXME: DIVREM
  { OPR_MAX,                  &xlate_BinaryOp },
  { OPR_MIN,                  &xlate_BinaryOp },
  // FIXME: MINMAX
  { OPR_EQ,                   &xlate_BinaryOp },
  { OPR_NE,                   &xlate_BinaryOp },
  { OPR_GE,                   &xlate_BinaryOp },
  { OPR_GT,                   &xlate_BinaryOp },
  { OPR_LE,                   &xlate_BinaryOp },
  { OPR_LT,                   &xlate_BinaryOp },
  { OPR_BAND,                 &xlate_BinaryOp },
  { OPR_BIOR,                 &xlate_BinaryOp },
  { OPR_BNOR,                 &xlate_BinaryOp },
  { OPR_BXOR,                 &xlate_BinaryOp },
  { OPR_LAND,                 &xlate_BinaryOp },
  { OPR_LIOR,                 &xlate_BinaryOp },
  { OPR_CAND,                 &xlate_BinaryOp },
  { OPR_CIOR,                 &xlate_BinaryOp },
  { OPR_SHL,                  &xlate_BinaryOp },
  { OPR_ASHR,                 &xlate_BinaryOp },
  { OPR_LSHR,                 &xlate_BinaryOp },
  // FIXME: COMPOSE_BITS, RROTATE, COMMA, RCOMMA
  
  // Expressions: Ternary operations
  { OPR_SELECT,               &xlate_TernaryOp },
  // FIXME: CSELECT
  { OPR_MADD,                 &xlate_TernaryOp },
  { OPR_MSUB,                 &xlate_TernaryOp },
  { OPR_NMADD,                &xlate_TernaryOp },
  { OPR_NMSUB,                &xlate_TernaryOp },

  // Expressions: N-ary operations
  { OPR_ARRAY,                &xlate_ARRAYx },
  { OPR_INTRINSIC_OP,         &xlate_xCALL },
  { OPR_IO_ITEM,              &xlate_IO_ITEM },
  { OPR_TRIPLET,              &xlate_TernaryOp },
  { OPR_SRCTRIPLET,           &xlate_TernaryOp },
  { OPR_ARRAYEXP,             &xlate_ARRAYx },
  { OPR_ARRSECTION,           &xlate_ARRAYx },
  { OPR_WHERE,                &xlate_TernaryOp },
  
  
  { OPERATOR_UNKNOWN,         &xlate_unknown } // just for convenience
};

unsigned int WNXlationTable::initTableSz = INIT_TABLE_SZ;

#undef TABLE_SZ
#undef INIT_TABLE_SZ


WNXlationTable::WNXlationTable()
{
  // Initialize table with default handler
  for (int i = 0; i < tableSz; ++i) {
    table[i] = &xlate_unknown;
  }
  
  // Initialize the table using 'initTable'
  for (int i = 0; i < initTableSz; ++i) {
    table[initTable[i].opr] = initTable[i].fn;
  }
  
  initialized = true; // could use to this initialize on demand
}

WNXlationTable::~WNXlationTable()
{
}

