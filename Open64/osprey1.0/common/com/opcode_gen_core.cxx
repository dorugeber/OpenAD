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


/* Initialization of operator information */

#include "errors.h"
#include "opcode.h"
#include "config.h"

struct OPERATOR_info_struct OPERATOR_info[OPERATOR_LAST+1] = {

  OPERATOR_info_struct(OPERATOR_UNKNOWN,
		       "UNKNOWN_OPERATOR",
		       0 /* nkids */, 
		       OPERATOR_MAPCAT_HDR /* mapcat */,
		       0),

  OPERATOR_info_struct(OPR_ABS,
		       "OPR_ABS",
		       1 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),  

  OPERATOR_info_struct(OPR_ADD,
		       "OPR_ADD",
		       2 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_AGOTO,
		       "OPR_AGOTO",
		       1 /* nkids */,
		       OPERATOR_MAPCAT_OSTMT /* mapcat */,
		       OPERATOR_PROPERTY_stmt                 |
		       OPERATOR_PROPERTY_non_scf              |
		       OPERATOR_PROPERTY_endsbb               |
		       OPERATOR_PROPERTY_next_prev),

  OPERATOR_info_struct(OPR_ALTENTRY,
		       "OPR_ALTENTRY",
		       -1 /* nkids */,
		       OPERATOR_MAPCAT_OSTMT /* mapcat */,
		       OPERATOR_PROPERTY_stmt                 |
		       OPERATOR_PROPERTY_non_scf              |
		       OPERATOR_PROPERTY_next_prev            |
		       OPERATOR_PROPERTY_sym),

  OPERATOR_info_struct(OPR_ARRAY,
		       "OPR_ARRAY",
		       -1 /* nkids */,
		       OPERATOR_MAPCAT_ARRAY /* mapcat */,
		       OPERATOR_PROPERTY_expression           |
		       OPERATOR_PROPERTY_ndim                 |
		       OPERATOR_PROPERTY_esize), 

  OPERATOR_info_struct(OPR_ARRAYEXP,
		       "OPR_ARRAYEXP",
		       -1 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */, 
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_ARRSECTION,
		       "OPR_ARRSECTION",
		       -1 /* nkids */,
		       OPERATOR_MAPCAT_ARRAY /* mapcat */,
		       OPERATOR_PROPERTY_expression           |
		       OPERATOR_PROPERTY_ndim                 |
		       OPERATOR_PROPERTY_esize),

  OPERATOR_info_struct(OPR_ASHR,
		       "OPR_ASHR",
		       2 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_ASSERT,
		       "OPR_ASSERT",
		       1 /* nkids */,
		       OPERATOR_MAPCAT_OSTMT /* mapcat */,
		       OPERATOR_PROPERTY_stmt                 |
		       OPERATOR_PROPERTY_next_prev            |
		       OPERATOR_PROPERTY_offset),

  OPERATOR_info_struct(OPR_BACKWARD_BARRIER,
		       "OPR_BACKWARD_BARRIER",
		       -1 /* nkids */,
		       OPERATOR_MAPCAT_OSTMT /* mapcat */,
		       OPERATOR_PROPERTY_stmt                 |
		       OPERATOR_PROPERTY_non_scf              |
		       OPERATOR_PROPERTY_next_prev            |
		       OPERATOR_PROPERTY_barrier),

  OPERATOR_info_struct(OPR_BAND,
		       "OPR_BAND",
		       2 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_BIOR,
		       "OPR_BIOR",
		       2 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_BLOCK,
		       "OPR_BLOCK",
		       -1 /* nkids */,
		       OPERATOR_MAPCAT_SCF /* mapcat */,
		       OPERATOR_PROPERTY_scf                  |
		       OPERATOR_PROPERTY_next_prev),

  OPERATOR_info_struct(OPR_BNOR,
		       "OPR_BNOR",
		       2 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_BNOT,
		       "OPR_BNOT",
		       1 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_BXOR,
		       "OPR_BXOR",
		       2 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_CALL,
		       "OPR_CALL",
		       -1 /* nkids */,
		       OPERATOR_MAPCAT_CALL /* mapcat */,
		       OPERATOR_PROPERTY_stmt                 |
		       OPERATOR_PROPERTY_call                 |
		       OPERATOR_PROPERTY_endsbb               |
		       OPERATOR_PROPERTY_next_prev            |
		       OPERATOR_PROPERTY_sym                  |
		       OPERATOR_PROPERTY_flags),

  OPERATOR_info_struct(OPR_CAND,
		       "OPR_CAND",
		       2 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression           |
		       OPERATOR_PROPERTY_boolean),

  OPERATOR_info_struct(OPR_CASEGOTO,
		       "OPR_CASEGOTO",
		       0 /* nkids */,
		       OPERATOR_MAPCAT_OSTMT /* mapcat */,
		       OPERATOR_PROPERTY_stmt                 |
		       OPERATOR_PROPERTY_leaf                 |
		       OPERATOR_PROPERTY_non_scf              |
		       OPERATOR_PROPERTY_next_prev            |
		       OPERATOR_PROPERTY_label                |
		       OPERATOR_PROPERTY_value),

  OPERATOR_info_struct(OPR_CEIL,
		       "OPR_CEIL",
		       1 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_CIOR,
		       "OPR_CIOR",
		       2 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression           |
		       OPERATOR_PROPERTY_boolean),

  OPERATOR_info_struct(OPR_COMMA,
		       "OPR_COMMA",
		       2 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_COMMENT,
		       "OPR_COMMENT",
		       0 /* nkids */,
		       OPERATOR_MAPCAT_OSTMT /* mapcat */,
		       OPERATOR_PROPERTY_stmt                 |
		       OPERATOR_PROPERTY_leaf                 |
		       OPERATOR_PROPERTY_not_executable       |
		       OPERATOR_PROPERTY_next_prev            |
		       OPERATOR_PROPERTY_sym),

  OPERATOR_info_struct(OPR_COMPGOTO,
		       "OPR_COMPGOTO",
		       -1 /* nkids */,
		       OPERATOR_MAPCAT_SCF /* mapcat */,
		       OPERATOR_PROPERTY_stmt                 |
		       OPERATOR_PROPERTY_non_scf              |
		       OPERATOR_PROPERTY_endsbb               |
		       OPERATOR_PROPERTY_next_prev            |
		       OPERATOR_PROPERTY_num_entries          |
		       OPERATOR_PROPERTY_last_label),

  OPERATOR_info_struct(OPR_PAIR,
		       "OPR_PAIR",
		       2 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_CONST,
		       "OPR_CONST",
		       0 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression           |
		       OPERATOR_PROPERTY_leaf                 |
		       OPERATOR_PROPERTY_sym),

  OPERATOR_info_struct(OPR_CSELECT,
		       "OPR_CSELECT",
		       3 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_CVT,
		       "OPR_CVT",
		       1 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_CVTL,
		       "OPR_CVTL",
		       1 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression           |
		       OPERATOR_PROPERTY_bits),

  OPERATOR_info_struct(OPR_DIV,
		       "OPR_DIV",
		       2 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_DIVREM,
		       "OPR_DIVREM",
		       2 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_DO_LOOP,
		       "OPR_DO_LOOP",
		       -1 /* nkids */,
		       OPERATOR_MAPCAT_SCF /* mapcat */,
		       OPERATOR_PROPERTY_scf                  |
		       OPERATOR_PROPERTY_next_prev),

  OPERATOR_info_struct(OPR_DO_WHILE,
		       "OPR_DO_WHILE",
		       2 /* nkids */,
		       OPERATOR_MAPCAT_SCF /* mapcat */,
		       OPERATOR_PROPERTY_scf                  |
		       OPERATOR_PROPERTY_next_prev),

  OPERATOR_info_struct(OPR_EQ,
		       "OPR_EQ",
		       2 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression           |
		       OPERATOR_PROPERTY_compare              |
		       OPERATOR_PROPERTY_boolean),

  OPERATOR_info_struct(OPR_EVAL,
		       "OPR_EVAL",
		       1 /* nkids */,
		       OPERATOR_MAPCAT_OSTMT /* mapcat */,
		       OPERATOR_PROPERTY_stmt                 |
		       OPERATOR_PROPERTY_next_prev),

  OPERATOR_info_struct(OPR_EXC_SCOPE_BEGIN,
		       "OPR_EXC_SCOPE_BEGIN",
		       -1 /* nkids */,
		       OPERATOR_MAPCAT_OSTMT /* mapcat */,
		       OPERATOR_PROPERTY_stmt                 |
		       OPERATOR_PROPERTY_non_scf              |
		       OPERATOR_PROPERTY_next_prev            |
		       OPERATOR_PROPERTY_offset               |
		       OPERATOR_PROPERTY_ereg_supp),

  OPERATOR_info_struct(OPR_EXC_SCOPE_END,
		       "OPR_EXC_SCOPE_END",
		       -1 /* nkids */,
		       OPERATOR_MAPCAT_OSTMT /* mapcat */,
		       OPERATOR_PROPERTY_stmt                 |
		       OPERATOR_PROPERTY_non_scf              |
		       OPERATOR_PROPERTY_next_prev            |
		       OPERATOR_PROPERTY_offset),

  OPERATOR_info_struct(OPR_FALSEBR,
		       "OPR_FALSEBR",
		       1 /* nkids */,
		       OPERATOR_MAPCAT_OSTMT /* mapcat */,
		       OPERATOR_PROPERTY_stmt                 |
		       OPERATOR_PROPERTY_non_scf              |
		       OPERATOR_PROPERTY_endsbb               |
		       OPERATOR_PROPERTY_next_prev            |
		       OPERATOR_PROPERTY_label),

  OPERATOR_info_struct(OPR_FLOOR,
		       "OPR_FLOOR",
		       1 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_FORWARD_BARRIER,
		       "OPR_FORWARD_BARRIER",
		       -1 /* nkids */,
		       OPERATOR_MAPCAT_OSTMT /* mapcat */,
		       OPERATOR_PROPERTY_stmt                 |
		       OPERATOR_PROPERTY_non_scf              |
		       OPERATOR_PROPERTY_next_prev            |
		       OPERATOR_PROPERTY_barrier),

  OPERATOR_info_struct(OPR_FUNC_ENTRY,
		       "OPR_FUNC_ENTRY",
		       -1 /* nkids */,
		       OPERATOR_MAPCAT_HDR /* mapcat */,
		       OPERATOR_PROPERTY_scf                  |
		       OPERATOR_PROPERTY_next_prev            |
		       OPERATOR_PROPERTY_sym),

  OPERATOR_info_struct(OPR_GE,
		       "OPR_GE",
		       2 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression           |
		       OPERATOR_PROPERTY_compare              |
		       OPERATOR_PROPERTY_boolean),

  OPERATOR_info_struct(OPR_GOTO,
		       "OPR_GOTO",
		       0 /* nkids */,
		       OPERATOR_MAPCAT_OSTMT /* mapcat */,
		       OPERATOR_PROPERTY_stmt                 |
		       OPERATOR_PROPERTY_leaf                 |
		       OPERATOR_PROPERTY_non_scf              |
		       OPERATOR_PROPERTY_endsbb               |
		       OPERATOR_PROPERTY_next_prev            |
		       OPERATOR_PROPERTY_label),

  OPERATOR_info_struct(OPR_GT,
		       "OPR_GT",
		       2 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression           |
		       OPERATOR_PROPERTY_compare              |
		       OPERATOR_PROPERTY_boolean),

  OPERATOR_info_struct(OPR_HIGHMPY,
		       "OPR_HIGHMPY",
		       2 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_HIGHPART,
		       "OPR_HIGHPART",
		       1 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_ICALL,
		       "OPR_ICALL",
		       -1 /* nkids */,
		       OPERATOR_MAPCAT_CALL /* mapcat */,
		       OPERATOR_PROPERTY_stmt                 |
		       OPERATOR_PROPERTY_call                 |
		       OPERATOR_PROPERTY_endsbb               |
		       OPERATOR_PROPERTY_next_prev            |
		       OPERATOR_PROPERTY_flags                |
		       OPERATOR_PROPERTY_1ty),

  OPERATOR_info_struct(OPR_IDNAME,
		       "OPR_IDNAME",
		       0 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression           |
		       OPERATOR_PROPERTY_leaf                 |
		       OPERATOR_PROPERTY_sym                  |
		       OPERATOR_PROPERTY_offset),

  OPERATOR_info_struct(OPR_IF,
		       "OPR_IF",
		       3 /* nkids */,
		       OPERATOR_MAPCAT_SCF /* mapcat */,
		       OPERATOR_PROPERTY_scf                  |
		       OPERATOR_PROPERTY_next_prev),

  OPERATOR_info_struct(OPR_ILDA,
		       "OPR_ILDA",
		       1 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression           |
		       OPERATOR_PROPERTY_offset               |
		       OPERATOR_PROPERTY_1ty		  |
		       OPERATOR_PROPERTY_field_id),

  OPERATOR_info_struct(OPR_ILDBITS,
		       "OPR_ILDBITS",
		       1 /* nkids */,
		       OPERATOR_MAPCAT_LDST /* mapcat */,
		       OPERATOR_PROPERTY_expression           |
		       OPERATOR_PROPERTY_load                 |
		       OPERATOR_PROPERTY_offset               |
		       OPERATOR_PROPERTY_2ty),

  OPERATOR_info_struct(OPR_ILOAD,
		       "OPR_ILOAD",
		       1 /* nkids */,
		       OPERATOR_MAPCAT_LDST /* mapcat */,
		       OPERATOR_PROPERTY_expression           |
		       OPERATOR_PROPERTY_load                 |
		       OPERATOR_PROPERTY_offset               |
		       OPERATOR_PROPERTY_2ty		  |
		       OPERATOR_PROPERTY_field_id),

  OPERATOR_info_struct(OPR_ILOADX,
		       "OPR_ILOADX",
		       2 /* nkids */,
		       OPERATOR_MAPCAT_LDST /* mapcat */,
		       OPERATOR_PROPERTY_expression           |
		       OPERATOR_PROPERTY_load                 |
		       OPERATOR_PROPERTY_2ty),

  OPERATOR_info_struct(OPR_SECONDPART,
		       "OPR_SECONDPART",
		       1 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_INTCONST,
		       "OPR_INTCONST",
		       0 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression           |
		       OPERATOR_PROPERTY_leaf                 |
		       OPERATOR_PROPERTY_value),

  OPERATOR_info_struct(OPR_INTRINSIC_CALL,
		       "OPR_INTRINSIC_CALL",
		       -1 /* nkids */,
		       OPERATOR_MAPCAT_CALL /* mapcat */,
		       OPERATOR_PROPERTY_stmt                 |
		       OPERATOR_PROPERTY_call                 |
		       OPERATOR_PROPERTY_endsbb               |
		       OPERATOR_PROPERTY_next_prev            |
		       OPERATOR_PROPERTY_flags                |
		       OPERATOR_PROPERTY_inumber),

  OPERATOR_info_struct(OPR_INTRINSIC_OP,
		       "OPR_INTRINSIC_OP",
		       -1 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression           |
		       OPERATOR_PROPERTY_flags                |
		       OPERATOR_PROPERTY_inumber),

  OPERATOR_info_struct(OPR_IO,
		       "OPR_IO",
		       -1 /* nkids */,
		       OPERATOR_MAPCAT_OSTMT /* mapcat */,
		       OPERATOR_PROPERTY_stmt                 |
		       OPERATOR_PROPERTY_endsbb               |
		       OPERATOR_PROPERTY_next_prev            |
		       OPERATOR_PROPERTY_flags                |
		       OPERATOR_PROPERTY_inumber),

  OPERATOR_info_struct(OPR_IO_ITEM,
		       "OPR_IO_ITEM",
		       -1 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression           |
		       OPERATOR_PROPERTY_inumber              |
		       OPERATOR_PROPERTY_1ty),

  OPERATOR_info_struct(OPR_ISTBITS,
		       "OPR_ISTBITS",
		       2 /* nkids */,
		       OPERATOR_MAPCAT_LDST /* mapcat */,
		       OPERATOR_PROPERTY_stmt                 |
		       OPERATOR_PROPERTY_store                |
		       OPERATOR_PROPERTY_next_prev            |
		       OPERATOR_PROPERTY_offset               |
		       OPERATOR_PROPERTY_1ty),

  OPERATOR_info_struct(OPR_ISTORE,
		       "OPR_ISTORE",
		       2 /* nkids */,
		       OPERATOR_MAPCAT_LDST /* mapcat */,
		       OPERATOR_PROPERTY_stmt                 |
		       OPERATOR_PROPERTY_store                |
		       OPERATOR_PROPERTY_next_prev            |
		       OPERATOR_PROPERTY_offset               |
		       OPERATOR_PROPERTY_1ty		  |
		       OPERATOR_PROPERTY_field_id),


  OPERATOR_info_struct(OPR_ISTOREX,
		       "OPR_ISTOREX",
		       3 /* nkids */,
		       OPERATOR_MAPCAT_LDST /* mapcat */,
		       OPERATOR_PROPERTY_stmt                 |
		       OPERATOR_PROPERTY_store                |
		       OPERATOR_PROPERTY_next_prev            |
		       OPERATOR_PROPERTY_1ty),

  OPERATOR_info_struct(OPR_LABEL,
		       "OPR_LABEL",
		       -1 /* nkids */,
		       OPERATOR_MAPCAT_OSTMT /* mapcat */,
		       OPERATOR_PROPERTY_stmt                 |
		       OPERATOR_PROPERTY_non_scf              |
		       OPERATOR_PROPERTY_not_executable       |
		       OPERATOR_PROPERTY_next_prev            |
		       OPERATOR_PROPERTY_label                |
		       OPERATOR_PROPERTY_flags),

  OPERATOR_info_struct(OPR_LAND,
		       "OPR_LAND",
		       2 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression           |
		       OPERATOR_PROPERTY_boolean),

  OPERATOR_info_struct(OPR_LDA,
		       "OPR_LDA",
		       0 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression           |
		       OPERATOR_PROPERTY_leaf                 |
		       OPERATOR_PROPERTY_sym                  |
		       OPERATOR_PROPERTY_offset               |
		       OPERATOR_PROPERTY_1ty		  |
		       OPERATOR_PROPERTY_field_id),

  OPERATOR_info_struct(OPR_LDBITS,
		       "OPR_LDBITS",
		       0 /* nkids */,
		       OPERATOR_MAPCAT_LDST /* mapcat */,
		       OPERATOR_PROPERTY_expression           |
		       OPERATOR_PROPERTY_leaf                 |
		       OPERATOR_PROPERTY_load                 |
		       OPERATOR_PROPERTY_sym                  |
		       OPERATOR_PROPERTY_offset               |
		       OPERATOR_PROPERTY_1ty),

  OPERATOR_info_struct(OPR_LDID,
		       "OPR_LDID",
		       0 /* nkids */,
		       OPERATOR_MAPCAT_LDST /* mapcat */,
		       OPERATOR_PROPERTY_expression           |
		       OPERATOR_PROPERTY_leaf                 |
		       OPERATOR_PROPERTY_load                 |
		       OPERATOR_PROPERTY_sym                  |
		       OPERATOR_PROPERTY_offset               |
		       OPERATOR_PROPERTY_1ty		  |
		       OPERATOR_PROPERTY_field_id),

  OPERATOR_info_struct(OPR_LE,
		       "OPR_LE",
		       2 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression           |
		       OPERATOR_PROPERTY_compare              |
		       OPERATOR_PROPERTY_boolean),

  OPERATOR_info_struct(OPR_LIOR,
		       "OPR_LIOR",
		       2 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression           |
		       OPERATOR_PROPERTY_boolean),

  OPERATOR_info_struct(OPR_LNOT,
		       "OPR_LNOT",
		       1 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression           |
		       OPERATOR_PROPERTY_boolean),

  OPERATOR_info_struct(OPR_LOOP_INFO,
		       "OPR_LOOP_INFO",
		       -1 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression           |
		       OPERATOR_PROPERTY_2offsets             |
		       OPERATOR_PROPERTY_flags),

  OPERATOR_info_struct(OPR_LOWPART,
		       "OPR_LOWPART",
		       1 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_LSHR,
		       "OPR_LSHR",
		       2 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_LT,
		       "OPR_LT",
		       2 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression           |
		       OPERATOR_PROPERTY_compare              |
		       OPERATOR_PROPERTY_boolean),

  OPERATOR_info_struct(OPR_MADD,
		       "OPR_MADD",
		       3 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_MAX,
		       "OPR_MAX",
		       2 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_MAXPART,
		       "OPR_MAXPART",
		       1 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_MIN,
		       "OPR_MIN",
		       2 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_MINMAX,
		       "OPR_MINMAX",
		       2 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_MINPART,
		       "OPR_MINPART",
		       1 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_MLOAD,
		       "OPR_MLOAD",
		       2 /* nkids */,
		       OPERATOR_MAPCAT_LDST /* mapcat */,
		       OPERATOR_PROPERTY_expression           |
		       OPERATOR_PROPERTY_load                 |
		       OPERATOR_PROPERTY_offset               |
		       OPERATOR_PROPERTY_1ty		  |
		       OPERATOR_PROPERTY_field_id),

  OPERATOR_info_struct(OPR_MOD,
		       "OPR_MOD",
		       2 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_MPY,
		       "OPR_MPY",
		       2 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_MSTORE,
		       "OPR_MSTORE",
		       3 /* nkids */,
		       OPERATOR_MAPCAT_LDST /* mapcat */,
		       OPERATOR_PROPERTY_stmt                 |
		       OPERATOR_PROPERTY_store                |
		       OPERATOR_PROPERTY_next_prev            |
		       OPERATOR_PROPERTY_offset               |
		       OPERATOR_PROPERTY_1ty		  |
		       OPERATOR_PROPERTY_field_id),

  OPERATOR_info_struct(OPR_MSUB,
		       "OPR_MSUB",
		       3 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_NE,
		       "OPR_NE",
		       2 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression           |
		       OPERATOR_PROPERTY_compare              |
		       OPERATOR_PROPERTY_boolean),

  OPERATOR_info_struct(OPR_NEG,
		       "OPR_NEG",
		       1 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_NMADD,
		       "OPR_NMADD",
		       3 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_NMSUB,
		       "OPR_NMSUB",
		       3 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_OPTPARM,
		       "OPR_OPTPARM",
		       1 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression           |
		       OPERATOR_PROPERTY_not_executable),

  OPERATOR_info_struct(OPR_OPT_CHI,
		       "OPR_OPT_CHI",
		       -1 /* nkids */,
		       OPERATOR_MAPCAT_OSTMT /* mapcat */,
		       OPERATOR_PROPERTY_stmt                 |
		       OPERATOR_PROPERTY_next_prev),

  OPERATOR_info_struct(OPR_OPT_RESERVE2,
		       "OPR_OPT_RESERVE2",
		       -1 /* nkids */,
		       OPERATOR_MAPCAT_OSTMT /* mapcat */,
		       OPERATOR_PROPERTY_stmt                 |
		       OPERATOR_PROPERTY_not_executable),

  OPERATOR_info_struct(OPR_PAREN,
		       "OPR_PAREN",
		       1 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_PARM,
		       "OPR_PARM",
		       1 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression           |
		       OPERATOR_PROPERTY_not_executable       |
		       OPERATOR_PROPERTY_flags                |
		       OPERATOR_PROPERTY_1ty),

  OPERATOR_info_struct(OPR_PICCALL,
		       "OPR_PICCALL",
		       -1 /* nkids */,
		       OPERATOR_MAPCAT_CALL /* mapcat */,
		       OPERATOR_PROPERTY_stmt                 |
		       OPERATOR_PROPERTY_call                 |
		       OPERATOR_PROPERTY_endsbb               |
		       OPERATOR_PROPERTY_next_prev            |
		       OPERATOR_PROPERTY_sym                  |
		       OPERATOR_PROPERTY_flags),

  OPERATOR_info_struct(OPR_PRAGMA,
		       "OPR_PRAGMA",
		       0 /* nkids */,
		       OPERATOR_MAPCAT_PRAGMA /* mapcat */,
		       OPERATOR_PROPERTY_stmt                 |
		       OPERATOR_PROPERTY_leaf                 |
		       OPERATOR_PROPERTY_not_executable       |
		       OPERATOR_PROPERTY_next_prev            |
		       OPERATOR_PROPERTY_sym                  |
		       OPERATOR_PROPERTY_offset               |
		       OPERATOR_PROPERTY_value),

  OPERATOR_info_struct(OPR_PREFETCH,
		       "OPR_PREFETCH",
		       1 /* nkids */,
		       OPERATOR_MAPCAT_OSTMT /* mapcat */,
		       OPERATOR_PROPERTY_stmt                 |
		       OPERATOR_PROPERTY_prefetch             |
		       OPERATOR_PROPERTY_next_prev            |
		       OPERATOR_PROPERTY_offset               |
		       OPERATOR_PROPERTY_flags),

  OPERATOR_info_struct(OPR_PREFETCHX,
		       "OPR_PREFETCHX",
		       2 /* nkids */,
		       OPERATOR_MAPCAT_OSTMT /* mapcat */,
		       OPERATOR_PROPERTY_stmt                 |
		       OPERATOR_PROPERTY_prefetch             |
		       OPERATOR_PROPERTY_next_prev            |
		       OPERATOR_PROPERTY_offset               |
		       OPERATOR_PROPERTY_flags),

  OPERATOR_info_struct(OPR_RCOMMA,
		       "OPR_RCOMMA",
		       2 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_FIRSTPART,
		       "OPR_FIRSTPART",
		       1 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_RECIP,
		       "OPR_RECIP",
		       1 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_REGION,
		       "OPR_REGION",
		       3 /* nkids */,
		       OPERATOR_MAPCAT_HDR /* mapcat */,
		       OPERATOR_PROPERTY_scf                  |
		       OPERATOR_PROPERTY_next_prev            |
		       OPERATOR_PROPERTY_ereg_supp),

  OPERATOR_info_struct(OPR_REGION_EXIT,
		       "OPR_REGION_EXIT",
		       0 /* nkids */,
		       OPERATOR_MAPCAT_OSTMT /* mapcat */,
		       OPERATOR_PROPERTY_stmt                 |
		       OPERATOR_PROPERTY_leaf                 |
		       OPERATOR_PROPERTY_non_scf              |
		       OPERATOR_PROPERTY_endsbb               |
		       OPERATOR_PROPERTY_next_prev            |
		       OPERATOR_PROPERTY_label),

  OPERATOR_info_struct(OPR_REM,
		       "OPR_REM",
		       2 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_RETURN,
		       "OPR_RETURN",
		       0 /* nkids */,
		       OPERATOR_MAPCAT_OSTMT /* mapcat */,
		       OPERATOR_PROPERTY_stmt                 |
		       OPERATOR_PROPERTY_non_scf              |
		       OPERATOR_PROPERTY_endsbb               |
		       OPERATOR_PROPERTY_next_prev),

  OPERATOR_info_struct(OPR_RETURN_VAL,
		       "OPR_RETURN_VAL",
		       1 /* nkids */,
		       OPERATOR_MAPCAT_OSTMT /* mapcat */,
		       OPERATOR_PROPERTY_stmt                 |
		       OPERATOR_PROPERTY_non_scf              |
		       OPERATOR_PROPERTY_endsbb               |
		       OPERATOR_PROPERTY_next_prev),

  OPERATOR_info_struct(OPR_RND,
		       "OPR_RND",
		       1 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_RSQRT,
		       "OPR_RSQRT",
		       1 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_SELECT,
		       "OPR_SELECT",
		       3 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_SHL,
		       "OPR_SHL",
		       2 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_SQRT,
		       "OPR_SQRT",
		       1 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_STBITS,
		       "OPR_STBITS",
		       1 /* nkids */,
		       OPERATOR_MAPCAT_LDST /* mapcat */,
		       OPERATOR_PROPERTY_stmt                 |
		       OPERATOR_PROPERTY_store                |
		       OPERATOR_PROPERTY_next_prev            |
		       OPERATOR_PROPERTY_sym                  |
		       OPERATOR_PROPERTY_offset               |
		       OPERATOR_PROPERTY_1ty),

  OPERATOR_info_struct(OPR_STID,
		       "OPR_STID",
		       1 /* nkids */,
		       OPERATOR_MAPCAT_LDST /* mapcat */,
		       OPERATOR_PROPERTY_stmt                 |
		       OPERATOR_PROPERTY_store                |
		       OPERATOR_PROPERTY_next_prev            |
		       OPERATOR_PROPERTY_sym                  |
		       OPERATOR_PROPERTY_offset               |
		       OPERATOR_PROPERTY_1ty		  |
		       OPERATOR_PROPERTY_field_id),

  OPERATOR_info_struct(OPR_SUB,
		       "OPR_SUB",
		       2 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_SWITCH,
		       "OPR_SWITCH",
		       -1 /* nkids */,
		       OPERATOR_MAPCAT_SCF /* mapcat */,
		       OPERATOR_PROPERTY_stmt                 |
		       OPERATOR_PROPERTY_non_scf              |
		       OPERATOR_PROPERTY_endsbb               |
		       OPERATOR_PROPERTY_next_prev            |
		       OPERATOR_PROPERTY_num_entries          |
		       OPERATOR_PROPERTY_last_label),

  OPERATOR_info_struct(OPR_TAS,
		       "OPR_TAS",
		       1 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression           |
		       OPERATOR_PROPERTY_1ty),

  OPERATOR_info_struct(OPR_TRAP,
		       "OPR_TRAP",
		       0 /* nkids */,
		       OPERATOR_MAPCAT_OSTMT /* mapcat */,
		       OPERATOR_PROPERTY_stmt                 |
		       OPERATOR_PROPERTY_next_prev            |
		       OPERATOR_PROPERTY_offset),

  OPERATOR_info_struct(OPR_TRIPLET,
		       "OPR_TRIPLET",
		       3 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_TRUEBR,
		       "OPR_TRUEBR",
		       1 /* nkids */,
		       OPERATOR_MAPCAT_OSTMT /* mapcat */,
		       OPERATOR_PROPERTY_stmt                 |
		       OPERATOR_PROPERTY_non_scf              |
		       OPERATOR_PROPERTY_endsbb               |
		       OPERATOR_PROPERTY_next_prev            |
		       OPERATOR_PROPERTY_label),

  OPERATOR_info_struct(OPR_TRUNC,
		       "OPR_TRUNC",
		       1 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_VFCALL,
		       "OPR_VFCALL",
		       -1 /* nkids */,
		       OPERATOR_MAPCAT_CALL /* mapcat */,
		       OPERATOR_PROPERTY_stmt                 |
		       OPERATOR_PROPERTY_call                 |
		       OPERATOR_PROPERTY_endsbb               |
		       OPERATOR_PROPERTY_next_prev            |
		       OPERATOR_PROPERTY_flags                |
		       OPERATOR_PROPERTY_1ty),

  OPERATOR_info_struct(OPR_WHERE,
		       "OPR_WHERE",
		       3 /* nkids */,
		       OPERATOR_MAPCAT_SCF /* mapcat */,
		       OPERATOR_PROPERTY_scf                  |
		       OPERATOR_PROPERTY_next_prev),

  OPERATOR_info_struct(OPR_WHILE_DO,
		       "OPR_WHILE_DO",
		       2 /* nkids */,
		       OPERATOR_MAPCAT_SCF /* mapcat */,
		       OPERATOR_PROPERTY_scf                  |
		       OPERATOR_PROPERTY_next_prev),

  OPERATOR_info_struct(OPR_XGOTO,
		       "OPR_XGOTO",
		       2 /* nkids */,
		       OPERATOR_MAPCAT_SCF /* mapcat */,
		       OPERATOR_PROPERTY_stmt                 |
		       OPERATOR_PROPERTY_non_scf              |
		       OPERATOR_PROPERTY_endsbb               |
		       OPERATOR_PROPERTY_next_prev            |
		       OPERATOR_PROPERTY_sym                  |
		       OPERATOR_PROPERTY_num_entries),

  OPERATOR_info_struct(OPR_XMPY,
		       "OPR_XMPY",
		       2 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_XPRAGMA,
		       "OPR_XPRAGMA",
		       1 /* nkids */,
		       OPERATOR_MAPCAT_PRAGMA /* mapcat */,
		       OPERATOR_PROPERTY_stmt                 |
		       OPERATOR_PROPERTY_not_executable       |
		       OPERATOR_PROPERTY_next_prev            |
		       OPERATOR_PROPERTY_sym                  |
		       OPERATOR_PROPERTY_offset),

  OPERATOR_info_struct(OPR_AFFIRM,
		       "OPR_AFFIRM",
		       1 /* nkids */,
		       OPERATOR_MAPCAT_OSTMT /* mapcat */,
		       OPERATOR_PROPERTY_stmt                 |
		       OPERATOR_PROPERTY_next_prev),

  OPERATOR_info_struct(OPR_ALLOCA,
		       "OPR_ALLOCA",
		       1 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_DEALLOCA,
		       "OPR_DEALLOCA",
		       -1 /* nkids */,
		       OPERATOR_MAPCAT_OSTMT /* mapcat */,
		       OPERATOR_PROPERTY_stmt                 |
		       OPERATOR_PROPERTY_next_prev),

  OPERATOR_info_struct(OPR_LDMA,
		       "OPR_LDMA",
		       0 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression           |
		       OPERATOR_PROPERTY_leaf                 |
		       OPERATOR_PROPERTY_sym                  |
		       OPERATOR_PROPERTY_offset               |
		       OPERATOR_PROPERTY_1ty		  |
		       OPERATOR_PROPERTY_field_id),

  OPERATOR_info_struct(OPR_ASM_STMT,
		       "OPR_ASM_STMT",
		       -1 /* nkids */,
		       OPERATOR_MAPCAT_OSTMT /* mapcat */,
		       OPERATOR_PROPERTY_stmt                 |
		       OPERATOR_PROPERTY_next_prev            |
		       OPERATOR_PROPERTY_sym                  |  /* SCLASS_NAME string symbol */
		       OPERATOR_PROPERTY_flags),

  OPERATOR_info_struct(OPR_ASM_EXPR,
		       "OPR_ASM_EXPR",
		       -1 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression           |
		       OPERATOR_PROPERTY_sym                  |  /* SCLASS_NAME string symbol */
		       OPERATOR_PROPERTY_flags),

  OPERATOR_info_struct(OPR_ASM_INPUT,
		       "OPR_ASM_INPUT",
		       1 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression           |
		       OPERATOR_PROPERTY_sym),		     /* SCLASS_NAME constraint symbol */

  OPERATOR_info_struct(OPR_RROTATE,
		       "OPR_RROTATE",
		       2 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_LDA_LABEL,
		       "OPR_LDA_LABEL",
		       0 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression           |
		       OPERATOR_PROPERTY_leaf                 |
		       OPERATOR_PROPERTY_1ty		  |
		       OPERATOR_PROPERTY_label),

  OPERATOR_info_struct(OPR_GOTO_OUTER_BLOCK,
		       "OPR_GOTO_OUTER_BLOCK",
		       0 /* nkids */,
		       OPERATOR_MAPCAT_OSTMT /* mapcat */,
		       OPERATOR_PROPERTY_stmt                 |
		       OPERATOR_PROPERTY_leaf                 |
		       OPERATOR_PROPERTY_non_scf              |
		       OPERATOR_PROPERTY_endsbb               |
		       OPERATOR_PROPERTY_next_prev            |
		       OPERATOR_PROPERTY_label),

  OPERATOR_info_struct(OPR_EXTRACT_BITS,
		       "OPR_EXTRACT_BITS",
		       1 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_COMPOSE_BITS,
		       "OPR_COMPOSE_BITS",
		       2 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_PSTORE,
		       "OPR_PSTORE",
		       2 /* nkids */,
		       OPERATOR_MAPCAT_LDST /* mapcat */,
		       OPERATOR_PROPERTY_stmt                 |
		       OPERATOR_PROPERTY_store                |
		       OPERATOR_PROPERTY_next_prev            |
		       OPERATOR_PROPERTY_offset               |
		       OPERATOR_PROPERTY_1ty                  |
		       OPERATOR_PROPERTY_field_id),

  OPERATOR_info_struct(OPR_PSTID,
		       "OPR_PSTID",
		       1 /* nkids */,
		       OPERATOR_MAPCAT_LDST /* mapcat */,
		       OPERATOR_PROPERTY_stmt                 |
		       OPERATOR_PROPERTY_store                |
		       OPERATOR_PROPERTY_next_prev            |
		       OPERATOR_PROPERTY_sym                  |
		       OPERATOR_PROPERTY_offset               |
		       OPERATOR_PROPERTY_1ty                  |
		       OPERATOR_PROPERTY_field_id),

  OPERATOR_info_struct(OPR_USE,
		       "OPR_USE",
		       -1 /* nkids */,
		       OPERATOR_MAPCAT_HDR /* mapcat */,
		       OPERATOR_PROPERTY_scf                  |
		       OPERATOR_PROPERTY_next_prev            |
		       OPERATOR_PROPERTY_sym),
 
  OPERATOR_info_struct(OPR_NAMELIST,
		       "OPR_NAMELIST",
		       -1 /* nkids */,
		       OPERATOR_MAPCAT_HDR /* mapcat */,
		       OPERATOR_PROPERTY_scf                  |
		       OPERATOR_PROPERTY_next_prev            |
		       OPERATOR_PROPERTY_sym),

  OPERATOR_info_struct(OPR_IMPLICIT_BND,
		       "OPR_IMPLICIT_BND",
		       0 /* nkids */,
		       OPERATOR_MAPCAT_HDR /* mapcat */,
		       OPERATOR_PROPERTY_scf           |
		       OPERATOR_PROPERTY_leaf                 |
		       OPERATOR_PROPERTY_sym),

  OPERATOR_info_struct(OPR_SRCTRIPLET,
		       "OPR_SRCTRIPLET",
		       3 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_NULLIFY,
		       "OPR_NULLIFY",
		       -1 /* nkids */,
		       OPERATOR_MAPCAT_HDR /* mapcat */,
		       OPERATOR_PROPERTY_scf                  |
		       OPERATOR_PROPERTY_next_prev            |
		       OPERATOR_PROPERTY_sym),


  OPERATOR_info_struct(OPR_INTERFACE,
		       "OPR_INTERFACE",
		       -1 /* nkids */,
		       OPERATOR_MAPCAT_HDR /* mapcat */,
		       OPERATOR_PROPERTY_scf                  |
		       OPERATOR_PROPERTY_next_prev            |
		       OPERATOR_PROPERTY_sym),

  OPERATOR_info_struct(OPR_ARRAY_CONSTRUCT,
		       "OPR_ARRAY_CONSTRUCT",
		       -1 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_IMPLIED_DO,
		       "OPR_IMPLIED_DO",
		       5 /* nkids */,
		       OPERATOR_MAPCAT_OEXP /* mapcat */,
		       OPERATOR_PROPERTY_expression),

  OPERATOR_info_struct(OPR_STRCTFLD,
                       "OPR_STRCTFLD",
                       1 /* nkids */,
                       OPERATOR_MAPCAT_LDST /* mapcat */,
                       OPERATOR_PROPERTY_expression           |
		       OPERATOR_PROPERTY_load                 |
                       OPERATOR_PROPERTY_2ty              |
                       OPERATOR_PROPERTY_field_id),

};

static BOOL
Is_MTYPE_b [MTYPE_LAST+1] = {
  0, /* MTYPE_UNKNOWN */
  1, /* MTYPE_B       */
  0, /* MTYPE_I1      */
  0, /* MTYPE_I2      */
  1, /* MTYPE_I4      */
  1, /* MTYPE_I8      */
  0, /* MTYPE_U1      */
  0, /* MTYPE_U2      */
  1, /* MTYPE_U4      */
  1, /* MTYPE_U8      */
  0, /* MTYPE_F4      */
  0, /* MTYPE_F8      */
  0, /* MTYPE_F10     */
  0, /* MTYPE_F16     */
  0, /* MTYPE_STR     */
  0, /* MTYPE_FQ      */
  0, /* MTYPE_M       */
  0, /* MTYPE_C4      */
  0, /* MTYPE_C8      */
  0, /* MTYPE_CQ      */
  0, /* MTYPE_V       */
  0, /* MTYPE_BS      */
  0, /* MTYPE_A4      */
  0, /* MTYPE_A8      */
  0, /* MTYPE_C10     */
  0, /* MTYPE_C16     */
  0, /* MTYPE_I16     */
  0  /* MTYPE_U16     */
};

static BOOL
Is_MTYPE_b_f_i_M_p_z [MTYPE_LAST+1] = {
  0, /* MTYPE_UNKNOWN */
  1, /* MTYPE_B       */
  0, /* MTYPE_I1      */
  0, /* MTYPE_I2      */
  1, /* MTYPE_I4      */
  1, /* MTYPE_I8      */
  0, /* MTYPE_U1      */
  0, /* MTYPE_U2      */
  1, /* MTYPE_U4      */
  1, /* MTYPE_U8      */
  1, /* MTYPE_F4      */
  1, /* MTYPE_F8      */
  1, /* MTYPE_F10     */
  1, /* MTYPE_F16     */
  0, /* MTYPE_STR     */
  1, /* MTYPE_FQ      */
  1, /* MTYPE_M       */
  1, /* MTYPE_C4      */
  1, /* MTYPE_C8      */
  1, /* MTYPE_CQ      */
  0, /* MTYPE_V       */
  0, /* MTYPE_BS      */
  1, /* MTYPE_A4      */
  1, /* MTYPE_A8      */
  1, /* MTYPE_C10     */
  1, /* MTYPE_C16     */
  1, /* MTYPE_I16     */
  1  /* MTYPE_U16     */
};

static BOOL
Is_MTYPE_b_f_i_M_p_V_z [MTYPE_LAST+1] = {
  0, /* MTYPE_UNKNOWN */
  1, /* MTYPE_B       */
  0, /* MTYPE_I1      */
  0, /* MTYPE_I2      */
  1, /* MTYPE_I4      */
  1, /* MTYPE_I8      */
  0, /* MTYPE_U1      */
  0, /* MTYPE_U2      */
  1, /* MTYPE_U4      */
  1, /* MTYPE_U8      */
  1, /* MTYPE_F4      */
  1, /* MTYPE_F8      */
  1, /* MTYPE_F10     */
  1, /* MTYPE_F16     */
  0, /* MTYPE_STR     */
  1, /* MTYPE_FQ      */
  1, /* MTYPE_M       */
  1, /* MTYPE_C4      */
  1, /* MTYPE_C8      */
  1, /* MTYPE_CQ      */
  1, /* MTYPE_V       */
  0, /* MTYPE_BS      */
  1, /* MTYPE_A4      */
  1, /* MTYPE_A8      */
  1, /* MTYPE_C10     */
  1, /* MTYPE_C16     */
  1, /* MTYPE_I16     */
  1  /* MTYPE_U16     */
};

static BOOL
Is_MTYPE_b_f_i_M_p_s_z [MTYPE_LAST+1] = {
  0, /* MTYPE_UNKNOWN */
  1, /* MTYPE_B       */
  1, /* MTYPE_I1      */
  1, /* MTYPE_I2      */
  1, /* MTYPE_I4      */
  1, /* MTYPE_I8      */
  1, /* MTYPE_U1      */
  1, /* MTYPE_U2      */
  1, /* MTYPE_U4      */
  1, /* MTYPE_U8      */
  1, /* MTYPE_F4      */
  1, /* MTYPE_F8      */
  1, /* MTYPE_F10     */
  1, /* MTYPE_F16     */
  0, /* MTYPE_STR     */
  1, /* MTYPE_FQ      */
  1, /* MTYPE_M       */
  1, /* MTYPE_C4      */
  1, /* MTYPE_C8      */
  1, /* MTYPE_CQ      */
  0, /* MTYPE_V       */
  0, /* MTYPE_BS      */
  1, /* MTYPE_A4      */
  1, /* MTYPE_A8      */
  1, /* MTYPE_C10     */
  1, /* MTYPE_C16     */
  1, /* MTYPE_I16     */
  1  /* MTYPE_U16     */
};

static BOOL
Is_MTYPE_b_f_i_p_z [MTYPE_LAST+1] = {
  0, /* MTYPE_UNKNOWN */
  1, /* MTYPE_B       */
  0, /* MTYPE_I1      */
  0, /* MTYPE_I2      */
  1, /* MTYPE_I4      */
  1, /* MTYPE_I8      */
  0, /* MTYPE_U1      */
  0, /* MTYPE_U2      */
  1, /* MTYPE_U4      */
  1, /* MTYPE_U8      */
  1, /* MTYPE_F4      */
  1, /* MTYPE_F8      */
  1, /* MTYPE_F10     */
  1, /* MTYPE_F16     */
  0, /* MTYPE_STR     */
  1, /* MTYPE_FQ      */
  0, /* MTYPE_M       */
  1, /* MTYPE_C4      */
  1, /* MTYPE_C8      */
  1, /* MTYPE_CQ      */
  0, /* MTYPE_V       */
  0, /* MTYPE_BS      */
  1, /* MTYPE_A4      */
  1, /* MTYPE_A8      */
  1, /* MTYPE_C10     */
  1, /* MTYPE_C16     */
  1, /* MTYPE_I16     */
  1  /* MTYPE_U16     */
};

static BOOL
Is_MTYPE_bs_I1_I2_I4_I8 [MTYPE_LAST+1] = {
  0, /* MTYPE_UNKNOWN */
  0, /* MTYPE_B       */
  1, /* MTYPE_I1      */
  1, /* MTYPE_I2      */
  1, /* MTYPE_I4      */
  1, /* MTYPE_I8      */
  0, /* MTYPE_U1      */
  0, /* MTYPE_U2      */
  0, /* MTYPE_U4      */
  0, /* MTYPE_U8      */
  0, /* MTYPE_F4      */
  0, /* MTYPE_F8      */
  0, /* MTYPE_F10     */
  0, /* MTYPE_F16     */
  0, /* MTYPE_STR     */
  0, /* MTYPE_FQ      */
  0, /* MTYPE_M       */
  0, /* MTYPE_C4      */
  0, /* MTYPE_C8      */
  0, /* MTYPE_CQ      */
  0, /* MTYPE_V       */
  1, /* MTYPE_BS      */
  0, /* MTYPE_A4      */
  0, /* MTYPE_A8      */
  0, /* MTYPE_C10     */
  0, /* MTYPE_C16     */
  0, /* MTYPE_I16     */
  0  /* MTYPE_U16     */
};

static BOOL
Is_MTYPE_bs_U1_U2_U4_U8 [MTYPE_LAST+1] = {
  0, /* MTYPE_UNKNOWN */
  0, /* MTYPE_B       */
  0, /* MTYPE_I1      */
  0, /* MTYPE_I2      */
  0, /* MTYPE_I4      */
  0, /* MTYPE_I8      */
  1, /* MTYPE_U1      */
  1, /* MTYPE_U2      */
  1, /* MTYPE_U4      */
  1, /* MTYPE_U8      */
  0, /* MTYPE_F4      */
  0, /* MTYPE_F8      */
  0, /* MTYPE_F10     */
  0, /* MTYPE_F16     */
  0, /* MTYPE_STR     */
  0, /* MTYPE_FQ      */
  0, /* MTYPE_M       */
  0, /* MTYPE_C4      */
  0, /* MTYPE_C8      */
  0, /* MTYPE_CQ      */
  0, /* MTYPE_V       */
  1, /* MTYPE_BS      */
  0, /* MTYPE_A4      */
  0, /* MTYPE_A8      */
  0, /* MTYPE_C10     */
  0, /* MTYPE_C16     */
  0, /* MTYPE_I16     */
  0  /* MTYPE_U16     */
};

static BOOL
Is_MTYPE_b_bs_f_i_p_s_z [MTYPE_LAST+1] = {
  0, /* MTYPE_UNKNOWN */
  1, /* MTYPE_B       */
  1, /* MTYPE_I1      */
  1, /* MTYPE_I2      */
  1, /* MTYPE_I4      */
  1, /* MTYPE_I8      */
  1, /* MTYPE_U1      */
  1, /* MTYPE_U2      */
  1, /* MTYPE_U4      */
  1, /* MTYPE_U8      */
  1, /* MTYPE_F4      */
  1, /* MTYPE_F8      */
  1, /* MTYPE_F10     */
  1, /* MTYPE_F16     */
  0, /* MTYPE_STR     */
  1, /* MTYPE_FQ      */
  0, /* MTYPE_M       */
  1, /* MTYPE_C4      */
  1, /* MTYPE_C8      */
  1, /* MTYPE_CQ      */
  0, /* MTYPE_V       */
  1, /* MTYPE_BS      */
  1, /* MTYPE_A4      */
  1, /* MTYPE_A8      */
  1, /* MTYPE_C10     */
  1, /* MTYPE_C16     */
  1, /* MTYPE_I16     */
  1  /* MTYPE_U16     */
};

static BOOL
Is_MTYPE_b_bs_f_i_M_p_s_z [MTYPE_LAST+1] = {
  0, /* MTYPE_UNKNOWN */
  1, /* MTYPE_B       */
  1, /* MTYPE_I1      */
  1, /* MTYPE_I2      */
  1, /* MTYPE_I4      */
  1, /* MTYPE_I8      */
  1, /* MTYPE_U1      */
  1, /* MTYPE_U2      */
  1, /* MTYPE_U4      */
  1, /* MTYPE_U8      */
  1, /* MTYPE_F4      */
  1, /* MTYPE_F8      */
  1, /* MTYPE_F10     */
  1, /* MTYPE_F16     */
  0, /* MTYPE_STR     */
  1, /* MTYPE_FQ      */
  1, /* MTYPE_M       */
  1, /* MTYPE_C4      */
  1, /* MTYPE_C8      */
  1, /* MTYPE_CQ      */
  0, /* MTYPE_V       */
  1, /* MTYPE_BS      */
  1, /* MTYPE_A4      */
  1, /* MTYPE_A8      */
  1, /* MTYPE_C10     */
  1, /* MTYPE_C16     */
  1, /* MTYPE_I16     */
  1  /* MTYPE_U16     */
};

static BOOL
Is_MTYPE_f [MTYPE_LAST+1] = {
  0, /* MTYPE_UNKNOWN */
  0, /* MTYPE_B       */
  0, /* MTYPE_I1      */
  0, /* MTYPE_I2      */
  0, /* MTYPE_I4      */
  0, /* MTYPE_I8      */
  0, /* MTYPE_U1      */
  0, /* MTYPE_U2      */
  0, /* MTYPE_U4      */
  0, /* MTYPE_U8      */
  1, /* MTYPE_F4      */
  1, /* MTYPE_F8      */
  1, /* MTYPE_F10     */
  1, /* MTYPE_F16     */
  0, /* MTYPE_STR     */
  1, /* MTYPE_FQ      */
  0, /* MTYPE_M       */
  0, /* MTYPE_C4      */
  0, /* MTYPE_C8      */
  0, /* MTYPE_CQ      */
  0, /* MTYPE_V       */
  0, /* MTYPE_BS      */
  0, /* MTYPE_A4      */
  0, /* MTYPE_A8      */
  0, /* MTYPE_C10     */
  0, /* MTYPE_C16     */
  0, /* MTYPE_I16     */
  0  /* MTYPE_U16     */
};

static BOOL
Is_MTYPE_f_I4_I8_I16 [MTYPE_LAST+1] = {
  0, /* MTYPE_UNKNOWN */
  0, /* MTYPE_B       */
  0, /* MTYPE_I1      */
  0, /* MTYPE_I2      */
  1, /* MTYPE_I4      */
  1, /* MTYPE_I8      */
  0, /* MTYPE_U1      */
  0, /* MTYPE_U2      */
  0, /* MTYPE_U4      */
  0, /* MTYPE_U8      */
  1, /* MTYPE_F4      */
  1, /* MTYPE_F8      */
  1, /* MTYPE_F10     */
  1, /* MTYPE_F16     */
  0, /* MTYPE_STR     */
  1, /* MTYPE_FQ      */
  0, /* MTYPE_M       */
  0, /* MTYPE_C4      */
  0, /* MTYPE_C8      */
  0, /* MTYPE_CQ      */
  0, /* MTYPE_V       */
  0, /* MTYPE_BS      */
  0, /* MTYPE_A4      */
  0, /* MTYPE_A8      */
  0, /* MTYPE_C10     */
  0, /* MTYPE_C16     */
  1, /* MTYPE_I16     */
  0  /* MTYPE_U16     */
};

static BOOL
Is_MTYPE_f_i_M_p_V_z [MTYPE_LAST+1] = {
  0, /* MTYPE_UNKNOWN */
  0, /* MTYPE_B       */
  0, /* MTYPE_I1      */
  0, /* MTYPE_I2      */
  1, /* MTYPE_I4      */
  1, /* MTYPE_I8      */
  0, /* MTYPE_U1      */
  0, /* MTYPE_U2      */
  1, /* MTYPE_U4      */
  1, /* MTYPE_U8      */
  1, /* MTYPE_F4      */
  1, /* MTYPE_F8      */
  1, /* MTYPE_F10     */
  1, /* MTYPE_F16     */
  0, /* MTYPE_STR     */
  1, /* MTYPE_FQ      */
  1, /* MTYPE_M       */
  1, /* MTYPE_C4      */
  1, /* MTYPE_C8      */
  1, /* MTYPE_CQ      */
  1, /* MTYPE_V       */
  0, /* MTYPE_BS      */
  1, /* MTYPE_A4      */
  1, /* MTYPE_A8      */
  1, /* MTYPE_C10     */
  1, /* MTYPE_C16     */
  1, /* MTYPE_I16     */
  1  /* MTYPE_U16     */
};

static BOOL
Is_MTYPE_f_i_M_p_s_V_z [MTYPE_LAST+1] = {
  0, /* MTYPE_UNKNOWN */
  0, /* MTYPE_B       */
  1, /* MTYPE_I1      */
  1, /* MTYPE_I2      */
  1, /* MTYPE_I4      */
  1, /* MTYPE_I8      */
  1, /* MTYPE_U1      */
  1, /* MTYPE_U2      */
  1, /* MTYPE_U4      */
  1, /* MTYPE_U8      */
  1, /* MTYPE_F4      */
  1, /* MTYPE_F8      */
  1, /* MTYPE_F10     */
  1, /* MTYPE_F16     */
  0, /* MTYPE_STR     */
  1, /* MTYPE_FQ      */
  1, /* MTYPE_M       */
  1, /* MTYPE_C4      */
  1, /* MTYPE_C8      */
  1, /* MTYPE_CQ      */
  1, /* MTYPE_V       */
  0, /* MTYPE_BS      */
  1, /* MTYPE_A4      */
  1, /* MTYPE_A8      */
  1, /* MTYPE_C10     */
  1, /* MTYPE_C16     */
  1, /* MTYPE_I16     */
  1  /* MTYPE_U16     */
};

static BOOL
Is_MTYPE_f_i_M_p_s_z [MTYPE_LAST+1] = {
  0, /* MTYPE_UNKNOWN */
  0, /* MTYPE_B       */
  1, /* MTYPE_I1      */
  1, /* MTYPE_I2      */
  1, /* MTYPE_I4      */
  1, /* MTYPE_I8      */
  1, /* MTYPE_U1      */
  1, /* MTYPE_U2      */
  1, /* MTYPE_U4      */
  1, /* MTYPE_U8      */
  1, /* MTYPE_F4      */
  1, /* MTYPE_F8      */
  1, /* MTYPE_F10     */
  1, /* MTYPE_F16     */
  0, /* MTYPE_STR     */
  1, /* MTYPE_FQ      */
  1, /* MTYPE_M       */
  1, /* MTYPE_C4      */
  1, /* MTYPE_C8      */
  1, /* MTYPE_CQ      */
  0, /* MTYPE_V       */
  0, /* MTYPE_BS      */
  1, /* MTYPE_A4      */
  1, /* MTYPE_A8      */
  1, /* MTYPE_C10     */
  1, /* MTYPE_C16     */
  1, /* MTYPE_I16     */
  1  /* MTYPE_U16     */
};

static BOOL
Is_MTYPE_f_i_M_p_z [MTYPE_LAST+1] = {
  0, /* MTYPE_UNKNOWN */
  0, /* MTYPE_B       */
  0, /* MTYPE_I1      */
  0, /* MTYPE_I2      */
  1, /* MTYPE_I4      */
  1, /* MTYPE_I8      */
  0, /* MTYPE_U1      */
  0, /* MTYPE_U2      */
  1, /* MTYPE_U4      */
  1, /* MTYPE_U8      */
  1, /* MTYPE_F4      */
  1, /* MTYPE_F8      */
  1, /* MTYPE_F10     */
  1, /* MTYPE_F16     */
  0, /* MTYPE_STR     */
  1, /* MTYPE_FQ      */
  1, /* MTYPE_M       */
  1, /* MTYPE_C4      */
  1, /* MTYPE_C8      */
  1, /* MTYPE_CQ      */
  0, /* MTYPE_V       */
  0, /* MTYPE_BS      */
  1, /* MTYPE_A4      */
  1, /* MTYPE_A8      */
  1, /* MTYPE_C10     */
  1, /* MTYPE_C16     */
  1, /* MTYPE_I16     */
  1  /* MTYPE_U16     */
};

static BOOL
Is_MTYPE_f_i_M_z [MTYPE_LAST+1] = {
  0, /* MTYPE_UNKNOWN */
  0, /* MTYPE_B       */
  0, /* MTYPE_I1      */
  0, /* MTYPE_I2      */
  1, /* MTYPE_I4      */
  1, /* MTYPE_I8      */
  1, /* MTYPE_U1      */
  0, /* MTYPE_U2      */
  1, /* MTYPE_U4      */
  1, /* MTYPE_U8      */
  1, /* MTYPE_F4      */
  1, /* MTYPE_F8      */
  1, /* MTYPE_F10     */
  1, /* MTYPE_F16     */
  0, /* MTYPE_STR     */
  1, /* MTYPE_FQ      */
  1, /* MTYPE_M       */
  1, /* MTYPE_C4      */
  1, /* MTYPE_C8      */
  1, /* MTYPE_CQ      */
  0, /* MTYPE_V       */
  0, /* MTYPE_BS      */
  0, /* MTYPE_A4      */
  0, /* MTYPE_A8      */
  1, /* MTYPE_C10     */
  1, /* MTYPE_C16     */
  1, /* MTYPE_I16     */
  1  /* MTYPE_U16     */
};

static BOOL
Is_MTYPE_f_i [MTYPE_LAST+1] = {
  0, /* MTYPE_UNKNOWN */
  0, /* MTYPE_B       */
  0, /* MTYPE_I1      */
  0, /* MTYPE_I2      */
  1, /* MTYPE_I4      */
  1, /* MTYPE_I8      */
  0, /* MTYPE_U1      */
  0, /* MTYPE_U2      */
  1, /* MTYPE_U4      */
  1, /* MTYPE_U8      */
  1, /* MTYPE_F4      */
  1, /* MTYPE_F8      */
  1, /* MTYPE_F10     */
  1, /* MTYPE_F16     */
  0, /* MTYPE_STR     */
  1, /* MTYPE_FQ      */
  0, /* MTYPE_M       */
  0, /* MTYPE_C4      */
  0, /* MTYPE_C8      */
  0, /* MTYPE_CQ      */
  0, /* MTYPE_V       */
  0, /* MTYPE_BS      */
  0, /* MTYPE_A4      */
  0, /* MTYPE_A8      */
  0, /* MTYPE_C10     */
  0, /* MTYPE_C16     */
  1, /* MTYPE_I16     */
  1  /* MTYPE_U16     */
};

static BOOL
Is_MTYPE_f_i_p [MTYPE_LAST+1] = {
  0, /* MTYPE_UNKNOWN */
  0, /* MTYPE_B       */
  0, /* MTYPE_I1      */
  0, /* MTYPE_I2      */
  1, /* MTYPE_I4      */
  1, /* MTYPE_I8      */
  0, /* MTYPE_U1      */
  0, /* MTYPE_U2      */
  1, /* MTYPE_U4      */
  1, /* MTYPE_U8      */
  1, /* MTYPE_F4      */
  1, /* MTYPE_F8      */
  1, /* MTYPE_F10     */
  1, /* MTYPE_F16     */
  0, /* MTYPE_STR     */
  1, /* MTYPE_FQ      */
  0, /* MTYPE_M       */
  0, /* MTYPE_C4      */
  0, /* MTYPE_C8      */
  0, /* MTYPE_CQ      */
  0, /* MTYPE_V       */
  0, /* MTYPE_BS      */
  1, /* MTYPE_A4      */
  1, /* MTYPE_A8      */
  0, /* MTYPE_C10     */
  0, /* MTYPE_C16     */
  1, /* MTYPE_I16     */
  1  /* MTYPE_U16     */
};

static BOOL
Is_MTYPE_f_i_p_s_z [MTYPE_LAST+1] = {
  0, /* MTYPE_UNKNOWN */
  0, /* MTYPE_B       */
  1, /* MTYPE_I1      */
  1, /* MTYPE_I2      */
  1, /* MTYPE_I4      */
  1, /* MTYPE_I8      */
  1, /* MTYPE_U1      */
  1, /* MTYPE_U2      */
  1, /* MTYPE_U4      */
  1, /* MTYPE_U8      */
  1, /* MTYPE_F4      */
  1, /* MTYPE_F8      */
  1, /* MTYPE_F10     */
  1, /* MTYPE_F16     */
  0, /* MTYPE_STR     */
  1, /* MTYPE_FQ      */
  0, /* MTYPE_M       */
  1, /* MTYPE_C4      */
  1, /* MTYPE_C8      */
  1, /* MTYPE_CQ      */
  0, /* MTYPE_V       */
  0, /* MTYPE_BS      */
  1, /* MTYPE_A4      */
  1, /* MTYPE_A8      */
  1, /* MTYPE_C10     */
  1, /* MTYPE_C16     */
  1, /* MTYPE_I16     */
  1  /* MTYPE_U16     */
};

static BOOL
Is_MTYPE_f_i_p_z [MTYPE_LAST+1] = {
  0, /* MTYPE_UNKNOWN */
  0, /* MTYPE_B       */
  0, /* MTYPE_I1      */
  0, /* MTYPE_I2      */
  1, /* MTYPE_I4      */
  1, /* MTYPE_I8      */
  0, /* MTYPE_U1      */
  0, /* MTYPE_U2      */
  1, /* MTYPE_U4      */
  1, /* MTYPE_U8      */
  1, /* MTYPE_F4      */
  1, /* MTYPE_F8      */
  1, /* MTYPE_F10     */
  1, /* MTYPE_F16     */
  0, /* MTYPE_STR     */
  1, /* MTYPE_FQ      */
  0, /* MTYPE_M       */
  1, /* MTYPE_C4      */
  1, /* MTYPE_C8      */
  1, /* MTYPE_CQ      */
  0, /* MTYPE_V       */
  0, /* MTYPE_BS      */
  1, /* MTYPE_A4      */
  1, /* MTYPE_A8      */
  1, /* MTYPE_C10     */
  1, /* MTYPE_C16     */
  1, /* MTYPE_I16     */
  1  /* MTYPE_U16     */
};

static BOOL
Is_MTYPE_f_i_z [MTYPE_LAST+1] = {
  0, /* MTYPE_UNKNOWN */
  0, /* MTYPE_B       */
  0, /* MTYPE_I1      */
  0, /* MTYPE_I2      */
  1, /* MTYPE_I4      */
  1, /* MTYPE_I8      */
  0, /* MTYPE_U1      */
  0, /* MTYPE_U2      */
  1, /* MTYPE_U4      */
  1, /* MTYPE_U8      */
  1, /* MTYPE_F4      */
  1, /* MTYPE_F8      */
  1, /* MTYPE_F10     */
  1, /* MTYPE_F16     */
  0, /* MTYPE_STR     */
  1, /* MTYPE_FQ      */
  0, /* MTYPE_M       */
  1, /* MTYPE_C4      */
  1, /* MTYPE_C8      */
  1, /* MTYPE_CQ      */
  0, /* MTYPE_V       */
  0, /* MTYPE_BS      */
  0, /* MTYPE_A4      */
  0, /* MTYPE_A8      */
  1, /* MTYPE_C10     */
  1, /* MTYPE_C16     */
  1, /* MTYPE_I16     */
  1  /* MTYPE_U16     */
};

static BOOL
Is_MTYPE_f_z [MTYPE_LAST+1] = {
  0, /* MTYPE_UNKNOWN */
  0, /* MTYPE_B       */
  0, /* MTYPE_I1      */
  0, /* MTYPE_I2      */
  0, /* MTYPE_I4      */
  0, /* MTYPE_I8      */
  0, /* MTYPE_U1      */
  0, /* MTYPE_U2      */
  0, /* MTYPE_U4      */
  0, /* MTYPE_U8      */
  1, /* MTYPE_F4      */
  1, /* MTYPE_F8      */
  1, /* MTYPE_F10     */
  1, /* MTYPE_F16     */
  0, /* MTYPE_STR     */
  1, /* MTYPE_FQ      */
  0, /* MTYPE_M       */
  1, /* MTYPE_C4      */
  1, /* MTYPE_C8      */
  1, /* MTYPE_CQ      */
  0, /* MTYPE_V       */
  0, /* MTYPE_BS      */
  0, /* MTYPE_A4      */
  0, /* MTYPE_A8      */
  1, /* MTYPE_C10     */
  1, /* MTYPE_C16     */
  0, /* MTYPE_I16     */
  0  /* MTYPE_U16     */
};

static BOOL
Is_MTYPE_i [MTYPE_LAST+1] = {
  0, /* MTYPE_UNKNOWN */
  0, /* MTYPE_B       */
  0, /* MTYPE_I1      */
  0, /* MTYPE_I2      */
  1, /* MTYPE_I4      */
  1, /* MTYPE_I8      */
  0, /* MTYPE_U1      */
  0, /* MTYPE_U2      */
  1, /* MTYPE_U4      */
  1, /* MTYPE_U8      */
  0, /* MTYPE_F4      */
  0, /* MTYPE_F8      */
  0, /* MTYPE_F10     */
  0, /* MTYPE_F16     */
  0, /* MTYPE_STR     */
  0, /* MTYPE_FQ      */
  0, /* MTYPE_M       */
  0, /* MTYPE_C4      */
  0, /* MTYPE_C8      */
  0, /* MTYPE_CQ      */
  0, /* MTYPE_V       */
  0, /* MTYPE_BS      */
  0, /* MTYPE_A4      */
  0, /* MTYPE_A8      */
  0, /* MTYPE_C10     */
  0, /* MTYPE_C16     */
  1, /* MTYPE_I16     */
  1  /* MTYPE_U16     */
};

static BOOL
Is_MTYPE_b_i_p [MTYPE_LAST+1] = {
  0, /* MTYPE_UNKNOWN */
  1, /* MTYPE_B       */
  0, /* MTYPE_I1      */
  0, /* MTYPE_I2      */
  1, /* MTYPE_I4      */
  1, /* MTYPE_I8      */
  0, /* MTYPE_U1      */
  0, /* MTYPE_U2      */
  1, /* MTYPE_U4      */
  1, /* MTYPE_U8      */
  0, /* MTYPE_F4      */
  0, /* MTYPE_F8      */
  0, /* MTYPE_F10     */
  0, /* MTYPE_F16     */
  0, /* MTYPE_STR     */
  0, /* MTYPE_FQ      */
  0, /* MTYPE_M       */
  0, /* MTYPE_C4      */
  0, /* MTYPE_C8      */
  0, /* MTYPE_CQ      */
  0, /* MTYPE_V       */
  0, /* MTYPE_BS      */
  1, /* MTYPE_A4      */
  1, /* MTYPE_A8      */
  0, /* MTYPE_C10     */
  0, /* MTYPE_C16     */
  1, /* MTYPE_I16     */
  1  /* MTYPE_U16     */
};

static BOOL
Is_MTYPE_p [MTYPE_LAST+1] = {
  0, /* MTYPE_UNKNOWN */
  0, /* MTYPE_B       */
  0, /* MTYPE_I1      */
  0, /* MTYPE_I2      */
  0, /* MTYPE_I4      */
  0, /* MTYPE_I8      */
  0, /* MTYPE_U1      */
  0, /* MTYPE_U2      */
  1, /* MTYPE_U4      */
  1, /* MTYPE_U8      */
  0, /* MTYPE_F4      */
  0, /* MTYPE_F8      */
  0, /* MTYPE_F10     */
  0, /* MTYPE_F16     */
  0, /* MTYPE_STR     */
  0, /* MTYPE_FQ      */
  0, /* MTYPE_M       */
  0, /* MTYPE_C4      */
  0, /* MTYPE_C8      */
  0, /* MTYPE_CQ      */
  0, /* MTYPE_V       */
  0, /* MTYPE_BS      */
  1, /* MTYPE_A4      */
  1, /* MTYPE_A8      */
  0, /* MTYPE_C10     */
  0, /* MTYPE_C16     */
  0, /* MTYPE_I16     */
  0  /* MTYPE_U16     */
};

static BOOL
Is_MTYPE_s [MTYPE_LAST+1] = {
  0, /* MTYPE_UNKNOWN */
  0, /* MTYPE_B       */
  1, /* MTYPE_I1      */
  1, /* MTYPE_I2      */
  0, /* MTYPE_I4      */
  0, /* MTYPE_I8      */
  1, /* MTYPE_U1      */
  1, /* MTYPE_U2      */
  0, /* MTYPE_U4      */
  0, /* MTYPE_U8      */
  0, /* MTYPE_F4      */
  0, /* MTYPE_F8      */
  0, /* MTYPE_F10     */
  0, /* MTYPE_F16     */
  0, /* MTYPE_STR     */
  0, /* MTYPE_FQ      */
  0, /* MTYPE_M       */
  0, /* MTYPE_C4      */
  0, /* MTYPE_C8      */
  0, /* MTYPE_CQ      */
  0, /* MTYPE_V       */
  0, /* MTYPE_BS      */
  0, /* MTYPE_A4      */
  0, /* MTYPE_A8      */
  0, /* MTYPE_C10     */
  0, /* MTYPE_C16     */
  0, /* MTYPE_I16     */
  0  /* MTYPE_U16     */
};

static BOOL
Is_MTYPE_b_i_s [MTYPE_LAST+1] = {
  0, /* MTYPE_UNKNOWN */
  1, /* MTYPE_B       */
  1, /* MTYPE_I1      */
  1, /* MTYPE_I2      */
  1, /* MTYPE_I4      */
  1, /* MTYPE_I8      */
  1, /* MTYPE_U1      */
  1, /* MTYPE_U2      */
  1, /* MTYPE_U4      */
  1, /* MTYPE_U8      */
  0, /* MTYPE_F4      */
  0, /* MTYPE_F8      */
  0, /* MTYPE_F10     */
  0, /* MTYPE_F16     */
  0, /* MTYPE_STR     */
  0, /* MTYPE_FQ      */
  0, /* MTYPE_M       */
  0, /* MTYPE_C4      */
  0, /* MTYPE_C8      */
  0, /* MTYPE_CQ      */
  0, /* MTYPE_V       */
  0, /* MTYPE_BS      */
  0, /* MTYPE_A4      */
  0, /* MTYPE_A8      */
  0, /* MTYPE_C10     */
  0, /* MTYPE_C16     */
  1, /* MTYPE_I16     */
  1  /* MTYPE_U16     */
};

static BOOL
Is_MTYPE_z [MTYPE_LAST+1] = {
  0, /* MTYPE_UNKNOWN */
  0, /* MTYPE_B       */
  0, /* MTYPE_I1      */
  0, /* MTYPE_I2      */
  0, /* MTYPE_I4      */
  0, /* MTYPE_I8      */
  0, /* MTYPE_U1      */
  0, /* MTYPE_U2      */
  0, /* MTYPE_U4      */
  0, /* MTYPE_U8      */
  0, /* MTYPE_F4      */
  0, /* MTYPE_F8      */
  0, /* MTYPE_F10     */
  0, /* MTYPE_F16     */
  0, /* MTYPE_STR     */
  0, /* MTYPE_FQ      */
  0, /* MTYPE_M       */
  1, /* MTYPE_C4      */
  1, /* MTYPE_C8      */
  1, /* MTYPE_CQ      */
  0, /* MTYPE_V       */
  0, /* MTYPE_BS      */
  0, /* MTYPE_A4      */
  0, /* MTYPE_A8      */
  1, /* MTYPE_C10     */
  1, /* MTYPE_C16     */
  0, /* MTYPE_I16     */
  0  /* MTYPE_U16     */
};


void
breakpoint ()
{
}

BOOL
Is_Valid_Opcode_Parts (OPERATOR opr, TYPE_ID rtype, TYPE_ID desc)
{
  BOOL valid;

       if (opr   < OPERATOR_FIRST || opr   > OPERATOR_LAST) valid = FALSE;
  else if (rtype < MTYPE_FIRST    || rtype > MTYPE_LAST)    valid = FALSE;
  else if (desc  < MTYPE_FIRST    || desc  > MTYPE_LAST)    valid = FALSE;

  else  {
    switch (opr) {

      case OPR_ABS:
        // [RTYPE] : f,I4,I8,I16 [DESC] : V
        valid = Is_MTYPE_f_I4_I8_I16 [rtype] && desc == MTYPE_V;
        break;

      case OPR_ADD:
      case OPR_CONST:
      case OPR_SUB:
      case OPR_ASM_EXPR:
        // [RTYPE] : f,i,p,z [DESC] : V
        valid = Is_MTYPE_f_i_p_z [rtype] && desc == MTYPE_V;
        break;

      case OPR_AGOTO:
      case OPR_ALTENTRY:
      case OPR_ASSERT:
      case OPR_BACKWARD_BARRIER:
      case OPR_BLOCK:
      case OPR_CASEGOTO:
      case OPR_COMMENT:
      case OPR_COMPGOTO:
      case OPR_DO_LOOP:
      case OPR_DO_WHILE:
      case OPR_EVAL:
      case OPR_EXC_SCOPE_BEGIN:
      case OPR_EXC_SCOPE_END:
      case OPR_FALSEBR:
      case OPR_FORWARD_BARRIER:
      case OPR_FUNC_ENTRY:
      case OPR_GOTO:
      case OPR_IDNAME:
      case OPR_IF:
      case OPR_IO:
      case OPR_IO_ITEM:
      case OPR_LABEL:
      case OPR_LOOP_INFO:
      case OPR_MSTORE:
      case OPR_OPT_CHI:
      case OPR_OPT_RESERVE2:
      case OPR_PRAGMA:
      case OPR_PREFETCH:
      case OPR_PREFETCHX:
      case OPR_REGION:
      case OPR_REGION_EXIT:
      case OPR_RETURN:
      case OPR_SWITCH:
      case OPR_TRAP:
      case OPR_TRUEBR:
      case OPR_WHERE:
      case OPR_WHILE_DO:
      case OPR_XGOTO:
      case OPR_XPRAGMA:
      case OPR_AFFIRM:
      case OPR_DEALLOCA:
      case OPR_ASM_STMT:
      case OPR_ASM_INPUT:
      case OPR_GOTO_OUTER_BLOCK:
      case OPR_NAMELIST:
      case OPR_IMPLICIT_BND: 
      case OPR_NULLIFY:
      case OPR_INTERFACE:

        // [RTYPE] : V [DESC] : V
        valid = rtype == MTYPE_V && desc == MTYPE_V;
        break;

      case OPR_USE: 
        // [RTYPE] : V,B [DESC] : V
        valid = (rtype == MTYPE_V || rtype == MTYPE_B) && desc == MTYPE_V;
        break;

      case OPR_ARRAY:
      case OPR_ARRSECTION:
      case OPR_LDA:
      case OPR_ILDA:
      case OPR_LDMA:
      case OPR_LDA_LABEL:
        // [RTYPE] : p [DESC] : V
        valid = Is_MTYPE_p [rtype] && desc == MTYPE_V;
        break;

      case OPR_ARRAYEXP:
      case OPR_ARRAY_CONSTRUCT:
        // [RTYPE] : f,i,M,z [DESC] : V
        valid = Is_MTYPE_f_i_M_z [rtype] && desc == MTYPE_V;
        break;

      case OPR_ASHR:
      case OPR_BAND:
      case OPR_BIOR:
      case OPR_BNOR:
      case OPR_BNOT:
      case OPR_BXOR:
      case OPR_COMPOSE_BITS:
      case OPR_CVTL:
      case OPR_DIVREM:
      case OPR_EXTRACT_BITS:
      case OPR_HIGHMPY:
      case OPR_HIGHPART:
      case OPR_LOWPART:
      case OPR_LSHR:
      case OPR_MOD:
      case OPR_REM:
      case OPR_SHL:
      case OPR_TRIPLET:
      case OPR_SRCTRIPLET:
      case OPR_XMPY:
      case OPR_IMPLIED_DO:

        // [RTYPE] : i [DESC] : V
        valid = Is_MTYPE_i [rtype] && desc == MTYPE_V;
        break;

      case OPR_CALL:
      case OPR_ICALL:
      case OPR_INTRINSIC_CALL:
      case OPR_PICCALL:
      case OPR_VFCALL:
        // [RTYPE] : f,i,M,p,s,V,z [DESC] : V,extra
        valid =    (Is_MTYPE_f_i_M_p_s_V_z [rtype] && desc == MTYPE_V)
                || (rtype == MTYPE_I4 && desc == MTYPE_I4)
                || (rtype == MTYPE_U4 && desc == MTYPE_U4)
                || (rtype == MTYPE_I8 && desc == MTYPE_I8)
                || (rtype == MTYPE_U8 && desc == MTYPE_U8)
                || (rtype == MTYPE_F4 && desc == MTYPE_F4)
                || (rtype == MTYPE_F4 && desc == MTYPE_F8)
                || (rtype == MTYPE_F8 && desc == MTYPE_F4)
                || (rtype == MTYPE_F8 && desc == MTYPE_F8);
        break;

      case OPR_CAND:
      case OPR_CIOR:
      case OPR_LAND:
      case OPR_LIOR:
        /* [RTYPE] : b : [DESC] : V */
        valid = Is_MTYPE_b [rtype] && desc == MTYPE_V;
        break;

#ifndef  FRONT_END_FORTRAN
        /* In Fortran, the rtype of floor() is int, but is double in C/C++. */

      case OPR_FLOOR:
        /* [RTYPE] : f,i [DESC] : f */
        valid = Is_MTYPE_f_i[rtype] && Is_MTYPE_f[desc];
        break;
#endif


      case OPR_CEIL:
#ifdef FRONT_END_FORTRAN
      case OPR_FLOOR: 
#endif
      case OPR_RND:
        // [RTYPE] : i [DESC] : f
        valid = Is_MTYPE_i [rtype] && Is_MTYPE_f [desc];
        break;

      case OPR_TRUNC:
        // [RTYPE] : i 
        valid = Is_MTYPE_i [rtype];
        break;

      case OPR_COMMA:
      case OPR_RCOMMA:
        // [RTYPE] : f,i,M,p,z [DESC] : V
        valid = Is_MTYPE_f_i_M_p_z [rtype] && desc == MTYPE_V;
        break;

      case OPR_PAIR:
        // [RTYPE] : z [DESC] : V
        valid = Is_MTYPE_z [rtype] && desc == MTYPE_V;
        break;

      case OPR_CSELECT:
        // [RTYPE] : b,f,i,M,p,V,z [DESC] : V
        valid = Is_MTYPE_b_f_i_M_p_V_z [rtype] && desc == MTYPE_V;
        break;

      case OPR_CVT:
        // [RTYPE] : f,i,p [DESC] : f,i,p extra
        valid = Is_MTYPE_f_i_p [rtype] && 
		(Is_MTYPE_f_i_p [desc] || desc == MTYPE_B) &&
		(rtype != desc);
        break;

      case OPR_MPY:
        // [RTYPE] : f,i,z,s [DESC] : V
        valid = (Is_MTYPE_f_i_z [rtype] || Is_MTYPE_s [rtype]) && 
		desc == MTYPE_V;
        break;

      case OPR_DIV:
      case OPR_NEG:
      case OPR_OPTPARM:
        // [RTYPE] : f,i,z [DESC] : V
        valid = Is_MTYPE_f_i_z [rtype] && desc == MTYPE_V;
        break;

      case OPR_EQ:
      case OPR_NE:
        // [RTYPE] : b [DESC] : f,i,p,z
        valid = Is_MTYPE_b [rtype] && Is_MTYPE_b_f_i_p_z [desc];
        break;

      case OPR_LNOT:
        // [RTYPE] : b : [DESC] : b,V
        valid = Is_MTYPE_b [rtype] && (desc == MTYPE_V || desc == MTYPE_B);
        break;

      case OPR_GE:
      case OPR_GT:
      case OPR_LE:
      case OPR_LT:
        // [RTYPE] : b [DESC] : f,i,p
        valid = Is_MTYPE_b [rtype] && Is_MTYPE_f_i_p [desc];
        break;

      case OPR_LDBITS:
      case OPR_ILDBITS:
        // [RTYPE] : i [DESC] : s
        valid = Is_MTYPE_i [rtype] && Is_MTYPE_b_i_s [desc];
        break;

      case OPR_ILOAD:
      case OPR_LDID:
        if (WHIRL_Mldid_Mstid_On) {

          // [RTYPE] : f,i,M,p,z [DESC] : bs,f,i,M,p,s,z
          valid =    (    (rtype == MTYPE_I4 || rtype == MTYPE_I8)
                       && Is_MTYPE_bs_I1_I2_I4_I8 [desc])
                  || (    (rtype == MTYPE_U4 || rtype == MTYPE_U8)
                       && Is_MTYPE_bs_U1_U2_U4_U8 [desc])
                  || (Is_MTYPE_b_f_i_M_p_z [rtype] && rtype == desc);
        }

        else {

          // [RTYPE] : f,i,p,z [DESC] : bs,f,i,p,s,z
          valid =    (    (rtype == MTYPE_I4 || rtype == MTYPE_I8)
                       && Is_MTYPE_bs_I1_I2_I4_I8 [desc])
                  || (    (rtype == MTYPE_U4 || rtype == MTYPE_U8)
                       && Is_MTYPE_bs_U1_U2_U4_U8 [desc])
                  || (Is_MTYPE_b_f_i_p_z [rtype] && rtype == desc);
        }
        break;

      case OPR_ILOADX:
      case OPR_SECONDPART:
      case OPR_MADD:
      case OPR_MSUB:
      case OPR_NMADD:
      case OPR_NMSUB:
      case OPR_FIRSTPART:
        // [RTYPE] : f [DESC] : V
        valid = Is_MTYPE_f [rtype] && desc == MTYPE_V;
        break;

      case OPR_INTCONST:
        // [RTYPE] : i,p [DESC] : V
        valid = Is_MTYPE_b_i_p [rtype] && desc == MTYPE_V;
        break;

      case OPR_INTRINSIC_OP:
        // [RTYPE] : b,f,i,M,p,s,z [DESC] : V
        valid = Is_MTYPE_b_f_i_M_p_s_z [rtype] && desc == MTYPE_V;
        break;

      case OPR_ISTBITS:
      case OPR_STBITS:
        // [RTYPE] : V [DESC] : s
        valid = rtype == MTYPE_V && Is_MTYPE_b_i_s [desc];
        break;

      case OPR_ISTORE:
      case OPR_PSTORE:   
      case OPR_STID:
      case OPR_PSTID:    

        if (WHIRL_Mldid_Mstid_On) {

          // [RTYPE] : V [DESC] : bs,f,i,M,p,s,z
          valid = rtype == MTYPE_V && Is_MTYPE_b_bs_f_i_M_p_s_z [desc];
        }

        else {

          // [RTYPE] : V [DESC] : bs,f,i,p,s,z
          valid = rtype == MTYPE_V && Is_MTYPE_b_bs_f_i_p_s_z [desc];
        }
        break;

      case OPR_ISTOREX:
        // [RTYPE] : V [DESC] : f
        valid = rtype == MTYPE_V && Is_MTYPE_f [desc];
        break;

      case OPR_MAX:
      case OPR_MIN:
      case OPR_MINMAX:
        // [RTYPE] : f,i,p [DESC] : V
        valid = Is_MTYPE_f_i_p [rtype] && desc == MTYPE_V;
        break;

      case OPR_MAXPART:
      case OPR_MINPART:
        // [RTYPE] : f,i [DESC] : V
        valid = Is_MTYPE_f_i [rtype] && desc == MTYPE_V;
        break;

      case OPR_MLOAD:
        // [RTYPE] : M [DESC] : V,M
        valid = rtype == MTYPE_M && 
		(desc == MTYPE_V || desc == MTYPE_M);
        break;

      case OPR_PAREN:
	// [RTYPE] : f,i,z [DESC] : V
        valid = Is_MTYPE_f_i_z [rtype] && desc == MTYPE_V;
        break;
	
      case OPR_RECIP:
      case OPR_RSQRT:
      case OPR_SQRT:
        // [RTYPE] : f,z [DESC] : V
        valid = Is_MTYPE_f_z [rtype] && desc == MTYPE_V;
        break;

      case OPR_PARM:
        // [RTYPE] : f,i,M,p,V,z [DESC] : V
        valid = Is_MTYPE_f_i_M_p_V_z [rtype] && desc == MTYPE_V;
        break;

      case OPR_RETURN_VAL:
        // [RTYPE] : f,i,M,p,s,z [DESC] : V
        valid = Is_MTYPE_f_i_M_p_s_z [rtype] && desc == MTYPE_V;
        break;

      case OPR_SELECT:
        // [RTYPE] : b,f,i,p,z [DESC] : V,B
        valid = Is_MTYPE_b_f_i_p_z [rtype] && 
		(desc == MTYPE_V || desc == MTYPE_B);
        break;

      case OPR_TAS:
        // [RTYPE] : f,i,p,s,z [DESC] : V
        valid = Is_MTYPE_f_i_p_s_z [rtype] && desc == MTYPE_V;
        break;

      case OPR_ALLOCA:
        // [RTYPE] : p [DESC] : V
        valid = Is_MTYPE_p [rtype] && desc == MTYPE_V;
        break;

      case OPR_RROTATE:
        // [RTYPE] : U1,U2,U4,U8 [DESC] : V
        valid =    ((rtype == MTYPE_U4) && (    desc == MTYPE_U1
                                             || desc == MTYPE_U2
                                             || desc == MTYPE_U4))
                || ((rtype == MTYPE_U8) && (desc == MTYPE_U8));
        break;

      case OPR_STRCTFLD: 
        // [RTYPE]: U8  [DESC]: U8
        valid =  (rtype == MTYPE_U8) && (desc == MTYPE_U8);
        break;

      default:
        valid = FALSE;
        break;
    }
  }

  if (valid == FALSE)
    breakpoint ();

  return valid;
}

BOOL
Is_Valid_Opcode_FUNC (OPCODE opc)
{
  OPERATOR opr   = (OPERATOR) (((UINT32) opc) & 0xFF);
  TYPE_ID  rtype = (TYPE_ID)  ((((UINT32) opc) >> 8) & 0x1F);
  TYPE_ID  desc  = (TYPE_ID)  ((((UINT32) opc) >> 13) & 0x1F);

  return Is_Valid_Opcode_Parts (opr, rtype, desc);
}

char*
OPCODE_name (OPERATOR opr, TYPE_ID rtype, TYPE_ID desc)
{
  static char buffer [64];

  Is_True(Is_Valid_Opcode_Parts (opr, rtype, desc),
          ("Bad opcode %d %d %d", opr, rtype, desc));

  switch (opr) {

    case OPR_ABS:
      // [RTYPE] : f,I4,I8,I16 [DESC] : V
      sprintf (buffer, "OPC_%s%s", MTYPE_name(rtype), &OPERATOR_info [opr]._name [4]);
      break;

    case OPR_ADD:
    case OPR_CONST:
    case OPR_SUB:
    case OPR_ASM_EXPR:
      // [RTYPE] : f,i,p,z [DESC] : V
      sprintf (buffer, "OPC_%s%s", MTYPE_name(rtype), &OPERATOR_info [opr]._name [4]);
      break;

    case OPR_AGOTO:
    case OPR_ALTENTRY:
    case OPR_ASSERT:
    case OPR_BACKWARD_BARRIER:
    case OPR_BLOCK:
    case OPR_CASEGOTO:
    case OPR_COMMENT:
    case OPR_COMPGOTO:
    case OPR_DO_LOOP:
    case OPR_DO_WHILE:
    case OPR_EVAL:
    case OPR_EXC_SCOPE_BEGIN:
    case OPR_EXC_SCOPE_END:
    case OPR_FALSEBR:
    case OPR_FORWARD_BARRIER:
    case OPR_FUNC_ENTRY:
    case OPR_GOTO:
    case OPR_IDNAME:
    case OPR_IF:
    case OPR_IO:
    case OPR_IO_ITEM:
    case OPR_LABEL:
    case OPR_LOOP_INFO:
    case OPR_MSTORE:
    case OPR_OPT_CHI:
    case OPR_OPT_RESERVE2:
    case OPR_PRAGMA:
    case OPR_PREFETCH:
    case OPR_PREFETCHX:
    case OPR_REGION:
    case OPR_REGION_EXIT:
    case OPR_RETURN:
    case OPR_SWITCH:
    case OPR_TRAP:
    case OPR_TRUEBR:
    case OPR_WHERE:
    case OPR_WHILE_DO:
    case OPR_XGOTO:
    case OPR_XPRAGMA:
    case OPR_AFFIRM:
    case OPR_DEALLOCA:
    case OPR_ASM_STMT:
    case OPR_ASM_INPUT:
    case OPR_GOTO_OUTER_BLOCK:
    case OPR_NAMELIST:
    case OPR_IMPLICIT_BND: 
    case OPR_NULLIFY:
    case OPR_INTERFACE:

      // [RTYPE] : V [DESC] : V
      sprintf (buffer, "OPC_%s", &OPERATOR_info [opr]._name [4]);
      break;

    case OPR_USE:  
    case OPR_ARRAY:
    case OPR_ARRSECTION:
    case OPR_LDA:
    case OPR_LDMA:
    case OPR_ILDA:
    case OPR_LDA_LABEL:
      // [RTYPE] : p [DESC] : V
      sprintf (buffer, "OPC_%s%s", MTYPE_name(rtype), &OPERATOR_info [opr]._name [4]);
      break;

    case OPR_ARRAYEXP:
    case OPR_ARRAY_CONSTRUCT:
      // [RTYPE] : f,i,M,z [DESC] : V
      sprintf (buffer, "OPC_%s%s", MTYPE_name(rtype), &OPERATOR_info [opr]._name [4]);
      break;

    case OPR_ASHR:
    case OPR_BAND:
    case OPR_BIOR:
    case OPR_BNOR:
    case OPR_BNOT:
    case OPR_BXOR:
    case OPR_COMPOSE_BITS:
    case OPR_CVTL:
    case OPR_DIVREM:
    case OPR_EXTRACT_BITS:
    case OPR_HIGHMPY:
    case OPR_HIGHPART:
    case OPR_LOWPART:
    case OPR_LSHR:
    case OPR_MOD:
    case OPR_REM:
    case OPR_SHL:
    case OPR_TRIPLET:
    case OPR_SRCTRIPLET:
    case OPR_XMPY:
    case OPR_IMPLIED_DO:

      // [RTYPE] : i [DESC] : V
      sprintf (buffer, "OPC_%s%s", MTYPE_name(rtype), &OPERATOR_info [opr]._name [4]);
      break;

    case OPR_CALL:
    case OPR_ICALL:
    case OPR_INTRINSIC_CALL:
    case OPR_PICCALL:
    case OPR_VFCALL:
      // [RTYPE] : f,i,M,p,s,V,z [DESC] : V,extra
      if (desc == MTYPE_V)
        sprintf (buffer, "OPC_%s%s", MTYPE_name(rtype), &OPERATOR_info [opr]._name [4]);
      else
        sprintf (buffer, "OPC_%s%s%s", MTYPE_name(rtype), MTYPE_name(desc), &OPERATOR_info [opr]._name [4]);
      break;

    case OPR_CAND:
    case OPR_CIOR:
    case OPR_LAND:
    case OPR_LIOR:
    case OPR_LNOT:
      // [RTYPE] : b : [DESC] : V
      sprintf (buffer, "OPC_%s%s", MTYPE_name(rtype), &OPERATOR_info [opr]._name [4]);
      break;

    case OPR_CEIL:
    case OPR_FLOOR:
    case OPR_RND:
    case OPR_TRUNC:
      // [RTYPE] : i [DESC] : f
      sprintf (buffer, "OPC_%s%s%s", MTYPE_name(rtype), MTYPE_name(desc), &OPERATOR_info [opr]._name [4]);
      break;

    case OPR_COMMA:
    case OPR_RCOMMA:
      // [RTYPE] : f,i,M,p,z [DESC] : V
      sprintf (buffer, "OPC_%s%s", MTYPE_name(rtype), &OPERATOR_info [opr]._name [4]);
      break;

    case OPR_PAIR:
      // [RTYPE] : z [DESC] : V
      sprintf (buffer, "OPC_%s%s", MTYPE_name(rtype), &OPERATOR_info [opr]._name [4]);
      break;

    case OPR_CSELECT:
      // [RTYPE] : b,f,i,M,p,V,z [DESC] : V
      sprintf (buffer, "OPC_%s%s", MTYPE_name(rtype), &OPERATOR_info [opr]._name [4]);
      break;

    case OPR_CVT:
      // [RTYPE] : f,i,p [DESC] : f,i,p extra
      sprintf (buffer, "OPC_%s%s%s", MTYPE_name(rtype), MTYPE_name(desc), &OPERATOR_info [opr]._name [4]);
      break;

    case OPR_DIV:
    case OPR_MPY:
    case OPR_NEG:
    case OPR_OPTPARM:
      // [RTYPE] : f,i,z [DESC] : V
      sprintf (buffer, "OPC_%s%s", MTYPE_name(rtype), &OPERATOR_info [opr]._name [4]);
      break;

    case OPR_EQ:
    case OPR_NE:
      // [RTYPE] : b [DESC] : f,i,p,z
      sprintf (buffer, "OPC_%s%s%s", MTYPE_name(rtype), MTYPE_name(desc), &OPERATOR_info [opr]._name [4]);
      break;

    case OPR_GE:
    case OPR_GT:
    case OPR_LE:
    case OPR_LT:
      // [RTYPE] : b [DESC] : f,i,p
      sprintf (buffer, "OPC_%s%s%s", MTYPE_name(rtype), MTYPE_name(desc), &OPERATOR_info [opr]._name [4]);
      break;

    case OPR_ILDBITS:
    case OPR_LDBITS:
      // [RTYPE] : i [DESC] : s
      sprintf (buffer, "OPC_%s%s%s", MTYPE_name(rtype), MTYPE_name(desc), &OPERATOR_info [opr]._name [4]);
      break;

    case OPR_ILOAD:
    case OPR_LDID:
    case OPR_STRCTFLD: 
      // [RTYPE] : f,i,M,p,z [DESC] : bs,f,i,M,p,s,z
      sprintf (buffer, "OPC_%s%s%s", MTYPE_name(rtype), MTYPE_name(desc), &OPERATOR_info [opr]._name [4]);
      break;

    case OPR_ILOADX:
    case OPR_SECONDPART:
    case OPR_MADD:
    case OPR_MSUB:
    case OPR_NMADD:
    case OPR_NMSUB:
    case OPR_FIRSTPART:
      // [RTYPE] : f [DESC] : V
      sprintf (buffer, "OPC_%s%s", MTYPE_name(rtype), &OPERATOR_info [opr]._name [4]);
      break;

    case OPR_INTCONST:
      // [RTYPE] : i,p [DESC] : V
      sprintf (buffer, "OPC_%s%s", MTYPE_name(rtype), &OPERATOR_info [opr]._name [4]);
      break;

    case OPR_INTRINSIC_OP:
      // [RTYPE] : b,f,i,M,p,s,z [DESC] : V
      sprintf (buffer, "OPC_%s%s", MTYPE_name(rtype), &OPERATOR_info [opr]._name [4]);
      break;

    case OPR_ISTBITS:
    case OPR_STBITS:
      // [RTYPE] : V [DESC] : s
      sprintf (buffer, "OPC_%s%s", MTYPE_name(desc), &OPERATOR_info [opr]._name [4]);
      break;

    case OPR_ISTORE:
    case OPR_PSTORE:  
    case OPR_STID:
    case OPR_PSTID:  
      // [RTYPE] : V [DESC] : bs,f,i,M,p,s,z
      sprintf (buffer, "OPC_%s%s", MTYPE_name(desc), &OPERATOR_info [opr]._name [4]);
      break;

    case OPR_ISTOREX:
      // [RTYPE] : V [DESC] : f
      sprintf (buffer, "OPC_%s%s", MTYPE_name(desc), &OPERATOR_info [opr]._name [4]);
      break;

    case OPR_MAX:
    case OPR_MIN:
    case OPR_MINMAX:
      // [RTYPE] : f,i,p [DESC] : V
      sprintf (buffer, "OPC_%s%s", MTYPE_name(rtype), &OPERATOR_info [opr]._name [4]);
      break;

    case OPR_MAXPART:
    case OPR_MINPART:
      // [RTYPE] : f,i [DESC] : V
      sprintf (buffer, "OPC_%s%s", MTYPE_name(rtype), &OPERATOR_info [opr]._name [4]);
      break;

    case OPR_MLOAD:
      // [RTYPE] : M [DESC] : V
      sprintf (buffer, "OPC_%s", &OPERATOR_info [opr]._name [4]);
      break;

    case OPR_PAREN:
      // [RTYPE] : f,i,z [DESC] : V
      sprintf (buffer, "OPC_%s%s", MTYPE_name(rtype), &OPERATOR_info [opr]._name [4]);
      break;

    case OPR_RECIP:
    case OPR_RSQRT:
    case OPR_SQRT:
      // [RTYPE] : f,z [DESC] : V
      sprintf (buffer, "OPC_%s%s", MTYPE_name(rtype), &OPERATOR_info [opr]._name [4]);
      break;

    case OPR_PARM:
      // [RTYPE] : f,i,M,p,V,z [DESC] : V
      sprintf (buffer, "OPC_%s%s", MTYPE_name(rtype), &OPERATOR_info [opr]._name [4]);
      break;

    case OPR_RETURN_VAL:
      // [RTYPE] : f,i,M,p,s,z [DESC] : V
      sprintf (buffer, "OPC_%s%s", MTYPE_name(rtype), &OPERATOR_info [opr]._name [4]);
      break;

    case OPR_SELECT:
      // [RTYPE] : b,f,i,p,z [DESC] : V,b
      sprintf (buffer, "OPC_%s%s%s", MTYPE_name(rtype), 
       desc == MTYPE_V ? "" : MTYPE_name(desc), &OPERATOR_info [opr]._name [4]);
      break;

    case OPR_TAS:
      // [RTYPE] : f,i,p,s,z [DESC] : V
      sprintf (buffer, "OPC_%s%s", MTYPE_name(rtype), &OPERATOR_info [opr]._name [4]);
      break;

    case OPR_ALLOCA:
      // [RTYPE] : p [DESC] : V
      sprintf (buffer, "OPC_%s%s", MTYPE_name(rtype), &OPERATOR_info [opr]._name [4]);
      break;

    case OPR_RROTATE:
      // [RTYPE] : U4,U8 [DESC] : U1,U2,U4,U8
      sprintf (buffer, "OPC_%s%s%s", MTYPE_name(rtype), MTYPE_name(desc), &OPERATOR_info [opr]._name [4]);
      break;

    default:
      buffer [0] = 0;
      break;
  }

  return buffer;
}

char*
OPCODE_name (OPCODE opc)
{
  OPERATOR opr   = (OPERATOR) (((UINT32) opc) & 0xFF);
  TYPE_ID  rtype = (TYPE_ID)  ((((UINT32) opc) >> 8) & 0x1F);
  TYPE_ID  desc  = (TYPE_ID)  ((((UINT32) opc) >> 13) & 0x1F);

  return OPCODE_name (opr, rtype, desc);
}


#if 0
// This stuff sets up a fast OPCODE_is_valid_opcode_table lookup table
// If an opcode is valid, it contains the opcode. Otherwise, is contains OPCODE_UNKNOWN.
OPCODE OPCODE_is_valid_opcode_table[OPCODE_LAST+1];

static struct setup_the_opcode_valid_table {
public:
   setup_the_opcode_valid_table() {
      INT i;
      for (i=0; i <= OPCODE_LAST; i++) {
	 if (Is_Valid_Opcode_FUNC((OPCODE) i)) {
	    OPCODE_is_valid_opcode_table[i] = (OPCODE) i;
	 } else {
	    OPCODE_is_valid_opcode_table[i] = OPCODE_UNKNOWN;
	 }	    
      }
   }
} dummy_setup_the_opcode;
#endif



