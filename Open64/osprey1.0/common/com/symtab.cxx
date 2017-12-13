/*

  Copyright (C) 2000, 2001 Silicon Graphics, Inc.  All Rights Reserved.

  This program is free software; you can redistribute it and/or modify it
  under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it would be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

  Further, this software is distributed without any warranty that it is
  free of the rightful claim of any third person regarding infringement 
  or the like.  Any license provided herein, whether implied or 
  otherwise, applies only to this software file.  Patent licenses, if 
  any, provided herein do not apply to combinations of this program with 
  other software, or any other product whatsoever.  

  You should have received a copy of the GNU General Public License along
  with this program; if not, write the Free Software Foundation, Inc., 59
  Temple Place - Suite 330, Boston MA 02111-1307, USA.

  Contact information:  Silicon Graphics, Inc., 1600 Amphitheatre Pky,
  Mountain View, CA 94043, or:

  http://www.sgi.com

  For further information regarding this notice, see:

  http://oss.sgi.com/projects/GenInfo/NoticeExplan

*/



#ifdef USE_PCH
#include "common_com_pch.h"
#endif /* USE_PCH */
#pragma hdrstop
#include <stdio.h>
#include <alloca.h>

#include <algorithm>
#include <string>
#include <map>
#include "HashTable.h"
using namespace stlCompatibility;

#include "defs.h"
#include "config.h"
#include "tracing.h"			// for DBar
#include "cxx_memory.h"			// for CXX_NEW

#include "symtab.h"
#include "irbdata.h"                    // for INITO_st_idx
#include "ir_a2b_util.h"                // for b2a and a2b utilities
#include "const.h"                      // for MAX_SYMBOLIC_CONST_NAME_LEN
#include "ttype.h"
#include "targ_sim.h"
#include "upc_symtab_utils.h"

// global tables
FILE_INFO	File_info;
SCOPE		*Scope_tab;
PU_TAB		Pu_Table;
SYMBOL_TABLE	St_Table;
TY_TAB		Ty_tab;
TYPE_TABLE	Ty_Table;
FLD_TAB		Fld_Table;
TYLIST_TAB	Tylist_Table;
ARB_TAB		Arb_Table;
TCON_TAB	Tcon_Table;
INITV_TAB	Initv_Table;
INITO_TABLE	Inito_Table;
PREG_TABLE	Preg_Table;
LABEL_TABLE	Label_Table;
BLK_TAB		Blk_Table;
ST_ATTR_TABLE	St_Attr_Table;

SYMTAB_IDX Current_scope;		// index to current scope
PU *Current_pu;				// ptr to current PU

//----------------------------------------------------------------------
// ST-related utilities
//----------------------------------------------------------------------

INT64
ST_size (const ST *st)
{
  switch (ST_class(st)) {
  case CLASS_BLOCK:
    return STB_size(st);
  case CLASS_VAR:
  case CLASS_PREG:
  case CLASS_PARAMETER:
    return TY_size(ST_type(st));
  case CLASS_CONST:
    if (TCON_ty(STC_val(st)) == MTYPE_STR)
        return Targ_String_Length (STC_val(st))
                + (TCON_add_null(STC_val(st)) ? 1 : 0);
    else
        return TY_size(ST_type(st));
  case CLASS_FUNC:
    return 0;
  }
  FmtAssert(FALSE, ("ST_size: unexpected CLASS"));
  /* NOTREACHED */
  return 0;
}

// make a copy of a ST in the SAME ST table
ST *
Copy_ST (ST *st)
{
    if (ST_sym_class (st) == CLASS_PREG)
	return st;

    SYMTAB_IDX level = ST_IDX_level (ST_st_idx (st));

    ST_TAB *st_tab = Scope_tab[level].st_tab;
    ST_IDX st_idx = make_ST_IDX (st_tab->Insert (*st), level);
    ST& copy = St_Table[st_idx];
    Set_ST_st_idx (copy, st_idx);
    if (ST_base_idx (st) == ST_st_idx (st))
	Set_ST_base_idx (copy, st_idx);
    return &copy;
} // Copy_ST


ST *
Copy_ST (ST *st, SYMTAB_IDX scope)
{
    if (scope == ST_level (st))
	return Copy_ST (st);

    ST_TAB *st_tab = Scope_tab[scope].st_tab;
    ST_IDX st_idx = make_ST_IDX (st_tab->Insert (*st), scope);
    ST& copy = St_Table[st_idx];
    Set_ST_st_idx (copy, st_idx);
    if (ST_base_idx (st) == ST_st_idx (st))
	Set_ST_base_idx (copy, st_idx);
    else if (scope != ST_IDX_level (ST_base_idx (st))) {
	ST *base_st = Copy_ST (&St_Table[ST_base_idx (st)], scope);
	Set_ST_base_idx (copy, ST_st_idx (base_st));
    }
    return &copy;
} // Copy_ST

ST *
Copy_ST_No_Base (ST *st, SYMTAB_IDX scope)
{
    static INT Temp_Index = 0;
    STR_IDX new_name;
    if (scope == GLOBAL_SYMTAB) 
        new_name = Save_Str2i(ST_name(st),"..", Temp_Index++);
    else
	new_name = ST_name_idx(st);

    if (scope == ST_level (st)) {
	ST* new_st = Copy_ST (st);
        Set_ST_name_idx (*new_st, new_name);
	return new_st;
    }

    ST_TAB *st_tab = Scope_tab[scope].st_tab;
    ST_IDX st_idx = make_ST_IDX (st_tab->Insert (*st), scope);
    ST& copy = St_Table[st_idx];
    Set_ST_st_idx (copy, st_idx);
    Set_ST_name_idx (copy, new_name);
    if (ST_base_idx (st) == ST_st_idx (st)) {
	Set_ST_base_idx (copy, st_idx);
    }
    return &copy;
} // Copy_ST


// Return TRUE if the ST is a constant literal or a const variable.
BOOL
ST_is_constant (const ST *st)
{
    switch (ST_sym_class(st)) {
    case CLASS_CONST:
	return TRUE;
    case CLASS_VAR:
	return ST_is_const_var(st);
    default:
	return FALSE;
    }
}

// ====================================================================
// Returns FALSE if the current PU is recursive or the ST is not in
// the local symtab. May return TRUE otherwise; the more we return
// TRUE, the better we optimize.
// ====================================================================

extern BOOL
ST_is_private_local(const ST *st)
{
  Is_True((PU_src_lang(Get_Current_PU()) &
	   (PU_src_lang(Get_Current_PU()) - 1)) == 0,
	  ("ST_is_private_local: Mixed-language inlining not "
	   "allowed; need to know original PU language"));
 
  if (ST_IDX_level(ST_st_idx(st)) != CURRENT_SYMTAB)
    return FALSE;

  // Fix 643997:  for F90, -LANG:recursive=on force the compiler
  // to assume every procedure is recursive.
  //
  // If -LANG:recursive=[on|off] is set by user
  // it overrides the language default.
  if (LANG_Recursive_Set)
    return !LANG_Recursive;
  
  // TODO:  should examine PU_recursive flag for all languages.
  //
  switch (PU_src_lang(Get_Current_PU())) {
  case PU_F77_LANG:
    // F77 default is to assume procedure are not recursive.
    return TRUE;

  case PU_F90_LANG:
    // The default is to rely on the PU_recursive flag set by frontend.
    return (!PU_recursive(Get_Current_PU()));
  }
  return FALSE;
}

//----------------------------------------------------------------------
// TY-related utilities
//----------------------------------------------------------------------


TY_IDX
Copy_TY (TY_IDX ty)
{

    TY_IDX copy = ty;
    Set_TY_IDX_index (copy, Ty_tab.Insert (Ty_Table[ty]));
    return copy;
}


// assume align is a power of 2.
UINT
TY_log_base2 (UINT align)
{
    UINT result = 0;

    Is_True (align, ("Invalid alignment for TY"));

    while ((align & 0x7) == 0 && align!=0) {//fzhao May
	result += 3;
	align >>= 3;
    }

    return result + (align >> 1);
} // TY_log_base2


/* ====================================================================
 *
 * FLD_get_to_field
 *
 * Given the TY of a struct, get to the field corresponding to field_id.
 * cur_field_id gives the field_id of the struct itself.  If the field is
 * found, cur_field_id is set equal to field_id.  Otherwise, Is_Null() is
 * true for the field handle returned, and cur_field_id is set to 
 * correspond to the last field of this struct.
 *
 * ==================================================================== */
FLD_HANDLE 
FLD_get_to_field (TY_IDX struct_ty_idx, UINT field_id, UINT &cur_field_id)
{
    FLD_ITER fld_iter = Make_fld_iter(TY_fld(struct_ty_idx));
    do {
	FLD_HANDLE fld(fld_iter);
	cur_field_id++;
	if (cur_field_id == field_id)
	    return fld;
	//WEI: if the field is a {p}shared_ptr_t, make sure we don't try to traverse its fields
	if (TY_kind(FLD_type(fld)) == KIND_STRUCT && !is_upcr_ptr(FLD_type(fld)) &&
            TY_fld(FLD_type(fld)) != FLD_HANDLE()) {
	    fld = FLD_get_to_field(FLD_type(fld), field_id, cur_field_id);
	    if (cur_field_id == field_id)
		return fld;
	} 
    } while (!FLD_last_field(fld_iter++));
    return FLD_HANDLE();
} // FLD_get_to_field


// reverse map for getting the pointer TY given the pointee


// Utility functions for mapping a TY to the one that points to it.
// A hash_map is used because the number of pointer types should be small
// compared to all TYs.

// we uniquely identify a pointee by it's TY_IDX and the TY_flags of the
// pointer 
typedef std::pair<TY_IDX, UINT32> TY_POINTEE_KEY;


// Solaris CC workaround
// hash_pointee_key contains the hash function for hash_map, we should 
// no longer use it after replacing hash_map with map
//
struct hash_pointee_key
{
    UINT32 operator() (TY_POINTEE_KEY key) const {
	return (key.first | key.second);
    }
};

typedef HashTable<TY_POINTEE_KEY, TY_IDX, hash_pointee_key> TY_IDX_POINTER_MAP;
// pointer_map maps a pointee to its corresponding pointer
static TY_IDX_POINTER_MAP pointer_map;
static std::pair<TY_POINTEE_KEY, TY_IDX> last_valid_map_entry;


static BOOL
Invalid_Pointer_Map_Entry (const TY_IDX_POINTER_MAP::value_type& entry)
{
    TY_IDX pointer = entry.second;
    TY_IDX pointee = entry.first.first;

    if (TY_IDX_index (pointer) >= TY_Table_Size () ||
	TY_IDX_index (pointee) >= TY_Table_Size ())
	return TRUE;

    const TY& ty = Ty_Table[pointer];

    if (TY_kind (ty) != KIND_POINTER ||
	TY_pointed (ty) != pointee ||
	TY_flags (ty) != entry.first.second)
	return TRUE;

    return FALSE;
} // Invalid_Pointer_Map_Entry


// in case the pointer_map is corrupted, we need to delete all invalid
// entries.  This should rarely happen, but still possible if some pointer
// types which have been inserted into the pointer_map are deleted.
static void
Validate_Pointer_Map ()
{
  typedef std::vector<TY_IDX_POINTER_MAP::KeyType> InvalidEntries;
  class FindInvalidEntries : public TY_IDX_POINTER_MAP::ForAllAction {
    TY_IDX_POINTER_MAP * map;
    InvalidEntries invalid_entries;

  public:
    FindInvalidEntries(TY_IDX_POINTER_MAP& _map) : map(&_map) {}
    
    void handle(const TY_IDX_POINTER_MAP::KeyType k, 
		TY_IDX_POINTER_MAP::ValueType &v) {
      UINT last_valid = 0;
      TY_IDX_POINTER_MAP::value_type kv(k, v);
      if (Invalid_Pointer_Map_Entry (kv))
	invalid_entries.push_back(k);
      else {
	if (last_valid < TY_IDX_index (v)) {
	  last_valid = TY_IDX_index (v);
	  last_valid_map_entry.first  =  k;
	  last_valid_map_entry.second =  v;
	}
      }
    }

    void eraseInvalidEntries() {
      InvalidEntries::const_iterator ieIterator;
      for ( ieIterator = invalid_entries.begin();
	    ieIterator != invalid_entries.end(); 
	    ++ieIterator )
	map->erase(*ieIterator);
    }
  }; // class FindInvalidEntries

  FindInvalidEntries fie(pointer_map);
  pointer_map.forAll(fie);
  fie.eraseInvalidEntries();
}

struct update_pointer_map
{
    void operator() (UINT32 idx, const TY* ty) const {
	if (TY_kind (*ty) != KIND_POINTER || TY_mtype (*ty) != Pointer_Mtype)
	    return;
	TY_POINTEE_KEY key (TY_pointed (*ty), TY_flags (*ty));
	last_valid_map_entry = std::make_pair (key, make_TY_IDX (idx));
	pointer_map.insert (last_valid_map_entry);
    }
};

static void
Update_Pointer_Map ()
{
  const TY_IDX_POINTER_MAP::value_type ss_entry( last_valid_map_entry.first,
						 last_valid_map_entry.second);
  
  if ( TY_IDX_index (last_valid_map_entry.second) != 0 &&
       Invalid_Pointer_Map_Entry (ss_entry)) {

    DevWarn ("Rehashing TY pointer map -- this should NOT happen often");
    Validate_Pointer_Map ();
  }
  For_all_entries (Ty_tab, update_pointer_map (),
		   TY_IDX_index (last_valid_map_entry.second) + 1);
}


static inline TY_IDX
Find_Ty_Pointer (TY_POINTEE_KEY key)
{
  TY_IDX_POINTER_MAP::ValueBoolPair result = pointer_map.find(key);

  if (result.second == false) return 0;

  TY_IDX_POINTER_MAP::value_type vt(key, result.first);
  if (Invalid_Pointer_Map_Entry (vt)) {
    DevWarn ("Rehashing TY pointer map -- this should NOT happen often");
    Validate_Pointer_Map ();
    return 0;
  }
  
  TY_IDX pointer = result.first;
  Set_TY_align (pointer, Pointer_Size);
  return pointer;
} // Find_Ty_Pointer	


TY_IDX
TY_pointer (TY_IDX pointee, BOOL f90_pointer)
{
    TY_POINTEE_KEY key (pointee, f90_pointer ? TY_IS_F90_POINTER : 0);

    TY_IDX result = Find_Ty_Pointer (key);

    if (result == 0) {
	Update_Pointer_Map ();
	return Find_Ty_Pointer (key);
    }

    return result;
} // TY_pointer

static string utoa(UINT64 i) {
  
  string val = "";
  
  while (i >= 10) {
    val = (char) ((i % 10) + '0') + val;  
    i = i / 10;
  }
  return ((char) (i + '0')) + val;
}

string get_TY_string(TY_IDX idx) {

  switch (TY_kind(idx)) {
  case KIND_SCALAR:
    return utoa(TY_mtype(idx));
  case KIND_POINTER:
    return "*" + utoa(TY_pointed(idx));
  case KIND_ARRAY: {
    TY_IDX base;
    int dim;
    for (base = TY_etype(idx), dim = 1; TY_kind(base) == KIND_ARRAY; base = TY_etype(base), dim++);
    string res = utoa(base);
    for (int i = 0; i < dim; i++, res += "[]");
    res += utoa(TY_size(idx));
    return res;
  }
  case KIND_STRUCT: 
    return utoa(TY_name_idx(idx));
  case KIND_VOID:
    return "void";
  }
  
  FmtAssert(FALSE, ("TY_kind: unexpected KIND"));
  /* NOTREACHED */
  return "";
}

class Idx_Pair {
public:
  TY_IDX idx;
  int bsize;
  Idx_Pair(TY_IDX i, int b) {idx = i; bsize = b;}
}; 

struct Pair_Cmp {
  bool operator() (const Idx_Pair* t1, const Idx_Pair* t2) const {
    return t1->idx < t2->idx || (t1->idx == t2->idx && t1->bsize < t2->bsize);
  }
};

//map<Idx_Pair*, TY_IDX, Pair_Cmp> ty_map;
map<string, TY_IDX> ty_map;


/**
 *
 * Given a type(shared/non-shared), this function returns an equivalent shared type with the specified block size
 */
TY_IDX Make_Shared_Type (TY_IDX ty_idx, int bsize, CONSISTENCY_class consistency) {

  string key = get_TY_string(ty_idx) + "_" + utoa(bsize) + (consistency == STRICT_CONSISTENCY ? "s" : "");
  
  if (TY_is_shared(ty_idx)) {
    //if the type passed in is already shared, check if it has the same block size as the one given
    //if yes, we can safely use the same TY entry
    int ty_bsize;
    switch (TY_kind(ty_idx)) {
    case KIND_SCALAR:
    case KIND_POINTER:
    case KIND_STRUCT:
      ty_bsize = TY_block_size(ty_idx);
      break;
    case KIND_ARRAY: {
      TY_IDX base;
      for (base = TY_etype(ty_idx); TY_kind(base) == KIND_ARRAY; base = TY_etype(base));
      ty_bsize = TY_block_size(base);
      break;
    }
    }
    if (ty_bsize == bsize && TY_is_strict(ty_idx) &&  consistency == STRICT_CONSISTENCY) {
      ty_map[key] = ty_idx;
      return ty_idx;
    }
  }

  //create a new shared type
  TY_IDX res = ty_map[key];  
  if (res == 0) {
    TY& ty = New_TY(res);
    TY_Init (ty, TY_size(ty_idx), TY_kind(ty_idx), TY_mtype(ty_idx),
	     Save_Str (TY_name(ty_idx)));
    Set_TY_flags(res, TY_flags(ty_idx));
    Set_TY_align(res, TY_align(ty_idx));
   
    if(consistency == STRICT_CONSISTENCY)
      Set_TY_is_strict(ty_idx);
    
    switch (TY_kind(ty_idx)) {
    case KIND_SCALAR:
      Set_TY_block_size(res, bsize);
      Set_TY_is_shared(res);
      break;
    case KIND_POINTER:
      Set_TY_block_size(res, bsize);
      Set_TY_is_shared(res);
      Set_TY_pointed(res, TY_pointed(ty_idx));      
      break;
    case KIND_STRUCT:
      Set_TY_block_size(res, bsize);
      Set_TY_is_shared(res);
      Set_TY_fld(res, TY_fld(ty_idx));
      break;
    case KIND_ARRAY:
      Set_TY_arb(res, TY_arb(ty_idx));
      Set_TY_etype(res, Make_Shared_Type(TY_etype(ty_idx), bsize, consistency));
      break;
    }
    ty_map[key] = res;
  }
  //cout << "MAKING SHARED: " << TY_name(res) << " " << key << " bsize: " << bsize << endl;
  return res;
}

TY_IDX
make_ptr_type (TY_IDX ty_idx, BOOL f90_pointer)
{
    TY_IDX ty_ptr = TY_pointer (ty_idx, f90_pointer);

    if (ty_ptr != 0)
	return ty_ptr;
   
    TY &ty = New_TY (ty_ptr);
    TY_Init (ty, Pointer_Size, KIND_POINTER, Pointer_Mtype,
	     Save_Str ("anon_ptr."));
    Set_TY_pointed (ty, ty_idx);
    if (f90_pointer)
	Set_TY_is_f90_pointer (ty);
    Set_TY_align (ty_ptr, Pointer_Size);
    
    return ty_ptr;
}

TY_IDX
Make_Pointer_Type (TY_IDX ty_idx, BOOL)
{
  // This now only returns non-f90 pointers. It ignores the second argument
  return make_ptr_type (ty_idx, FALSE);
} // Make_Pointer_Type

TY_IDX
Make_F90_Pointer_Type (TY_IDX ty_idx)
{
  // This now only returns f90 pointers
  return make_ptr_type (ty_idx, TRUE);
} // Make_F90_Pointer_Type


TY_IDX
Make_Function_Type(TY_IDX return_ty_idx)
{
  // Return the type of a function with unspecified parameters and the
  // specified return type. First, iterate through the known types and
  // see if we can return an existing type. Then as a last resort,
  // create the new type if we didn't find it.
#ifdef ROBERT
  //
  // Note: Check for duplicates only against functions of no
  // parameters. I believe functions that return values through their
  // first parameter have TY_arg_area_size != 0, and so are excluded
  // from being seen as duplicates. Wilson says we currently call
  // Make_Function_Type only for functions of no parameters.
  //      -- RK 971013
  TY_ITER ty;
  // TODO: There should be some way to get at Ty_tab.begin() and
  // Ty_tab.end() through Ty_Table. In other words, Ty_Table should be
  // the only visible way of getting at the table of types. Having two
  // names for the same thing is nasty. -- RK
  ty = Ty_tab.begin();
  for (++ty;
       ty != Ty_tab.end();
       ty++) {
    TYLIST_IDX known_return_tylist;
    if (TY_kind(*ty) == KIND_FUNCTION &&
	(known_return_tylist = TY_tylist(*ty)) != (TYLIST_IDX) 0 &&
	TYLIST_type(known_return_tylist) == return_ty_idx &&
	(return_ty_idx == 0 ||
	 TYLIST_type(known_return_tylist + 1) == (TY_IDX) 0)) {
      return ty.Index();
    }
  }
  TYLIST_IDX  new_return_tylist_idx;
  TYLIST     &new_return_tylist = New_TYLIST(new_return_tylist_idx);
  Set_TYLIST_type(new_return_tylist, return_ty_idx);

  if (return_ty_idx != (TY_IDX) 0) {
    TYLIST_IDX tail_return_tylist_idx;
    new_return_tylist = New_TYLIST(tail_return_tylist_idx);
    Is_True(tail_return_tylist_idx == new_return_tylist_idx + 1,
	    ("Make_Function_Type: TYLIST allocation messed up"));
    Set_TYLIST_type(new_return_tylist, (TY_IDX) 0);
  }

  TY_IDX  function_ty_idx;
  TY     &function_ty = New_TY(function_ty_idx);
  static char  buf[32];
  static INT32 tcount = 0;

  ++tcount;
  sprintf(buf, ".anon_functype.%d", tcount);
  TY_Init(function_ty, (UINT64) 0, KIND_FUNCTION, MTYPE_UNKNOWN,
	  Save_Str(buf));
  return function_ty_idx;
#else
  TY_IDX ty_idx;
  TY&    ty = New_TY (ty_idx);

  TY_Init (ty, 0, KIND_FUNCTION, MTYPE_UNKNOWN, 0);
  Set_TY_align (ty_idx, 1);

  TYLIST_IDX tylist_idx;

  (void) New_TYLIST (tylist_idx);
  Tylist_Table [tylist_idx] = return_ty_idx;
  Set_TY_tylist (ty, tylist_idx);

  (void) New_TYLIST (tylist_idx);
  Tylist_Table [tylist_idx] = 0;

  // check that TY is unique
  TY_IDX new_ty_idx = TY_is_unique (ty_idx);
  if (new_ty_idx != ty_idx && TY_IDX_index (ty_idx) == Ty_tab.Size () - 1) {
	// found duplicate
	// delete last two tylist entries
        Tylist_Table.Delete_last (2);
	Ty_tab.Delete_last ();
	ty_idx = new_ty_idx;
  }
  return ty_idx;
#endif
}

static std::vector<ST *> intrinsic_list;

struct matches_pu_ty_and_name {
  const TY_IDX        ty;
  const char   *const name;

  matches_pu_ty_and_name(const TY_IDX type, const char *const func_name) :
    ty(type), name(func_name)
      { }

  BOOL operator()(const ST *const st) {
    return (ST_pu_type(st) == ty &&
	    strcmp(ST_name(st), name) == 0);
  }
};

static void INTRINSIC_LIST_add(ST *st)
{
	intrinsic_list.push_back(st);
}

static ST *
INTRINSIC_LIST_lookup(TY_IDX  ty,
		      const char *function_name)
{
  /*
   * GNU g++-3.0 STL workaround
   * g++-3.0 has a new implementation of STL - a container's 
   * iterator is no longer a simple pointer, but a smart pointer
   * defined as __normal_iterator<Pointer, Container> in stl_algo.h.
   * And begin() end() return the smart pointers. To ensure 
   * compatibility, we use type "iterator" instead of an explicit
   * pointer.
   */
  
  std::vector<ST *>::iterator result =
    std::find_if(intrinsic_list.begin(),
		 intrinsic_list.end(),
		 matches_pu_ty_and_name(ty, function_name));
  
  if (result == intrinsic_list.end()) {
    return NULL;
  }
  else {
    return *result;  // okay because stl_algo.h defined operator* 
  }
}

ST *
Gen_Intrinsic_Function(TY_IDX  ty,
		       const char *function_name)
{
  ST *st = INTRINSIC_LIST_lookup(ty, function_name);

  if (st == NULL) {
    st = New_ST(GLOBAL_SYMTAB);

    PU_IDX pu_idx;
    PU&    pu = New_PU (pu_idx);

    PU_Init (pu, ty, CURRENT_SYMTAB);

    ST_Init (st, Save_Str(function_name), CLASS_FUNC, SCLASS_EXTERN,
             EXPORT_PREEMPTIBLE, (TY_IDX) pu_idx);

    INTRINSIC_LIST_add(st);
  }
  return st;
}


// Make an array type of equal len in each dimension
TY_IDX
Make_Array_Type (TYPE_ID element, INT32 ndim, INT64 len)
{
    FmtAssert(MTYPE_RegisterSize(element) > 0 && MTYPE_align_best(element) > 0,
              ("Cannot make an array of %s", MTYPE_name(element))); 
    ARB_HANDLE arb,arb_first;
    for (UINT i = 0; i < ndim; ++i) {
       arb = New_ARB ();
       if (i==0) {
	 arb_first = arb;
       }
       ARB_Init (arb, 0, len - 1, MTYPE_RegisterSize (element));
       Set_ARB_dimension (arb, ndim-i);
    }
    
    Set_ARB_last_dimen (arb);
    Set_ARB_first_dimen (arb_first);

    TY_IDX ty_idx;
    TY& ty = New_TY (ty_idx);
    TY_Init (ty, MTYPE_RegisterSize(element) * ndim * len, KIND_ARRAY,
	     MTYPE_UNKNOWN, 0);
    Set_TY_align (ty_idx, MTYPE_align_best(element));
    Set_TY_etype (ty, MTYPE_To_TY (element));
    Set_TY_arb (ty, arb_first);

    return ty_idx;

} // Make_Array_Type

//-------------------------------------------------------------------------
// The next few TY related functions allow us to check for TYPE_EQUIVALENCE
//-------------------------------------------------------------------------

// Hash Map
// -------------
// We keep a STL hash_map (see below) on the side to narrow down our search
// for duplicate types.
//
// The hash_map maps all TY_IDXs that represent identical types, all present
// in our global type tables, to ONE representative TY_IDX (first encountered).
// In other words, we have a map from multiple TY_IDXs to one unique
// representative.
//
// The hash_map stores the TY_IDX of every unique TY encountered so far.
// To enter types into the hash-table, call TY_is_unique(); if an equivalent
// type does not already exist the given TY_IDX will be entered into the
// hash-table as a unique type; otherwise the equivalent TY_IDX is returned.
// Note that both our hashing function and our "equal" function operate
// on the TY, not just on the TY_IDX.

// Why multiple hash_maps?
// ----------------------------
// Since the TY_are_equivalent function is fairly expensive we need good hash
// functions to reduce the number of likely matches. To that end we define 
// five different hash multimaps (rather than one) depending on the TY_kind; 
// one map each for 
// (a) scalars (and void), (b) pointers, (c) functions, (d) arrays, (e) structs

// the type of the hash table key (i.e., what the hash function takes as input)
typedef TY_IDX HashKeyType;

// Hash functions based on TY_Kind
// -------------------------------
// We use TY_kind to determine an appropriate hash-function:
// a For scalars (and void) we just use the "mtype", 
// b For pointers we use the mtype of what the pointer points to
// c For functions we use a hash based on the function signature (TY_fun_hash)
// d For arrays we use a hash function based on dim/base/bound (TY_array_hash)
// e For structs we use a hash function based on the struct name/size
// These hash functions are now defined

//----------------------------------------------------------------------
// TY_fun_hash   : maps a function prototype into a size_t value
// If the function has a signature: t1 * .. * tn -> t
// then it is represented in the TYLIST_TABLE as <t,t1,...,tn>
// We hash it in base MTYPE_LAST as the number <t_m,t1_m,...,tn_m>
// We create an MTYPE_LAST-ary number by extracting the mtypes of 
// each ti denoted by ti_m i.e., (let ML denote MTYPE_LAST)
//  (((t_m *ML) + t1_m) * ML) + ... + tn_m
//----------------------------------------------------------------------
size_t 
TY_fun_hash (const TY& ty) {
  size_t value = 0;

  TYLIST_ITER fn_iter = Make_tylist_iter(TY_tylist(ty));

  while (*fn_iter) {
    value = MTYPE_LAST * value + TY_mtype(Ty_Table[*fn_iter++]);
  }

  return value;
} // TY_fun_hash

//----------------------------------------------------------------------
// TY_array_hash : hashes an array into a size_t value determined by
//  upper_bound_value (for first dim) + (#dimensions * mtype(array_elmt))
//  if two arrays have the same base type, the same number of dimensions
//  and the same value for the upper bound of the first dimension then
//  they are likely the same and are hashed to the same bucket
//  eg int a[10][2]   and int a[10][3] would be hashed to the same bucket
//  whereas  int a[10] and double b[10] would be hashed to diff buckets 
//  likewise int a[10] and int b[11] would be hashed to different buckets
//  just as  int a[10] and int a[10][2] would be hashed to different buckets
//----------------------------------------------------------------------
size_t 
TY_array_hash (const TY& ty) {
  return ARB_ubnd_val(TY_arb(ty)) + 
    ARB_dimension(TY_arb(ty))  * TY_mtype(Ty_Table[TY_etype(ty)]) ;

} // TY_array_hash

//----------------------------------------------------------------------
// TY_struct_hash: maps a struct into a size_t value
// Return the name_idx (if set) or the size
// If the struct has a name use the unique Str_Table index as a hash
// If the struct is anonymous, use its size as a distinguishing feature
//----------------------------------------------------------------------
inline size_t 
TY_struct_hash (const TY& ty) {
  return TY_name_idx(ty) ? TY_name_idx(ty) : TY_size(ty);
} // TY_struct_hash
 

size_t TY_hash_func(const TY_IDX ty_id) {
  TY &ty = Ty_Table[ty_id];
  switch (TY_kind(ty)) {
  case KIND_SCALAR:
  case KIND_VOID:
    return TY_mtype(ty);
  case KIND_ARRAY:
    return TY_array_hash(ty);
  case KIND_POINTER:
    return TY_mtype(Ty_Table [TY_pointed(ty)]);
  case KIND_FUNCTION:
    return TY_fun_hash(ty);
  case KIND_STRUCT:
    return TY_struct_hash(ty);
  case KIND_INVALID:
  default:
  case KIND_LAST:     
    Fail_FmtAssertion ("invalid TY_KIND in hash fn");
    return 0;
  } // switch
} // TY_hash_func

struct TY_hash {
  size_t operator()(const TY_IDX ty_id) const { 
    return TY_hash_func(ty_id);
  } // operator()
}; // struct TY_hash


struct TY_EQUIV
{
  bool operator () ( const TY_IDX k1, const TY_IDX k2) const 
    {
      return TY_are_equivalent(k1, k2);
    }
};

typedef HashTable<TY_IDX, TY_IDX, TY_hash, TY_EQUIV> HASH_TY_TABLE;

// The global Hash Table data structures
// Depending on the TY_KIND we pick one of the five tables below.

HASH_TY_TABLE Hash_ty_scalar_table; 
HASH_TY_TABLE Hash_ty_array_table; 
HASH_TY_TABLE Hash_ty_struct_table; 
HASH_TY_TABLE Hash_ty_pointer_table; 
HASH_TY_TABLE Hash_ty_function_table; 

// The following is necessary to handle recursively defined types,
// such as struct with a pointer to itself as a member field.
// We maintain a record of the order in which types are visited
// in the global type table for each of the two types being compared.
// The following datastructure should be initialized to zero between
// top-level calls to TY_are_equivalent(), and Ty_Equiv_Visit_Number
// should also be zero between such top-level calls.
//
typedef SEGMENTED_ARRAY<UINT32> TY_EQUIV_VISIT_ORDER;
static TY_EQUIV_VISIT_ORDER Ty_Equiv_Visit1;
static TY_EQUIV_VISIT_ORDER Ty_Equiv_Visit2;
static UINT32               Ty_Equiv_Visit_Number = 0;

static BOOL
Push_Equiv_Visit_Order(UINT32 ty_index1, UINT32 ty_index2, BOOL &are_equiv)
{
  const UINT32 ty_tbl_size = TY_Table_Size();
  BOOL         pushed = FALSE;

  // Make sure the number of entries match the number of types,
  // and that entries are initialized to zero.
  //
  for (UINT32 i = Ty_Equiv_Visit1.Size(); i < ty_tbl_size; i++)
  {
     UINT dummy;
     Ty_Equiv_Visit1.New_entry(dummy) = 0;
     Ty_Equiv_Visit2.New_entry(dummy) = 0;
  }
  
  if (Ty_Equiv_Visit1[ty_index1] != Ty_Equiv_Visit2[ty_index2])
     are_equiv = FALSE;
  else if (Ty_Equiv_Visit1[ty_index1] > 0)
     are_equiv = TRUE;
  else
  {
     Ty_Equiv_Visit_Number++;
     Ty_Equiv_Visit1[ty_index1] = Ty_Equiv_Visit_Number;
     Ty_Equiv_Visit2[ty_index2] = Ty_Equiv_Visit_Number;
     are_equiv = FALSE;
     pushed = TRUE;
  }
  return pushed;
} // Push_Equiv_Visit_Order

static void
Pop_Equiv_Visit_Order(UINT32 ty_index1, UINT32 ty_index2)
{
  Ty_Equiv_Visit_Number--;
  Ty_Equiv_Visit1[ty_index1] = 0;
  Ty_Equiv_Visit2[ty_index2] = 0;
} // Push_Equiv_Visit_Order


// ARB_are_equivalent determines when two array types are equivalent?
// ARB_are_equivalent is presumably  called with indices for which
// first_dim is set.
// For each entry in the Arb_Table  in the range
// arb_id1 .. arb_id1'(for which last_dim is set)
// ensure that Arb_Table[arb_id1++] == Arb_Table[arb_id2++]

BOOL
ARB_are_equivalent(ARB_HANDLE arb_id1, ARB_HANDLE arb_id2, UINT32 flags) 
{
  if (arb_id1 == arb_id2)
    return TRUE;

  ARB_HANDLE arb1=arb_id1;
  ARB_HANDLE arb2=arb_id2;
  BOOL keep_going;

  do {
     
    keep_going = ! ARB_last_dimen(arb1) && ! ARB_last_dimen(arb2);
     
    if (ARB_dimension(arb1) != ARB_dimension(arb2))
      return FALSE;

    if (ARB_co_dimension(arb1) != ARB_co_dimension(arb2))
      return FALSE;

    if (ARB_flags(arb1) != ARB_flags( arb2))
      return FALSE;

    if (ARB_const_lbnd(arb1)) {
      if (ARB_lbnd_val( arb1) != ARB_lbnd_val( arb2))
        return FALSE;
    } else {
      if (ARB_lbnd_var( arb1) != ARB_lbnd_var( arb2))
        return FALSE;
    }

    if (ARB_const_ubnd(arb1)) {
      if (ARB_ubnd_val( arb1) != ARB_ubnd_val( arb2))
        return FALSE;
    } else {
      if (ARB_ubnd_var( arb1) != ARB_ubnd_var( arb2))
        return FALSE;
    }

    if (ARB_const_stride(arb1)) {
      if (ARB_stride_val( arb1) != ARB_stride_val( arb2))
        return FALSE;
    } else {
      if (ARB_stride_var(arb1) != ARB_stride_var( arb2))
        return FALSE;
    }
      
    // All tests pass; continue until you reach "last Dimension"
    if (keep_going) {
       arb1 = arb1[1];
       arb2 = arb2[1];
    }
    
  } while (keep_going);
  
  if ( ARB_last_dimen(arb2) && ARB_last_dimen(arb1))
     return TRUE;
  else
     return FALSE;
           
} // ARB_are_equivalent 

// ARB_swap swaps the positions of two ARB's in the table
void
ARB_swap(ARB_HANDLE arb1, ARB_HANDLE arb2)
{
   ARB temp;
   temp = *(arb1.Entry());
   *(arb1.Entry()) = *(arb2.Entry());
   *(arb2.Entry()) = temp;
}
   


// FLD_are_equivalent determines when two struct fields are type equivalent
// T1 fld_id1     ==   T2 fld_id2 (where fld_idi are struct fields) iff
//    a. T1 == T2 AND
//    b. FLD_name_idx  for fld_id1 == FLD_name_idx  for fld_id2
//    c. FLD_attribute for fld_id1 == FLD_attribute for fld_id2
BOOL
FLD_are_equivalent (FLD_HANDLE fld1, FLD_HANDLE fld2, UINT32 flags)
{
    if (!(flags & TY_EQUIV_IGNORE_NAMES) &&
	FLD_name_idx(fld1) != FLD_name_idx(fld2))
	return FALSE;

    if (FLD_ofst(fld1) != FLD_ofst(fld2) ||
	FLD_bsize(fld1) != FLD_bsize(fld2) ||
	FLD_bofst(fld1) != FLD_bofst(fld2) ||
	FLD_flags(fld1) != FLD_flags(fld2) ||
	FLD_st(fld1) != FLD_st(fld2))
      return FALSE;

    // For recursive type definitions, the front end may not YET have
    // set the TY_IDX (ie set it to 0) when it tries to do TY_are_equivalent
    // We have to special case this case and return FALSE (ie not duplicates)
    // Presumably this case should NOT arise in the backend and hence
    // the #if defined (FRONT_END)
    // TODO: should we restrict to NONF77 front ends?
    // If  either ty_idx is 0  return FALSE

    TY_IDX ty_id1 = FLD_type(fld1);
    TY_IDX ty_id2 = FLD_type(fld2);

#ifdef FRONT_END
    if ( (ty_id1 == 0) || (ty_id2 == 0))
	return FALSE;
#endif //FRONT_END  

    // All fields except type are equivalent; now do structural recursion

    return TY_are_equivalent(ty_id1, ty_id2, flags);

} // FLD_are_equivalent


// go through all members of a struct and return true if all of them are
// equivalent 
static BOOL
Struct_are_equivalent (FLD_HANDLE fld_id1, FLD_HANDLE fld_id2, UINT32 flags)
{
    if (fld_id1 == fld_id2)
	return TRUE;

    FLD_ITER fld1_iter = Make_fld_iter (fld_id1);
    FLD_ITER fld2_iter = Make_fld_iter (fld_id2);

    while (!FLD_last_field (fld1_iter) && !FLD_last_field (fld2_iter)) {
	if (! FLD_are_equivalent (fld1_iter, fld2_iter, flags))
	    return FALSE;
	++fld1_iter;
	++fld2_iter;
    }

    return FLD_are_equivalent (fld1_iter, fld2_iter, flags);
} // Struct_are_equivalent


// TYLIST_are_equivalent determines when two function prototypes are equivalent
// does a  pointwise comparison of component TYs (including return types)

BOOL
TYLIST_are_equivalent (TYLIST_IDX tylist_id1, 
		       TYLIST_IDX tylist_id2, 
		       UINT32     flags)
{
    if (tylist_id1 == tylist_id2)
	return TRUE;
  
    // Function prototypes are lists of tys terminated with a "0" TY_IDX entry
    //
    TY_IDX ty_id1;
    TY_IDX ty_id2;  

    while (Tylist_Table[tylist_id1] != 0) {
    
	ty_id1 = Tylist_Table[tylist_id1];
	ty_id2 = Tylist_Table[tylist_id2];
    
	if ( !TY_are_equivalent(ty_id1, ty_id2, flags))
	    return FALSE;
    
	tylist_id1++;
	tylist_id2++;
    } // end while 

    if (Tylist_Table[tylist_id2] == 0)
	return TRUE;
    else
	return FALSE;
} // TYLIST_are_equivalent


// TY_are_equivalent
// Returns TRUE iff ty1 and ty2 are structurally equivalent
// also checks that names of fields are identical 
// TODO: whether one  COMMON block is a complete subset of the other

BOOL
TY_are_equivalent (TY_IDX ty_id1, TY_IDX ty_id2, UINT32 flags)
{

  // The two types are equivalent when they have identical TY_IDX.
  // They are not necessarily equivalent if their alignments or
  // qualifiers are different.
  //
  const UINT32 ty_index1 = TY_IDX_index(ty_id1);
  const UINT32 ty_index2 = TY_IDX_index(ty_id2);
   
  if (ty_id1 == ty_id2)
    return TRUE;
  else if ((flags & TY_EQUIV_ALIGN) && 
	   TY_align_exp(ty_id1) != TY_align_exp(ty_id2))
    return FALSE;
  else if ((flags & TY_EQUIV_QUALIFIER) &&
	   (TY_is_const(ty_id1) != TY_is_const(ty_id2)       ||
	    TY_is_volatile(ty_id1) != TY_is_volatile(ty_id2) ||
	    TY_is_restrict(ty_id1) != TY_is_restrict(ty_id2)))
    return FALSE;
  else if (ty_index1 == ty_index2)
    return TRUE;

  // Handle nested structural equivalence.  If either of these types were
  // encountered before, then consider the types equivalent only if they
  // were encountered in the same order in recursive applications of this
  // subroutine.
  //
  BOOL are_equiv = FALSE;

  if (Push_Equiv_Visit_Order(ty_index1, ty_index2, are_equiv))
  {
    // Not visited before.
    //
    const TY &ty1 = Ty_Table[ty_id1];
    const TY &ty2 = Ty_Table[ty_id2];
  
    // Check attributes and names, and if they match, check for structural
    // equivalence.
    //
    if ( TY_size(ty1) != TY_size(ty2) ||
	 TY_kind(ty1) != TY_kind(ty2) ||
	 TY_mtype(ty1)!= TY_mtype(ty2)||
	 TY_flags(ty1)!= TY_flags(ty2) )
    { 
      // TODO: do we want to refine this to allow ignoring certain flags?
      are_equiv = FALSE;
    }
    else if (!(flags & TY_EQUIV_IGNORE_NAMES) &&
	     TY_name_idx (ty1) != TY_name_idx (ty2))
    { 
      are_equiv = FALSE;
    }
    else
    {
      // Set flags for recursive calls to this routine.  We always check 
      // that alignment and qualifiers are identical for nested type 
      // references.
      //
      flags |= (TY_EQUIV_ALIGN | TY_EQUIV_QUALIFIER);
       
      // At this point all the above attributes are equal, 
      // so we are ready to do "structural equivalence" checks

      switch (TY_kind (ty1)) {

      case KIND_SCALAR:
      case KIND_VOID:
	are_equiv = TRUE;
	break;

      case KIND_ARRAY:
	// array bounds check and structural recursion on base type
	are_equiv = (ARB_are_equivalent(TY_arb(ty1), TY_arb(ty2), flags) &&
		     TY_are_equivalent( TY_etype(ty1), TY_etype(ty2), flags));
	break;
	  
      case KIND_STRUCT:
	// At this point both the ty have NON_ZERO FLD_IDX

	// FLD equivalence check 
	are_equiv = Struct_are_equivalent(TY_fld(ty1), TY_fld(ty2), flags);
	break;

      case KIND_POINTER:
	// structural recursion
	are_equiv = TY_are_equivalent (TY_pointed(ty1),TY_pointed(ty2), flags);
	break;

      case KIND_FUNCTION:
	// PU flags check and structural recursion on function prototype
	are_equiv = ( (ty1.Pu_flags() == ty2.Pu_flags()) &&
		      (TYLIST_are_equivalent (TY_tylist(ty1), 
					      TY_tylist(ty2),
					      flags)));
	break;

      case KIND_INVALID:
      default:
	// TODO: do we want to assert here instead of returning FALSE?
	are_equiv = FALSE;
	break;
      }
    }
    Pop_Equiv_Visit_Order(ty_index1, ty_index2);
  }
  return are_equiv;
} // TY_are_equivalent


// TY_is_unique: returns
// a. the input TY_IDX if the input defines a unique ("new") TY. 
// b. TY_IDX with index of an existing type if the input defines a TY that
// is equivalent to the existing TY (uses TY_are_equivalent for this purpose)

static inline TY_IDX
TY_is_unique_op (const TY_IDX ty_idx, HASH_TY_TABLE& hash_ty_table)
{
  TY_IDX return_idx = ty_idx;

  HASH_TY_TABLE::ValueBoolPair isInserted = 
    hash_ty_table.insert(HASH_TY_TABLE::KeyValuePair(ty_idx, return_idx));

  if (isInserted.second == false) {
    // match was found
    Set_TY_IDX_index(return_idx, TY_IDX_index(isInserted.first));
  }
  return return_idx;

/*
  //  HASH_TY_TABLE::const_iterator hash_entry = hash_ty_table.find (ty_idx);
  HASH_TY_TABLE::const_iterator hash_entry;
  for ( hash_entry = hash_ty_table.begin(); 
        hash_entry != hash_ty_table.end();
	hash_entry++ ) {
    if (TY_are_equivalent(ty_idx, hash_entry->first)) break;
  }

  if (hash_entry != hash_ty_table.end ())
    // step b: If found, return the TY_IDX of the duplicate
     Set_TY_IDX_index(return_idx, TY_IDX_index((*hash_entry).second));
  else
    hash_ty_table.insert (HASH_TY_TABLE::value_type(ty_idx, ty_idx));
  return return_idx;
*/
}


extern TY_IDX
TY_is_unique (const TY_IDX ty_idx) 
{
  TY    &ty = Ty_Table[ty_idx];

  // search for the ty in the hash table determined by its TY_kind 

  // step a. Find "potential" matches (using hash tables).  Note that
  // the hashing and equivalence functions ignore any alignment and 
  // qualifiers in the ty_idx.
  //
  switch (TY_kind(ty)) {
    
  case KIND_SCALAR:
  case KIND_VOID:
    return TY_is_unique_op (ty_idx, Hash_ty_scalar_table);

  case KIND_ARRAY:
    return TY_is_unique_op (ty_idx, Hash_ty_array_table);

  case KIND_POINTER:
    return TY_is_unique_op (ty_idx, Hash_ty_pointer_table);

  case KIND_FUNCTION:
    return TY_is_unique_op (ty_idx, Hash_ty_function_table);

  case KIND_STRUCT:
    return TY_is_unique_op (ty_idx, Hash_ty_struct_table);

  case KIND_INVALID:
  default:
  case KIND_LAST:
    Fail_FmtAssertion ("invalid TY_KIND in TY_is_unique");
    return ty_idx;			// should never reach this
  };
} // TY_is_unique

//----------------------------------------------------------------------
// PREG-related utilities
//----------------------------------------------------------------------

const char *
Preg_Name (PREG_NUM i)
{
    Is_True((i > Last_Dedicated_Preg_Offset),
	    ("Preg_Name:  not valid for dedicated preg"));

    const PREG& preg = Preg_Table[i - Last_Dedicated_Preg_Offset];

    if (PREG_name_idx (preg) == 0)
	return "<preg>";
    else
	return PREG_name (preg);
} // Preg_Name 


// PREG_IDX == PREG_NUM - Last_Dedicated_Preg_Offset

PREG_NUM
Create_Preg_explicit(TYPE_ID mtype, const char *name, 
		     SCOPE *scope_tab, SYMTAB_IDX level)
{
	PREG_IDX preg_idx;
	PREG_IDX preg_idx2;
	PREG &p = New_PREG_explicit (scope_tab, level, preg_idx);
	Set_PREG_name_idx (p, Save_Str(name));
	// pregs of simulated types have to allocate space for
	// all the parts of the lowered types.
	switch (mtype) {
	case MTYPE_C4:
	case MTYPE_C8:
	case MTYPE_FQ:
		// reserve space for another preg
		(void) New_PREG_explicit (scope_tab, level, preg_idx2);
                Set_PREG_name_idx ((*scope_tab[level].preg_tab)[preg_idx2], 0);
		break;
	case MTYPE_CQ:
		// reserve space for 3 more pregs
		(void) New_PREG_explicit (scope_tab, level, preg_idx2);
                Set_PREG_name_idx ((*scope_tab[level].preg_tab)[preg_idx2], 0);
		(void) New_PREG_explicit (scope_tab, level, preg_idx2);
                Set_PREG_name_idx ((*scope_tab[level].preg_tab)[preg_idx2], 0);
		(void) New_PREG_explicit (scope_tab, level, preg_idx2);
                Set_PREG_name_idx ((*scope_tab[level].preg_tab)[preg_idx2], 0);
		break;
	case MTYPE_B:
		// bool needs 2 pregs cause can have complement predicate tn
		(void) New_PREG_explicit (scope_tab, level, preg_idx2);
                Set_PREG_name_idx ((*scope_tab[level].preg_tab)[preg_idx2], 0);
		break;
	}
	// return preg-num of first preg
	return (PREG_NUM) preg_idx + Last_Dedicated_Preg_Offset;
}

PREG_NUM
Create_Preg (TYPE_ID mtype, const char *name)
{
  return Create_Preg_explicit(mtype, name, Scope_tab, CURRENT_SYMTAB);
}

// uses the real preg size because simulated pregs might take more than one
// preg number.
INT32
Preg_Increment (TYPE_ID mtype)
{
    switch (mtype) {

    case MTYPE_C4:
    case MTYPE_C8:
    case MTYPE_FQ:
	return 2;

    case MTYPE_CQ:
	return 4;

    case MTYPE_I8:
    case MTYPE_U8:
	if (MTYPE_size_reg(MTYPE_I8) > MTYPE_size_reg(Spill_Int_Mtype))
	    return 2;
    case MTYPE_B:
	// bool mtype not usually used, but if used, saves space for
	// complement preg.
	return 2;
    }
    return 1;
}


//----------------------------------------------------------------------
// BLK utilities
//----------------------------------------------------------------------

BLK_IDX
Copy_BLK (BLK_IDX b)
{
    BLK_IDX copy = b;
    copy = Blk_Table.Insert (Blk_Table[b]);
    return copy;
}

/* ====================================================================
 *
 * Base_Symbol_And_Offset
 *      Input:  ST *st                   Symbol to analyze
 *      Result: ST **base_symbol         primary base of st
 *      Result: INT64 *offset_from_base  offset from primary base
 *
 *
 * ====================================================================
 */
void
Base_Symbol_And_Offset (ST     *st,
                        ST    **base_symbol,
                        INT64  *offset_from_base)
{
  INT64 ofst = 0;
  ST *base = st;

  while ( ST_base(base) != base  ) {
      ofst += ST_ofst(base);
      base = ST_base(base);
  }

  *base_symbol      = base;
  *offset_from_base = ofst;
}


//----------------------------------------------------------------------
// Printing routines
//----------------------------------------------------------------------

/* ====================================================================
 *
 * Class_Name / Sclass_Name / Export_Name / Kind_Name
 *   Given ST field values, convert to ASCII names.
 *
 * Name_To_Class / Name_To_Sclass / Name_To_Export / Name_To_Kind
 *   Given ASCII names, convert to ST field values.
 *
 * ====================================================================
 */

// eraxxon (2005.01): Re-implement these routines to support
// conversion in both directions.

// The type for enumeration value -> string tables
struct EnumToStr_t : public ir_a2b::enum2str_tbl_entry_t {
  EnumToStr_t(INT val_ = 0, const char* str_ = 0) 
    : val(val_), str(str_) { }

  virtual ~EnumToStr_t() { }

  virtual INT getEnumVal() const     { return val; }
  virtual const char* getStr() const { return str; }

  INT         val;
  const char* str;
};


// The data tables
EnumToStr_t STClassToNameTbl[CLASS_COUNT] = {
  EnumToStr_t(CLASS_UNK,       "C_UNK"),
  EnumToStr_t(CLASS_VAR,       "C_VAR"),
  EnumToStr_t(CLASS_FUNC,      "C_FUNC"),
  EnumToStr_t(CLASS_CONST,     "C_CONST"),
  EnumToStr_t(CLASS_PREG,      "C_PREG"),
  EnumToStr_t(CLASS_BLOCK,     "C_BLOCK"),
  EnumToStr_t(CLASS_NAME,      "C_NAME"),
  EnumToStr_t(CLASS_MODULE,    "C_MODULE"),
  EnumToStr_t(CLASS_TYPE,      "C_TYPE"),
  EnumToStr_t(CLASS_PARAMETER, "C_PARAMETER")
};

EnumToStr_t STSclassToNameTbl[SCLASS_COUNT] = {
  EnumToStr_t(SCLASS_UNKNOWN,   "S_UNKNOWN"),
  EnumToStr_t(SCLASS_AUTO,      "S_AUTO"),
  EnumToStr_t(SCLASS_FORMAL,    "S_FORMAL"),
  EnumToStr_t(SCLASS_FORMAL_REF,"S_FORMAL_REF"),
  EnumToStr_t(SCLASS_PSTATIC,   "S_PSTATIC"),
  EnumToStr_t(SCLASS_FSTATIC,   "S_FSTATIC"),
  EnumToStr_t(SCLASS_COMMON,    "S_COMMON"),
  EnumToStr_t(SCLASS_EXTERN,    "S_EXTERN"),
  EnumToStr_t(SCLASS_UGLOBAL,   "S_UGLOBAL"),
  EnumToStr_t(SCLASS_DGLOBAL,   "S_DGLOBAL"),
  EnumToStr_t(SCLASS_TEXT,      "S_TEXT"),
  EnumToStr_t(SCLASS_REG,       "S_REG"),
  EnumToStr_t(SCLASS_CPLINIT,   "S_CPLINIT"),
  EnumToStr_t(SCLASS_EH_REGION, "S_EH_REGION"),
  EnumToStr_t(SCLASS_EH_REGION_SUPP, "S_EH_REGION_SUPP"),
  EnumToStr_t(SCLASS_DISTR_ARRAY, "S_DISTR_ARRAY"),
  EnumToStr_t(SCLASS_COMMENT,   "S_COMMENT"),
  EnumToStr_t(SCLASS_THREAD_PRIVATE_FUNCS, "S_THREAD_PRIVATE_FUNCS"),
  EnumToStr_t(SCLASS_MODULE,    "S_MODULE"),
  EnumToStr_t(SCLASS_COMMON1,   "S_COMMON1"),
};

EnumToStr_t STExportToNameTbl[EXPORT_COUNT] = {
  EnumToStr_t(EXPORT_LOCAL,          "X_LOCAL"),
  EnumToStr_t(EXPORT_LOCAL_INTERNAL, "X_LOCAL_INTERNAL"),
  EnumToStr_t(EXPORT_INTERNAL,       "X_INTERNAL"),
  EnumToStr_t(EXPORT_HIDDEN,         "X_HIDDEN"),
  EnumToStr_t(EXPORT_PROTECTED,      "X_PROTECTED"),
  EnumToStr_t(EXPORT_PREEMPTIBLE,    "X_PREEMPTIBLE"),
  EnumToStr_t(EXPORT_OPTIONAL,       "X_OPTIONAL"),
  EnumToStr_t(EXPORT_INTRINSIC,       "X_INTRINSIC"),
};

EnumToStr_t TYKindToNameTbl[KIND_LAST] = {
  EnumToStr_t(KIND_INVALID,  "INVALID"),
  EnumToStr_t(KIND_SCALAR,   "SCALAR"),
  EnumToStr_t(KIND_ARRAY,    "ARRAY"),
  EnumToStr_t(KIND_STRUCT,   "STRUCT"),
  EnumToStr_t(KIND_POINTER,  "POINTER"),
  EnumToStr_t(KIND_FUNCTION, "FUNCTION"),
  EnumToStr_t(KIND_VOID,     "VOID"),
};


#define STATTR_ToStrTblENTRY(flg) \
  EnumToStr_t(flg, #flg)

EnumToStr_t STATTR_ToStrTbl[] = {
  STATTR_ToStrTblENTRY(ST_ATTR_UNKNOWN),
  STATTR_ToStrTblENTRY(ST_ATTR_DEDICATED_REGISTER),
  STATTR_ToStrTblENTRY(ST_ATTR_SECTION_NAME)
};


#define LKIND_ToStrTblENTRY(flg) \
  EnumToStr_t(flg, #flg)

EnumToStr_t LKIND_ToStrTbl[] = {
  LKIND_ToStrTblENTRY(LKIND_DEFAULT),
  LKIND_ToStrTblENTRY(LKIND_ASSIGNED),
  LKIND_ToStrTblENTRY(LKIND_BEGIN_EH_RANGE),
  LKIND_ToStrTblENTRY(LKIND_END_EH_RANGE),
  LKIND_ToStrTblENTRY(LKIND_BEGIN_HANDLER),
  LKIND_ToStrTblENTRY(LKIND_END_HANDLER),
  LKIND_ToStrTblENTRY(LKIND_TAG),
  LKIND_ToStrTblENTRY(LKIND_INTERNAL),
  LKIND_ToStrTblENTRY(LKIND_LOOP_GEN),
  LKIND_ToStrTblENTRY(LKIND_SELECT_GEN)
};


const char *
Class_Name (ST_CLASS cl)
{
  using namespace ir_a2b;
  return MapEnumToStr<EnumToStr_t, STClassToNameTbl,
                      CLASS_COUNT>("STClassToNameTbl", (INT)cl);
}

ST_CLASS
Name_To_Class (const char* nm) 
{
  using namespace ir_a2b;
  return (ST_CLASS)MapStrToEnum<EnumToStr_t, STClassToNameTbl, 
                                CLASS_COUNT>("STClassToNameTbl", nm);
}


const char *
Sclass_Name (ST_SCLASS sc)
{
  using namespace ir_a2b;
  return MapEnumToStr<EnumToStr_t, STSclassToNameTbl, 
                      SCLASS_COUNT>("STSclassToNameTbl", (INT)sc);
}

ST_SCLASS
Name_To_Sclass (const char* nm)
{
  using namespace ir_a2b;
  return (ST_SCLASS)MapStrToEnum<EnumToStr_t, STSclassToNameTbl, 
                                 SCLASS_COUNT>("STSclassToNameTbl", nm);
}


const char *
Export_Name (ST_EXPORT ex)
{
  using namespace ir_a2b;
  return MapEnumToStr<EnumToStr_t, STExportToNameTbl, 
                      EXPORT_COUNT>("STExportToNameTbl", (INT)ex);
}

ST_EXPORT
Name_To_Export (const char* nm)
{
  using namespace ir_a2b;
  return (ST_EXPORT)MapStrToEnum<EnumToStr_t, STExportToNameTbl, 
                                 EXPORT_COUNT>("STExportToNameTbl", nm);
}


const char *
Kind_Name (TY_KIND k)
{
  using namespace ir_a2b;
  return MapEnumToStr<EnumToStr_t, TYKindToNameTbl, 
                      KIND_LAST>("TYKindToNameTbl", (INT)k);
}

TY_KIND
Name_To_Kind (const char* nm)
{
  using namespace ir_a2b;
  return (TY_KIND)MapStrToEnum<EnumToStr_t, TYKindToNameTbl, 
                               KIND_LAST>("TYKindToNameTbl", nm);
}


const char *
LABEL_Kind_Name (LABEL_KIND k)
{
  using namespace ir_a2b;
  return MapEnumToStr<EnumToStr_t, LKIND_ToStrTbl, 
                      LKIND_COUNT>("LKIND_ToStrTbl", (INT)k);
}

LABEL_KIND
Name_To_LABEL_Kind (const char* nm)
{
  using namespace ir_a2b;
  return (LABEL_KIND)MapStrToEnum<EnumToStr_t, LKIND_ToStrTbl, 
                                  LKIND_COUNT>("LKIND_ToStrTbl", nm);
}


const char *
ST_ATTR_Kind_Name (ST_ATTR_KIND k)
{
  using namespace ir_a2b;
  return MapEnumToStr<EnumToStr_t, STATTR_ToStrTbl, 
                      ST_ATTR_KIND_COUNT>("STATTR_ToStrTbl", (INT)k);
}

ST_ATTR_KIND
Name_To_ST_ATTR_Kind (const char* nm)
{
  using namespace ir_a2b;
  return (ST_ATTR_KIND)MapStrToEnum<EnumToStr_t, STATTR_ToStrTbl, 
                                    ST_ATTR_KIND_COUNT>("STATTR_ToStrTbl", nm);
}


/* ====================================================================
 *
 * xxxFlag_Name 
 *   Given flag set, convert to a comma-separated flag string
 *
 * Name_To_xxxFlag
 *   Given a comma-separated flag string, convert to a flags set
 *
 * ====================================================================
 */

// eraxxon (2005.01): Create these routines to support conversion in
// both directions.

// The type for flag value -> string tables
struct FlagToStr_t : public ir_a2b::flag2str_tbl_entry_t {
  FlagToStr_t(UINT64 val_ = 0, const char* str_ = 0) 
    : val(val_), str(str_) { }

  virtual ~FlagToStr_t() { }

  virtual UINT64 getFlagVal() const  { return val; }
  virtual const char* getStr() const { return str; }

  UINT64      val;
  const char* str;
};


// FILE_INFO_FLAGS
#define FIFLAGS_ToStrTblENTRY(flg) \
  FlagToStr_t(flg, #flg)

FlagToStr_t FIFLAGS_ToStrTbl[] = {
  FIFLAGS_ToStrTblENTRY(FI_IPA),
  FIFLAGS_ToStrTblENTRY(FI_NEEDS_LNO),
  FIFLAGS_ToStrTblENTRY(FI_HAS_INLINES),
  FIFLAGS_ToStrTblENTRY(FI_HAS_MP)
};

const UINT FIFLAGS_ToStrTblSZ = 
  (sizeof(FIFLAGS_ToStrTbl) / sizeof(FlagToStr_t));


// ST_FLAGS
#define STFLAGS_ToStrTblENTRY(flg) \
  FlagToStr_t(flg, #flg)

FlagToStr_t STFLAGS_ToStrTbl[] = {
  STFLAGS_ToStrTblENTRY(ST_IS_WEAK_SYMBOL),
  STFLAGS_ToStrTblENTRY(ST_IS_SPLIT_COMMON),
  STFLAGS_ToStrTblENTRY(ST_IS_NOT_USED),
  STFLAGS_ToStrTblENTRY(ST_IS_INITIALIZED),
  STFLAGS_ToStrTblENTRY(ST_IS_RETURN_VAR),
  STFLAGS_ToStrTblENTRY(ST_IS_VALUE_PARM),
  STFLAGS_ToStrTblENTRY(ST_PROMOTE_PARM),
  STFLAGS_ToStrTblENTRY(ST_KEEP_NAME_W2F),
  STFLAGS_ToStrTblENTRY(ST_IS_DATAPOOL),
  STFLAGS_ToStrTblENTRY(ST_IS_RESHAPED),
  STFLAGS_ToStrTblENTRY(ST_EMIT_SYMBOL),
  STFLAGS_ToStrTblENTRY(ST_HAS_NESTED_REF),
  STFLAGS_ToStrTblENTRY(ST_INIT_VALUE_ZERO),
  STFLAGS_ToStrTblENTRY(ST_GPREL),
  STFLAGS_ToStrTblENTRY(ST_NOT_GPREL),
  STFLAGS_ToStrTblENTRY(ST_IS_NAMELIST),
  STFLAGS_ToStrTblENTRY(ST_IS_F90_TARGET),
  STFLAGS_ToStrTblENTRY(ST_DECLARED_STATIC),
  STFLAGS_ToStrTblENTRY(ST_IS_EQUIVALENCED),
  STFLAGS_ToStrTblENTRY(ST_IS_FILL_ALIGN),
  STFLAGS_ToStrTblENTRY(ST_IS_OPTIONAL_ARGUMENT),
  STFLAGS_ToStrTblENTRY(ST_PT_TO_UNIQUE_MEM),
  STFLAGS_ToStrTblENTRY(ST_IS_TEMP_VAR),
  STFLAGS_ToStrTblENTRY(ST_IS_CONST_VAR),
  STFLAGS_ToStrTblENTRY(ST_ADDR_SAVED),
  STFLAGS_ToStrTblENTRY(ST_ADDR_PASSED),
  STFLAGS_ToStrTblENTRY(ST_IS_THREAD_PRIVATE),
  STFLAGS_ToStrTblENTRY(ST_PT_TO_COMPILER_GENERATED_MEM),
  STFLAGS_ToStrTblENTRY(ST_IS_SHARED_AUTO),
  STFLAGS_ToStrTblENTRY(ST_ASSIGNED_TO_DEDICATED_PREG),
  STFLAGS_ToStrTblENTRY(ST_ASM_FUNCTION_ST),
  STFLAGS_ToStrTblENTRY(ST_HAS_NAMED_SECTION)
};

const UINT STFLAGS_ToStrTblSZ = 
  (sizeof(STFLAGS_ToStrTbl) / sizeof(FlagToStr_t));


// ST_EXT_FLAGS
#define STEXTFLAGS_ToStrTblENTRY(flg) \
  FlagToStr_t(flg, #flg)

FlagToStr_t STEXTFLAGS_ToStrTbl[] = {
  STEXTFLAGS_ToStrTblENTRY(ST_IS_POINTER),  
  STEXTFLAGS_ToStrTblENTRY(ST_IS_ALLOCATABLE),
  STEXTFLAGS_ToStrTblENTRY(ST_IS_IN_MODULE),
  STEXTFLAGS_ToStrTblENTRY(ST_IS_EXTERNAL),
  STEXTFLAGS_ToStrTblENTRY(ST_IS_BLOCK_DATA), 
  STEXTFLAGS_ToStrTblENTRY(ST_IS_INTENT_IN_ARGUMENT),
  STEXTFLAGS_ToStrTblENTRY(ST_IS_INTENT_OUT_ARGUMENT),
  STEXTFLAGS_ToStrTblENTRY(ST_IS_ASSIGN_INTERFACE),
  STEXTFLAGS_ToStrTblENTRY(ST_IS_OPERATOR_INTERFACE),
  STEXTFLAGS_ToStrTblENTRY(ST_IS_U_OPERATOR_INTERFACE),
  STEXTFLAGS_ToStrTblENTRY(ST_IS_PRIVATE),
  STEXTFLAGS_ToStrTblENTRY(ST_IS_M_IMPORTED),
  STEXTFLAGS_ToStrTblENTRY(ST_IS_IMPLEM_LEVEL_TEMP),
  STEXTFLAGS_ToStrTblENTRY(ST_IS_PARAMETER),
  STEXTFLAGS_ToStrTblENTRY(ST_ONE_PER_PU),
  STEXTFLAGS_ToStrTblENTRY(ST_COPY_CONSTRUCTOR_ST),
  STEXTFLAGS_ToStrTblENTRY(ST_INITV_IN_OTHER_ST),
  STEXTFLAGS_ToStrTblENTRY(ST_IS_INITIALIZED_IN_F90),
  STEXTFLAGS_ToStrTblENTRY(ST_IS_DELETED), 
  STEXTFLAGS_ToStrTblENTRY(ST_IS_COARRAY_CONCURRENT), 
  STEXTFLAGS_ToStrTblENTRY(ST_KEEP_IN_OPENAD), 
};

const UINT STEXTFLAGS_ToStrTblSZ = 
  (sizeof(STEXTFLAGS_ToStrTbl) / sizeof(FlagToStr_t));


// FLD_FLAGS
#define FLDFLAGS_ToStrTblENTRY(flg) \
  FlagToStr_t(flg, #flg)

FlagToStr_t FLDFLAGS_ToStrTbl[] = {
  FLDFLAGS_ToStrTblENTRY(FLD_LAST_FIELD),
  FLDFLAGS_ToStrTblENTRY(FLD_EQUIVALENCE),
  FLDFLAGS_ToStrTblENTRY(FLD_BEGIN_UNION),
  FLDFLAGS_ToStrTblENTRY(FLD_END_UNION),
  FLDFLAGS_ToStrTblENTRY(FLD_BEGIN_MAP),
  FLDFLAGS_ToStrTblENTRY(FLD_END_MAP),
  FLDFLAGS_ToStrTblENTRY(FLD_IS_BIT_FIELD),
  FLDFLAGS_ToStrTblENTRY(FLD_IS_POINTER)
};

const UINT FLDFLAGS_ToStrTblSZ = 
  (sizeof(FLDFLAGS_ToStrTbl) / sizeof(FlagToStr_t));


// ARB_FLAGS
#define ARBFLAGS_ToStrTblENTRY(flg) \
  FlagToStr_t(flg, #flg)

FlagToStr_t ARBFLAGS_ToStrTbl[] = {
  ARBFLAGS_ToStrTblENTRY(ARB_CONST_LBND),
  ARBFLAGS_ToStrTblENTRY(ARB_CONST_UBND),
  ARBFLAGS_ToStrTblENTRY(ARB_CONST_STRIDE),
  ARBFLAGS_ToStrTblENTRY(ARB_FIRST_DIMEN),
  ARBFLAGS_ToStrTblENTRY(ARB_LAST_DIMEN),
  ARBFLAGS_ToStrTblENTRY(ARB_EMPTY_LBND),
  ARBFLAGS_ToStrTblENTRY(ARB_EMPTY_UBND),
  ARBFLAGS_ToStrTblENTRY(ARB_STAR_UBND),
  ARBFLAGS_ToStrTblENTRY(ARB_EMPTY_STRIDE)
};

const UINT ARBFLAGS_ToStrTblSZ = 
  (sizeof(ARBFLAGS_ToStrTbl) / sizeof(FlagToStr_t));


// LABEL_FLAGS
#define LBLFLAGS_ToStrTblENTRY(flg) \
  FlagToStr_t(flg, #flg)

FlagToStr_t LBLFLAGS_ToStrTbl[] = {
  LBLFLAGS_ToStrTblENTRY(LABEL_TARGET_OF_GOTO_OUTER_BLOCK),
  LBLFLAGS_ToStrTblENTRY(LABEL_ADDR_SAVED),
  LBLFLAGS_ToStrTblENTRY(LABEL_ADDR_PASSED)
};

const UINT LBLFLAGS_ToStrTblSZ = 
  (sizeof(LBLFLAGS_ToStrTbl) / sizeof(FlagToStr_t));


// TY_FLAGS
#define TYFLAGS_ToStrTblENTRY(flg) \
  FlagToStr_t(flg, #flg)

FlagToStr_t TYFLAGS_ToStrTbl[] = {
  TYFLAGS_ToStrTblENTRY(TY_IS_CHARACTER),
  TYFLAGS_ToStrTblENTRY(TY_IS_LOGICAL),
  TYFLAGS_ToStrTblENTRY(TY_IS_UNION),
  TYFLAGS_ToStrTblENTRY(TY_IS_PACKED),
  TYFLAGS_ToStrTblENTRY(TY_PTR_AS_ARRAY),
  TYFLAGS_ToStrTblENTRY(TY_ANONYMOUS),
  TYFLAGS_ToStrTblENTRY(TY_SPLIT),
  TYFLAGS_ToStrTblENTRY(TY_IS_F90_POINTER),
  TYFLAGS_ToStrTblENTRY(TY_NOT_IN_UNION),
  TYFLAGS_ToStrTblENTRY(TY_NO_ANSI_ALIAS),
  TYFLAGS_ToStrTblENTRY(TY_IS_NON_POD),
  TYFLAGS_ToStrTblENTRY(TY_IS_F90_ASSUMED_SHAPE),
  TYFLAGS_ToStrTblENTRY(TY_IS_F90_ASSUMED_SIZE),
  TYFLAGS_ToStrTblENTRY(TY_IS_F90_DEFERRED_SHAPE),
  TYFLAGS_ToStrTblENTRY(TY_IS_EXTERNAL),
  TYFLAGS_ToStrTblENTRY(TY_IS_SEQUENCE),
  TYFLAGS_ToStrTblENTRY(TY_IS_SHARED),
  TYFLAGS_ToStrTblENTRY(TY_IS_STRICT),
  TYFLAGS_ToStrTblENTRY(TY_IS_RELAXED),
  TYFLAGS_ToStrTblENTRY(TY_IS_CO_ARRAY),
  TYFLAGS_ToStrTblENTRY(TY_IS_WRITTEN),
  TYFLAGS_ToStrTblENTRY(TY_RETURN_IN_MEM)
};

const UINT TYFLAGS_ToStrTblSZ = 
  (sizeof(TYFLAGS_ToStrTbl) / sizeof(FlagToStr_t));


// TY_PU_FLAGS
#define TYPUFLAGS_ToStrTblENTRY(flg) \
  FlagToStr_t(flg, #flg)

FlagToStr_t TYPUFLAGS_ToStrTbl[] = {
  TYPUFLAGS_ToStrTblENTRY(TY_RETURN_TO_PARAM),
  TYPUFLAGS_ToStrTblENTRY(TY_IS_VARARGS),
  TYPUFLAGS_ToStrTblENTRY(TY_HAS_PROTOTYPE)
};

const UINT TYPUFLAGS_ToStrTblSZ = 
  (sizeof(TYPUFLAGS_ToStrTbl) / sizeof(FlagToStr_t));


// PU_FLAGS
#define PUFLAGS_ToStrTblENTRY(flg) \
  FlagToStr_t(flg, #flg)

FlagToStr_t PUFLAGS_ToStrTbl[] = {
  PUFLAGS_ToStrTblENTRY(PU_IS_PURE),
  PUFLAGS_ToStrTblENTRY(PU_NO_SIDE_EFFECTS),
  PUFLAGS_ToStrTblENTRY(PU_IS_INLINE_FUNCTION),
  PUFLAGS_ToStrTblENTRY(PU_NO_INLINE),
  PUFLAGS_ToStrTblENTRY(PU_MUST_INLINE),
  PUFLAGS_ToStrTblENTRY(PU_NO_DELETE),
  PUFLAGS_ToStrTblENTRY(PU_HAS_EXC_SCOPES),
  PUFLAGS_ToStrTblENTRY(PU_IS_NESTED_FUNC),
  PUFLAGS_ToStrTblENTRY(PU_HAS_NON_MANGLED_CALL),
  PUFLAGS_ToStrTblENTRY(PU_ARGS_ALIASED),
  PUFLAGS_ToStrTblENTRY(PU_NEEDS_FILL_ALIGN_LOWERING),
  PUFLAGS_ToStrTblENTRY(PU_NEEDS_T9),
  PUFLAGS_ToStrTblENTRY(PU_HAS_VERY_HIGH_WHIRL),
  PUFLAGS_ToStrTblENTRY(PU_HAS_ALTENTRY),
  PUFLAGS_ToStrTblENTRY(PU_RECURSIVE),
  PUFLAGS_ToStrTblENTRY(PU_IS_MAINPU),
  PUFLAGS_ToStrTblENTRY(PU_UPLEVEL),
  PUFLAGS_ToStrTblENTRY(PU_MP_NEEDS_LNO),
  PUFLAGS_ToStrTblENTRY(PU_HAS_ALLOCA),
  PUFLAGS_ToStrTblENTRY(PU_IN_ELF_SECTION),
  PUFLAGS_ToStrTblENTRY(PU_HAS_MP),
  PUFLAGS_ToStrTblENTRY(PU_MP),
  PUFLAGS_ToStrTblENTRY(PU_HAS_NAMELIST),
  PUFLAGS_ToStrTblENTRY(PU_HAS_RETURN_ADDRESS),
  PUFLAGS_ToStrTblENTRY(PU_HAS_REGION),
  PUFLAGS_ToStrTblENTRY(PU_HAS_INLINES),
  PUFLAGS_ToStrTblENTRY(PU_CALLS_SETJMP),
  PUFLAGS_ToStrTblENTRY(PU_CALLS_LONGJMP),
  PUFLAGS_ToStrTblENTRY(PU_IPA_ADDR_ANALYSIS),
  PUFLAGS_ToStrTblENTRY(PU_SMART_ADDR_ANALYSIS),
  PUFLAGS_ToStrTblENTRY(PU_HAS_SYSCALL_LINKAGE),
  PUFLAGS_ToStrTblENTRY(PU_HAS_GLOBAL_PRAGMAS),
  PUFLAGS_ToStrTblENTRY(PU_HAS_USER_ALLOCA),
  PUFLAGS_ToStrTblENTRY(PU_HAS_UNKNOWN_CONTROL_FLOW),
  PUFLAGS_ToStrTblENTRY(PU_IS_THUNK),
  PUFLAGS_ToStrTblENTRY(PU_DECL_VIEW),
  PUFLAGS_ToStrTblENTRY(PU_NEED_UNPARSED),
  PUFLAGS_ToStrTblENTRY(PU_NEEDS_MANUAL_UNWINDING),
#ifdef TARG_X8664
  PUFLAGS_ToStrTblENTRY(PU_FF2C_ABI)
#endif
};

const UINT PUFLAGS_ToStrTblSZ = 
  (sizeof(PUFLAGS_ToStrTbl) / sizeof(FlagToStr_t));


// PU_SRC_LANG_FLAGS
#define PUSRCLANGFLAGS_ToStrTblENTRY(flg) \
  FlagToStr_t(flg, #flg)

FlagToStr_t PUSRCLANGFLAGS_ToStrTbl[] = {
  PUSRCLANGFLAGS_ToStrTblENTRY(PU_UNKNOWN_LANG),
  PUSRCLANGFLAGS_ToStrTblENTRY(PU_MIXED_LANG),
  PUSRCLANGFLAGS_ToStrTblENTRY(PU_C_LANG),
  PUSRCLANGFLAGS_ToStrTblENTRY(PU_CXX_LANG),
  PUSRCLANGFLAGS_ToStrTblENTRY(PU_F77_LANG),
  PUSRCLANGFLAGS_ToStrTblENTRY(PU_F90_LANG),
  PUSRCLANGFLAGS_ToStrTblENTRY(PU_JAVA_LANG)
};

const UINT PUSRCLANGFLAGS_ToStrTblSZ = 
  (sizeof(PUSRCLANGFLAGS_ToStrTbl) / sizeof(FlagToStr_t));


// BLK
#define BLKFLAGS_ToStrTblENTRY(flg) \
  FlagToStr_t(flg, #flg)

FlagToStr_t BLKFLAGS_ToStrTbl[] = {
  BLKFLAGS_ToStrTblENTRY(BLK_SECTION),
  BLKFLAGS_ToStrTblENTRY(BLK_ROOT_BASE),
  BLKFLAGS_ToStrTblENTRY(BLK_IS_BASEREG),
  BLKFLAGS_ToStrTblENTRY(BLK_DECREMENT),
  BLKFLAGS_ToStrTblENTRY(BLK_EXEC),
  BLKFLAGS_ToStrTblENTRY(BLK_NOBITS),
  BLKFLAGS_ToStrTblENTRY(BLK_MERGE),
  BLKFLAGS_ToStrTblENTRY(BLK_COMPILER_LAYOUT)
};

const UINT BLKFLAGS_ToStrTblSZ = 
  (sizeof(BLKFLAGS_ToStrTbl) / sizeof(FlagToStr_t));


const char *
FILE_INFO_FLAGS_To_Str (UINT64 flags)
{
  using namespace ir_a2b;
  return MapFlagsToStr<FlagToStr_t, FIFLAGS_ToStrTbl, 
                       FIFLAGS_ToStrTblSZ>("FIFLAGS_ToStrTbl", flags);
}

UINT64
Str_To_FILE_INFO_FLAGS (const char* str)
{
  using namespace ir_a2b;
  return MapStrToFlags<FlagToStr_t, FIFLAGS_ToStrTbl, 
                       FIFLAGS_ToStrTblSZ>("FIFLAGS_ToStrTbl", str);
}


const char *
ST_FLAGS_To_Str (UINT64 flags)
{
  using namespace ir_a2b;
  return MapFlagsToStr<FlagToStr_t, STFLAGS_ToStrTbl, 
                       STFLAGS_ToStrTblSZ>("STFLAGS_ToStrTbl", flags);
}

UINT64
Str_To_ST_FLAGS (const char* str)
{
  using namespace ir_a2b;
  return MapStrToFlags<FlagToStr_t, STFLAGS_ToStrTbl, 
                       STFLAGS_ToStrTblSZ>("STFLAGS_ToStrTbl", str);
}


const char *
ST_EXT_FLAGS_To_Str (UINT64 flags)
{
  using namespace ir_a2b;
  return MapFlagsToStr<FlagToStr_t, STEXTFLAGS_ToStrTbl,
		       STEXTFLAGS_ToStrTblSZ>("STEXTFLAGS_ToStrTbl", flags);
}

UINT64
Str_To_ST_EXT_FLAGS (const char* str)
{
  using namespace ir_a2b;
  return MapStrToFlags<FlagToStr_t, STEXTFLAGS_ToStrTbl, 
                       STEXTFLAGS_ToStrTblSZ>("STEXTFLAGS_ToStrTbl", str);
}


const char *
FLD_FLAGS_To_Str (UINT64 flags)
{
  using namespace ir_a2b;
  return MapFlagsToStr<FlagToStr_t, FLDFLAGS_ToStrTbl,
		       FLDFLAGS_ToStrTblSZ>("FLDFLAGS_ToStrTbl", flags);
}

UINT64
Str_To_FLD_FLAGS (const char* str)
{
  using namespace ir_a2b;
  return MapStrToFlags<FlagToStr_t, FLDFLAGS_ToStrTbl, 
                       FLDFLAGS_ToStrTblSZ>("FLDFLAGS_ToStrTbl", str);
}


const char *
ARB_FLAGS_To_Str (UINT64 flags)
{
  using namespace ir_a2b;
  return MapFlagsToStr<FlagToStr_t, ARBFLAGS_ToStrTbl, 
		       ARBFLAGS_ToStrTblSZ>("ARBFLAGS_ToStrTbl", flags);
}

UINT64
Str_To_ARB_FLAGS (const char* str)
{
  using namespace ir_a2b;
  return MapStrToFlags<FlagToStr_t, ARBFLAGS_ToStrTbl, 
                       ARBFLAGS_ToStrTblSZ>("ARBFLAGS_ToStrTbl", str);
}


const char *
LABEL_FLAGS_To_Str (UINT64 flags)
{
  using namespace ir_a2b;
  return MapFlagsToStr<FlagToStr_t, LBLFLAGS_ToStrTbl, 
		       LBLFLAGS_ToStrTblSZ>("LBLFLAGS_ToStrTbl", flags);
}

UINT64
Str_To_LABEL_FLAGS (const char* str)
{
  using namespace ir_a2b;
  return MapStrToFlags<FlagToStr_t, LBLFLAGS_ToStrTbl, 
                       LBLFLAGS_ToStrTblSZ>("LBLFLAGS_ToStrTbl", str);
}


const char *
TY_FLAGS_To_Str (UINT64 flags)
{
  using namespace ir_a2b;
  return MapFlagsToStr<FlagToStr_t, TYFLAGS_ToStrTbl, 
		       TYFLAGS_ToStrTblSZ>("TYFLAGS_ToStrTbl", flags);
}

UINT64
Str_To_TY_FLAGS (const char* str)
{
  using namespace ir_a2b;
  return MapStrToFlags<FlagToStr_t, TYFLAGS_ToStrTbl, 
                       TYFLAGS_ToStrTblSZ>("TYFLAGS_ToStrTbl", str);
}


const char *
TY_PU_FLAGS_To_Str (UINT64 flags)
{
  using namespace ir_a2b;
  return MapFlagsToStr<FlagToStr_t, TYPUFLAGS_ToStrTbl, 
		       TYPUFLAGS_ToStrTblSZ>("TYPUFLAGS_ToStrTbl", flags);
}

UINT64
Str_To_TY_PU_FLAGS (const char* str)
{
  using namespace ir_a2b;
  return MapStrToFlags<FlagToStr_t, TYPUFLAGS_ToStrTbl, 
                       TYPUFLAGS_ToStrTblSZ>("TYPUFLAGS_ToStrTbl", str);
}


const char *
PU_FLAGS_To_Str (UINT64 flags)
{
  using namespace ir_a2b;
  return MapFlagsToStr<FlagToStr_t, PUFLAGS_ToStrTbl, 
		       PUFLAGS_ToStrTblSZ>("PUFLAGS_ToStrTbl", flags);
}

UINT64
Str_To_PU_FLAGS (const char* str)
{
  using namespace ir_a2b;
  return MapStrToFlags<FlagToStr_t, PUFLAGS_ToStrTbl, 
                       PUFLAGS_ToStrTblSZ>("PUFLAGS_ToStrTbl", str);
}


const char *
PU_SRC_LANG_FLAGS_To_Str (UINT64 flags)
{
  using namespace ir_a2b;
  return MapFlagsToStr<FlagToStr_t, PUSRCLANGFLAGS_ToStrTbl, 
		       PUSRCLANGFLAGS_ToStrTblSZ>("PUSRCLANGFLAGS_ToStrTbl", 
						  flags);
}

UINT64
Str_To_PU_SRC_LANG_FLAGS (const char* str)
{
  using namespace ir_a2b;
  return MapStrToFlags<FlagToStr_t, PUSRCLANGFLAGS_ToStrTbl, 
                       PUSRCLANGFLAGS_ToStrTblSZ>("PUSRCLANGFLAGS_ToStrTbl", 
						  str);
}


const char *
BLK_FLAGS_To_Str (UINT64 flags)
{
  using namespace ir_a2b;
  return MapFlagsToStr<FlagToStr_t, BLKFLAGS_ToStrTbl, 
		       BLKFLAGS_ToStrTblSZ>("BLKFLAGS_ToStrTbl", flags);
}

UINT64
Str_To_BLK_FLAGS (const char* str)
{
  using namespace ir_a2b;
  return MapStrToFlags<FlagToStr_t, BLKFLAGS_ToStrTbl, 
                       BLKFLAGS_ToStrTblSZ>("BLKFLAGS_ToStrTbl", str);
}


/* ====================================================================
 *
 * Print routines
 *
 * ====================================================================
 */

static void
Print_type_attributes (FILE *f, TY_IDX ty)
{
    if (TY_is_const (ty))
	fputs ("const ", f);
    if (TY_is_volatile (ty))
	fputs ("volatile ", f);
    if (TY_is_restrict (ty))
	fputs ("restrict ", f);
} // Print_type_attributes


static void
Print_TY_IDX_verbose (FILE *f, TY_IDX idx)
{
    Print_type_attributes (f, idx);
    if (TY_IDX_index (idx) == 0) {
	fputs ("<NULL>", f);
	return;
    }
	
    const TY& ty = Ty_Table[idx];
    const char *name = TY_name_idx (ty) == 0 ? "(anon)" : TY_name (ty);
    fprintf (f, "%s (#%d) align %d", name, TY_IDX_index (idx), TY_align (idx));
} // Print_TY_IDX_verbose


static const char *
TY_kind_name (const TY& ty)
{
    if (TY_kind (ty) == KIND_SCALAR && TY_mtype (ty) != MTYPE_UNKNOWN)
	return MTYPE_name (TY_mtype (ty));
    else
	return Kind_Name (TY_kind (ty));
}


void
ST::Print (FILE *f, BOOL verbose) const
{
    const char *name_str = (sym_class == CLASS_CONST) ?
	"<constant>" : &Str_Table[u1.name_idx];

    fprintf (f, "%-14s\t<%d,%d> ", name_str, ST_IDX_level (st_idx),
	     ST_IDX_index (st_idx));
    if (strlen (name_str) > 20)
	fputs ("\n\t\t", f);

    fprintf (f, "%s", Class_Name(sym_class));

    TY_IDX ty_idx = 0;

    switch (sym_class) {
    case CLASS_VAR:
    case CLASS_TYPE: 
	ty_idx = u2.type;
	break;

    case CLASS_FUNC:
	ty_idx = PU_prototype (Pu_Table[u2.pu]);
	break;

    case CLASS_CONST:
	ty_idx = u2.type;
	break;

    case CLASS_PREG:
	ty_idx = u2.type;
	break;

    case CLASS_BLOCK:
	fprintf (f, " (#%d)", u2.blk);
	break;
    }

    const TY& ty = Ty_Table [ty_idx];

    if (ty_idx != 0) {

	name_str = TY_name_idx (ty) == 0 ? NULL : TY_name (ty);

	if (!(sym_class == CLASS_FUNC) || name_str != NULL) {
	    fputs (" of type ", f);

	    Print_type_attributes (f, ty_idx);

	    fputs (name_str ? name_str : "(anon)", f);
	    
	    const TY *pty = &ty;
	    INT pcount = 0;
	    while (TY_kind (*pty) == KIND_POINTER) {
		pty = &Ty_Table[TY_pointed (*pty)];
		++pcount;
	    }
	    
	    if (verbose) {
		name_str = TY_kind_name (*pty);
		fprintf (f, " (#%d, %s", TY_IDX_index (ty_idx), name_str);
		while (pcount-- > 0)
		    fputc ('*', f);
		fputc (')', f);
	    } else
		fprintf (f, " (#%d)", TY_IDX_index (ty_idx));
	}
    }

    if (!verbose) {
	/* quick address */
	fprintf (f, " @ 0x%llx", offset);
	if (base_idx != 0)
	    fprintf (f, "(%s)", ST_name (base_idx));
    }
    fputc ('\n', f);

    if (sym_class == CLASS_FUNC && verbose) {
	/* Give info about the type being returned, which is different
	 * than the type of the function.
	 */
	if (ty_idx != 0 && TY_tylist (ty) != 0) {
	    TY_IDX rettype_idx = Tylist_Table[TY_tylist (ty)];
	    const TY& rettype = Ty_Table[rettype_idx];
	    fputs ("\t\tReturning ", f);
	    Print_type_attributes (f, rettype_idx);
	    fputs (TY_name (rettype), f);
	    name_str = TY_kind_name (rettype);
	    fprintf (f, " (#%d, %s)  ", TY_IDX_index (rettype_idx), name_str);

	    fprintf (f, "PU[%d] ", u2.pu);
	    const char* srclang = 
	      PU_SRC_LANG_FLAGS_To_Str(Pu_Table[u2.pu].src_lang);
	    fprintf (f, "%s  ", srclang);
	    
	    mUINT64 flags = Pu_Table[u2.pu].flags;
	    fprintf (f, "flags: %s", PU_FLAGS_To_Str(flags));
	    if (TY_return_to_param(ty_idx)) fprintf (f, " return_to_param");
	    if (TY_is_varargs(ty_idx))	    fprintf (f, " varargs");
	    if (TY_has_prototype(ty_idx))   fprintf (f, " prototype");
	    fprintf (f, "\n");
	}
    }
	
    if (sym_class == CLASS_CONST)
	fprintf (f, "\t\tvalue: %s\n", Targ_Print (NULL, Tcon_Table[u1.tcon]));

    if (verbose) {
	// Print address
	if (base_idx != 0) {
	    const ST& base_st = St_Table[base_idx];
	    fprintf (f, "\t\tAddress: %lld(%s<%d,%d>)  ", offset,
		     ST_class (base_st) == CLASS_CONST ? "" :
		     ST_name (base_idx), ST_IDX_level (base_idx),
		     ST_IDX_index (base_idx));
	}

	if (ty_idx != 0) {
	    if (base_idx == 0 && offset == 0)
		fputs ("\t\t", f);
	    fprintf (f, "Alignment: %d bytes", TY_align (ty_idx));
	}
	fprintf (f, "\n");

	fprintf (f, "\t\tFlags:\t0x%08x", flags);
	fprintf (f, " %s", ST_FLAGS_To_Str(flags));

	fprintf (f, ", %s", Export_Name(export_class));
	
	mUINT64 flags_ext = St_Table[st_idx].flags_ext;
	if (flags_ext) {
	   const char* flgstr = ST_EXT_FLAGS_To_Str(flags_ext);
           fprintf (f, "\n\t\tFlags_ext:\t0x%016llx", flags_ext);
	   fprintf (f, " %s", flgstr);
	}
	
	fprintf (f, "\n\t\tSclass: %s\n", Sclass_Name (storage_class));
    }
} // ST::Print


void
FLD::Print (FILE *f) const
{
    fprintf (f, "\t%6lld  %-8s\t", ofst, &Str_Table[name_idx]);
    Print_TY_IDX_verbose (f, type);
    fprintf (f, "\n\t\tfl:0x%04x", flags);
    if (flags) {
        const char* flgstr = FLD_FLAGS_To_Str(flags);
	fprintf (f, " %s", flgstr);
    }
    if (st != 0)
	fprintf (f, " st (%d,%d)", ST_IDX_level (st), ST_IDX_index (st));
    if (flags & FLD_IS_BIT_FIELD)
	fprintf (f, " bit field size:%d ofst:%d", bsize, bofst);
    fputc ('\n', f);
} // FLD::Print


void
ARB::Print (FILE *f) const
{
    if (flags & ARB_CONST_LBND)
	fprintf (f, "%lld:", Lbnd_val ());
    else
	fprintf (f, "st(%d):", ST_IDX_index (Lbnd_var ()));

    if (flags & ARB_CONST_UBND)
	fprintf (f, "%lld:", Ubnd_val ());
    else
	fprintf (f, "st(%d):", ST_IDX_index (Ubnd_var ()));

    if (flags & ARB_CONST_STRIDE)
	fprintf (f, "%lld:", Stride_val ());
    else
	fprintf (f, "st(%d):", ST_IDX_index (Stride_var ()));

} // ARB::Print


void
TY::Print (FILE *f) const
{
    fprintf (f, "%-14s:",
	     name_idx ? &Str_Table[name_idx] : "(anon)");

    fprintf (f, " (f: 0x%08x", flags);
    if (flags) {
        const char* flgstr = TY_FLAGS_To_Str(flags);
	fprintf (f, " %s", flgstr);
    }
    fprintf (f, ")");

    fprintf (f, " size %lld %s: ",
	     size, (mtype != 0) ? Mtype_Name (mtype) : ""); 

    switch (kind) {
    case KIND_SCALAR:
	fprintf (f, "SCALAR (%s)", Mtype_Name (mtype));
	fputc ('\n', f);
	break;

    case KIND_ARRAY:
	fputs ("ARRAY of ", f);
	Print_TY_IDX_verbose (f, Etype ());
	if (Arb () != 0) {
	   ARB_HANDLE arb(Arb());
	   INT i,ndim,co_dim,array_dim;
	   
	   ndim = ARB_dimension(arb);
           co_dim = ARB_co_dimension(arb);
	   array_dim = ndim-co_dim;

           fprintf(f,"\n\t\t dimension (%d) ,co_dimension(%d) ",array_dim,co_dim);

	   for (i = 0; i < ndim; i++) {
	      fputs (" (", f);
	      (*arb[i].Entry()).Print (f);
	      fputc (')', f);
	      // possible early exit for broken tables
	      if (ARB_last_dimen(arb[i])) break;
	   }
	}
	fputc ('\n', f);
	break;
	    
    case KIND_STRUCT:
	if (TY_is_packed (*this))
	    fputs ("packed ", f);
	fputs (kind == KIND_STRUCT ? "STRUCT\n": "CLASS\n", f);
	if (Fld () != 0) {
	    FLD_ITER iter = Make_fld_iter (FLD_HANDLE (Fld ()));
	    do {
		(*iter).Print (f);
	    } while (! FLD_last_field (iter++));
	} else
	    fputc ('\n', f);
	break;

    case KIND_POINTER:
	fputs ("-> ", f);
	Print_TY_IDX_verbose (f, Pointed ());
	fputc ('\n', f);
	break;

    case KIND_FUNCTION:
	fprintf (f, "FUNCTION (f: 0x%04x)\n", Pu_flags());
	{
	    TYLIST_IDX idx = Tylist ();
	    fprintf (f, "\treturns ");
	    Print_TY_IDX_verbose (f, Tylist_Table[idx]);
    	    if (Tylist_Table[idx] == 0) {
	    	fputc ('\n', f);
		break;
	    }
	    ++idx;
	    while (Tylist_Table[idx] != 0) {
		fputs ("\n\tparameter ", f);
		Print_TY_IDX_verbose (f, Tylist_Table[idx]);
		++idx;
	    }
	    fputc ('\n', f);
	}
	break;

    case KIND_VOID:
	fputs ("VOID", f);
	fputc ('\n', f);
	break;

    default:
	fprintf (f, "Unknown type (%d)", kind);
	fputc ('\n', f);
	break;
    }

    if (flags & TY_IS_SHARED)
	fprintf (f, "%s %d\n", "Block:", block_size);
} // TY::Print


void
PU::Print (FILE *f) const
{
    Print_TY_IDX_verbose (f, prototype);
    fprintf (f, ", flags 0x%016llx,\n"
	     "\tlexical level %d, LANG 0x%02x, TARGET_INFO %d\n",
	     flags, lexical_level, src_lang, target_idx); 
    if (flags & PU_DECL_VIEW) {
        fprintf(f,"Attention!!!: Declaration only (PU_DECL_VIEW)!");
        fprintf(f,"\n");
    }

} // PU::Print

void
BLK::Print (FILE *f) const
{
    fprintf (f, "size %lld, align %d, flags 0x%04x, section %d, scninfo %d\n",
		size, align, flags, section_idx, scninfo_idx);
} // BLK::Print


void
LABEL::Print (FILE *f) const
{
    const char *name_str = name_idx ? &Str_Table[name_idx] : "(anon)";
    fprintf (f, "%s: kind = 0x%08x fl = 0x%08x", name_str, kind, flags);

    const char* flgstr = LABEL_FLAGS_To_Str(flags);
    fprintf (f, " %s\n", flgstr);
} // LABEL::Print


void
PREG::Print (FILE *f) const
{
    const char *name_str = name_idx ? &Str_Table[name_idx] : "(anon)";

    fprintf (f, "%s\n", name_str);
} // PREG::Print


void
ST_ATTR::Print (FILE* f) const
{
    fprintf (f, "0x%x (%s) --> ", st_idx, ST_name (st_idx));
    switch (kind) {
    case ST_ATTR_UNKNOWN:
	fprintf (f, "(NOT USED)\n");
	break;
    case ST_ATTR_DEDICATED_REGISTER:
	fprintf (f, "(DREG) %d\n", u.reg_id);
	break;
    case ST_ATTR_SECTION_NAME:
	fprintf (f, "(SECTION) %s\n", Index_To_Str (u.section_name));
	break;
    default:
	fprintf (f, "(UNKNOWN) 0x%x\n", u.value);
	break;
    }
}

void
FILE_INFO::Print (FILE *f) const
{
    fprintf (f, "gp_group: %d, flags: 0x%08x", gp_group, flags);
    if (flags) {
      const char* flgstr = FILE_INFO_FLAGS_To_Str(flags);
      fprintf (f, " %s", flgstr);
    }

    fputs ("\n", f);

} // FILE_INFO::Print
    


// function object used in "For_all"
template <class T>
struct print_op
{
    FILE *fid;

    print_op (FILE *f) : fid (f) {}

    void operator () (UINT idx, T *entry) const;
}; // print_op


template <class T>
inline void
print_op<T>::operator () (UINT idx, T *entry) const {
	fprintf (fid, "[%d]: ", idx);
	entry->Print (fid);
}


// specialization for printing TCONs
template <>
inline void
print_op<TCON>::operator () (UINT idx, TCON *c) const {
    fprintf (fid, "[%d] %s: %s\n", idx, MTYPE_name(TCON_ty(*c)),Targ_Print (NULL, *c));
} // print_op<TCON>::operator ()


// Print all local symbol tables corresponding to a PU
void
Print_local_symtab (FILE *f, const SCOPE& scope)
{
    // Print the function name

    fprintf (f, "\n%sSYMTAB for %s: level %d, st %d, label %d, preg %d,"
 	     " inito %d, st_attr %d\n%s\n", DBar, ST_name (scope.st),
	     PU_lexical_level (scope.st), scope.st_tab->Size () - 1,
	     scope.label_tab->Size () - 1, scope.preg_tab->Size () - 1,
	     scope.inito_tab->Size () - 1, scope.st_attr_tab->Size () - 1, DBar);

    fputs ("Symbols:\n", f);
    For_all_entries (*scope.st_tab, print_op<ST> (f), 1);

    fprintf (f, "%sLabels:\n", DBar);
    For_all_entries (*scope.label_tab, print_op<LABEL> (f), 1);
    
    fprintf (f, "%sPseudo-registers:\n", DBar);
    For_all_entries (*scope.preg_tab, print_op<PREG> (f), 1);
    
    fprintf (f, "%sINITOs:\n", DBar);
    For_all_entries (*scope.inito_tab, print_op<INITO> (f), 1);

    fprintf (f, "%sST_ATTRs:\n", DBar);
    For_all_entries (*scope.st_attr_tab, print_op<ST_ATTR> (f), 1);

    fprintf (f, "%s\n", DBar);
    
} // Print_local_symtab


void
Print_global_symtab (FILE *f)
{
    fprintf(f,"%sGLOBAL SYMTAB:\n", DBar);

    fprintf (f, "%sFile Info:\n", DBar);
    File_info.Print (f);

    fprintf (f, "%sSymbols:\n", DBar);
    For_all (St_Table, GLOBAL_SYMTAB, print_op<ST> (f));

    fprintf (f, "%sPUs:\n", DBar);
    For_all (Pu_Table, print_op<PU> (f));

    fprintf (f, "%sTypes:\n", DBar);
    For_all (Ty_Table, print_op<TY> (f));

    fprintf (f, "%sBlocks:\n", DBar);
    For_all (Blk_Table, print_op<BLK> (f));

    fprintf (f, "%sTcons:\n", DBar);
    For_all (Tcon_Table, print_op<TCON> (f));

    fprintf (f, "%sINITOs:\n", DBar);
    For_all (Inito_Table, GLOBAL_SYMTAB, print_op<INITO> (f));

    fprintf (f, "%sST_ATTRs:\n", DBar);
    For_all (St_Attr_Table, GLOBAL_SYMTAB, print_op<ST_ATTR> (f));

    fprintf (f, "%sString table size = %d\n", DBar, STR_Table_Size());
    fprintf (f, "%s\n", DBar);
} // Print_global_symtab

// for ease of debugging, because I don't know how to call Print
// routines from dbx, add simple dump routines.
void
dump_st (ST *st)
{
	st->Print(stdout);
    	switch (st->sym_class) {
	case CLASS_BLOCK:
		Blk_Table[st->u2.blk].Print (stdout);
		break;
	case CLASS_FUNC:
		Pu_Table[st->u2.pu].Print (stdout);
		break;
	}
}

void
dump_st(ST_IDX st_idx)
{
   ST *st = &St_Table[st_idx];
   dump_st(st);
}

// Since dbx cannot handle overloaded functions...
void 
dump_st_idx (ST_IDX st) 
{
  dump_st(st);
}


void
dump_ty (TY_IDX ty_idx)
{
	TY& ty = Ty_Table[ty_idx];
	ty.Print(stdout);
}

// Since dbx cannot handle overloaded function...
void
dump_ty_idx (TY_IDX ty_idx)
{
  dump_ty (ty_idx);
}

void
dump_ty(const TY &ty)
{
  ty.Print(stdout);
}

void 
dump_label (LABEL_IDX idx)
{
   Label_Table[idx].Print(stdout);
}

void 
dump_inito(INITO_IDX idx)
{
   Inito_Table[idx].Print(stdout);
}

void
dump_st_attr (ST_ATTR_IDX idx)
{
    St_Attr_Table[idx].Print(stdout);
}

static ST *
Gen_Temp_Named_Symbol (TY_IDX ty, const char *rootname,
		       ST_CLASS sym_class, ST_SCLASS storage_class)
{
  static INT Temp_Index = 0;
  ST *st = New_ST(CURRENT_SYMTAB);
  STR_IDX str_idx = Save_Str2i(rootname, "_temp_", Temp_Index++);
  ST_Init(st, str_idx, sym_class, storage_class, EXPORT_LOCAL, ty);
  return st;
}

// Create stack symbol for temp var.
ST *
Gen_Temp_Symbol (TY_IDX      ty,	// type of the desired symbol
		 const char *rootname)	// root of the name to use
{
  ST *st = Gen_Temp_Named_Symbol(ty, rootname, CLASS_VAR, SCLASS_AUTO);
  Set_ST_is_temp_var(st);
  return st;
}

// Create a read-only file-level static symbol of the given type in
// the current symbol table.
ST *
Gen_Read_Only_Symbol(TY_IDX ty, const char *rootname)
{
  ST *st = Gen_Temp_Named_Symbol(ty, rootname, CLASS_VAR, SCLASS_FSTATIC);
  Set_ST_is_const_var(st);
  return st;
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------

TY_IDX
Promoted_Parm_Type(const ST *formal_parm)
{
  TY_IDX ptype = ST_type(formal_parm);
  TY_IDX newtype;
  BOOL parm_float;

  if ( ! ST_promote_parm(formal_parm) ) return ptype;

  /* Otherwise, figure out the conversion: */
  ptype = ST_type(formal_parm);
  parm_float = ST_is_value_parm(formal_parm) &&
		(TY_kind(ptype) == KIND_SCALAR) &&
		(MTYPE_float(TY_mtype(ptype)));
  if ( parm_float ) {
    /* Must be a promotion from float to double: */
    newtype = MTYPE_To_TY(MTYPE_F8);
  } else {
    /* Must be a promotion to int: */
    newtype = MTYPE_To_TY(Integer_type);
  }

  /* For sanity, assert that we don't demote: */
  Is_True(TY_size(newtype) >= TY_size(ptype),
	  ("Nonsensical demotion of parameter type"));
  return newtype;
}

//----------------------------------------------------------------------
// Initialization routines
//----------------------------------------------------------------------

// for fast conversion of predefined types and preg.
ST *MTYPE_TO_PREG_array[MTYPE_LAST+1];

ST *Int_Preg, *Float_Preg, *Return_Val_Preg;

TY_IDX MTYPE_TO_TY_array[MTYPE_LAST+1];

TY_IDX Quad_Type, Void_Type, FE_int_Type, FE_double_Type;
TY_IDX Spill_Int_Type, Spill_Float_Type;

#if defined(FRONT_END) && !defined(FRONT_END_MFEF77)
extern "C" TYPE_ID FE_int_To_Mtype (void);
#endif

static INT Max_scope = 8;

// Set reserve_index_zero to FALSE when reading the symtab from a file
void
New_Scope (SYMTAB_IDX level, MEM_POOL *pool, BOOL reserve_index_zero)
{
    if (level >= Max_scope) {
	UINT size = Max_scope * sizeof(SCOPE);
	Max_scope *= 2;
	Scope_tab = (SCOPE *) MEM_POOL_Realloc (Malloc_Mem_Pool, Scope_tab,
						size, size * 2);
    }

    Current_scope = level;

    ST_TAB *st_tab = CXX_NEW (ST_TAB (pool), pool);
    INITO_TAB *inito_tab = CXX_NEW (INITO_TAB (pool), pool);
    ST_ATTR_TAB *st_attr_tab = CXX_NEW (ST_ATTR_TAB (pool), pool);
    LABEL_TAB *label_tab = NULL;
    PREG_TAB *preg_tab = NULL;

    UINT32 dummy_idx;
    if (reserve_index_zero) {
	st_tab->New_entry ((ST_IDX&) dummy_idx);
	inito_tab->New_entry ((INITO_IDX&) dummy_idx);
	st_attr_tab->New_entry ((ST_ATTR_IDX&) dummy_idx);
    }
    
    if (level > GLOBAL_SYMTAB) {
	label_tab = CXX_NEW (LABEL_TAB (pool), pool);
	preg_tab = CXX_NEW (PREG_TAB (pool), pool);

	if (reserve_index_zero) {
	    label_tab->New_entry ((LABEL_IDX&) dummy_idx);
	    preg_tab->New_entry ((PREG_IDX&) dummy_idx);
	}
    
    }
	

    // Allocate tables for STs, LABELS, PREGs and INITOs
    Scope_tab[level].Init (st_tab, label_tab, preg_tab, inito_tab,
			   st_attr_tab, pool);

} // New_Scope


void
Delete_Scope (SYMTAB_IDX level)
{
    SCOPE& scope = Scope_tab[level];

    CXX_DELETE (scope.st_tab, scope.pool);
    CXX_DELETE (scope.label_tab, scope.pool);
    CXX_DELETE (scope.preg_tab, scope.pool);
    CXX_DELETE (scope.inito_tab, scope.pool);
    CXX_DELETE (scope.st_attr_tab, scope.pool);
    
} // Delete_Scope


static void
Setup_Preg_Pointers ()
{
    for (TYPE_ID i = MTYPE_FIRST; i <= MTYPE_LAST; ++i) {
	if (MTYPE_To_PREG (i) != NULL || MTYPE_To_TY (i) == 0)
	    continue;
	Is_True (MTYPE_byte_size (i) < 4, ("Invalid mtype"));
	MTYPE_To_PREG (i) = MTYPE_signed (i) ? MTYPE_To_PREG (MTYPE_I4) :
	    MTYPE_To_PREG (MTYPE_U4);
    }
    
    if (ST_type(Int64_Preg) == Spill_Int_Type)
	Int_Preg = Int64_Preg;
    else
	Int_Preg = Int32_Preg;
    if (ST_type(Float32_Preg) == Spill_Float_Type)
	Float_Preg = Float32_Preg;
    else
	Float_Preg = Float64_Preg;
} // Setup_Preg_Pointers


static void
Create_All_Preg_Symbols ()
{
    for (TYPE_ID i = MTYPE_FIRST; i <= MTYPE_LAST; ++i) {
	if (MTYPE_To_TY (i) == 0)
	    continue;
	if (MTYPE_To_PREG (i) != NULL)
	    continue;
	if (MTYPE_byte_size(i) < 4) {
	    // special case:  allow mtype_B
	    if (i != MTYPE_B) continue;
	}

	ST *st = New_ST (GLOBAL_SYMTAB);
	ST_Init (st, Save_Str2 (".preg_", MTYPE_name(i)),
		 CLASS_PREG, SCLASS_REG, EXPORT_LOCAL, MTYPE_To_TY (i));
	MTYPE_To_PREG(i) = st;
    }

    if (Return_Val_Preg == NULL) {

	ST *st = New_ST (GLOBAL_SYMTAB);
	ST_Init (st, Save_Str (".preg_return_val"),
		 CLASS_PREG, SCLASS_REG, EXPORT_LOCAL, 0);
	Return_Val_Preg = st;
    }

    Setup_Preg_Pointers ();
} // Create_All_Preg_Symbols


// for phases other then the front end, we read in the global symtab from
// file, so we don't create the preg sybmols.  But we still need to intialize
// the MTYPE_TO_PREG_array.
static void
Set_up_all_preg_symbols ()
{
    UINT32 idx = 1;
    
    UINT32 last = MIN (MTYPE_LAST + 1, ST_Table_Size (GLOBAL_SYMTAB));

    for (INT i = MTYPE_FIRST; i < last; ++i) {
	ST_IDX st_idx = make_ST_IDX (idx++, GLOBAL_SYMTAB);
	ST& st = St_Table[st_idx];
	if (ST_sym_class (st) != CLASS_PREG)
	    break;			// ASSUME all predefined pregs are
					// placed before all other symbols

	if (Return_Val_Preg == NULL) {
	    if (strcmp (ST_name (&st), ".preg_return_val") == 0) {
		Return_Val_Preg = &st;
		continue;
	    }
	}

	TY_IDX ty_idx = ST_type (st);
	const TY& ty = Ty_Table[ty_idx];
	TYPE_ID mtype = TY_mtype (ty);
	if (ty_idx != MTYPE_To_TY (mtype))
	    continue;
	MTYPE_To_PREG (mtype) = &st;
    }

    // create any missing preg
    Create_All_Preg_Symbols ();

} // Set_up_all_preg_symbols
    
/* ====================================================================
 *
 * Gen_Predef_Type_Name
 *
 * Just allocate and create a predefined type name
 *
 * ====================================================================
 */
static inline STR_IDX
Gen_predef_type_name (const char *basename)
{
    return Save_Str2 (".predef_", basename);
}


/* ====================================================================
 *
 * Create_Special_Global_Symbols
 *
 * Create special global symbols and types.  These are created before
 * the per-file initialization, because they are used for various
 * global purposes like initializing the segment descriptors.
 *
 * This should only be called once per execution.
 *
 * ====================================================================
 */

static void
Create_Special_Global_Symbols ()
{
    TY_IDX ty_idx;
    TY_KIND ty_kind = KIND_SCALAR;
    
    /* Make predefined types for the machine types: */
    for (TYPE_ID i = MTYPE_FIRST; i <= MTYPE_LAST; ++i) {

	if (MTYPE_To_TY (i) != 0)
	    continue;

	TY &ty = New_TY (ty_idx);

	ty_kind = KIND_SCALAR;
        if (MTYPE_align_req (i))
	    Set_TY_align (ty_idx, MTYPE_align_req(i));

	/* If this is one of the basic spill types, remember it: */
	if ( i == Spill_Int_Mtype )	{
	    Spill_Int_Type = ty_idx;
	} else if ( i == Spill_Float_Mtype ) {
	    Spill_Float_Type = ty_idx;
	}
	if ( i == MTYPE_FQ )
	    Quad_Type = ty_idx;

#if defined(FRONT_END_C) || defined(FRONT_END_CPLUSPLUS)
#ifndef FRONT_END_MFEF77
	if ( i == FE_int_To_Mtype() )
	    FE_int_Type = ty_idx;
#endif /* FRONT_END_MFEF77 */
	if ( i == MTYPE_F8 )
	    FE_double_Type = ty_idx;
#endif /* FRONT_END_C || FRONT_END_CPLUSPLUS */
	if ( i == MTYPE_V ) {
	    Void_Type = ty_idx;
	    ty_kind = KIND_VOID;
	}

	if (i == MTYPE_A4 || i == MTYPE_A8) {
	    if (MTYPE_byte_size (i) != Pointer_Size) {
		Ty_tab.Delete_last ();
		MTYPE_To_TY (i) = 0;
		continue;
	    }

	    ty_kind = KIND_POINTER;
	    TY_Init (ty, MTYPE_byte_size (i), ty_kind, i,
		     Gen_predef_type_name (MTYPE_name (i)));
	    Is_True (i > MTYPE_V,
		     ("Create_Special_Global_Symbols() assumes MTYPE_V < "
		      "MTYPE_A4 and MTYPE_A8"));
	    Set_TY_pointed (ty, MTYPE_To_TY (MTYPE_V));
	    MTYPE_To_TY (i) = ty_idx;
	    continue;
	}

	TY_Init (ty, MTYPE_byte_size (i), ty_kind, i,
		 Gen_predef_type_name (MTYPE_name (i)));
	MTYPE_To_TY (i) = ty_idx;
    }

} // Create_Special_Global_Symbols


static inline BOOL
Predefined_Void_Type (TY_IDX ty_idx)
{
    const TY& ty = Ty_Table[ty_idx];
    return (ty_idx == make_TY_IDX (TY_IDX_index (ty_idx)) &&
	    TY_mtype (ty) == MTYPE_V && TY_kind (ty) == KIND_VOID);
    
} // Predefined_Void_Type


// In phases other then the frontends, we read in the global symtab from the
// file, and that includes all the predefined global symbols and types.  We
// don't need to create them again, but we still need to initialize the
// MTYPE_TO_TY array.
void
Initialize_Special_Global_Symbols ()
{
    // ASSUME all mtype TYs, if defined, are placed before any other TY's

    UINT32 last = MIN (MTYPE_LAST + 1, TY_Table_Size());
    // for trivial program, the type table may have less entry than MTYPE_LAST

    for (UINT32 i = MTYPE_FIRST; i < last; ++i) {
	TY_IDX ty_idx = make_TY_IDX (i);
	const TY& ty = Ty_Table[ty_idx];

	TYPE_ID mtype = TY_mtype (ty);
	if (MTYPE_byte_size (mtype) != TY_size (ty) ||
	    TY_flags (ty) != 0)
	    continue;
	switch (mtype) {
	case MTYPE_V:
	    if (TY_kind (ty) != KIND_VOID)
		continue;
	    break;

	case MTYPE_A4:
	case MTYPE_A8:
	    if (TY_kind (ty) != KIND_POINTER || 
		! Predefined_Void_Type (TY_pointed (ty)))
		continue;
	    break;

	default:
	    if (TY_kind (ty) != KIND_SCALAR)
		continue;
	    break;
	}
	if (MTYPE_align_req (mtype))
	    Set_TY_align (ty_idx, MTYPE_align_req (mtype));
	MTYPE_To_TY (mtype) = ty_idx;
    }
	
    // create any missing predefined types
    Create_Special_Global_Symbols ();
	    
    Spill_Int_Type = MTYPE_To_TY (Spill_Int_Mtype);
    Spill_Float_Type = MTYPE_To_TY (Spill_Float_Mtype);
    Quad_Type = MTYPE_To_TY (MTYPE_FQ);
    Void_Type = MTYPE_To_TY (MTYPE_V);

    Set_up_all_preg_symbols ();

} // Initialize_Special_Global_Symbols


// Initialize all symbol tables.  This function must be called before any
// symbol table operation is performed.
// If reading the symtab from a file, then reserve_index_zero should be FALSE
void
Initialize_Symbol_Tables (BOOL reserve_index_zero)
{
    if (Scope_tab != NULL)
	return;
    
    Scope_tab = (SCOPE *) MEM_POOL_Alloc (Malloc_Mem_Pool,
					  Max_scope * sizeof(SCOPE));
    memset(Scope_tab, '\0', Max_scope * sizeof(SCOPE));

    memset (MTYPE_TO_PREG_array, '\0', sizeof(ST*) * (MTYPE_LAST + 1));
    memset (MTYPE_TO_TY_array, '\0', sizeof(TY_IDX) * (MTYPE_LAST + 1));

    if (reserve_index_zero) {
	// For producer, we reserve first entry for all global tables

	Initialize_Strtab (0x1000);	// start with 4Kbytes for strtab.

	UINT dummy_idx;
	memset (&New_PU ((PU_IDX&) dummy_idx), '\0', sizeof(PU));
	memset (&New_TY ((TY_IDX&) dummy_idx), '\0', sizeof(TY));
	memset (New_FLD ().Entry(), '\0', sizeof(FLD));
	memset (&New_TYLIST ((TYLIST_IDX&) dummy_idx), '\0', sizeof(TYLIST));
	memset (New_ARB ().Entry(), '\0', sizeof(ARB));
	memset (&New_BLK ((BLK_IDX&) dummy_idx), '\0', sizeof(BLK));
	memset (&Initv_Table.New_entry ((INITV_IDX&) dummy_idx), '\0', 
		sizeof(INITV));
	Init_Constab ();
	New_Scope (GLOBAL_SYMTAB, Malloc_Mem_Pool, TRUE);
	
	Create_Special_Global_Symbols ();
	Create_All_Preg_Symbols();
    }
#ifdef BACK_END
    if (!Read_Global_Data) {
       // reserve zero index in BLK table
       UINT blk_idx;
       memset (&New_BLK ((BLK_IDX&) blk_idx), '\0', sizeof(BLK));
    }
#endif
}

//----------------------------------------------------------------------
// TCON
//----------------------------------------------------------------------
TCON_IDX
Enter_tcon (const TCON& tcon)
{
    switch (TCON_ty (tcon)) {
    case MTYPE_F4:
	if (TCON_ival (tcon) == 0)
	    return 1;
    case MTYPE_F8:
        if (TCON_k0 (tcon) == 0)
            return 2;
    default:
	return Tcon_Table.Insert (tcon);
    }
} // Enter_tcon


void
Init_Constab ()
{
    if (Tcon_Table.Size () == 0) {
	TCON Zero;
	UINT32 idx;
        memset (&Zero, '\0', sizeof(TCON));
        idx = Tcon_Table.Insert (Zero);	// index 0: dummy
	Set_TCON_ty (Zero, MTYPE_F4); 
        idx = Tcon_Table.Insert (Zero);	// index 1: float (0.0)
	Set_TCON_ty (Zero, MTYPE_F8); 
        idx = Tcon_Table.Insert (Zero);	// index 2: double (0.0)

	Is_True (idx == MAX_PREDEFINED_TCON_IDX,
		 ("Number of predefined tcons incorrect"));

	Initialize_TCON_strtab (1024);	// string table for TCONs
    }
}


#ifdef Is_True_On

//
// Additional debugging functions because Workshop can't deal with 
// the [] operator correctly.
//

ST ST_from_IDX(ST_IDX x) {return St_Table[x];}
TY TY_from_IDX(TY_IDX x) {return Ty_Table[x];}
INITO INITO_from_IDX(INITO_IDX x) {return Inito_Table[x];}
INITV INITV_from_IDX(INITV_IDX x) {return Initv_Table[x];}
PU PU_from_IDX(PU_IDX x) {return Pu_Table[x];}
ARB ARB_from_IDX(ARB_IDX x) {return Arb_Table[x];}
TCON TCON_from_IDX(TCON_IDX x) {return Tcon_Table[x];}
FLD FLD_from_IDX(FLD_IDX x) {return Fld_Table[x];}
LABEL LABEL_from_IDX(LABEL_IDX x) {return Label_Table[x];}
TYLIST TYLIST_from_IDX(TYLIST_IDX x) {return Tylist_Table[x];}

#endif // Is_True_On
