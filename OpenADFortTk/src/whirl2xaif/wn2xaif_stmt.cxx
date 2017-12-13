// -*-Mode: C++;-*-
// $Header: /Volumes/cvsrep/developer/OpenADFortTk/src/whirl2xaif/wn2xaif_stmt.cxx,v 1.44 2006/05/12 16:12:23 utke Exp $

#include <alloca.h>
#include <sstream> 


#include "Open64IRInterface/Open64BasicTypes.h"
#include "Open64IRInterface/IntrinsicInfo.h"
#include "IntrinsicXlationTable.h"

#include "Args.h"
#include "wn2xaif.h"
#include "wn2xaif_stmt.h"
#include "wn2xaif_mem.h"
#include "wn2xaif_io.h"
#include "st2xaif.h"
#include "ty2xaif.h"


using namespace whirl2xaif;
using namespace xml; // for xml::ostream, etc


static BOOL WN2F_Skip_Stmt(WN *wn) { return FALSE; /* FIXME */ }


//***************************************************************************
// Passive Statements
//***************************************************************************
void 
whirl2xaif::xlate_PassiveStmt(xml::ostream& xos, WN *wn_p, PUXlationContext& ctxt) {
  OPERATOR opr = WN_operator(wn_p);
  
  // Short-circuit handling of the following:
  // NOTE: we could incorporate these two routines into this one
  switch (opr) {
  case OPR_GOTO:
    return xlate_GOTO(xos, wn_p, ctxt);
  case OPR_LABEL:
    return xlate_LABEL(xos, wn_p, ctxt);
  case OPR_IO: 
    return xlate_IO(xos, wn_p, ctxt);
  default:
    break;
  }

  // FIXME: cleanup AGOTO, RETURN, RETURN_VAL, PRAGMA, COMMENT, USE
  //  INTRN_CASSIGNSTMT, INTRN_STOP, INTRN_STOP_F90, IO

  if (opr==OPR_RETURN && WN_kid_count(wn_p) == 0) {  // no kids
    // get the Parent 
    WN* func_p=ctxt.findParentWN(ctxt.findParentBlockWN(wn_p));
    // and see if it is the FUNC: 
    if (WN_operator(func_p)==OPR_FUNC_ENTRY) {  
      WN* last_p=WN_last(WN_kid(func_p,WN_kid_count(func_p)-1));
      // if it is the last one: 
      if (last_p==wn_p)
	return;
      // if it is not the last one, check if there is anything 
      // else significant: 
      WN* next_p=WN_next(wn_p);
      while (next_p!=0) {
	opr=WN_operator(next_p);
	if (opr != OPR_RETURN && opr != OPR_COMMENT)
	  break; 
	if (next_p==last_p)
	  return;
	next_p=WN_next(next_p);
      }
    }
    USRCPOS srcpos;
    int aLineNumber;
    USRCPOS_srcpos(srcpos) = WN_Get_Linenum(wn_p);
    aLineNumber=USRCPOS_linenum(srcpos);
    if (!aLineNumber) {
      USRCPOS_srcpos(srcpos) = WN_Get_Linenum(func_p);
      aLineNumber=USRCPOS_linenum(srcpos);
    }
    FORTTK_MSG(1,"whirl2xaif::xlate_PassiveStmt: unstructured control flow (early return) related to line " << aLineNumber);
  }
  
  if (opr==OPR_RETURN) { 
    fortTkSupport::WNId stmtid = ctxt.findWNId(wn_p);
    xos << BegElem(XAIFStrings.elem_Marker())
	<< Attr("statement_id", stmtid)
	<< BegAttr("annotation") << WhirlIdAnnotVal(stmtid)
	<< StmtReturnAnnotVal(WN_label_number(wn_p)) << EndAttr
	<< EndElem;
  }
  else { 
    fortTkSupport::WNId stmtid = ctxt.findWNId(wn_p);
    xos << BegElem(XAIFStrings.elem_Marker()) 
	<< Attr("statement_id", stmtid)
	<< BegAttr("annotation") << WhirlIdAnnotVal(stmtid)
	<< " [passive: " << OPERATOR_name(opr) << "]" << EndAttr
	<< EndElem;
  }
}


//***************************************************************************
// Structured Control Flow Statements: translation of these is
//   superceded by construction of the control flow graph.
//***************************************************************************

void 
whirl2xaif::xlate_BLOCK(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  FORTTK_ASSERT(WN_operator(wn) == OPR_BLOCK, fortTkSupport::Diagnostics::UnexpectedInput); 
  
  FORTTK_DIE(fortTkSupport::Diagnostics::Unimplemented);
  
}


void 
whirl2xaif::WN2F_region(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  FORTTK_ASSERT(WN_operator(wn) == OPR_REGION, fortTkSupport::Diagnostics::UnexpectedInput); 

  FORTTK_DIE(fortTkSupport::Diagnostics::Unimplemented);
  
}


void 
whirl2xaif::xlate_DO_LOOP(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  FORTTK_ASSERT(WN_operator(wn) == OPR_DO_LOOP, fortTkSupport::Diagnostics::UnexpectedInput); 
  
  FORTTK_DIE(fortTkSupport::Diagnostics::Unimplemented);
  
}


void 
whirl2xaif::xlate_DO_WHILE(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  FORTTK_ASSERT(WN_operator(wn) == OPR_DO_WHILE, fortTkSupport::Diagnostics::UnexpectedInput); 

  FORTTK_DIE(fortTkSupport::Diagnostics::Unimplemented);
  
}


void 
whirl2xaif::xlate_WHILE_DO(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  FORTTK_ASSERT(WN_operator(wn) == OPR_WHILE_DO, fortTkSupport::Diagnostics::UnexpectedInput); 
  
  FORTTK_DIE(fortTkSupport::Diagnostics::Unimplemented);
  
}


void 
whirl2xaif::xlate_IF(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  FORTTK_ASSERT(WN_operator(wn) == OPR_IF, fortTkSupport::Diagnostics::UnexpectedInput); 
  
  FORTTK_DIE(fortTkSupport::Diagnostics::Unimplemented);
  
}


//***************************************************************************
// Unstructured Control Flow Statements
//***************************************************************************

void 
whirl2xaif::WN2F_implied_do(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  // REMOVE
  FORTTK_ASSERT(ctxt.currentXlationContext().isFlag(XlationContext::IO_STMT), 
		fortTkSupport::Diagnostics::UnexpectedInput); 
  
  /* This is a fortran implied do_loop, which can only occur as an
   * an OPR_IO_ITEM.  We should always be able to regenerate
   * an implied do-loop from this WHIRL tree, and we should safely
   * be able to assert that IO_STMT is TRUE.  Strictly
   * speaking this can be viewed as an expression, rather than as a
   * statement, but due to the commonality with regular do-loops
   * we handle it in this module.
   */
  
  /* Start an implied do-loop expression */
  xos << '(';
  
  /* Generate all the expression trees, separated by commas */
  for (INT kid = 4; kid < WN_kid_count(wn); kid++) {
    BOOL emitted = xlate_IO_ITEM(xos, WN_kid(wn, kid), ctxt);
    if (emitted)
      xos << ",";
  }
  
  /* Generate the loop expression */
  ST* idx_name = WN_st(WN_index(wn));
  xlate_SymRef(xos, idx_name,                      /* base-symbol */
		     Stab_Pointer_To(ST_type(idx_name)), /* base-type */
		     ST_type(idx_name),                  /* object-type */
		     0,                                  /* object-ofst */
		     ctxt);
  xos << '=';
  TranslateWN(xos, WN_start(wn), ctxt);
  xos << ',';
  TranslateWN(xos, WN_end(wn), ctxt);
  xos << ',';
  TranslateWN(xos, WN_step(wn), ctxt);
  
  /* Terminate the implied do-loop expression */
  xos << ')';
  
  
} /* WN2F_implied_do */


void
whirl2xaif::WN2F_noio_implied_do(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  xos << "(";
  TranslateWN(xos,WN_kid0(wn),ctxt);
  xos << ",";
  TranslateWN(xos,WN_kid1(wn),ctxt);
  xos << "=";
  
  INT numkids = 5;
  for (INT kid = 2;kid<numkids; kid++) {
    TranslateWN(xos,WN_kid(wn,kid),ctxt);
    if (kid < numkids-1)
      xos << ",";
  }
  
  xos << ")";
  
}


void 
whirl2xaif::xlate_GOTO(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  FORTTK_ASSERT(WN_operator(wn) == OPR_GOTO ||
		WN_operator(wn) == OPR_REGION_EXIT, fortTkSupport::Diagnostics::UnexpectedInput); 
  
  fortTkSupport::WNId stmtid = ctxt.findWNId(wn);
  xos << BegElem(XAIFStrings.elem_Marker()) 
      << Attr("statement_id", stmtid)
      << BegAttr("annotation") << WhirlIdAnnotVal(stmtid)
      << StmtGotoAnnotVal(WN_label_number(wn)) << EndAttr
      << EndElem;
}


void
whirl2xaif::xlate_SWITCH(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  FORTTK_ASSERT(WN_operator(wn) == OPR_SWITCH, fortTkSupport::Diagnostics::UnexpectedInput); 

  FORTTK_DIE(fortTkSupport::Diagnostics::Unimplemented);
  
}


void
whirl2xaif::WN2F_casegoto(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  // REMOVE
  ST *st;
  st = WN_st(wn);
  
  xos << std::endl;
  //  xos << "CASE";
  xos << "IF (";
  TranslateSTUse(xos, st, ctxt);
  std::string val = TCON2F_translate(Host_To_Targ(MTYPE_I4,WN_const_val(wn)),
				     FALSE);
  xos << " .EQ. " << val << ')' << " GO TO " << WN_label_number(wn);
  
}

void 
whirl2xaif::WN2F_agoto(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  // REMOVE
  FORTTK_ASSERT(WN_operator(wn) == OPR_AGOTO, fortTkSupport::Diagnostics::UnexpectedInput); 

  fortTkSupport::WNId stmtid = ctxt.findWNId(wn);
  xos << BegElem(XAIFStrings.elem_Marker())
      << Attr("statement_id", stmtid)
      << BegAttr("annotation") << WhirlIdAnnotVal(stmtid)
      << " [***FIXME: agoto]" << EndAttr
      << EndElem;

  xos << std::endl << "GO TO";
  TranslateWN(xos, WN_kid0(wn), ctxt); // FIXME
  
  
} /* WN2F_agoto */


void 
whirl2xaif::xlate_condBR(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  OPERATOR opr = WN_operator(wn);
  FORTTK_ASSERT(opr == OPR_TRUEBR || opr == OPR_FALSEBR,
		fortTkSupport::Diagnostics::UnexpectedInput);
  
  FORTTK_DIE(fortTkSupport::Diagnostics::Unimplemented);
  
}


void 
whirl2xaif::xlate_RETURN(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  FORTTK_ASSERT(WN_operator(wn) == OPR_RETURN, fortTkSupport::Diagnostics::UnexpectedInput); 
  // for now: 
  FORTTK_DIE(fortTkSupport::Diagnostics::Unimplemented);
}


void 
whirl2xaif::xlate_RETURN_VAL(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  FORTTK_ASSERT(WN_operator(wn) == OPR_RETURN_VAL, fortTkSupport::Diagnostics::UnexpectedInput); 
  
  fortTkSupport::WNId stmtid = ctxt.findWNId(wn);
  xos << BegElem(XAIFStrings.elem_Marker()) 
      << Attr("statement_id", stmtid)
      << BegAttr("annotation") << WhirlIdAnnotVal(stmtid)
      << " [return_val]" << EndAttr
      << EndElem;
  
  
}


void 
whirl2xaif::xlate_LABEL(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  FORTTK_ASSERT(WN_operator(wn) == OPR_LABEL, fortTkSupport::Diagnostics::UnexpectedInput); 
  
  fortTkSupport::WNId stmtid = ctxt.findWNId(wn);
  xos << BegElem(XAIFStrings.elem_Marker())
      << Attr("statement_id", stmtid)
      << BegAttr("annotation") << WhirlIdAnnotVal(stmtid)
      << StmtLabelAnnotVal(WN_label_number(wn)) << EndAttr
      << EndElem;
  
  
}


//***************************************************************************
// Calls
//***************************************************************************

void 
whirl2xaif::xlate_CALL(xml::ostream& xos, WN *wn, PUXlationContext& ctxt) {
  // XAIF distinguishes between a subroutine call (statement) and
  // function call (expression).
  // FIXME: this is a nearly incomprehensible function.  I've cleaned
  // it up a little, but it needs a lot more work.
  /* Generates a function-call and ensures that the return value
   * is returned into the appropriate context, be it a variable
   * or a register.  Note that intrinsic calls are dispatched to
   * this function from xlate_INTRINSIC_CALL() when appropriate.
   * Make sure the handling of instrinsic ops in wn2f_expr.c is
   * kept up to date with changes that occur here.
   */
  // We can't handle ICALLs yet
  OPERATOR opr = WN_operator(wn);
  FORTTK_ASSERT(opr != OPR_ICALL, fortTkSupport::Diagnostics::UnexpectedInput); 
  // -------------------------------------------------------
  // Gather info...
  // -------------------------------------------------------
  TY_IDX return_ty = WN_Call_Return_Type(wn);
  INT first_arg_idx = WN_Call_First_Arg_Idx(wn);
  INT last_arg_idx = WN_Call_Last_Arg_Idx(wn);
  BOOL is_user_call = FALSE;
  BOOL is_allocate_stmt = FALSE; 
  if (opr == OPR_CALL) {
    is_user_call = TRUE;
    const char* nm = ST_name(WN_st(wn));
    if (strcmp(nm, "_ALLOCATE") == 0) {
      xlate_PassiveStmt(xos,wn,ctxt);
      // cut short here
      return; 
      is_allocate_stmt = TRUE;
    } else if (strcmp(nm, "_DEALLOCATE") == 0) {
      xlate_PassiveStmt(xos,wn,ctxt);
      // cut short here
      return; 
      ctxt.currentXlationContext().setFlag(XlationContext::HAS_NO_ARR_ELMT);
      is_allocate_stmt = TRUE;
    } else if (strcmp(nm, "PRESENT") == 0) {
      ctxt.currentXlationContext().setFlag(XlationContext::HAS_NO_ARR_ELMT);
    }
  } else if (opr == OPR_PICCALL) {
    is_user_call = TRUE;
  }
  enum CallKind_E{SUBROUTINE_CALL,
		  FUNCTION_CALL,
		  INTRINSIC_CALL};
  CallKind_E xlate_as=SUBROUTINE_CALL; // default
  UINT targid = 0; // (FIXME)
  // -------------------------------------------------------  
  // Determine the number of implicit arguments appended to the end
  // of the argument list (i.e. string lengths).
  INT total_implicit_args = 0;
  TY_IDX arg_ty, kid_ty, parm_ty;
  TYPE_ID fmtry;
  for (INT arg_idx = first_arg_idx; 
       arg_idx <= last_arg_idx - total_implicit_args; 
       arg_idx++) {
    if (WN_kid(wn, arg_idx) != NULL) {
      OPCODE tempopc = WN_opcode(WN_kid(wn, arg_idx));
      WN* kidofparm = WN_kid0(WN_kid(wn, arg_idx));
      if (WN_operator(kidofparm) != OPR_CALL && 
	  WN_operator(kidofparm) != OPR_INTRINSIC_CALL) {
	arg_ty = WN_Tree_Type(WN_kid(wn, arg_idx));	
	parm_ty = WN_ty(WN_kid(wn,arg_idx));
	if (TY_Is_Pointer(arg_ty)) {
	  fmtry = TY_mtype(TY_pointed(arg_ty));
	} 
	else {
	  fmtry = TY_mtype(arg_ty); 
	}
	if (fmtry == MTYPE_M) {
	  if (TY_Is_Pointer(parm_ty)) { // FIXME: hack to handle KIND_STRUCT
	    fmtry = TY_pointed(parm_ty);
	    fmtry = TY_mtype(fmtry);
	  }
	}
	if (
	    (TY_Is_Character_Reference(arg_ty) 
	     || 
	     TY_Is_Chararray_Reference(arg_ty) 
	     || 
	     (
	      (TY_Is_Pointer(arg_ty) 
	       && 
	       TY_mtype(TY_pointed(arg_ty))==MTYPE_M) 
	      && 
	      (TY_Is_Character_Reference(parm_ty) 
	       || 
	       TY_Is_Chararray_Reference(parm_ty)))
	     )
	    && !is_allocate_stmt) {
	  total_implicit_args++;
	}
      } 
      else { /* the argument is function call
	      * if the return value is Chararray or Character Reference:
	      */
	if (WN_operator(kidofparm) == OPR_CALL) {
	  kid_ty = PU_prototype (Pu_Table[ST_pu(WN_st(kidofparm))]);
	  if (Func_Return_Character (kid_ty))
	    total_implicit_args++; 
	} 
	else {
	  if (WN_operator(kidofparm) == OPR_INTRINSIC_CALL &&
	      WN_intrinsic(kidofparm) == INTRN_CONCATEXPR)
	    total_implicit_args++;
	}
      }
    }
  }
  if (opr == OPR_INTRINSIC_CALL) {
    // xlate_INTRINSIC_CALL() has already handled certain intrinsics (FIXME)
    // ... only consider returns through a first non-string parameter here
    const char* inm = IntrinsicInfo::intrinsicBaseName(WN_intrinsic(wn));
    fortTkSupport::IntrinsicXlationTable::XAIFInfoPair infoPair(Whirl2Xaif::getIntrinsicXlationTable().findXAIFInfo(opr, inm));
    xlate_as = INTRINSIC_CALL;
    targid = ctxt.currentXlationContext().getNewVertexId();
    xos << BegElem("xaif:Intrinsic") 
	<< Attr("vertex_id", targid) << Attr("name", infoPair.second.name)
	<< Attr("type", "***");
    TY_IDX result_ty=WN_GetExprType(wn);
    bool isPointer = TY_Is_Pointer(result_ty) || TY_is_f90_pointer(result_ty);
    bool isFnPointer = isPointer && (TY_kind(TY_pointed(result_ty)) == KIND_FUNCTION);
    const char* ty_str = 
      (isPointer && (!isFnPointer)) ? TranslateTYToSymType(TY_pointed(result_ty))
      : TranslateTYToSymType(result_ty);
    if (!ty_str) { ty_str = "***"; }
    const char* shape_str = 
      (isPointer && (!isFnPointer)) ? TranslateTYToSymShape(TY_pointed(result_ty))
      : TranslateTYToSymShape(result_ty);
    if (strcmp(ty_str,"real"))
      xos << xml::Attr("rType", ty_str); 
    if (strcmp(shape_str,"scalar"))
      xos << xml::Attr("rShape", shape_str);
    xos << EndElem;
  } 
  else {
    // Could translate as an XAIF SubroutineCall, FunctionCall or Intrinsic
    // OPR_ICALL: TranslateWN(xos, WN_kid(wn, WN_kid_count(wn) - 1), ctxt);
    ST* st = WN_st(wn);
    ST_TAB* sttab = Scope_tab[ST_level(st)].st_tab;
    fortTkSupport::SymTabId scopeid = ctxt.findSymTabId(sttab);
    const char* funcNm = ST_name(st);
    fortTkSupport::IntrinsicXlationTable::XAIFInfoPair infoPair(Whirl2Xaif::getIntrinsicXlationTable().
								findXAIFInfo(opr, 
									     funcNm,
									     false)); // don't complain if it is not there
    if (infoPair.first) {
      // Intrinsic
      xlate_as = INTRINSIC_CALL;
      targid = ctxt.currentXlationContext().getNewVertexId();
      xos << BegElem("xaif:Intrinsic")
	  << Attr("vertex_id", targid) << Attr("name", infoPair.second.name)
	  << Attr("type", "***");
      if (infoPair.second.key) { xos << IntrinsicKeyAnnot(infoPair.second.key); }
      TY_IDX result_ty=WN_GetExprType(wn);
      bool isPointer = TY_Is_Pointer(result_ty) || TY_is_f90_pointer(result_ty);
      bool isFnPointer = isPointer && (TY_kind(TY_pointed(result_ty)) == KIND_FUNCTION);
      const char* ty_str = 
	(isPointer && (!isFnPointer)) ? TranslateTYToSymType(TY_pointed(result_ty))
	: TranslateTYToSymType(result_ty);
      if (!ty_str) { ty_str = "***"; }
      const char* shape_str = 
	(isPointer && (!isFnPointer)) ? TranslateTYToSymShape(TY_pointed(result_ty))
	: TranslateTYToSymShape(result_ty);
      if (strcmp(ty_str,"real"))
	xos << xml::Attr("rType", ty_str);
      if (strcmp(shape_str,"scalar"))
	xos << xml::Attr("rShape", shape_str);
      xos << EndElem;
    } else if (return_ty != (TY_IDX)0 && TY_kind(return_ty) != KIND_VOID) {
      // FunctionCall
      xlate_as = FUNCTION_CALL;
      // JU: for now: 
      USRCPOS srcpos;
      USRCPOS_srcpos(srcpos) = WN_Get_Linenum(wn);
      if(!USRCPOS_linenum(srcpos)) { 
	WN* parWN=ctxt.findParentWN(wn);
	if (parWN) { 
	  USRCPOS_srcpos(srcpos) = WN_Get_Linenum(parWN);
	  while (!USRCPOS_linenum(srcpos) && parWN) { 
	    parWN=ctxt.findParentWN(parWN);
	    USRCPOS_srcpos(srcpos) = WN_Get_Linenum(parWN);
	  }
	}
      }
      FORTTK_DIE("whirl2xaif::xlate_CALL: call to function: " 
		 << funcNm 
		 << " near line " 
		 << USRCPOS_linenum(srcpos)
		 << " is not supported! This should either be recognized as an intrinsic or should have been canonicalized into a subroutine call"); 
      // we leave the rest of the code as is...
      xos << BegElem("xaif:FunctionCall") 
	  << Attr("vertex_id", ctxt.currentXlationContext().getNewVertexId())
	  << Attr("scope_id", scopeid) << AttrSymId(st);
    } else {
      // std::cout << "last " << last_arg_idx << " first " << first_arg_idx << " impl " << total_implicit_args << std::endl;
      // SubroutineCall
      USRCPOS srcpos;
      USRCPOS_srcpos(srcpos) = WN_Get_Linenum(wn);
      xlate_as = SUBROUTINE_CALL; 
      xos << BegElem("xaif:SubroutineCall")
	  << Attr("statement_id", ctxt.findWNId(wn))
	  << Attr("scope_id", scopeid) 
	  << Attr("lineNumber",USRCPOS_linenum(srcpos))
	  << Attr("formalArgCount",last_arg_idx-first_arg_idx-total_implicit_args+1)
	  << AttrSymId(st);
    }
  }
  // Append the argument list to the function reference, skipping
  // implicit character-string-length arguments assumed to be the
  // last ones in the list (see also ST2F_func_header()).  Note
  // that we should not need to use any special-casing for 
  // ADRTMP or VALTMP OPR_INTRINSIC_OP nodes, as these should be
  // handled appropriately by TranslateWN().
  BOOL has_stat = FALSE;
  BOOL first_nonemptyarg = FALSE;
  INT implicit_args;
  UINT position = 0; // invalid position id
  UINT srcid = 0; // used for intrinsics (FIXME)
  for (INT arg_idx = first_arg_idx, implicit_args = 0; 
       arg_idx <= last_arg_idx - implicit_args; 
       arg_idx++) {
    position++; // need to account for optional parameters (when not present they are represented a NULL nodes)
    if (WN_kid(wn, arg_idx) != NULL) {
      WN* kidofparm = WN_kid0(WN_kid(wn, arg_idx));
      if (WN_operator(kidofparm) != OPR_CALL)
	arg_ty = WN_Tree_Type(WN_kid(wn, arg_idx));
      else
	arg_ty = PU_prototype (Pu_Table[ST_pu(WN_st(kidofparm))]);
      if (xlate_as == SUBROUTINE_CALL || xlate_as == FUNCTION_CALL) { 
	xos << BegElem("xaif:Argument");
	if (xlate_as == SUBROUTINE_CALL) { 
	  xos << Attr("position", position); 
	}
	if ((WN_kid(wn, arg_idx))->u3.ty_fields.ty) {  // hack for keyword call
	  xos << BegAttr("annotation") 
	      << SymIdAnnotVal((WN_kid(wn, arg_idx))->u3.ty_fields.ty) 
	      << EndAttr;
	}
	ctxt.createXlationContext(XlationContext::NOFLAG, kidofparm);// implicit for Argument
      }
      // FIXME
      if (opr == OPR_INTRINSIC_CALL &&
	  INTRN_by_value(WN_intrinsic(wn))) {
	/* Call-by value, but argument should be emitted without the
	 * %val() qualifier. */
	first_nonemptyarg = TRUE;
	srcid = ctxt.currentXlationContext().peekVertexId(); // used for intrinsics
	TranslateWN(xos, WN_kid(wn, arg_idx), ctxt);
      } 
      else if ((WN_operator(kidofparm) != OPR_CALL 
		&& (TY_Is_Character_Reference(arg_ty)  
		    || ((TY_Is_Pointer(arg_ty)
			 && TY_mtype(TY_pointed(arg_ty))==MTYPE_M)
			&& (TY_Is_Character_Reference(parm_ty) 
			    || TY_Is_Chararray_Reference(parm_ty)))) 
		|| WN_operator(kidofparm)==OPR_CALL 
		&& Func_Return_Character(arg_ty) )
	       && !is_allocate_stmt) {
	/* Handle substring arguments here.  These are always assumed
	 * to be passed by reference. For a function result, the length
	 * follows the address - does this look like char fn result?
	 * can't tell, but make good guess..
	 */
	INT len_idx;
	INT cur_idx = arg_idx;
	implicit_args++;
	if ((is_user_call) && (cur_idx == first_arg_idx) 
	    && (cur_idx == first_arg_idx) 
	    && (WN_kid_count(wn) >= cur_idx + 2) 
	    && (WN_kid(wn,cur_idx+1) != NULL) 
	    && (WN_Parm_By_Value(WN_kid(wn,cur_idx + 1))) 
	    && ((return_ty != 0) && (TY_kind(return_ty) == KIND_VOID))) {
	  len_idx = cur_idx + 1;
	} else {
	  len_idx = last_arg_idx - (total_implicit_args - implicit_args);
	}
	if ( !(first_nonemptyarg && !has_stat) )
	  has_stat = FALSE;
	first_nonemptyarg = TRUE;
	srcid=ctxt.currentXlationContext().peekVertexId();
	WN2F_String_Argument(xos, WN_kid(wn, cur_idx), /* string base */
			     WN_kid(wn, len_idx), /* string length */
			     ctxt);
      } 
      else if (!TY_Is_Pointer(arg_ty) 
	       || 
	       ((WN_operator(kidofparm) == OPR_INTRINSIC_OP
		 || 
		 WN_operator(kidofparm) == OPR_INTRINSIC_CALL)
		&&
		INTR_is_valtmp(WN_intrinsic(kidofparm)))
	       ||
	       WN_operator(kidofparm) == OPR_ARRAYEXP) {
	// Need to explicitly note this as a value parameter.
	if (WN_operator(kidofparm) == OPR_INTRINSIC_CALL &&
	    WN_intrinsic(kidofparm) == INTRN_CONCATEXPR)
	  implicit_args++; 
	  /*parser always generate an extra arg for concat operator*/
	if ( !(first_nonemptyarg && !has_stat) )
	  has_stat = FALSE;
	first_nonemptyarg = TRUE;
	srcid = ctxt.currentXlationContext().peekVertexId(); 
	TranslateWN(xos, WN_kid(wn, arg_idx), ctxt);
      } 
      else { /* TY_Is_Pointer(arg_ty) */
	/* There is also an implicit string length when the argument
	 * is an array of character strings. */
	if (TY_Is_Chararray_Reference(arg_ty) && !is_allocate_stmt)
	  implicit_args++;
	/* Assume call-by-reference parameter passing */
	if ( !(first_nonemptyarg && !has_stat) )
	  has_stat = FALSE;
	first_nonemptyarg = TRUE;
	srcid = ctxt.currentXlationContext().peekVertexId(); 
	WN* ctxtRefWN=0;
	if (ctxt.currentXlationContext().hasWN())
	  ctxtRefWN=ctxt.currentXlationContext().getWN();
	ctxt.currentXlationContext().setWN(WN_kid(wn, arg_idx));
	xlate_MemRef(xos, 
			   WN_kid(wn, arg_idx), /* address expression */
			   arg_ty,              /* address type */
			   TY_pointed(arg_ty),  /* object type */
			   0,                   /* offset from address */
			   ctxt);
	if (ctxtRefWN)
	  ctxt.currentXlationContext().setWN(ctxtRefWN);
      }
      if ((arg_idx+implicit_args) == (last_arg_idx-1)) { 
	if (opr == OPR_CALL && is_allocate_stmt) {
	  if (WN_operator(WN_kid0(WN_kid(wn, last_arg_idx))) == OPR_LDA) {
	    // xos << ",";
	    xos << "STAT=";
	    has_stat=TRUE;
	  } else {
	    arg_idx++;
	  }
	}
      }
      if (xlate_as == SUBROUTINE_CALL || xlate_as == FUNCTION_CALL) { 
        ctxt.deleteXlationContext(); // end Argument context
	xos << EndElem; // End Argument
      } else {
	// Intrinsic: create an edge
	DumpExprGraphEdge(xos, ctxt.currentXlationContext().getNewEdgeId(), srcid, targid, position);
      }
    } // non-null argument
  } // loop over arguments
  ctxt.currentXlationContext().unsetFlag(XlationContext::HAS_NO_ARR_ELMT);
  if (xlate_as == SUBROUTINE_CALL || xlate_as == FUNCTION_CALL) 
    xos << EndElem; 
} /* xlate_CALL */


void 
whirl2xaif::xlate_INTRINSIC_CALL(xml::ostream& xos, WN *wn,
				 PUXlationContext& ctxt)
{
  // Handles all intrinsics that are translated into XAIF statements

  FORTTK_ASSERT(WN_operator(wn) == OPR_INTRINSIC_CALL, 
		fortTkSupport::Diagnostics::UnexpectedInput); 

  WN   *arg_expr;
  TY_IDX arg_ty;
  INT  str_kid, length_kid, first_length_kid;
  BOOL regular_call = FALSE; /* Specially treated intrinsic call? */
  
  fortTkSupport::WNId wnid = ctxt.findWNId(wn);

  switch (WN_intrinsic(wn)) {
  case INTRN_CONCATEXPR:    
    /* In the context of an IO statement, emit the concatenation
     * but disregard the temporary result buffer. */
    
    /* Determine the range of kids denoting the base of the string-
     * arguments and the the length of these strings respectively. */
    str_kid = 1;
    length_kid = first_length_kid = (WN_kid_count(wn) + 2)/2;
    
    /* Emit the concatenation operations */
    WN2F_String_Argument(xos, 
			 WN_kid(wn, str_kid),    /* base of string1 */
			 WN_kid(wn, length_kid), /* length of string1 */
			 ctxt);
    while ((++str_kid) < first_length_kid) {
      length_kid++;
      xos << "//";
      WN2F_String_Argument(xos, 
			   WN_kid(wn, str_kid),    /* base of stringN */
			   WN_kid(wn, length_kid), /* length of stringN */
			   ctxt);
    }
    break;

  case INTRN_CASSIGNSTMT: 
    // string assignment
    // kid 0, 2: base of and length of destination; kid 1, 3: same, for source
    //WN2F_String_Argument(xos, WN_kid(wn,0), WN_kid(wn,2), ctxt);
    //WN2F_String_Argument(xos, WN_kid(wn,1), WN_kid(wn,3), ctxt);
    xos << BegElem(XAIFStrings.elem_Marker()) 
	<< Attr("statement_id", wnid)
	<< BegAttr("annotation") << WhirlIdAnnotVal(wnid)
	<< " [cassignstmt]" << EndAttr << EndElem;
    break;
    
  case INTRN_STOP:
  case INTRN_STOP_F90:
    // Either the F90 or F77 stop
    xos << BegElem(XAIFStrings.elem_Marker()) 
	<< Attr("statement_id", wnid)
	<< BegAttr("annotation") << WhirlIdAnnotVal(wnid)
	<< " [stop]" << EndAttr << EndElem;
    break;
    
  default:
    regular_call = TRUE;
    xlate_CALL(xos, wn, ctxt);
    break;
  }
  
  
} /* xlate_INTRINSIC_CALL */


//***************************************************************************
// Other Statements
//***************************************************************************

void
whirl2xaif::WN2F_eval(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  /* This generates code that will not recompile.  Short of
   * some kind of surrounding statement there is no way to do 
   * this in Fortran-77.
   */
  FORTTK_ASSERT(WN_operator(wn) == OPR_EVAL, fortTkSupport::Diagnostics::UnexpectedInput); 
  
  xos << "CALL _EVAL(";
  TranslateWN(xos, WN_kid0(wn), ctxt);
  xos << ')';
  
  
} /* WN2F_eval */


void
whirl2xaif::xlate_PRAGMA(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  FORTTK_ASSERT(WN_operator(wn) == OPR_PRAGMA ||
		WN_operator(wn) == OPR_XPRAGMA, fortTkSupport::Diagnostics::UnexpectedInput); 
  
  // switch (WN_pragma(apragma))
  fortTkSupport::WNId stmtid = ctxt.findWNId(wn);
  xos << BegElem(XAIFStrings.elem_Marker()) 
      << Attr("statement_id", stmtid)
      << BegAttr("annotation") << WhirlIdAnnotVal(stmtid)
      << " [pragma]" << EndAttr << EndElem;
  
  
}


void 
whirl2xaif::xlate_PREFETCH(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  FORTTK_ASSERT(WN_operator(wn) == OPR_PREFETCH ||
		WN_operator(wn) == OPR_PREFETCHX, fortTkSupport::Diagnostics::UnexpectedInput); 
  
  
}


void 
whirl2xaif::xlate_COMMENT(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  FORTTK_ASSERT(WN_operator(wn) == OPR_COMMENT, fortTkSupport::Diagnostics::UnexpectedInput); 
  
  // Note: Index_To_Str(WN_GetComment(wn)) returns comment text
  fortTkSupport::WNId stmtid = ctxt.findWNId(wn);
  xos << BegElem(XAIFStrings.elem_Marker()) 
      << Attr("statement_id", stmtid)
      << BegAttr("annotation") << WhirlIdAnnotVal(stmtid)
      << " [comment]" << EndAttr << EndElem;
  
  
}

void 
whirl2xaif::WN2F_dealloca(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  FORTTK_ASSERT(WN_operator(wn) == OPR_DEALLOCA, fortTkSupport::Diagnostics::UnexpectedInput); 

  INT16 n,i;
  n = WN_kid_count(wn);
  
  xos << std::endl << "CALL OPR_DEALLOCA(";
  i = 0 ;
  while (i < n) {
    TranslateWN(xos,WN_kid(wn,i),ctxt);
    if (++i < n)
      xos << ",";
  }
  xos << ")";
   
  
} /* WN2F_dealloca */


void
whirl2xaif::xlate_USE(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  FORTTK_ASSERT(WN_operator(wn) == OPR_USE, fortTkSupport::Diagnostics::UnexpectedInput); 
  
  fortTkSupport::WNId stmtid = ctxt.findWNId(wn);
  const char* nm = ST_name(WN_st(wn));
  xos << BegElem(XAIFStrings.elem_Marker()) 
      << Attr("statement_id", stmtid)
      << BegAttr("annotation") << WhirlIdAnnotVal(stmtid)
      << " [use " << nm << "]" << EndAttr << EndElem;
  
  
}


void
whirl2xaif::WN2F_namelist_stmt(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  FORTTK_ASSERT(WN_operator(wn) == OPR_NAMELIST, fortTkSupport::Diagnostics::UnexpectedInput); 
  
  const char *st_name =  ST_name(WN_st(wn));
  if (ST_is_external(WN_st(wn))) {
    ;
  } else {
    xos << "NAMELIST /" << st_name << " /";
    int k ;
    for(k=0;k< WN_kid_count(wn);k++ ) {
      st_name = ST_name(WN_st(WN_kid(wn,k)));
      Set_BE_ST_w2fc_referenced(WN_st(WN_kid(wn,k)));
      if (k==0)
	;
      else
	xos << ",";
      xos << st_name;
    }
  }
  
  
} //WN2F_namelist_stmt


void
whirl2xaif::WN2F_implicit_bnd(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  xos << "[+-+]";
  
}


void
whirl2xaif::WN2F_nullify_stmt(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  FORTTK_ASSERT(WN_operator(wn) == OPR_NULLIFY, fortTkSupport::Diagnostics::UnexpectedInput); 

  int k ;  
  const char *st_name;

  xos << "NULLIFY (";
  for(k=0;k< WN_kid_count(wn);k++ ) {
    st_name = ST_name(WN_st(WN_kid(wn,k)));
    Set_BE_ST_w2fc_referenced(WN_st(WN_kid(wn,k)));
    if (k!=0)
      xos << ",";
    xos << st_name;
    
  }
  xos << ")";
  
  
} //WN2F_namelist_stmt


void
whirl2xaif::WN2F_interface_blk(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  FORTTK_ASSERT(WN_operator(wn) == OPR_INTERFACE, fortTkSupport::Diagnostics::UnexpectedInput); 

  int           k ;
  ST            **param_st;
  ST            *st = WN_st(wn);
  ST            *rslt = NULL;
  INT           param,num_params;
  INT           first_param;
  TY_IDX        return_ty;
  INT           implicit = 0 ;
  
  const char *intface_name = ST_name(st);
  if (ST_is_external(WN_st(wn))) {
    ;
  } else {
   xos << std::endl;
    xos << "interface ";
    
    if (ST_is_assign_interface(st)) {
      xos << "assignment (";
    }
    
    if (ST_is_operator_interface(st) || ST_is_u_operator_interface(st)){
      xos << "operator (";
    }
    
    if (ST_is_u_operator_interface(st)) 
      xos << ".";

#if 0    
static const char unnamed_interface[] = "unnamed interface"; 
    if (strcmp(intface_name,unnamed_interface)) 
      Append_Token_String(xos, intface_name);
#endif
    
    if (ST_is_u_operator_interface(st))
      xos << ".";
    
    if (ST_is_assign_interface(st) ||
	ST_is_operator_interface(st) ||
	ST_is_u_operator_interface(st))
      xos << ")";
    
    xos << '\n';
    
    for(k=0;k< WN_kid_count(wn);k++ ) { // each kid is OPR_FUNC_ENTRY wn
      num_params = WN_kid_count(WN_kid(wn,k));
      param_st = (ST **)alloca((num_params + 1) * sizeof(ST *));
      for (param = 0; param < num_params; param++) {
	param_st[param] = WN_st(WN_formal(WN_kid(wn,k), param));
      }
      
      st = &St_Table[WN_entry_name(WN_kid(wn,k))];
      TY_IDX       funtype = ST_pu_type(st);
      
      return_ty = Func_Return_Type(funtype);
      
      if (ST_is_in_module(st) ) {
	xos << "module procedure ";
	Append_Token_String(xos, ST_name(st));     
      } else {
	if (return_ty != (TY_IDX) 0 && TY_kind(return_ty) != KIND_VOID) {
          xos << "FUNCTION";
	  
          if (PU_recursive(Get_Current_PU())) {
	    //Prepend_Token_String(xos, "RECURSIVE");
	    xos << "RECURSIVE";
	  }
	  
	  /* Note that we cannot have functions returning pointer types
	   * in Fortran, so we use the corresponding integral type
	   * instead.
	   */
          if (TY_Is_Pointer(return_ty))
	    TY2F_translate(xos, Stab_Mtype_To_Ty(TY_mtype(return_ty)), ctxt);
          else {
	    if (TY_kind(return_ty)==KIND_ARRAY && !TY_is_character(return_ty))
	      TY2F_translate(xos,TY_AR_etype(return_ty), ctxt);
	    else
	      TY2F_translate(xos, return_ty, ctxt);
	  }
	  
	} else { /* subroutine */
	  xos << "SUBROUTINE";
	}
	
	Append_Token_String(xos, ST_name(st));
	
	/* Emit the parameter name-list, if one is present, and skip any
	 * implicit "length" parameters associated with character strings.
	 * Such implicit parameters should be at the end of the parameter list.
	 */
	
	first_param = ST2F_FIRST_PARAM_IDX(funtype);
	BOOL isFirstArg = TRUE; /* become FALSE after first argument has been emitted */
	/* (radu@par.univie.ac.at) */
	if (param_st[first_param] != NULL) {
	  xos << "(";
	  for (param = first_param;
	       param < num_params;
	       param++)
	    {
	      if (implicit){
		param_st[param] = NULL;
		implicit = 0;
	      } else {
		if (STAB_PARAM_HAS_IMPLICIT_LENGTH(param_st[param])) 
		  implicit = 1;
		if (!ST_is_return_var(param_st[param])) {
		  /* separate argument with a comma, if not the first one */
		  /* (radu@par.univie.ac.at) */
		  if (isFirstArg == FALSE)
		    xos << ",";
		  else
		    isFirstArg = FALSE;
		  Append_Token_String(xos,
				      ST_name(param_st[param]));
		  
		  /* Bug: next and last param may be implicit */
		  /* this causes the argument list to end with a comma (radu@par.univie.ac.at) */
		  /* if (param+1 < num_params) */
		  /*     xos << ","; */
                   } else
		     rslt = param_st[param];
		
	      }
	    }
	  xos << ")";
	} else {
          /* Use the "()" notation for "no parameters" */
	  xos << "()";
	}
	
	if (rslt !=NULL && strcasecmp(ST_name(st), 
				      ST_name(rslt)) != 0) {
	  /* append the RESULT option only if it is different from the function name */
	  /* (radu@par.univie.ac.at) */
	  xos << "result(";
	  Append_Token_String(xos, ST_name(rslt));
	  xos << ")";
	}
	/* Emit parameter declarations, indented and on a new line */
	
	for (param = first_param; param < num_params ; param++)
	  
	  if (param_st[param] != NULL) {
	    xos << std::endl;
	    TranslateSTDecl(xos, param_st[param], ctxt);
	    if (ST_is_optional_argument(param_st[param])) {
	      xos << std::endl;
	      xos << "OPTIONAL ";
	      Append_Token_String(xos, ST_name(param_st[param]));
	    }
	    if (ST_is_intent_in_argument(param_st[param])) {
	      xos << std::endl;
	      xos << "INTENT(in) ";
	      Append_Token_String(xos, ST_name(param_st[param]));
	    }
	    if (ST_is_intent_out_argument(param_st[param])) {
	      xos << std::endl;
	      xos << "INTENT(out) ";
	      Append_Token_String(xos, ST_name(param_st[param]));
	    }
	  }
	
	xos << "\n";
	xos << std::endl;	
	
	if (return_ty != (TY_IDX) 0 && TY_kind(return_ty) != KIND_VOID)
	  xos << "END FUNCTION";
	else /* subroutine */
	  xos << "END SUBROUTINE";
      }
      
      xos << "\n";
    }

    xos << std::endl;    
    xos << "end interface ";
    xos << std::endl;    
  }
  
  
} //WN2F_interface_blk

void
whirl2xaif::WN2F_ar_construct(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  xos << "(/";
  for (INT kid = 0; kid < WN_kid_count(wn); kid++) {
    TranslateWN(xos,WN_kid(wn,kid), ctxt);
    if (kid < WN_kid_count(wn)-1)
      xos << ",";
  }
  xos << "/)";
  
  
}

