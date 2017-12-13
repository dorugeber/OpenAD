// -*-Mode: C++;-*-
// $Header: /Volumes/cvsrep/developer/OpenADFortTk/src/whirl2sexp/main.cxx,v 1.2 2004/08/23 18:44:42 eraxxon Exp $

#include <iostream>
#include <string>

#include "Open64IRInterface/Open64BasicTypes.h"
#include "cmplrs/rcodes.h"  // return codes
#include "tracing.h"        // trace routines
#include "ir_reader.h"      // fdump_tree
#include "Open64IRInterface/WhirlIO.h"
#include "Open64IRInterface/IFDiagnostics.h"

#include "Diagnostics.h"
#include "sexpostream.h"

#include "Args.h"
#include "whirl2sexp.h"
#include "Exception.h"

//************************** Forward Declarations ***************************

static int
real_main(int argc, char **argv);

static std::ostream*
InitOutputStream(Args& args);

static void
FiniOutputStream(std::ostream* os);

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
  catch (sexp::ostream::Exception& e) {
    e.Report(cerr);
    exit(1);
  }
  catch (...) {
    cerr << "Unknown exception caught\n";
    exit(1);
  }
  // FIXME: catch badalloc?
}

static int
real_main(int argc, char **argv)
{
  // -------------------------------------------------------
  // 1. Open64 Initialization
  // -------------------------------------------------------
  Handle_Signals();
  MEM_Initialize();
  Init_Error_Handler( 100 );
  Set_Error_Line( ERROR_LINE_UNKNOWN );
  Set_Error_File( NULL );
  Set_Error_Phase("whirl2sexp");
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
  Diag_Set_Phase("WHIRL to sexp: driver");
  
  Args args(argc, argv);
  std::ostream* os = InitOutputStream(args);
  fortTkSupport::Diagnostics::setDiagnosticFilterLevel(args.debug);
  
  // -------------------------------------------------------
  // 3. Read WHIRL and Translate into S-expressions
  // -------------------------------------------------------
  PU_Info* pu_forest = ReadIR(args.whirlFileNm.c_str(),args.myNoCleanUpFlag);
  whirl2sexp::TranslateIR(*os, pu_forest);
  FreeIR(pu_forest); // Writing frees some of the WHIRL maps
  
  // -------------------------------------------------------
  // 4. Finalization
  // -------------------------------------------------------
  FiniOutputStream(os);
  
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

static std::ostream*
InitOutputStream(Args& args)
{
  if (args.sexpFileNm.empty()) {
    // Use cout
    return &(std::cout);
  } else {
    ofstream* ofs = new ofstream;
    OpenFile(*ofs, args.sexpFileNm.c_str());
    return ofs;
  }
}

static void
FiniOutputStream(std::ostream* os)
{
  if (os != &std::cout) {
    delete os;
  }
}


static void 
OpenFile(std::ofstream& fs, const char* filename)
{
  using namespace std;

  fs.open(filename, ios::out | ios::trunc);
  if (!fs.is_open() || fs.fail()) {
    ErrMsg(EC_IR_Open, filename, 0/*FIXME*/);
  } 
}

static void
CloseFile(std::ofstream& fs)
{
  fs.close();
}
