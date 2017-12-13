// ##########################################################
// # This file is part of OpenADFortTk.                     #
// # The full COPYRIGHT notice can be found in the top      #
// # level directory of the OpenADFortTk source tree.       #
// # For more information visit                             #
// # http://www.mcs.anl.gov/openad                          #
// ##########################################################

#include <sstream> 

#include "Open64IRInterface/Open64BasicTypes.h"

#include "wn2xaif.h"
#include "wn2xaif_mem.h"
#include "st2xaif.h"
#include "ty2xaif.h"

namespace whirl2xaif { 

  extern WN* PU_Body;
  extern BOOL Array_Bnd_Temp_Var;

  /* TY2F_Handler[] maps a TY_kind to a function that translates
   * a type of the given kind into Fortran.  Should the ordinal
   * numbering of the KIND change in "../common/com/stab.h", then
   * a corresponding change must be made here.
   */

  typedef void (*TY2F_HANDLER_FUNC)(xml::ostream&, TY_IDX, PUXlationContext& ctxt);

  static void 
  TY2F_invalid(xml::ostream& xos, TY_IDX ty, PUXlationContext& ctxt);
  static void 
  TY2F_scalar(xml::ostream& xos, TY_IDX ty, PUXlationContext& ctxt);
  static void 
  TY2F_array(xml::ostream& xos, TY_IDX ty, PUXlationContext& ctxt);
  static void 
  TY2F_array_for_pointer(xml::ostream& xos, TY_IDX ty, PUXlationContext& ctxt);
  static void 
  TY2F_struct(xml::ostream& xos, TY_IDX ty, PUXlationContext& ctxt);
  static void 
  TY2F_2_struct(xml::ostream& xos,TY_IDX ty, PUXlationContext& ctxt);
  static void 
  TY2F_pointer(xml::ostream& xos, TY_IDX ty, PUXlationContext& ctxt);
  static void 
  TY2F_void(xml::ostream& xos, TY_IDX ty, PUXlationContext& ctxt);

  // ***************************************************************************

  static const TY2F_HANDLER_FUNC TY2F_Handler[KIND_LAST/*TY_KIND*/] = {
    &TY2F_invalid,   /* KIND_INVALID */
    &TY2F_scalar,    /* KIND_SCALAR */
    &TY2F_array,     /* KIND_ARRAY */
    &TY2F_struct,    /* KIND_STRUCT */
    &TY2F_pointer,   /* KIND_POINTER */
    &TY2F_invalid,   /* KIND_FUNCTION */
    &TY2F_void,      /* KIND_VOID */
  }; /* TY2F_Handler */

  /* detect parts of f90 dope vectors which should be output. Most are
     I4 boundaries except the bofst >16 - just for num_dims */
#define NOT_BITFIELD_OR_IS_FIRST_OF_BITFIELD(f)				\
  (!FLD_is_bit_field(f) || (FLD_is_bit_field(f) && (FLD_bofst(f) == 0) || FLD_bofst(f) > 16))

  // ***************************************************************************

  void
  TY2F_translate(xml::ostream& xos, TY_IDX ty, BOOL notyapp, PUXlationContext& ctxt)
  {
    // Dispatch the translation-task to the appropriate handler function.
    if (!notyapp)
      TY2F_Handler[TY_kind(Ty_Table[ty])](xos, ty, ctxt);
    else
      TY2F_2_struct(xos, ty, ctxt);
  }

  void 
  TY2F_translate(xml::ostream& xos, TY_IDX ty, PUXlationContext& ctxt)
  {
    TY2F_translate(xos, ty, 0, ctxt);
  }


  /*---------------------- A few utility routines -----------------------*/
  /*---------------------------------------------------------------------*/

  // static void
  // WN2F_tempvar_rhs(xml::ostream& xos, WN * wn)
  // {
  //   /* The rhs */
  //   PUXlationContext ctxt;
  //   whirl2xaif::TranslateWN(xos, WN_kid0(wn), ctxt);
  // }

  // static void
  // GetTmpVarTransInfo(xml::ostream& xos, ST_IDX arbnd, WN* wn)
  // {
  //   WN * stmt;
  //   stmt = WN_first(wn);
  //   while ((stmt !=NULL)
  // 	 && ((WN_operator(stmt)!=OPR_STID) || (WN_operator(stmt) ==OPR_STID)
  // 	     && strcmp(ST_name(WN_st(stmt)), ST_name(ST_ptr(arbnd)))))
  //     stmt = WN_next(stmt);
  //   if (stmt != NULL)
  //     WN2F_tempvar_rhs(xos, stmt);
  // }

  static std::string
  TY2F_Append_Array_Bnd_Ph(ST_IDX arbnd)
  {
    // FIXME: 
    std::ostringstream xos_abdstr;
    xml::ostream xos_abd(xos_abdstr.rdbuf());

#if 0 // FIXME
    WN* wn = PU_Body; // FIXME--Yuck!!!
    GetTmpVarTransInfo(xos_abd, arbnd, wn);
#endif
  
    return xos_abdstr.str();
  } 

  static void
  TY2F_Append_ARB(xml::ostream& xos, ARB_HANDLE arb, TY_IDX ty_idx, 
		  PUXlationContext& ctxt)
  {  
    if (TY_is_f90_deferred_shape(ty_idx)) {

      xos << xml::BegElem("xaif:Property") << xml::Attr("id", ctxt.currentXlationContext().getNewVertexId()) 
	  << xml::Attr("name", "shape") << xml::Attr("value", ':') << xml::EndElem;
    
    } else {

      std::string lb, ub;
      if (ARB_const_lbnd(arb)) {
	lb = TCON2F_translate(Host_To_Targ(MTYPE_I4, ARB_lbnd_val(arb)), 
			      FALSE /*is_logical*/);
      } else if (ARB_lbnd_var(arb) != 0) {
	lb = TY2F_Append_Array_Bnd_Ph(ARB_lbnd_var(arb));
      }
    
      if (ARB_const_ubnd(arb)) {
	ub = TCON2F_translate(Host_To_Targ(MTYPE_I4, ARB_ubnd_val(arb)),
			      FALSE /*is_logical*/); 
      } else if (ARB_ubnd_var(arb) != 0) {
	ub = TY2F_Append_Array_Bnd_Ph(ARB_ubnd_var(arb));
      }

      xos << xml::BegElem("xaif:Property") << xml::Attr("id", ctxt.currentXlationContext().getNewVertexId()) 
	  << xml::Attr("name", "lb") << xml::Attr("value", lb) << xml::EndElem;
      xos << xml::BegElem("xaif:Property") << xml::Attr("id", ctxt.currentXlationContext().getNewVertexId()) 
	  << xml::Attr("name", "ub") << xml::Attr("value", ub) << xml::EndElem;

    }
  } 

  static BOOL
  TY2F_is_character(TY_IDX ty)
  {
    while (TY_kind(ty) == KIND_ARRAY)
      ty = TY_etype(ty);

    return TY_is_character(ty);
  }
  /*------ Utilities for accessing and declaring KIND_STRUCT FLDs ------
   *---------------------------------------------------------------------*/

#define FLD_INFO_ALLOC_CHUNK 16
  static FLD_PATH_INFO *Free_Fld_Path_Info = NULL;


  static BOOL
  TY2F_Pointer_To_Dope(TY_IDX ty)
  {
    /* Is this a pointer to a dope vector base */
    return (strcmp(TY_name(TY_pointed(ty)),".base.") == 0) ;
  }

  static FLD_PATH_INFO *
  New_Fld_Path_Info(FLD_HANDLE fld)
  {
    /* Allocates a new FLD_PATH_INFO, reusing any that have earlier
     * been freed up.  Dynamic allocation occurs in chunks of 16
     * (FLD_INFO_ALLOC_CHUNK) FLD_PATH_INFOs at a time.
     */
    FLD_PATH_INFO *fld_info;
   
    if (Free_Fld_Path_Info != NULL)
      {
	fld_info = Free_Fld_Path_Info;
	Free_Fld_Path_Info = fld_info->next;
      }
    else
      {
	INT info_idx;
      
	/* Allocate a new chunk of path infos, and put all except the
	 * first one on the free-list.
	 */
	fld_info = TYPE_MEM_POOL_ALLOC_N(FLD_PATH_INFO, Malloc_Mem_Pool, 
					 FLD_INFO_ALLOC_CHUNK);
	fld_info[FLD_INFO_ALLOC_CHUNK-1].next = Free_Fld_Path_Info;
	for (info_idx = FLD_INFO_ALLOC_CHUNK-2; info_idx > 0; info_idx--)
	  fld_info[info_idx].next = &fld_info[info_idx+1];
	Free_Fld_Path_Info = &fld_info[1];
      }

    fld_info->next = NULL;
    fld_info->arr_elt = FALSE;
    fld_info->arr_ofst = 0;
    fld_info->arr_wn = NULL;
    fld_info->fld = fld;
    return fld_info;
  } /* New_Fld_Path_Info */

  static STAB_OFFSET
  TY2F_Fld_Size(FLD_HANDLE this_fld, mUINT64  max_size)
  {
    /* Returns the size of the field, taking into account the offset
     * to the next (non-equivalence) field and the maximum field-size
     * (based on the structure size).
     */
  
    mUINT64 fld_size = TY_size(FLD_type(this_fld));

    /* Restrict the fld_size to the max_size */
    if (fld_size > max_size)
      fld_size = max_size;
  
    /* If this_fld is an equivalence field, then just return the current
     * fld_size (cannot be any different), otherwise search for a non-
     * equivalent next_fld at a higher offset.
     * TODO: mfef90 & mfef77 set the flag slightly differently in COMMON.
     * this really works only for mfef77.
     */

    if (!FLD_equivalence(this_fld))
      {
	FLD_ITER fld_iter = Make_fld_iter(this_fld);

	if (!FLD_last_field (fld_iter)) 
	  {
	    ++fld_iter;
	    BOOL found = FALSE;
	    mUINT64 noffset = 0; 

	    do
	      {
		FLD_HANDLE next_fld (fld_iter);

		if (!FLD_is_bit_field(next_fld)) 
		  if (!(FLD_equivalence(next_fld) || FLD_ofst(this_fld) >= FLD_ofst(next_fld)))
		    {
		      found  = TRUE;
		      noffset =  FLD_ofst(next_fld) ;
		      break ;
		    }
	      } while (!FLD_last_field (fld_iter ++ )) ;

	    if (found) 
	      if (fld_size > noffset - FLD_ofst(this_fld))
		fld_size = noffset - FLD_ofst(this_fld) ;
	  }
      }
    return fld_size;
  } /* TY2F_Fld_Size */


  static FLD_PATH_INFO *
  Select_Best_Fld_Path(FLD_PATH_INFO *path1,
		       FLD_PATH_INFO *path2,
		       TY_IDX         desired_ty,
		       mUINT64        desired_offset)
  {
    /* PRECONDITION: Both paths must be non-NULL and lead to a field
     *    at the desired_offset.
     *
     * Try to find the best of two paths to a field.  This routine
     * will be called for EVERY field at every place where a struct,
     * union, or equivalence field is accessed, so efficiency is of
     * uttmost importance.  The best path is returned, while the other
     * on is freed up.
     */
    FORTTK_ASSERT(path1 && path2, fortTkSupport::Diagnostics::UnexpectedInput);
   
    FLD_PATH_INFO *best_path;
    mUINT64        offs1, offs2;
    FLD_PATH_INFO *p1, *p2;
    TY_IDX         t1,  t2;
   
    /* Find the last field on each path */
    offs1 = FLD_ofst(path1->fld) + path1->arr_ofst;
    for (p1 = path1; p1->next != NULL; p1 = p1->next)
      offs1 += FLD_ofst(p1->next->fld) + p1->next->arr_ofst;
    offs2 = FLD_ofst(path2->fld) + path2->arr_ofst;
    for (p2 = path2; p2->next != NULL; p2 = p2->next)
      offs2 += FLD_ofst(p2->next->fld) + p2->next->arr_ofst;

    FORTTK_ASSERT(offs1 == desired_offset && offs2 == desired_offset,
		  "Unexpected offset");

    /* Get the element type (either the field type or the type of an
     * array element.
     */
    if (p1->arr_elt)
      t1 = TY_AR_etype(FLD_type(p1->fld));
    else
      t1 = FLD_type(p1->fld);
    if (p2->arr_elt)
      t2 = TY_AR_etype(FLD_type(p2->fld));
    else
      t2 = FLD_type(p2->fld);

    /* Compare types, in order of increasing accuracy */
    if (TY_mtype(t1) == TY_mtype(desired_ty) &&
	TY_mtype(t2) != TY_mtype(desired_ty))
      best_path = path1;
    else if (TY_mtype(t2) == TY_mtype(desired_ty) &&
	     TY_mtype(t1) != TY_mtype(desired_ty))
      best_path = path2;
    else if (Stab_Identical_Types(t1, desired_ty,
				  FALSE,  /* check_quals */
				  TRUE,   /* check_scalars */
				  FALSE)) /* ptrs_as_scalars */
      best_path = path1; /* path2 cannot possibly be any better */
    else if (Stab_Identical_Types(t2, desired_ty,
				  FALSE,  /* check_quals */
				  TRUE,   /* check_scalars */
				  FALSE)) /* ptrs_as_scalars */
      best_path = path2;
    else
      best_path = path1;

    /* Free up the path not chosen */
    if (best_path == path1)
      TY2F_Free_Fld_Path(path2);
    else
      TY2F_Free_Fld_Path(path1);

    return best_path;
  } /* Select_Best_Fld_Path */


  static FLD_PATH_INFO *
  Construct_Fld_Path(FLD_HANDLE   fld,
		     TY_IDX    struct_ty,
		     TY_IDX    desired_ty,
		     mUINT64   desired_offset,
		     mUINT64   max_fld_size)
  {
    /* Returns the field path through "fld" found to best match the 
     * given offset and type.  As a minimum requirement, the offset 
     * must be as desired and the type must have the desired size
     * and alignment (with some concessions allowed for substrings).
     * The path is terminate with a NULL next pointer.  When no 
     * field matches the desired type and offset, NULL is returned.
     */
    FLD_PATH_INFO    *fld_path;
    const mUINT64     fld_offset = FLD_ofst(fld);
    TY_IDX            fld_ty = FLD_type(fld);
    BOOL              is_array_elt = FALSE;
    STAB_OFFSET       ofst_in_fld = 0;
  
    if (TY_is_f90_pointer(fld_ty))
      fld_ty = TY_pointed(fld_ty);
  
  
    /* This field cannot be on the path to a field with the given
     * attributes, unless the desired_offset is somewhere within
     * the field.
     */
#if DBGPATH
    printf (" Construct: fld %s, struct %s, desired %s , des off %d \n",
	    FLD_name(fld), TY_name(struct_ty), TY_name(desired_ty),
	    desired_offset);
#endif

    if (desired_offset < fld_offset ||
	desired_offset >= (fld_offset + TY_size(fld_ty))) {
      /* This field cannot be on the path to a field with the given
       * attributes, since the desired_offset is nowhere within
       * the field.
       */
      fld_path = NULL;
#if DBGPATH
      printf ("     found NULL\n");
#endif
    } else if (TY_Is_Array(fld_ty) && TY_is_character(fld_ty) &&
	       TY_Is_Array(desired_ty) && TY_is_character(desired_ty)) {
#if DBGPATH
      printf ("     found char substring\n");
#endif
      /* A match is found! */
      ofst_in_fld = (desired_offset - fld_offset)/TY_size(TY_AR_etype(fld_ty));
      ofst_in_fld *= TY_size(TY_AR_etype(fld_ty));
      if ((ofst_in_fld + TY_size(desired_ty)) > TY_size(fld_ty)) {
	fld_path = NULL; /* The string does not fit */
      } else {
	fld_path = New_Fld_Path_Info(fld);
	if (TY_size(fld_ty) != TY_size(desired_ty)) {
	  fld_path->arr_elt = TRUE;
	  fld_path->arr_ofst = ofst_in_fld;
	} 
      }
    } else {
      /* See if the field we are looking for may be an array element */
    
      if (TY_kind(desired_ty)==KIND_POINTER)   
	desired_ty = TY_pointed(desired_ty);
      if (TY_kind(desired_ty)==KIND_ARRAY)
	desired_ty = TY_AR_etype(desired_ty);

      is_array_elt = (TY_Is_Array(fld_ty) &&
		      (TY_Is_Structured(TY_AR_etype(fld_ty))||
		       TY2F_is_character(fld_ty) ||
		       Stab_Identical_Types(TY_AR_etype(fld_ty), desired_ty,
					    FALSE,   /* check_quals */
					    FALSE,   /* check_scalars */
					    TRUE))); /* ptrs_as_scalars */
#if DBGPATH
      printf ("     is_array = %d, fld_ty %s \n",is_array_elt,TY_name(fld_ty));
#endif

      if (is_array_elt) {
	fld_ty = TY_AR_etype(fld_ty);
	ofst_in_fld =
	  ((desired_offset - fld_offset)/TY_size(fld_ty)) * TY_size(fld_ty);
      }
    
      if (TY_Is_Structured(fld_ty) &&
	  !Stab_Identical_Types(fld_ty, desired_ty,
				FALSE,  /* check_quals */
				FALSE,  /* check_scalars */
				TRUE)) {  /* ptrs_as_scalars */
#if DBGPATH
	printf ("     recurse \n");
#endif
	FLD_PATH_INFO *fld_path2 = 
	  TY2F_Get_Fld_Path(fld_ty, desired_ty, 
			    desired_offset - (fld_offset+ofst_in_fld));
      
	/* If a matching path was found, attach "fld" to the path */
	if (fld_path2 != NULL) {
	  if (TY_split(Ty_Table[fld_ty]))
	    fld_path = fld_path2; /* A stransparent substructure */
	  else {
	    fld_path = New_Fld_Path_Info(fld);
	    fld_path->arr_elt = is_array_elt;
	    fld_path->arr_ofst = ofst_in_fld;
	    fld_path->next = fld_path2;
	  }
	} else {
	  fld_path = NULL;
	}
      } else { /* This may be a field we want to take into account */
	const STAB_OFFSET fld_size = TY2F_Fld_Size(fld, max_fld_size);

	/* We only match a field with the expected size, offset
	 * and alignment.
	 */
	if (desired_offset != fld_offset+ofst_in_fld || /* unexpected ofst */
	    // fld_size < (TY_size(fld_ty)+ofst_in_fld) || /* unexpected size */
	    TY_align(struct_ty) < TY_align(fld_ty)) {   /* unexpected align */
#if DBGPATH
	  printf ("     account - miss\n");
#endif
	
	  fld_path = NULL;
	} else { /* A match is found! */
#if DBGPATH
	  printf ("     account - match\n");
#endif
	  fld_path = New_Fld_Path_Info(fld);
	  fld_path->arr_elt = is_array_elt;
	  fld_path->arr_ofst = ofst_in_fld;
	}/*if*/
      } /*if*/
    } /*if*/
  
    return fld_path;
  } /* Construct_Fld_Path */


  static const char * 
  TY2F_Fld_Name(FLD_HANDLE fld, 
		BOOL       common_or_equivalence,
		BOOL       alt_return_name)
  {
    /* Since fields may be accessed in an unqualified manner in Fortran,
     * e.g. for common block members and equivalences, so we need to treat
     * them similar to the way we would treat regular objects.
     */
    const char *fld_name = NULL;

    if (common_or_equivalence && !alt_return_name) {
      fld_name = FLD_name(fld);
    } else {
      fld_name = FLD_name(fld);
    }
    if (fld_name == NULL || *fld_name == '\0') { fld_name = "anon-fld"; }

    return fld_name;
  } /* TY2F_Fld_Name */


  /*------ Utilities for accessing and declaring KIND_STRUCTs ------
   *----------------------------------------------------------------*/

  static void
  TY2F_Equivalence(xml::ostream& xos, 
		   const char  *equiv_name, 
		   const char  *fld_name,
		   STAB_OFFSET  fld_ofst)
  {
    /* Append one equivalence statement to the tokens buffer,
     * keeping in mind that the equiv_name is based at index 1. */
    xos << "EQUIVALENCE(" << equiv_name; /* equiv_name at given offset */
    xos << "(" << Num2Str(fld_ofst, "%lld") << "),";
    xos << fld_name << ")";  /* fld_name at offset zero */
  } /* TY2F_Equivalence */


  static void
  TY2F_Equivalence_FldList(xml::ostream& xos, 
			   FLD_HANDLE   fldlist,
			   UINT         equiv_var_idx,
			   mUINT64      ofst,
			   BOOL        *common_block_equivalenced)
  {
    FLD_ITER fld_iter = Make_fld_iter(fldlist);

    do {
      FLD_HANDLE fld (fld_iter);

      if (TY_split(Ty_Table[FLD_type(fld)]))
	{
	  TY2F_Equivalence_FldList(xos, 
				   TY_flist(Ty_Table[FLD_type(fld)]),
				   equiv_var_idx,
				   ofst + FLD_ofst(fld),
				   common_block_equivalenced);
	}
      else if (FLD_equivalence(fld) || !*common_block_equivalenced)
	{
	  xos << std::endl;
	  const char* tmpvar = StrCat("tmp", Num2Str(equiv_var_idx, "%d"));
	  TY2F_Equivalence(xos, tmpvar, TY2F_Fld_Name(fld_iter, TRUE/*equiv*/,
						      FALSE/*alt_ret*/),
			   ofst + FLD_ofst(fld));
	  if (!FLD_equivalence(fld))
	    *common_block_equivalenced = TRUE;
	}

    }
    while (!FLD_last_field (fld_iter++)) ;

  } /* TY2F_Equivalence_FldList */


  static void
  TY2F_Equivalence_List(xml::ostream& xos, 
			const TY_IDX struct_ty)
  {
    /* Append a nameless EQUIVALENCE specification statement for
     * each equivalence field in the given struct.  Declare a 
     * dummy symbol as an array of INTEGER*1 elements to represent
     * the structure and each EQUIVALENCE specification will then 
     * equivalence a field to this dummy-symbol at the field offset.
     *
     * Group these declarations together by prepending each 
     * declaration (including the first one) with a newline.
     *
     * For COMMON blocks, it is also necessary to emit one element
     * that is not an equivalence!
     */
    TY_IDX     equiv_ty;
    UINT       equiv_var_idx;
    BOOL       common_block_equivalenced = FALSE;

    /* Declare an INTEGER*1 array (or CHARACTER string?) variable
     * to represent the whole equivalenced structure. Don't unlock
     * the tmpvar, or a similar equivalence group (ie: TY) will 
     * get the same temp.
     */
    equiv_ty = Stab_Array_Of(Stab_Mtype_To_Ty(MTYPE_I1), TY_size(struct_ty));
    equiv_var_idx = Stab_Lock_Tmpvar(equiv_ty, &ST2F_Declare_Tempvar);

    /* Relate every equivalence field to the temporary variable.
     */
    TY2F_Equivalence_FldList(xos, 
			     TY_flist(Ty_Table[struct_ty]),
			     equiv_var_idx,
			     0, /* Initial offset */
			     &common_block_equivalenced);

  } /* TY2F_Equivalence_List */

  // static void
  // TY2F_Translate_Structure(xml::ostream& xos, TY_IDX ty)
  // {
  //   FORTTK_ASSERT(TY_kind(ty) == KIND_STRUCT, "Unexpected type " << TY_kind(ty));

  //   FLD_ITER     fld_iter;
  //   TY& ty_rt  = Ty_Table[ty];

  //   PUXlationContext ctxt;// FIXME

  //   xos << std::endl;
  
  //   /* Emit structure header */
  //   xos << "TYPE " << TY_name(ty);
  
  //   if (TY_is_sequence(ty_rt)) {
  //     xos << std::endl << "SEQUENCE ";
  //   }
  
  //   /* Emit structure body */
  //   FLD_IDX flist = ty_rt.Fld();
  
  //   if (flist != 0) {
  //     fld_iter = Make_fld_iter(TY_flist(ty_rt));
  //     do {
  //       FLD_HANDLE fld (fld_iter);
      
  //       /* if it's a bitfield, then assume it's part of a dope vector & */
  //       /* just put out the name of the first bitfield in this I4       */
  //       if (NOT_BITFIELD_OR_IS_FIRST_OF_BITFIELD(fld_iter)) {
  // 	/* See if this field starts a map or a union */
	
  // 	xos << std::endl;
  // 	if (FLD_begin_union(fld)) {
  // 	  xos << "  UNION" << std::endl;
  // 	} else if (FLD_begin_map(fld)) {
  // 	  xos << "  MAP" << std::endl;
  // 	}
	
  // 	/* Declare this field */
  // 	if (FLD_is_pointer(fld)) {
  // 	  xos << ",POINTER::";
  // 	}

  // 	xos << TY2F_Fld_Name(fld_iter, FALSE/*common*/, FALSE/*alt_ret_name*/);
	
  // 	if (FLD_is_pointer(fld) && (TY_kind(FLD_type(fld)) == KIND_ARRAY)) {
  // 	  TY2F_array_for_pointer(xos, FLD_type(fld), ctxt);
  // 	} else {
  // 	  TY2F_translate(xos, FLD_type(fld), ctxt);
  // 	}
	
  // 	/* See if this field terminates a map or union */
  // 	if (FLD_end_union(fld)) {
  // 	  xos << std::endl << "END UNION";
  // 	} else if (FLD_end_map(fld)) {
  // 	  xos << std::endl << "END MAP";
  // 	}
  //       }       
  //     } while (!FLD_last_field (fld_iter++)) ;
  //   }
  
  //   /* Emit structure tail */
  //   xos << std::endl;
  //   xos << "END TYPE" << std::endl;
  // }


  static void
  TY2F_Translate_EquivCommon_PtrFld(xml::ostream& xos, FLD_HANDLE fld)
  {
    assert(0);
  }

  static void
  TY2F_Declare_Common_Flds(xml::ostream& xos,
			   FLD_HANDLE   fldlist, 
			   BOOL         alt_return, /* Alternate return points */
			   BOOL        *is_equiv)   /* out */
  {
    assert(0);
  } 

  static void
  TY2F_List_Common_Flds(xml::ostream& xos, FLD_HANDLE fldlist)
  {
    FLD_ITER fld_iter = Make_fld_iter(fldlist);

    do {
      FLD_HANDLE fld (fld_iter);
      TY & ty  = Ty_Table[FLD_type(fld)];       
    
      if (TY_split(ty)) {
	/* Treat a full split element as a transparent data-structure */
	TY2F_List_Common_Flds(xos, TY_flist(ty));
      } else if (!FLD_equivalence(fld)) {
	xos << TY2F_Fld_Name(fld_iter, TRUE/*common*/, FALSE/*alt_ret_name*/);
      }
    
      if (!FLD_last_field(fld)) {
	FLD_ITER  next_iter = fld_iter ;
	FLD_HANDLE next (++next_iter);
	if (!FLD_equivalence(next))
	  xos << ',';
      }
    
    } while (!FLD_last_field (fld_iter++)) ;

  } /* TY2F_List_Common_Flds */

  /*------------- Hidden routines to declare variable types -------------*/
  /*---------------------------------------------------------------------*/

  static void
  TY2F_invalid(xml::ostream& xos, TY_IDX ty, PUXlationContext& ctxt)
  {
    FORTTK_DIE(fortTkSupport::Diagnostics::UnexpectedInput << TY_kind(Ty_Table[ty]));
  }

  static void
  TY2F_scalar(xml::ostream& xos, TY_IDX ty_idx, PUXlationContext& ctxt)
  {
    FORTTK_ASSERT(TY_kind(ty_idx) == KIND_SCALAR, fortTkSupport::Diagnostics::UnexpectedInput);
  
    TY&   ty = Ty_Table[ty_idx];
    MTYPE mt = TY_mtype(ty);

    const char* type_str;
    if (TY_is_character(ty)) {
      type_str = "CHARACTER";
    } else if (TY_is_logical(ty)) {
      type_str = "LOGICAL";
    } else {
      switch(mt) {
      case MTYPE_U1: // Strictly speaking unsigned integers not supported
      case MTYPE_U2: // in Fortran, but we are lenient and treat them
      case MTYPE_U4: // as the signed equivalent.
      case MTYPE_U8:
      
      case MTYPE_I1:
      case MTYPE_I2:
      case MTYPE_I4:
      case MTYPE_I8:
	type_str = "INTEGER";
	break;
      
      case MTYPE_F4:
      case MTYPE_F8:
      case MTYPE_FQ:
	type_str = "REAL";
	break;
      
      case MTYPE_C4:
      case MTYPE_C8:
      case MTYPE_CQ:
	type_str = "COMPLEX";
	break;
      
      case MTYPE_M:
	type_str = "memory block";
	break;
      
      default:
	FORTTK_DIE("Unexpected type " << MTYPE_name(mt));
      }
    }
  
    const char* size_str;
    INT64 size;
    if (TY_size(ty) > 0) {
      if (ctxt.isF90() && MTYPE_is_complex(mt)) {
	size = TY_size(ty) / 2; 
      } else {
	size = TY_size(ty);
      }
      size_str = Num2Str(size, "%lld");
    } else {
      if (mt == MTYPE_M) {
	size_str = ".mblock.";
      } else {
	FORTTK_ASSERT(TY_is_character(ty), 
		      "Unexpected type size " << TY_size(ty));
	size_str = "*";
      }
    }

    const char* str = StrCat(type_str, size_str);

    xos << xml::BegElem("xaif:Property") << xml::Attr("id", ctxt.currentXlationContext().getNewVertexId()) 
	<< xml::Attr("name", "type") << xml::Attr("value", str) << xml::EndElem;

    xos << xml::BegElem("xaif:Property") << xml::Attr("id", ctxt.currentXlationContext().getNewVertexId()) 
	<< xml::Attr("name", "whirltype") << xml::Attr("value", TY_name(ty)) << xml::EndElem;
  }

  static void
  TY2F_array(xml::ostream& xos, TY_IDX ty_idx, PUXlationContext& ctxt)
  {
    TY& ty = Ty_Table[ty_idx];

    FORTTK_ASSERT(TY_kind(ty) == KIND_ARRAY, fortTkSupport::Diagnostics::UnexpectedInput);

    xos << xml::BegElem("xaif:Property") << xml::Attr("id", ctxt.currentXlationContext().getNewVertexId()) 
	<< xml::Attr("name", "whirlkind") << xml::Attr("value", "array") << xml::EndElem;


    if (TY_is_character(ty)) { // FIXME
      // A character string... 
      if (TY_size(ty) > 0) /* ... of known size */
	xos << "CHARACTER*" << Num2Str(TY_size(ty), "%lld");
      else /* ... of unknown size */
	xos << "CHARACTER*(*)";

    } else {
      // A regular array, so prepend the element type and append
      // the index bounds.
      ARB_HANDLE arb_base = TY_arb(ty);
      INT32 dim = ARB_dimension(arb_base) ;
      INT32 co_dim = ARB_co_dimension(arb_base);
      INT32 array_dim = dim - co_dim;
      INT32 revdim = 0;

      if (ARB_co_dimension(arb_base) <= 0) {
	co_dim = 0;
	array_dim = dim;
      }

      // 1. Translate element type
      xos << xml::BegElem("xaif:Property") << xml::Attr("id", ctxt.currentXlationContext().getNewVertexId()) 
	  << xml::Attr("name", "ArrayElementType");

      // Do not permit pointers as elements of arrays, so just use
      // the corresponding integral type instead.  We do not expect
      // such pointers to be dereferenced anywhere. (FIXME)
      if (TY_Is_Pointer(TY_AR_etype(ty)))
	TY2F_translate(xos, Stab_Mtype_To_Ty(TY_mtype(TY_AR_etype(ty))), ctxt);
      else
	TY2F_translate(xos, TY_AR_etype(ty), ctxt);
    
      xos << xml::EndElem;

      // 2. Translate dimension attributes
      while (array_dim > 0) {
      
	xos << xml::BegElem("xaif:Property") << xml::Attr("id", ctxt.currentXlationContext().getNewVertexId()) 
	    << xml::Attr("name", "ArrayDimensionAttr") << xml::Attr("dim", dim);
      
	ARB_HANDLE arb = arb_base[dim-1];
	TY2F_Append_ARB(xos, arb, ty_idx, ctxt);
      
	xos << xml::EndElem;
      
	array_dim--;
	dim--;
	revdim++;
      } 

      // 3. What is this???
      dim = ARB_dimension(arb_base);
      array_dim = dim - co_dim;
      --dim;
    
      if (co_dim > 0) {
	xos << '[';
	while (co_dim > 0) {
	  ARB_HANDLE arb = arb_base[dim-array_dim];
	

	  if (TY_is_f90_deferred_shape(ty))
	    TY2F_Append_ARB(xos, arb, ty_idx, ctxt);
	  else {
	    if (co_dim == 1)
	      TY2F_Append_ARB(xos, arb, ty_idx, ctxt); // TRUE
	    else
	      TY2F_Append_ARB(xos, arb, ty_idx, ctxt); // FALSE
	  }
	
	  dim--;

	  if (co_dim > 1)
	    xos << ',';

	  co_dim--;
	  ++revdim;
	}
	xos << ']';
      }

    }
  } /* TY2F_array */


  static void
  TY2F_array_for_pointer(xml::ostream& xos, TY_IDX ty_idx, PUXlationContext& ctxt)
  {
    TY& ty = Ty_Table[ty_idx] ;

    FORTTK_ASSERT(TY_kind(ty) == KIND_ARRAY, fortTkSupport::Diagnostics::UnexpectedInput);
  
    if (TY_is_character(ty)) {
      /* A character string...
       */
      if (TY_size(ty) > 0) /* ... of known size */
	xos << "CHARACTER*" << Num2Str(TY_size(ty), "%lld");
      else /* ... of unknown size */
	xos << "CHARACTER*(*)";
    } else {
      /* A regular array, so prepend the element type and append
       * the index bounds.
       */
      ARB_HANDLE arb_base = TY_arb(ty);
      INT32       dim = ARB_dimension(arb_base) ;
      INT32       co_dim = ARB_co_dimension(arb_base);
      INT32       array_dim = dim-co_dim;
      INT32       revdim = 0;
    
      /* Do not permit pointers as elements of arrays, so just use
       * the corresponding integral type instead.  We do not expect
       * such pointers to be dereferenced anywhere.
       */
      if (TY_Is_Pointer(TY_AR_etype(ty)))
	TY2F_translate(xos, Stab_Mtype_To_Ty(TY_mtype(TY_AR_etype(ty))), ctxt);
      else
	TY2F_translate(xos, TY_AR_etype(ty), ctxt);
    
      if (ARB_co_dimension(arb_base)<=0) {
	co_dim=0;
	array_dim = dim;
      }
    
      if (array_dim>0) {
	xos << "(";
      
	while (array_dim > 0) {
	  ARB_HANDLE arb = arb_base[dim-1];
	  xos << ':';
	  if (array_dim-- > 1)
	    xos << ',';
	
	  --dim;
	  ++revdim;
	}
      
	xos << ')';
      }
    
      dim = ARB_dimension(arb_base);
      array_dim = dim - co_dim;
      --dim;
    
      if (co_dim > 0) {
	xos << '[';
	while (co_dim > 0) {
	  ARB_HANDLE arb = arb_base[dim-array_dim];
	  xos << ':';
	  dim--;

	  if (co_dim-- > 1)
	    xos << ',';
	
	  ++revdim;
	}
	xos << ']';
      }
    }
  } /* TY2F_array_for_pointer */


  static void
  TY2F_struct(xml::ostream& xos, TY_IDX ty, PUXlationContext& ctxt)
  {
    /* Structs are supported by VAX-Fortran and Fortran-90.  Note
     * that we here emit a RECORD declaration, while we expect
     * the STRUCTURE to have been declared through a call to
     * TY2F_Translate_Structure().
     */
    TY& ty_rt = Ty_Table[ty];
    FORTTK_ASSERT(TY_kind(ty_rt) == KIND_STRUCT, fortTkSupport::Diagnostics::UnexpectedInput);
  
    xos << "(" << TY_name(ty) << ")" << "TYPE";
  
#if 0 // see Open64 stab_attr.cxx; if needed simulate thru PUXlationContext
    if (!TY_is_translated_to_c(ty)) {
      TY2F_Translate_Structure(xos, ty);
      Set_TY_is_translated_to_c(ty); /* Really, translated to Fortran, not C */
    }
#endif
  }


  static void
  TY2F_2_struct(xml::ostream& xos, TY_IDX ty, PUXlationContext& ctxt)
  {
    /* Structs are supported by VAX-Fortran and Fortran-90.  Note
     * that we here emit a RECORD declaration, while we expect
     * the STRUCTURE to have been declared through a call to
     * TY2F_Translate_Structure().
     */
    TY & ty_rt = Ty_Table[ty];
    FORTTK_ASSERT(TY_kind(ty_rt) == KIND_STRUCT, fortTkSupport::Diagnostics::UnexpectedInput);

#if 0 // see Open64 stab_attr.cxx; if needed simulate thru PUXlationContext
    if (!TY_is_translated_to_c(ty)) {
      TY2F_Translate_Structure(xos, ty);
      Set_TY_is_translated_to_c(ty); /* Really, translated to Fortran, not C */
    }
#endif
  }


  static void
  TY2F_pointer(xml::ostream& xos, TY_IDX ty, PUXlationContext& ctxt)
  {
    /* Is a dope vector base address? Put out an integer large enough */
    /* to hold an address for now. Don't really want POINTER because  */
    /* implies cray/f90 pointer instead of address slot               */
  
    if (TY2F_Pointer_To_Dope(ty)) {
#if 0
      Prepend_Token_String(xos,",POINTER ::");
#endif
      TY2F_translate(xos,Be_Type_Tbl(Pointer_Mtype), ctxt);
    } else {
      /* avoid recursive type declarations */
      if (TY_kind(TY_pointed(ty)) == KIND_STRUCT) {
#if 0
	Prepend_Token_String(xos,",POINTER ::");
	Prepend_Token_String(xos, TY_name(TY_pointed(ty)));
#endif
	TY2F_translate(xos,Be_Type_Tbl(Pointer_Mtype), ctxt);
      
      } else
	TY2F_translate(xos,TY_pointed(ty), ctxt);
    }
  } /* TY2F_pointer */

  static void
  TY2F_void(xml::ostream& xos, TY_IDX ty_idx, PUXlationContext& ctxt)
  {
    TY& ty = Ty_Table[ty_idx];
    FORTTK_ASSERT(TY_kind(ty) == KIND_VOID, fortTkSupport::Diagnostics::UnexpectedInput);
    xos << std::endl << "! <Void Type>";
  }

  /*------------------------ exported routines --------------------------*/
  /*---------------------------------------------------------------------*/


  // JU: I don't think the conditions under which this method is called
  // in the rest of the code are ever satisfied. 
  void 
  TY2F_Translate_ArrayElt(xml::ostream& xos, 
			  TY_IDX       arr_ty_idx,
			  STAB_OFFSET  arr_ofst)
  {  
 	std::cout << "TEMP WARNING" << std::endl; 
  }


  void
  TY2F_Translate_Common(xml::ostream& xos, const char *name, TY_IDX ty_idx)
  {
    TY& ty = Ty_Table[ty_idx];
    BOOL is_equiv = FALSE;
  
    FORTTK_ASSERT(TY_kind(ty) == KIND_STRUCT, 
		  fortTkSupport::Diagnostics::UnexpectedInput << TY_kind(ty));

    // Emit specification statements for every element of the common
    // block, including equivalences.
    xos << xml::BegComment << "COMMON";
    if (name != NULL && *name != '\0') { xos << " name = " << name; }
    xos << xml::EndComment;

#if 0 // FIXME
    TY2F_List_Common_Flds(xos, TY_flist(ty));
  
    // variables in common block type declaration
    TY2F_Declare_Common_Flds(xos, TY_flist(ty), FALSE /*alt_return*/, &is_equiv);
  
    // Emit equivalences, if there are any
    if (is_equiv)
      TY2F_Equivalence_List(xos, ty_idx /*struct_ty*/);
#endif
  }


  void
  TY2F_Translate_Equivalence(xml::ostream& xos, TY_IDX ty_idx, BOOL alt_return)
  {
    /* When alt_return==TRUE, this represents an alternate return variable,
     * in which case we should declare the elements of the equivalence
     * with unmangled names and ignore the fact that they are in an
     * equivalence.  The first element in such an alternate return is
     * the function/subprogram return-variable, which we should never
     * declare.
     */
    TY& ty = Ty_Table[ty_idx];

    FLD_HANDLE first_fld;
    BOOL is_equiv;
   
    FORTTK_ASSERT(TY_kind(ty) == KIND_STRUCT, 
		  fortTkSupport::Diagnostics::UnexpectedInput << TY_kind(ty));

    if (alt_return) {
      first_fld = FLD_next(TY_flist(ty)); /* skip func_entry return var */
    } else {
      first_fld = TY_flist(ty);
    }

    /* Emit specification statements for every element of the
     * equivalence block.
     */  
    TY2F_Declare_Common_Flds(xos, first_fld, alt_return,
			     &is_equiv);  /* Redundant in this call */

    if (!alt_return)
      TY2F_Equivalence_List(xos, ty_idx /*struct_ty*/);

  } /* TY2F_Translate_Equivalence */


  FLD_PATH_INFO * 
  TY2F_Free_Fld_Path(FLD_PATH_INFO *fld_path)
  {
    FLD_PATH_INFO *free_list;
   
    if (fld_path != NULL) {
      free_list = Free_Fld_Path_Info;
      Free_Fld_Path_Info = fld_path;
      while (fld_path->next != NULL)
	fld_path = fld_path->next;
      fld_path->next = free_list;
    }
    return NULL;
  } /* TY2F_Free_Fld_Path */


  FLD_PATH_INFO * 
  TY2F_Get_Fld_Path(const TY_IDX struct_ty, const TY_IDX object_ty, 
		    STAB_OFFSET offset)
  {
    FLD_PATH_INFO* fld_path;
    FLD_PATH_INFO* fld_path2 = NULL;
    TY& s_ty = Ty_Table[struct_ty];
    FLD_ITER fld_iter;

    FORTTK_ASSERT(TY_kind(s_ty) == KIND_STRUCT, 
		  fortTkSupport::Diagnostics::UnexpectedInput << TY_kind(s_ty));
  
    /* Get the best matching field path into fld_path2 */
    fld_iter = Make_fld_iter(TY_flist(s_ty));
  
    do {
      FLD_HANDLE fld (fld_iter);
    
      if (NOT_BITFIELD_OR_IS_FIRST_OF_BITFIELD(fld_iter)) {
	fld_path = Construct_Fld_Path(fld_iter, struct_ty, object_ty,
				      offset, TY_size(s_ty));
	if (fld_path2 == NULL)
	  fld_path2 = fld_path;
	else if (fld_path != NULL)
	  fld_path2 = Select_Best_Fld_Path(fld_path2, fld_path, object_ty,
					   offset);
      }
    } while (!FLD_last_field (fld_iter++));
  
    /* POSTCONDITION: fld_path2 points to the best match found */
    return fld_path2;
  }

  void
  TY2F_Translate_Fld_Path(xml::ostream&   xos,
			  FLD_PATH_INFO *fld_path, 
			  BOOL           deref, 
			  BOOL           member_of_common, 
			  BOOL           alt_ret_name,
			  PUXlationContext& ctxt)
  {
    /* Append the name of each field to the tokens, separated them
     * from each other by the field-selection operator ('.').  The
     * first name on the path may optionally be emitted in unclobbered 
     * form, as it may represent an alternate return point.
     */
    while (fld_path != NULL) {
      FLD_HANDLE f (fld_path->fld);
      const char* str = TY2F_Fld_Name(f, member_of_common, alt_ret_name);
      if (deref && TY_Is_Pointer(FLD_type(f))) {
	str = StrCat("deref_", str); // W2CF_Symtab_Nameof_Fld_Pointee(f);
      }
      xos << xml::BegElem("TYFLD") << xml::Attr("***name", str) << xml::EndElem;

      member_of_common = FALSE; /* Can only be true first time around */
    
      /* if an array element, form the subscript list. If an OPC_ARRAY */
      /* provides the subscripts, use it o/w use offset                */
      if (fld_path->arr_elt) {
	if (fld_path->arr_wn != NULL)
	  WN2F_array_bounds(xos, fld_path->arr_wn, FLD_type(f), ctxt);
      }
    
      /* Separate fields with the dot-notation. */
      fld_path = fld_path->next;
      if (fld_path != NULL) {
	TY2F_Fld_Separator(xos) ;
	alt_ret_name = FALSE; /* Only applies to first field on the path */
      }
    } /* while */
  
  } /* TY2F_Translate_Fld_Path */


  extern void
  TY2F_Fld_Separator(xml::ostream& xos)
  {
    /* puts out the appropriate structure component separator*/
    xos << '%';
  }

  extern FLD_HANDLE
  TY2F_Last_Fld(FLD_PATH_INFO *fld_path)
  {
    FLD_HANDLE f = FLD_HANDLE () ;

    while (fld_path != NULL) {
      f = fld_path->fld;
      fld_path = fld_path->next ;
    }
  
    return f;
  }

  extern FLD_PATH_INFO * 
  TY2F_Point_At_Path(FLD_PATH_INFO * path, STAB_OFFSET off)
  {
    /* given a fld path, return a pointer to */
    /* the slot at the given offset          */
    while (path != NULL) {
      if ((INT64)FLD_ofst(path->fld) >= off)
	break ;    
      path=path->next;
    }
    return path;
  }

  extern void
  TY2F_Dump_Fld_Path(FLD_PATH_INFO *fld_path)
  {
    printf ("path ::");
    while (fld_path != NULL) {
      FLD_HANDLE f = fld_path->fld;
    
      printf ("%s(#%d)",TY2F_Fld_Name(f,FALSE,FALSE),f.Idx ());
    
      if (fld_path->arr_elt)
	printf (" array");
    
      if (fld_path->arr_ofst)
	printf (" offset 0x%x",(mINT32) fld_path->arr_ofst);
    
      if (fld_path->arr_wn != NULL)
	printf (" tree 0x%p",fld_path->arr_wn);
    
      printf (" ::");
      fld_path = fld_path->next ;
    }
    printf ("\n");
  }


  // ***************************************************************************
  //
  // ***************************************************************************


  const char*
  TranslateTYToSymType(TY_IDX ty_idx)
  {
    TY& ty = Ty_Table[ty_idx];
    const char* str = NULL;

    if (TY_kind(ty) == KIND_SCALAR) {
      MTYPE mt = TY_mtype(ty);
      if (TY_is_character(ty)) {
	str = "char"; 
      } 
      else if (TY_is_logical(ty)) {
	str = "bool"; 
      } 
      else if (MTYPE_is_integral(mt)) {
	str = "integer"; 
      } 
      else if (MTYPE_is_complex(mt)) { /* must come before 'float' */
	str = "complex";
      } 
      else if (MTYPE_is_float(mt)) { 
	str = "real"; 
      }
    } 
    else if (TY_kind(ty) == KIND_ARRAY) {
      if (TY_is_character(ty)) { 
	str = "string"; 
      } 
      else {
	// Do not permit pointers as elements of arrays, so just use
	// the corresponding integral type instead.  We do not expect
	// such pointers to be dereferenced anywhere. (FIXME)
	TY_IDX ety_idx = TY_AR_etype(ty);
	if (TY_Is_Pointer(ety_idx)) {
	  ety_idx = Stab_Mtype_To_Ty(TY_mtype(ety_idx));
	} 
	str = TranslateTYToSymType(ety_idx);
      }
    } 
    else if (TY_kind(ty) == KIND_STRUCT 
	     || 
	     TY_kind(ty) == KIND_INVALID) {
      // the latter applies to symbols that are f90 interface names
      str = "opaque";
    }  
    else if (TY_kind(ty) == KIND_FUNCTION) {
      str = "void";
    } 
    else if (TY_kind(ty) == KIND_POINTER) { 
      str = "opaque";
      if (TY_kind(TY_pointed(ty)) == KIND_FUNCTION) {
	str = "void";
      }
    } 
    else 
      FORTTK_DIE("whirl2xaif::TranslateTYToSymType: no logic to handle type of kind " << TY_kind(ty));
    return str;
  }

  const char*
  TranslateTYToMType(TY_IDX ty_idx) {
    TY& ty_r = Ty_Table[ty_idx];
    if (TY_kind(ty_r) == KIND_SCALAR) {
      return Mtype_Name(TY_mtype(ty_r));
    } 
    else if (TY_kind(ty_r) == KIND_ARRAY) {
      if (TY_is_character(ty_r)) { 
	return Mtype_Name(TY_mtype(ty_r));
      } 
      else {
	// Do not permit pointers as elements of arrays, so just use
	// the corresponding integral type instead.  We do not expect
	// such pointers to be dereferenced anywhere. (FIXME)
	TY_IDX ety_idx = TY_AR_etype(ty_r);
	if (TY_Is_Pointer(ety_idx)) {
	  ety_idx = Stab_Mtype_To_Ty(TY_mtype(ety_idx));
	} 
	return TranslateTYToMType(ety_idx);
      }
    } 
    else if (TY_kind(ty_r) == KIND_STRUCT 
	     || 
	     TY_kind(ty_r) == KIND_INVALID
	     ||
	     TY_kind(ty_r) == KIND_FUNCTION) { 
      return Mtype_Name(TY_mtype(ty_r));
    } 
    else if (TY_kind(ty_r) == KIND_POINTER) { 
      return TranslateTYToMType(TY_pointed(ty_r));
    } 
    else 
      FORTTK_DIE("whirl2xaif::TranslateTYToMType: no logic to handle type of kind " << TY_kind(ty_r));
    return "";
  }

  const char*
  TranslateTYToSymShape(TY_IDX ty_idx)
  {
    TY& ty = Ty_Table[ty_idx];
    const char* str = NULL;
  
    if (TY_kind(ty) == KIND_SCALAR) {
      str = "scalar";
    } 
    else if (TY_kind(ty) == KIND_ARRAY) {
    
      ARB_HANDLE arb_base = TY_arb(ty);
      INT32 dim = ARB_dimension(arb_base);
      // ARB_co_dimension(arb_base) <= 0 FIXME

      if (TY_is_character(ty)) { 
	str = "scalar"; 
      } 
      else {
	switch (dim) {
	case 1:  str = "vector"; break;
	case 2:  str = "matrix"; break;
	case 3:  str = "three_tensor"; break;
	case 4:  str = "four_tensor"; break;
	case 5:  str = "five_tensor"; break;
	case 6:  str = "six_tensor"; break;
	case 7:  str = "seven_tensor"; break;
	default: 
	  FORTTK_DIE("Invalid array dimension: " << dim);
	}
      }

    } 
    else if (TY_kind(ty) == KIND_STRUCT
	     || 
	     TY_kind(ty) == KIND_INVALID) {
      // the latter applies to symbols that are f90 interface names
      str = "scalar"; // FIXME
    }  
    else if ((TY_kind(ty) == KIND_POINTER) &&
	     (TY_kind(TY_pointed(ty)) == KIND_FUNCTION)) {
      str = "void";
    }
  
    return str;
  }

}
