// ##########################################################
// # This file is part of OpenADFortTk.                     #
// # The full COPYRIGHT notice can be found in the top      #
// # level directory of the OpenADFortTk source tree.       #
// # For more information visit                             #
// # http://www.mcs.anl.gov/openad                          #
// ##########################################################

#include "Open64BasicTypes.h"

#include "ir_bread.h"	   // Read_Global_Info(), etc.
#include "ir_bwrite.h"	   // Write_Global_Info(), etc.
#include "ir_reader.h"     // fdump_tree
#include "tracing.h"       // trace routines

#include "be_util.h"       // Advance_/Reset_Current_PU_Count(), etc

#include "stblock.h"       // Create_Slink_Symbol()

#include "WhirlIO.h"
#include "WhirlGlobalStateUtils.h"
#include "Open64IRInterface.hpp"

#include "IFDiagnostics.h"

#include "CleanUpWhirl.h"

static void 
ReadPU(PU_Info* pu);


// ReadIR:
PU_Info*
ReadIR(const char* irfilename, bool noCleanUpWhirl)
{
  Diag_Set_Phase("WHIRL IO: Load IR");

  MEM_POOL_Push(&MEM_src_pool);
  MEM_POOL_Push(&MEM_src_nz_pool);
  Set_Error_Source(Src_File_Name);

  MEM_POOL_Push(MEM_pu_nz_pool_ptr);
  MEM_POOL_Push(MEM_pu_pool_ptr);

  // -------------------------------------------------------
  // 1. Read pu tree info and global symbol table
  // -------------------------------------------------------
  
  // Open file, read PU info and setup symbol tables
  Open_Input_Info(irfilename);
  Read_Global_Data = (char*)"bogus-value-as-argument-to-Initialize_Symbol_Tables";
  Initialize_Symbol_Tables(FALSE /*reserve_index_zero*/);
  New_Scope(GLOBAL_SYMTAB, Malloc_Mem_Pool, FALSE);
  PU_Info *pu_forest = Read_Global_Info(NULL);
  Initialize_Special_Global_Symbols();
  
  // -------------------------------------------------------
  // 2. Read PUs and local symbol tables
  // -------------------------------------------------------
  // Perform our own iteration to ensure correctness and safety
  for (PU_Info* pu = pu_forest; pu != NULL; pu = PU_Info_next(pu)) {
    ReadPU(pu);
  }
  
  if (!noCleanUpWhirl)
    CleanUpWhirl::forPUInfoForest(pu_forest);

  // Free_Input_Info // should we do this?

  return pu_forest;
}


void
ReadPU(PU_Info* pu)
{
  Current_PU_Info = pu;
  
  // Read program unit (Reads the PUs (WHIRL trees), symbol tables;
  // sets CURRENT_SYMTAB and Scope_tab[]).
  Read_Local_Info(MEM_pu_nz_pool_ptr, pu);
  WN *wn_pu = PU_Info_tree_ptr(pu); // made possible by Read_Local_Info()
  
  Advance_Current_PU_Count();
  
  // Now recursively process the child PU's.
  for (PU_Info* child = PU_Info_child(pu); child != NULL;
       child = PU_Info_next(child)) {
    ReadPU(child);
  }

  WhirlGlobalStateUtils_hidden::PU_SaveGlobalState(pu);
}


//***************************************************************************
// WriteIR
//***************************************************************************

static void
WritePU(PU_Info* pu);

static void
SetPUInfoStateIR(PU_Info* pu_forest, Subsect_State state);

static void
SetPUInfoStatePU(PU_Info* pu, Subsect_State state);

static void
SetPUInfoState(PU_Info* pu, Subsect_State state);


// WriteIR
void 
WriteIR(const char* irfilename, PU_Info* pu_forest)
{
  Diag_Set_Phase("WHIRL IO: Write IR");

  Open_Output_Info(irfilename);

  // -------------------------------------------------------
  // 1. Write PUs and local symbol tables
  // -------------------------------------------------------
  // Perform our own iteration to ensure correctness and safety
  for (PU_Info* pu = pu_forest; pu != NULL; pu = PU_Info_next(pu)) {
    WritePU(pu);
  }

  // -------------------------------------------------------
  // 2. Write Global info
  // -------------------------------------------------------
  Write_Global_Info(pu_forest); // expects PU state to be Subsect_Written
  Close_Output_Info();
}


static void
WritePU(PU_Info* pu)
{
  PU_SetGlobalState(pu);
  
  Write_PU_Info(pu); // sets PU state to Subsect_Written
  
  // Recur
  for (PU_Info* child = PU_Info_child(pu); child != NULL;
       child = PU_Info_next(child)) {
    WritePU(child);
  }
}


// SetPUInfoStateIR: For each PU in IR 'pu_forest', sets each
// subsection to have the state 'state' if the subsection is present
// (i.e. not Subsect_Missing).
static void
SetPUInfoStateIR(PU_Info* pu_forest, Subsect_State state) 
{
  for (PU_Info* pu = pu_forest; pu != NULL; pu = PU_Info_next(pu)) {
    SetPUInfoStatePU(pu, state);
  }
}


// SetPUInfoStatePU: Sets the state for the PU tree: the current PU
// and all children.
static void
SetPUInfoStatePU(PU_Info* pu, Subsect_State state) 
{
  // Set the state
  SetPUInfoState(pu, state);
  
  // Recur
  for (PU_Info* child = PU_Info_child(pu); child != NULL;
       child = PU_Info_next(child)) {
    SetPUInfoStatePU(child, state);
  }
}


// SetPUInfoState: Set the state for the current PU
static void
SetPUInfoState(PU_Info* pu, Subsect_State state) 
{
  for (int subsec = 0; subsec < WT_SUBSECTIONS; ++subsec) {
    if (PU_Info_state(pu, subsec) != Subsect_Missing) {
      Set_PU_Info_state(pu, subsec, state);
    }
  }
}


//***************************************************************************
// FreeIR
//***************************************************************************

static void 
FreePU(PU_Info* pu);


// FreeIR: 
void
FreeIR(PU_Info* pu_forest)
{
  Diag_Set_Phase("WHIRL IO: Free IR");
  
  // -------------------------------------------------------
  // 1. Free PUs and local symbol tables
  // -------------------------------------------------------
  // Perform our own iteration to ensure correctness and safety
  for (PU_Info* pu = pu_forest; pu != NULL; pu = PU_Info_next(pu)) {
    FreePU(pu);
  }
  
  // -------------------------------------------------------
  // 2. Free pu tree info and global symbol tables
  // -------------------------------------------------------
  Verify_SYMTAB(GLOBAL_SYMTAB);
  
  MEM_POOL_Pop(MEM_pu_nz_pool_ptr);
  MEM_POOL_Pop(MEM_pu_pool_ptr);
  
  MEM_POOL_Pop(&MEM_src_pool);
  MEM_POOL_Pop(&MEM_src_nz_pool);
  
#ifdef Is_True_On
  if (Get_Trace (TKIND_ALLOC, TP_MISC)) {
    fprintf (TFile, "\n%s\tMemory allocation info\n", DBar);
    MEM_Trace();
  }
#endif
}


static void
FreePU(PU_Info* pu)
{
  PU_SetGlobalState(pu);
  
  Free_Local_Info(pu); // deletes all maps
  
  // Now recursively process the child PU's.
  for (PU_Info *child = PU_Info_child(pu); child != NULL;
       child = PU_Info_next(child)) {
    FreePU(child);
  }
}


//***************************************************************************
// PrepareIR
//***************************************************************************

void 
PrepareIR(PU_Info* pu_forest)
{
  Diag_Set_Phase("WHIRL IO: Prepare IR");

  Open64IRProcIterator procIt(pu_forest);
  for ( ; procIt.isValid(); ++procIt) { 
    PU_Info* pu = (PU_Info*)procIt.current().hval();
    WN* wn_pu = PU_Info_tree_ptr(pu);
    Create_Slink_Symbol(); // FIXME: do we need?
  }
}


//***************************************************************************
// DumpIR
//***************************************************************************

void 
DumpIR(PU_Info* pu_forest)
{
  bool dumpST = false;
  IR_set_dump_order(TRUE); // Preorder dump

  if (dumpST) {
    // Global 
    Print_global_symtab(stdout);
  }

  Open64IRProcIterator procIt(pu_forest);
  for ( ; procIt.isValid(); ++procIt) { 
    PU_Info* pu = (PU_Info*)procIt.current().hval();  
    WN* wn_pu = PU_Info_tree_ptr(pu);
    
    //IR_put_func(wn_pu, NULL);
    fdump_tree(stderr, wn_pu);
    
    if (dumpST) {
      Print_local_symtab(stdout, Scope_tab[CURRENT_SYMTAB]);
    }
  }
}


