// -*-Mode: C++;-*-
#ifndef wn_attr_INCLUDED
#define wn_attr_INCLUDED

#include "Open64BasicTypes.h"


/**
 * Access-macros and access-functions for obtaining attributes of 
 * WN trees for general use in translating WN to another language
 * form (e.g. to C or Fortran).  For the most part, this module
 * supplements common/com/wn_core.h.
 */

/* several craylib/dope items represent a no-op by a zero inconst...*/
#define IS_IO_NULL_OPR(wn) ((WN_operator(wn) == OPR_INTCONST) && (WN_const_val(wn) == 0))

#define WN_IOITEM(x) ((IOITEM) WN_io_item(x))
#define WN_IOSTMT(x) ((IOSTATEMENT) WN_io_statement(x))

#define IS_IO_ITEM_IOU(item) \
   (WN_IOITEM(item) >= IOU_NONE && WN_IOITEM(item) <= IOU_INTERNAL)
#define IS_IO_ITEM_IOF(item) \
   ((WN_IOITEM(item) >= IOF_NONE && WN_IOITEM(item) <= IOF_CR_FMTSRC_DOPE))
#define IS_IO_ITEM_IOC(item) \
   (WN_IOITEM(item) >= IOC_ACCESS && WN_IOITEM(item) <= IOC_ERRFLAG)
#define IS_IO_ITEM_IOL(item) \
   ((WN_IOITEM(item) >= IOL_ARRAY && WN_IOITEM(item) <= IOL_VAR) || \
    (WN_IOITEM(item) == IOL_DOPE))

/**
 *  an ADRTMP intrinsic opcode.
 */
#define INTR_is_adrtmp(intrn) \
   ((intrn) == INTRN_U4I1ADRTMP || \
    (intrn) == INTRN_U4I2ADRTMP || \
    (intrn) == INTRN_U4I4ADRTMP || \
    (intrn) == INTRN_U4I8ADRTMP || \
    (intrn) == INTRN_U4F4ADRTMP || \
    (intrn) == INTRN_U4F8ADRTMP || \
    (intrn) == INTRN_U4FQADRTMP || \
    (intrn) == INTRN_U4C4ADRTMP || \
    (intrn) == INTRN_U4C8ADRTMP || \
    (intrn) == INTRN_U4CQADRTMP || \
    (intrn) == INTRN_U4VADRTMP  || \
    (intrn) == INTRN_U8I1ADRTMP || \
    (intrn) == INTRN_U8I2ADRTMP || \
    (intrn) == INTRN_U8I4ADRTMP || \
    (intrn) == INTRN_U8I8ADRTMP || \
    (intrn) == INTRN_U8F4ADRTMP || \
    (intrn) == INTRN_U8F8ADRTMP || \
    (intrn) == INTRN_U8FQADRTMP || \
    (intrn) == INTRN_U8C4ADRTMP || \
    (intrn) == INTRN_U8C8ADRTMP || \
    (intrn) == INTRN_U8CQADRTMP || \
    (intrn) == INTRN_U8VADRTMP)

/**
 * a VALTMP intrinsic opcode
 */
#define INTR_is_valtmp(intrn) \
   ((intrn) == INTRN_I4VALTMP || \
    (intrn) == INTRN_I8VALTMP || \
    (intrn) == INTRN_F4VALTMP || \
    (intrn) == INTRN_F8VALTMP || \
    (intrn) == INTRN_FQVALTMP || \
    (intrn) == INTRN_C4VALTMP || \
    (intrn) == INTRN_C8VALTMP || \
    (intrn) == INTRN_CQVALTMP)

#define WN_Skip_Parm(arg) \
   ((arg)!=NULL && WN_operator(arg) == OPR_PARM? WN_kid0(arg) : arg)


/**
 * The TY_IDX denoting the type of a WN expression.  For typeless
 * WNs (e.g. statements), the TY will be "void" in C terminology.
 * Similarly, there may be expression we cannot sensibly type,
 * e.g. for MLOADs, in which case we return an inaccurate type.
 * For MLOADs we return the type of a field at the given offset
 * and of the given size, provided the size is known statically;
 * otherwise we return the base-type.
 * Returns a TY_IDX that as closely as possible represents the type of the
 * given subexpression (wn).  Supports calls (both as statements and
 * expressions).  Also supports stores, returning the type of the
 * referenced object (possibly dereferencing a pointer type).
 * Note: TY_is_logical() will only hold true when the TY is resolved
 * from a WN_ty or ST_ty attribute, not when it is resolved from an
 * MTYPE.
 * Note: Pointer types may be created as a result of a call to this
 * routine.
 */
extern TY_IDX 
WN_Tree_Type(const WN *wn);

extern TY_IDX 
Get_Field_Type(TY_IDX base, int field_id);

/** 
 * For loads/stores: the ref object type is the type 
 * after all 'dereferencing' has been done,
 * e.g. for t%f it should be the type of the 
 * field f in type t
 */ 
extern TY_IDX 
WN_GetRefObjType(const WN* wn);

/**
 * For loads/stores: the base object type is the type
 * before any 'dereferencing' has been done,
 * e.g. for t%f it should be the type t
 */
extern TY_IDX 
WN_GetBaseObjType(const WN* wn);

/**
 * For expression sub trees get the type
 * This is being introduced as a last 
 * resort to get around the inscrutable paths 
 * taken by WN_[Tree_|GetRefObj|GetBaseObj]Type
 */
extern TY_IDX 
WN_GetExprType(const WN* wn);

extern TY_IDX 
WN_Call_Type(const WN* wn);

extern TY_IDX 
WN_Call_Return_Type(const WN* wn);

extern INT
WN_Call_First_Arg_Idx(const WN* wn);

extern INT
WN_Call_Last_Arg_Idx(const WN* wn);

/**
 * The name of the function or macro representing the intrinsic.
 * Special handling is needed when the name is NULL.
 */
extern const char * 
WN_intrinsic_name(INTRINSIC intr_opc);

/**
 * Map the intrinsic code to a return type.  If no return type
 * is known, then use the result type of the OPCODE as the
 * return type.
 */
extern TY_IDX 
WN_intrinsic_return_ty(const WN *call);

/**
 * TRUE when the given return type is returned through the first
 * parameter; FALSE when returning through pregs.
 */
extern BOOL 
WN_intrinsic_return_to_param(TY_IDX return_ty);

/**
 * Given the two operands of an ADD expression, see if this
 * can be considered a pointer addition, and if it is return
 * a pointer to an INTCONST that can be normalized w.r.t. the
 * size of the pointed_ty; if the pointed_ty has size 1, then
 * just return the integral expression operand when this should
 * be considered a pointer addition; otherwise return NULL when
 * this should not be considered a pointer addition.
 */
extern WN *
WN_Get_PtrAdd_Intconst(WN* wn0, WN* wn1, TY_IDX pointed_ty);

#endif 
