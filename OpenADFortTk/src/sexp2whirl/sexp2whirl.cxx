// -*-Mode: C++;-*-
// $Header: /m_home/m_utkej/Argonne/cvs2svn/cvs/OpenADFortTk/src/sexp2whirl/sexp2whirl.cxx,v 1.6 2007-10-08 18:28:33 utke Exp $
#include <sexp.h>

#include "Open64IRInterface/Open64BasicTypes.h"
#include "Open64IRInterface/WhirlGlobalStateUtils.h"

#include "SexpTags.h"
#include "sexputil.h"

#include "sexp2whirl.h"
using sexp2whirl::ErrIR;
#include "sexp2wn.h"
#include "sexp2symtab.h"

static PU_Info*
xlate_IR(sexp_t* ir_sx, int flags);

static PU_Info*
xlate_PUForest(sexp_t* pu_forest_sx, int flags);

static PU_Info*
xlate_PUTree(sexp_t* pu_tree_sx, int flags);

static PU_Info* 
xlate_PU(sexp_t* pu_sx, int flags);

static WN* 
xlate_WN(sexp_t* ast_sx, int flags);

//***************************************************************************
// Implementation of interface routines
//***************************************************************************

PU_Info*
sexp2whirl::TranslateIR(sexp_t* ir, int flags)
{
  return xlate_IR(ir, flags);
}


const char* 
sexp2whirl::ErrIR(sexp_t* ir, int flags)
{
  const int sz = 1 << 20;
  char* buf = new char[sz];
  
  if (ir) {
    int rval = print_sexp(buf, sz, ir);
  } 
  else {
    strcpy(buf, "null");
  }
  return buf; // must be deleted, but ignore for generating errors
}


void 
sexp2whirl::DumpIR(sexp_t* ir, int flags)
{
  const int sz = 1 << 20;
  char* buf = new char[sz];
  
  int rval = print_sexp(buf, sz, ir);
  std::cout << buf << std::endl;
  delete[] buf;
}


//***************************************************************************
// Helper routines
//***************************************************************************

// xlate_IR: Translate the whole WHIRL_IR, return a PU_FOREST and set
// GBL_SYMTAB state.
PU_Info*
xlate_IR(sexp_t* ir_sx, int flags) 
{
  using namespace sexp;
  
  // Sanity check
  FORTTK_ASSERT(ir_sx && is_list(ir_sx), 
		fortTkSupport::Diagnostics::UnexpectedInput << ErrIR(ir_sx));

  sexp_t* tag_sx = get_elem0(ir_sx);
  const char* tagstr = get_value(tag_sx);
  FORTTK_ASSERT(tag_sx && strcmp(tagstr, SexpTags::WHIRL) == 0,
		fortTkSupport::Diagnostics::UnexpectedInput << ErrIR(tag_sx));

  // Translate GBL_SYMTAB and PU_FOREST
  sexp_t* gbl_symtab_sx = get_elem1(ir_sx);
  sexp_t* pu_forest_sx  = get_elem2(ir_sx);
  FORTTK_ASSERT(gbl_symtab_sx, 
		fortTkSupport::Diagnostics::UnexpectedInput << ErrIR(gbl_symtab_sx));
  FORTTK_ASSERT(pu_forest_sx, 
		fortTkSupport::Diagnostics::UnexpectedInput << ErrIR(pu_forest_sx));
  
  sexp2whirl::TranslateGlobalSymbolTables(gbl_symtab_sx, flags);
  PU_Info* pu_forest = xlate_PUForest(pu_forest_sx, flags);
  
  return pu_forest;
}


// xlate_PUForest: Translate and return a PU_FOREST, *assuming* that
// GBL_SYMTAB has already been translated.
PU_Info*
xlate_PUForest(sexp_t* pu_forest_sx, int flags)
{
  using namespace sexp;

  // Sanity check
  FORTTK_ASSERT(pu_forest_sx && is_list(pu_forest_sx), 
		fortTkSupport::Diagnostics::UnexpectedInput);  
  
  sexp_t* tag_sx = get_elem0(pu_forest_sx);
  const char* tagstr = get_value(tag_sx);
  FORTTK_ASSERT(tag_sx && strcmp(tagstr, SexpTags::PU_FOREST) == 0,
		fortTkSupport::Diagnostics::UnexpectedInput);

  MEM_POOL_Push(MEM_pu_nz_pool_ptr);
  MEM_POOL_Push(MEM_pu_pool_ptr);
  
  // Translate the list of PU_TREEs
  PU_Info* pu_forest = NULL;
  sexp_t* cur_pu_tree_sx = get_elem1(pu_forest_sx);
  if (cur_pu_tree_sx) {
    // Translate head of the PU_TREE list
    pu_forest = xlate_PUTree(cur_pu_tree_sx, flags);
    PU_Info* cur_pu_tree = pu_forest;
    
    // Translate rest of the PU_TREE list
    for (cur_pu_tree_sx = get_next(cur_pu_tree_sx) ; cur_pu_tree_sx; 
	 cur_pu_tree_sx = get_next(cur_pu_tree_sx)) {
      PU_Info* pu_tree = xlate_PUTree(cur_pu_tree_sx, flags);

      PU_Info_next(cur_pu_tree) = pu_tree;
      cur_pu_tree = pu_tree;
    }
  }
  
  return pu_forest;
}


// xlate_PUTree: Translate and return a PU_TREE (a PU and all
// children), *assuming* that GBL_SYMTAB has already been translated.
PU_Info*
xlate_PUTree(sexp_t* pu_tree_sx, int flags)
{
  using namespace sexp;
  
  if (!pu_tree_sx) { return NULL; }

  // Sanity check
  FORTTK_ASSERT(is_list(pu_tree_sx), fortTkSupport::Diagnostics::UnexpectedInput);  
  
  sexp_t* tag_sx = get_elem0(pu_tree_sx);
  const char* tagstr = get_value(tag_sx);
  FORTTK_ASSERT(tag_sx && strcmp(tagstr, SexpTags::PU_TREE) == 0,
		fortTkSupport::Diagnostics::UnexpectedInput);
  
  // Translate PU
  sexp_t*  pu_sx = get_elem1(pu_tree_sx);
  PU_Info* pu = xlate_PU(pu_sx, flags);
  
  // Recursively translate PUs children
  PU_Info* cur_child = NULL;
  for (sexp_t* child_sx = get_elem2(pu_tree_sx); child_sx; 
       child_sx = get_next(child_sx)) {
    PU_Info* child = xlate_PU(child_sx, flags);
    if (cur_child) {
      PU_Info_next(cur_child) = child;
    }
    else {
      PU_Info_child(pu) = child;
    }
    cur_child = child;
  }

  if (cur_child) { // if there was at least one child
    Set_PU_Info_flags(pu, PU_HAS_NESTED_PU);
  }
  return pu;
}


// xlate_PU: Translate and return a PU, setting PU_SYMTAB state but
// *assuming* that GBL_SYMTAB has already been translated.
PU_Info*
xlate_PU(sexp_t* pu_sx, int flags)
{ 
  using namespace sexp;

  // Sanity check
  FORTTK_ASSERT(pu_sx && is_list(pu_sx), fortTkSupport::Diagnostics::UnexpectedInput);
  
  sexp_t* tag_sx = get_elem0(pu_sx);
  const char* tagstr = get_value(tag_sx);
  FORTTK_ASSERT(tag_sx && strcmp(tagstr, SexpTags::PU) == 0,
		fortTkSupport::Diagnostics::UnexpectedInput);

  // Translate PU_SYMTAB and WHIRL_AST
  sexp_t* pu_sym_sx    = get_elem1(pu_sx);
  sexp_t* pu_symtab_sx = get_elem2(pu_sx);
  sexp_t* ast_sx       = get_elem3(pu_sx);

  ST_IDX st_idx = sexp2whirl::GetWhirlSymRef(pu_sym_sx);
  CURRENT_SYMTAB = PU_lexical_level(&St_Table[st_idx]);
  
  sexp2whirl::TranslateLocalSymbolTables(pu_symtab_sx, CURRENT_SYMTAB, flags);
  
  WN* ast = xlate_WN(ast_sx, flags);

  // Create the PU_Info
  PU_Info* pu = TYPE_MEM_POOL_ALLOC(PU_Info, MEM_pu_pool_ptr);
  PU_Info_init(pu);
  // N.B. 'next', 'child' and PU_HAS_NESTED_PU are set by caller routines
  PU_Info_proc_sym(pu) = st_idx;
  Set_PU_Info_tree_ptr(pu, ast);
  Set_PU_Info_state(pu, WT_TREE,     Subsect_InMem);
  Set_PU_Info_state(pu, WT_SYMTAB,   Subsect_InMem);
  Set_PU_Info_state(pu, WT_PROC_SYM, Subsect_InMem);
  Set_PU_Info_pu_dst(pu, DST_INVALID_IDX);
  Set_PU_Info_cu_dst(pu, DST_INVALID_IDX);
  PU_Info_maptab(pu) = WN_MAP_TAB_Create(MEM_pu_pool_ptr);

  // Set and save global state (cf. PU_SetGlobalState)
  //Advance_Current_PU_Count();
  Current_Map_Tab = PU_Info_maptab(pu);
  Current_pu = &PU_Info_pu(pu);
  Current_PU_Info = pu;
  WhirlGlobalStateUtils_hidden::PU_SaveGlobalState(pu);

  return pu;
}


// xlate_PUTree: Translate and return a WHIRL_AST, *assuming* that
// GBL_SYMTAB and the relevant PU_SYMTABs have been translated.
WN*
xlate_WN(sexp_t* ast_sx, int flags)
{
  sexp2whirl::TranslateWN(ast_sx);
}

//***************************************************************************

