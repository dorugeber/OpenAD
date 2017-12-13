#include <assert.h>
#include <symtab.h>
#include <symtab_utils.h>
#include <upc_symtab_utils.h>
#include <wintrinsic.h>
#include "wn.h"


char *shared_ptr_name = "shared_ptr_struct";
char *hsync_reg_name;
char *hsync_mem_name;

TY_IDX shared_ptr_idx = 0; 
TY_IDX pshared_ptr_idx = 0;
TY_IDX upc_hsync_reg_ty = 0;
TY_IDX upc_hsync_mem_ty = 0;


ST *upc_ptradd_st = 0;
ST *upc_ptrdiff_st = 0;
ST *upc_ptrget_st = 0;
ST *upc_ptrput_st = 0;
ST *upc_threadof_st = 0;
ST *upc_mythread_st = 0;
ST *upc_memget_st = 0;
ST *upc_memput_st = 0;
ST *upc_memcpy_st = 0;
ST *upc_adddisp_st = 0;
ST *upc_getshared_st = 0;
ST *upc_phreset_st = 0;
ST* upc_nulleq_st = 0;
ST* upc_nullneq_st = 0;
ST* upc_ptreqtest_st = 0;
ST* upc_ptrneqtest_st = 0;
ST* upc_forall_control_st = 0;

TYPE_ID MTYPE_TO_Shared_TY_array[MTYPE_LAST+1];



static char *upc_library_names[] = {
  "upc_global_alloc",                "UPCR_GLOBAL_ALLOC",
  "upc_all_alloc",                   "UPCR_ALL_ALLOC",
  "upc_local_alloc",                 "UPCR_LOCAL_ALLOC",
  "upc_free",                        "UPCR_FREE",
  "upc_threadof",                    "UPCR_THREADOF_SHARED",
  "upc_phaseof",                     "UPCR_PHASEOF_SHARED",
  "upc_addrfield",                   "UPCR_ADDRFIELD_SHARED",
  "upc_global_lock_alloc",           "UPCR_GLOBAL_LOCK_ALLOC",
  "upc_all_lock_alloc",              "UPCR_ALL_LOCK_ALLOC",
  "upc_lock",                        "UPCR_LOCK",
  "upc_lock_attempt",                "UPCR_LOCK_ATTEMPT",
  "upc_unlock",                      "UPCR_UNLOCK",
  "upc_memcpy",                      "UPCR_MEMCPY",
  "upc_memget",                      "UPCR_MEMGET",
  "upc_memput",                      "UPCR_MEMPUT",
  "upc_memset",                      "UPCR_MEMSET",
  "upc_global_exit",                 "upcr_global_exit"};


TY_IDX Get_Field_Type (TY_IDX struct_type, UINT field_id)
{
  Is_True (TY_kind (struct_type) == KIND_STRUCT, ("expecting KIND_STRUCT"));
  UINT cur_field_id = 0;
  FLD_HANDLE fld = FLD_get_to_field (struct_type, field_id, cur_field_id);
  Is_True (! fld.Is_Null(), ("Invalid field id %d for type 0x%x",
                             field_id, struct_type));
  return FLD_type (fld);
}

bool is_upcr_ptr(TY_IDX ty) {
  
  return ty == shared_ptr_idx ||
    ty == pshared_ptr_idx ||
    //WEI: shouldn't need the following two, but for some reason the two above don't seem to be enough
    strcmp(TY_name(ty), "shared_ptr_struct") == 0 || 
    strcmp(TY_name(ty), "pshared_ptr_struct") == 0;
}
  


void
Save_Upc_Rts_Calls(char *name,ST *st)
{
  if (strcmp(name, "upcr_add_shared") == 0) {
    upc_ptradd_st = st;
  } else if (strcmp(name, "upcr_sub_shared") == 0) {
    upc_ptrdiff_st = st;
  } else if(strcmp(name, "upcr_getsp") == 0) {
    upc_ptrget_st  = st;
  }  else if (strcmp(name, "upcr_putsp") == 0) {
    upc_ptrput_st = st;
  }  else if (strcmp(name, "upcr_threadof_shared") == 0) {
    upc_threadof_st = st;
  }  else if (strcmp(name, "upcr_memget") == 0) {
    upc_memget_st = st;
  }  else if (strcmp(name, "upcr_memput") == 0) {
    upc_memput_st = st;
  }  else if (strcmp(name, "upcr_memcpy") == 0) {
    upc_memcpy_st = st;
  } else if (strcmp(name, "upcr_add_disp") == 0) {
    upc_adddisp_st = st;
  } else if (strcmp(name, "upcr_get_shared") == 0) {
    upc_getshared_st = st;
  } else if (strcmp(name, "upcr_shared_resetphase") == 0) {
    upc_phreset_st = st;
  } else  if (strcmp(name, "upcr_isnull_shared") == 0) {
    upc_nulleq_st = st;
  } else  if (strcmp(name, "upcr_isnotnull_shared") == 0) {
    upc_nullneq_st = st;
  }else if (strcmp(name, "upcr_isequal_shared_shared") == 0) {
    upc_ptreqtest_st = st;
  } else  if (strcmp(name, "upcr_isnotequal_shared_shared") == 0) {
    upc_ptrneqtest_st = st;
  }
}

void Upc_Translate_Name(char *from, char **to) {

  for(int i = 0; i < sizeof(upc_library_names) / sizeof(char*); i += 2)
    if(strcmp(upc_library_names[i], from) == 0) {
      *to = upc_library_names[i+1];
      return;
    }
}




TY_IDX Get_Inner_Array_Type( TY_IDX idx) {
  
  Is_True(TY_kind(idx) == KIND_ARRAY,("",""));
  TY_IDX eidx = TY_etype(idx);
  while (TY_kind(eidx) == KIND_ARRAY)
    eidx = TY_etype(eidx);

  return eidx;
  
}

UINT Get_Type_Block_Size (TY_IDX idx) {

  UINT result;
  switch(TY_kind(idx)) {
  case KIND_STRUCT:
  case KIND_SCALAR:
  case KIND_VOID:
    Is_True(TY_is_shared(idx),("Request for the block_size of a non-shared type",""));
    result =  TY_block_size(idx);
    break;
  case KIND_POINTER: {
   //  Ty_Table[idx].Print(stderr);
//     Ty_Table[TY_pointed(idx)].Print(stderr);
    if(TY_is_shared(idx))
      result =  TY_block_size(idx);
    else  if(TY_is_shared(TY_pointed(idx))) {
      if(TY_kind(TY_pointed(idx)) == KIND_ARRAY)
	result = Get_Type_Block_Size(TY_pointed(idx));
      else
	result =   TY_block_size(TY_pointed(idx));
    }
    else 
       Is_True(0, ("Request for the block_size of a non-shared type",""));
    }
    break;

  case KIND_ARRAY:
    result =  TY_block_size(Get_Inner_Array_Type(idx));
    break;
  default:
    Is_True(0, ("Request for the block_size of a non-shared type",""));
  }
  return result;
}


UINT64 Get_Type_Inner_Size (TY_IDX idx) {
  
  switch(TY_kind(idx)) {
  case KIND_STRUCT:
    return Adjusted_Type_Size(idx);
  case KIND_SCALAR:
    return TY_size(idx);
  case KIND_POINTER:
    if (TY_kind(TY_pointed(idx)) == KIND_ARRAY)
      return TY_size(TY_etype(TY_pointed(idx)));
    else { 
      if(TY_kind(TY_pointed(idx)) == KIND_STRUCT)
	return Adjusted_Type_Size(TY_pointed(idx));
      else 
	return TY_size(TY_pointed(idx));
    }
    break;
  case KIND_ARRAY:
    if(TY_kind(Get_Inner_Array_Type(idx)) == KIND_STRUCT)
      return Adjusted_Type_Size(Get_Inner_Array_Type(idx));
    else
      return TY_size(Get_Inner_Array_Type(idx));
 
  default:
    Is_True(0,("Unexpected type for shared ptr arithmetic",""));
  }
  return 0;
}



void Initialize_Upc_Metadata() {
  int level, i;
  ST *st;

  FOREACH_SYMBOL(GLOBAL_SYMTAB, st, i)
    Save_Upc_Rts_Calls( ST_name(st), st);
  
}

UINT Adjusted_Type_Size(TY_IDX idx, unsigned int pshared, unsigned int shared) {
  
  UINT result = 0;

  switch(TY_kind(idx)) {
  case KIND_SCALAR:
  case KIND_POINTER:
    if(!Type_Is_Shared_Ptr(idx))
      return TY_size(idx);
    else if (pshared != 0) {
      /* called from the front end */
      if (TY_is_shared(idx)) {
	return TY_is_pshared(idx) ? pshared : shared;
      } else {
	return TY_is_pshared(TY_pointed(idx)) ? pshared : shared;
      }
    } else {
      return TY_size(TY_To_Sptr_Idx(idx));
    }
  case KIND_STRUCT: {
    // sum_i(Adjusted_type_size(field_i))
    if(idx == pshared_ptr_idx || idx == shared_ptr_idx)
      return TY_size(idx);

    TY_IDX last_fld_idx = 0;
    UINT field_id = 1;
    UINT offset = 0;
    FLD_ITER  fiter  = Make_fld_iter(TY_fld(idx));
    FLD_ITER last = Fld_Table.end ();
    FLD_HANDLE fh;
    do {
      fh = FLD_HANDLE(fiter);
      last_fld_idx = FLD_type(fh);
      field_id++;
    } while (!FLD_last_field(fiter) && ++fiter != last);
    
    if(field_id) {
      offset += Adjust_Field_Offset(idx, field_id-1) + Adjusted_Type_Size(last_fld_idx);
      if(offset  % 8 == 0)
	return offset;
      else 
	return offset + (8 - (offset % 8));
    }
  }
    break;
  case KIND_ARRAY:
    if(Type_Is_Shared_Ptr(TY_etype(idx), TRUE)) 
      return (TY_size(idx) / TY_size(TY_etype(idx))) * TY_size(TY_To_Sptr_Idx(TY_etype(idx)));
    else
      return TY_size(idx); 
    
    break;
  case KIND_VOID:
    return 0;
  }
  
  assert(false);
  /* NOTREACHED */
  return 0;
}



// This function assumes that the field offsets within
// a struct are already adjusted to account for the 
// shared pointers.
// The adjusted type size is offset(last_field) + sizeof(last_field)
void Adjust_Type_Size(TY *ty) {
  
  if(TY_kind(*ty) == KIND_STRUCT) {
    FLD_IDX field = ty->Fld();
    TY_IDX last_fld_idx = 0;
    UINT field_id = 1;
    UINT offset;
    FLD_ITER  fiter  = Make_fld_iter(FLD_HANDLE(field));
    FLD_ITER last = Fld_Table.end ();
    FLD_HANDLE fh;
    do {
      fh = FLD_HANDLE(fiter);
      last_fld_idx = FLD_type(fh);
      offset = FLD_ofst(fh);
    } while (!FLD_last_field(fiter) && ++fiter != last);
    //
    if(field) {
      offset += Adjusted_Type_Size(last_fld_idx);
      if(offset  % 8 == 0)
	Set_TY_size(*ty, offset);
      else 
	Set_TY_size(*ty, offset + (8 - (offset % 8)));
    }
  }
  
}

template <class T>
struct adjust_type_size_op
{
  TY *t;
  adjust_type_size_op (TY *it) : t(it) {}
  
  void operator () (UINT idx, TY *entry) const;
}; // adjust_type_size_op


template <class T>
inline void
adjust_type_size_op<T>::operator () (UINT idx, TY *entry) const {
  Adjust_Type_Size(entry);
}




template <class T>
struct change_type_op
{
  ST *s;
  change_type_op (ST *st) : s(st) {}
  
  void operator () (UINT idx, ST *entry) const;
}; // change_type_op


template <class T>
inline void
change_type_op<T>::operator () (UINT idx, ST *entry) const {
  Set_ST_type(entry, TY_To_Sptr_Idx(ST_type(entry)));
}


static 
void Change_Type_To_Shared( ST *st) 
{
  int i;
  TY_IDX st_ty  = ST_class(st) == CLASS_VAR ? ST_type(st) :
    ST_class(st) == CLASS_FUNC ? ST_pu_type(st) : ST_type(st);
  
  if (ST_class(st) == CLASS_VAR && Type_Is_Shared_Ptr(st_ty)) {
    Set_ST_type (st, TY_To_Sptr_Idx(st_ty));
  } // else if(ST_class(st) == CLASS_VAR && TY_kind(st_ty) == KIND_STRUCT) {
//     UINT field_id = 1;
//     UINT tmp_field_id = 0;
//     FLD_HANDLE fh = FLD_get_to_field(st_ty, field_id, tmp_field_id);
//     while(!FLD_last_field(fh)) {
//       TY_IDX idx = FLD_type(fh);
//       if(Type_Is_Shared_Ptr(idx)) {
// 	Set_FLD_type(fh, TY_To_Sptr_Idx(idx));
//       }
//     }
//   }
  else if (ST_class(st) == CLASS_FUNC) {
    TYLIST_IDX idx = TY_tylist(ST_pu_type(st));
    while(Tylist_Table [idx]) {
      
      TY_IDX tidx = Tylist_Table[idx];
      //Ty_Table[tidx].Print(stderr);
      if (Type_Is_Shared_Ptr(tidx)) {
	  Set_TYLIST_type (Tylist_Table [idx++], TY_To_Sptr_Idx(tidx));
      }
      else idx++;
    }
    For_all_entries (*Scope_tab[PU_lexical_level(st)].st_tab, change_type_op<ST> (st), 1);
   
  }
}

void Upc_Lower_SymbolTable() {
  int level, i;
  ST *st;
  Is_True (shared_ptr_idx, ("Shared ptr type not initialized",""));
  FOREACH_SYMBOL(GLOBAL_SYMTAB, st, i)
    Change_Type_To_Shared ( st );

  TY_ITER tlast = Ty_tab.end();
  TY_ITER titer = Make_ty_iter(MTYPE_To_TY(MTYPE_F8));
  FLD_HANDLE fh;
  do {
    TY idx = *titer;
    if(TY_kind(idx) == KIND_STRUCT) {
      FLD_IDX field = idx.Fld();
      UINT field_id = 1;
      FLD_ITER  fiter  = Make_fld_iter(FLD_HANDLE(field));
      FLD_ITER last = Fld_Table.end ();
      
      do {
	fh = FLD_HANDLE(fiter);
	TY_IDX fidx = FLD_type(fh);
	Set_FLD_ofst(fh, // FLD_ofst(fh) +
		     Adjust_Field_Offset(idx, field_id));
	field_id++;
      } while (!FLD_last_field(fiter) && ++fiter != last);

     //  Set_TY_size(idx, FLD_ofst(fh) + TY_size(FLD_type(fh)));
    //   idx.Print(stderr);
      
      field_id = 1;
      fiter =  Make_fld_iter(FLD_HANDLE(field));
      do {
	FLD_HANDLE fh = FLD_HANDLE(fiter);
	TY_IDX fidx = FLD_type(fh);
	if(Type_Is_Shared_Ptr(fidx)) {
	  Set_FLD_type(fh, TY_To_Sptr_Idx(fidx));
	}
	field_id++;
      } while (!FLD_last_field(fiter) && ++fiter != last);
    }
  } while(++titer != tlast);

  
  For_all <adjust_type_size_op<TY> > (Ty_Table, adjust_type_size_op <TY>(0) );
}
  


UINT Adjust_Field_Offset(TY& struct_ty, UINT field_id) {
  
  UINT off = 0;
  UINT cur_id = 0;
  UINT aligned = 0;
  UINT displacement;
  UINT alignment = 1;
  TY_IDX fld_ty;
  Is_True(TY_kind(struct_ty) == KIND_STRUCT, ("",""));

  if(field_id  == 0|| field_id == 1)
    return 0;

  FLD_IDX idx = struct_ty.Fld();
  FLD_ITER fiter = Make_fld_iter(FLD_HANDLE(idx));
  FLD_HANDLE prev_fld = FLD_HANDLE(fiter++);
  FLD_HANDLE fh;
  cur_id = 2;  
  while(cur_id++ <= field_id) {
    fh = FLD_HANDLE(fiter);
    fld_ty = FLD_type(fh);
    alignment = TY_align(fld_ty);
    if(Type_Is_Shared_Ptr(fld_ty))
      alignment = TY_align(TY_To_Sptr_Idx(fld_ty));
    displacement = off + Adjusted_Type_Size(FLD_type(prev_fld)) ;
    if(displacement % alignment == 0) 
      off = displacement;
    else 
      off = displacement + (alignment - displacement % alignment);
    prev_fld = fh;
    ++fiter;
  } ;

  return off;
}




UINT Adjust_Field_Offset(TY_IDX struct_ty, UINT field_id) {
  
  UINT off = 0;
  UINT tmp_field_id = 0;
  TY_IDX fld_ty;
  UINT displacement = 0;
  UINT alignment;
  UINT cur_id;
  Is_True(TY_kind(struct_ty) == KIND_STRUCT, ("",""));

  if(field_id == 0 || field_id == 1)
    return 0;
  
  FLD_ITER fiter = Make_fld_iter(TY_fld(struct_ty));
  FLD_HANDLE prev_fld = FLD_HANDLE(fiter++);
  FLD_HANDLE fh;
  cur_id = 2;  
  // while(cur_id++ <= field_id)
  do  {
    fh = FLD_HANDLE(fiter);
    fld_ty = FLD_type(fh);
    alignment = TY_align(fld_ty);
    if(Type_Is_Shared_Ptr(fld_ty))
      alignment = TY_align(TY_To_Sptr_Idx(fld_ty));
    displacement = off + Adjusted_Type_Size(FLD_type(prev_fld)) ;
    if(displacement % alignment == 0) 
      off = displacement;
    else 
      off = displacement + (alignment - displacement % alignment);
    prev_fld = fh;
    ++fiter;
  } while(!FLD_last_field(prev_fld) && cur_id++ < field_id);


 //  TY_IDX prev_fld_idx = Get_Field_Type(struct_ty, 1);
  
//   for(int i = 2; i <= field_id; i++) {
//     fld_ty = Get_Field_Type(struct_ty, i);
//     alignment = TY_align(fld_ty);
//     if(Type_Is_Shared_Ptr(fld_ty))
//       alignment = TY_align(TY_To_Sptr_Idx(fld_ty));
//     displacement = off + Adjusted_Type_Size(prev_fld_idx) ;
    
//     if(displacement % alignment == 0) 
//       off = displacement;
//     else 
//       off = displacement + (alignment - displacement % alignment);
//     prev_fld_idx = fld_ty;
//     tmp_field_id = i-1;
//     if(FLD_last_field(FLD_get_to_field (struct_ty, i, tmp_field_id)))
//       break;;
//   } 
  return off;
}



void Verify_Upc_Metadata() {
  Is_True( shared_ptr_idx //  upc_ptradd_st &&  
// 	    upc_ptrget_st &&
// 	    upc_threadof_st &&
// 	    upc_memcpy_st &&
// 	    upc_adddisp_st &&
// 	    upc_getshared_st &&
// 	    upc_phreset_st &&
// 	    upc_nulleq_st &&
// 	    upc_nullneq_st &&
// 	    upc_ptreqtest_st &&
// 	    upc_ptrneqtest_st  &&
	    // upc_ptrput_st &&
// 	    upc_mythread_st &&
// 	    upc_memget_st &&
// 	    upc_memput_st &&
	    
	    // upc_ptrdiff_st &&
// 	    upc_hsync_reg_ty &&
// 	    upc_hsync_mem_ty
	    , ("UPC metadata not initialized",""));
	    
}



static TYPE_ID
Size_To_Mtype (UINT32 size) 
{
  
  if (size == TY_size(MTYPE_To_TY(MTYPE_I1)))
     return MTYPE_I1;
  else if (size == TY_size(MTYPE_To_TY(MTYPE_I2)))
     return MTYPE_I2;
  else if (size == TY_size(MTYPE_To_TY(MTYPE_I4)))
     return MTYPE_I4;
  else if (size == TY_size(MTYPE_To_TY(MTYPE_I8)))
     return MTYPE_I8;
  else 
    return MTYPE_M;

}


static void 
Fill_Structure_Type (TY &ty, UINT size) {
  
  TY_IDX bogus_idx = 0;
  TY &bogus = New_TY (bogus_idx);
  TY_Init (bogus, size, KIND_ARRAY, MTYPE_M, Save_Str("bogus_name"));
  Set_TY_etype (bogus, MTYPE_To_TY(MTYPE_I1));
  Set_TY_align (bogus_idx, TY_align(TY_etype(bogus)));
  ARB_HANDLE arb = New_ARB (); 
  ARB_Init (arb, 0, 0, 0);
  Set_TY_arb (bogus, arb);
  Set_ARB_first_dimen (arb);
  Set_ARB_last_dimen (arb);
  Set_ARB_dimension (arb, 1);
  Set_ARB_const_stride (arb);
  Set_ARB_stride_val (arb, size);
  Set_ARB_const_lbnd (arb);
  Set_ARB_lbnd_val (arb, 0);
  Set_ARB_const_ubnd (arb);
  Set_ARB_ubnd_val (arb,size-1);
  
  //Ty_Table[bogus_idx].Print(stderr);
  
  FLD_HANDLE fld = New_FLD();
  FLD_Init(fld, Save_Str("bogus_field_name"), bogus_idx, 0);
  Set_TY_fld(ty, fld);
  Set_FLD_last_field(fld);
  

}           
    



void Create_Special_Shared_Global_Symbols()
{
  TY_IDX ty_idx;
  TY_KIND ty_kind = KIND_SCALAR;
  

#define LAST_SCALAR_SHARED_TYPE MTYPE_F16

  int j;

  /* Make predefined types for the machine types: */
  for (TYPE_ID i = MTYPE_I1; i  <= LAST_SCALAR_SHARED_TYPE; ++i) {
    
    TY &ty = New_TY (ty_idx);
    ty_kind = KIND_SCALAR;
    Set_TY_align (ty_idx, TY_align(MTYPE_To_TY(i)));
    TY_Init (ty, MTYPE_byte_size (i), ty_kind, i,
	     Save_Str2(".predef_shared_", MTYPE_name (i)));
    MTYPE_To_Shared_TY (i) = ty_idx;
    Set_TY_is_shared(ty_idx);
  }
}

class TY_shared {
public:
  bool shared;

  TY_shared(bool shared) {
    this->shared = shared;
  }
  
  bool operator() (UINT, const TY* ty) const {
    if (shared) {
      return strcmp(TY_name(*ty),"shared_ptr_struct") == 0; 
    } 
    return strcmp(TY_name(*ty),"pshared_ptr_struct") == 0;
  }
};
  
void Find_Upc_Vars () {

  ST* s;
  int i;

  FOREACH_SYMBOL(GLOBAL_SYMTAB, s, i) {
    if (strcmp(ST_name(*s),"upcr_forall_control") == 0)  {
      upc_forall_control_st = s;
      break;
    }
  }
  Is_True(upc_forall_control_st != NULL, ("Can't find upcr_forall_control in symbol table", ""));
  
  shared_ptr_idx = For_all_until(Ty_Table, TY_shared(true));
  pshared_ptr_idx = For_all_until(Ty_Table, TY_shared(false));
  
  Is_True(shared_ptr_idx != 0 && pshared_ptr_idx != 0, ("Can't find shared_ptr_struct in symbol table", ""));
}

BOOL Use_32_Bit(const char* filename) {

  FILE* config_file = fopen(filename, "r");
  char line[MAX_LINE_LEN_UPC];
  int size;
  char param[MAX_LINE_LEN_UPC];
  while (fgets(line, MAX_LINE_LEN_UPC, config_file) != NULL) {
    if (sscanf(line, "%s\t%d", param, &size) != 2) {
      continue;
    }
    if (strcmp(param, "ptr_size") == 0 && size == 4) {
      printf ("USE 32 BIT!!!!\n");
      return TRUE;
    }
  }
  return FALSE;
}

void Initialize_Upc_Vars () {

  upc_forall_control_st = New_ST(GLOBAL_SYMTAB);
  TY_IDX idx = MTYPE_To_TY(MTYPE_I4);
  ST_Init(upc_forall_control_st, 
	  Save_Str("upcr_forall_control"),
	  CLASS_VAR, SCLASS_COMMON, EXPORT_PREEMPTIBLE, idx);
}

//sizes are in bytes
void Initialize_Upc_Types (char *sptr_name, UINT sptr_size,
			   char *psptr_name, UINT psptr_size,
			   char *hreg_name, UINT hreg_size,  // reg_handle_t 
			   char *hmem_name, UINT hmem_size)  // mem_handle_t

{
  
  TYPE_ID mtype;
  TY_KIND ty_kind;
  UINT align;
  TY &sty = New_TY(shared_ptr_idx); 
  TY &psty = New_TY(pshared_ptr_idx); 
  TY &rty = New_TY (upc_hsync_reg_ty);
  TY &mty = New_TY (upc_hsync_mem_ty);
  BOOL _64_bit_target = TY_size(MTYPE_To_TY(Pointer_type)) > 4;

  mtype = Size_To_Mtype(sptr_size);
  if(!_64_bit_target && mtype == MTYPE_I8) {
    mtype = MTYPE_M;
  }
  ty_kind = (mtype < MTYPE_M) ? KIND_SCALAR : KIND_STRUCT;
  align = (mtype < MTYPE_M) ? TY_align(MTYPE_To_TY(mtype)) : TY_align(Integer_type); 
  TY_Init(sty, sptr_size, ty_kind, mtype, Save_Str (sptr_name));
  Set_TY_align(shared_ptr_idx, align);
  if (mtype == MTYPE_M)
    Fill_Structure_Type (sty, sptr_size);
  
  mtype = Size_To_Mtype(psptr_size);
  if(!_64_bit_target && mtype == MTYPE_I8) {
    mtype = MTYPE_M;
  }
  ty_kind = (mtype < MTYPE_M) ? KIND_SCALAR : KIND_STRUCT;
  align = (mtype < MTYPE_M) ? TY_align(MTYPE_To_TY(mtype)) : 8// TY_align(Pointer_type)
    ; 
  TY_Init(psty, psptr_size, ty_kind, mtype, Save_Str (psptr_name));
  Set_TY_align(pshared_ptr_idx, align);
  if (mtype == MTYPE_M)
    Fill_Structure_Type (psty, psptr_size);

  
  mtype = Size_To_Mtype(hreg_size);
  if(!_64_bit_target && mtype == MTYPE_I8) {
    mtype = MTYPE_M;
  }
  ty_kind = (mtype < MTYPE_M) ? KIND_SCALAR : KIND_STRUCT;
  TY_Init(rty, hreg_size, ty_kind, mtype, Save_Str (hreg_name));
  align = (mtype < MTYPE_M) ? TY_align(MTYPE_To_TY(mtype)) : TY_align(Integer_type); 
  Set_TY_align(upc_hsync_reg_ty, align);
  if (mtype == MTYPE_M)
    Fill_Structure_Type (rty, hreg_size);
  
  mtype = Size_To_Mtype(hmem_size);
  if(!_64_bit_target && mtype == MTYPE_I8) {
    mtype = MTYPE_M;
  }
  ty_kind = (mtype < MTYPE_M) ? KIND_SCALAR : KIND_STRUCT;
  TY_Init(mty, hmem_size, ty_kind, mtype, Save_Str (hmem_name));
  align = (mtype < MTYPE_M) ? TY_align(MTYPE_To_TY(mtype)) : TY_align(Integer_type); 
  Set_TY_align(upc_hsync_mem_ty, align);
  if (mtype == MTYPE_M)
    Fill_Structure_Type (mty, hmem_size);

  //Create_Special_Shared_Global_Symbols();
  // cout << "\tshared pointer size: " << sptr_size << "\t pshared pointer size: " << psptr_size << endl;
//   cout << "\tregister size: " << hreg_size << "\t memory handle size: " << hmem_size << endl;
}

//It's illegal to call WN_ty() on a OPR_CONST, so we need this wrapper here 
//(may apply to other nodes too??)
BOOL WN_Type_Is_Shared_Ptr(const WN* wn, BOOL real_ptr) {

  switch (WN_operator(wn)) {
  case OPR_CONST:
    return false;
  default:
    return Type_Is_Shared_Ptr(WN_ty(wn), real_ptr);
  }
}

BOOL Type_Is_Shared_Ptr (TY_IDX idx, BOOL real_ptr) {

  return real_ptr ?  
    TY_kind(idx) == KIND_POINTER  && TY_is_shared(TY_pointed(idx)) :
    
    (TY_is_shared(idx) ||
     (TY_kind(idx) == KIND_POINTER  && TY_is_shared(TY_pointed(idx))) ||
     (TY_kind(idx) == KIND_ARRAY && TY_is_shared(Get_Inner_Array_Type(idx)))) ; 
}

BOOL TY_is_pshared(TY_IDX idx) {

  //NOTE: TY_is_shared must be true as a precondition of this method
  Is_True(TY_is_shared(idx), ("Calling TY_is_pshared with a non-shared type",""));

  switch (TY_kind(idx)) {
  case KIND_SCALAR:
  case KIND_STRUCT:
  case KIND_POINTER:
    return TY_mtype(idx) != MTYPE_V &&
    TY_block_size(idx) <= 1;
  case KIND_ARRAY:
    return TY_is_pshared(TY_etype(idx));
  }
  return false;
}



TY_IDX TY_To_Sptr_Idx (TY_IDX idx) 
{
  if (TY_kind(idx) != KIND_VOID && 
      Type_Is_Shared_Ptr(idx) && Get_Type_Block_Size(idx) <= 1 && 
      !(Type_Is_Shared_Ptr(idx, TRUE) && TY_kind(TY_pointed(idx)) == KIND_VOID))
    return pshared_ptr_idx;
  else
    if (Type_Is_Shared_Ptr(idx))
      return shared_ptr_idx;
    else 
      return idx;
}

TY_IDX Fix_Intrinsic_Return_Type(INTRINSIC op) 
{
  switch(op) {
  case INTRN_ADD_PI:
  case INTRN_ADD_P1:
  case INTRN_S_TO_P:
    return pshared_ptr_idx;
  case INTRN_SPTRADD:
  case INTRN_SPTRDIFF:
  case INTRN_S_RESET:
  case INTRN_ADD_S:
  case INTRN_P_TO_S:
    return shared_ptr_idx;
  default: 
    assert(false);
    return 0;
  }
}



BOOL Need_StoP_Cvt(TY_IDX src_idx, TY_IDX dest_idx, INTRINSIC *iop)
{
  BOOL result = FALSE;
  *iop = INTRINSIC_LAST;
  UINT src_sz = Get_Type_Inner_Size(src_idx);
  UINT src_blk = Get_Type_Block_Size(src_idx);
  UINT dest_sz = Get_Type_Inner_Size(dest_idx);
  UINT dest_blk = Get_Type_Block_Size(dest_idx);
  
  if(TY_is_shared(src_idx)) {
    switch(TY_kind(src_idx)) {
    case KIND_POINTER:
      src_blk = Get_Type_Block_Size(TY_pointed(src_idx));
      break;
    case KIND_ARRAY:
      Is_True(0,("",""));
    }
  }

  if(TY_is_shared(dest_idx)) {
    switch(TY_kind(dest_idx)) {
    case KIND_POINTER:
      dest_blk = Get_Type_Block_Size(TY_pointed(dest_idx));
      break;
    case KIND_ARRAY:
      Is_True(0,("",""));
    }
  }

 //  Ty_Table[src_idx].Print(stderr);
//   Ty_Table[dest_idx].Print(stderr);
  Is_True(((TY_kind(src_idx) == KIND_POINTER || TY_kind(src_idx) == KIND_ARRAY) &&
	  (TY_kind(dest_idx) == KIND_POINTER || TY_kind(dest_idx) == KIND_ARRAY)) ||
	  (TY_kind(dest_idx) == KIND_SCALAR && TY_kind(src_idx) == KIND_SCALAR),
	  ("Incorrect type combination in StoP",""));

  if(TY_kind(dest_idx) == KIND_SCALAR && TY_kind(src_idx) == KIND_SCALAR)
    return FALSE;

  if(src_sz == 0) {
    if(dest_blk <= 1 && dest_sz) {
      *iop = INTRN_S_TO_P;
      result = TRUE;
    }
  } else {
    if (src_blk <= 1) {
      if(dest_sz == 0 || dest_blk > 1) {
	*iop = INTRN_P_TO_S;
	result = TRUE;
      } 
     
    } else {
      if (src_sz != dest_sz && dest_sz) { 
	result = true;
	if (dest_blk)
	  *iop =  INTRN_S_RESET;
	else
	  *iop = INTRN_S_TO_P;
      } else if (src_blk != dest_blk && dest_sz) {
	if (dest_blk <= 1) {
	  *iop = INTRN_S_TO_P;
	} else {
	  *iop = INTRN_S_RESET;
	}  
	result = TRUE;
      }   
    }
  }
  
  return result;
}



BOOL Types_Are_Equiv(TY_IDX idx1, TY_IDX idx2)
{
  

  if (idx1 == idx2)
    return TRUE;

  // Ty_Table[idx1].Print(stderr);
//   Ty_Table[idx2].Print(stderr);
  

  if(TY_kind(idx1) == KIND_POINTER) 
    idx1 = TY_pointed(idx1);
  else 
    if(TY_kind(idx1) == KIND_ARRAY)
      idx1 = Get_Inner_Array_Type(idx1);
  
  if(TY_kind(idx2) == KIND_POINTER) 
    idx2 = TY_pointed(idx2);
  else 
    if(TY_kind(idx2) == KIND_ARRAY)
      idx2 = Get_Inner_Array_Type(idx2);

  // Ty_Table[idx1].Print(stderr);
//   Ty_Table[idx2].Print(stderr);
  
  return (idx1 == idx2) || (Get_Type_Inner_Size(idx1) == Get_Type_Inner_Size(idx2)) ;
  
}



