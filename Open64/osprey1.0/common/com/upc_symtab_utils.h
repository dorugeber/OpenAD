/* -*-Mode: c++;-*- (Tell emacs to use c++ mode) */

#ifndef upc_symtab_utils_INCLUDED
#define upc_symtab_utils_INCLUDED


// #include <opcode.h>
#include <wintrinsic.h>
// #include <wn_core.h>
// #include <symtab_defs.h>

class WN;

typedef enum {
  STRICT_CONSISTENCY,
  RELAXED_CONSISTENCY,
  NO_CONSISTENCY
} CONSISTENCY_class;


/* UPC specific */
extern TY_IDX shared_ptr_idx;
extern TY_IDX pshared_ptr_idx;
extern int compiling_upc;


/* synchronization */

#define  UPCR_BARRIERFLAG_ANONYMOUS 1 /* keep this consistent with upcr.h */
#define UPCR_BARRIERVAL_ANONYMOUS 0xdeadbeef

#define MAX_LINE_LEN_UPC 100

// extern const char *upc_ptradd_name;
extern char *shared_ptr_name;
extern ST* upc_ptradd_st;
extern ST* upc_ptrdiff_st;
extern ST* upc_ptrget_st;
extern ST* upc_ptrput_st;
extern ST* upc_putblock_st;
extern ST* upc_getblock_st;
extern ST* upc_threadof_st;
extern ST* upc_mythread_st;
extern ST* upc_memget_st;
extern ST* upc_memput_st;
extern ST* upc_memcpy_st;
extern ST* upc_adddisp_st;
extern ST* upc_getshared_st;
extern ST* upc_phreset_st;
extern ST* upc_nulleq_st;
extern ST* upc_nullneq_st;
extern ST* upc_ptreqtest_st;
extern ST* upc_ptrneqtest_st;
extern TY_IDX upc_hsync_reg_ty;
extern TY_IDX upc_hsync_mem_ty;
extern ST* upc_forall_control_st;

extern void Save_Upc_Rts_Calls(char *name,ST *st);
extern void Upc_Translate_Name(char *from, char **to);
extern void Initialize_Upc_Metadata ();
extern void Initialize_Upc_Vars();
extern void Find_Upc_Vars();
extern void Verify_Upc_Metadata ();
extern void Initialize_Upc_Types(char *, UINT, char *, UINT, char*, UINT, char*, UINT);
extern void Upc_Lower_SymbolTable();
extern void Create_Special_Shared_Global_Symbols();
extern UINT64 Get_Type_Inner_Size (TY_IDX idx);
extern TY_IDX Get_Inner_Array_Type( TY_IDX idx);
extern TY_IDX TY_To_Sptr_Idx (TY_IDX);
extern UINT Get_Type_Block_Size (TY_IDX idx);
extern BOOL WN_Type_Is_Shared_Ptr (const WN* wn, BOOL real_ptr = FALSE);
extern BOOL Type_Is_Shared_Ptr (TY_IDX idx, BOOL real_ptr = FALSE);
extern TY_IDX Fix_Intrinsic_Return_Type(INTRINSIC i);
extern BOOL Types_Are_Equiv(TY_IDX idx1, TY_IDX idx2);
extern BOOL Need_StoP_Cvt(TY_IDX src_idx, TY_IDX dest_idx, INTRINSIC *iop);

extern BOOL TY_is_pshared(TY_IDX idx);
extern BOOL Use_32_Bit(const char* filename);
extern UINT Adjust_Field_Offset(TY_IDX struct_idx, UINT field_id);
extern UINT Adjust_Field_Offset(TY&  struct_idx, UINT field_id);
extern TY_IDX MTYPE_TO_Shared_TY_array[MTYPE_LAST+1];
extern TY_IDX Get_Field_Type (TY_IDX struct_type, UINT field_id);
extern UINT Adjusted_Type_Size(TY_IDX idx, unsigned int pshared = 0, unsigned int shared = 0);
extern bool is_upcr_ptr(TY_IDX ty);

#define MTYPE_To_Shared_TY(t)  MTYPE_TO_Shared_TY_array[t]

#endif /* upc_symtab_utils_INCLUDED */ 
