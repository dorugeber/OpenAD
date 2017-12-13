/*

  Copyright (C) 2003 Rice University. All rights reserved.

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

*/

#ifndef unparse_target_ftn_INCLUDED
#define unparse_target_ftn_INCLUDED
/* ====================================================================
 *
 *
 * Revision history:
 *  06-Jun-03 - Original Version
 *
 * Description:
 *
 *     Runtime tests for the Fortran target language.
 */

#include "unparse_target.h"
#include "whirl2f_common.h"
#include "symtab.h"
#include "intrn_info.h"
#include "wutil.h"
#include "token_names.h"

static const char *
W2CF_Get_Ftn_St_Name(const ST *st, const char *original_name)
{ 
   const char *extern_name;

   char *name_ptr;

   if (Stab_External_Linkage(st) && 
       !Stab_Is_Based_At_Common_Or_Equivalence(st) &&
       !(ST_sym_class(st) == CLASS_VAR && ST_is_namelist(st)))
   {
      /* Here we deal with a curiosity of the Fortran naming scheme for
       * external names:
       *
       *    + If the name ends with a '_', the name was without the '_'
       *      in the original Fortran source.
       *
       *    + If the name ends without a '_', the name was with a '$'
       *      suffix in the original Fortran source.
       *
       *    + Unless the external name was a namelist variable, then even
       *      though there isn't a trailing '_', don't emit a '$'.
       */
      extern_name = name_ptr =
	 strcpy(Get_Name_Buf_Slot(strlen(original_name)+2), original_name);
      
      /* Get the last character */
      while (name_ptr[1] != '\0')
	 name_ptr++;
       
      /* Correct the name-suffix */
      if (extern_name[0] != '_'  && name_ptr[0] == '_')
      {
	 if (name_ptr[-1] == '_')
	    name_ptr[-1] = '\0';
	 else
	    name_ptr[0] = '\0';
      }
      else if (!WN2F_F90_pu)
      {
	 name_ptr[1] = '$';
	 name_ptr[2] = '\0';
      }
   }
   else /* Not an external variable */
      extern_name = original_name;

   return extern_name;
   
} /* W2CF_Get_Ftn_St_Name */

static const char *Ftn_Reserved_Ty_Name[] =
{
   "split_st",                /* compiler generated */
   "__$w2c_predef_ld_union",  /* compiler generated */
   "__$w2c_predef_ldv_union"  /* compiler generated */
}; /* Ftn_Reserved_Ty_Name */

static const char *Ftn_Reserved_St_Name[] =
{
   "TO DO"
}; /* Ftn_Reserved_St_Names */

#define NUM_FTN_TY_RNAMES (sizeof(Ftn_Reserved_Ty_Name)/sizeof(char *))
#define NUM_FTN_ST_RNAMES (sizeof(Ftn_Reserved_St_Name)/sizeof(char *))

class Unparse_Target_FTN : public Unparse_Target {
public:
	Unparse_Target_FTN ()
	{
		reserved_ty_names = new Reserved_Name_Set (NUM_FTN_TY_RNAMES, Ftn_Reserved_Ty_Name);
		reserved_st_names = new Reserved_Name_Set (NUM_FTN_ST_RNAMES, Ftn_Reserved_St_Name);
	}

	~Unparse_Target_FTN () {};

	const char *Make_Valid_Name(const char *name, BOOL allow_dot)
	{ return WHIRL2F_make_valid_name(name, WN2F_F90_pu && allow_dot); }

	const char *Get_St_Name(const ST *st, const char *original_name)
	{ return W2CF_Get_Ftn_St_Name (st, original_name); }

	const char *Intrinsic_Name(INTRINSIC intr_opc)
	{
   const char *name;
   
   Is_True(INTRINSIC_FIRST<=intr_opc && intr_opc<=INTRINSIC_LAST,
	   ("Intrinsic Opcode (%d) out of range", intr_opc)); 
   if (INTRN_specific_name(intr_opc) != NULL)
      name = INTRN_specific_name(intr_opc);
   else
   {
/*      ASSERT_WARN(FALSE, 
		  (DIAG_A_STRING,
		   Concat2_Strings("Missing intrinsic name ", 
				   get_intrinsic_name(intr_opc))));
*/
      name = get_intrinsic_name(intr_opc);
   }

   return name;

	}

	BOOL Avoid_Common_Suffix(void)
	{
	  BOOL avoid = TRUE;
	  return avoid;
	}

        BOOL Reduce_Const_Ptr_Exprs(void)
	{
	  return FALSE;
	}

	BOOL Enter_Symtab_Pointee_Names(void)
	{
	  return TRUE;
	}

	BOOL Redeclare_File_Types (void)
	{
	  return TRUE;
	}

	BOOL Builtin_Type (TY_IDX ty)
	{
	  return FALSE;  /* Mimicking behavior of WHIRL2F. */
	}

	BOOL Is_Binary_Or_Tertiary_Op (char c)
	{
          return (c==PLUS          || \
                  c==MINUS         || \
                  c==MULTIPLY      || \
                  c==DIVIDE        || \
                  c==BITAND        || \
                  c==BITOR         || \
                  c==EQUAL         || \
                  c==NOT           || \
                  c==QUESTION_MARK || \
                  c==COLON         || \
                  c==LESS_THAN     || \
                  c==LARGER_THAN);
	}

	/*------ Function type attributes ------*/
	/*--------------------------------------*/

	BOOL Func_Return_Character(TY_IDX func_ty)
	{
		return TY_is_character(Ty_Table[TY_ret_type(func_ty)]);
	} /* Func_Return_Character */

	TY_IDX Func_Return_Type(TY_IDX func_ty)
	{
		return TY_ret_type(func_ty);
	} /* Func_Return_Type */

	BOOL Func_Return_To_Param(TY_IDX func_ty)
	{
		return TY_return_to_param(Ty_Table[func_ty]);
	} /* Func_Return_To_Param */


};

#endif /* unparse_target_ftn_INCLUDED */
