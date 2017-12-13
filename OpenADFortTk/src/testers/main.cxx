// -*-Mode: C++;-*-
// $Header: /Volumes/cvsrep/developer/OpenADFortTk/src/testers/main.cxx,v 1.10 2005/03/19 22:54:51 eraxxon Exp $

#include <iostream>
#include <fstream>
#include <string>

#include <OpenAnalysis/Utils/Exception.hpp>

#include "Open64IRInterface/Open64BasicTypes.h"
#include "cmplrs/rcodes.h"  // return codes
#include "tracing.h"        // trace routines
#include "ir_reader.h"      // fdump_tree
#include "Open64IRInterface/WhirlIO.h"
#include "Open64IRInterface/IFDiagnostics.h"

#include "Exception.h"

#include "Args.h"
#include "tester.h"

static int 
real_main(int argc, char **argv);

static void 
OpenFile(std::ofstream& fs, const char* filename);

static void 
CloseFile(std::ofstream& fs);

//***************************************************************************

int
main(int argc, char **argv)
{
  try {
    return real_main(argc, argv);
  }
  catch (CmdLineParser::Exception& e) {
    e.Report(cerr); // fatal error
    exit(1);
  }
  catch (fortTkSupport::BaseException& e) {
    e.Report(cerr);
    exit(1);
  }
  catch (OA::Exception& e) {
    e.report(cerr);
    exit(1);
  }
  catch (...) {
    cerr << "Unknown exception caught\n";
    exit(1);
  }
}

static int
real_main(int argc, char* argv[])
{
  // -------------------------------------------------------
  // 1. Open64 Initialization
  // -------------------------------------------------------
  Handle_Signals();
  MEM_Initialize();
  Init_Error_Handler( 100 );
  Set_Error_Line( ERROR_LINE_UNKNOWN );
  Set_Error_File( NULL );
  Set_Error_Phase("whirltester");
  IR_set_dump_order(TRUE /*pre*/); // pre-order trees when debugging, please!
  
#ifdef Is_True_On
  if (Get_Trace(TKIND_ALLOC, TP_MISC)) {
    MEM_Tracing_Enable();
  }
#endif
  
  Preconfigure();         // from config.cxx...
  Configure();            // needed for WN_lower!
  Configure_Source(NULL); // Most config variables set here

  Init_Operator_To_Opcode_Table(); // FIXME
    
  // -------------------------------------------------------
  // 2. Local initialization (options, etc.)
  // -------------------------------------------------------
  Diag_Init();
  Diag_Set_Max_Diags(100); // Maximum 100 warnings by default
  Diag_Set_Phase("WHIRL Harness");

  Args args(argc, argv);
  
  // -------------------------------------------------------
  // 3. Read WHIRL IR
  // -------------------------------------------------------
  PU_Info* pu_forest = ReadIR(args.whirlFileNm.c_str());
  PrepareIR(pu_forest); // used in whirl2xaif, xaif2whirl

  // -------------------------------------------------------
  // 4. Do something
  // -------------------------------------------------------  
  
  if (args.dumpIR) { 
    DumpIR(pu_forest); 
  }
  
  if (args.runMode == 1) {
    whirltester::TestIR(std::cout, pu_forest);
  } else if (args.runMode == 2) {
    whirltester::TestIR_OA(std::cout, pu_forest);
  } else if (args.runMode == 3) {
    whirltester::TestIR_whirl2f(std::cout, pu_forest);
  }
  
  FreeIR(pu_forest); // N.B. cannot use with WriteIR

  // -------------------------------------------------------
  // 5. Finalization
  // -------------------------------------------------------

  // If we've seen errors, note them and terminate
  INT local_ecount, local_wcount;
  if ( Get_Error_Count ( &local_ecount, &local_wcount ) ) {
    Terminate(Had_Internal_Error() ? RC_INTERNAL_ERROR : 
              RC_NORECOVER_USER_ERROR);
  }

  Diag_Exit();
  Cleanup_Files(TRUE, FALSE); // Open64

  return RC_OKAY;
}


//***************************************************************************
// 
//***************************************************************************

#include "file_util.h" // New_Extension, Last_Pathname_Component

#if 0
  // progname = Last_Pathname_Component(argv[0]);
  // new_name = New_Extension(Src_File_Name, IRB_FILE_EXTENSION);
  
  // We want the output files to be created in the current directory,
  // so strip off any directory path, and substitute the suffix 
  // appropriately.
  // new_file = New_Extension(Last_Pathname_Component(whirlfn), ".xaif");
  
  // Src_File_Name, Irb_File_Name, Obj_File_Name are from Open64 "glob.h"
  // Src_File_Name = Irb_File_Name = (char*)WHIRL_filename.c_str();
#endif

//***************************************************************************

static void 
OpenFile(std::ofstream& fs, const char* filename)
{
  using namespace std;

  fs.open(filename, ios::out | ios::trunc);
  if (!fs.is_open() || fs.fail()) {
    ErrMsg(EC_IR_Open, filename, 0/*FIXME*/);
  } 
  
#if 0//FIXME
  fs.open(filename, ios::out | ios::app);
  fs = Open_Append_File(W2F_File_Name[kind]);
#endif

}

static void
CloseFile(std::ofstream& fs)
{
  fs.close();
}
