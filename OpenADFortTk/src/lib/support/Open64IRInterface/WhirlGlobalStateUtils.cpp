// -*-Mode: C++;-*-
//************************* System Include Files ****************************

#include <cassert>

//*************************** User Include Files ****************************

#include "WhirlGlobalStateUtils.h"

//************************** Forward Declarations ***************************

//***************************************************************************

PUToScopedSymTabMap PUToScopeTabMap;

//***************************************************************************

// PU_SetGlobalState: 
void
PU_SetGlobalState(PU_Info* pu)
{
  // 'pu' must be in memory (change some global pointers around)
  assert(PU_Info_state(pu, WT_TREE) == Subsect_InMem); // Subsect_Written?
  Current_Map_Tab = PU_Info_maptab(pu);
  Current_pu = &PU_Info_pu(pu);
  Current_PU_Info = pu;
  CURRENT_SYMTAB = PU_lexical_level(*Current_pu); // == Current_scope
  Scope_tab = PUToScopeTabMap.Find(pu);
  assert(Scope_tab);
}


// PU_AllocBEGlobalSymtab:
void
PU_AllocBEGlobalSymtab()
{
  // Initialize back end symbol table (e.g. w2cf relies on this)
  BE_symtab_initialize_be_scopes();
  BE_symtab_alloc_scope_level(GLOBAL_SYMTAB);
  for (SYMTAB_IDX lvl = 0; lvl <= GLOBAL_SYMTAB; ++lvl) {
    if (Scope_tab[lvl].st_tab) {
      Scope_tab[lvl].st_tab->Register(*Be_scope_tab[lvl].be_st_tab);
    } else { // only level 0 should not have st_tab
      Is_True(lvl == 0, ("Nonexistent st_tab for level %d", lvl));
    }
  }
}


// PU_FreeBEGlobalSymtab: 
void
PU_FreeBEGlobalSymtab()
{
  // Free back end symtabs (from 0 <= lvl <= GLOBAL_SYMTAB)
  for (SYMTAB_IDX lvl = GLOBAL_SYMTAB; lvl <= GLOBAL_SYMTAB; --lvl) {
    if (Scope_tab[lvl].st_tab) {
      Scope_tab[lvl].st_tab->Un_register(*Be_scope_tab[lvl].be_st_tab);
      Be_scope_tab[lvl].be_st_tab->Clear();
    } else { // only level 0 should not have st_tab
      Is_True(lvl == 0, ("Nonexistent st_tab for level %d", lvl));
    }
  }
  BE_symtab_free_be_scopes();
}


// PU_AllocBELocalSymtab:
void
PU_AllocBELocalSymtab_helper(PU_Info* pu);

void
PU_AllocBELocalSymtab(PU_Info* pu)
{
  PU_SetGlobalState(pu); // just in case
  
  PU_AllocBELocalSymtab_helper(pu);
  for (PU_Info* child = PU_Info_child(pu); child != NULL;
       child = PU_Info_next(child)) {
    PU_AllocBELocalSymtab_helper(child);
  }
}

void
PU_AllocBELocalSymtab_helper(PU_Info* pu)
{
  // Initialize local back end symtab.  Assume symtab is already
  // prepared for pu.
  SYMTAB_IDX lvl = PU_lexical_level(PU_Info_pu(pu));
  BE_symtab_alloc_scope_level(lvl);
  Scope_tab[lvl].st_tab->Register(*Be_scope_tab[lvl].be_st_tab);
}


// PU_FreeBELocalSymtab:
void
PU_FreeBELocalSymtab_helper(PU_Info* pu);

void
PU_FreeBELocalSymtab(PU_Info* pu)
{
  PU_SetGlobalState(pu); // just in case

  PU_FreeBELocalSymtab_helper(pu);
  for (PU_Info* child = PU_Info_child(pu); child != NULL;
       child = PU_Info_next(child)) {
    PU_FreeBELocalSymtab_helper(child);
  }
}

void
PU_FreeBELocalSymtab_helper(PU_Info* pu)
{
  // Free local back end symtab.  Assume symtab is already prepared
  // for pu.
  SYMTAB_IDX lvl = PU_lexical_level(PU_Info_pu(pu));
  Scope_tab[lvl].st_tab->Un_register(*Be_scope_tab[lvl].be_st_tab);
  Be_scope_tab[lvl].be_st_tab->Clear();
}


//***************************************************************************

// PU_SaveGlobalState: <see header comments>
//
// The only state we need to save is the lexical scope table (SCOPE*).
// We do this by creating a copy of the lexical table for each PU.
// Each lexical table contains several pointers to the various tables
// the comprise the symbols tables.  Because the pointers are simply
// copied, THE ACTUAL ST TABLES MAY BE SHARED; THEY ARE NOT
// DUPLICATED.  The global symbol table will be shared accros all PUs;
// nested PUs will share symbol tables from the outer scopes.
//
// N.B. This function will likely create a small memory leak.  Because
// each PU will be given its own lexical table, there will be no owner
// for the lexical table created in Initialize_Symbol_Tables().
//
// See the following:
//   ipa/main/analyze/ipa_cg.cxx :
//     read_pu_including_parents(..)
//     IPA_NODE::Scope(..)
//     IPA_NODE::Set_Scope(..)
//   be/com/clone.cxx
//     IPO_SYMTAB::New_Symtab(..)
void
WhirlGlobalStateUtils_hidden::PU_SaveGlobalState(PU_Info* pu)
{
  // We assume that the global state has already been properly prepared.
  
  // Create a scope table large enough for this lexical level and
  // insert it into the map. Note: we intentionally do not set
  // 'Scope_tab' to this copy in order to stave off the likely memory
  // leak as long as possible.
  SYMTAB_IDX lvl = PU_lexical_level(*Current_pu);
  INT sz = (lvl + 1) * sizeof(SCOPE);
  SCOPE* new_scope_tab = (SCOPE*)MEM_POOL_Alloc(Malloc_Mem_Pool, sz);
  memcpy(new_scope_tab, Scope_tab, sz);
  
  PUToScopeTabMap.Insert(pu, new_scope_tab);
}

//***************************************************************************

PUToScopedSymTabMap::PUToScopedSymTabMap() 
{ 
}

PUToScopedSymTabMap::~PUToScopedSymTabMap() 
{
  using namespace WhirlGlobalStateUtils_hidden;
  
  for (PUToScopedSymTabBaseMap::iterator it = this->begin();
       it != this->end(); ++it) {
    SCOPE* scopetab = (*it).second;
    MEM_POOL_FREE(Malloc_Mem_Pool, scopetab);
  }
}

//***************************************************************************

#if 0

// The following were the former versions of these functions.  The
// user called Restore before using a pu and Save after using it.  If
// this was not strictly followed very bad things could happen because
// only the local symbol table was saved, using a gross hack.  I
// couldn't get it to handle nested PUs.  

// When using multiple PUs at a time, this should be called to
// prepare the global symbol table pointers the current PU.
void 
RestoreOpen64PUGlobalVars(PU_Info* pu);

// When using multiple PUs at a time, this should be called to save
// the global symbol table pointer for later use.
void
SaveOpen64PUGlobalVars(PU_Info* pu);



// cf. wn_mp.cxx 1278 ; dra_clone.cxx:974
// PU_Info_symtab_ptr(pu)

// (PU_lexical_level (&St_Table[PU_Info_proc_sym (pu)]))
// Scope_tab[CURRENT_SYMTAB].st = WN_get_proc_sym(pu);

// Callgraph: ipa_cg.cxx (1053)

void 
RestoreOpen64PUGlobalVars(PU_Info* pu)
{
  ST_IDX st = PU_Info_proc_sym(pu);
  DBGMSG(2, "** Restoring Open64 global vars for '%s' **", ST_name(st));
  
  // The PU is in memory (change some global pointers around)
  assert(PU_Info_state(pu, WT_TREE) == Subsect_InMem);
  Current_Map_Tab = PU_Info_maptab(pu);
  Current_pu = &PU_Info_pu(pu);
  CURRENT_SYMTAB = PU_lexical_level(*Current_pu);
  
  Restore_Local_Symtab(pu);
  // Can we make this restore itself and all its parents?
}

void
SaveOpen64PUGlobalVars(PU_Info* pu)
{
  ST_IDX st = PU_Info_proc_sym(pu);
  DBGMSG(2, "** Saving Open64 global vars for '%s' **", ST_name(st));
  
  Set_PU_Info_symtab_ptr(pu, NULL);
  Save_Local_Symtab(CURRENT_SYMTAB, pu);
  // Can we make this save itself and all its parents?
}

#endif

//***************************************************************************
