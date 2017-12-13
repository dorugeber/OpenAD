// -*-Mode: C++;-*-
// $Header: /Volumes/cvsrep/developer/OpenADFortTk/src/whirl2sexp/wn2sexp.cxx,v 1.16 2006/07/06 18:38:41 utke Exp $

#include <stdlib.h>

#include "Open64IRInterface/Open64BasicTypes.h"

#include "SexpTags.h"
#include "Diagnostics.h"

#include "wn2sexp.i"
#include "wn2sexp.h"


//************************** Forward Declarations ***************************

using namespace whirl2sexp;
using namespace sexp; // for sexp::ostream, etc


//***************************************************************************
// Translate any WN
//***************************************************************************

// TranslateWN: see header. 
whirl2sexp::status 
whirl2sexp::TranslateWN(sexp::ostream& sos, WN* wn)
{   
  // The task of translation is dispatched to the appropriate handler
  // registered in the handler table.
  static WNXlationTable handlerTable;
  
  if (!wn) { 
    sos << BegList << EndList;
    return whirl2sexp::good; 
  }
  
  OPERATOR opr = WN_operator(wn);
  FORTTK_DEVMSG(3, "Translating " << OPERATOR_name(opr));
  
  // Dispatch to the appropriate handler for this construct.
  return handlerTable[opr](sos, wn);
}


// TranslateWNChildren: see header.
whirl2sexp::status 
whirl2sexp::TranslateWNChildren(sexp::ostream& sos, WN* wn)
{   
  FORTTK_ASSERT(wn, fortTkSupport::Diagnostics::UnexpectedInput); 
  
  INT nkids = WN_kid_count(wn);
  for (INT kidno = 0; kidno < nkids; ++kidno) {
    WN* kidwn = WN_kid(wn, kidno);
    TranslateWN(sos, kidwn);
    if (kidno < nkids - 1) {
      sos << EndLine; // do not output after last call
    }
  }
  
  return whirl2sexp::good;
}


//***************************************************************************
// S-expressions sexp::ostream operators
//***************************************************************************

// Cf. GenSexpSym
sexp::ostream&
operator<<(std::ostream& os, const GenSexpSymInfo_& x)
{
  sexp::ostream& sos = dynamic_cast<sexp::ostream&>(os);

  ST_IDX st_idx = x.val;
  UINT lvl = (UINT)ST_IDX_level(st_idx);
  UINT idx = (UINT)ST_IDX_index(st_idx);

  sos << BegList << Atom(SexpTags::ST) << Atom(lvl) << Atom(idx) << EndList;
  
  return sos;
}


// Cf. GenSexpTy
sexp::ostream&
operator<<(std::ostream& os, const GenSexpTyInfo_& x)
{
  sexp::ostream& sos = dynamic_cast<sexp::ostream&>(os);

  TY_IDX ty_idx = x.val;
  const char* nm = TY_name(ty_idx);
  UINT32 idx = TY_id(ty_idx);
  UINT32 algn = TY_align(ty_idx);

  using namespace sexp::IOFlags;
  sos << BegList 
      << Atom(SexpTags::TY) << Atom(A_DQUOTE, nm) << Atom(idx) << Atom(algn)
      << EndList;
  
  return sos;
}


// Cf. GenSexpWNOpr
sexp::ostream&
operator<<(std::ostream& os, const GenSexpWNOprInfo_& x)
{
  sexp::ostream& sos = dynamic_cast<sexp::ostream&>(os);

  WN* wn = x.val;
  OPCODE opc = WN_opcode(wn);

  OPERATOR opr = OPCODE_operator(opc);
  TYPE_ID rty = OPCODE_rtype(opc);
  TYPE_ID dty = OPCODE_desc(opc);
  
  const char* oprNm = OPERATOR_name(opr) + 4; // skip "OPR_" 
  const char* rtyNm = Mtype_Name(rty);
  const char* dtyNm = Mtype_Name(dty);
  
  sos << Atom(oprNm) << Atom(rtyNm) << Atom(dtyNm);
  
  return sos;
}


// Cf. GenSexpSymRef
sexp::ostream&
operator<<(std::ostream& os, const GenSexpSymRefInfo_& x)
{
  sexp::ostream& sos = dynamic_cast<sexp::ostream&>(os);

  ST_IDX st_idx = x.val;
  ST* st = (st_idx != 0) ? &St_Table[st_idx] : NULL;

  UINT lvl = 0;
  UINT idx = 0;
  if (st) {
    lvl = (UINT)ST_level(st);
    idx = (UINT)ST_index(st);
  } 
  
  sos << BegList << Atom(SexpTags::ST) << GenSexpSymNm(st)
      << Atom(lvl) << Atom(idx) << EndList;
  
  return sos;
}


// Cf. GenSexpSymNm
sexp::ostream&
operator<<(std::ostream& os, const GenSexpSymNmInfo_& x)
{
  sexp::ostream& sos = dynamic_cast<sexp::ostream&>(os);

  ST* st = x.val;
  
  const char* nm = NULL;
  if (st) {
    if (ST_class(st) == CLASS_CONST) {
      TCON& tcon = STC_val(st);
      if (TCON_ty(tcon) == MTYPE_STR) {
	STR_IDX idx = TCON_str_idx(tcon);
	nm = Index_to_char_array(idx);
      }
      else {
	nm = Targ_Print(NULL, tcon);
      }
    }
    else {
      nm = ST_name(st);
    }
  }
  
  using namespace sexp::IOFlags;
  sos << Atom(A_DQUOTE, nm);
  
  return sos;
}


// Cf. GenSexpTyUse
sexp::ostream&
operator<<(std::ostream& os, const GenSexpTyUseInfo_& x)
{
  sexp::ostream& sos = dynamic_cast<sexp::ostream&>(os);

  TY_IDX ty_idx = x.val;
  TY* ty = (ty_idx != 0) ? &Ty_Table[ty_idx] : NULL;
  
  const char* nm = NULL;
  UINT32 idx = 0;
  UINT32 algn = 0;
  if (ty) {
    nm = TY_name(ty_idx);
    idx = TY_id(ty_idx);
    algn = TY_align(ty_idx);
  } 
  
  using namespace sexp::IOFlags;
  sos << BegList 
      << Atom(SexpTags::TY) << Atom(A_DQUOTE, nm) << Atom(idx) << Atom(algn)
      << EndList;
  
  return sos;
}


// Cf. GenSexpFlg
sexp::ostream&
operator<<(std::ostream& os, const GenSexpFlgInfo_& x)
{
  sexp::ostream& sos = dynamic_cast<sexp::ostream&>(os);
  
  const char* val = x.val;

  using namespace sexp::IOFlags;
  sos << BegList << Atom(SexpTags::FLG) << Atom(A_DQUOTE, val) << EndList;
  
  return sos;
}


// Cf. GenSexpOpaqueFlg
sexp::ostream&
operator<<(std::ostream& os, const GenSexpOpaqueFlgInfo_& x)
{
  sexp::ostream& sos = dynamic_cast<sexp::ostream&>(os);
  
  UINT64 val = x.val;

  using namespace sexp::IOFlags;
  sos << BegList << Atom(SexpTags::OFLG) << Atom(A_HEX, val) << EndList;
  
  return sos;
}


//***************************************************************************
// Structured Control Flow Statements: translation of these is
//   superceded by construction of the control flow graph.
//***************************************************************************

whirl2sexp::status
whirl2sexp::xlate_FUNC_ENTRY(sexp::ostream& sos, WN* wn)
{
  FORTTK_ASSERT(WN_operator(wn) == OPR_FUNC_ENTRY, fortTkSupport::Diagnostics::UnexpectedInput); 

  // For a routine definition, the following is true (but there are
  // exceptions, e.g., in INTERFACE node)
  //   kid 0..n-4: formals
  //   kid n-3:    WN_func_pragmas(wn)
  //   kid n-2:    WN_func_varrefs(wn)
  //   kid n-1:    WN_func_body(wn)
  
  ST_IDX st_idx = WN_entry_name(wn);
  
  sos << BegList << GenSexpWNOpr(wn);                 // WN_OPR
  sos << BegList << GenSexpSymRef(st_idx) << EndList; // WN_ATTRS
  if (WN_kid_count(wn) > 0) {
    sos << EndLine;
  }
  TranslateWNChildren(sos, wn); // KIDs
  sos << EndList;
  
  return whirl2sexp::good;
}


whirl2sexp::status 
whirl2sexp::xlate_BLOCK(sexp::ostream& sos, WN* wn)
{
  FORTTK_ASSERT(WN_operator(wn) == OPR_BLOCK, fortTkSupport::Diagnostics::UnexpectedInput);
  
  sos << BegList << GenSexpWNOpr(wn); // WN_OPR
  sos << BegList << EndList;          // WN_ATTRS
  if (WN_first(wn)) {
    // Generate a new line only if something is to follow.  BLOCKS are
    // special in that the attribute list is followed by 0 or more
    // WHIRL_ASTs, not it's own list.
    sos << EndLine;
  }
  
  for (WN* x = WN_first(wn); x != NULL; x = WN_next(x)) { // KIDs
    TranslateWN(sos, x);
    if (WN_next(x)) {
      sos << EndLine; // do not output after last call
    }
  }
  sos << EndList;
  
  return whirl2sexp::good;
}


whirl2sexp::status 
whirl2sexp::xlate_REGION(sexp::ostream& sos, WN* wn)
{
  FORTTK_ASSERT(WN_operator(wn) == OPR_REGION, fortTkSupport::Diagnostics::UnexpectedInput); 

  FORTTK_DIE(fortTkSupport::Diagnostics::Unimplemented);
  return whirl2sexp::good;
}


whirl2sexp::status 
whirl2sexp::xlate_structured_cf(sexp::ostream& sos, WN* wn)
{
  OPERATOR opr = WN_operator(wn);
  FORTTK_ASSERT(opr == OPR_DO_LOOP || opr == OPR_DO_WHILE || 
		opr == OPR_WHILE_DO || opr == OPR_IF, 
		fortTkSupport::Diagnostics::UnexpectedInput); 
  
  sos << BegList << GenSexpWNOpr(wn);   // WN_OPR
  sos << BegList;                       // WN_ATTRS
  if (opr == OPR_IF) {
    UINT32 flg = WN_if_flag(wn);
    sos << GenSexpOpaqueFlg(flg);
  }
  sos << EndList << EndLine; 
  TranslateWNChildren(sos, wn);         // KIDs
  sos << EndList;

  
  return whirl2sexp::good;
}


//***************************************************************************
// Unstructured Control Flow Statements
//***************************************************************************

whirl2sexp::status
whirl2sexp::xlate_IMPLIED_DO(sexp::ostream& sos, WN* wn)
{
  FORTTK_ASSERT(WN_operator(wn) == OPR_IMPLIED_DO, fortTkSupport::Diagnostics::UnexpectedInput); 
  FORTTK_DIE(fortTkSupport::Diagnostics::Unimplemented);
  return whirl2sexp::good;
}


whirl2sexp::status 
whirl2sexp::xlate_GOTOx_LABEL(sexp::ostream& sos, WN* wn)
{
  OPERATOR opr = WN_operator(wn);
  FORTTK_ASSERT(opr == OPR_GOTO || opr == OPR_GOTO_OUTER_BLOCK ||
		opr == OPR_REGION_EXIT || opr == OPR_LABEL, 
		fortTkSupport::Diagnostics::UnexpectedInput); 
  
  INT32 lbl = WN_label_number(wn);
  sos << BegList << GenSexpWNOpr(wn); // WN_OPR
  sos << BegList << Atom(lbl);        // WN_ATTRS
  if (opr == OPR_GOTO_OUTER_BLOCK) {
    UINT32 lbl_lvl = WN_label_level(wn);
    sos << Atom(lbl_lvl) << EndList;
  }
  else if (opr == OPR_LABEL) {
    UINT32 flg = WN_label_flag(wn);
    sos << GenSexpOpaqueFlg(flg);
  }
  sos << EndList;
  if (opr == OPR_LABEL) {             // KID 0
    sos << EndLine;
    TranslateWN(sos, WN_kid0(wn));
  }
  sos << EndList;
  
  return whirl2sexp::good;
}


whirl2sexp::status
whirl2sexp::xlate_multiBR(sexp::ostream& sos, WN* wn)
{
  OPERATOR opr = WN_operator(wn);
  FORTTK_ASSERT(opr == OPR_SWITCH || opr == OPR_COMPGOTO, 
		fortTkSupport::Diagnostics::UnexpectedInput); 
  
  INT32 nentries = WN_num_entries(wn);
  INT32 llbl     = WN_last_label(wn);
  
  sos << BegList << GenSexpWNOpr(wn);                       // WN_OPR
  sos << BegList << Atom(nentries) << Atom(llbl) << EndList // WN_ATTRS
      << EndLine;
  TranslateWNChildren(sos, wn);                             // KIDs
  sos << EndList;
  
  return whirl2sexp::good;
}


whirl2sexp::status
whirl2sexp::xlate_CASEGOTO(sexp::ostream& sos, WN* wn)
{
  FORTTK_ASSERT(WN_operator(wn) == OPR_CASEGOTO, fortTkSupport::Diagnostics::UnexpectedInput); 
  
  INT64 cval = WN_const_val(wn);
  INT32 lbl = WN_label_number(wn);
  
  sos << BegList << GenSexpWNOpr(wn);                   // WN_OPR
  sos << BegList << Atom(cval) << Atom(lbl) << EndList; // WN_ATTRS
  sos << EndList;
  
  return whirl2sexp::good;
}


whirl2sexp::status 
whirl2sexp::xlate_AGOTO(sexp::ostream& sos, WN* wn)
{
  FORTTK_ASSERT(WN_operator(wn) == OPR_AGOTO, fortTkSupport::Diagnostics::UnexpectedInput); 
  
  // XGOTO: number_entries st_idx kid0 kid1
  // AGOTO: kid0
  FORTTK_DIE(fortTkSupport::Diagnostics::Unimplemented);
  return whirl2sexp::good;
}


whirl2sexp::status
whirl2sexp::xlate_ALTENTRY(sexp::ostream& sos, WN* wn)
{
  FORTTK_ASSERT(WN_operator(wn) == OPR_ALTENTRY, fortTkSupport::Diagnostics::UnexpectedInput); 
  
  FORTTK_DIE(fortTkSupport::Diagnostics::Unimplemented);  
  return whirl2sexp::good;
}


whirl2sexp::status 
whirl2sexp::xlate_condBR(sexp::ostream& sos, WN* wn)
{
  OPERATOR opr = WN_operator(wn);
  FORTTK_ASSERT(opr == OPR_TRUEBR || opr == OPR_FALSEBR,
		fortTkSupport::Diagnostics::UnexpectedInput);
  
  INT32 lbl = WN_label_number(wn);
  sos << BegList << GenSexpWNOpr(wn);                // WN_OPR
  sos << BegList << Atom(lbl) << EndList << EndLine; // WN_ATTRS
  TranslateWN(sos, WN_kid0(wn));                     // KID 0
  sos << EndList;
  
  return whirl2sexp::good;
}


whirl2sexp::status 
whirl2sexp::xlate_RETURNx(sexp::ostream& sos, WN* wn)
{
  OPERATOR opr = WN_operator(wn);
  FORTTK_ASSERT(opr == OPR_RETURN || opr == OPR_RETURN_VAL,
		fortTkSupport::Diagnostics::UnexpectedInput);

  sos << BegList << GenSexpWNOpr(wn); // WN_OPR
  sos << BegList << EndList;          // WN_ATTRS
  if (opr == OPR_RETURN_VAL) {        // KID 0
    sos << EndLine;
    TranslateWN(sos, WN_kid0(wn));
  }
  sos << EndList;

  return whirl2sexp::good;
}


//***************************************************************************
// Calls
//***************************************************************************

whirl2sexp::status 
whirl2sexp::xlate_xCALL(sexp::ostream& sos, WN* wn)
{
  OPERATOR opr = WN_operator(wn);
  FORTTK_ASSERT(opr == OPR_CALL || opr == OPR_ICALL || 
		opr == OPR_VFCALL || opr == OPR_PICCALL ||
		opr == OPR_INTRINSIC_CALL || opr == OPR_INTRINSIC_OP, 
		fortTkSupport::Diagnostics::UnexpectedInput); 
  
  UINT32 flg = 0;
  if (opr != OPR_VFCALL) {
    flg = WN_call_flag(wn); // VFCALL doesn't have a flags field...
  }
  
  sos << BegList << GenSexpWNOpr(wn); // WN_OPR

  sos << BegList;                     // WN_ATTRS
  if (opr == OPR_CALL || opr == OPR_PICCALL) {
    ST_IDX st_idx = WN_st_idx(wn);
    sos << GenSexpSymRef(st_idx);
  } 
  else if (opr == OPR_ICALL || opr == OPR_VFCALL) {
    TY_IDX ty_idx = WN_ty(wn);
    sos << GenSexpTyUse(ty_idx);
  } 
  else {
    INTRINSIC intrn = WN_intrinsic(wn);
    const char* nm = INTRINSIC_name(intrn);
    sos << Atom(nm);
  }
  if (opr != OPR_VFCALL) {
    sos << GenSexpOpaqueFlg(flg);
  }
  sos << EndList;
  if (WN_kid_count(wn) > 0) {
    sos << EndLine;
  }

  TranslateWNChildren(sos, wn); // KIDs
  sos << EndList;
  
  return whirl2sexp::good;
}


whirl2sexp::status 
whirl2sexp::xlate_IO(sexp::ostream& sos, WN* wn)
{
  FORTTK_ASSERT(WN_operator(wn) == OPR_IO, fortTkSupport::Diagnostics::UnexpectedInput);
  
  IOSTATEMENT ios = WN_io_statement(wn);
  UINT32      flg = WN_io_flag(wn);
  const char* nm  = IOSTATEMENT_name(ios);
  
  using namespace sexp::IOFlags;
  sos << BegList << GenSexpWNOpr(wn); // WN_OPR
  sos << BegList                      // WN_ATTRS
      << Atom(nm) << GenSexpOpaqueFlg(flg)
      << EndList << EndLine;
  TranslateWNChildren(sos, wn);       // KIDs
  sos << EndList;
  
  return whirl2sexp::good;
}


whirl2sexp::status 
whirl2sexp::xlate_IO_ITEM(sexp::ostream& sos, WN* wn)
{
  FORTTK_ASSERT(WN_operator(wn) == OPR_IO_ITEM, fortTkSupport::Diagnostics::UnexpectedInput);
  
  IOITEM ioi    = WN_io_item(wn);
  TY_IDX ty_idx = WN_ty(wn);
  const char* nm = IOITEM_name(ioi);
  
  using namespace sexp::IOFlags;
  sos << BegList << GenSexpWNOpr(wn); // WN_OPR
  sos << BegList                      // WN_ATTRS
      << Atom(nm) << GenSexpTyUse(ty_idx)
      << EndList;
  if (WN_kid_count(wn) > 0) {
    sos << EndLine;
  }
  TranslateWNChildren(sos, wn);       // KIDs
  sos << EndList;
  
  return whirl2sexp::good;
}


//***************************************************************************
// Other Statements
//***************************************************************************

whirl2sexp::status
whirl2sexp::xlate_misc_stmt(sexp::ostream& sos, WN* wn)
{
  OPERATOR opr = WN_operator(wn);
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
  
  // misc. statements 
  //   EVAL:                kid0
  //   PREFETCH:     flag,  kids
  //   PREFETCHX:    flag,  kids
  //   COMMENT:      stidx  
  //   TRAP:         ofst,  kids
  //   ASSERT:       ofst,  kids
  //   AFFIRM:              kids
  //   FBARRIER:            kids
  //   BBARRIER:            kids
  //   DEALLOCA:            kids
  //   USE:          stidx, kids
  //   NAMELIST:     stidx, kids
  //   IMPLICIT_BND: 
  //   NULLIFY:             kids
  //   INTERFACE:    stidx, kids
  //   ACONSTRUCT:          kids
  
  sos << BegList << GenSexpWNOpr(wn); // WN_OPR
  
  sos << BegList;                     // WN_ATTRS
  if (OPERATOR_has_sym(opr)) {
    ST_IDX st_idx = WN_st_idx(wn);  
    sos << GenSexpSymRef(st_idx);
  }
  if (OPERATOR_has_offset(opr)) {
    WN_OFFSET ofst = WN_offset(wn);
    sos << Atom(ofst);
  }
  if (OPERATOR_has_flags(opr)) {
    UINT32 flg = WN_flag(wn);
    sos << GenSexpOpaqueFlg(flg);
  }
  sos << EndList;

  if (WN_kid_count(wn) > 0) {
    sos << EndLine;
  }
  TranslateWNChildren(sos, wn); // KIDs
  sos << EndList;

  return whirl2sexp::good;
}


whirl2sexp::status
whirl2sexp::xlate_xPRAGMA(sexp::ostream& sos, WN* wn)
{
  OPERATOR opr = WN_operator(wn);
  FORTTK_ASSERT(opr == OPR_PRAGMA || opr == OPR_XPRAGMA, 
		fortTkSupport::Diagnostics::UnexpectedInput); 
  
  WN_PRAGMA_ID prag = (WN_PRAGMA_ID)WN_pragma(wn);
  UINT16 flg    = WN_pragma_flags(wn);
  ST_IDX st_idx = WN_st_idx(wn);
  
  sos << BegList << GenSexpWNOpr(wn); // WN_OPR
  sos << BegList << Atom(prag) << GenSexpSymRef(st_idx) // WN_ATTRS
      << GenSexpOpaqueFlg(flg);
  
  if (opr == OPR_PRAGMA) {
    INT64 cval = WN_const_val(wn);
    sos << Atom(cval) << EndList; // end WN_ATTRS
  } 
  else {
    sos << EndList; // end WN_ATTRS
    sos << EndLine;	
    TranslateWN(sos, WN_kid0(wn)); // KID 0
  }
  
  sos << EndList;
  return whirl2sexp::good;
}


//***************************************************************************
// Loads (In WHIRL, loads are expressions.)
// Stores (In WHIRL, stores are statements.)
//***************************************************************************

whirl2sexp::status 
whirl2sexp::xlate_LDA_LDMA(sexp::ostream& sos, WN* wn)
{
  OPERATOR opr = WN_operator(wn);
  FORTTK_ASSERT(opr == OPR_LDA || opr == OPR_LDMA, fortTkSupport::Diagnostics::UnexpectedInput);

  ST_IDX    st_idx = WN_st_idx(wn);
  WN_OFFSET ofst   = WN_load_offset(wn);
  TY_IDX    ty_idx = WN_ty(wn);
  UINT      fldid  = WN_field_id(wn);

  sos << BegList << GenSexpWNOpr(wn); // WN_OPR
  sos << BegList << GenSexpSymRef(st_idx) << Atom(ofst)
      << GenSexpTyUse(ty_idx) << Atom(fldid) << EndList; // WN_ATTRS
  sos << EndList;
  
  return whirl2sexp::good;
}


whirl2sexp::status 
whirl2sexp::xlate_LDID_STID(sexp::ostream& sos, WN* wn)
{
  OPERATOR opr = WN_operator(wn);
  FORTTK_ASSERT(opr == OPR_LDID 
		|| 
		opr == OPR_STID
		||
		opr == OPR_PSTID, 
		fortTkSupport::Diagnostics::UnexpectedInput);
  
  ST_IDX    st_idx = WN_st_idx(wn);
  WN_OFFSET ofst   = WN_offset(wn); // WN_load_offset, WN_store_offset
  TY_IDX    ty_idx = WN_ty(wn);
  UINT      fldid  = WN_field_id(wn);
  
  sos << BegList << GenSexpWNOpr(wn); // WN_OPR
  sos << BegList << GenSexpSymRef(st_idx) << Atom(ofst) 
      << GenSexpTyUse(ty_idx) << Atom(fldid) << EndList; // WN_ATTRS
  if (opr == OPR_STID || opr == OPR_PSTID) {
    sos << EndLine;
    TranslateWN(sos, WN_kid0(wn)); // KID 0
  }
  sos << EndList;

  return whirl2sexp::good;
} 


whirl2sexp::status 
whirl2sexp::xlate_IDNAME(sexp::ostream& sos, WN* wn)
{
  OPERATOR opr = WN_operator(wn);
  FORTTK_ASSERT(opr == OPR_IDNAME, fortTkSupport::Diagnostics::UnexpectedInput); 
  
  ST_IDX st_idx = WN_st_idx(wn);
  WN_OFFSET ofst = WN_idname_offset(wn);
  
  sos << BegList << GenSexpWNOpr(wn) // WN_OPR
      << BegList << GenSexpSymRef(st_idx) << Atom(ofst) << EndList // WN_ATTRS
      << EndList;
  return whirl2sexp::good;
}


whirl2sexp::status 
whirl2sexp::xlate_xLOADx_xSTOREx(sexp::ostream& sos, WN* wn)
{
  OPERATOR opr = WN_operator(wn);
  FORTTK_ASSERT(opr == OPR_ILOAD || opr == OPR_MLOAD || opr == OPR_ILOADX ||
		opr == OPR_ISTORE || opr == OPR_MSTORE || opr == OPR_ISTOREX ||
		opr == OPR_PSTORE,
		fortTkSupport::Diagnostics::UnexpectedInput);
  
  // ILOAD:   ofst field_id ty_idx1 ty_idx2 kid0
  // MLOAD:   ofst field_id ty_idx          kid0 kid1  
  // ISTORE:  ofst field_id ty_idx          kid0 kid1
  // PSTORE:  ofst field_id ty_idx          kid0 kid1
  // MSTORE:  ofst field_id ty_idx          kid0 kid1 kid2  
  // ILOADX:                ty_idx1 ty_idx2 kid0 kid1
  // ISTOREX:               ty_idx          kid0 kid1 kid2

  TY_IDX ty_idx1 = WN_ty(wn);
  
  sos << BegList << GenSexpWNOpr(wn); // WN_OPR

  sos << BegList;                     // WN_ATTRS
  if (opr == OPR_ILOAD || opr == OPR_MLOAD || 
      opr == OPR_ISTORE || opr == OPR_MSTORE || 
      opr == OPR_PSTORE) {
    WN_OFFSET ofst  = WN_load_offset(wn);
    UINT      fldid = WN_field_id(wn);
    sos << Atom(ofst) << Atom(fldid);
  }
  sos << GenSexpTyUse(ty_idx1);
  if (opr == OPR_ILOAD || opr == OPR_ILOADX) {
    TY_IDX ty_idx2 = WN_load_addr_ty(wn);
    sos << GenSexpTyUse(ty_idx2);
  }
  sos << EndList << EndLine;
  
  TranslateWNChildren(sos, wn); // KIDs
  sos << EndList;

  return whirl2sexp::good;
}

//***************************************************************************
// Array Operators (N-ary Operations)
//***************************************************************************

whirl2sexp::status
whirl2sexp::xlate_ARRAYx(sexp::ostream& sos, WN* wn)
{
  OPERATOR opr = WN_operator(wn);
  FORTTK_ASSERT(opr == OPR_ARRAY || opr == OPR_ARRAYEXP || 
		opr == OPR_ARRSECTION, fortTkSupport::Diagnostics::UnexpectedInput);
  
  // N.B.: WHIRL indices are 0-based and memory layout is row-major
  // (right-most index represents contiguous elements).  
  // In contrast, Fortran indices are 1-based and memory layout is
  // column-major (left-most index represents contiguous elements).
  // To convert WHIRL indices into a Fortran index expression, reverse
  // their order and denormalize to base 1.
  
  sos << BegList << GenSexpWNOpr(wn); // WN_OPR
  sos << BegList;                     // WN_ATTRS
  if (opr == OPR_ARRAY || opr == OPR_ARRSECTION) {
    WN_ESIZE sz = WN_element_size(wn);
    sos << Atom(sz);
  }
  sos << EndList << EndLine;
  TranslateWNChildren(sos, wn);       // KIDs
  sos << EndList;
  
  return whirl2sexp::good;
}


//***************************************************************************
// Type Conversion
//***************************************************************************

whirl2sexp::status 
whirl2sexp::xlate_CVT_CVTL(sexp::ostream& sos, WN* wn)
{
  OPERATOR opr = WN_operator(wn);
  FORTTK_ASSERT(opr == OPR_CVT || opr == OPR_CVTL, fortTkSupport::Diagnostics::UnexpectedInput);

  sos << BegList << GenSexpWNOpr(wn); // WN_OPR
  sos << BegList;                     // WN_ATTRS
  if (opr == OPR_CVTL) {
    INT16 cvtlbits = WN_cvtl_bits(wn);
    sos << Atom(cvtlbits);
  }
  sos << EndList << EndLine;
  TranslateWN(sos, WN_kid0(wn));
  sos << EndList;

  return whirl2sexp::good;
}


whirl2sexp::status 
whirl2sexp::xlate_TAS(sexp::ostream& sos, WN* wn)
{
  FORTTK_ASSERT(WN_operator(wn) == OPR_TAS, fortTkSupport::Diagnostics::UnexpectedInput); 

  TY_IDX ty_idx = WN_ty(wn);

  sos << BegList << GenSexpWNOpr(wn); // WN_OPR
  sos << BegList << GenSexpTyUse(ty_idx) << EndList; // WN_ATTRS
  TranslateWN(sos, WN_kid0(wn));
  sos << EndList;

  return whirl2sexp::good;
}


//***************************************************************************
// Leaf (Other)
//***************************************************************************

whirl2sexp::status 
whirl2sexp::xlate_INTCONST(sexp::ostream& sos, WN* wn)
{
  FORTTK_ASSERT(WN_operator(wn) == OPR_INTCONST, fortTkSupport::Diagnostics::UnexpectedInput);
  
  INT64 val = WN_const_val(wn);
  
  sos << BegList << GenSexpWNOpr(wn);     // WN_OPR
  sos << BegList << Atom(val) << EndList; // WN_ATTRS
  sos << EndList;
  
  return whirl2sexp::good;
}


whirl2sexp::status 
whirl2sexp::xlate_CONST(sexp::ostream& sos, WN* wn)
{
  FORTTK_ASSERT(WN_operator(wn) == OPR_CONST, fortTkSupport::Diagnostics::UnexpectedInput); 

  ST_IDX st_idx = WN_st_idx(wn);

  sos << BegList << GenSexpWNOpr(wn);                 // WN_OPR
  sos << BegList << GenSexpSymRef(st_idx) << EndList; // WN_ATTRS
  sos << EndList;
  
  return whirl2sexp::good;
}


//***************************************************************************
// Expression Operators: Unary Operations
//***************************************************************************

whirl2sexp::status
whirl2sexp::xlate_UnaryOp(sexp::ostream& sos, WN* wn)
{
  OPERATOR opr = WN_operator(wn);
  FORTTK_ASSERT(WN_kid_count(wn) == 1, 
		fortTkSupport::Diagnostics::UnexpectedInput << OPERATOR_name(opr));
  
  sos << BegList << GenSexpWNOpr(wn);   // WN_OPR
  sos << BegList << EndList << EndLine; // WN_ATTRS
  TranslateWN(sos, WN_kid0(wn));        // KID 0
  sos << EndList;
  
  return whirl2sexp::good;
}

//***************************************************************************
// structure field access
//***************************************************************************

whirl2sexp::status 
whirl2sexp::xlate_STRCTFLD(sexp::ostream& sos, WN* wn)
{
  OPERATOR opr = WN_operator(wn);
  FORTTK_ASSERT(opr == OPR_STRCTFLD,
		fortTkSupport::Diagnostics::UnexpectedInput);
  
  FORTTK_ASSERT(WN_kid_count(wn) == 1, 
		fortTkSupport::Diagnostics::UnexpectedInput << OPERATOR_name(opr));

  sos << BegList << GenSexpWNOpr(wn); // WN_OPR
  {
    // get the attributes
    TY_IDX ty_idx1 = WN_load_addr_ty(wn); // the type of the structure
    TY_IDX ty_idx2 = WN_ty(wn);           // the type of the field being accessed
    UINT   fldid   = WN_field_id(wn);     // the field_id of the field
    // put them in the list
    sos << BegList;                       
    sos << GenSexpTyUse(ty_idx1);
    sos << GenSexpTyUse(ty_idx2);
    sos << Atom(fldid);
    sos << EndList << EndLine;
    TranslateWNChildren(sos, wn);
  }
  sos << EndList;
  return whirl2sexp::good;
}

whirl2sexp::status 
whirl2sexp::xlate_PARM(sexp::ostream& sos, WN* wn)
{
  FORTTK_ASSERT(WN_operator(wn) == OPR_PARM, fortTkSupport::Diagnostics::UnexpectedInput);

  UINT32 flg    = WN_parm_flag(wn);
  TY_IDX ty_idx = WN_ty(wn);

  sos << BegList << GenSexpWNOpr(wn);     // WN_OPR
  sos << BegList                          // WN_ATTRS
      << GenSexpOpaqueFlg(flg) << GenSexpTyUse(ty_idx);
  if (wn->u3.ty_fields.ty) {  // hack for keyword call
    sos << GenSexpSymRef(wn->u3.ty_fields.ty);
  }
  sos << EndList << EndLine; 
  TranslateWN(sos, WN_kid0(wn));          // KID 0
  sos << EndList;

  return whirl2sexp::good;
}


whirl2sexp::status 
whirl2sexp::xlate_ALLOCA(sexp::ostream& sos, WN* wn)
{
  FORTTK_ASSERT(WN_operator(wn) == OPR_ALLOCA, fortTkSupport::Diagnostics::UnexpectedInput);
  FORTTK_DIE(fortTkSupport::Diagnostics::Unimplemented);
  return whirl2sexp::good;
}


//***************************************************************************
// Expression Operators: Binary Operations
//***************************************************************************

whirl2sexp::status
whirl2sexp::xlate_BinaryOp(sexp::ostream& sos, WN* wn)
{
  OPERATOR opr = WN_operator(wn);
  FORTTK_ASSERT(WN_kid_count(wn) == 2, 
		fortTkSupport::Diagnostics::UnexpectedInput << OPERATOR_name(opr));
  
  sos << BegList << GenSexpWNOpr(wn);   // WN_OPR
  sos << BegList << EndList << EndLine; // WN_ATTRS
  TranslateWNChildren(sos, wn);         // KIDs
  sos << EndList;

  return whirl2sexp::good;
}


//***************************************************************************
// Expression Operators: Ternary Operations; N-ary Operations
//***************************************************************************

whirl2sexp::status
whirl2sexp::xlate_TernaryOp(sexp::ostream& sos, WN* wn)
{
  OPERATOR opr = WN_operator(wn);
  FORTTK_ASSERT(WN_kid_count(wn) == 3, 
		fortTkSupport::Diagnostics::UnexpectedInput << OPERATOR_name(opr));
  
  sos << BegList << GenSexpWNOpr(wn);   // WN_OPR
  sos << BegList << EndList << EndLine; // WN_ATTRS
  TranslateWNChildren(sos, wn);         // KIDs
  sos << EndList;

  return whirl2sexp::good;
}


//***************************************************************************
// WNXlationTable
//***************************************************************************

whirl2sexp::status
whirl2sexp::xlate_unknown(sexp::ostream& sos, WN* wn)
{
  // Warn about opcodes we cannot translate, but keep translating.
  OPERATOR opr = WN_operator(wn);
  
  FORTTK_DEVMSG(0, fortTkSupport::Diagnostics::UnexpectedOpr << OPERATOR_name(opr));
  sos << BegComment << "*** Unknown WHIRL operator: " << OPERATOR_name(opr)
      << " ***" << EndComment;
  
  return whirl2sexp::good;
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
  { OPR_INTERFACE,            &xlate_misc_stmt },
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
  { OPR_PSTORE,               &xlate_xLOADx_xSTOREx },// pointer version of STORE

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
  // structure field access: 
  { OPR_STRCTFLD,             &xlate_STRCTFLD },
  // parameter:
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

