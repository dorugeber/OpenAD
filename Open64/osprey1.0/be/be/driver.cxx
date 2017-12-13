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
 *  13-Feb-95 - Original Version
 *
 * Description:
 *  Main driver for the entire backend.
 *
 * ====================================================================
 * ====================================================================
 */

#include <sys/types.h>
#include <elf.h>		    /* for wn.h */
#include <cmplrs/rcodes.h>
#include <dirent.h>
#include <iostream>
#include <sys/param.h>

#include "defs.h"
#include "dso.h"		    /* for load_so() */
#include "errors.h"		    /* Set_Error_Tables(), etc. */
#include "err_host.tab"		    /* load all the error messages */
#include "erglob.h"		    /* for EC_ errmsg */
#include "erauxdesc.h"		    /* for BE error messages */
#include "mempool.h"		    /* for MEM_Initialze()  */
#include "phase.h"		    /* for PHASE_CG */
#include "be_util.h"                /* for Reset_Current_PU_Count(), etc */
#include "wn.h"			    /* for WN */
#include "driver_util.h"	    /* for Process_Command_Line() */
#include "timing.h"		    /* for Reset_Timer(), etc. */
#include "glob.h"		    /* for Show_Progress */
#include "stab.h"		    /* for ir_bread.h */
#include "pu_info.h"		    /* for PU_Info */
#include "ir_bread.h"		    /* for Read_Global_Info() */
#include "ir_bwrite.h"		    /* for Write_Global_Info(), etc. */
#include "config.h"		    /* for LNO_Path, etc. */
#include "config_opt.h"             /* for Instrumentation_Enabled */
#include "config_list.h"	    /* for List_Enabled, etc. */
#include "config_lno.h"		    /* for LNO_Run_Lego, etc. */
#include "file_util.h"		    /* for New_Extension () */
#include "xstats.h"		    /* for Print_Stats () */
#include "data_layout.h"	    /* for Initialize_Stack_Frame() */
#include "opt_alias_interface.h"    /* for ALIAS_MANAGER stuff */
#include "wn_lower.h"		    /* for WN_Lower() */
#include "ori.h"		    /* for Olimit_Region_Insertion */
#include "w2c_driver.h"		    /* for W2C_Process_Command_Line, etc. */
#include "w2f_driver.h"		    /* for W2F_Process_Command_Line, etc. */
#include "anl_driver.h"		    /* for Anl_Process_Command_Line, etc. */
#include "region_util.h"	    /* for Regions_Around_Inner_Loops */
#include "region_main.h"	    /* for REGION_* driver specific routines */
#include "tracing.h"                /* For the trace routines */
#include "ir_reader.h"              /* For fdump_tree */
#include "dwarf_DST.h"	    	    /* for Orig_PU_Name */
#include "fb_whirl.h"		    /* for FEEDBACK */
#include "iter.h"		    /* PU iterator for loops */
#include "dra_export.h"             /* for DRA routines */
#include "ti_init.h"		    /* for targ_info */
#include "opt_alias_interface.h"    /* for Create_Alias_Manager */
#include "omp_lower.h"              /* for OMP pre-lowering interface */
#include "cxx_memory.h"		    /* CXX_NEW */
#include "options_stack.h"	    /* for Options_Stack */
#include "be_symtab.h"		    /* for Be_preg_tab */
#include "prompf.h" 		    /* Prompf support */ 
#include "wb_omp.h"		    /* whirl browser for omp prelowerer */
#include "wb_lwr.h"		    /* whirl browser for lowerer */ 
#include "wb_anl.h"		    /* whirl browser for prompf static anal */ 
#include "wn_instrument.h"          /* whirl instrumenter */
#include "mem_ctr.h"
#include "upc_symtab_utils.h"
#include "ipl_driver.h"


#if defined(__CYGWIN__)
# define DSOext ".dll" /* cygwin needs to use dll for DSOs */
#else
# define DSOext ".so"
#endif


extern void Initialize_Targ_Info(void);

#if defined(_GCC_NO_PRAGMAWEAK) || defined(__CYGWIN__)
extern "C" {
INT64       New_Construct_Id(void) { return 0; }
INT64       Get_Next_Construct_Id(void) { return 0; }
}
#endif


// symbols defined in wopt.so
#if defined(__linux__) || defined(_GCC_NO_PRAGMAWEAK) || defined(__CYGWIN__)

extern void (*wopt_main_p) (INT argc, char **argv, INT, char **);
#define wopt_main (*wopt_main_p)

extern void (*Wopt_Init_p) ();
#define Wopt_Init (*Wopt_Init_p)

extern void (*Wopt_Fini_p) ();
#define Wopt_Fini (*Wopt_Fini_p)

extern WN* (*Perform_Preopt_Optimization_p) (WN *, WN *);
#define Perform_Preopt_Optimization (*Perform_Preopt_Optimization_p)

extern WN* (*Perform_Global_Optimization_p) (WN *, WN *, ALIAS_MANAGER *);
#define Perform_Global_Optimization (*Perform_Global_Optimization_p)

extern WN* (*Pre_Optimizer_p) (INT32, WN*, DU_MANAGER*, ALIAS_MANAGER*);
#define Pre_Optimizer (*Pre_Optimizer_p)

extern DU_MANAGER* (*Create_Du_Manager_p) (MEM_POOL *);
#define Create_Du_Manager (*Create_Du_Manager_p)

extern void (*Delete_Du_Manager_p) (DU_MANAGER *, MEM_POOL *);
#define Delete_Du_Manager (*Delete_Du_Manager_p)

extern BOOL (*Verify_alias_p) (ALIAS_MANAGER *, WN *);
#define Verify_alias (*Verify_alias_p)

#else

#pragma weak wopt_main
#pragma weak Wopt_Init
#pragma weak Wopt_Fini
#pragma weak Perform_Global_Optimization
#pragma weak Perform_Preopt_Optimization
#pragma weak Pre_Optimizer
#pragma weak Create_Du_Manager
#pragma weak Delete_Du_Manager
#pragma weak Verify_alias

#endif // __linux__


// symbols defined in lno.so
#if defined(__linux__) || defined(_GCC_NO_PRAGMAWEAK) || defined(__CYGWIN__)

extern void (*lno_main_p) (INT, char**, INT, char**);
#define lno_main (*lno_main_p)

extern void (*Lno_Init_p) ();
#define Lno_Init (*Lno_Init_p)

extern void (*Lno_Fini_p) ();
#define Lno_Fini (*Lno_Fini_p)

extern WN* (*Perform_Loop_Nest_Optimization_p) (PU_Info*, WN*, WN*, BOOL);
#define Perform_Loop_Nest_Optimization (*Perform_Loop_Nest_Optimization_p)

#else 

#pragma weak lno_main
#pragma weak Lno_Init
#pragma weak Lno_Fini
#pragma weak Perform_Loop_Nest_Optimization

#endif // __linux__


// symbols defined in ipl.so
#if defined(__linux__) || defined(_GCC_NO_PRAGMAWEAK) || defined(__CYGWIN__)

extern void (*Ipl_Extra_Output_p) (Output_File *);
#define Ipl_Extra_Output (*Ipl_Extra_Output_p)

extern void (*Ipl_Init_p) ();
#define Ipl_Init (*Ipl_Init_p)

extern void (*Ipl_Fini_p) ();
#define Ipl_Fini (*Ipl_Fini_p)

extern void (*ipl_main_p) (INT, char **);
#define ipl_main (*ipl_main_p)

extern void (*Perform_Procedure_Summary_Phase_p) (WN*, DU_MANAGER*,
						  ALIAS_MANAGER*, void*);
#define Perform_Procedure_Summary_Phase (*Perform_Procedure_Summary_Phase_p)

#else

#pragma weak ipl_main
#pragma weak Ipl_Init
#pragma weak Ipl_Fini
#pragma weak Ipl_Extra_Output
#pragma weak Perform_Procedure_Summary_Phase

#endif // __linux__


#include "w2c_weak.h"
#include "w2f_weak.h"


#if defined(_GCC_NO_PRAGMAWEAK) || defined(__CYGWIN__)

extern "C" {
  void        Anl_Process_Command_Line (INT phase_argc, char *phase_argv[],
					INT argc, char *argv[]) { }
  BOOL        Anl_Needs_Whirl2c(void) { return false; }
  BOOL        Anl_Needs_Whirl2f(void) { return false; }
  void        Anl_Init(void) { }
  WN_MAP      Anl_Init_Map(MEM_POOL *id_map_pool) { return NULL; }
  void        Anl_Static_Analysis(WN *pu, WN_MAP id_map) { }
  const char *Anl_File_Path(void)        { return NULL; }
  void        Anl_Fini(void)   { }
  void        Anl_Cleanup(void){ }
}

void        Prompf_Emit_Whirl_to_Source(PU_Info* current_pu,
                                        WN* func_nd) { }

#else

#pragma weak Anl_Cleanup
#pragma weak Anl_Process_Command_Line
#pragma weak Anl_Needs_Whirl2c
#pragma weak Anl_Needs_Whirl2f
#pragma weak Anl_Init
#pragma weak Anl_Init_Map
#pragma weak Anl_Static_Analysis
#pragma weak Anl_Fini

#endif


/* Solaris CC porting
 * Solaris CC #pragma weak can be either followed by one mangled name in
 * string form, or followed by two mangled names. A better way is to 
 * define an Asserta and let these weak functions to equal to Asserta
 */
#if !defined(__GNUC__) && defined(_SOLARIS_SOLARIS)
# pragma weak "__1cbBPrompf_Emit_Whirl_to_Source6FpnHpu_info_pnCWN__v_"
#elif !defined(__GNUC__)
# pragma weak Prompf_Emit_Whirl_to_Source__GP7pu_infoP2WN
#else
# pragma weak Prompf_Emit_Whirl_to_Source__FP7pu_infoP2WN

/* FIXME */
# if defined(_LINUX_LINUX) && !defined(__CYGWIN__) \
      && defined(__GNUC__) && (__GNUC__ >= 3)
  void Prompf_Emit_Whirl_to_Source(PU_Info* current_pu, WN* func_nd) { }
# endif
#endif

extern void Prompf_Emit_Whirl_to_Source(PU_Info* current_pu, WN* func_nd);


static INT ecount = 0;
static BOOL need_wopt_output = FALSE;
static BOOL need_lno_output = FALSE;
static BOOL need_ipl_output = FALSE;
static Output_File *ir_output = 0;

// options stack for PU and region level pragmas
static OPTIONS_STACK *Options_Stack;

static BOOL reset_opt_level = FALSE;
static struct ALIAS_MANAGER *alias_mgr = NULL;

static BOOL Run_Distr_Array = FALSE;
BOOL Run_MemCtr = FALSE;

static BOOL Saved_run_prompf = FALSE; /* TODO: Remove when uses are removed */
static BOOL Saved_run_w2c = FALSE;        /* TODO: Remove */
static BOOL Saved_run_w2f = FALSE;        /* TODO: Remove */
static BOOL Saved_run_w2fc_early = FALSE; /* TODO: Remove */

extern WN_MAP Prompf_Id_Map; /* Maps WN constructs to unique identifiers */

/* Keep track of which optional components are loaded, where we need
 * to do so.
 */
static BOOL   wopt_loaded = FALSE;
extern BOOL   Prompf_anl_loaded; /* Defined in cleanup.c */
extern BOOL   Purple_loaded;     /* Defined in cleanup.c */
extern BOOL   Whirl2f_loaded;    /* Defined in cleanup.c */
extern BOOL   Whirl2c_loaded;    /* Defined in cleanup.c */

extern void *Current_Dep_Graph;
FILE *DFile = stderr;

//Default Sizes (in bytes) for shared ptr, reg, memory handle, etc.
int s_size = 16, p_size = 16, r_size = 4, m_size = 4;

static void
load_components (INT argc, char **argv)
{
    INT phase_argc;
    char **phase_argv;

    if (Run_cg || Run_lno || Run_autopar) {
      // initialize target-info before cg or lno
      Initialize_Targ_Info();
    }

    if (!(Run_lno || Run_wopt || Run_preopt || Run_cg || 
	  Run_prompf || Run_purple || Run_w2c || Run_w2f 
          || Run_w2fc_early || Run_ipl))
      Run_cg = TRUE;		    /* if nothing is set, run CG */

    if (Run_ipl) {
      Run_lno = Run_wopt = Run_cg = Run_w2fc_early
	= Run_prompf = Run_purple = Run_w2c = Run_w2f = FALSE;
    }

    if (Run_prompf || Run_w2fc_early) {
      Get_Phase_Args (PHASE_PROMPF, &phase_argc, &phase_argv);
      load_so("prompf_anl" DSOext, Prompf_Anl_Path, Show_Progress);
      Prompf_anl_loaded = TRUE;
      Anl_Process_Command_Line(phase_argc, phase_argv, argc, argv);
    }

    if (Run_w2f)
    {
      Get_Phase_Args (PHASE_W2F, &phase_argc, &phase_argv);
      load_so("whirl2f" DSOext, W2F_Path, Show_Progress);
      Whirl2f_loaded = TRUE;
      if (Run_prompf)
	W2F_Set_Prompf_Emission(&Prompf_Id_Map);

      W2F_Process_Command_Line(phase_argc, phase_argv, argc, argv);
    }
} /* load_components */


/* phase-specific initializations that need to be done after reading
 * in the global symbol tables.
 */
static void
Phase_Init (void)
{
    char *output_file_name = Obj_File_Name;

    if (Run_Distr_Array      && 
        (Run_w2c || Run_w2f) &&
        !Run_lno             &&
        !Run_wopt            &&
        !Run_cg)
    {
       /* A special case, where it looks as though we only wish to
        * run some early phases and then put out the flist or clist.
        * Disable the turning on of subsequent phases due to the
        * Run_Distr_Array flag.
        */
       Run_Distr_Array = FALSE;
    }
    if ( LNO_Run_Lego_Set && ( LNO_Run_Lego == FALSE ) )
      Run_Distr_Array = FALSE;

    if (Run_w2c || (Run_prompf && Anl_Needs_Whirl2c()))
	W2C_Outfile_Init (TRUE/*emit_global_decls*/);
    if (Run_w2f || (Run_prompf && Anl_Needs_Whirl2f()))
	W2F_Outfile_Init ();
    if (Run_prompf) 
	Anl_Init (); 
	/* Must be done after w2c and w2f */
    if ((Run_lno || Run_preopt) && !Run_cg && !Run_wopt)
	need_lno_output = TRUE;
    if (Run_wopt && !Run_cg)
	need_wopt_output = TRUE;

    if (Run_ipl) {
	need_ipl_output = TRUE;
	need_lno_output = need_wopt_output = FALSE;
    }

    if (output_file_name == 0) {
	if (Src_File_Name)
	    output_file_name = Last_Pathname_Component (Src_File_Name);
	else
	    output_file_name = Irb_File_Name;
    }

    if (need_lno_output) {
	Write_BE_Maps = TRUE;
	ir_output = Open_Output_Info(New_Extension(output_file_name,".N"));
    }
    if (need_wopt_output) {
	Write_ALIAS_CLASS_Map = TRUE;
	Write_BE_Maps = TRUE;
	ir_output = Open_Output_Info(New_Extension(output_file_name,".O"));
    }
    if (need_ipl_output) {
	Write_BE_Maps = FALSE;
	ir_output = Open_Output_Info (Obj_File_Name ?
				      Obj_File_Name :
				      New_Extension(output_file_name, ".o"));
    }
    if (Emit_Global_Data) {
	Write_BE_Maps = FALSE;
	ir_output = Open_Output_Info (Global_File_Name);
    }
	
} /* Phase_Init */


static void
Phase_Fini (void)
{
    CURRENT_SYMTAB = GLOBAL_SYMTAB;

    /* Always finish prompf analysis file, purple, w2c and w2f first */
    if (Run_prompf)
	Anl_Fini();
    if (Run_w2f || (Run_prompf && Anl_Needs_Whirl2f()))
	W2F_Outfile_Fini ();
    Verify_SYMTAB (CURRENT_SYMTAB); /* Verifies global SYmtab */
} /* Phase_Fini */

/* static */ char *
Get_Orig_PU_Name (PU_Info * current_pu)
{
    DST_IDX dst;
    DST_INFO *info;
    DST_SUBPROGRAM *PU_attr;

    dst = PU_Info_pu_dst(current_pu);

    if (DST_IS_NULL (dst)) {
       return ST_name(PU_Info_proc_sym(current_pu));
    }

    info = DST_INFO_IDX_TO_PTR (dst);

    if ( (DST_INFO_tag(info) != DW_TAG_subprogram)
        || DST_IS_declaration(DST_INFO_flag(info)) )
    {
	return ST_name(PU_Info_proc_sym(current_pu));
    }
    PU_attr = DST_ATTR_IDX_TO_PTR(DST_INFO_attributes(info), DST_SUBPROGRAM);
    if (PU_attr->def.name.byte_idx < 0) {
      return NULL;
      /* Why not the following line instead? -- RK 960808
       * return ST_name(PU_Info_proc_sym(current_pu));
       */
    }
    return DST_STR_IDX_TO_PTR(DST_SUBPROGRAM_def_name(PU_attr));
}

static void
Save_Cur_PU_Name (char *name, INT rid_id)
{
    if ( Cur_PU_Name == NULL ) {
	/* ST_name will return a pointer into the symbol table, which is
	 * mmap-ed. This causes a problem in the error message routines
	 * when an unexpected signal occurs, because as part of the cleanup
	 * files are closed. To fix the problem we just allocate some
	 * memory and make a copy.
	 * Allocate 8 extra bytes to leave room for RGN suffix.
	 */
	Cur_PU_Name = TYPE_MEM_POOL_ALLOC_N(char, &MEM_pu_nz_pool, 
		strlen(name) + 8);
	Cur_PU_Name = strcpy(Cur_PU_Name, name);
    }
    if (rid_id != 0) {
	/* add RGN suffix */
	sprintf(Cur_PU_Name,"%s.RGN%03d", name, rid_id);
    }
    else if (strlen(name) != strlen(Cur_PU_Name)) {
	/* clear RGN suffix */
	Cur_PU_Name = strcpy(Cur_PU_Name, name);
    }
}


//  Adjust/Lower optimization level based on
//   1. size of PU and Olimit
//   2. existence of non-ANSI setjmp calls
//
static WN *
Adjust_Opt_Level (PU_Info* current_pu, WN *pu, char *pu_name)
{
    INT new_opt_level = 0;
    COMPUTE_PU_OLIMIT;

    if (Get_Trace(TKIND_INFO, TINFO_STATS)) {
	/* Print Olimit stats to trace file: */
	INT PU_Var_Cnt = ST_Table_Size (CURRENT_SYMTAB) +
	    PREG_Table_Size (CURRENT_SYMTAB); 
	fprintf (TFile, "PU_Olimit for %s is %d (bbs=%d,stms=%d,vars=%d)\n", 
		pu_name, PU_Olimit, PU_WN_BB_Cnt, PU_WN_Stmt_Cnt, PU_Var_Cnt);
    }

    if ((Opt_Level > 0 || Run_autopar) && PU_Olimit > Olimit && !Olimit_opt) {
	if (Show_OPT_Warnings)
	  ErrMsg (EC_Olimit_Exceeded, pu_name, PU_Olimit);
	reset_opt_level = TRUE;
    }
    if (((Opt_Level > 0 || Run_autopar) || Olimit_opt)
      && Query_Skiplist ( Optimization_Skip_List, Current_PU_Count() ) )
    {
	if (Show_OPT_Warnings)
	  ErrMsg (EC_Not_Optimized, pu_name, Current_PU_Count() );
	reset_opt_level = TRUE;
    } 
    if (/* !LANG_Ansi_Setjmp_On && */ 
      /* 1. Cannot check LANG_Ansi_Setjmp_On because IPA does not pass -LANG group.
	 2. The ST_pu_calls_setjmp is not set unless LANG_Ansi_Setjmp_On = false */
	PU_calls_setjmp (Get_Current_PU ())) {
      reset_opt_level = TRUE;
      new_opt_level = 1;
      ErrMsg (EC_Not_Ansi_Setjmp, pu_name, Current_PU_Count(), new_opt_level );
    } 
    if (reset_opt_level) {
	Opt_Level = new_opt_level;
	Run_lno = Run_preopt = Run_wopt = Run_autopar = FALSE;
	alias_mgr = NULL;
	Olimit_opt = FALSE;
        if (Run_prompf) 
	  Prompf_Emit_Whirl_to_Source(current_pu, pu);
    }

    return pu;
} /* Adjust_Opt_Level */

/* Misc. processing after LNO is done, PU exists as a whole during this
   procedure */
static void
Post_LNO_Processing (PU_Info *current_pu, WN *pu)
{
    BOOL is_user_visible_pu = (CURRENT_SYMTAB == GLOBAL_SYMTAB + 1) || 
                              ((Language == LANG_F90) &&
			       (CURRENT_SYMTAB == GLOBAL_SYMTAB + 2) &&
			       (!Is_Set_PU_Info_flags(current_pu, PU_IS_COMPILER_GENERATED))) ;
    
    /* Only run w2c and w2f on top-level PUs, unless otherwise requested.
     */
    if (Run_w2c && !Run_w2fc_early && !Run_prompf) {
	if (W2C_Should_Emit_Nested_PUs() || is_user_visible_pu) {
	    W2C_Outfile_Translate_Pu(pu, TRUE/*emit_global_decls*/);
	}
    }
    if (Run_w2f && !Run_w2fc_early && !Run_prompf) {
	if (W2F_Should_Emit_Nested_PUs() || is_user_visible_pu) {
              if (PU_need_unparsed(ST_pu(WN_st(pu)))) 
	          W2F_Outfile_Translate_Pu(pu);
	}
    }

    /* only write .N file for PU, no need to replace region because
       REGION_remove_and_mark does nothing for pu (rwn is the pu) */
    if (need_lno_output) {
	Set_PU_Info_tree_ptr(current_pu, pu);
	Write_PU_Info(current_pu);
	Verify_SYMTAB (CURRENT_SYMTAB);
    }

} /* Post_LNO_Processing */


extern "C" {
  extern void Process_Fill_Align_Pragmas (WN* func_wn);
  extern void Rewrite_Pragmas_On_Structs (WN* block_wn, WN* wn);
}

/***********************************************************************
 *
 * Find all EH regions in the PU, and mark their INITOs as used.
 *
 ***********************************************************************/
static void Update_EHRegion_Inito_Used (WN *wn) {
  if (!wn) return;
  
  OPERATOR opr = WN_operator(wn);

  if (opr == OPR_REGION && WN_ereg_supp(wn)) {
    INITO_IDX ino_idx = WN_ereg_supp(wn);
    ST *st = INITO_st(ino_idx);
    Clear_ST_is_not_used(st);
  }

  // now recurse
  if (opr == OPR_BLOCK) {
    WN *kid = WN_first (wn);
    while (kid) {
      Update_EHRegion_Inito_Used(kid);
      kid = WN_next(kid);
    }
  } else {
    for (INT kidno=0; kidno<WN_kid_count(wn); kidno++) {
      Update_EHRegion_Inito_Used(WN_kid(wn,kidno));
    }
  }
}

/***********************************************************************
 *
 * This pass is called after preopt+lno+mplowering. 
 * Any of those passes may have deleted EH-regions, but left the
 * INITO sts for those regions hanging around.
 * This pass will search for all used INITOs, and mark the rest unused.
 *
 ***********************************************************************/
static void Update_EHRegion_Inito (WN *pu) {
  INT i;
  INITO *ino;

  // first mark all EH-region STs unused.
  FOREACH_INITO (CURRENT_SYMTAB, ino, i) {
    ST *st = INITO_st(ino);
    if (ST_sclass(st) == SCLASS_EH_REGION ||
        ST_sclass(st) == SCLASS_EH_REGION_SUPP) {
      Set_ST_is_not_used(st);
    }
  }

  // now find INITO sts that are referenced in WHIRL,
  // and mark them used.
  Update_EHRegion_Inito_Used (pu);
}

static void
Backend_Processing (PU_Info *current_pu, WN *pu)
{
    {
	/* Always process the first PU for fill-align, since that one contains
	 * the pragmas for global symbols. And, because of IPA, we cannot
	 * depend on the SYMTAB_id being 1 for the first PU.
	 */
	static BOOL done_first_pu = FALSE;
	BOOL needs_fill_align_lowering =
	    PU_needs_fill_align_lowering (Get_Current_PU ());
	if (needs_fill_align_lowering || !done_first_pu) {
	    Process_Fill_Align_Pragmas (pu);
	    done_first_pu = TRUE;
	}
    }

    PU_adjust_addr_flags(Get_Current_PU_ST(), pu);

    if (Run_MemCtr)
	MemCtr_Add (pu);

    /* Make sure that RETURN_VAL nodes, Return_Val_Preg references and
       MLDID/MSTID nodes have been lowered. This requires its own pass
       because it may have to go back to change the previous CALL statement
       to add a fake parameter. */
    if (WHIRL_Return_Val_On || WHIRL_Mldid_Mstid_On) {
        Is_True(WHIRL_Return_Val_On && WHIRL_Mldid_Mstid_On,
	        ("-INTERNAL:return_val and -INTERNAL:mldid_mstid must be on the same time"));
    }

    /* First round output (.N file, w2c, w2f, etc.) */
    Set_Error_Phase ( "Post LNO Processing" );
    Post_LNO_Processing (current_pu, pu);

    return;
} /* Backend_Processing */

static WN *
Preprocess_PU (PU_Info *current_pu)
{
  WN *pu = NULL;

  Initialize_PU_Stats ();  /* Needed for Olimit as well as tracing */

  Current_PU_Info = current_pu;
  MEM_POOL_Push(MEM_pu_nz_pool_ptr);
  MEM_POOL_Push(MEM_pu_pool_ptr);

  BOOL is_mp_nested_pu = FALSE;

  /* read from mmap area */
  Start_Timer ( T_ReadIR_CU );
  // The current PU could already be memory as happens when the
  // compiler creates it during back end compilation of an earlier PU. 
  if (PU_Info_state (current_pu, WT_TREE) != Subsect_InMem) {
    Read_Local_Info (MEM_pu_nz_pool_ptr, current_pu);
  } else {			    /* retrieve transferred maps */
      // change some globals to define current_pu as the current PU
    Current_Map_Tab = PU_Info_maptab(current_pu);
    Current_pu = &PU_Info_pu(current_pu);
    CURRENT_SYMTAB = PU_lexical_level(*Current_pu);
    if ((PU_is_nested_func(*Current_pu) && PU_mp(*Current_pu)) ||
        Is_Set_PU_Info_flags(current_pu, PU_IS_DRA_CLONE)) {
      is_mp_nested_pu = TRUE;
      // hack to restore nested PU's symtab
      Restore_Local_Symtab(current_pu);
    } else {
      Is_True(FALSE, ("Robert doesn't understand where symtabs come from"));
    }
  }

  BE_symtab_alloc_scope_level(CURRENT_SYMTAB);
  Scope_tab[CURRENT_SYMTAB].st_tab->Register(*Be_scope_tab[CURRENT_SYMTAB].be_st_tab);

  /* NOTE: "pu" is not defined until this point, since the actual
   * (WN *) is calculated by Read_Local_Info().
   */
  pu = PU_Info_tree_ptr(current_pu);

  /* Disable all prompf processing for PUs generated by the compiler,
   * such as cloned subroutines, with exception of mp routines which
   * we do want to process (we just don't want to do the static
   * analysis part for them).  
   *
   * TODO:  Disable Anl_Static_Analysis() when this condition holds,
   * but generate the subroutines in the .m file and have the cloner
   * assign an ID map for the subroutine with unique ID numbers.
   */
  if (!Saved_run_prompf                                          &&
      Run_prompf                                                 &&
      Is_Set_PU_Info_flags(current_pu, PU_IS_COMPILER_GENERATED) &&
      !PU_mp (Get_Current_PU ())) {
    Saved_run_prompf = Run_prompf;
    Saved_run_w2c = Run_w2c;
    Saved_run_w2f = Run_w2f;
    Saved_run_w2fc_early = Run_w2fc_early;
    Run_prompf = FALSE;
    Run_w2c = FALSE;
    Run_w2f = FALSE;
    Run_w2fc_early = FALSE;
  }

  /* store original pu name */
  Orig_PU_Name = Get_Orig_PU_Name(current_pu);
  Save_Cur_PU_Name(ST_name(PU_Info_proc_sym(current_pu)), 0);

  Set_Current_PU_For_Trace(ST_name(PU_Info_proc_sym(current_pu)), 
			   Current_PU_Count());

  Stop_Timer (T_ReadIR_CU);
  Check_for_IR_Dump(TP_IR_READ,pu,"IR_READ");

  if (Show_Progress) {
    fprintf(stderr, "Compiling %s(%d)\n",
	    ST_name(PU_Info_proc_sym(current_pu)),
	    Current_PU_Count());
  }

  if (Get_Trace(TP_REGION,TT_REGION_ALL)) {
    fprintf(TFile,"===== BE driver, PU loop: PU %s(%d)\n",
	    ST_name(PU_Info_proc_sym(current_pu)),Current_PU_Count());
  }

  if (Tlog_File) {
    fprintf(Tlog_File,"BEGIN %s\n",ST_name(PU_Info_proc_sym(current_pu)));
  }

  WN_Mem_Push ();

  pu = Adjust_Opt_Level (current_pu, pu, ST_name(PU_Info_proc_sym(current_pu)));

  return pu;
} /* Preprocess_PU */

static void
Postprocess_PU (PU_Info *current_pu)
{
  if (Tlog_File) {
    fprintf (Tlog_File, "END %s\n", ST_name(PU_Info_proc_sym(current_pu)));
  }

  Current_Map_Tab = PU_Info_maptab(current_pu);
 
  // Delete alias manager after CG finished ? PV 525127, 527977
  WN_Mem_Pop (); // WN pool


  SYMTAB_IDX scope_level = PU_lexical_level(PU_Info_pu(current_pu));

  Scope_tab[scope_level].st_tab->
    Un_register(*Be_scope_tab[scope_level].be_st_tab);
  Be_scope_tab[scope_level].be_st_tab->Clear();

  Free_Local_Info(current_pu); // deletes all maps
  MEM_POOL_Pop(MEM_pu_nz_pool_ptr);
  MEM_POOL_Pop(MEM_pu_pool_ptr);

  /* Re-enable prompf processing if relevant.  
   *
   * TODO:  Disable Anl_Static_Analysis() when this condition holds,
   * but generate the subroutines in the .m file and have the cloner
   * assign an ID map for the subroutine with unique ID numbers.
   */
  if (Saved_run_prompf) {
    Run_prompf = Saved_run_prompf;
    Run_w2c = Saved_run_w2c;
    Run_w2f = Saved_run_w2f;
    Run_w2fc_early = Saved_run_w2fc_early;
    Saved_run_prompf = FALSE;
    Saved_run_w2c = FALSE;
    Saved_run_w2f = FALSE;
    Saved_run_w2fc_early = FALSE;
  }
} /* Postprocess_PU */

/* compile each PU through all phases before going to the next PU */
static void
Preorder_Process_PUs (PU_Info *current_pu)
{
  INT orig_opt_level = Opt_Level;
  BOOL orig_run_lno = Run_lno;
  BOOL orig_run_preopt = Run_preopt;
  BOOL orig_run_wopt = Run_wopt;
  BOOL orig_olimit_opt = Olimit_opt;

  WN *pu;
  Start_Timer(T_BE_PU_CU);

  pu = Preprocess_PU(current_pu);


  // Quick! Before anyone risks creating any PREGs in the back end,
  // register the back end's PREG table with the main PREG table so
  // they will grow together as PREGs are created.
  Scope_tab[CURRENT_SYMTAB].preg_tab->Register(Be_preg_tab);

  WN_verifier(pu);

  Verify_SYMTAB (CURRENT_SYMTAB);

  Backend_Processing (current_pu, pu);
  Verify_SYMTAB (CURRENT_SYMTAB);

  if (reset_opt_level) {
    Opt_Level = orig_opt_level;
    Run_lno = orig_run_lno;
    Run_preopt = orig_run_preopt;
    Run_wopt = orig_run_wopt;
    reset_opt_level = FALSE;
    Olimit_opt = orig_olimit_opt;
  }

  Scope_tab[CURRENT_SYMTAB].preg_tab->Un_register(Be_preg_tab);
  Be_preg_tab.Clear();

  Stop_Timer(T_BE_PU_CU);
  Finish_BE_Timing ( Tim_File, ST_name(PU_Info_proc_sym(current_pu)) );
  Advance_Current_PU_Count();

  Cur_PU_Name = NULL;		// memory will not be leaked; eventual
                                // pop occurs in Postprocess_PU's call
                                // to WN_MEM_Pop. Reset here is
                                // required so Save_Cur_PU_Name will
                                // not misbehave.

  // Print miscellaneous statistics to trace file:
  Print_PU_Stats ();

  // Now recursively process the child PU's.

  for (PU_Info *child = PU_Info_child(current_pu);
       child != NULL;
       child = PU_Info_next(child)) {
    Preorder_Process_PUs(child);
  }

  Postprocess_PU (current_pu);
} /* Preorder_Process_PUs */

static void Print_Tlog_Header(INT argc, char **argv)
{
  INT i;
  if (Get_Trace(TP_PTRACE1, TP_PTRACE1_NOHDR))
    return; 
  fprintf(Tlog_File,"1.0\n"); /* initial version number */
  fprintf(Tlog_File,"{ ");
  for (i=0; i<argc; i++)
    fprintf(Tlog_File,"%s ", argv[i]);
  fprintf(Tlog_File,"}\n");
}


#define FEEDBACK_PATH_MAXLEN 1024


// Provide a place to stop after components are loaded
extern "C" {
  void be_debug(void) {}
}


void RiceWhirl2f (INT * argc, char * **argv);



INT
main (INT argc, char **argv)
{
  INT local_ecount, local_wcount;
  PU_Info *pu_tree;

  
  setlinebuf (stdout);
  setlinebuf (stderr);
  Handle_Signals ();
  MEM_Initialize ();
  Cur_PU_Name = NULL;
  Init_Error_Handler ( 100 );
  Set_Error_Line ( ERROR_LINE_UNKNOWN );
  Set_Error_File ( NULL );
  Set_Error_Phase ( "Back End Driver" );
  Preconfigure ();

  RiceWhirl2f ( & argc, & argv);

  Process_Command_Line (argc, argv);
  
  if (Inhibit_EH_opt && Opt_Level > 1) Opt_Level = 1;
  Reset_Timers ();
  Start_Timer(T_BE_Comp);
  Prepare_Source ();
  Initialize_Stats ();

  Configure ();
  Configure_Source(NULL); /* Most configuration variables are set here */
#ifdef Is_True_On
  if (Get_Trace (TKIND_ALLOC, TP_MISC)) {
    MEM_Tracing_Enable();
  }
#endif
  if ( List_Enabled ) {
    Prepare_Listing_File ();
    List_Compile_Options ( Lst_File, "", FALSE, List_All_Options, FALSE );
  }

  Init_Operator_To_Opcode_Table();

  
 
  /* decide which phase to call */
  load_components (argc, argv);
  be_debug();

  MEM_POOL_Push (&MEM_src_pool);
  MEM_POOL_Push (&MEM_src_nz_pool);
  if ( Show_Progress ) {
    fprintf ( stderr, "Compiling %s (%s) -- Back End\n",
	     Src_File_Name, Irb_File_Name );
    fflush ( stderr );
  }
  Set_Error_Source (Src_File_Name);

  // Push initial file level options
  Options_Stack = CXX_NEW(OPTIONS_STACK(&MEM_src_nz_pool), &MEM_src_nz_pool);
  Options_Stack->Push_Current_Options();

  Start_Timer (T_ReadIR_Comp);
  
  if (Read_Global_Data) {
	// get input from two separate files
  	Irb_File = (FILE *)Open_Global_Input (Global_File_Name);
  	Irb_File = (FILE *)Open_Local_Input (Irb_File_Name);
  }
  else {
  	Irb_File = (FILE *)Open_Input_Info (Irb_File_Name);
  }
  Initialize_Symbol_Tables (FALSE);
  New_Scope (GLOBAL_SYMTAB, Malloc_Mem_Pool, FALSE);
  pu_tree = Read_Global_Info (NULL);
  Stop_Timer (T_ReadIR_Comp);

  Initialize_Special_Global_Symbols ();

  // if compiling an ipa-generated file, do not instrument phases that
  // have already been done at ipl time.
  if (FILE_INFO_ipa (File_info)) {
      if (Instrumentation_Enabled &&
	  Instrumentation_Phase_Num <= PROFILE_PHASE_IPA_CUTOFF) {
	  Instrumentation_Enabled = FALSE;
	  Instrumentation_Phase_Num = PROFILE_PHASE_NONE;
      }
  } 
 
  /* initialize the BE symtab. Note that w2cf relies on the BE_ST */
  /* during Phase_Init and Phase_Fini                             */

  BE_symtab_initialize_be_scopes();
  BE_symtab_alloc_scope_level(GLOBAL_SYMTAB);
  SYMTAB_IDX scope_level;
  for (scope_level = 0;
       scope_level <= GLOBAL_SYMTAB;
       ++scope_level) {
    // No need to deal with levels that don't have st_tab's. Currently
    // this should be only zero.
    if (Scope_tab[scope_level].st_tab != NULL) {
      Scope_tab[scope_level].st_tab->
	Register(*Be_scope_tab[scope_level].be_st_tab);
    }
    else {
      Is_True(scope_level == 0,
	      ("Nonexistent st_tab for level %d", scope_level));
    }
  }

  Phase_Init ();
 
  if (Run_preopt || Run_wopt || Run_lno || Run_Distr_Array || Run_autopar 
	|| Run_cg) {
    Set_Error_Descriptor (EP_BE, EDESC_BE);
  }
 /* For UPC - disable  optimizations for the time being */
  if (Compile_Upc) {
    Run_lno = 0;
    if (Run_w2c) {
      //the symbols should already be in the symbol table, need to find them
      Find_Upc_Vars();
    }
  }

  if (Tlog_File)
    Print_Tlog_Header(argc, argv);


  for (PU_Info *current_pu = pu_tree;
       current_pu != NULL;
       current_pu = PU_Info_next(current_pu)) {
    Preorder_Process_PUs(current_pu);
  }

  if(Compile_Upc && !Run_w2c) 
    Upc_Lower_SymbolTable();

  /* Terminate stdout line if showing PUs: */
  if (Show_Progress) {
    fprintf (stderr, "\n");
    fflush (stderr);
  }

  Phase_Fini ();

  /* free the BE symtabs. w2cf requires BE_ST in Phase_Fini */

  Is_True(scope_level == GLOBAL_SYMTAB + 1,
	  ("scope_level must be GLOBAL_SYMTAB + 1, left from earlier loop"));

  do {
    --scope_level;
    // No need to deal with levels that don't have st_tab's. Currently
    // this should be only zero.
    if (Scope_tab[scope_level].st_tab != NULL) {
      Scope_tab[scope_level].st_tab->
	Un_register(*Be_scope_tab[scope_level].be_st_tab);
      Be_scope_tab[scope_level].be_st_tab->Clear();
    }
    else {
      Is_True(scope_level == 0,
	      ("Nonexistent st_tab for level %d", scope_level));
    }
  } while (scope_level != 0);

  BE_symtab_free_be_scopes();

  
  if (need_wopt_output || need_lno_output || need_ipl_output) {
    Write_Global_Info (pu_tree);
    if (need_ipl_output)
      Ipl_Extra_Output (ir_output);
    Close_Output_Info ();
  }
  else if (Emit_Global_Data) {
    Write_Global_Info (NULL);	/* even if dummy pu, don't write any pu's */
    Close_Output_Info ();
  }

  /* Print miscellaneous statistics to trace file: */
  Print_Total_Stats ();
  if ((Opt_Level > 0 || Run_autopar) 
	&& Max_Src_Olimit > Olimit && !Olimit_opt && Show_OPT_Warnings) {
    ErrMsg (EC_File_Olimit_Exceeded, Max_Src_Olimit);
  }
    
  Stop_Timer(T_BE_Comp);
  Finish_Compilation_Timing ( Tim_File, Src_File_Name );

  MEM_POOL_Pop ( &MEM_src_pool );
  MEM_POOL_Pop ( &MEM_src_nz_pool );
#ifdef Is_True_On
	if (Get_Trace (TKIND_ALLOC, TP_MISC)) {
	    fprintf (TFile, "\n%s\tMemory allocation information after be\n", DBar);
	    MEM_Trace ();
	}
#endif

  /* If we've seen errors, note them and terminate: */
  if ( Get_Error_Count ( &local_ecount, &local_wcount ) ) {
    ecount += local_ecount;
  }
 
  if ( ecount > 0 ) {
    Terminate(Had_Internal_Error() ? RC_INTERNAL_ERROR : RC_NORECOVER_USER_ERROR) ;
  }

  /* Close and delete files as necessary: */
  Cleanup_Files ( TRUE, FALSE );

  
  exit ( RC_OKAY );
  /*NOTREACHED*/

  
} /* main */


static BOOL
Has_Extension__1 (char *name,	/* The filename to check */
	       char *ext)	/* The extension to look for */
{
  INT16 nlen = strlen(name);
  INT16 elen = strlen(ext);

  /* If ext is longer than name, no chance: */
  if ( elen > nlen ) return FALSE;

  /* Otherwise compare the tail of name to ext: */
  return ( strcmp ( &name[nlen-elen], ext ) == 0 );
} /* Has_Extension__1 */


static char *libpath[3] = 
{"LD_LIBRARY_PATH",
 "LD_LIBRARYN32_PATH",
 "LD_LIBRARY64_PATH"
};
static const char * const errstring = "%s: can't allocate memory\n";

void RiceWhirl2f (INT * _argc, char * **_argv) {
  char path[PATH_MAX];
  char *p;
  char *env;
  char * myname;
  int argidx, i, len;
  char **new_argv;
  BOOL dash_fB_option = FALSE;
  char *newlibpath[3];
  int argc = *_argc;
  char **argv = *_argv;
  myname= Last_Pathname_Component (argv[0]);

  if (myname && strcmp(myname, "whirl2f90") == 0) {

    // set Run_w2f
    Run_w2f=1;
    strcpy (path, argv[0]);
    if (p = strrchr(path, '/'))
      p[0] = 0;
    else
      strcpy (path, ".");

    for (i = 0; i<3; i++)
      {
	len = strlen (path) + 1;
	len += strlen (libpath[i]) + 1;    /* env. variable name plus '=' */

	env = getenv (libpath[i]);

	if (env) {
	  len += strlen (env) + 1;    /* old path plus ':' */

	  newlibpath[i] = (char *) malloc (len);
	  if (newlibpath[i] == 0) {
	    fprintf (stderr, errstring, argv[0]);
	    exit(RC_NORECOVER_USER_ERROR);
	  }

	  sprintf (newlibpath[i], "%s=%s:%s", libpath[i], env, path);
	} else {
	  newlibpath[i] = (char *) malloc (len);
	  if (newlibpath[i] == 0) {
	    fprintf (stderr, errstring, argv[0]);
	    exit(RC_NORECOVER_USER_ERROR);
	  }

	  sprintf (newlibpath[i], "%s=%s", libpath[i], path);
	}
      } /* For each libpath kind */
    for (i = 0; i<3; i++)
      putenv (newlibpath[i]);
    // strcat (path, "/whirl2f");


    /* Copy the argument list into a new list of strings, with a spare
     * element for a missing -fB option.
     */
    new_argv = (char **) malloc((argc+2)*sizeof(char *));
    for (argidx = 0; argidx < argc; argidx++)
      {
	new_argv[argidx] = (char *) malloc(strlen(argv[argidx]) + 1);
	new_argv[argidx] = strcpy(new_argv[argidx], argv[argidx]);
	if (new_argv[argidx][0] == '-' &&
	    new_argv[argidx][1] == 'f' &&
	    new_argv[argidx][2] == 'B')
	  dash_fB_option = TRUE;
      }

    if (!dash_fB_option)
      {
	/* Create a "-fB" option, provided the file-argument (only argument
	 * not preceded by a '-') represents the WHIRL file if suffixed by
	 * ".B", ".I", ".N" or ".o".
	*/
	argidx = argc-1;
	while (argidx > 0)
	  {
	    if (new_argv[argidx][0] != '-' && /* A file argument */
		(Has_Extension__1(new_argv[argidx], ".B") ||
		 Has_Extension__1(new_argv[argidx], ".I") ||
		 Has_Extension__1(new_argv[argidx], ".N") ||
		 Has_Extension__1(new_argv[argidx], ".o")))
	      {
		/* A file argument representing the WHIRL input file.  We need
		 * to change this around a little bit.  Put this filename under
		 * a "-fB,filename" option and add a new filename with the
		 * suffix substituted by ".f".
		 */
		dash_fB_option = TRUE;
		new_argv[argc] = (char *) malloc(strlen(new_argv[argidx]) + 5);
		(void)strcpy(new_argv[argc], "-fB,");
		(void)strcpy(&new_argv[argc][4], new_argv[argidx]);
		argc++;
	  
		new_argv[argidx][strlen(new_argv[argidx])-1] = 'f';
		argidx = 1; /* We are done! */
	      }
	    argidx--;
	  } /*while*/
      } /*if (!dash_fB_option)*/
    new_argv[argc] = NULL;

    // change the program name to be whirl2f
    new_argv[0][ strlen(new_argv[0])-2 ] = 0;

    * _argc = argc;
    * _argv = new_argv;
  }
}

