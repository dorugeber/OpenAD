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


/* ====================================================================
 * ====================================================================
 *
 *
 * Revision history:
 *  07-May-95 - Original Version
 *
 * Description:
 *
 *    See st2f.h for a description of the exported functions and 
 *    variables.  This module translates ST nodes into variable and
 *    function declarations (ST2F_decl_translate), and gets the 
 *    lvalue for a variable or function when directly referenced in
 *    an expression (ST2F_use_translate).  We provide a special 
 *    interface to deal with pseudo registers (pregs), but some 
 *    symbols must be handled by the context in which they appear,
 *    since this context uniquely determines the reference (e.g. 
 *    labels has label-numbers in the WN tree).
 *
 *    Possibly necessary TODO: sym_consts are only partially
 *    supported at the moment.
 *
 *    Fortran pointers are represented by two declarations, where
 *    one declares the pointer object (which is allocated memory)
 *    and one denotes the pointer dereference which also serves to
 *    specify the type of object to which is pointed:
 *
 *        INTEGER*4 a(12)
 *        POINTER(p, a)
 *
 *    Only "p" occurs in the WHIRL symbol table.  We have to derive
 *    "a" from "p" (with a name derived from "p").  The w2cf_symtab.h
 *    facilities coordinates this for us.
 *
 *    It is crucial that names with external linkage are generated 
 *    with the same name between compilation units.  For this reason
 *    we give file-scope variables precedence in name-ownership (i.e.
 *    they are entered first into the symbol-table).  If, despite this
 *    effort, there are clashes between names with static and external 
 *    linkage, the generated code may not be compilable or correctly
 *    executable.  TODO: Emit warning about this.
 * 
 * ====================================================================
 * ====================================================================
 */

#ifdef _KEEP_RCS_ID
/*REFERENCED*/
#endif

#include <ctype.h>
#include <alloca.h>
#include <set>    // STL
#include <vector> // STL
using std::set;
using std::vector;
#include <string> 

#include "whirl2f_common.h"
#include "PUinfo.h"
#include "tcon2f.h"
#include "wn2f.h"
#include "ty2f.h"
#include "st2f.h"
#include "init2f.h"
#include "cxx_memory.h"
#include "be_symtab.h"
#include "unparse_target.h"
#include "ty_ftn.h"

typedef std::set<int> PARMSET;

 /* Defined in ty2f.c; signifies special translation of adjustable and
  * assumed sized arrays.
  */
extern BOOL Use_Purple_Array_Bnds_Placeholder;
extern WN* PU_Body; 
/*------- Fwd refs for miscellaneous utilities ------------------------*/
/*---------------------------------------------------------------------*/

static BOOL ST2F_Is_Dummy_Procedure(ST *st) ;
static void ST2F_Declare_Return_Type(TOKEN_BUFFER tokens,TY_IDX return_ty, const char* name) ;

/*------- Handlers for references to and declarations of symbols ------*/
/*---------------------------------------------------------------------*/

static void ST2F_ignore(TOKEN_BUFFER tokens, ST *st);

static void ST2F_decl_error(TOKEN_BUFFER tokens, ST *st);
static void ST2F_decl_var(TOKEN_BUFFER tokens, ST *st);
static void ST2F_decl_func(TOKEN_BUFFER tokens, ST *st);
static void ST2F_decl_const(TOKEN_BUFFER tokens, ST *st);
static void ST2F_decl_type (TOKEN_BUFFER tokens, ST *st);
static void ST2F_decl_parameter (TOKEN_BUFFER tokens, ST *st);

static void ST2F_use_error(TOKEN_BUFFER tokens, ST *st);
static void ST2F_use_var(TOKEN_BUFFER tokens, ST *st);
static void ST2F_use_func(TOKEN_BUFFER tokens, ST *st);
static void ST2F_use_const(TOKEN_BUFFER tokens, ST *st);
static void ST2F_use_block(TOKEN_BUFFER tokens, ST *st);

TOKEN_BUFFER  param_tokens =  New_Token_Buffer();

/* The following maps every ST class to a function that can translate
 * it to C.
 */
typedef void (*ST2F_HANDLER_FUNC)(TOKEN_BUFFER, ST *);

static const ST2F_HANDLER_FUNC ST2F_Decl_Handler[CLASS_COUNT] =
{
  &ST2F_ignore,      /* CLASS_UNK   == 0x00 */
  &ST2F_decl_var,    /* CLASS_VAR   == 0x01 */
  &ST2F_decl_func,   /* CLASS_FUNC  == 0x02 */
  &ST2F_decl_const,  /* CLASS_CONST == 0x03 */
  &ST2F_decl_error,  /* CLASS_PREG  == 0x04 */
  &ST2F_decl_error,  /* CLASS_BLOCK == 0x05 */
  &ST2F_decl_error,  /* CLASS_NAME  == 0x06 */
  &ST2F_decl_error,  /* CLASS_MODULE == 0x07*/
  &ST2F_decl_type,   /* CLASS_TYPE   ==0x08 */
  &ST2F_decl_parameter, /*CLASS_PARAMETER == 0x08 */
}; /* ST2F_Decl_Handler */

static const ST2F_HANDLER_FUNC ST2F_Use_Handler[CLASS_COUNT] =
{
  &ST2F_ignore,        /* CLASS_UNK   == 0x00 */
  &ST2F_use_var,       /* CLASS_VAR   == 0x01 */
  &ST2F_use_func,      /* CLASS_FUNC  == 0x02 */
  &ST2F_use_const,     /* CLASS_CONST == 0x03 */
  &ST2F_use_error,     /* CLASS_PREG  == 0x04 */
  &ST2F_use_block,     /* CLASS_BLOCK == 0x05 */
  &ST2F_use_error      /* CLASS_NAME  == 0x06 */
}; /* ST2F_Use_Handler */


/*----------- hidden routines to handle ST declarations ---------------*/
/*---------------------------------------------------------------------*/
static void
ST2F_Define_Preg(const char *name, TY_IDX ty)
{
   /* Declare a preg of the given type, name and offset as a local
    * (register) variable in the current context.
    */
   TOKEN_BUFFER decl_tokens = New_Token_Buffer();
   UINT         current_indent = Current_Indentation();

   Set_Current_Indentation(PUinfo_local_decls_indent);
   Append_F77_Indented_Newline(PUinfo_local_decls, 1, NULL/*label*/);
   Append_Token_String(decl_tokens, name);
   TY2F_translate(decl_tokens, ty);
   Append_And_Reclaim_Token_List(PUinfo_local_decls, &decl_tokens);
   Set_Current_Indentation(current_indent);
} /* ST2F_Define_Preg */


static void 
ST2F_ignore(TOKEN_BUFFER tokens, ST *st)
{
   return; /* Just ignore it, i.e. do nothing! */
} /* ST2F_ignore */

static void 
ST2F_decl_error(TOKEN_BUFFER tokens, ST *st)
{
   ASSERT_DBG_FATAL(FALSE, 
		    (DIAG_W2F_UNEXPECTED_SYMCLASS,
		     ST_sym_class(st), "ST2F_decl_error"));
} /* ST2F_decl_error */

static void 
ST2F_decl_var(TOKEN_BUFFER tokens, ST *st)
{
   INITO_IDX    inito;
   const char  *pointee_name;
   const char  *st_name = W2CF_Symtab_Nameof_St(st);
   TOKEN_BUFFER decl_tokens = New_Token_Buffer();
   TY_IDX       ty_rt = ST_type(st);
   ST *base;

   ASSERT_DBG_FATAL(ST_sym_class(st)==CLASS_VAR, 
		    (DIAG_W2F_UNEXPECTED_SYMCLASS, 
		     ST_sym_class(st), "ST2F_decl_var"));

   if (Current_scope > GLOBAL_SYMTAB) 
   {
       ASSERT_DBG_FATAL(!PUINFO_RETURN_TO_PARAM || st != PUINFO_RETURN_PARAM, 
		       (DIAG_W2F_DECLARE_RETURN_PARAM, "ST2F_decl_var"));
   }

  base = ST_base(st);


//  if (ST_sclass(st)==SCLASS_DGLOBAL && Stab_Is_Common_Block(base))
//	goto INITPRO;

   /* Declare the variable */

 if (Stab_Is_Common_Block(st))
   {
      /* Declare a common block */
      TY2F_Translate_Common(decl_tokens, st_name, ST_type(st));
   }
   else if (Stab_Is_Equivalence_Block(st))
   {
      if (ST_is_return_var(st))
	 TY2F_Translate_Equivalence(decl_tokens, 
				    ST_type(st), 
				    TRUE /* alternate return point */);
      else
	 TY2F_Translate_Equivalence(decl_tokens, 
				    ST_type(st), 
				    FALSE /* regular equivalence */);
   }
   else if (TY_Is_Pointer(ty_rt) && 
            !TY_is_f90_pointer(ty_rt) &&
            ST_sclass(st) != SCLASS_FORMAL)
   {
      /* Declare pointee with the name specified in the symbol table */
//      pointee_name = W2CF_Symtab_Nameof_St_Pointee(st);
      Append_Token_String(decl_tokens, st_name);

      if (TY_ptr_as_array(Ty_Table[ty_rt]))
	 TY2F_translate(decl_tokens, 
			Stab_Array_Of(TY_pointed(ty_rt), 0/*size*/));
      else
	 TY2F_translate(decl_tokens, TY_pointed(ty_rt));

      Append_F77_Indented_Newline(decl_tokens, 1, NULL/*label*/);

      /* Declare the pointer object */
//       Append_Token_String(decl_tokens, "POINTER");
//       Append_Token_Special(decl_tokens, '(');
//       Append_Token_String(decl_tokens, st_name);
//       Append_Token_Special(decl_tokens, ',');
//       Append_Token_String(decl_tokens, pointee_name);
//       Append_Token_Special(decl_tokens, ')');
   }
   else if (ST_sclass(st) == SCLASS_FORMAL && !ST_is_value_parm(st))
   {
      /* ie, regular f77 dummy argument,expect pointer TY      */
      /* To counteract the Fortran call-by-reference semantics */

      ASSERT_DBG_FATAL(TY_Is_Pointer(ty_rt), 
		       (DIAG_W2F_UNEXPECTED_TYPE_KIND, 
			TY_kind(ty_rt), "ST2F_decl_var"));
      Append_Token_String(decl_tokens, st_name);
      if (TY_kind(TY_pointed(ST_type(st))) == KIND_FUNCTION)
      {
	 Prepend_Token_String(decl_tokens, "EXTERNAL");
      }
      else
      {
	 TY_IDX ty;
	 TY_IDX ty1 = TY_pointed(ty_rt);

	 if (TY_Is_Pointer(ty1) && TY_ptr_as_array(Ty_Table[ty1]))
	 {
	    /* Handle ptr as array parameters
	     */
	    ty = Stab_Array_Of(TY_pointed(ty1), 0/*size*/);
	 }
	 else
	 {
	    ty = TY_pointed(ty_rt);
	 }
	 if (Use_Purple_Array_Bnds_Placeholder && TY_Is_Array(ty))
	    TY2F_Translate_Purple_Array(decl_tokens, st, ty);
	 else {
	    TY2F_translate(decl_tokens, ty);
         } 
      }
   }
   else if (ST2F_Is_Dummy_Procedure(st))
   {
       TYLIST tylist_idx = TY_tylist(TY_pointed(ST_type(st)));
       TY_IDX rt = TY_IDX_ZERO;
       if (tylist_idx != (TYLIST) 0)
         rt = TYLIST_type(Tylist_Table[tylist_idx]);

       ST2F_Declare_Return_Type(tokens,rt,ST_name(st));
   }
   else if (ST_sclass(st) == SCLASS_EXTERN &&
	    (strcmp(ST_name(st), "__mp_cur_numthreads") == 0 ||
	     strcmp(ST_name(st), "__mp_sug_numthreads") == 0))
   {
      /* Special case */
      st_name = Concat3_Strings(ST_name(st), "_func", "$");
      Append_Token_String(decl_tokens, st_name);
      TY2F_translate(decl_tokens, ST_type(st));
      Append_F77_Indented_Newline(decl_tokens, 1, NULL/*label*/);
      Append_Token_String(decl_tokens, "EXTERNAL ");
      Append_Token_String(decl_tokens, st_name);
   }
   else
   {
      /* Declare as specified in the symbol table */
      Append_Token_String(decl_tokens, st_name);
      if (Use_Purple_Array_Bnds_Placeholder && TY_Is_Array(ST_type(st)))
	 TY2F_Translate_Purple_Array(decl_tokens, st, ST_type(st));
      else {
	 TY2F_translate(decl_tokens, ST_type(st));
       }
   }
   TY2F_Prepend_Structures(decl_tokens);
   Append_And_Reclaim_Token_List(tokens, &decl_tokens);

   if (ST_is_allocatable(st)) {
       TOKEN_BUFFER decl_tokens=New_Token_Buffer();
       Append_Token_String(decl_tokens,"ALLOCATABLE");
       Append_Token_String(decl_tokens,ST_name(st));
       Append_Token_Special(tokens, '\n');
       Append_F77_Indented_Newline(tokens, 0, NULL);
       Append_And_Reclaim_Token_List(tokens,&decl_tokens); }


   if (ST_is_private(st)) {
       TOKEN_BUFFER decl_tokens=New_Token_Buffer();
       Append_Token_String(decl_tokens,"PRIVATE");
       Append_Token_String(decl_tokens,ST_name(st));
       Append_Token_Special(tokens, '\n');
       Append_F77_Indented_Newline(tokens, 0, NULL);
       Append_And_Reclaim_Token_List(tokens,&decl_tokens); }

   if (ST_is_my_pointer(st)) {
       TOKEN_BUFFER decl_tokens=New_Token_Buffer();
       Append_Token_String(decl_tokens,"POINTER");
       Append_Token_String(decl_tokens,ST_name(st));
       Append_Token_Special(tokens, '\n');
       Append_F77_Indented_Newline(tokens, 0, NULL);
       Append_And_Reclaim_Token_List(tokens,&decl_tokens); }

   if (ST_is_f90_target(st)) {
       TOKEN_BUFFER decl_tokens=New_Token_Buffer();
       Append_Token_String(decl_tokens,"TARGET");
       Append_Token_String(decl_tokens,ST_name(st));
       Append_Token_Special(tokens, '\n');
       Append_F77_Indented_Newline(tokens, 0, NULL);
       Append_And_Reclaim_Token_List(tokens,&decl_tokens); }


   /* Save it's value between calls, if so specified, unless it is
    * an equivalence, in which case it is implicitly SAVE.
    */
   if (!Stab_Is_Equivalence_Block(st) &&
       !ST_is_parameter(st) &&
       (ST_sclass(st) == SCLASS_FSTATIC || 
	ST_sclass(st) == SCLASS_PSTATIC))
   {
      Append_F77_Indented_Newline(tokens, 1, NULL/*label*/);
      Append_Token_String(tokens, "SAVE");
      Append_Token_String(tokens, st_name);
   }

INITPRO:
   /* Generate a DATA statement for initializers */
   if (ST_is_parameter(st)){
       inito = Find_INITO_For_Symbol(st);
       if (inito != (INITO_IDX) 0) {
          TOKEN_BUFFER decl_tokens=New_Token_Buffer();
          PARAMETER2F_translate(decl_tokens,inito);
          Append_F77_Indented_Newline(tokens, 1, NULL);
          Append_And_Reclaim_Token_List(tokens,&decl_tokens); }
       }
     else {
      if (ST_is_initialized(st) && 
       !Stab_No_Linkage(st)  )
//     (!TY_Is_Structured(ST_type(st)) ||  /*structure can be initialized--FMZ*/
//	(Stab_Is_Common_Block(st)      || 
//	Stab_Is_Equivalence_Block(st))) 
       {
          inito = Find_INITO_For_Symbol(st);
          if (inito != (INITO_IDX) 0)
	     INITO2F_translate(Data_Stmt_Tokens, inito);
       }
     }
} /* ST2F_decl_var */

static void 
ST2F_decl_type(TOKEN_BUFFER tokens, ST *st)
{
   const char  *st_name = W2CF_Symtab_Nameof_St(st);
   TOKEN_BUFFER decl_tokens = New_Token_Buffer();
   TY_IDX       ty_rt = ST_type(st);

   ASSERT_DBG_FATAL(ST_sym_class(st)==CLASS_TYPE, 
		    (DIAG_W2F_UNEXPECTED_SYMCLASS, 
		     ST_sym_class(st), "ST2F_decl_type"));

   if (Current_scope > GLOBAL_SYMTAB) 
       ASSERT_DBG_FATAL(!PUINFO_RETURN_TO_PARAM || st != PUINFO_RETURN_PARAM, 
		       (DIAG_W2F_DECLARE_RETURN_PARAM, "ST2F_decl_var"));

      if (Use_Purple_Array_Bnds_Placeholder && TY_Is_Array(ST_type(st)))
	 TY2F_Translate_Purple_Array(decl_tokens, st, ST_type(st));
      else {
	 TY2F_translate(decl_tokens, ST_type(st),1);
       }
   TY2F_Prepend_Structures(decl_tokens);
   Append_And_Reclaim_Token_List(tokens, &decl_tokens);

} /* ST2F_decl_type */

static void
ST2F_decl_parameter(TOKEN_BUFFER tokens, ST *st)
{
  const char	*st_name = W2CF_Symtab_Nameof_St(st);
  TOKEN_BUFFER	decl_tokens = New_Token_Buffer();
  TY_IDX	ty_rt = ST_type(st);
  ST 		*base = ST_base(st);
  
  
  Append_Token_String(decl_tokens,st_name);
  if (Use_Purple_Array_Bnds_Placeholder && TY_Is_Array(ST_type(st)))
     TY2F_Translate_Purple_Array(decl_tokens, st, ST_type(st));
  else
     TY2F_translate(decl_tokens, ST_type(st));
  TY2F_Prepend_Structures(decl_tokens);
  Append_And_Reclaim_Token_List(tokens, &decl_tokens);
/*other attributes that are allowed with the PARAMETER attribute are:
 *	DIMENSION
 *	PRIVATE
 *	PUBLIC
 *	SAVE
 */

  if (ST_is_private(st)) {
       decl_tokens=New_Token_Buffer();
       Append_Token_String(decl_tokens,"PRIVATE");
       Append_Token_String(decl_tokens,ST_name(st));
       Append_Token_Special(tokens, '\n');
       Append_F77_Indented_Newline(tokens, 0, NULL);
       Append_And_Reclaim_Token_List(tokens,&decl_tokens); }

/* print out para_name = (value) */

    decl_tokens=New_Token_Buffer();
    Append_Token_String(decl_tokens,"PARAMETER (");
    Append_Token_String(decl_tokens,st_name);
    Append_Token_Special(decl_tokens, '=' );
    TCON2F_translate(decl_tokens,STC_val(base),TY_is_logical(ST_type(st)));
    Append_Token_Special(decl_tokens, ')'); 
    
    Append_Token_Special(tokens, '\n');
    Append_F77_Indented_Newline(tokens, 0, NULL);
    Append_And_Reclaim_Token_List(tokens,&decl_tokens); 

} /* ST_decl_parameter */

static void 
ST2F_decl_func(TOKEN_BUFFER tokens, ST *st)
{
   /* This only makes sense for "external" functions in Fortran,
    * while we should not do anything for other functions.
    */
   ASSERT_DBG_FATAL(ST_sym_class(st)==CLASS_FUNC,
		    (DIAG_W2F_UNEXPECTED_SYMCLASS, 
		     ST_sym_class(st), "ST2F_decl_func"));
   
   /* if f90 internal procedure, don't declare it */

   if (ST_export(st) == EXPORT_LOCAL_INTERNAL)
     return ;

   const char  *func_name = W2CF_Symtab_Nameof_St(st);
   TY_IDX       return_ty;


   /* Specify whether or not the function is EXTERNAL */

   if ((ST_sclass(st) == SCLASS_EXTERN) &&
      (strcmp(ST_name(st),"_ALLOCATE")!=0) &&
      (strcmp(ST_name(st),"_END")!=0) &&
      (strcmp(ST_name(st),"_DEALLOCATE") !=0)&&
      (strcmp(ST_name(st),"_CLOSE") !=0 )    &&
      (strcmp(ST_name(st),"_OPEN")!=0   ))
   {
     if(strncmp("_",func_name,1)!=0) { /*don't declare function name begin with "_" as external*/
        Append_Token_String(tokens, "EXTERNAL");
        Append_Token_String(tokens, func_name);
      }
   }

   /* Specify the function return type, unless it is void */

   return_ty = W2X_Unparse_Target->Func_Return_Type(ST_pu_type(st));
  if (strncmp("_",func_name,1)!=0)
       ST2F_Declare_Return_Type(tokens,return_ty,func_name);

} /* ST2F_decl_func */

static void 
ST2F_decl_const(TOKEN_BUFFER tokens, ST *st)
{
   /* A CLASS_CONST symbol never has a name, and as such cannot be
    * declared!
    */
   ASSERT_DBG_FATAL(FALSE, 
		    (DIAG_W2F_UNEXPECTED_SYMCLASS, 
		     ST_sym_class(st), "ST2F_decl_const"));
} /* ST2F_decl_const */


/*---------------- hidden routines to handle ST uses ------------------*/
/*---------------------------------------------------------------------*/

static void 
ST2F_use_error(TOKEN_BUFFER tokens, ST *st)
{
   ASSERT_DBG_FATAL(FALSE, 
		    (DIAG_W2F_UNEXPECTED_SYMCLASS,
		     ST_sym_class(st), "ST2F_use_error"));
} /* ST2F_use_error */

bool haveCommonBlockName(ST *st) { 
  static set<std::string> nameSet;
  if (st==NULL) { 
    nameSet.clear();
    return false;
  }
  for (set<std::string>::iterator it=nameSet.begin(); it!=nameSet.end(); ++it){ 
    if (*it==ST_name(st)) { 
      return true;
    }
  }
  nameSet.insert(ST_name(st));
  return false; 
}

static void 
ST2F_use_var(TOKEN_BUFFER tokens, ST *st)
{
   TY_IDX return_ty;

   ASSERT_DBG_FATAL(ST_sym_class(st)==CLASS_VAR, 
		    (DIAG_W2F_UNEXPECTED_SYMCLASS, 
		     ST_sym_class(st), "ST2F_use_var"));

   /* Note that we do not trust the ST_is_return_var() flag,
    * unless the return_ty is non-void.  This is due to purple,
    * which may change a function into a subroutine.
    */
   return_ty = PUINFO_RETURN_TY;
   if ((return_ty != (TY_IDX) 0 && 
	TY_kind(return_ty) == KIND_SCALAR && 
	ST_is_return_var(st)) ||    
       (PUINFO_RETURN_TO_PARAM && st == PUINFO_RETURN_PARAM))
   {
      /* If we have a reference to the implicit return-variable, then
       * refer to the function return value.
       */
      Append_Token_String(tokens, PUINFO_FUNC_NAME);
   }
   else if (ST_keep_name_w2f(st))
   {
      /* Use the name as it is (after making it a legal fortran name)
       * and do not mark this variable as having been referenced.
       * Assume this a special symbol not to be declared.
       */
      Append_Token_String(tokens, 
			  WHIRL2F_make_valid_name(ST_name(st),WN2F_F90_pu && !ST_is_temp_var(st)));
      if  (Stab_Is_Based_At_Common_Or_Equivalence(st)) { 
	if (!haveCommonBlockName((ST *)ST_base(st))) {
	  Set_BE_ST_w2fc_referenced((ST *)ST_base(st));
	}
      }
     else
           Set_BE_ST_w2fc_referenced(st); //June
   }
   else if (Stab_Is_Based_At_Common_Or_Equivalence(st))
   {
      /* Reference the corresponding field in the common block (we do this
       * only to ensure that the name referenced matches the one used for
       * the member of the common-block at the place of declaration).  Note
       * that will full splitting, the original common block can be found
       * at ST_full(ST_base(st)).
       */
      WN2F_CONTEXT context = INIT_WN2F_CONTEXT;

      WN2F_Offset_Symref(tokens, 
			 ST_base(st),                         /* base-symbol */
			 Stab_Pointer_To(ST_type(ST_base(st))), /* base-type */
			 ST_type(st),                         /* object-type */
		         ST_ofst(st),                         /* object-ofst */
		         context);
      Set_BE_ST_w2fc_referenced((ST *)ST_base(st));
   }
   else if (ST_sclass(st) == SCLASS_EXTERN &&
	    (strcmp(ST_name(st), "__mp_cur_numthreads") == 0 ||
	     strcmp(ST_name(st), "__mp_sug_numthreads") == 0))
   {
      /* Special case */
      Append_Token_String(tokens, Concat3_Strings(ST_name(st), "_func", "$"));
      Append_Token_Special(tokens, '(');
      Append_Token_Special(tokens, ')');
      Set_BE_ST_w2fc_referenced(st);
   }
   else
   {
      Append_Token_String(tokens, W2CF_Symtab_Nameof_St(st));

      if (strcmp(TY_name(ST_type(st)),".Namelist."))

      Set_BE_ST_w2fc_referenced(st);
   }
} /* ST2F_use_var */


static void 
ST2F_use_func(TOKEN_BUFFER tokens, ST *st)
{
   ASSERT_DBG_FATAL(ST_sym_class(st)==CLASS_FUNC, 
		    (DIAG_W2F_UNEXPECTED_SYMCLASS, 
		     ST_sym_class(st), "ST2F_use_func"));

   Append_Token_String(tokens, W2CF_Symtab_Nameof_St(st));
   Set_BE_ST_w2fc_referenced(st);
}

static void 
ST2F_use_const(TOKEN_BUFFER tokens, ST *st)
{
   TY_IDX ty_idx = ST_type(st);
   TY& ty = Ty_Table[ty_idx];

   ASSERT_DBG_FATAL(ST_sym_class(st)==CLASS_CONST, 
		    (DIAG_W2F_UNEXPECTED_SYMCLASS, 
		     ST_sym_class(st), "ST2F_use_const"));
   
   /* A CLASS_CONST symbol never has a name, so just emit the value.
    */

   if (TY_mtype(ty) == MTYPE_STR && TY_align(ty_idx) > 1)
   {
      /* This must be a hollerith constant */
      TCON2F_hollerith(tokens, STC_val(st));
   }
   else
   {
      TCON2F_translate(tokens, STC_val(st), TY_is_logical(ty));
   }
} /* ST2F_use_const */


static void 
ST2F_use_block(TOKEN_BUFFER tokens, ST *st)
{
  /* with f90 at -O2, CLASS_BLOCK can appear on LDAs etc. in IO */
  /* put out something, so whirlbrowser doesn't fall over       */

   ASSERT_DBG_FATAL(ST_sym_class(st)==CLASS_BLOCK, 
		    (DIAG_W2F_UNEXPECTED_SYMCLASS, 
		     ST_sym_class(st), "ST2F_use_block"));


   Append_Token_String(tokens, ST_name(st));   
} 

/*------------------------ exported routines --------------------------*/
/*---------------------------------------------------------------------*/


void 
ST2F_initialize()
{

  return; 
} /* ST2F_initialize */

void 
ST2F_finalize()
{
  return; 
} 

void 
ST2F_use_translate(TOKEN_BUFFER tokens, ST *st)
{ 
   ST2F_Use_Handler[ST_sym_class(st)](tokens, st);
} 

void 
ST2F_deref_translate(TOKEN_BUFFER tokens, ST *st)
{
   ASSERT_DBG_FATAL(ST_sym_class(st)==CLASS_VAR && 
		    TY_Is_Pointer(ST_type(st)) &&
		    !Stab_Is_Based_At_Common_Or_Equivalence(st), 
		    (DIAG_W2F_UNEXPECTED_SYMCLASS, 
		     ST_sym_class(st), "ST2F_deref_translate"));

   /* Consider this a reference to the pointer value */
   Append_Token_String(tokens, W2CF_Symtab_Nameof_St_Pointee(st));
   Set_BE_ST_w2fc_referenced(st);

} /* ST2F_deref_translate */


// dispatch handler for declarations..
void 
ST2F_decl_translate(TOKEN_BUFFER tokens, const ST *st)
{ 
   ST2F_Decl_Handler[ST_sym_class(st)](tokens, (ST *) st);
} 

static void
collectst(WN *wn,PARMSET &tempset)
 {

   if (!wn) return;

   if (WN_opc_operator(wn) == OPR_LDID ||
       WN_opc_operator(wn) == OPR_LDA)
      tempset.insert(WN_st_idx(wn));
   else
     for (INT32 kidnum = 0; kidnum < WN_kid_count(wn); kidnum++)
       collectst(WN_kid(wn, kidnum),tempset);
   return;
 }


static void GetStSet(ST_IDX bnd,PARMSET &tempset)
{
   WN * stmt;
   WN *first_stmt = WN_first(PU_Body);
   WN kid;

   stmt = first_stmt;
   while ((stmt !=NULL)&&((WN_operator(stmt)!=OPR_STID)
                           ||(WN_operator(stmt) ==OPR_STID)
                           &&strcmp(ST_name(WN_st(stmt)),ST_name(ST_ptr(bnd)))))

       stmt = WN_next(stmt);

  if (stmt && WN_kid(stmt,0))
     collectst(WN_kid(stmt,0),tempset);
}

void ReorderParms(ST **parms,INT32 num_params)
{
  INT32 i;
  ST **reorder_parms;
  ST_IDX bdindex;
  TY_IDX ty_index;
  ST_IDX real_index;
  PARMSET::iterator runner;

  vector<PARMSET> dependset(num_params);
  map<ST_IDX,int>  st_idx_to_parms;
  PARMSET  workset, tempst;

  workset.clear();
  reorder_parms = (ST **)alloca((num_params + 1) * sizeof(ST *));
  for (i=0; i<num_params; i++)
      st_idx_to_parms[(ST_IDX)(parms[i]->st_idx)] = i;

  for (i=0; i<num_params; i++)
   if (TY_kind(ST_type(parms[i])) == KIND_POINTER ){
        ty_index = TY_pointed(ST_type(parms[i]));

        if ((TY_kind(ty_index) == KIND_ARRAY) &&
             !TY_is_character(ty_index) &&
              !TY_is_f90_deferred_shape(ty_index)){

          TY& ty = Ty_Table[ty_index];
          ARB_HANDLE  arb_base = TY_arb(ty);
          ARB_HANDLE  arb;
          INT32       dim = ARB_dimension(arb_base) ;
          while (dim > 0){
             arb = arb_base[dim-1];
             if (ARB_const_lbnd(arb)&& ARB_const_ubnd(arb))
                     ;
             else {
              workset.insert(i);
              if (!ARB_const_lbnd(arb) && !ARB_empty_lbnd(arb)){
                 bdindex = ARB_lbnd_var(arb);
                 if (ST_is_temp_var(St_Table[bdindex])){
                     GetStSet(bdindex,tempst);
                     runner = tempst.begin();
                     while (runner != tempst.end()){
                     if (st_idx_to_parms[*runner]!=i)
                         dependset[i].insert(st_idx_to_parms[*runner]);
                      ++runner;
                    }
                  }
                 }

              if (!ARB_const_ubnd(arb) && !ARB_empty_ubnd(arb)){
                 bdindex = ARB_ubnd_var(arb);
                 if (ST_is_temp_var(St_Table[bdindex])){
                     GetStSet(bdindex,tempst);
                     runner = tempst.begin();
                     while (runner != tempst.end()){
                     if (st_idx_to_parms[*runner]!=i)
                         dependset[i].insert(st_idx_to_parms[*runner]);
                      ++runner;
                    }
                  }
                 }
               }
            dim--;
          }/*while*/
      }
   }
  INT32 keep = 0;

  for (i = 0; i<num_params; i++){
   if (dependset[i].empty()){
     workset.erase(i);
     reorder_parms[keep] = parms[i];
     keep++;
     for (INT32 j=0; j<num_params; j++){
           dependset[j].erase(i);
     } 
    }
  }

  PARMSET::iterator cleaner;
  vector<int> elems;
 
  if (!workset.empty())
   {
    runner = workset.begin();
    while (runner != workset.end()) {
     if (dependset[*runner].empty()){
        reorder_parms[keep] = parms[*runner];
        keep++;
        cleaner = workset.begin();
        while(cleaner !=workset.end()){
           dependset[*cleaner].erase(*runner);
           ++cleaner;
        }
         elems.push_back(*runner);
     }
     ++runner;
   }
 }

 while (!elems.empty())
  {
    INT32 i = elems.back();
    workset.erase(i);
    elems.pop_back();
  }

//tempory for interface has temp variable but there is no assginment
// statement kept in the interface block  
  if (!workset.empty()){
   runner = workset.begin();
   while (runner != workset.end()){
      reorder_parms[keep] = parms[*runner];
      runner++;
      keep++;
   }
  }

  for(INT32 k=0; k<num_params; k++)
      parms[k] = reorder_parms[k];
  return;
}

void
ST2F_func_header(TOKEN_BUFFER tokens,
		 ST          *st,          /* Function ST entry     */
		 ST         **params,      /* Array of  parameters  */
		 INT32        num_params,  /* # of parms in array   */
		 BOOL         is_altentry) /* Alternate entry point */
{
   /* Emit the header for a function definition or an alternate entry
    * point.  Note that the resultant token buffer will not have 
    * appended a newline after the function header.
    */
   TOKEN_BUFFER header_tokens = New_Token_Buffer();
   INT          param, first_param, implicit_parms = 0;
   TY_IDX       funtype = ST_pu_type(st);
   TY_IDX       return_ty;
   WN *wn;   
   WN *stmt;
   ST *rslt = NULL;
   BOOL needcom=1;
   BOOL has_result = 0;
   BOOL add_rsl_type = 0;
   BOOL is_module_program_unit = FALSE;

   const char * func_n_name= W2CF_Symtab_Nameof_St(st);

   ASSERT_DBG_FATAL(TY_kind(funtype) == KIND_FUNCTION,
		    (DIAG_W2F_UNEXPECTED_SYMBOL, "ST2F_func_header"));

   return_ty = W2X_Unparse_Target->Func_Return_Type(funtype);

   /* Append the function name */

   Append_Token_String(header_tokens, W2CF_Symtab_Nameof_St(st));

   /* Emit the parameter name-list, if one is present, and skip any
    * implicit "length" parameters associated with character strings.
    * Such implicit parameters should be at the end of the parameter list.
    */

   first_param = ST2F_FIRST_PARAM_IDX(funtype);

   if (params[first_param] != NULL)
   {
      Append_Token_Special(header_tokens, '(');
      for (param = first_param; 
	   param < num_params - implicit_parms; 
	   param++)
      {
	 if (!ST_is_return_var(params[param]))  
	         Append_Token_String(header_tokens, 
			      W2CF_Symtab_Nameof_St(params[param]));
         else {
              rslt = params[param];
              needcom = 0;
              }
 
	 if (STAB_PARAM_HAS_IMPLICIT_LENGTH(params[param])) 
	 {
	    implicit_parms++;

	    /* is function returning character_TY? if length follows    */
	    /* address - skip over it, but account for ',' in arg list  */

	    if ((param == first_param) && (params[param+1] != NULL)) 
	    {
	      if (ST_is_value_parm(params[param]) && ST_is_value_parm(params[param+1])) 
	      {
		if (return_ty != (TY_IDX) 0 && TY_kind(return_ty) == KIND_VOID ) 
		{
		  param ++ ;
		  params[param] = NULL; 
		  implicit_parms--;
		}
	      }
            }
	 }

	 if (param+implicit_parms+1 < num_params && needcom)
	           Append_Token_Special(header_tokens, ',');
         needcom = 1;
    }
      Append_Token_Special(header_tokens, ')');
   }
   else if (!PU_is_mainpu(Get_Current_PU()) &&
             !ST_is_in_module(st) &&
             !ST_is_block_data(st) ||
             TY_kind(return_ty) != KIND_VOID)   // module&&block data cannot have "()" 

   {
      /* Use the "()" notation for "no parameters", except for
       * the main program definition.
       */
      Append_Token_Special(header_tokens, '(');
      Append_Token_Special(header_tokens, ')');
   }

/* need to see if the result variable has same name with the function's 
 * name,if it does,don't declare the result variable
 */
  
    if (rslt !=NULL       && 
         strcasecmp(W2CF_Symtab_Nameof_St(rslt),W2CF_Symtab_Nameof_St(st))) { 
        has_result = 1;
        Append_Token_String(header_tokens,"result(");
        Append_Token_String(header_tokens,
                             W2CF_Symtab_Nameof_St(rslt));
        Append_Token_Special(header_tokens, ')');
     }
   
   /* Prepend one of the keywords ENTRY, PROGRAM, FUNCTION, or
    * SUBROUTINE to the function name, as is appropriate.
    */

   if (PU_is_mainpu(Get_Current_PU()))
   {
      Prepend_Token_String(header_tokens, "PROGRAM");
   }
   else if (return_ty != (TY_IDX) 0 && TY_kind(return_ty) != KIND_VOID)
   {
     if (is_altentry)
       Prepend_Token_String(header_tokens, "ENTRY");
     else
     {
       Prepend_Token_String(header_tokens, "Function");

     if (PU_recursive(Get_Current_PU())) 
       Prepend_Token_String(header_tokens, "RECURSIVE");
       
      if (!has_result && (TY_kind(return_ty)!= KIND_ARRAY ||
	                        !TY_is_character(TY_AR_etype(return_ty))))
              add_rsl_type=1;
      }
   }
   else /* subroutine */
   {
      if (is_altentry)
	 Prepend_Token_String(header_tokens, "ENTRY");
      else
      if (ST_is_in_module(st) && !PU_is_nested_func(Pu_Table[ST_pu(st)])){
         Prepend_Token_String(header_tokens, "MODULE");  
         is_module_program_unit = TRUE;
       }
      else
      if (ST_is_block_data(st))
         Prepend_Token_String(header_tokens, "BLOCK DATA");
      else { 
	 Prepend_Token_String(header_tokens, "SUBROUTINE");
         if (PU_recursive(Get_Current_PU()))
           Prepend_Token_String(header_tokens, "RECURSIVE");
      }
   }

  
    wn=PU_Body;
    stmt = WN_first(wn);
    int k;
    const char *st_name;
    const char *st_name1;

/* add a use stmt corresponding to an added module in *.w2f.f
 * to solve the real kind problems
 * if the block is alter entry,do nothing.
 *--------fzhao
 */
   if (!is_altentry) {
     Append_F77_Indented_Newline(header_tokens, 1/*empty-lines*/, NULL/*label*/);
     Append_Token_String(header_tokens, "use w2f__types");
    }

    while (stmt) {
      if (WN_operator(stmt)==OPR_USE){
       st_name = W2CF_Symtab_Nameof_St(WN_st(stmt));
       Append_F77_Indented_Newline(header_tokens, 1/*empty-lines*/, NULL/*label*/);
       Append_Token_String(header_tokens, "use");
       Append_Token_String(header_tokens, st_name);
       if (WN_rtype(stmt) == MTYPE_B) // signals presence of the ONLY predicate
	 Append_Token_String(header_tokens, ",only:");
       else { 
	 if ( WN_kid_count(stmt) ) { 
	   Append_Token_String(header_tokens, ",");
	 }
       }
       
       for(k=0;k< WN_kid_count(stmt);k=k+2 ) {
	 
	 st_name = W2CF_Symtab_Nameof_St(WN_st(WN_kid(stmt,k)));
	 st_name1= W2CF_Symtab_Nameof_St(WN_st(WN_kid(stmt,k+1)));
	 if (k==0)
	   ;
	 else
	   Append_Token_String(header_tokens,",");
	 if (strcmp(st_name,st_name1)) {
	   Append_Token_String(header_tokens,st_name);
	   Append_Token_String(header_tokens,"=>");
	   Append_Token_String(header_tokens, st_name1);
	 } 
	 else
	   Append_Token_String(header_tokens,st_name);
       } 
      }
      stmt = WN_next(stmt);
    }  /*while*/

   if (num_params)
       ReorderParms(params,num_params-implicit_parms);
   param_tokens = New_Token_Buffer();

   if (!is_altentry)
   {
      /* Emit parameter declarations, indented and on a new line */
      Append_F77_Indented_Newline(header_tokens, 1, NULL/*label*/);
      Append_Token_String(header_tokens, "IMPLICIT NONE");

      if (is_module_program_unit){
          Append_F77_Indented_Newline(header_tokens, 1, NULL/*label*/);
          Append_Token_String(header_tokens, "SAVE");
          is_module_program_unit = FALSE;
        } 

      for (param = first_param; param < num_params -implicit_parms; param++) {

	 Append_F77_Indented_Newline(param_tokens, 1, NULL/*label*/);
	 if (params[param] )  {
            if (strcasecmp(W2CF_Symtab_Nameof_St(params[param]),W2CF_Symtab_Nameof_St(st))) {

	      ST2F_decl_translate(param_tokens, params[param]);

             if (ST_is_optional_argument( params[param])) {
                Append_F77_Indented_Newline(param_tokens, 1, NULL/*label*/);
                Append_Token_String(param_tokens,"OPTIONAL ");
                Append_Token_String(param_tokens,
                              W2CF_Symtab_Nameof_St(params[param]));
             }
             if (ST_is_intent_in_argument( params[param])) {
                TOKEN_BUFFER temp_tokens = New_Token_Buffer();
                Append_F77_Indented_Newline(temp_tokens, 1, NULL/*label*/);
                Append_Token_String(temp_tokens,"INTENT(IN) ");
                Append_Token_String(temp_tokens,
                              W2CF_Symtab_Nameof_St(params[param]));
                Append_And_Reclaim_Token_List(param_tokens, &temp_tokens);

              }
             if (ST_is_intent_out_argument( params[param])) {
                 Append_F77_Indented_Newline(param_tokens, 1, NULL/*label*/);
                 Append_Token_String(param_tokens,"INTENT(OUT) ");
                 Append_Token_String(param_tokens,
                              W2CF_Symtab_Nameof_St(params[param]));
             }

           } 
        else
             if (!strcasecmp(W2CF_Symtab_Nameof_St(rslt),W2CF_Symtab_Nameof_St(st)))
                     ST2F_decl_translate(param_tokens, params[param]);
       }
   }

    }
    
    if (add_rsl_type){
      TOKEN_BUFFER temp_tokens = New_Token_Buffer();
       Append_F77_Indented_Newline(param_tokens, 1, NULL/*label*/);
       if (TY_Is_Pointer(return_ty))
           TY2F_translate(temp_tokens, Stab_Mtype_To_Ty(TY_mtype(return_ty)));
       else {
           if (TY_kind(return_ty)==KIND_ARRAY)  {
                  if (TY_is_character(TY_AR_etype(return_ty)))
                             ;
                   else
  	   		 TY2F_translate(temp_tokens,TY_AR_etype(return_ty));
                  }
           else
                TY2F_translate(temp_tokens, return_ty);
            }
       Append_Token_String(temp_tokens, W2CF_Symtab_Nameof_St(st));
       Append_And_Reclaim_Token_List(param_tokens, &temp_tokens);
    }

   Append_Token_Special(tokens, '\n');
   Append_F77_Indented_Newline(tokens, 0, NULL);
   Append_And_Reclaim_Token_List(tokens, &header_tokens);

} /* ST2F_func_header */

void
ST2F_Use_Preg(TOKEN_BUFFER tokens,
	      TY_IDX       preg_ty,
	      PREG_IDX     preg_idx)
{
   /* Append the name of the preg to the token-list and declare the
    * preg in the current PU context unless it is already declared.
    */
   const char *preg_name;

   preg_ty = PUinfo_Preg_Type(preg_ty, preg_idx);
   preg_name = W2CF_Symtab_Nameof_Preg(preg_ty, preg_idx);

   /* Declare the preg, if it has not already been declared */
   if (!PUinfo_Is_Preg_Declared(preg_ty, preg_idx))
   {
      ST2F_Define_Preg(preg_name, preg_ty);
      PUinfo_Set_Preg_Declared(preg_ty, preg_idx);
   }

   Append_Token_String(tokens, preg_name);
} /* ST2F_Use_Preg */

void 
ST2F_Declare_Tempvar(TY_IDX ty, UINT idx)
{
   TOKEN_BUFFER tmp_tokens = New_Token_Buffer();
   UINT         current_indent = Current_Indentation();

   Set_Current_Indentation(PUinfo_local_decls_indent);
   Append_F77_Indented_Newline(PUinfo_local_decls, 1, NULL/*label*/);
   if (TY_Is_Pointer(ty))
   {
      /* Assume we never need to dereference the pointer, or else we
       * need to maintain a map from tmp_idx->pointee_idx (new temporary
       * for pointee_idx), so declare this temporary variable to be of
       * an integral type suitable for a pointer value.
       */
      ty = Stab_Mtype_To_Ty(Pointer_Mtype);
   }
   Append_Token_String(tmp_tokens, W2CF_Symtab_Nameof_Tempvar(idx)); /* name */
   TY2F_translate(tmp_tokens, ty);                                   /* type */
  if (ST_is_in_module(Scope_tab[Current_scope].st) &&
      !PU_is_nested_func(Pu_Table[ST_pu(Scope_tab[Current_scope].st)]))
     {
       Append_F77_Indented_Newline(tmp_tokens, 1, NULL/*label*/);
       Append_Token_String(tmp_tokens,"PRIVATE ");
       Append_Token_String(tmp_tokens, W2CF_Symtab_Nameof_Tempvar(idx));
     }

   Append_And_Reclaim_Token_List(PUinfo_local_decls, &tmp_tokens);
   Set_Current_Indentation(current_indent);
} /* ST2F_Declare_Tempvar */


static BOOL
ST2F_Is_Dummy_Procedure(ST *st)
{
  /* Does this ST represent a dummy procedure ? */

  BOOL dummy = FALSE;

  if (ST_sclass(st) == SCLASS_FORMAL && ST_is_value_parm(st))
  {
      TY_IDX ty = ST_type(st);

      if (TY_kind(ty) == KIND_POINTER)
	if (TY_kind(TY_pointed(ty)) == KIND_FUNCTION)
	  dummy = TRUE ;
  }
  return dummy ;
}


static void
ST2F_Declare_Return_Type(TOKEN_BUFFER tokens,TY_IDX return_ty, const char *name)
{
  /* The TY represents a dummy procedure or a function return type */

  if (return_ty != (TY_IDX) 0) 
  {
    if (TY_kind(return_ty) != KIND_VOID)
    {
	TOKEN_BUFFER decl_tokens = New_Token_Buffer();
	
	Append_F77_Indented_Newline(tokens, 1, NULL/*label*/);
	Append_Token_String(decl_tokens, name);

	/* Use integral type for pointer returns */

	if (TY_Is_Pointer(return_ty))
	  TY2F_translate(decl_tokens, Stab_Mtype_To_Ty(TY_mtype(return_ty)));
	else  {
              TY2F_translate(decl_tokens, return_ty);
         }
	TY2F_Prepend_Structures(decl_tokens);
	Append_And_Reclaim_Token_List(tokens, &decl_tokens);
    }
  }
}

void
ST2F_output_keyword(TOKEN_BUFFER tokens, ST * st)
{
  TCON 		strcon = STC_val(st);
  INT32 	strlen ;
  INT32 	stridx ;
  const char	*strbase;
  char 		*keyword;

  strlen  = Targ_String_Length(strcon);
  strbase = Targ_String_Address(strcon);
  keyword = (char *) alloca(strlen +1);
  for (stridx = 0; stridx<strlen;stridx++)
       keyword[stridx] = strbase[stridx];
  keyword[stridx] = '\0';
  Append_Token_String(tokens,keyword);
#if 0 
     TCON2F_trans_to_keyword(tokens, STC_val(st));
#endif
     
}

