// -*-Mode: C++;-*-
// $Header: /m_home/m_utkej/Argonne/cvs2svn/cvs/OpenADFortTk/src/whirl2xaif/main.cxx,v 1.29 2007-10-08 18:28:34 utke Exp $

#include <iostream>
#include <fstream>
#include <string>

#include <xmlostream.h>

#include "OpenAnalysis/Utils/Exception.hpp"

#include "Open64IRInterface/Open64BasicTypes.h"
// extra from Open64: 
#include "cmplrs/rcodes.h"  // return codes
#include "tracing.h"        // trace routines
#include "ir_reader.h"      // fdump_tree
// from FortTk again: 
#include "Open64IRInterface/WhirlIO.h"
#include "Diagnostics.h"
#include "Args.h"
#include "whirl2xaif.h"

// some forward decls
static int real_main(int argc, char **argv);
static std::ostream& InitOutputStream(Args& args);
static void FiniOutputStream(std::ostream& os, Args& args);

int
main(int argc, char **argv) {
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
  catch (xml::ostream::Exception& e) {
    e.Report(cerr);
    exit(1);
  }
  catch (OA::Exception& e) {
    e.report(cerr);
    exit(1);
  }
  catch (...) {
    std::cerr << "Unknown exception caught\n";
    exit(1);
  }
}

static int real_main(int argc, char **argv) {
  // -------------------------------------------------------
  // 1. Open64 Initialization
  // -------------------------------------------------------
  Handle_Signals();
  MEM_Initialize();
  Init_Error_Handler( 100 );
  Set_Error_Line( ERROR_LINE_UNKNOWN );
  Set_Error_File( NULL );
  Set_Error_Phase("whirl2xaif");
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
  Diag_Set_Phase("WHIRL to XAIF: driver");
  
  Args args(argc, argv);
  std::ostream& os(InitOutputStream(args));
  fortTkSupport::Diagnostics::setDiagnosticFilterLevel(args.debug);
  
  // -------------------------------------------------------
  // 3. Read WHIRL IR
  // -------------------------------------------------------
  PU_Info* pu_forest = ReadIR(args.whirlFileNm.c_str());
  PrepareIR(pu_forest); // FIXME (should this be part of translation?)
  
  // -------------------------------------------------------
  // 4. Translate WHIRL into XAIF
  // -------------------------------------------------------  
  
  os << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\n";
  whirl2xaif::Whirl2Xaif::translateIR(os, pu_forest, args.tmpVarPrefix.c_str());
  
  bool writeIR = false;
  if (writeIR) { 
    std::string file = "out.B"; // FIXME
    WriteIR(file.c_str(), pu_forest); 
  } 
  else {
    FreeIR(pu_forest); // Writing frees some of the WHIRL maps
  }
  
  // If we've seen errors, note them and terminate
  INT local_ecount, local_wcount;
  if ( Get_Error_Count ( &local_ecount, &local_wcount ) ) {
    Terminate(Had_Internal_Error() ? RC_INTERNAL_ERROR : 
              RC_NORECOVER_USER_ERROR);
  }

  Diag_Exit();
  Cleanup_Files(TRUE, FALSE); // Open64

  // -------------------------------------------------------
  // 5. Finalization
  // -------------------------------------------------------
  FiniOutputStream(os,args);
  return RC_OKAY;
}

static std::ostream& InitOutputStream(Args& args) {
  if (args.xaifFileNm.empty()) {
    return std::cout;
  } 
  else {
    std::ofstream* ofs = new std::ofstream;
    std::string filename(args.xaifFileNm+".tmp");
    int keepErrno;
    errno=0;
    ofs->open(filename.c_str(), ios::out | ios::trunc);
    if (keepErrno=errno || !ofs->is_open() || ofs->fail()) 
      FORTTK_DIE("cannot open temporary file " << filename.c_str() << " because: " << strerror(keepErrno));
    return *ofs;
  }
}

static void
FiniOutputStream(std::ostream& os,Args& args) {
  if (!args.xaifFileNm.empty()) {
    delete &os;
    std::string tmpfilename(args.xaifFileNm+".tmp");
    int keepErrno;
    errno=0;
    rename(tmpfilename.c_str(), args.xaifFileNm.c_str());
    if (keepErrno=errno)
      FORTTK_DIE("cannot rename temporary file " << tmpfilename.c_str() << " to output file " <<  args.xaifFileNm.c_str() << " because: " << strerror(keepErrno));
  }
}

