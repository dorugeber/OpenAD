#ifndef upc_wn_util_INCLUDED
#define upc_wn_util_INCLUDED

#include "wn.h"
#include <stack>
#include "mempool.h"
#include "wn_map.h"
#include "upc_symtab_utils.h"


typedef std::stack<WN*> SPTR_OFFSET_TERM_STACK;
typedef std::stack<SPTR_OFFSET_TERM_STACK*> SPTR_OFFSET_ACCUMULATION_STACK;
extern  SPTR_OFFSET_ACCUMULATION_STACK sptr_off_accumulation_stack;

typedef enum {
  ACCUMULATION,
  ARRAY_ACCUMULATION,
  NO_ARRAY_ACCUMULATION,
  NO_ACCUMULATION
} SPTR_ACCUMULATION_STATE;
extern SPTR_ACCUMULATION_STATE sptr_accumulation_state;
  


 
extern std::stack<CONSISTENCY_class> consistency_stack;
extern MEM_POOL consistency_pool;
extern WN_MAP consistency_map;

extern BOOL Fold_Keep_Shared_Tas;
extern WN* WN_Create_Shared_Load (WN *ld, 
				  WN *dest = 0, 
				  BOOL is_struct = FALSE,  
				  WN_OFFSET xtra_offst = 0,
				  TY_IDX  access_ty = 0,
				  BOOL has_off = FALSE,  WN *off_wn = 0);
extern WN* WN_Create_Shared_Store (WN *st, 
				   BOOL src_is_shared = FALSE,
				   WN_OFFSET xtra_offst = 0,
				   BOOL has_offt = FALSE, WN *offt = 0);
extern WN* WN_Create_Shared_Ptr_Arithmetic( WN *base, WN *disp, OPERATOR opr, 
					    int esize = 0, int bsize = 0, int phaseless = 0);
extern WN* WN_Convert_Shared_To_Local ( WN *ptr, TY_IDX ty = 0, ST* st = 0);
extern WN* WN_Create_PtrEq_Test(OPERATOR op, WN *awn0, WN *awn1, TY_IDX idx0, TY_IDX idx1);
extern WN* WN_Create_StoP_Cvt(WN *init_wn, INTRINSIC iop); 

//extern WN* WN_Create_Shared_Ptr_Diff( WN *op0, WN *op1, UINT esize, UINT bsize);
extern WN* WN_Create_Shared_Ptr_Diff( WN *op0, WN *op1, TY_IDX t1, TY_IDX t2);
extern WN* WN_SetNull_Sptr(WN *st);
extern WN* Strip_TAS(WN *);
extern WN* Spill_Shared_Load( WN *ld);
extern void LowerUPC_Init_Consistency();
extern WN *Combine_Offset_Terms(SPTR_OFFSET_TERM_STACK &stack);
extern TY_IDX Get_Type_From_ArrayOp(WN *array_wn);

#endif
                                                              
