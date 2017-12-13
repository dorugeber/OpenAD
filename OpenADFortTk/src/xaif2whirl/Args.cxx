// ##########################################################
// # This file is part of OpenADFortTk.                     #
// # The full COPYRIGHT notice can be found in the top      #
// # level directory of the OpenADFortTk source tree.       #
// # For more information visit                             #
// # http://www.mcs.anl.gov/openad                          #
// ##########################################################

#include "Open64IRInterface/Open64BasicTypes.h"
#include "file_util.h"      // New_Extension, Last_Pathname_Component

#include "Args.h"

using std::cerr;
using std::endl;
using std::string;


TYPE_ID Args::ourDefaultMTypeInt  = MTYPE_I8;
TYPE_ID Args::ourDefaultMTypeUInt = MTYPE_U8;
TYPE_ID Args::ourDefaultMTypeReal = MTYPE_F8;
std::string Args::ourActiveTypeNm("oadactive");

static const char* version_info = "version .289";

static const char* usage_summary =
"[options] <whirl-file> <xaif-file>\n";

static const char* usage_details =
"Given a WHIRL file and a *corresponding* XAIF file, generates new WHIRL.\n"
"By default, the output is sent to the filename formed by replacing the\n"
"extension of <xaif-file> with 'x2w.B'.\n"
"\n"
"Algorithms:\n"
"  --bb-patching     TEMPORARY: use basic-block patch algorithm\n"
"\n"
"Options:\n"
"  -o, --output <file> send output to <file> instead of default file\n"
"      --i4            make integers 4 byte where not specified \n"
"                      (default 8 bytes)\n"
"      --u4            make unsigned integers 4 byte where not specified \n"
"                      (default 8 bytes)\n"
"      --r4            make reals 4 byte where not specified (default 8 bytes)\n"
"  -t, --type <name>   abstract active type name (default oadactive), no longer\n" 
"                      than 26 characters\n"
"  -V, --version       print version information and exit\n"
"  -v, --validate      validate agains schema\n"
"  -h, --help          print this help and exit\n"
"  -n, --noCleanUp     only for development: do not perform whirl cleanup \n"
"                      needed for OpenAD\n"
"      --debug [lvl]   only for development: debug mode at level `lvl'\n";


#define CLP CmdLineParser

CmdLineParser::OptArgDesc Args::optArgs[] = {
  // Options
  {  0 , "bb-patching",     CLP::ARG_NONE, CLP::DUPOPT_CLOB, NULL },
  {  0 , "i4",              CLP::ARG_NONE, CLP::DUPOPT_CLOB, NULL },
  {  0 , "u4",              CLP::ARG_NONE, CLP::DUPOPT_CLOB, NULL },
  {  0 , "r4",              CLP::ARG_NONE, CLP::DUPOPT_CLOB, NULL },
  { 'n', "noCleanUp",CLP::ARG_NONE, CLP::DUPOPT_CLOB, NULL },
  { 'o', "output",   CLP::ARG_REQ , CLP::DUPOPT_ERR,  NULL },
  { 't', "type",     CLP::ARG_REQ , CLP::DUPOPT_ERR,  NULL },
  { 'V', "version",  CLP::ARG_NONE, CLP::DUPOPT_CLOB, NULL },
  { 'v', "validate", CLP::ARG_NONE, CLP::DUPOPT_CLOB, NULL },
  { 'h', "help",     CLP::ARG_NONE, CLP::DUPOPT_CLOB, NULL },
  {  0 , "debug",    CLP::ARG_OPT,  CLP::DUPOPT_CLOB, NULL },
  CmdLineParser::OptArgDesc_NULL
};

#undef CLP

//***************************************************************************
// Args
//***************************************************************************

Args::Args()
{
  Ctor();
}

Args::Args(int argc, const char* const argv[])
{
  Ctor();
  Parse(argc, argv);
}

void
Args::Ctor()
{
  algorithm = xaif2whirl::ALG_NULL;
  validate  = false; 
  debug     = 0; // default: 0 (off)
  myNoCleanUpFlag = false;
}

Args::~Args()
{
}


void 
Args::PrintVersion(std::ostream& os) const
{
  os << GetCmd() << ": " << version_info << endl;
}


void 
Args::PrintUsage(std::ostream& os) const
{
  os << "Usage: " << GetCmd() << " " << usage_summary << endl
     << usage_details << endl;
} 


void 
Args::PrintError(std::ostream& os, const char* msg) const
{
  os << GetCmd() << ": " << msg << endl
     << "Try `" << GetCmd() << " --help' for more information." << endl;
}

void 
Args::PrintError(std::ostream& os, const std::string& msg) const
{
  PrintError(os, msg.c_str());
}


void
Args::Parse(int argc, const char* const argv[])
{
  try {
    // -------------------------------------------------------
    // Parse the command line
    // -------------------------------------------------------
    parser.Parse(optArgs, argc, argv);
    
    // -------------------------------------------------------
    // Sift through results, checking for semantic errors
    // -------------------------------------------------------
    
    // Special options that should be checked first
    if (parser.IsOpt("debug")) { 
      debug = 1; 
      if (parser.IsOptArg("debug")) {
	const string& arg = parser.GetOptArg("debug");
	debug = (int)CmdLineParser::ToLong(arg);
      }
    }
    if (parser.IsOpt("help")) { 
      PrintUsage(std::cerr); 
      exit(0);
    }
    if (parser.IsOpt("validate")) { 
      validate=true;
    }
    if (parser.IsOpt("version")) { 
      PrintVersion(std::cerr);
      exit(0);
    }
    
    if (parser.IsOpt("bb-patching")) { 
      algorithm = xaif2whirl::ALG_BB_PATCHING;
    }

    // Check for other options    
    if (parser.IsOpt("output")) { 
      outWhirlFileNm = parser.GetOptArg("output");
    }

    if (parser.IsOpt("type")) { 
      ourActiveTypeNm = parser.GetOptArg("type");
      if (ourActiveTypeNm.length()>ourActiveTypeNmLength-1) {
	PrintError(std::cerr, "-t argument: " + ourActiveTypeNm + " too long." );
	exit(1);
      }
    }

    if (parser.IsOpt("i4")) { 
      ourDefaultMTypeInt=MTYPE_I4;
    }
    
    if (parser.IsOpt("u4")) { 
      ourDefaultMTypeUInt=MTYPE_U4;
    }
 
    if (parser.IsOpt("r4")) { 
      ourDefaultMTypeReal=MTYPE_F4;
    }
    
    if (parser.IsOpt("noCleanUp")) { 
      myNoCleanUpFlag = true;
    }

    // Check for required arguments
    if (parser.GetNumArgs() != 2) {
      PrintError(std::cerr, "Invalid number of arguments!");
      exit(1);
    }
    inWhirlFileNm = parser.GetArg(0);
    xaifFileNm = parser.GetArg(1);
  } 
  catch (CmdLineParser::ParseError& e) {
    PrintError(std::cerr, e.GetMessage());
    exit(1);
  }

  // -------------------------------------------------------
  // Postprocess
  // -------------------------------------------------------
  if (outWhirlFileNm.empty()) {
    // FIXME: should we place in current directory?
    outWhirlFileNm = New_Extension(xaifFileNm.c_str(), ".x2w.B");
  }
}


void 
Args::Dump(std::ostream& os) const
{
  parser.Dump(os);
}

void 
Args::DDump() const
{
  Dump(std::cerr);
}

