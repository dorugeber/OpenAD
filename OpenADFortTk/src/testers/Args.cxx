#include <cstdlib>

#include "Args.h"

//*************************** Forward Declarations **************************

using std::cerr;
using std::endl;
using std::string;


//***************************************************************************

static const char* version_info = "version .289";

static const char* usage_summary =
"[mode] [options] <whirl-file>\n";

static const char* usage_details =
"Given a WHIRL file and a mode, do something.\n"
"\n"
"Modes:\n"
"      --ir            run a test routine on IR\n"
"      --oa-ujnum      test OA UJ numbering\n"
"      --whirl2f       test whirl2f\n"
"\n"
"Options:\n"
"  -d, --dump          dump the WHIRL IR\n"
"  -V, --version       print version information\n"
"  -h, --help          print this help\n"
"      --debug [lvl]   debug mode at level `lvl'\n";


#define CLP CmdLineParser

CmdLineParser::OptArgDesc Args::optArgs[] = {
  // Modes
  {  0 , "ir",         CLP::ARG_NONE, CLP::DUPOPT_ERR,  NULL },
  {  0 , "oa-ujnum",   CLP::ARG_NONE, CLP::DUPOPT_ERR,  NULL },
  {  0 , "whirl2f",    CLP::ARG_NONE, CLP::DUPOPT_ERR,  NULL },
  
  // Options
  { 'd', "dump",       CLP::ARG_NONE, CLP::DUPOPT_CLOB, NULL },
  { 'V', "version",    CLP::ARG_NONE, CLP::DUPOPT_CLOB, NULL },
  { 'h', "help",       CLP::ARG_NONE, CLP::DUPOPT_CLOB, NULL },
  {  0 , "debug",      CLP::ARG_OPT,  CLP::DUPOPT_CLOB, NULL },
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
  debug = 0;      // default: 0 (off)
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
      exit(1);
    }
    if (parser.IsOpt("version")) { 
      PrintVersion(std::cerr);
      exit(1);
    }
    
    // Check for mode
    if (parser.IsOpt("ir")) { runMode = 1; }
    if (parser.IsOpt("oa-ujnum")) { runMode = 2; }
    if (parser.IsOpt("whirl2f")) { runMode = 3; }
    
    // Check for other options
    if (parser.IsOpt("dump")) { dumpIR = true; }
    
    // Check for required arguments
    if (parser.GetNumArgs() != 1) {
      PrintError(std::cerr, "Invalid number of arguments!");
      exit(1);
    }
    whirlFileNm = parser.GetArg(0);
  } 
  catch (CmdLineParser::ParseError& e) {
    PrintError(std::cerr, e.GetMessage());
    exit(1);
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

