// -*-Mode: C++;-*-
#include "Open64BasicTypes.h"

#include <sstream>
#include "stab_attr.h"
#include "wn_attr.h"
#include "IFDiagnostics.h"
#include "IntrinsicInfo.h"

static TY_IDX
WN_get_tld_type(const WN* wn);


/**
 * INTRN_c_name() is only implemented when defined(BUILD_WHIRL2C), while
 * we must use INTRN_specific_name() when defined(BUILD_WHIRL2F).  To 
 * abstract away from this, we define the macro INTRN_high_level_name.
 * NOT TRUE anymore, but keep this cause I'm not sure which they really want.
 */
#define INTRN_high_level_name INTRN_specific_name


/*--------- Hidden utility to get type info about a cvtl node ---------*
 *---------------------------------------------------------------------*/

static const MTYPE WN_Cvtl_Mtype[2/*is_signed*/][9/*byte-size*/] =
{/* unsigned */
 {MTYPE_UNKNOWN,      /* 0 bytes */
  MTYPE_U1,           /* 1 byte  */
  MTYPE_U2,           /* 2 bytes */
  MTYPE_UNKNOWN,      /* 3 bytes */
  MTYPE_U4,           /* 4 bytes */
  MTYPE_UNKNOWN,      /* 5 bytes */
  MTYPE_UNKNOWN,      /* 6 bytes */
  MTYPE_UNKNOWN,      /* 7 bytes */
  MTYPE_U8},          /* 8 bytes */
 /* signed */
 {MTYPE_UNKNOWN,      /* 0 bytes */
  MTYPE_I1,           /* 1 byte  */
  MTYPE_I2,           /* 2 bytes */
  MTYPE_UNKNOWN,      /* 3 bytes */
  MTYPE_I4,           /* 4 bytes */
  MTYPE_UNKNOWN,      /* 5 bytes */
  MTYPE_UNKNOWN,      /* 6 bytes */
  MTYPE_UNKNOWN,      /* 7 bytes */
  MTYPE_I8}           /* 8 bytes */
}; /* WN_Cvtl_Mtype */

static TY_IDX
WN_Cvtl_Ty(const WN *wn)
{
   /* The type of a CVTL node is the return type scaled down to the
    * given bitsize.
    */ 
   const INT   cvtl_bytes = WN_cvtl_bits(wn)>>3;
   const MTYPE dest_mtype = WN_rtype(wn);
   const BOOL  is_signed = MTYPE_signed(dest_mtype);
   const MTYPE cvtl_mtype = WN_Cvtl_Mtype[is_signed? 1 : 0][cvtl_bytes];

   return Stab_Mtype_To_Ty(cvtl_mtype);
} /* WN_Cvtl_Ty */


TY_IDX
WN_Tree_Type(const WN* wn)
{
  TY_IDX ty = MTYPE_To_TY(MTYPE_V); // default is void
  if (wn == NULL)
    return ty;

  OPERATOR opr = WN_operator(wn);

  if (OPERATOR_is_stmt(opr)) {
    // -------------------------------------------------------
    // Statements
    // -------------------------------------------------------
    if (OPERATOR_is_call(opr)) {
      // CALLs: statements and expressions (in VH WHIRL)
      ty = WN_Call_Return_Type(wn);      
    } 
    else if (OPERATOR_is_store(opr)) {
      // STOREs: return type of lhs referenced *object*
      ty = WN_GetRefObjType(wn);
    }
  }
  else if (OPERATOR_is_expression(opr)) {
    // -------------------------------------------------------
    // Expressions
    // -------------------------------------------------------
    switch (opr) {
      
      // LOADs
    case OPR_LDA:
    case OPR_LDMA:
    case OPR_LDID:
    case OPR_LDBITS:
    case OPR_ILOAD:
    case OPR_ILOADX: // type of referenced object
      ty = WN_GetRefObjType(wn);
      break;
      
    case OPR_MLOAD: // type of referenced object
      // There is not much we can do about this case
      if (WN_operator(WN_kid1(wn)) == OPR_INTCONST &&
	  TY_Is_Structured(TY_pointed(WN_ty(wn)))) {
	
	/* WEI: for field accesses, get the type of the field */
	if (WN_field_id(wn) != 0) {
	  ty = Get_Field_Type(TY_pointed(WN_ty(wn)), WN_field_id(wn));
	} else {
	  ty = Stab_Get_Mload_Ty(TY_pointed(WN_ty(wn)), 
				 WN_load_offset(wn), 
				 WN_const_val(WN_kid1(wn)));
	}
      } else {
	ty = TY_pointed(WN_ty(wn));
      }
      break;
      
      // ARRAYs
    case OPR_ARRSECTION:  
    case OPR_ARRAY:
    case OPR_ARRAYEXP:
    case OPR_ARRAY_CONSTRUCT:
    case OPR_IMPLIED_DO:      
      /* Get the address type denoted by the base offset by the
       * given indexing expression. Note that we do handle
       * pointers as arrays when there is no ambiguity, and
       * we rely on the flag TY_ptr_as_array() to handle indexing
       * of an array of arrays.  The following should access the
       * fifth element, each element consisting of 17 ints:
       *
       *    int (*a)[17]; .... a[5] ....
       *
       * This will be represented as (OPR_ARRAY (OPR_LDID a) ...),
       * but, since the type of kid0 is ptr-to-arrayOfInts, we need
       * the flag TY_ptr_as_array or else the type of the ARRAY
       * node would be considered a ptr-to-ints (see also 
       * WN2C_array()).
       */
      ty = WN_Tree_Type(WN_kid0(wn));
      if (!TY_Is_Pointer(ty)) {
	/* Create a pointer to be treated as an array when also used
	 * as the base-type of this OPC_ARRAY.  This must be handled
	 * very carefully in WN2C_array().
	 */
	ty = Stab_Pointer_To(Stab_Array_Of(MTYPE_To_TY(MTYPE_U1),
					   WN_element_size(wn)));
      }
      else if (!TY_ptr_as_array(Ty_Table[ty]) && TY_Is_Array(TY_pointed(ty))) {
	ty = Stab_Pointer_To(TY_AR_etype(TY_pointed(ty)));
      }
      break;
      
    case OPR_TAS:
      ty = WN_ty(wn);
      break;
      
    case OPR_SELECT:
      // We make an attempt at retaining pointer types for ptr
      // arithmetics.
      if (WN_rtype(wn) == Pointer_Mtype) {
	ty = WN_Tree_Type(WN_kid0(wn));
	if (!TY_Is_Pointer(ty)) {
	  ty = WN_Tree_Type(WN_kid1(wn));
	  if (!TY_Is_Pointer(ty))
	    ty = MTYPE_To_TY(WN_rtype(wn));
	}
      } else
	ty = MTYPE_To_TY(WN_rtype(wn));
      break;	    
      
    case OPR_CVTL:
      ty = WN_Cvtl_Ty(wn);
      break;
      
    case OPR_PAREN:
      ty = WN_Tree_Type(WN_kid0(wn));
      break;
      
    case OPR_ADD:
      /* We make an attempt at retaining pointer types for ptr
       * arithmetics.  If either one of the operands is a pointer,
       * then return this as the type of the expression, otherwise
       * return the type indicated by the opc_rtype.
       */
      if (WN_rtype(wn) == Pointer_Mtype) {
	ty = WN_Tree_Type(WN_kid0(wn));
	if (!TY_Is_Pointer(ty)) {
	  ty = WN_Tree_Type(WN_kid1(wn));
	  if (!TY_Is_Pointer(ty))
	    ty = MTYPE_To_TY(WN_rtype(wn));
	}
	
#ifdef _BUILD_WHIRL2C
	/* Also check that the constant expression can be reduced */
	if (TY_Is_Pointer(ty) && 
	    WN_Get_PtrAdd_Intconst(WN_kid0(wn), 
				   WN_kid1(wn),
				   TY_pointed(ty)) == NULL) {
	  ty = MTYPE_To_TY(WN_rtype(wn));
	}
#endif /* _BUILD_WHIRL2C */
      } 
      else {
	ty = MTYPE_To_TY(WN_rtype(wn));
      }
      break;
      
    case OPR_INTRINSIC_OP:
      if (WN_intrinsic(wn) == INTRN_TLD_ADDR) {
	//in this case we get its actual type from its arguments
	ty = WN_get_tld_type(wn);
	break;
      }
      
      if (INTR_is_adrtmp(WN_intrinsic(wn))) {
	if (WN_opcode(WN_kid0(wn)) == OPC_VCALL ||
	    WN_opcode(WN_kid0(wn)) == OPC_VINTRINSIC_CALL) {
	  ty = WN_Tree_Type(WN_kid0(WN_kid0(wn)));
	} else {
	  ty = Stab_Pointer_To(WN_Tree_Type(WN_kid0(wn)));
	}
      } else if (INTR_is_valtmp(WN_intrinsic(wn))) {
	if (WN_opcode(WN_kid0(wn)) == OPC_VCALL ||
	    WN_opcode(WN_kid0(wn)) == OPC_VINTRINSIC_CALL) {
	  ty = TY_pointed(WN_Tree_Type(WN_kid0(WN_kid0(wn))));
	} else {
	  ty = WN_Tree_Type(WN_kid0(wn));
	}
      } else {
	ty = WN_intrinsic_return_ty(wn);
      }
      break;
      
    case OPR_CVT:
    case OPR_NEG:
    case OPR_ABS:
    case OPR_SQRT:
    case OPR_REALPART:
    case OPR_IMAGPART:
    case OPR_RND:
    case OPR_TRUNC:
    case OPR_CEIL:
    case OPR_FLOOR:
    case OPR_BNOT:
    case OPR_LNOT:
    case OPR_SUB:
    case OPR_MPY:
    case OPR_DIV:
    case OPR_MOD:
    case OPR_REM:
    case OPR_MAX:
    case OPR_MIN:
    case OPR_BAND:
    case OPR_BIOR:
    case OPR_BXOR:
    case OPR_BNOR:
    case OPR_LAND:
    case OPR_LIOR:
    case OPR_CAND:
    case OPR_CIOR:
    case OPR_SHL:
    case OPR_ASHR:
    case OPR_LSHR:
    case OPR_COMPLEX:
    case OPR_RECIP:
    case OPR_RSQRT:
    case OPR_EQ:
    case OPR_NE:
    case OPR_GT:
    case OPR_GE:
    case OPR_LT:
    case OPR_LE:
    case OPR_CONST:
    case OPR_INTCONST:
    case OPR_DIVREM:
    case OPR_HIGHPART:
    case OPR_LOWPART:
    case OPR_HIGHMPY:
      ty = MTYPE_To_TY(WN_rtype(wn));
      break;
      
    case OPR_PARM:
      ty = WN_Tree_Type(WN_kid0(wn));
      break;
      
    case OPR_COMMA:
      ty = WN_Tree_Type(WN_kid1(wn));
      break;
      
    case OPR_RCOMMA:
      ty = WN_Tree_Type(WN_kid0(wn));
      break;
      
    case OPR_ALLOCA:
      ty = WN_ty(wn);
      break;

    case OPR_STRCTFLD:
      // we need to get the pointer here 
      // because the whirl documentation 
      // claims the STRCTFLD operator 
      // returns a pointer to the field.
      ty = Stab_Pointer_To(WN_ty(wn));
      break;
      
    default:
      ASSERT_FATAL(false, (DIAG_A_STRING, "Programming Error."));
    } /* switch */
  }
   
  return ty;
}


TY_IDX 
Get_Field_Type(TY_IDX base, int field_id) 
{
  ASSERT_FATAL(TY_Is_Structured(base), 
	       (DIAG_A_STRING, "GET_FIELD_TYPE: non struct type"));
  
  UINT cur_fld_id = 0;
  FLD_HANDLE fh = FLD_get_to_field(base, field_id, cur_fld_id);
  return FLD_type(fh);
}

TY_IDX 
WN_GetRefObjType(const WN* wn)
{
  TY_IDX ty = 0;
  OPERATOR opr = WN_operator(wn); 
  
  switch (opr) {
    case OPR_LDA:     // type of referenced (returned) address
    case OPR_LDMA:
      ty = WN_ty(wn);
      break;
    
    case OPR_LDID:    // type of referenced object
    case OPR_LDBITS: {
      ty = WN_ty(wn);
      ST* st = WN_st(wn);
      TY_IDX st_ty = ST_type(st);
      if (st_ty != ty) {
        // this can actually happen when we back-translate portions
        // using WN sub trees from the original whirl (see the patchWN... methods) and the variable
        // in question has become active which obviously not reflected in the original whirl
        const char* nm=ST_name(st);
        DBGMSG_PUB(0, "Warning: WN_GetRefObjType: type mismatch detected for %s", nm);
        ty=st_ty;
      }
      break;
    }
      
    case OPR_ILOAD:   // type of referenced object
    case OPR_ILOADX:
    case OPR_MLOAD:   // Priya added MLOAD case             
      ty = WN_ty(wn);
      break;
    
    // STOREs represent the left-hand-side expression
    case OPR_STID:    // type of referenced lhs object
    case OPR_PSTID:
    case OPR_PSTORE:
    case OPR_STBITS:
      ty = WN_ty(wn);
      break;
      
    case OPR_ISTORE:  // type of referenced lhs object
    case OPR_ISTOREX:
    case OPR_ISTBITS:
    case OPR_MSTORE:  // Priya added MSTORE case
      ty = TY_pointed(WN_ty(wn));
      if (TY_is_f90_pointer(ty)) { 
	ty = TY_pointed(ty);
      }
      if (TY_Is_Array(ty)) { 
	ty = TY_AR_etype(ty);
      }
      break;
    
    case OPR_STRCTFLD:
      ty = WN_ty(wn);           
      break;

    default: 
      { 
      // NOTE: MLOAD, MSTORE are not supported
      std::ostringstream msg;
      msg << "not implemented for opcode " 
	  << &OPERATOR_info [opr]._name [4];
      ASSERT_FATAL(false, (DIAG_A_STRING, msg.str().c_str()));
      }
      break;
  }
  return ty;
}


TY_IDX 
WN_GetBaseObjType(const WN* wn)
{
  TY_IDX ty = 0;
  OPERATOR opr = WN_operator(wn); 

  ST* st = NULL;
  switch (opr) {
    case OPR_LDA:
    case OPR_LDMA:
      st = WN_st(wn);
      ty = ST_type(st);
      if (TY_is_f90_pointer(ty)) { ty = TY_pointed(ty); }
      break;
    
    case OPR_LDID:
    case OPR_LDBITS:
      st = WN_st(wn);
      ty = ST_type(st);
      break;
      
    case OPR_ILOAD:
    case OPR_ILOADX: 
    case OPR_MLOAD:      // Priya added MLOAD case
    {
      WN* baseptr = WN_kid0(wn); // address expression as WN
      TY_IDX baseptr_ty = WN_Tree_Type(baseptr);
      ASSERT_FATAL((TY_kind(baseptr_ty) == KIND_POINTER),
		   (DIAG_A_STRING, "Internal error: expected a pointer type"));
      ty = TY_pointed(baseptr_ty);
      // Note: neither WN_ty() nor TY_pointed(WN_load_addr_ty(wn))
      // always give the base object.  For example, for a reference
      // like F(i)%v, both return the type of v and not the type of
      // the structure.
      break;
    }
      
    // ARRAYs
    case OPR_ARRAY:
      ty = WN_GetBaseObjType(WN_kid0(wn));
      break;

    // STOREs represent the left-hand-side expression
    case OPR_STID: 
    case OPR_PSTID: 
    case OPR_STBITS:
      st = WN_st(wn);
      ty = ST_type(st);
      break;
      
    case OPR_ISTORE: 
    case OPR_ISTOREX:
    case OPR_ISTBITS: 
    case OPR_MSTORE:   // Priya added MSTORE case
    {
      // Note: use WN_Tree_Type(baseptr) instead of WN_ty(wn) to find
      // type of baseptr because the former will attempt to interpret
      // pointer arithmetic, e.g., the this structure reference
      // "X(1)%a%y = ..."
      //   F8ISTORE 0 T<38,anon_ptr.,8>
      //    ...
      //    U8ADD
      //     U8ARRAY 1 48
      //      U8U8LDID 0 <2,3,X> T<35,anon_ptr.,8>
      //      I4INTCONST 2 (0x2)
      //      I4INTCONST 1 (0x1)
      //     U8INTCONST 8 (0x8)
      WN* baseptr = WN_kid1(wn); // address expression as WN
      TY_IDX baseptr_ty = WN_Tree_Type(baseptr); // was: WN_ty(wn)
      ty = TY_pointed(baseptr_ty);
      // This assertion is not always true, e.g. given this Fortan:
      //   F(i)%v = y
      // and this WHIRL
      //   F8ISTORE 0 T<29,anon_ptr.,8>
      //    ...
      //    U8ARRAY 1 16                          (lhs)
      //     U8U8LDID 0 <2,3,F> T<57,anon_ptr.,8> (base)
      //     ...
      // because the baseptr_ty and baseptr types are different, the
      // structure element reference will implicitly take place!
      //FORTTK_ASSERT((baseptr_ty == WN_Tree_Type(baseptr)),
      //              "Internal error: base pointer types are inconsistent");
      break;
    }

    case OPR_PSTORE:
      ty=WN_ty(wn);
      break;

    case OPR_STRCTFLD:
      ty = WN_load_addr_ty(wn);           
      break;
    
    default: { 
      // NOTE: MLOAD, MSTORE are not supported
      std::ostringstream msg;
      msg << "not implemented for opcode " 
	  << &OPERATOR_info [opr]._name [4];
      ASSERT_FATAL(false, (DIAG_A_STRING, msg.str().c_str()));
      break;
    }
  }
  return ty;
}

TY_IDX 
WN_Call_Type(const WN* wn)
{
  OPERATOR opr = WN_operator(wn); 
  switch (opr) {
  case OPR_CALL:  
    return ST_pu_type(WN_st(wn)); 
  case OPR_ICALL: 
  case OPR_VFCALL: 
    return WN_ty(wn);
  case OPR_PICCALL: 
    return ST_type(WN_st(wn));
  case OPR_INTRINSIC_CALL:
  default:
    ASSERT_FATAL(false, (DIAG_A_STRING, "Programming Error."));
    return 0;
  }
}


TY_IDX 
WN_Call_Return_Type(const WN* wn)
{
  TY_IDX return_ty = 0;
  OPERATOR opr = WN_operator(wn); 
  if (opr == OPR_INTRINSIC_CALL) {
    return_ty = WN_intrinsic_return_ty(wn);
  } else {
    if (opr == OPR_CALL && IntrinsicInfo::isIntrinsic(wn)) {
      // here come hacks for special cases that will be made obsolete by the move to Rose
      ST* st_p = WN_st(wn);
      const char* funcNm = ST_name(st_p);
      if (!strcmp(funcNm,"LEN")
	  ||
	  !strcmp(funcNm,"ALLOCATED")
	  || 
	  !strcmp(funcNm,"MAXVAL")
          || 
	  !strcmp(funcNm,"MAXLOC")
          || 
	  !strcmp(funcNm,"MINVAL")
          || 
	  !strcmp(funcNm,"SIZE")
	  ||
	  !strcmp(funcNm,"SCAN")
	  ||
	  !strcmp(funcNm,"SUM")) { 
	return_ty=Stab_Mtype_To_Ty(OPCODE_rtype(WN_opcode(wn)));
      }
      else if (!strcmp(funcNm,"DBLE")
	       ||
	       !strcmp(funcNm,"FLOAT")
	       ||
	       !strcmp(funcNm,"REAL")
	       ||
	       !strcmp(funcNm,"INT")) { 
	return_ty=WN_GetExprType(WN_kid0(wn));
	// this can be for instance an integer array
	if (TY_mtype(return_ty) != OPCODE_rtype(WN_opcode(wn))) { 
	  if (TY_Is_Pointer(return_ty)) {
	    if (TY_Is_Array(TY_pointed(return_ty)) 
		||
		TY_Is_Scalar(TY_pointed(return_ty))) {
	      return_ty=TY_pointed(return_ty);
	    }
	    else { 
	      ASSERT_FATAL(false, (DIAG_UNIMPLEMENTED, "in WN_Call_Return_Type"));
	    }
	  } 
	  if  (TY_Is_Array(return_ty)) { 
	    // make a fake type to return
	    return_ty=Make_Array_Type(OPCODE_rtype(WN_opcode(wn)),
				      (TY_arb(return_ty)).Entry()->dimension,
				      1);
	  }
	  else 
	    return_ty=Stab_Mtype_To_Ty(OPCODE_rtype(WN_opcode(wn)));
	}
      }
      else if (!strcmp(funcNm,"LBOUND")
	       ||
	       !strcmp(funcNm,"UBOUND")) {
	if (WN_kid_count(wn)==2) { // returns an integer 
	  return_ty=Stab_Mtype_To_Ty(OPCODE_rtype(WN_opcode(wn)));
	}
	else { // returns a vector
	  // make up a type to return
	  return_ty=Make_Array_Type(OPCODE_rtype(WN_opcode(wn)),
				    1, // is just a vector
				    (TY_arb(WN_GetExprType(WN_kid0(wn)))).Entry()->dimension); // length of the vector is the number of dimensions of the first argument
	} 
      }
      else { 
	return_ty=WN_GetExprType(WN_kid0(wn));
      }
    }
    else { 
      TY_IDX func_ty = WN_Call_Type(wn);
      return_ty = Func_Return_Type(func_ty);    
    }
  }
  return return_ty;
}


INT
WN_Call_First_Arg_Idx(const WN* wn)
{
  INT idx = 0;
  OPERATOR opr = WN_operator(wn); 
  if (opr == OPR_INTRINSIC_CALL) {
    TY_IDX return_ty = WN_Call_Return_Type(wn);
    BOOL return_to_param = WN_intrinsic_return_to_param(return_ty);
    idx = (return_to_param? 1 : 0);
  } else {
    TY_IDX func_ty = WN_Call_Type(wn);
    BOOL return_to_param = Func_Return_To_Param(func_ty);
    idx = (return_to_param) ? (Func_Return_Character(func_ty)? 2 : 1) : 0;
  }
  return idx;
}


INT
WN_Call_Last_Arg_Idx(const WN* wn)
{
  INT idx = WN_kid_count(wn) - 1; // default

  OPERATOR opr = WN_operator(wn); 
  switch (opr) {
  case OPR_ICALL: 
  case OPR_VFCALL: 
  case OPR_PICCALL: 
    idx = WN_kid_count(wn) - 2;
    break;
  default: 
    break; // fall through
  }
  return idx;
}


const char *
WN_intrinsic_name(INTRINSIC intr_opc)
{
  const char *name = NULL;
  Is_True(INTRINSIC_FIRST<=intr_opc && intr_opc<=INTRINSIC_LAST,
	  ("Intrinsic Opcode (%d) out of range", intr_opc)); 
  
  if (INTRN_high_level_name(intr_opc) != NULL) {
    name = INTRN_high_level_name(intr_opc);
  } else {
    name = get_intrinsic_name(intr_opc);
  }
  
  return name;
}


TY_IDX
WN_intrinsic_return_ty(const WN* call)
{
  TY_IDX ret_ty = 0;
  
  OPERATOR opr = WN_operator(call);
  ASSERT_FATAL(opr == OPR_INTRINSIC_CALL || opr == OPR_INTRINSIC_OP,
	       (DIAG_A_STRING, "Programming Error!"));
  
  INTRINSIC intr_opc = (INTRINSIC)WN_intrinsic(call);
  switch (INTRN_return_kind(intr_opc)) {
  case IRETURN_UNKNOWN: {
    /* Use the opcode to get the type */
    OPCODE opc = WN_opcode(call);
    ret_ty = Stab_Mtype_To_Ty(OPCODE_rtype(opc));
    break;
  }
  case IRETURN_V:
    ret_ty = Stab_Mtype_To_Ty(MTYPE_V);
    break;
  case IRETURN_I1:
    ret_ty = Stab_Mtype_To_Ty(MTYPE_I1);
    break;
  case IRETURN_I2:
    ret_ty = Stab_Mtype_To_Ty(MTYPE_I2);
    break;
  case IRETURN_I4:
    ret_ty = Stab_Mtype_To_Ty(MTYPE_I4);
    break;
  case IRETURN_I8:
    ret_ty = Stab_Mtype_To_Ty(MTYPE_I8);
    break;
  case IRETURN_U1:
    ret_ty = Stab_Mtype_To_Ty(MTYPE_U1);
    break;
  case IRETURN_U2:
    ret_ty = Stab_Mtype_To_Ty(MTYPE_U2);
    break;
  case IRETURN_U4:
    ret_ty = Stab_Mtype_To_Ty(MTYPE_U4);
    break;
  case IRETURN_U8:
    ret_ty = Stab_Mtype_To_Ty(MTYPE_U8);
    break;
  case IRETURN_F4:
    ret_ty = Stab_Mtype_To_Ty(MTYPE_F4);
    break;
  case IRETURN_F8:
    ret_ty = Stab_Mtype_To_Ty(MTYPE_F8);
    break;
  case IRETURN_FQ:
    ret_ty = Stab_Mtype_To_Ty(MTYPE_FQ);
    break;
  case IRETURN_C4:
    ret_ty = Stab_Mtype_To_Ty(MTYPE_C4);
    break;
  case IRETURN_C8:
    ret_ty = Stab_Mtype_To_Ty(MTYPE_C8);
    break;
  case IRETURN_CQ:
    ret_ty = Stab_Mtype_To_Ty(MTYPE_CQ);
    break;
  case IRETURN_PV:
    ret_ty = Stab_Pointer_To(Stab_Mtype_To_Ty(MTYPE_V));
    break;
  case IRETURN_PU1:
    ret_ty = Stab_Pointer_To(Stab_Mtype_To_Ty(MTYPE_U1));
    break;
  case IRETURN_DA1:
    ret_ty = WN_Tree_Type(WN_kid0(call));
    break;
  case IRETURN_M:
    ret_ty = Stab_Mtype_To_Ty(MTYPE_M);
    break;
  default:
    ASSERT_FATAL(false, (DIAG_A_STRING, "Programming Error."));
    ret_ty = Stab_Mtype_To_Ty(MTYPE_V);
    break;
  }
  
  return ret_ty;
} /* WN_intrinsic_return_ty */


BOOL 
WN_intrinsic_return_to_param(TY_IDX return_ty)
{
  // Assume there is only one case when the return value cannot be
  // passed through registers: a quad precision complex number.
  return (TY_mtype(return_ty) == MTYPE_CQ);
}


WN *
WN_Get_PtrAdd_Intconst(WN* wn0, WN* wn1, TY_IDX pointed_ty)
{
   /* We make an attempt at retaining pointer types for ptr
    * additions, where we expect the ptr expression to be of
    * one of the following forms:
    *
    *    1)  ptr + expr
    *    2)  ptr + expr*const
    *    3)  ptr + const
    *
    * where const must be a multiple of the size of the pointed_ty
    * and only abscent when the size is 1. If this pattern is not 
    * found, then return NULL; otherwise return the const expression,
    * if one is found, or the integral expression when size==1.
    */
   WN *intconst = NULL;
	 
   /* Identify the integral expression */
   if (!TY_Is_Pointer(WN_Tree_Type(wn0)))
      intconst = wn0;
   else if (!TY_Is_Pointer(WN_Tree_Type(wn1)))
      intconst = wn1;

   /* Get the constant expression */
   if (intconst != NULL && TY_size(pointed_ty) > 1)
   {
      /* Identify the integral constant expression */
      if (WN_operator(intconst) == OPR_MPY)
      {
	 if (WN_operator(WN_kid0(intconst)) == OPR_INTCONST)
	    intconst = WN_kid0(intconst);
	 else if (WN_operator(WN_kid1(intconst)) == OPR_INTCONST)
	    intconst = WN_kid1(intconst);
	 else
	    intconst = NULL;
      }
      else if (WN_operator(intconst) != OPR_INTCONST)
	 intconst = NULL;
   }
   
   /* Make sure the constant expression is a multiple of the size of type
    * pointed to.
    */
   if (TY_size(pointed_ty) == 0 ||    /* incomplete type */
       (intconst != NULL && 
	WN_operator(intconst) == OPR_INTCONST &&
	WN_const_val(intconst)%TY_size(pointed_ty) != 0LL))
   {
      intconst = NULL;
   }
   return intconst;
} /* WN_Get_PtrAdd_Intconst */

static TY_IDX
WN_get_tld_type(const WN* wn) 
{
  //wn must be TLD_ADDR(...)
  WN* kid = WN_kid0(WN_kid0(wn));
  TY_IDX result_ty = WN_Tree_Type(kid);
  switch (TY_kind(result_ty)) {
  case KIND_ARRAY: {
    TY_IDX new_ty;
    int dim = 1;
    for (new_ty = TY_etype(result_ty); TY_kind(new_ty) == KIND_ARRAY; new_ty = TY_etype(new_ty), dim++);
    for (;dim > 0; new_ty = Make_Pointer_Type(new_ty), dim--);
    return new_ty;
  }
  case KIND_STRUCT:
    if (WN_field_id(kid) != 0) {
      return Make_Pointer_Type(Get_Field_Type(result_ty, WN_field_id(kid)));
    }
    return Make_Pointer_Type(result_ty);
  case KIND_POINTER: {
    //need to handle ptr to shared data as a speical case
    TY_IDX pointed = TY_pointed(result_ty);
    if (TY_is_shared(pointed)) {
      if (TY_kind(pointed) != KIND_VOID &&
	  Get_Type_Block_Size(pointed) <= 1) {
	return Make_Pointer_Type(Make_Pointer_Type(pshared_ptr_idx));
      } else {
	return Make_Pointer_Type(Make_Pointer_Type(shared_ptr_idx));
      }
    }
    //fall thru to the default case if not shared
  }
  default:
    return Make_Pointer_Type(result_ty);
  }
}

TY_IDX
WN_GetExprType(const WN* wn) {
  TY_IDX ty = MTYPE_To_TY(MTYPE_V); // default is void
  if (wn == NULL)
    return ty;

  OPERATOR opr = WN_operator(wn);

  if (OPERATOR_is_stmt(opr)) {
    // -------------------------------------------------------
    // Statements
    // -------------------------------------------------------
    if (OPERATOR_is_call(opr)) {
      // CALLs: statements and expressions (in VH WHIRL)
      ty = WN_Call_Return_Type(wn);      
    } 
    else if (OPERATOR_is_store(opr)) {
      // STOREs: return type of lhs referenced *object*
      ty = WN_GetRefObjType(wn);
    }
  }
  else if (OPERATOR_is_expression(opr)) {
    // -------------------------------------------------------
    // Expressions
    // -------------------------------------------------------
    switch (opr) {
      
      // LOADs
      // LOADs
    case OPR_LDA:
    case OPR_LDMA:
    case OPR_LDID:
    case OPR_LDBITS:
      ty = WN_GetRefObjType(wn);
      break;
    case OPR_ILOAD:
    case OPR_ILOADX: // type of referenced object
      ty = WN_GetExprType(WN_kid0(wn));
      break;
      
    case OPR_MLOAD: // type of referenced object
      // There is not much we can do about this case
      if (WN_operator(WN_kid1(wn)) == OPR_INTCONST &&
	  TY_Is_Structured(TY_pointed(WN_ty(wn)))) {
	
	/* WEI: for field accesses, get the type of the field */
	if (WN_field_id(wn) != 0) {
	  ty = Get_Field_Type(TY_pointed(WN_ty(wn)), WN_field_id(wn));
	} else {
	  ty = Stab_Get_Mload_Ty(TY_pointed(WN_ty(wn)), 
				 WN_load_offset(wn), 
				 WN_const_val(WN_kid1(wn)));
	}
      } else {
	ty = TY_pointed(WN_ty(wn));
      }
      break;
      
      // ARRAYs
    case OPR_ARRSECTION:  
    case OPR_ARRAYEXP:
    case OPR_ARRAY_CONSTRUCT:
    case OPR_IMPLIED_DO:      
      ty = WN_GetExprType(WN_kid0(wn));
      break;
      
    case OPR_ARRAY:
      ty = WN_Tree_Type(WN_kid0(wn));
      if (!TY_Is_Pointer(ty)) {
	/* Create a pointer to be treated as an array when also used
	 * as the base-type of this OPC_ARRAY.  This must be handled
	 * very carefully in WN2C_array().
	 */
	ty = Stab_Pointer_To(Stab_Array_Of(MTYPE_To_TY(MTYPE_U1),
					   WN_element_size(wn)));
      }
      else if (!TY_ptr_as_array(Ty_Table[ty]) && TY_Is_Array(TY_pointed(ty))) {
	ty = Stab_Pointer_To(TY_AR_etype(TY_pointed(ty)));
      }
      break;

    case OPR_TAS:
      ty = WN_ty(wn);
      break;
      
    case OPR_SELECT:
      // We make an attempt at retaining pointer types for ptr
      // arithmetics.
      if (WN_rtype(wn) == Pointer_Mtype) {
	ty = WN_Tree_Type(WN_kid0(wn));
	if (!TY_Is_Pointer(ty)) {
	  ty = WN_Tree_Type(WN_kid1(wn));
	  if (!TY_Is_Pointer(ty))
	    ty = MTYPE_To_TY(WN_rtype(wn));
	}
      } else
	ty = MTYPE_To_TY(WN_rtype(wn));
      break;	    
      
    case OPR_CVTL:
      ty = WN_Cvtl_Ty(wn);
      break;
      
    case OPR_PAREN:
      ty = WN_GetExprType(WN_kid0(wn));
      break;
      
    case OPR_INTRINSIC_OP:
      if (WN_intrinsic(wn) == INTRN_TLD_ADDR) {
	//in this case we get its actual type from its arguments
	ty = WN_get_tld_type(wn);
	break;
      }
      
      if (INTR_is_adrtmp(WN_intrinsic(wn))) {
	if (WN_opcode(WN_kid0(wn)) == OPC_VCALL ||
	    WN_opcode(WN_kid0(wn)) == OPC_VINTRINSIC_CALL) {
	  ty = WN_Tree_Type(WN_kid0(WN_kid0(wn)));
	} else {
	  ty = Stab_Pointer_To(WN_Tree_Type(WN_kid0(wn)));
	}
      } else if (INTR_is_valtmp(WN_intrinsic(wn))) {
	if (WN_opcode(WN_kid0(wn)) == OPC_VCALL ||
	    WN_opcode(WN_kid0(wn)) == OPC_VINTRINSIC_CALL) {
	  ty = TY_pointed(WN_Tree_Type(WN_kid0(WN_kid0(wn))));
	} else {
	  ty = WN_Tree_Type(WN_kid0(wn));
	}
      } else {
	ty = WN_intrinsic_return_ty(wn);
      }
      break;
      
    case OPR_CVT:
    case OPR_NEG:
    case OPR_ABS:
    case OPR_SQRT:
    case OPR_REALPART:
    case OPR_IMAGPART:
    case OPR_RND:
    case OPR_TRUNC:
    case OPR_CEIL:
    case OPR_FLOOR:
    case OPR_BNOT:
    case OPR_LNOT:
    case OPR_BAND:
    case OPR_BIOR:
    case OPR_BXOR:
    case OPR_BNOR:
    case OPR_LAND:
    case OPR_LIOR:
    case OPR_CAND:
    case OPR_CIOR:
    case OPR_SHL:
    case OPR_ASHR:
    case OPR_LSHR:
    case OPR_COMPLEX:
    case OPR_RECIP:
    case OPR_RSQRT:
    case OPR_EQ:
    case OPR_NE:
    case OPR_GT:
    case OPR_GE:
    case OPR_LT:
    case OPR_LE:
    case OPR_CONST:
    case OPR_INTCONST:
    case OPR_DIVREM:
    case OPR_HIGHPART:
    case OPR_LOWPART:
    case OPR_HIGHMPY:
      ty = MTYPE_To_TY(WN_rtype(wn));
      break;

    case OPR_ADD:
    case OPR_SUB:
    case OPR_MPY:
    case OPR_DIV:
    case OPR_MOD:
    case OPR_REM:
    case OPR_MAX:
    case OPR_MIN: { 
      // look at the operands
      TY_IDX ty1=WN_GetExprType(WN_kid0(wn));
      TY_IDX ty2=WN_GetExprType(WN_kid1(wn));
      if (TY_kind(ty1) == KIND_ARRAY) 
	ty=ty1;
      else if (TY_Is_Pointer(ty1) 
	       && 
	       TY_kind(TY_pointed(ty1)) == KIND_ARRAY) 
	ty=TY_pointed(ty1);
      else if (TY_kind(ty2) == KIND_ARRAY)
	ty=ty2;
      else if (TY_Is_Pointer(ty2) 
	       && 
	       TY_kind(TY_pointed(ty2)) == KIND_ARRAY) 
	ty=TY_pointed(ty2);
      else 
	ty=MTYPE_To_TY(WN_rtype(wn));
      break;
    } 
      
    case OPR_PARM:
      if (WN_operator(WN_kid0(wn))==OPR_ILOAD || WN_operator(WN_kid0(wn))==OPR_ARRSECTION) {
	ty = WN_GetExprType(WN_kid0(WN_kid0(wn)));
      }
      else { 
	ty = WN_GetExprType(WN_kid0(wn));
      }
      break;
      
    case OPR_COMMA:
      ty = WN_Tree_Type(WN_kid1(wn));
      break;
      
    case OPR_RCOMMA:
      ty = WN_Tree_Type(WN_kid0(wn));
      break;
      
    case OPR_ALLOCA:
      ty = WN_ty(wn);
      break;

    case OPR_STRCTFLD:
      // we need to get the pointer here 
      // because the whirl documentation 
      // claims the STRCTFLD operator 
      // returns a pointer to the field.
      ty = Stab_Pointer_To(WN_ty(wn));
      break;
      
    default:
      ASSERT_FATAL(false, (DIAG_A_STRING, "Programming Error."));
    } /* switch */
  }
   
  return ty;
}
