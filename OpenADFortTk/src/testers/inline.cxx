
//***************************************************************************
// 
//***************************************************************************

#include "clone.h"          /* Open64: IPO_CLONE */
#include <lib/support/WhirlParentize.h>
#include <vector>
using std::vector;

static WN* 
FindCallToInlinedFn(const char* callee_nm, WN* wn);

void 
InlineTest(PU_Info* pu_forest)
{
  // Given a PU forest 'pu_forest'
  //   * searches for the first pu named 'inline_*'.  This is
  //     assumed to be the subroutine for inlining.
  //   * searches the program pu for the first call of this subroutine.
  //   * inlines the subroutine into the main pu
  // 
  // Limitations: can only inline subroutines that
  //   - have one return statement (or none)
  //   - are called with variables as actual parameters (as opposed to
  //     an expression like sin(x)).
  //
  // Note: if we create entirely new functions, we will need to update
  //   pu->sym_tab map.
  
  // -------------------------------------------------------
  // 1. Find inlined functions (callee) named inline_x
  // -------------------------------------------------------
  const char* calleeNm = NULL;
  PU_Info*    calleePU = NULL;
  WN*         calleeWN = NULL; // OPR_FUNC_ENTRY
  WN_MAP_TAB* calleeMaptab = NULL;
  SCOPE*      calleeStab = NULL;
  SYMTAB_IDX  calleeStabIdx = 0;
  
  Pro64IRProcIterator procIt(pu_forest);
  for ( ; procIt.IsValid(); ++procIt) { 
    PU_Info* pu = (PU_Info*)procIt.Current();
    ST_IDX st = PU_Info_proc_sym(pu);
    const char* nm = ST_name(st);

    if (strncmp(nm, "inline_", 7) == 0) {
      calleeNm = nm;
      calleePU = pu;
      calleeWN = PU_Info_tree_ptr(calleePU);
      calleeMaptab = PU_Info_maptab(pu);
      calleeStab = Scope_tab;
      calleeStabIdx = PU_lexical_level(PU_Info_pu(pu));
      break;
    }
  } // global state set for calleePU
  
  // -------------------------------------------------------
  // 2. Find the caller, i.e. callsites to inline_x. In the MainPU
  // -------------------------------------------------------
  PU_Info*    callerPU = NULL;
  WN*         callsiteWN = NULL;
  WN_MAP_TAB* callerMaptab = NULL;
  SCOPE*      callerStab = NULL;
  SYMTAB_IDX  callerStabIdx = 0;
  
  procIt.Reset();
  for ( ; procIt.IsValid(); ++procIt) { 
    PU_Info* pu = (PU_Info*)procIt.Current();
    if (PU_is_mainpu(PU_Info_pu(pu))) {
      callerPU = pu;
      callsiteWN = FindCallToInlinedFn(calleeNm, PU_Info_tree_ptr(callerPU));
      callerMaptab = PU_Info_maptab(pu);
      callerStab = Scope_tab;
      callerStabIdx = PU_lexical_level(PU_Info_pu(pu));
      break;
    }
  } // global state set for callerPU
  
  if (!callsiteWN) {
    return;
  }

  // * Gather actual parameters (at callsite)
  INT nparam = WN_kid_count(callsiteWN);
  vector<ST*> callsiteParams(nparam);
  for (int i = 0; i < nparam; ++i) {
    WN* parm = WN_kid(callsiteWN, i);
    ST* st = WN_st(WN_kid0(parm));
    callsiteParams[i] = st;
  }
  
  // For the below, compare to
  // IPO_INLINE::Process(), ipa/main/optimize/ipo_inline.cxx.
  
  // -------------------------------------------------------
  // 1. Create a copy of the callee
  // -------------------------------------------------------
  // IPO_INLINE::Process_Callee(...)
  //   IPO_INLINE::Clone_Callee(...)
  
  // Global tables should point to callee
  PU_SetGlobalState(calleePU);
  
  // FIXME: allocates WN_mem_pool_ptr, which is used in IPO_CLONE::Copy_Node
  WN* bogus_wn = WN_CreateIntconst(OPC_I4INTCONST, 0);
  
  WN_MAP parentmap = WN_MAP_Create(MEM_pu_pool_ptr);
  IPO_SYMTAB ipo_symtab(calleeStab, callerStab, calleeStabIdx, callerStabIdx,
			MEM_pu_pool_ptr, TRUE /*same_file*/);
  IPO_CLONE cloner(callerMaptab, calleeMaptab, parentmap,
		   calleeStab, callerStab, calleeStabIdx, callerStabIdx,
		   &ipo_symtab, MEM_pu_pool_ptr, TRUE /*same_file*/, 0);  
  
  // * Copy symtabs of callee into caller's
  //cloner.Promote_Statics();
  cloner.Get_sym()->Update_Symtab(FALSE /*label_only*/);
    
  // * Clone, setting symtab indices to reference caller's tables
  WN* inlinedBodyWn = cloner.Clone_Tree(WN_func_body(calleeWN));
  
  // * Gather formals parameters (at callee)
  nparam = WN_num_formals(calleeWN);
  vector<ST*> calleeParams(nparam);
  for (int i = 0; i < nparam; ++i) {
    ST* st = WN_st(WN_formal(calleeWN, i));
    st = ipo_symtab.Get_ST(st); // get the version on the caller side
    calleeParams[i] = st;
  }
  assert(callsiteParams.size() == calleeParams.size());
  
  // -------------------------------------------------------
  // 2. Update and patch body of inlined function
  // -------------------------------------------------------
  // IPO_INLINE::Process_Callee(...)
  //   IPO_INLINE::Clone_Callee(...) and
  //   IPO_INLINE::Walk_and_Update_Callee(...)

  // Global tables should point to caller (cloned tree uses caller's symtab)
  PU_SetGlobalState(callerPU);
  
  // * Recreate parent pointers
  // W2CF_Parentize(inlinedBodyWn); // WN_Parentize() - ipo_parent.cxx
  
  // Prepare map: formal params -> actual params
  map<ST*, ST*> formals2actuals;
  for (int i = 0; i < nparam; ++i) {
    ST* formalST = calleeParams[i];
    ST* actualST = callsiteParams[i];
    formals2actuals[formalST] = actualST;
  }
  
  // * Replace any formal parameters with actual parameters
  //   Cf. IPO_INLINE::Process_Formals(...) (actually only preparation
  //   for Process_Op_Code(...)
  // * Remove RETURN in body of inlined function
  //   Cf. IPO_INLINE::Process_Op_Code(...)
  WN_TREE_CONTAINER<PRE_ORDER> wnIt(inlinedBodyWn);
  WN_TREE_CONTAINER<PRE_ORDER>::iterator it;
  for (it = wnIt.begin(); it != wnIt.end(); ++it) {
    WN* curWN = it.Wn();
    OPERATOR opr = WN_operator(curWN);
    
    // Replace reference to a formal param with actual param
    if (OPERATOR_has_sym(opr)) {
      ST* curST = WN_st(curWN);
      ST* actualST = formals2actuals[curST];
      if (actualST) {
	WN_st_idx(curWN) = ST_st_idx(actualST);
      }
    }

    // Remove returns
    if (opr == OPR_RETURN) {
      WN* blkWN = FindParentWNBlock(inlinedBodyWn, curWN);
      WN_DELETE_FromBlock(blkWN, curWN);
      break; // should only be one, at end of block (FIXME)
    }
  }
  
  // -------------------------------------------------------
  // 3. Replace callsite with body of inlined function
  // -------------------------------------------------------
  // IPO_INLINE::Post_Process_Caller(...)
  if (WN_first (inlinedBodyWn) != NULL) {
    // Replace callsite with body of inlined function
    WN_next(WN_prev(callsiteWN)) = WN_first(inlinedBodyWn);
    WN_prev(WN_first(inlinedBodyWn)) = WN_prev(callsiteWN);
    
    WN_next(WN_last(inlinedBodyWn)) = WN_next(callsiteWN);
    WN_prev(WN_next(callsiteWN)) = WN_last(inlinedBodyWn);
  } else {
    // Replace callsite with (empty) body of inlined function
    WN_next(WN_prev(callsiteWN)) = WN_next(callsiteWN);
    WN_prev(WN_next(callsiteWN)) = WN_prev(callsiteWN);
  }

#if 0  
  WN* callerWN = PU_Info_tree_ptr(callerPU);
  dump_tree(callerWN);
#endif  
}

static WN* 
FindCallToInlinedFn(const char* calleeNm, WN* wn)
{
  WN_TREE_CONTAINER<PRE_ORDER> wnIt(wn);
  WN_TREE_CONTAINER<PRE_ORDER>::iterator it;
  for (it = wnIt.begin(); it != wnIt.end(); ++it) {
    WN* curWN = it.Wn();
    
    OPERATOR opr = WN_operator(curWN);
    if (opr == OPR_CALL && (strcmp(ST_name(WN_st(curWN)), calleeNm) == 0)) {
      return curWN;
    }
  }
  
  return NULL;
}

