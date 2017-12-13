/*

  Copyright (C) 2000, 2001 Silicon Graphics, Inc.  All Rights Reserved.
  Portions Copyright (C) 2003 Rice University.

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
#ifndef ty_ftn_INCLUDED
#define ty_ftn_INCLUDED
/* ====================================================================
 * ====================================================================
 *
 *
 * Revision history:
 *  18-Jun-03 - Original version based on text extracted from stab_attr.h
 *
 * Description:
 *
 * Fortran-specific TY information.
 */

inline BOOL TY_Is_Character_Reference(TY_IDX ty)
{
   return TY_Is_Pointer(ty) &&
          (TY_is_character(TY_pointed(ty)) ||
           TY_mtype(TY_pointed(ty)) == MTYPE_STR);
} /* TY_Is_Character_Reference */

/* The front-end is not always reliable in where it sets the is_character
 * flag, so we look for it both on the array and on the element type.
 */
inline BOOL TY_Is_Character_String(TY_IDX ty)
{
   return TY_is_character(ty) ||
          TY_mtype(ty) == MTYPE_STR ||
          (TY_Is_Array(ty)                 &&
           TY_Is_Integral(TY_AR_etype(ty)) &&
           TY_size(TY_AR_etype(ty)) == 1   &&
           TY_is_character(TY_AR_etype(ty)));
} /* TY_Is_Character_String */

inline BOOL TY_Is_Chararray(TY_IDX ty)
{
   return TY_Is_Array(ty) && TY_Is_Character_String(TY_AR_etype(ty));
} /* TY_Is_Chararray */

inline BOOL TY_Is_Chararray_Reference(TY_IDX ty)
{
   return TY_Is_Pointer(ty) && TY_Is_Chararray(TY_pointed(ty));
} /* TY_Is_Chararray_Reference */

/* A macro to test if a parameter is a character string, in which case
 * it needs an implicit length parameter.  Note that in the test on the
 * argument (caller) side we only need to consider reference types, since
 * the call-by-reference always should be explicit on that side.  This
 * macro should only be used on the subprogram definition side!  This
 * only applies to Fortran code.
 */
inline BOOL STAB_PARAM_HAS_IMPLICIT_LENGTH(const ST *st)
{
   return ((ST_sclass(st) == SCLASS_FORMAL_REF &&
            TY_Is_Character_String(ST_type(st))) ||
           (ST_sclass(st) == SCLASS_FORMAL &&
            (TY_Is_Character_Reference(ST_type(st)) ||
             TY_Is_Chararray_Reference(ST_type(st)))));
}

/* Identify cases when a reference parameter is explicitly represented
 * as a pointer (as opposed to an SCLASS_FORMAL_REF).  This only applies
 * to Fortran code.
*/
inline BOOL STAB_IS_POINTER_REF_PARAM(const ST *st)
{
   return (TY_Is_Pointer(ST_type(st))   &&
           ST_sclass(st)==SCLASS_FORMAL &&
           !ST_is_value_parm(st));
}

#endif /* ty_ftn_INCLUDED */
