
#include "defs.h"
#include "glob.h"
#include "config.h"
#include "wn.h"
#include "wn_util.h"
#include "const.h"
#include <cmplrs/rcodes.h>
#include <upc_wn_util.h>
#include <upc_symtab_utils.h>
#include <stack>

extern void fdump_tree(FILE *f, WN *wn);

BOOL Fold_Keep_Shared_Tas = TRUE;

SPTR_OFFSET_ACCUMULATION_STACK sptr_off_accumulation_stack;
SPTR_ACCUMULATION_STATE sptr_accumulation_state = NO_ACCUMULATION;;

std::stack<CONSISTENCY_class> consistency_stack;
MEM_POOL consistency_pool;
WN_MAP consistency_map;



static inline TYPE_ID
Widen_Mtype (TYPE_ID t)
{
  if (MTYPE_is_m(t))
    return t;
  if (MTYPE_is_void(t) || t == MTYPE_BS) {
    Fail_FmtAssertion ("Widen_Mtype: for MTYPE_V or MTYPE_BS");
    return t;
  }
  if (MTYPE_byte_size(t) >= 4)
    return t;
  return Mtype_TransferSize(MTYPE_I4, t);
}


INTRINSIC 
WN_Type_To_SyncIntrinsic(TYPE_ID mtype) {
  switch (mtype) {
  case MTYPE_I1:
  case MTYPE_I2:
  case MTYPE_I4:
  case MTYPE_I8:
  case MTYPE_U1:
  case MTYPE_U2:
  case MTYPE_U4:
  case MTYPE_U8:
  case MTYPE_A4:
  case MTYPE_A8:
    return INTRN_WAIT_SYNC_REG;
    
    //WEI: There's no sync function for float and double, 
    //use the general wait_syncnb instead
  case MTYPE_F4:
    //return INTRN_WAIT_SYNC_FLOAT;
  case MTYPE_F8:
    //return INTRN_WAIT_SYNC_DOUBLE;
  default:
    return INTRN_WAIT_SYNC_MEM;
  }
}



INTRINSIC 
WN_Type_To_Intrinsic(OPERATOR opr, int mtype, int strict=1, BOOL phaseless = TRUE) {
  switch(opr) {
  case OPR_LDID:
  case OPR_ILOAD:
  case OPR_MLOAD:
    switch(mtype) {
    case MTYPE_I1:
    case MTYPE_I2:
    case MTYPE_I4:
    case MTYPE_I8:
    case MTYPE_U1:
    case MTYPE_U2:
    case MTYPE_U4:
    case MTYPE_U8:
    case MTYPE_A4:
    case MTYPE_A8:
      return strict ? (phaseless ? INTRN_GET_P_VAL : INTRN_SILD) :
	(phaseless ? INTRN_GET_NB_P_VAL : INTRN_SILD_NB);
    case MTYPE_F4:
      return phaseless ? INTRN_GET_P_FVAL : INTRN_SF4LD;
    case MTYPE_F8:
      return phaseless ? INTRN_GET_P_DVAL : INTRN_SF8LD;
    case MTYPE_F10:
    case MTYPE_F16:
    case MTYPE_FQ:
    case MTYPE_C4:
    case MTYPE_C8:
    case MTYPE_CQ:
    case MTYPE_BS:
    case MTYPE_M:
      return strict ? (phaseless ? INTRN_GET_P : INTRN_SMLD) : 
	(phaseless ? INTRN_GET_NB_P : INTRN_SMLD_NB);
    default :
      Is_True(0, ("Wrong mtype in WN_Type_To_Intrinsic",""));
    }
    break;
  case OPR_ISTORE:
  case OPR_STID:
  case OPR_MSTORE:
    switch(mtype) {
    case MTYPE_I1:
    case MTYPE_I2:
    case MTYPE_I4:
    case MTYPE_I8:
    case MTYPE_U1:
    case MTYPE_U2:
    case MTYPE_U4:
    case MTYPE_U8:
    case MTYPE_A4:
    case MTYPE_A8:
      return strict ? (phaseless ? INTRN_PUT_P_VAL : INTRN_SIST ) : 
	(phaseless ? INTRN_PUT_NB_P_VAL : INTRN_SIST_NB);

    case MTYPE_F4:
      return (phaseless ? INTRN_PUT_P_FVAL : INTRN_SF4ST); 

    case MTYPE_F8:
      return phaseless  ? INTRN_PUT_P_DVAL : INTRN_SF8ST ;
    case MTYPE_F10:
    case MTYPE_F16:
    case MTYPE_FQ:
    case MTYPE_C4:
    case MTYPE_C8:
    case MTYPE_CQ:
    case MTYPE_BS:
    case MTYPE_M:
      return strict ? (phaseless ? INTRN_PUT_P : INTRN_SMST) : 
	(phaseless ? INTRN_PUT_NB_P : INTRN_SMST_NB);
    default :
      Is_True(0, ("Wrong mtype in WN_Type_To_Intrinsic",""));
    }
    break;
  default:
    Is_True(0, ("Operator not supported in WFE_Type_To_Intrinsic",""));
  }

  /* NOTREACHED */
  Is_True(0, ("Programming error.",""));
  return INTRINSIC_INVALID;
}


// For opr == OPR_EQ, bsize and idx1 are either shared_ptr_idx or pshared 
INTRINSIC
WN_Operator_To_Intrinsic (OPERATOR opr, INT bsize  = 0, INT idx1 = 0)
{  
  switch (opr) {
  case OPR_ADD:
  case OPR_SUB:
    switch(bsize) {
    case 0:
      return INTRN_ADD_PI;
    case 1:
      return INTRN_ADD_P1;
    default:
      return INTRN_SPTRADD;
    }
    break;
  case OPR_EQ:
    if (idx1 == shared_ptr_idx) {
      return (bsize == shared_ptr_idx) ? INTRN_EQ_S_S :
	(bsize == pshared_ptr_idx) ? INTRN_EQ_S_P : INTRN_ISNULL_S;
    } else if (idx1 == pshared_ptr_idx) 
      return (bsize == shared_ptr_idx) ? INTRN_EQ_S_P :
	(bsize == pshared_ptr_idx) ? INTRN_EQ_P_P : INTRN_ISNULL_P;
    else 
      return bsize == shared_ptr_idx ? INTRN_ISNULL_S : INTRN_ISNULL_P; 
	  
    break;
  default:
    Is_True(0, ("",""));
  }

 
  
  return INTRN_SPTRADD;
}



static WN* 
Spill_And_Take_Address(WN *wn) {

  WN *result;
  TY_IDX spill_ty;

  if(WN_operator(wn) == OPR_LDA)
    return wn;
  else  if(WN_operator(wn) == OPR_TAS) {
    wn = Strip_TAS(wn);
  }

  switch(WN_operator(wn)) {
  case OPR_ILOAD:
    spill_ty = WN_field_id(wn) == 0 ? WN_ty(wn) : Get_Field_Type(WN_ty(wn), WN_field_id(wn));
    if(WN_field_id(wn) && Type_Is_Shared_Ptr(spill_ty))
      spill_ty = TY_To_Sptr_Idx(spill_ty);
    break;
  default:
    spill_ty =  MTYPE_To_TY(WN_rtype(wn));
    break;
  }

  TYPE_ID desc = TY_mtype(spill_ty);
  result = WN_CreateBlock ();
  ST *handle_st = Gen_Temp_Symbol(spill_ty, (char*) ".spillstoreparm");
  WN_INSERT_BlockLast (result, WN_Stid (desc, 0, handle_st, spill_ty, wn));
  result  = WN_CreateComma (OPR_COMMA, Pointer_Mtype, MTYPE_V, result, 
			    WN_Lda(Pointer_Mtype, 0, handle_st, 0));
  return result;
}

static
TYPE_ID Shared_Load_Extend_Mtyp (TYPE_ID typ) 
{
  BOOL _64_bit_target = TY_size(MTYPE_To_TY(Pointer_type)) > 4;
  switch (typ) {
  case MTYPE_B:
  case MTYPE_I1:
  case MTYPE_I2:       
  case MTYPE_I4: 
  case MTYPE_U1:        
  case MTYPE_U2:        
  case MTYPE_U4: 
    if(_64_bit_target)
      return MTYPE_I8;
    else
      return MTYPE_I4;
  case MTYPE_I8:   
  case MTYPE_U8:
    if(_64_bit_target)
      return MTYPE_I8;
    else
      Is_True(0,("",""));
    
  default:
    return typ;
  }

}



static CONSISTENCY_class 
Get_Access_Consistency (TY_IDX idx) {
  
  if (idx == 0)
    return consistency_stack.top();
  
  switch(TY_kind(idx)) {
  case KIND_POINTER:
    idx = TY_pointed(idx);
    break;
  case KIND_ARRAY:
    idx = Get_Inner_Array_Type(idx);
    break;
  }
  
  return  TY_is_strict (idx) ? STRICT_CONSISTENCY : 
    (TY_is_relaxed(idx) ? RELAXED_CONSISTENCY : consistency_stack.top());
}



WN*
WN_Create_Shared_Store (WN *st, BOOL src_is_shared, WN_OFFSET xtra_offst,
			BOOL has_offt, WN *offt) {
  
  ST *op_st = 0, *handle_st; 
  WN *arg0, *call_wn;
  WN *wn0, *wn1, *init_wn = 0, *handle_wn;
  TYPE_ID rtype, desc;
  WN_OFFSET offset;
  CONSISTENCY_class consistency = STRICT_CONSISTENCY;
  TY_IDX handle_ty_idx;
  TY_IDX actual_shared_ptr_idx;
  rtype = ((WN_desc(st) == MTYPE_V) ? MTYPE_M : WN_desc(st)) ;
  //desc = WN_desc(st);
  int strictw;
  int transfer_size = 0;
  WN *spilled_preg_ldid = 0;
  
  BOOL _64_bit_target = TY_size(MTYPE_To_TY(Pointer_type)) > 4;

  // fprintf(stderr, "Shared Store IN\n");
//   fdump_tree(stderr, st);
    

  actual_shared_ptr_idx = pshared_ptr_idx;
  
  switch (WN_operator(st)) {
  case OPR_STID:
    op_st = WN_st(st);
    consistency = Get_Access_Consistency(WN_ty(st));
    if(TY_kind(WN_ty(st)) == KIND_POINTER && TY_is_shared(WN_ty(st))) {
      if(Type_Is_Shared_Ptr(TY_pointed(WN_ty(st)))) {
	 rtype = TY_mtype(TY_To_Sptr_Idx(TY_pointed(WN_ty(st))));
	 transfer_size = TY_size(TY_To_Sptr_Idx(TY_pointed(WN_ty(st))));
      } else 
	 transfer_size = TY_size(WN_ty(st));
    }
    break;
  case OPR_ISTORE:
  case OPR_MSTORE: {
    arg0 = WN_kid1(st);
    TY_IDX pt_idx = TY_pointed(WN_ty(st));
    consistency = Get_Access_Consistency(WN_ty(arg0));
    if(Type_Is_Shared_Ptr(pt_idx)) {
      if(Type_Is_Shared_Ptr(pt_idx, TRUE)) {
	//ptr to shared - need to adjust size to sizeof(shared_ptr)
	rtype = TY_mtype(TY_To_Sptr_Idx(TY_pointed(WN_ty(st))));
	transfer_size = TY_size(TY_To_Sptr_Idx(TY_pointed(WN_ty(st))));
      } else if(TY_kind(pt_idx) == KIND_STRUCT) {
	if(WN_field_id(st)) {
	  rtype = TY_mtype(Get_Field_Type(pt_idx, WN_field_id(st)));
	  transfer_size = TY_size(Get_Field_Type(pt_idx, WN_field_id(st)));
	} else {
	  rtype = MTYPE_M;
	  transfer_size = TY_size(pt_idx);
	}
      }
    }
    arg0 = Strip_TAS(arg0);
    break;
  }
  default:
    Is_True(0,("",""));
    
  }
  
  actual_shared_ptr_idx = TY_To_Sptr_Idx(WN_ty(st));
  if(TY_is_shared(WN_ty(st)) && TY_kind(WN_ty(st)) != KIND_SCALAR && TY_kind(WN_ty(st)) != KIND_STRUCT) {
    // rtype = TY_mtype(actual_shared_ptr_idx)
      ;
    // src_is_shared = TRUE;
  }

  
  if(!_64_bit_target && (rtype == MTYPE_I8 ||  rtype == MTYPE_A8 || rtype == MTYPE_U8) ) {
    transfer_size = TY_size(MTYPE_To_TY(rtype));
    rtype = MTYPE_M;
  } else if(consistency == RELAXED_CONSISTENCY && (rtype == MTYPE_F4 || rtype == MTYPE_F8)) {
    transfer_size = TY_size(MTYPE_To_TY(rtype));
    rtype = MTYPE_M;
  }
  

  // if (Get_Type_Block_Size(WN_ty(st)) > 1)
//     actual_shared_ptr_idx = shared_ptr_idx;
 
  
  handle_ty_idx = (rtype == MTYPE_M) ? upc_hsync_mem_ty : upc_hsync_reg_ty;
  desc = (consistency == STRICT_CONSISTENCY || rtype == MTYPE_F4 || rtype == MTYPE_F8) ? 
    MTYPE_V : TY_mtype(handle_ty_idx);
  offset = xtra_offst;
  // shared scalar of field of shared struct
  if (op_st) {
    if(WN_field_id(st) >1) 
      offset += Adjust_Field_Offset(ST_type(op_st), WN_field_id(st));
    else
      offset += WN_offset(st);

    arg0 = WN_CreateParm(TY_mtype(actual_shared_ptr_idx), 
                         WN_CreateLdid(OPR_LDID, TY_mtype(actual_shared_ptr_idx), 
				       TY_mtype(actual_shared_ptr_idx),
				       0 /*offset*/, ST_st_idx(*op_st),actual_shared_ptr_idx, 0), 
			 actual_shared_ptr_idx, WN_PARM_BY_VALUE);
			 
  } else {
    arg0 = WN_CreateParm(TY_mtype(actual_shared_ptr_idx), arg0, actual_shared_ptr_idx, 
			 WN_PARM_BY_VALUE);
  }

  
  
  call_wn = WN_Create (OPR_INTRINSIC_CALL, desc, desc,
                       (rtype == MTYPE_F4 || rtype == MTYPE_F8 || src_is_shared) ? 3 : 4);
  WN_intrinsic(call_wn) = WN_Type_To_Intrinsic(OPR_STID, 
					       src_is_shared ? MTYPE_M : rtype, 
					       (consistency == STRICT_CONSISTENCY), 
					       (actual_shared_ptr_idx == pshared_ptr_idx));
  if (src_is_shared)
    WN_intrinsic(call_wn) = INTRN_UPC_MEMCPY;

  WN_kid0 (call_wn) = arg0;

  if (!src_is_shared)
    WN_kid1(call_wn) = WN_CreateParm(Integer_type, 
				     has_offt ? offt : 
				     WN_Intconst(Integer_type, offset),
				     MTYPE_To_TY(Integer_type), WN_PARM_BY_VALUE);
  else  
    WN_kid1(call_wn) = WN_CreateParm(TY_mtype(actual_shared_ptr_idx), Strip_TAS(WN_kid0(st)),
				     actual_shared_ptr_idx, WN_PARM_BY_VALUE);

  if (src_is_shared) {
    WN_kid2(call_wn) = WN_CreateParm(Integer_type,  
				     (WN_operator(st) == OPR_MSTORE || WN_operator(st) == OPR_ISTORE ) ? 
				     WN_COPY_Tree(WN_kid2(st)) : 
				     WN_Intconst(Integer_type,
						 TY_size(actual_shared_ptr_idx)),
				     MTYPE_To_TY(Integer_type), WN_PARM_BY_VALUE);
  } else {
    WN *value = WN_kid0(st);
    TY_IDX parm_ty_idx = (WN_ty(value)) ? WN_ty(value) : MTYPE_To_TY(WN_rtype(value));
    if(TY_kind(parm_ty_idx) != KIND_SCALAR && Type_Is_Shared_Ptr(parm_ty_idx))
      transfer_size = TY_size(TY_To_Sptr_Idx(parm_ty_idx));
    value = Strip_TAS(value);
    //WEI: the old code was testing WN_rtype(st), fix it here
    if (WN_rtype(value) != MTYPE_M && rtype != MTYPE_M)
      WN_kid2 (call_wn) = WN_CreateParm(Mtype_comparison(WN_rtype(value)), value,
					parm_ty_idx, WN_PARM_BY_VALUE);
    else { // need to extract the address of the symbol accessed by value
      if (WN_operator(value) == OPR_LDID && WN_st_idx(value) != 0) {
	ST *tst;
	//need to check here for the special case of ldid of return_preg
	//can't take it's address - spill
	if(ST_class(WN_st(value)) == CLASS_PREG) {
	 //  fprintf(stderr, "BEFORE SPILL \n");
// 	  fdump_tree(stderr, value);
// 	  value = Spill_Shared_Load(value);
// 	  fprintf(stderr, "AFTER  SPILL \n");
// 	  fdump_tree(stderr, value);
// 	  tst = WN_st(WN_kid1(value));
// 	  spilled_preg_ldid = value;
	  Is_True(0,("",""));
	} else 
	  tst = WN_st(value);
	

	WN_kid2(call_wn) = WN_CreateParm(Pointer_Mtype, 
					 WN_Lda(Pointer_Mtype, 0, tst, 0),
					 MTYPE_To_TY(Pointer_Mtype), WN_PARM_BY_VALUE);
      } else if(WN_operator(value) == OPR_COMMA && 
		WN_operator(WN_kid1(value)) == OPR_LDID){
	WN *kid1 = WN_kid1(value);
	WN *kid0 = WN_kid0(value);
	Is_True(WN_st(kid1), ("",""));
	
	value = WN_CreateComma (OPR_COMMA, Pointer_Mtype, MTYPE_V,
				kid0, WN_Lda(Pointer_Mtype, 0, WN_st(kid1), 0));
	WN_kid2(call_wn) = WN_CreateParm(Pointer_Mtype, value,
					 MTYPE_To_TY(Pointer_Mtype), WN_PARM_BY_VALUE);
      }  else if (WN_operator(value) == OPR_INTCONST) {
	TYPE_ID desc = WN_rtype(value);
	Is_True((!_64_bit_target && TY_size(MTYPE_To_TY(desc)) > 4) ||
		/* check for NULL ptr assignment */
		(WN_operator(st) != OPR_STID && Type_Is_Shared_Ptr(TY_pointed(WN_ty(st)), TRUE)), ("",""));
	
	//NULL ptr assignment 
	if (TY_kind(WN_ty(st)) == KIND_POINTER && Type_Is_Shared_Ptr(TY_pointed(WN_ty(st)), TRUE)) {
	  return WN_SetNull_Sptr(st);
	} 
	//this is for constants assigned to 64 bit int values on 32 bit targets
	//spill the const into a variable and pass the address into the assignment.
	init_wn = WN_CreateBlock ();
	handle_st = Gen_Temp_Symbol(MTYPE_To_TY(desc), (char*) ".spill64ct");
	WN_INSERT_BlockLast (init_wn, WN_Stid (desc, 0, handle_st, MTYPE_To_TY(desc), value));
	WN_kid2(call_wn) = WN_CreateParm(Pointer_Mtype, WN_Lda(Pointer_Mtype, 0, handle_st, 0),
					 MTYPE_To_TY(Pointer_Mtype), WN_PARM_BY_VALUE);
      } else { 
	WN_kid2(call_wn) = WN_CreateParm(Pointer_Mtype, Spill_And_Take_Address(value), 
					 MTYPE_To_TY(Pointer_Mtype), WN_PARM_BY_VALUE);
	
      }
    }
  }

  if (rtype != MTYPE_F8 && rtype != MTYPE_F4 && !src_is_shared ) { 
    Is_True((rtype == MTYPE_M && transfer_size != 0) || rtype != MTYPE_M, ("Illegal transfer size for remote store","")); 
    WN_kid3(call_wn) = WN_CreateParm(Integer_type, 
				     (WN_operator(st) == OPR_MSTORE) ?  WN_COPY_Tree(WN_kid2(st)):
				     WN_Intconst(Integer_type, (rtype == MTYPE_M) ? transfer_size : TY_size(MTYPE_To_TY(rtype))),
				     MTYPE_To_TY(Integer_type), WN_PARM_BY_VALUE);

  }  
  if(consistency == RELAXED_CONSISTENCY && !src_is_shared && 
     //no NB puts for FP
     rtype != MTYPE_F4 && rtype != MTYPE_F8) {
    TYPE_ID hdesc = TY_mtype(handle_ty_idx);
    wn1 = WN_Ldid (hdesc, -1, Return_Val_Preg, handle_ty_idx); 
    if(!init_wn)
      init_wn = WN_CreateBlock (); 
    WN_INSERT_BlockLast (init_wn, call_wn);
    init_wn = WN_CreateComma (OPR_COMMA, WN_rtype (wn1), MTYPE_V, init_wn, wn1);
    wn0 = WN_CreateBlock();
    call_wn = WN_Create (OPR_INTRINSIC_CALL, MTYPE_V, MTYPE_V, 1);
    WN_intrinsic(call_wn) = WN_Type_To_SyncIntrinsic(MTYPE_M); 
    
    wn1 = WN_CreateBlock();

    handle_st = Gen_Temp_Symbol(handle_ty_idx, (char*) ".Msync");
    WN_INSERT_BlockLast (wn1, WN_Stid (hdesc, 0, handle_st, handle_ty_idx, init_wn));

    handle_wn = WN_CreateLdid(OPR_LDID, hdesc,hdesc,
			      ST_ofst(handle_st), handle_st, handle_ty_idx, 0);
    
    wn1 = WN_CreateComma (OPR_COMMA, hdesc, MTYPE_V, wn1, handle_wn);
    WN_kid0(call_wn) = WN_CreateParm(hdesc, wn1, handle_ty_idx, WN_PARM_BY_VALUE);
    WN_INSERT_BlockLast (wn0, call_wn);
    call_wn = wn0;
  } else if (init_wn) {
    //this should happen only for strict puts with long long cts as srcs
    
    WN_INSERT_BlockLast (init_wn, call_wn);
    call_wn = init_wn;
  }

return call_wn;

}




WN*
WN_Create_Shared_Load( WN *ld, 
		       WN *dest, 
		       BOOL is_struct, 
		       WN_OFFSET xtra_offst, 
		       TY_IDX  access_ty, 
		       BOOL has_off, WN *off_wn)
{
  WN *call_wn;
  WN *src, *dst, *offt, *size;

  WN *init_wn = 0, *wn1 = 0, *wn2 = 0, *wn3 = 0;
  WN *handle_wn = 0, *wn0 = 0;
  INTRINSIC iop;
  TY_IDX desc_ty_idx;
  TY_IDX orig_ty_idx;
  TYPE_ID hdesc, rtype, desc;
  UINT orig_field_id = 0;
  UINT orig_offset = 0;
  CONSISTENCY_class consistency;
  TY_IDX handle_ty_idx = 0;
  ST *ret_st = 0;
  ST *handle_st = 0;
  WN *ldc;
  TY_IDX ret_ty; 
  TY_IDX sptr_idx = 0;
  UINT tmp_field_id = 0;
  WN_OFFSET offset = 0;
  BOOL _64_bit_target = TY_size(MTYPE_To_TY(Pointer_type)) > 4;

  // fprintf(stderr,"Shared Load IN\n");
//   fdump_tree(stderr,ld);
  
  consistency = Get_Access_Consistency(WN_ty(ld));
  rtype = WN_rtype(ld);
  desc = WN_desc(ld);
  
  if(WN_operator(ld) == OPR_TAS)
    ld = Strip_TAS(ld);

  switch (WN_operator(ld)) {
  case OPR_LDID:
    ldc = WN_COPY_Tree(ld);
    orig_field_id = WN_field_id(ldc);
    
    offset = WN_field_id(ld) > 1 ? Adjust_Field_Offset(WN_ty(ld), WN_field_id(ld)) : WN_offset(ld) + xtra_offst;
    
    // modify on the fly the type of the argument, otherwise
    // can't create parm node
    // have to set it back at the end
    sptr_idx = TY_To_Sptr_Idx(WN_ty(ld));
    WN_set_ty(ldc, sptr_idx);
    WN_set_rtype(ldc, TY_mtype(sptr_idx));
    WN_set_desc (ldc, TY_mtype(sptr_idx));
    WN_set_field_id(ldc, 0);
    WN_offset(ldc) = 0;
    ret_ty = WN_object_ty(ld);
    //shared ptr to shared
    if ((TY_kind(WN_ty(ld)) == KIND_POINTER || 
	 TY_kind(WN_ty(ld)) == KIND_ARRAY )   && TY_is_shared(WN_ty(ld))) 
      {
	TY_IDX tmp_idx = WN_ty(ld);
	if(WN_field_id(ld)) 
	  tmp_idx = Get_Field_Type(tmp_idx, WN_field_id(ld)); 
	if(TY_kind(tmp_idx) == KIND_POINTER) {
	  if (Type_Is_Shared_Ptr(TY_pointed(tmp_idx))) {
	    tmp_idx = TY_pointed(tmp_idx);
	    rtype = desc = TY_mtype(TY_To_Sptr_Idx(tmp_idx));
	    is_struct = (rtype == MTYPE_M);
	    ret_ty = TY_To_Sptr_Idx(tmp_idx);
	  } else {
	    ret_ty = Make_Pointer_Type(TY_pointed(tmp_idx));
	    rtype = desc = TY_mtype(ret_ty);
	  }
	} else if(TY_kind(tmp_idx) == KIND_ARRAY) {
	  Is_True(0,("",""));
	}    

      } else if (Type_Is_Shared_Ptr(ret_ty, TRUE)) {
	ret_ty = TY_To_Sptr_Idx(ret_ty);
	rtype = desc = TY_mtype(ret_ty);
	is_struct = (rtype == MTYPE_M);
      }
      
    break;
    
  case OPR_MLOAD:
  case OPR_ILOAD:
    ldc  = WN_COPY_Tree(WN_kid0(ld));
   //  fdump_tree(stderr, ldc);
    offset = WN_offset(ld) + xtra_offst;
    if (WN_field_id(ld)) { 
      ret_ty = FLD_type(FLD_get_to_field(WN_object_ty(ld), WN_field_id(ld), tmp_field_id));
    } else 
       ret_ty = WN_object_ty(ld);
    
    sptr_idx = (WN_operator(ld) == OPR_ILOAD) ? TY_To_Sptr_Idx(WN_load_addr_ty(ld)) : TY_To_Sptr_Idx(WN_ty(ld));

    if((TY_is_shared(WN_ty(ld)) &&   
	(TY_kind(WN_ty(ld)) == KIND_POINTER ||TY_kind(WN_ty(ld)) == KIND_ARRAY))
       //  && !WN_field_id(ld)
       ) {
      TY_IDX tmp_idx = ret_ty;
      if(TY_kind(tmp_idx) == KIND_POINTER) {
	  if (Type_Is_Shared_Ptr(TY_pointed(tmp_idx))) {
	    tmp_idx = TY_pointed(tmp_idx);
	    rtype = desc = TY_mtype(TY_To_Sptr_Idx(tmp_idx));
	    is_struct = (rtype == MTYPE_M);
	    ret_ty = TY_To_Sptr_Idx(tmp_idx);
	  } else {
	    ret_ty = Make_Pointer_Type(TY_pointed(tmp_idx));
	    rtype = desc = TY_mtype(ret_ty);
	  }
	} else if(TY_kind(tmp_idx) == KIND_ARRAY) {
	  Is_True(0,("",""));
	}    
    } if(Type_Is_Shared_Ptr(ret_ty, TRUE)) {
      ret_ty = TY_To_Sptr_Idx(ret_ty);
      rtype = desc = TY_mtype(ret_ty);
      is_struct = (rtype == MTYPE_M);
    }     

    break;
    
  case OPR_COMMA:
    Is_True(WN_rtype(ld) == TY_mtype(shared_ptr_idx) || WN_rtype(ld) == TY_mtype(pshared_ptr_idx),
	    ("Bad rtype for shared ld src",""));
    ldc = WN_COPY_Tree(ld);
    offset += xtra_offst;
    ret_ty = access_ty;
    sptr_idx = TY_To_Sptr_Idx(WN_ty(WN_kid1(ld)));
    break;
  default:
    Is_True(0,("Not implemented yet",""));
  }

  if (is_struct) {
    rtype = MTYPE_M;
    desc = MTYPE_M;
  } else if (!_64_bit_target && TY_size(MTYPE_To_TY(rtype)) > 4 && rtype != MTYPE_F8)
    {
      rtype = MTYPE_M;
      desc = MTYPE_M;
      is_struct = TRUE;
    } else   
      if ((rtype == MTYPE_F8 || rtype == MTYPE_F4) && consistency == RELAXED_CONSISTENCY) { 
	rtype = MTYPE_M;  
	desc = MTYPE_M;
	is_struct = TRUE;
	// consistency = STRICT_CONSISTENCY;
      } 

  iop =  WN_Type_To_Intrinsic(OPR_LDID  , dest ? MTYPE_M : rtype, 
			      consistency == STRICT_CONSISTENCY, (sptr_idx == pshared_ptr_idx));
  if (consistency == STRICT_CONSISTENCY) {
    /* strict load -> blocking call */
    call_wn = WN_Create (OPR_INTRINSIC_CALL, 
			 (rtype == MTYPE_M) ? MTYPE_V : Shared_Load_Extend_Mtyp(rtype),  
			 (rtype == MTYPE_M) ? MTYPE_V : Shared_Load_Extend_Mtyp(desc), 
			 (rtype == MTYPE_M || dest) ? 4 : 
			 (rtype == MTYPE_F4 || rtype == MTYPE_F8) ? 2 : 3);    
  } else {
    /* relaxed load - non-blocking load */
      if (rtype < MTYPE_F8) {
	hdesc = upc_hsync_reg_ty ? TY_mtype (upc_hsync_reg_ty) : MTYPE_I8;   
	handle_ty_idx =  upc_hsync_reg_ty ? upc_hsync_reg_ty : MTYPE_To_TY(MTYPE_I8);
      } else { 
	hdesc = upc_hsync_mem_ty ? TY_mtype (upc_hsync_mem_ty) : MTYPE_I8; 
	handle_ty_idx  = upc_hsync_mem_ty ? upc_hsync_mem_ty : MTYPE_To_TY(MTYPE_I8); 
      }
      call_wn = WN_Create (OPR_INTRINSIC_CALL, hdesc,  hdesc, 
			   (rtype == MTYPE_M) ? 4 : 
			   (rtype == MTYPE_F4 || rtype == MTYPE_F8) ? 2 : 3);
  }

  WN_intrinsic(call_wn) = iop; 
    
  if(has_off) 
    offt = WN_CreateParm(Integer_type, off_wn,  MTYPE_To_TY(Integer_type), WN_PARM_BY_VALUE);
    else
    offt = WN_CreateParm(Integer_type, WN_Intconst(Integer_type, offset),
			 MTYPE_To_TY(Integer_type), WN_PARM_BY_VALUE);
  
  size = WN_CreateParm(Integer_type, (WN_operator(ld)) == OPR_MLOAD ? WN_COPY_Tree(WN_kid1(ld)) :
		       WN_Intconst(Integer_type, 
				   dest ? (access_ty ? TY_size(access_ty) :TY_size(TY_pointed(WN_ty(dest)))) : 
				   TY_size(ret_ty)),
		       MTYPE_To_TY(Integer_type), WN_PARM_BY_VALUE);
  src = WN_CreateParm(TY_mtype(sptr_idx), Strip_TAS(ldc),
		      sptr_idx, WN_PARM_BY_VALUE);
  
  
  if(TY_kind(ret_ty) == KIND_SCALAR){
    ret_ty = MTYPE_To_TY(TY_mtype(ret_ty));
  }  else if(TY_kind(ret_ty) == KIND_STRUCT && TY_is_shared(ret_ty))
    // Is_True(0,("",""))
      ;

  if (dest) {
    WN_kid0(call_wn) = WN_CreateParm(Pointer_Mtype, WN_COPY_Tree(dest), MTYPE_To_TY(Pointer_Mtype),
				     WN_PARM_BY_VALUE);
    WN_kid1(call_wn) = src;
    WN_kid2(call_wn) = offt;
    WN_kid3(call_wn) = size;
    if(consistency == STRICT_CONSISTENCY)
      return call_wn;
  } else {
    // for shared ptr to shared 
    if(is_struct || (rtype == MTYPE_M && desc == MTYPE_M)) {
      Is_True(ret_ty == shared_ptr_idx || ret_ty == pshared_ptr_idx  ||
	      (!_64_bit_target && TY_size(ret_ty) > 4) ||
	      (consistency == RELAXED_CONSISTENCY && 
	       (TY_mtype(ret_ty) == MTYPE_F4 || TY_mtype(ret_ty) == MTYPE_F8)) , ("",""));
      
      
      //the type of the temp is given by the the type of the point-to
      //type of the load
      TY_IDX ret_ty1 = WN_ty(ld);
      switch(TY_kind(ret_ty1)) {
      case KIND_POINTER:
	ret_ty1 = TY_To_Sptr_Idx(TY_pointed(ret_ty1));
	break;
      case KIND_ARRAY:
	Is_True(0,("",""));
	break;
      case KIND_SCALAR:
      case KIND_STRUCT:
	ret_ty1 = ret_ty;
	break;
      default:
	Is_True(0,("",""));
	
      }
      ret_st = Gen_Temp_Symbol(ret_ty1, (char*) ".spillld");
      WN_kid0(call_wn) = WN_CreateParm(Pointer_Mtype, WN_Lda(Pointer_Mtype, 0, ret_st,0),
				       MTYPE_To_TY(Pointer_Mtype),
				       WN_PARM_BY_VALUE);
      WN_kid1(call_wn) = src;
      WN_kid2(call_wn) = offt;
      WN_kid3(call_wn) = size;
    } else {
      WN_kid0(call_wn) = src;
      WN_kid1(call_wn) = offt;
    }
  }

  
  
  

  init_wn = WN_CreateBlock ();
  WN_INSERT_BlockLast (init_wn, call_wn);


  if (rtype < MTYPE_M) 
    if( consistency == STRICT_CONSISTENCY)
      wn1 = WN_Ldid (rtype, -1, Return_Val_Preg,  ret_ty);
    else {
      wn1 = WN_Ldid (TY_mtype(handle_ty_idx), -1, Return_Val_Preg, handle_ty_idx); 
    }
  else {
    if(consistency == RELAXED_CONSISTENCY)
      wn1 = WN_Ldid (TY_mtype(handle_ty_idx), -1, Return_Val_Preg, handle_ty_idx); 
    else {
      Is_True(ret_st, ("",""));
      return WN_CreateComma(OPR_COMMA, TY_mtype(ret_ty), MTYPE_V, init_wn, 
			     WN_Ldid (TY_mtype(ret_ty), 0, ret_st, ret_ty));
    }
  }

  switch (rtype) {
  case  MTYPE_B:        
  case MTYPE_I1:        
  case MTYPE_I2:    
  case MTYPE_I4:       
  case MTYPE_I8:       
  case MTYPE_U1:        
  case MTYPE_U2:        
  case MTYPE_U4:       
  case MTYPE_U8:
    WN_kid2(call_wn) = size;
    break;
  default:
    break;
  }
  
  
  if (consistency == STRICT_CONSISTENCY) {
    call_wn  = WN_CreateComma (OPR_COMMA, WN_rtype (wn1), MTYPE_V, init_wn, wn1);
    // Is_True(rtype == MTYPE_M, ("",""));
    goto spill;
  } else {
    init_wn = WN_CreateComma (OPR_COMMA, WN_rtype (wn1), MTYPE_V, init_wn, wn1);
    wn0 = WN_CreateBlock();
    call_wn = WN_Create (OPR_INTRINSIC_CALL, (rtype < MTYPE_M) ? rtype : MTYPE_V, MTYPE_V, 1);
    WN_intrinsic(call_wn) = WN_Type_To_SyncIntrinsic(rtype); 
    //WEI: fix rtype here, since we use wait_syncnb for float and double now
    if (WN_intrinsic(call_wn) == INTRN_WAIT_SYNC_MEM) {
      WN_set_rtype(call_wn, MTYPE_V);
    }
    
    wn1 = WN_CreateBlock();

    handle_st = Gen_Temp_Symbol(handle_ty_idx, (char*) ".Msync");
    WN_INSERT_BlockLast (wn1, WN_Stid (TY_mtype(handle_ty_idx), 0, handle_st, 
				       handle_ty_idx, init_wn));

    
    handle_wn = WN_CreateLdid(OPR_LDID, hdesc, hdesc, ST_ofst(handle_st), handle_st, handle_ty_idx, 0);
    
    wn1 = WN_CreateComma (OPR_COMMA, hdesc, MTYPE_V, wn1, handle_wn);
    WN_kid0(call_wn) = WN_CreateParm(hdesc, wn1, handle_ty_idx, WN_PARM_BY_VALUE);
    WN_INSERT_BlockLast (wn0, call_wn);
    if (rtype < MTYPE_M) {
      wn2 = WN_Ldid (rtype, -1, Return_Val_Preg, ret_ty);
      call_wn = WN_CreateComma(OPR_COMMA, WN_rtype(wn2), MTYPE_V, wn0, wn2);
    } else 
      call_wn = wn0;
  }

spill:
  if (rtype < MTYPE_M || (is_struct && !dest)) {
    wn1 = WN_CreateBlock();
    if (rtype < MTYPE_M) {
      
      //for assignments whirl2c casts lhs to a pointer with the rhs type
      //Until we fix that - generate the temp with the register size/type
      //rather than the lhs type.
      if(rtype != MTYPE_F4 && rtype != MTYPE_F8) {
// 	ret_ty = ( _64_bit_target) ? 
// 	  (MTYPE_is_unsigned(rtype) ? MTYPE_To_TY(MTYPE_U8) : MTYPE_To_TY(MTYPE_I8)) :
// 	  (MTYPE_is_unsigned(rtype) ? MTYPE_To_TY(MTYPE_U4) :MTYPE_To_TY(MTYPE_I4) )
	  ;
      }
      ret_st = Gen_Temp_Symbol(ret_ty, (char*) ".Tspilload");
      WN_INSERT_BlockLast (wn1, WN_Stid (TY_mtype(ret_ty), 0, ret_st, 
					 ret_ty, call_wn));
      rtype = Widen_Mtype(TY_mtype(ret_ty));
      if (MTYPE_is_integral(desc)) {
	if (MTYPE_signed(rtype) != MTYPE_signed(desc)) {
	  if (MTYPE_size_min(rtype) > MTYPE_size_min(desc))
	    rtype = Mtype_TransferSign(desc, rtype);
	  else desc = Mtype_TransferSign(rtype, desc);
	}
      }
    } else WN_INSERT_BlockLast (wn1, call_wn);
    
    
    if(TY_kind(ret_ty) == KIND_SCALAR && rtype != MTYPE_F4 && rtype != MTYPE_F8 ) {
      if(consistency == RELAXED_CONSISTENCY && rtype == MTYPE_M && 
	 (TY_mtype(ret_ty) == MTYPE_F4 || TY_mtype(ret_ty) == MTYPE_F8)) 
	rtype = desc = TY_mtype(ret_ty);
      else 
	rtype // = desc
	  = // TY_mtype(ret_ty)
	  ( _64_bit_target) ? 
 	  (MTYPE_is_unsigned(rtype) ? MTYPE_U8 : MTYPE_I8) :
 	  (MTYPE_is_unsigned(rtype) ? MTYPE_U4 :MTYPE_I4 )	;
    } else if (TY_kind(ret_ty) == KIND_SCALAR && (rtype == MTYPE_F4 ||  rtype == MTYPE_F8))
      rtype = desc = TY_mtype(ret_ty);
      
     call_wn  = WN_CreateComma (OPR_COMMA, rtype, MTYPE_V, wn1,
			       WN_CreateLdid(OPR_LDID, rtype, desc, ST_ofst(ret_st), 
					     ret_st, ret_ty, 0) );
    
  }
  
  // fprintf(stderr,"Shared Load OUT");
//   fdump_tree(stderr, call_wn);
   
 return call_wn;
}  




// assumes lowering already called on the children of the ADD/SUB
// i.e. base returns a shared_ptr_t
WN*
WN_Create_Shared_Ptr_Arithmetic( WN *base, WN *disp, OPERATOR opr, 
				 int esize, int bsize, int phaseless)
{
 //  fprintf(stderr,"Shared Ptr Arith IN : ");
//   fdump_tree(stderr, base);
//   fprintf(stderr,"PTR ARITH DISP : ");
//   fdump_tree(stderr, disp);
  
  WN *call_wn;
  TY_IDX actual_shared_ptr_idx = (bsize <= 1) ? pshared_ptr_idx : shared_ptr_idx;
  TYPE_ID rtype = TY_mtype(actual_shared_ptr_idx);

  call_wn = WN_Create (OPR_INTRINSIC_CALL,
		       (rtype == MTYPE_M) ? MTYPE_M : Shared_Load_Extend_Mtyp(rtype),  
		       MTYPE_V,  (bsize <= 1) ? 3 : 4 );    
  WN_intrinsic(call_wn) = WN_Operator_To_Intrinsic (opr, bsize);
  
  if(WN_operator(base) == OPR_TAS)
    base = WN_kid0(base);
  
  WN_kid0(call_wn) =  WN_CreateParm(TY_mtype(actual_shared_ptr_idx), base, 
				    actual_shared_ptr_idx, WN_PARM_BY_VALUE);
  TY_IDX tidx = WN_ty(base);
  WN_kid1(call_wn) =  WN_CreateParm(Integer_type,  
                                 WN_Intconst(Integer_type, esize),
                                 MTYPE_To_TY(Integer_type), 
                                 WN_PARM_BY_VALUE);  

  WN_kid2(call_wn) = WN_CreateParm(WN_rtype(disp), disp,
				MTYPE_To_TY(WN_rtype(disp)),
				WN_PARM_BY_VALUE);
  if (bsize > 1)
    WN_kid3(call_wn) =  WN_CreateParm(Integer_type, 
		    WN_Intconst(Integer_type,  bsize),
		    MTYPE_To_TY(Integer_type), WN_PARM_BY_VALUE);
  
  WN *wn0 = WN_CreateBlock();
  WN_INSERT_BlockLast (wn0, call_wn); 
  WN *wn1 = WN_Ldid(rtype, -1, Return_Val_Preg, actual_shared_ptr_idx);
  WN *wn2;
  
  ST *ret_st;
  ret_st = Gen_Temp_Symbol(actual_shared_ptr_idx, (char*) ".Mreturn.");
  wn1 = WN_Stid (TY_mtype(actual_shared_ptr_idx), 0, ret_st, actual_shared_ptr_idx, wn1);
  WN_INSERT_BlockLast (wn0, wn1);
  wn2 = WN_CreateLdid(OPR_LDID, rtype, rtype, ST_ofst(ret_st),
		      ret_st, actual_shared_ptr_idx, 0);
 
  
  wn1 = WN_CreateComma (OPR_COMMA, WN_rtype (wn2), MTYPE_V, wn0, wn2);
  // fprintf(stderr,"Shared Ptr Arith OUT : ");
//   fdump_tree(stderr, wn1);
  return wn1;
} 


WN*
WN_Convert_Shared_To_Local ( WN *ptr, TY_IDX ty, ST* st)
{
  int st0 = 0;


  // A TAS here was artificially generated in the front-end only to
   // pass the type info.
  if(WN_operator(ptr) == OPR_TAS)
    ptr = WN_kid0(ptr);
  
  TY_IDX sptr_idx = WN_ty(ptr);

  if (!sptr_idx) {
    Is_True(WN_operator(ptr) == OPR_COMMA, 
	    ("Can't determine the the implementation type of shared ptr in cast to local",""));
    sptr_idx = TY_To_Sptr_Idx(WN_ty(WN_kid1(ptr)));
  }

  if(!st /*&& TY_mtype(sptr_idx) != MTYPE_M */) {
    // cast to local in the middle of an expression
    if(ty == 0)
      ty = MTYPE_To_TY(Pointer_Mtype);
    st = Gen_Temp_Symbol(ty, (char*) ".Mcvtptr.");
    st0 = 1;
  }
   
 
  WN *call_wn  = WN_Create (OPR_INTRINSIC_CALL, Pointer_Mtype, MTYPE_V, 1);
  if (sptr_idx == shared_ptr_idx)
    WN_intrinsic (call_wn) = INTRN_SCVTADDR;
  else 
    WN_intrinsic (call_wn) = INTRN_P_TO_L;


  WN_kid0 (call_wn) = WN_CreateParm(TY_mtype(sptr_idx), ptr, sptr_idx, WN_PARM_BY_VALUE);

  WN *wn0 = WN_CreateBlock();
  WN_INSERT_BlockLast (wn0, call_wn);
  WN *wn1 = WN_Ldid(Pointer_Mtype, -1, Return_Val_Preg, ty);
  call_wn  = WN_CreateComma (OPR_COMMA, WN_rtype (wn1), MTYPE_V, wn0, wn1);
  if(st0) {
    call_wn = WN_CreateStid(OPR_STID, MTYPE_V, Pointer_Mtype, 0, st, ty, call_wn, 0);
    wn0 = WN_CreateBlock();
    WN_INSERT_BlockLast(wn0, call_wn);
    wn1 = WN_Ldid(TY_mtype(ty), 0, st, ty, 0);
    call_wn = WN_CreateComma(OPR_COMMA, TY_mtype(ty), MTYPE_V, wn0, wn1);
  }

  // fprintf(stderr, "SHARED_TO_LOCAL \n");
//   fdump_tree(stderr, call_wn);
  return call_wn;
}


WN *
WN_Create_PtrEq_Test(OPERATOR op, WN *awn0, WN *awn1, TY_IDX idx0, TY_IDX idx1) 
{
  INTRINSIC iop = INTRN_EQ_P_P;
  BOOL negate = FALSE;
  UINT args = 2;
  
  
  WN *arg0 = awn0;
  WN *arg1 = awn1;
  TY_IDX tmp;
  
  if (Type_Is_Shared_Ptr(idx0))
    idx0 = TY_To_Sptr_Idx (idx0);
  else {
    if(WN_operator(arg0) == OPR_LDID)
      idx0 = TY_To_Sptr_Idx(WN_ty(arg0));
  }
  
  if (Type_Is_Shared_Ptr(idx1))
    idx1 = TY_To_Sptr_Idx (idx1);
  else {
    if(WN_operator(arg1) == OPR_LDID)
      idx1 = TY_To_Sptr_Idx(WN_ty(arg1));
  }
  
  iop = WN_Operator_To_Intrinsic(OPR_EQ, idx0, idx1);
  
  if(WN_operator(awn0) == OPR_INTCONST) {
    args = 1;
    arg0 = awn1;
  } else if (WN_operator(awn1) == OPR_INTCONST) {
    args = 1;
    arg0 = awn0;
  } else if(idx0 == pshared_ptr_idx) {
    arg0 = awn1;
    arg1 = awn0;
    tmp = idx0;
    idx0 = idx1;
    idx1 = tmp;
    
  }
      
  switch (op) {
  case OPR_EQ:
    break;
  case OPR_NE:
    negate = TRUE;
    break;
  default:
    Is_True(0,("",""));
  }
 
  WN *call_wn = WN_Create(OPR_INTRINSIC_CALL, MTYPE_I4, MTYPE_V, args);
  WN_intrinsic(call_wn) = iop;

  WN_kid0(call_wn) = WN_CreateParm(TY_mtype(idx0), arg0, idx0, WN_PARM_BY_VALUE);
  
  if(args > 1) {
    WN_kid1(call_wn) = WN_CreateParm(TY_mtype(idx1), arg1, idx1, WN_PARM_BY_VALUE);
  }
  
  WN *wn0 = WN_CreateBlock();
  WN_INSERT_BlockLast (wn0, call_wn);
  WN *wn1 = WN_Ldid(MTYPE_I4, -1, Return_Val_Preg, MTYPE_To_TY(MTYPE_I4));
   
  wn1 = WN_CreateComma (OPR_COMMA, WN_rtype (wn1), MTYPE_V, wn0, wn1);
  if (negate)
      wn1 = WN_LNOT(wn1);

  ST *st = Gen_Temp_Symbol(MTYPE_To_TY(MTYPE_I4), (char*) ".Mptreq.");  
  idx0 = MTYPE_To_TY(MTYPE_I4);
  wn1 = WN_CreateStid(OPR_STID, MTYPE_V, MTYPE_I4, 0, st, idx0, wn1, 0);
  wn0 = WN_CreateBlock();
  WN_INSERT_BlockLast(wn0, wn1);
  wn1 = WN_Ldid(MTYPE_I4, 0, st, idx0, 0);
  wn1 = WN_CreateComma(OPR_COMMA, MTYPE_I4, MTYPE_V, wn0, wn1);
 
  return  wn1;
}


WN*
WN_Create_StoP_Cvt(WN *init_wn, INTRINSIC iop)
{
  
  TY_IDX ret_ty;
  TY_IDX arg_ty;

  switch(iop) {
  case INTRN_S_RESET:
    ret_ty = arg_ty = shared_ptr_idx;
    break;
  case INTRN_S_TO_P:
    ret_ty = pshared_ptr_idx;
    arg_ty = shared_ptr_idx;
    break;
  case INTRN_P_TO_S:
    ret_ty = shared_ptr_idx;
    arg_ty = pshared_ptr_idx;
    break;
  case INTRINSIC_LAST:
    return init_wn;
  default:
    Is_True(0, ("Bad intrinsic value in PtoS_Cvt",""));
  }

  WN *call_wn = WN_Create(OPR_INTRINSIC_CALL, TY_mtype(ret_ty),
			  MTYPE_V, 1);
  WN_intrinsic(call_wn) = iop;
  WN_kid0(call_wn) = 
    WN_CreateParm(TY_mtype(arg_ty), init_wn, arg_ty, WN_PARM_BY_VALUE);

  TY_IDX ty_idx1 = shared_ptr_idx;
  TY_IDX component_ty_idx = shared_ptr_idx;
  TY_IDX nop_ty_idx = shared_ptr_idx;
  
  WN *wn0 = WN_CreateBlock();
  WN_INSERT_BlockLast (wn0, call_wn);
  WN *wn1 = WN_Ldid(TY_mtype(ret_ty), -1, Return_Val_Preg, ret_ty);
   
  ST *ret_st = Gen_Temp_Symbol(ret_ty, (char*) ".Mstopcvt.");
  wn1 = WN_Stid (TY_mtype(ret_ty), 0, ret_st, ret_ty, wn1);
  WN_INSERT_BlockLast (wn0, wn1);
  wn1 = WN_CreateLdid(OPR_LDID, TY_mtype(ret_ty), TY_mtype(ret_ty), 0,
		      ret_st, ret_ty, 0);
  
  wn1 = WN_CreateComma (OPR_COMMA, WN_rtype (wn1), MTYPE_V, wn0, wn1);

  return  wn1;

}



WN *
Spill_Shared_Load( WN *ld)
{
 //  fdump_tree(stderr, ld);
  Is_True(WN_operator(ld) == OPR_COMMA || WN_operator(ld) == OPR_LDID, ("",""));
  WN *block = WN_CreateBlock();
  
  TY_IDX idx = WN_ty(WN_operator(ld) == OPR_COMMA ? WN_kid1(ld) : ld);
  ST* ret_st = Gen_Temp_Symbol(idx, 
			    Index_To_Str(Save_Str2((char*)".Mreturn.",(char*)".Mreturn.")));
  WN_INSERT_BlockLast (block, WN_Stid (TY_mtype(idx), 0, ret_st, idx, ld));
  return  WN_CreateComma (OPR_COMMA, Widen_Mtype(TY_mtype(idx)),MTYPE_V, block, 
			  WN_Ldid(TY_mtype(idx), 0, ret_st, idx));
}


void LowerUPC_Init_Consistency()
{
  static int first = 1;
  
  if (first) {
    MEM_POOL_Initialize(&consistency_pool, "Memory consistency pool", FALSE);
    MEM_POOL_Push(&consistency_pool);
    consistency_map = WN_MAP_Create(&consistency_pool);
    first = 0;
  }
}


WN *
WN_Create_Shared_Ptr_Diff( WN *op0, WN *op1, TY_IDX t1, TY_IDX t2)
{

  WN *call_wn;
  
  TYPE_ID rtype = MTYPE_I4;
  TY_IDX ret_ty = MTYPE_To_TY(rtype);
  int esize = Get_Type_Inner_Size(t1);
  int bsize = Get_Type_Block_Size(t1);
  //t1, t2 are both guaranteed to be local pointer to shared data
  t1 = TY_pointed(t1);
  t2 = TY_pointed(t2);

  if (!TY_is_pshared(t1)) {
    call_wn = WN_Create (OPR_INTRINSIC_CALL, rtype, MTYPE_V,  4 );    
    WN_intrinsic(call_wn) = INTRN_SUB_S;
    
    WN_kid0(call_wn) =  WN_CreateParm(TY_mtype(shared_ptr_idx), op0, 
				      shared_ptr_idx, WN_PARM_BY_VALUE);
    if (TY_is_pshared(t2)) {
      op1 = WN_Create_StoP_Cvt(op1, INTRN_P_TO_S);
    }
    WN_kid1(call_wn) = WN_CreateParm(TY_mtype(shared_ptr_idx), op1,
				     shared_ptr_idx, WN_PARM_BY_VALUE);
    WN_kid2(call_wn) =  WN_CreateParm(Integer_type,  
				      WN_Intconst(Integer_type, esize),
				      MTYPE_To_TY(Integer_type), 
				      WN_PARM_BY_VALUE);  
    WN_kid3(call_wn) = WN_CreateParm(Integer_type, 
				     WN_Intconst(Integer_type,  bsize ? bsize: 1),
				     MTYPE_To_TY(Integer_type), WN_PARM_BY_VALUE);
  } else {
    call_wn = WN_Create (OPR_INTRINSIC_CALL, rtype, MTYPE_V,  3);    
    if (TY_block_size(t1) == 1) {
      WN_intrinsic(call_wn) = INTRN_SUB_P1;
    } else {
      WN_intrinsic(call_wn) = INTRN_SUB_PI;
    }
      WN_kid0 (call_wn) =  WN_CreateParm(TY_mtype(pshared_ptr_idx), op0, 
					 pshared_ptr_idx, WN_PARM_BY_VALUE);
    
      if (!TY_is_pshared(t2)) {
	op1 = WN_Create_StoP_Cvt(op1, INTRN_S_TO_P);
      }
      WN_kid1(call_wn) = WN_CreateParm(TY_mtype(pshared_ptr_idx), op1,
				       pshared_ptr_idx, WN_PARM_BY_VALUE);
      WN_kid2(call_wn) =  WN_CreateParm(Integer_type,  
					WN_Intconst(Integer_type, esize),
					MTYPE_To_TY(Integer_type), 
					WN_PARM_BY_VALUE);      
  }
  
  WN *wn0 = WN_CreateBlock();
  WN_INSERT_BlockLast (wn0, call_wn); 
  WN *wn1 = WN_Ldid(rtype, -1, Return_Val_Preg, ret_ty);
  WN *wn2;
  
  ST *ret_st;
  ret_st = Gen_Temp_Symbol(ret_ty, (char*) ".Mreturn.");
  wn1 = WN_Stid (rtype, 0, ret_st, ret_ty, wn1);
  WN_INSERT_BlockLast (wn0, wn1);
  wn2 = WN_CreateLdid(OPR_LDID, rtype, rtype, ST_ofst(ret_st),
		      ret_st, ret_ty, 0);
  
  wn1 = WN_CreateComma (OPR_COMMA, rtype, MTYPE_V, wn0, wn2);
  return wn1;
}



WN*
WN_SetNull_Sptr(WN *st) {
  
  WN *call_wn;
  TY_IDX actual_shared_ptr_idx;
  WN *arg;
  INTRINSIC iop;

  call_wn = WN_Create (OPR_INTRINSIC_CALL, MTYPE_V, MTYPE_V, 1);
  actual_shared_ptr_idx = TY_To_Sptr_Idx(WN_ty(st));
  switch (WN_operator(st)) {
  case OPR_STID:

    WN_kid0(call_wn) = WN_CreateParm(Pointer_Mtype, WN_Lda(Pointer_Mtype, 0, WN_st(st), 0),
					 MTYPE_To_TY(Pointer_Mtype), WN_PARM_BY_VALUE);
    // WN_kid0(call_wn) = WN_CreateParm(TY_mtype(actual_shared_ptr_idx), 
// 				     WN_CreateLdid(OPR_LDID, 
// 						   TY_mtype(actual_shared_ptr_idx), 
// 						   TY_mtype(actual_shared_ptr_idx),
// 						   0 , ST_st_idx(WN_st(st)),
// 						   actual_shared_ptr_idx, 0), 
// 				   actual_shared_ptr_idx, WN_PARM_BY_VALUE);
    WN_intrinsic(call_wn) = (actual_shared_ptr_idx == shared_ptr_idx) ? 
      INTRN_SETNULL_S : INTRN_SETNULL_P;
    break;
  case OPR_ISTORE:
  case OPR_MSTORE:
    if(actual_shared_ptr_idx == pshared_ptr_idx) {
      //need to convert to shared first
      WN_kid1(st) = WN_Create_StoP_Cvt(WN_kid1(st), INTRN_P_TO_S);
      actual_shared_ptr_idx = shared_ptr_idx;
    }
    
     WN_kid0(call_wn)  = WN_CreateParm(TY_mtype(actual_shared_ptr_idx), WN_kid1(st),
				       actual_shared_ptr_idx, 
				       WN_PARM_BY_VALUE);
     WN_intrinsic(call_wn) = (TY_To_Sptr_Idx(TY_pointed(WN_ty(st))) == shared_ptr_idx) ? 
       INTRN_SETNULL_S_R : INTRN_SETNULL_P_R;
     break;
  default:
    Is_True(0,("",""));
  }

  return call_wn;
}



WN*
Strip_TAS(WN *wn) 
{
  while(WN_operator(wn) == OPR_TAS)
    wn = WN_kid0(wn);
  return wn;
}

WN *Combine_Offset_Terms(SPTR_OFFSET_TERM_STACK &stack)
{
  if(stack.empty())
    return 0;
  WN *wn0 = stack.top();
  stack.pop();
  if(stack.empty())
      return wn0;
  WN *wn1 = stack.top();
  stack.pop();
  
  wn0 = WN_Binary (OPR_ADD, MTYPE_I4, wn0, wn1);

  while(!stack.empty()) {
    wn1 = stack.top();
    stack.pop();
    wn0 = WN_Binary (OPR_ADD, MTYPE_I4, wn0, wn1);
  }

  return wn0;
  
}


TY_IDX Get_Type_From_ArrayOp(WN *array_wn)
{
  
  WN *base = array_wn;
  TY_IDX result;

  if (WN_operator(array_wn) == OPR_ARRAY) {
    base = WN_array_base(array_wn);
    while(WN_operator(base) == OPR_ARRAY) {
      base = WN_array_base(base);
    }
  }
  
  if (WN_operator(base) == OPR_LDA)
    result = TY_pointed(WN_ty(base));
  else 
    result = WN_ty(base);

  return result;
}
