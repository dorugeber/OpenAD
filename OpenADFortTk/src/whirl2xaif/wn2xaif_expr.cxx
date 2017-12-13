// ##########################################################
// # This file is part of OpenADFortTk.                     #
// # The full COPYRIGHT notice can be found in the top      #
// # level directory of the OpenADFortTk source tree.       #
// # For more information visit                             #
// # http://www.mcs.anl.gov/openad                          #
// ##########################################################

#include "Open64IRInterface/Open64BasicTypes.h"
#include "Open64IRInterface/IntrinsicInfo.h"

#include "wn2xaif.h"
#include "wn2xaif_expr.h"
#include "wn2xaif_mem.h"
#include "ty2xaif.h"
#include "st2xaif.h"

using namespace whirl2xaif;
using namespace xml; // for xml::ostream, etc

static void 
xlate_UnaryOpUsingIntrinsicTable(xml::ostream& xos, OPCODE opcode, 
				 TY_IDX result_ty,
				 WN* wn, PUXlationContext& ctxt);

static void 
xlate_BinaryOpUsingIntrinsicTable(xml::ostream& xos, OPCODE opcode, 
				  TY_IDX result_ty,
				  WN *wn0, WN *wn1, PUXlationContext& ctxt);

static void 
xlate_Operand(xml::ostream& xos, WN *opnd, TY_IDX assumed_ty, BOOL callByValue,
	      PUXlationContext& ctxt);

//***************************************************************************

/*------------------------- Value Conversions -------------------------*/
/*---------------------------------------------------------------------*/

/* Create a mapping from a pair of MTYPEs to the Fortran intrinsic
 * or builtin operation that carries out the conversion.  NULL means
 * that either the conversion is redundant and can be ignored or there
 * is no way we can do it.
 */
static const char *Conv_Op[MTYPE_LAST+1][MTYPE_LAST+1];
static bool Conv_OpInitialized = false;

typedef struct Conv_Op
{
   MTYPE       from, to;
   const char *name;
} CONV_OP;

#define NUMBER_OF_CONV_OPS sizeof(Conv_Op_Map)/sizeof(CONV_OP)

static const CONV_OP Conv_Op_Map[] = 
{
   /* from   |   to   |   op-name */

   /* Only consider conversion to ptr sized unsigned numbers 
    * valid in Fortran.
    */
   {MTYPE_I1, MTYPE_U4, "C_JZEXT"},
   {MTYPE_I2, MTYPE_U4, "C_JZEXT"},
   {MTYPE_I4, MTYPE_U4, "C_JZEXT"},
   {MTYPE_I8, MTYPE_U4, "C_JZEXT"},
   /*{MTYPE_U1, MTYPE_U4, ""},*/
   /*{MTYPE_U2, MTYPE_U4, ""},*/
   /*{MTYPE_U4, MTYPE_U4, ""},*/
   {MTYPE_U8, MTYPE_U4, "C_JZEXT"},

   {MTYPE_I1, MTYPE_U8, "C_KZEXT"},
   {MTYPE_I2, MTYPE_U8, "C_KZEXT"},
   {MTYPE_I4, MTYPE_U8, "C_KZEXT"},
   {MTYPE_I8, MTYPE_U8, "C_KZEXT"},
   /*{MTYPE_U1, MTYPE_U8, ""},*/
   /*{MTYPE_U2, MTYPE_U8, ""},*/
   /*{MTYPE_U4, MTYPE_U8, ""},*/
   /*{MTYPE_U8, MTYPE_U8, ""},*/

   /*{MTYPE_I1, MTYPE_I1, ""},*/
   {MTYPE_I2, MTYPE_I1, "C_INT1"},
   {MTYPE_I4, MTYPE_I1, "C_INT1"},
   {MTYPE_I8, MTYPE_I1, "C_INT1"},
   /*{MTYPE_U1, MTYPE_I1, ""},*/
   {MTYPE_U2, MTYPE_I1, "C_INT1"},
   {MTYPE_U4, MTYPE_I1, "C_INT1"},
   {MTYPE_U8, MTYPE_I1, "C_INT1"},
   {MTYPE_F4, MTYPE_I1, "C_INT1"},
   {MTYPE_F8, MTYPE_I1, "C_INT1"},
   {MTYPE_FQ, MTYPE_I1, "C_INT1"},

   {MTYPE_I1, MTYPE_I2, "C_INT2"},
   /*{MTYPE_I2, MTYPE_I2, ""},*/
   {MTYPE_I4, MTYPE_I2, "C_INT2"},
   {MTYPE_I8, MTYPE_I2, "C_INT2"},
   {MTYPE_U1, MTYPE_I2, "C_INT2"},
   /*{MTYPE_U2, MTYPE_I2, ""},*/
   {MTYPE_U4, MTYPE_I2, "C_INT2"},
   {MTYPE_U8, MTYPE_I2, "C_INT2"},
   {MTYPE_F4, MTYPE_I2, "C_INT2"},
   {MTYPE_F8, MTYPE_I2, "C_INT2"},
   {MTYPE_FQ, MTYPE_I2, "C_INT2"},

   {MTYPE_I1, MTYPE_I4, "C_INT"},
   {MTYPE_I2, MTYPE_I4, "C_INT"},
   /*{MTYPE_I4, MTYPE_I4, ""},*/
   {MTYPE_I8, MTYPE_I4, "C_INT"},
   {MTYPE_U1, MTYPE_I4, "C_INT"},
   {MTYPE_U2, MTYPE_I4, "C_INT"},
   /*{MTYPE_U4, MTYPE_I4, ""},*/
   {MTYPE_U8, MTYPE_I4, "C_INT"},
   {MTYPE_F4, MTYPE_I4, "C_INT"},
   {MTYPE_F8, MTYPE_I4, "C_INT"},
   {MTYPE_FQ, MTYPE_I4, "C_INT"},


   {MTYPE_I1, MTYPE_I8, "C_INT"},
   {MTYPE_I2, MTYPE_I8, "C_INT"},
   {MTYPE_I4, MTYPE_I8, "C_INT"},
   /*{MTYPE_I8, MTYPE_I8, ""},*/
   {MTYPE_U1, MTYPE_I8, "C_INT"},
   {MTYPE_U2, MTYPE_I8, "C_INT"},
   {MTYPE_U4, MTYPE_I8, "C_INT"},
   /*{MTYPE_U8, MTYPE_I8, ""},*/
   {MTYPE_F4, MTYPE_I8, "C_INT"},
   {MTYPE_F8, MTYPE_I8, "C_INT"},
   {MTYPE_FQ, MTYPE_I8, "C_INT"},

   {MTYPE_I1, MTYPE_F4, "C_REAL"},
   {MTYPE_I2, MTYPE_F4, "C_REAL"},
   {MTYPE_I4, MTYPE_F4, "C_REAL"},
   {MTYPE_I8, MTYPE_F4, "C_REAL"},
   {MTYPE_U1, MTYPE_F4, "C_REAL"},
   {MTYPE_U2, MTYPE_F4, "C_REAL"},
   {MTYPE_U4, MTYPE_F4, "C_REAL"},
   {MTYPE_U8, MTYPE_F4, "C_REAL"},
   /*{MTYPE_F4, MTYPE_F4, ""},*/
   {MTYPE_F8, MTYPE_F4, "C_REAL"},
   {MTYPE_FQ, MTYPE_F4, "C_REAL"},

   {MTYPE_I1, MTYPE_F8, "C_DBLE"},
   {MTYPE_I2, MTYPE_F8, "C_DBLE"},
   {MTYPE_I4, MTYPE_F8, "C_DBLE"},
   {MTYPE_I8, MTYPE_F8, "C_DBLE"},
   {MTYPE_U1, MTYPE_F8, "C_DBLE"},
   {MTYPE_U2, MTYPE_F8, "C_DBLE"},
   {MTYPE_U4, MTYPE_F8, "C_DBLE"},
   {MTYPE_U8, MTYPE_F8, "C_DBLE"},
   {MTYPE_F4, MTYPE_F8, "C_DBLE"},
   /*{MTYPE_F8, MTYPE_F8, ""},*/
   {MTYPE_FQ, MTYPE_F8, "C_DBLE"},

   {MTYPE_I1, MTYPE_FQ, "C_QREAL"},
   {MTYPE_I2, MTYPE_FQ, "C_QREAL"},
   {MTYPE_I4, MTYPE_FQ, "C_QREAL"},
   {MTYPE_I8, MTYPE_FQ, "C_QREAL"},
   {MTYPE_U1, MTYPE_FQ, "C_QREAL"},
   {MTYPE_U2, MTYPE_FQ, "C_QREAL"},
   {MTYPE_U4, MTYPE_FQ, "C_QREAL"},
   {MTYPE_U8, MTYPE_FQ, "C_QREAL"},
   {MTYPE_F4, MTYPE_FQ, "C_QREAL"},
   {MTYPE_F8, MTYPE_FQ, "C_QREAL"}
   /*{MTYPE_FQ, MTYPE_FQ, ""}*/
}; /* Conv_Op_Map */

static void 
WN2F_Expr_initialize(void)
{
  /* Initialize the Conv_Op array (default value is NULL) */
  for (UINT i = 0; i < NUMBER_OF_CONV_OPS; i++) {
    Conv_Op[Conv_Op_Map[i].from][Conv_Op_Map[i].to] = 
      Conv_Op_Map[i].name;
  }
}

static void
WN2F_Convert(xml::ostream& xos, MTYPE from_mtype, MTYPE to_mtype)
{
   /* We emit a warning message for conversions not covered (TODO: put
    * this warning under a command-line option).  Converts the expression
    * in the given token-buffer to the given mtype.
    */
  xos << "("; // FIXMEprepend
  if (Conv_Op[from_mtype][to_mtype] == NULL) {
    FORTTK_ASSERT_WARN(Conv_Op[from_mtype][to_mtype] != NULL,
      "unexpected conversion from " << MTYPE_name(from_mtype) << "to" 
      << MTYPE_name(to_mtype));
    xos << "WN2F_Convert"; // FIXMEprepend
  } else {
    /* Note all these are intrinsics in the mongoose compiler and
     * need not be declared.
     */
    Append_Token_String(xos, Conv_Op[from_mtype][to_mtype]); // FIXMEprepend
  }
  xos << ")";
} /* WN2F_Convert */


//***************************************************************************
// Type Conversion
//***************************************************************************

static void 
InitConvOpMap() // FIXME
{
  if (!Conv_OpInitialized) {
    WN2F_Expr_initialize();
    Conv_OpInitialized = true;
  }
}

void 
whirl2xaif::WN2F_cvt(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  FORTTK_ASSERT(WN_operator(wn) == OPR_CVT, fortTkSupport::Diagnostics::UnexpectedInput); 

  InitConvOpMap();
  TranslateWN(xos, WN_kid0(wn), ctxt);

/*  Maybe we shouldn't or needn't  explicitly output these kinds of 
    convert in .w2f.f file----fzhao
*/
//   WN2F_Convert(xos, WN_desc(wn), WN_rtype(wn));

  
}

void 
whirl2xaif::WN2F_cvtl(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  FORTTK_ASSERT(WN_operator(wn) == OPR_CVTL, fortTkSupport::Diagnostics::UnexpectedInput); 

  TY_IDX  rtype, dtype;
  InitConvOpMap();
  
  dtype = WN_Tree_Type(WN_kid0(wn));
  rtype = WN_Tree_Type(wn);
  
  /* Only convert if it is necessary */
  if (Conv_Op[TY_mtype(dtype)][TY_mtype(rtype)] != NULL ) {
    TranslateWN(xos, WN_kid0(wn), ctxt);
    WN2F_Convert(xos, TY_mtype(dtype), TY_mtype(rtype));
  } else {
    TranslateWN(xos, WN_kid0(wn), ctxt);
  }
  
}

void 
whirl2xaif::WN2F_tas(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  FORTTK_ASSERT(WN_operator(wn) == OPR_TAS, fortTkSupport::Diagnostics::UnexpectedInput); 
  
  // Just ignore TAS operators for now.  TODO: make sure this
  // is always ok.
  return TranslateWN(xos, WN_kid0(wn), ctxt);
}


//***************************************************************************
// Leaf (Other)
//***************************************************************************

// xlate_INTCONST: Translate a WHIRL integer constant into an XAIF
// constant.
void 
whirl2xaif::xlate_INTCONST(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  FORTTK_ASSERT(WN_operator(wn) == OPR_INTCONST, fortTkSupport::Diagnostics::UnexpectedInput); 
  
  // FIXME: use xlate_CONST
  TCON tval = Host_To_Targ(WN_rtype(wn), WN_const_val(wn));
  bool logical = ctxt.currentXlationContext().isFlag(XlationContext::IS_LOGICAL_ARG);
  std::string val = TCON2F_translate(tval, logical);  
  const char* ty_str = (logical) ? "bool" : "integer";

  xos << BegElem("xaif:Constant") 
      << Attr("vertex_id", ctxt.currentXlationContext().getNewVertexId()) 
      << Attr("type", ty_str) 
      << Attr("feType",Mtype_Name(OPCODE_rtype(WN_opcode(wn))))
      << Attr("value", val) 
      << EndElem;
}

// xlate_CONST: Translate a WHIRL constant (string, floating point,
// etc.) into an XAIF constant.
void 
whirl2xaif::xlate_CONST(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  FORTTK_ASSERT(WN_operator(wn) == OPR_CONST, fortTkSupport::Diagnostics::UnexpectedInput); 

  TY_IDX ty_idx = ST_type(WN_st(wn));
  
  BOOL logical = (TY_is_logical(ty_idx) || ctxt.currentXlationContext().isFlag(XlationContext::IS_LOGICAL_ARG));
  std::string val = TCON2F_translate(STC_val(WN_st(wn)), logical);

  const char* ty_str = TranslateTYToSymType(ty_idx); // FIXME: logical
  if (!ty_str) { ty_str = "***"; }  

  xos << BegElem("xaif:Constant") 
      << Attr("vertex_id", ctxt.currentXlationContext().getNewVertexId())
      << Attr("type", ty_str) 
      << Attr("feType",Mtype_Name(TY_mtype(ty_idx)))
      << Attr("value", val) 
      << EndElem;
}


//***************************************************************************
// Expression Operators: Unary Operations
//***************************************************************************

void
whirl2xaif::xlate_UnaryOp(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  OPERATOR opr = WN_operator(wn);
  OPCODE opc = WN_opcode(wn);
  FORTTK_ASSERT(WN_kid_count(wn) == 1, 
		fortTkSupport::Diagnostics::UnexpectedInput << OPERATOR_name(opr));
  xlate_UnaryOpUsingIntrinsicTable(xos, 
				   opc, 
				   WN_Tree_Type(wn), 
				   WN_kid0(wn), 
				   ctxt);
}


void 
whirl2xaif::WN2F_rsqrt(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  FORTTK_DIE(fortTkSupport::Diagnostics::Unimplemented);
  
  TY_IDX const result_ty = Stab_Mtype_To_Ty(WN_rtype(wn));
  
  xos << "(1.0/SQRT(";
  xlate_Operand(xos, WN_kid(wn,0), result_ty,
		!TY_Is_Character_Reference(result_ty), ctxt);
  xos << "))";
  
  
} /* WN2F_rsqrt */


void 
whirl2xaif::WN2F_realpart(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  FORTTK_DIE(fortTkSupport::Diagnostics::Unimplemented);
   
  switch (WN_rtype(wn)) {
  case MTYPE_F4:
    xos << "REAL";
    break;
  case MTYPE_F8:
    xos << "DBLE";
    break;
  case MTYPE_FQ:
    xos << "QREAL";
    
    break;
  default:
    FORTTK_DIE("Unexpected type: " << MTYPE_name(WN_rtype(wn)));
    break;
  }
  xos << "(";
  TranslateWN(xos, WN_kid0(wn), ctxt);
  xos << ")";
  
  
} /* WN2F_realpart */


void 
whirl2xaif::WN2F_imagpart(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  FORTTK_DIE(fortTkSupport::Diagnostics::Unimplemented);
  
  switch (WN_rtype(wn)) {
  case MTYPE_F4:
    xos << "AIMAG";
    break;
  case MTYPE_F8:
    xos << "DIMAG";
    break;
  case MTYPE_FQ:
    xos << "QIMAG";
    break;
  default:
    FORTTK_DIE("Unexpected type: " << MTYPE_name(WN_rtype(wn)));
    break;
  }
  xos << "(imagpart";
  TranslateWN(xos, WN_kid0(wn), ctxt);
  xos << "imagpart)";
  
  
} /* WN2F_imagpart */


void 
whirl2xaif::xlate_PAREN(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  FORTTK_ASSERT(WN_operator(wn) == OPR_PAREN, fortTkSupport::Diagnostics::UnexpectedInput);

  return TranslateWN(xos, WN_kid0(wn), ctxt);
}


void 
whirl2xaif::xlate_RECIP(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  FORTTK_ASSERT(WN_operator(wn) == OPR_RECIP, fortTkSupport::Diagnostics::UnexpectedInput);

   const TY_IDX result_ty = Stab_Mtype_To_Ty(WN_rtype(wn));
   
   // Translate using a temporary DIV expression [1 / kid0(wn)]
   TYPE_ID rty = TY_mtype(result_ty);
   OPCODE opc = OPCODE_make_op(OPR_DIV, rty, MTYPE_V);

   TCON tcon;
   if (MTYPE_is_integral(rty)) {
     tcon = Host_To_Targ(rty, 1);
   } 
   else if (MTYPE_is_float(rty)) {
     tcon = Host_To_Targ_Float(rty, 1.0);
   } 
   else if (MTYPE_is_complex(rty)) { 
     tcon = Host_To_Targ_Complex(rty, 1, 0);
   } 
   else {
     FORTTK_DIE("Cannot take reciprical of this TCON");
   }
   WN* wn_one = Make_Const(tcon);
   
   xlate_BinaryOpUsingIntrinsicTable(xos, opc, result_ty, wn_one, 
				     WN_kid0(wn), ctxt);
   
   WN_DELETE_Tree(wn_one);
   
   
}


void 
whirl2xaif::WN2F_parm(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  /* TODO: handle opcode parms properly, i.e. take some advantage
   * of the information provided in this packaging of argument 
   * expressions.  For now, just skip these nodes.
   */
  FORTTK_ASSERT(WN_operator(wn) == OPR_PARM, fortTkSupport::Diagnostics::UnexpectedInput);
  if (TY_is_logical(Ty_Table[WN_ty(wn)]) || 
      ctxt.currentXlationContext().isFlag(XlationContext::IS_LOGICAL_ARG)) { //fzhao Jan
    ctxt.currentXlationContext().setFlag(XlationContext::HAS_LOGICAL_ARG);
    TranslateWN(xos, WN_kid0(wn), ctxt);
    ctxt.currentXlationContext().unsetFlag(XlationContext::HAS_LOGICAL_ARG);
  } 
  else {
    TranslateWN(xos, WN_kid0(wn), ctxt);
  }
  

} /* WN2F_parm */


void 
whirl2xaif::WN2F_alloca(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  FORTTK_DIE(fortTkSupport::Diagnostics::Unimplemented);
  
  xos << "OPR_ALLOCA(";
  TranslateWN(xos,WN_kid0(wn),ctxt);
  xos << ")";
  
  
} /* WN2F_alloca */


//***************************************************************************
// Expression Operators: Binary Operations
//***************************************************************************

void
whirl2xaif::xlate_BinaryOp(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  OPERATOR opr = WN_operator(wn);
  OPCODE opc = WN_opcode(wn);
  FORTTK_ASSERT(WN_kid_count(wn) == 2, 
		fortTkSupport::Diagnostics::UnexpectedInput << OPERATOR_name(opr));
  xlate_BinaryOpUsingIntrinsicTable(xos, 
				    opc, 
				    WN_GetExprType(wn), 
				    WN_kid0(wn), 
				    WN_kid1(wn), 
				    ctxt);
  ctxt.currentXlationContext().unsetFlag(XlationContext::IS_LOGICAL_OPERATION);
}

void 
whirl2xaif::WN2F_bnor(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  FORTTK_DIE(fortTkSupport::Diagnostics::Unimplemented);

  TY_IDX const result_ty = Stab_Mtype_To_Ty(WN_rtype(wn));
  
  xos << "NOT(IOR(";
  xlate_Operand(xos, WN_kid(wn,0), result_ty,
		!TY_Is_Character_Reference(result_ty), ctxt);
  xos << ",";
  xlate_Operand(xos, WN_kid(wn,1), result_ty,
		!TY_Is_Character_Reference(result_ty), ctxt);
  xos << "))";
  
  
} /* WN2F_bnor */


void 
whirl2xaif::WN2F_lshr(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  FORTTK_DIE(fortTkSupport::Diagnostics::Unimplemented);
  
  TY_IDX const result_ty = Stab_Mtype_To_Ty(WN_rtype(wn));
  
  xos << "ISHIFT(";
  xlate_Operand(xos, WN_kid(wn,0), result_ty,
		!TY_Is_Character_Reference(result_ty), ctxt);
  xos << ",-(";
  xlate_Operand(xos, WN_kid(wn,1), result_ty,
		!TY_Is_Character_Reference(result_ty), ctxt);
  xos << "))";
  
  
} /* WN2F_lshr */


//***************************************************************************
// Expression Operators: Ternary Operations; N-ary Operations
//***************************************************************************

void 
whirl2xaif::WN2F_select(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  /* SELECT is almost the same as the F90 MERGE intrinsic, 
     so I will output it that way for now */
  FORTTK_DIE(fortTkSupport::Diagnostics::Unimplemented);
  
  xos << "MERGE(";
  TranslateWN(xos, WN_kid1(wn), ctxt);
  xos << ",";  
  TranslateWN(xos, WN_kid2(wn), ctxt);
  xos << ",";  
  TranslateWN(xos, WN_kid0(wn), ctxt);  
  xos << ")";

  
} /* WN2F_select */


void 
whirl2xaif::WN2F_madd(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  FORTTK_ASSERT(WN_operator(wn) == OPR_MADD, fortTkSupport::Diagnostics::UnexpectedInput);

  TY_IDX const result_ty = Stab_Mtype_To_Ty(WN_rtype(wn));
  
  xos << "(";
  xlate_Operand(xos, WN_kid(wn,1), result_ty,
		!TY_Is_Character_Reference(result_ty), ctxt);
  xos << "*";
  xlate_Operand(xos, WN_kid(wn,2), result_ty,
		!TY_Is_Character_Reference(result_ty),ctxt);
  xos << "+";
  xlate_Operand(xos, WN_kid(wn,0), result_ty,
		!TY_Is_Character_Reference(result_ty), ctxt);
  xos << ")";
  
  
} /* WN2F_madd */


void 
whirl2xaif::WN2F_msub(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  FORTTK_ASSERT(WN_operator(wn) == OPR_MSUB, fortTkSupport::Diagnostics::UnexpectedInput);

  TY_IDX const result_ty = Stab_Mtype_To_Ty(WN_rtype(wn));
  
  xos << "(";
  xlate_Operand(xos, WN_kid(wn,1), result_ty,
		!TY_Is_Character_Reference(result_ty), ctxt);
  xos << "*";
  xlate_Operand(xos, WN_kid(wn,2), result_ty,
		!TY_Is_Character_Reference(result_ty), ctxt);
  xos << "-";
  xlate_Operand(xos, WN_kid(wn, 0), result_ty,
		!TY_Is_Character_Reference(result_ty), ctxt);
  xos << ")";
  
  
} /* WN2F_msub */


void 
whirl2xaif::WN2F_nmadd(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  FORTTK_ASSERT(WN_operator(wn) == OPR_NMADD, fortTkSupport::Diagnostics::UnexpectedInput);

  TY_IDX const result_ty = Stab_Mtype_To_Ty(WN_rtype(wn));
  
  xos << "-(";
  xlate_Operand(xos, WN_kid(wn,1), result_ty,
		!TY_Is_Character_Reference(result_ty), ctxt);
  xos << "*";
  xlate_Operand(xos, WN_kid(wn,2), result_ty,
		!TY_Is_Character_Reference(result_ty), ctxt);
  xos << "+";
  xlate_Operand(xos, WN_kid(wn, 0), result_ty,
		!TY_Is_Character_Reference(result_ty), ctxt);
  xos << ")";
  
  
} /* WN2F_nmadd */


void 
whirl2xaif::WN2F_nmsub(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  FORTTK_ASSERT(WN_operator(wn) == OPR_NMSUB, fortTkSupport::Diagnostics::UnexpectedInput);
  
  TY_IDX const result_ty = Stab_Mtype_To_Ty(WN_rtype(wn));
  
  xos << "-(";
  xlate_Operand(xos, WN_kid(wn,1), result_ty,
		!TY_Is_Character_Reference(result_ty), ctxt);
  xos << "*";
  xlate_Operand(xos, WN_kid(wn,2), result_ty,
		!TY_Is_Character_Reference(result_ty), ctxt);
  xos << "-";
  xlate_Operand(xos, WN_kid(wn, 0), result_ty,
		!TY_Is_Character_Reference(result_ty), ctxt);
  xos << ")";
  
  
} /* WN2F_nmsub */


//***************************************************************************
// Expression Operators: N-ary Operations
//***************************************************************************

static void 
WN2F_Intr_Funcall(xml::ostream& xos, WN* wn, fortTkSupport::IntrinsicXlationTable::XAIFInfoPair& infoPair,
		  INT begArgIdx, INT endArgIdx, BOOL callByValue, 
		  PUXlationContext& ctxt);

void 
whirl2xaif::xlate_INTRINSIC_OP(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  // An intrinsic operator expression (function call). This call is
  // not related to the call-info generated by PUinfo.  Note that
  // either all or none of the arguments are call-by-value.
  OPERATOR opr = WN_operator(wn);
  FORTTK_ASSERT(opr == OPR_INTRINSIC_OP, fortTkSupport::Diagnostics::UnexpectedInput);
  
  INTRINSIC intrn = WN_intrinsic(wn);
  BOOL by_value = INTRN_by_value(intrn);
  INT begArgIdx = 0; // Assume we never return to first argument
  INT endArgIdx = WN_kid_count(wn) - 1;
  
  const char* inm = IntrinsicInfo::intrinsicBaseName(intrn);
  fortTkSupport::IntrinsicXlationTable::XAIFInfoPair infoPair(Whirl2Xaif::getIntrinsicXlationTable().findXAIFInfo(opr, inm));
  if ((strcmp(inm, "ADRTMP") == 0) || (strcmp(inm, "VALTMP") == 0)) {
    // Special cases:
    //   ADRTMP: Call-by-reference.  Emit the dereferenced parameter.
    //   VALTMP: Call-by-value.  Assume 'ctxt' determines when it
    //     is necessary to put a %val qualifier around the argument.
    TranslateWN(xos, WN_kid0(wn), ctxt);
  } else {
    // General case
    WN2F_Intr_Funcall(xos, wn, infoPair, 
		      begArgIdx, endArgIdx, by_value, ctxt);
  }
  
  
} /* xlate_INTRINSIC_OP */


static void 
WN2F_Intr_Funcall(xml::ostream& xos, WN* wn, fortTkSupport::IntrinsicXlationTable::XAIFInfoPair& infoPair,
		  INT begArgIdx, INT endArgIdx, BOOL callByValue, 
		  PUXlationContext& ctxt)
{
  /* An intrinsic operator expression to be emitted with function
   * call syntax.  All arguments are passed by value or by reference,
   * i.e. we never have some arguments passed by value and some by 
   * reference, unless we have explicit INTR_OPC_ADRTMP or 
   * INTR_OPC_VALTMP argument expressions. Note that we also
   * handle substring arguments here. 
   */
  
  /* Determine the number of implicit arguments appended to the end
   * of the argument list (i.e. string lengths).
   */
  if (WN_intrinsic(wn) == INTRN_COUNT) {
    endArgIdx--;
  }
  
  INT argIdx = begArgIdx, total_implicit_args = 0;
  TY_IDX opnd_type;
  for ( ; argIdx <= endArgIdx - total_implicit_args; argIdx++) {
    opnd_type = WN_Tree_Type(WN_kid(wn, argIdx));
    if (TY_Is_Character_Reference(opnd_type) ||
	TY_Is_Chararray_Reference(opnd_type)) {
      total_implicit_args++;
    }
  }
  
  // Emit Intrinsic name
  UINT targid = ctxt.currentXlationContext().getNewVertexId();
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
   
  // Emit Intrinsic argument list, skipping implicit
  // character-string-length arguments assumed to be the last ones in
  // the list (see also ST2F_func_header()).
  UINT srcid = 0;
  UINT position = 0;
  switch (WN_intrinsic(wn)) {
  case INTRN_F90INDEX: // FIXME
  case INTRN_SCAN:
  case INTRN_VERIFY: {
    for (argIdx = begArgIdx; argIdx < endArgIdx; argIdx = argIdx + 2) {
      opnd_type = WN_Tree_Type(WN_kid(wn, argIdx));
      
      position++;
      srcid = ctxt.currentXlationContext().peekVertexId();
      if (TY_Is_Character_Reference(opnd_type) ||
	  TY_Is_Chararray_Reference(opnd_type)) {
	WN2F_String_Argument(xos, WN_kid(wn, argIdx) /* string base */,
			     WN_kid(wn, endArgIdx) /* string len */, ctxt);
      } else {
	xlate_Operand(xos, WN_kid(wn, argIdx), opnd_type, callByValue, ctxt);
      }
      DumpExprGraphEdge(xos, ctxt.currentXlationContext().getNewEdgeId(), srcid, targid, position);
    }
    
    ctxt.currentXlationContext().setFlag(XlationContext::HAS_LOGICAL_ARG);
    position++;
    srcid = ctxt.currentXlationContext().peekVertexId();
    xlate_Operand(xos, WN_kid(wn, endArgIdx), opnd_type, callByValue, ctxt);
    ctxt.currentXlationContext().unsetFlag(XlationContext::HAS_LOGICAL_ARG);
    DumpExprGraphEdge(xos, ctxt.currentXlationContext().getNewEdgeId(), srcid, targid, position);
    break;
  }
  default: {
    
    INT implicit_args = 0;
    for (argIdx = begArgIdx; argIdx <= endArgIdx - implicit_args; argIdx++) {
      opnd_type = WN_Tree_Type(WN_kid(wn, argIdx));
      
      position++;
      srcid = ctxt.currentXlationContext().peekVertexId();
      if (TY_Is_Character_Reference(opnd_type) ||
	  TY_Is_Chararray_Reference(opnd_type)) {
	implicit_args++;
	INT strlIdx = (endArgIdx - (total_implicit_args - implicit_args));
	WN2F_String_Argument(xos, WN_kid(wn, argIdx) /* string base */,
			     WN_kid(wn, strlIdx) /* string len */, ctxt);
      } else {
	xlate_Operand(xos, WN_kid(wn, argIdx), opnd_type, callByValue, ctxt);
      }
      DumpExprGraphEdge(xos, ctxt.currentXlationContext().getNewEdgeId(), srcid, targid, position);

    }
    break;
  }
  } /* switch */
  
  
} /* WN2F_Intr_Funcall */


//***************************************************************************
// Helpers
//***************************************************************************

static void
xlate_UnaryOpUsingIntrinsicTable(xml::ostream& xos, 
				 OPCODE opcode, 
				 TY_IDX result_ty,
				 WN* wn, 
				 PUXlationContext& ctxt)
{
  xlate_BinaryOpUsingIntrinsicTable(xos, opcode, result_ty, wn, NULL, ctxt);
  
}


// xlate_BinaryOpUsingIntrinsicTable: 
static void
xlate_BinaryOpUsingIntrinsicTable(xml::ostream& xos, 
				  OPCODE opcode, 
				  TY_IDX result_ty,
				  WN* wn0, 
				  WN* wn1, 
				  PUXlationContext& ctxt) {
  BOOL is_binary_op = (wn1 != NULL);
  // Get the expected types for the two operands, dependent on whether
  // or not we have a descriptor type.
  TY_IDX wn0_ty, wn1_ty;
  if (OPCODE_desc(opcode) == MTYPE_V) {
    wn0_ty = wn1_ty = result_ty;
  } else {
    wn0_ty = wn1_ty = Stab_Mtype_To_Ty(OPCODE_desc(opcode));
  }
  
  OPERATOR opr = OPCODE_operator(opcode);
  fortTkSupport::IntrinsicXlationTable::XAIFInfoPair infoPair(Whirl2Xaif::getIntrinsicXlationTable().findXAIFInfo(opr, NULL));
  UINT targid, srcid0, srcid1;
  bool noParent=false;
  if (opr==OPR_TRUNC && WN_operator(wn0)==OPR_CALL && strcmp(ST_name(WN_st(wn0)),"TRANSFER")==0) {  // skip the TRUNC node
    if (ctxt.currentXlationContext().peekVertexId()==1) { // nothing there yet
      noParent=true;
    }
    else
      targid=ctxt.currentXlationContext().getVertexId();
  }
  else { 
    // Get XAIF operator type
    const char* opStr = NULL;
    const char* typeStr = NULL;
    switch (infoPair.second.opr) {
    case fortTkSupport::IntrinsicXlationTable::XAIFIntrin: {
      opStr = "xaif:Intrinsic";
      typeStr = "***";
      break;
    }
    case fortTkSupport::IntrinsicXlationTable::XAIFBoolOp: {
      opStr = "xaif:BooleanOperation";
      break;
    }
    default:
      FORTTK_DIE("xlate_BinaryOpUsingIntrinsicTable: no logic to handle: " 
		 << fortTkSupport::IntrinsicXlationTable::toString(infoPair.second.opr).c_str());
    }
    
    // Operation
    targid = ctxt.currentXlationContext().getNewVertexId();
    xos << BegElem(opStr) << Attr("vertex_id", targid)
	<< Attr("name", infoPair.second.name);
    if (typeStr) {
      xos << Attr("type", typeStr);
    }
    bool isPointer = TY_Is_Pointer(result_ty) || TY_is_f90_pointer(result_ty);
    bool isFnPointer = isPointer && (TY_kind(TY_pointed(result_ty)) == KIND_FUNCTION);
    const char* ty_str = 
      (isPointer && (!isFnPointer)) ? TranslateTYToSymType(TY_pointed(result_ty))
      : TranslateTYToSymType(result_ty);
    if (!ty_str) { ty_str = "***"; }
    const char* shape_str = 
      (isPointer && (!isFnPointer)) ? TranslateTYToSymShape(TY_pointed(result_ty))
      : TranslateTYToSymShape(result_ty);
    if (infoPair.second.opr!=fortTkSupport::IntrinsicXlationTable::XAIFBoolOp && strcmp(ty_str,"real"))
      xos << xml::Attr("rType", ty_str);
    if (infoPair.second.opr!=fortTkSupport::IntrinsicXlationTable::XAIFBoolOp && strcmp(shape_str,"scalar"))
      xos << xml::Attr("rShape", shape_str);
    xos << EndElem;
  }    
  // First operand
  srcid0 = ctxt.currentXlationContext().peekVertexId();
  xlate_Operand(xos, wn0, wn0_ty, TRUE/*call-by-value*/, ctxt);
  
  // Second operand (only for binary op)
  if (is_binary_op) {
    srcid1 = ctxt.currentXlationContext().peekVertexId();
    xlate_Operand(xos, wn1, wn1_ty, TRUE/*call-by-value*/, ctxt);
  }
  if (!noParent) { 
    // Edges
    DumpExprGraphEdge(xos, ctxt.currentXlationContext().getNewEdgeId(), srcid0, targid, 1);
    if (is_binary_op) { 
      DumpExprGraphEdge(xos, ctxt.currentXlationContext().getNewEdgeId(), srcid1, targid, 2); 
    }
  }
}


// xlate_Operand: Translate a WHIRL operand (from an operator) to XAIF.
static void
xlate_Operand(xml::ostream& xos, WN *opnd, TY_IDX assumed_ty, 
	      BOOL callByValue, PUXlationContext& ctxt)
{
  // Translate an operand to a function or built-in operator invocation,
  // based on whether the ctxt indicates that we have call-by-value
  // or call-by-reference.  Also, the ctxt indicates what type of
  // argument we expect. FIXME
  
  // We do not handle substring expressions here, and assume any
  // such expression will be dispatched to another. 
  FORTTK_ASSERT(!TY_Is_Character_Reference(assumed_ty) &&
		!TY_Is_Chararray_Reference(assumed_ty),
		fortTkSupport::Diagnostics::UnexpectedInput << "substring reference");
  
  if (!callByValue && !TY_Is_Character_String(assumed_ty)) {
    xlate_MemRef(xos, opnd,              /* address expression */
		 assumed_ty,             /* address type */
		 TY_pointed(assumed_ty), /* object type */
		 0,                      /* offset from address */
		 ctxt);
  } 
  else {
    TranslateWN(xos, opnd, ctxt);
  }
  
  
}

