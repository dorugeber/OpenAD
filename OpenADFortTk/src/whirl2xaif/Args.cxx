#include <cstdlib>

#include "Args.h"

//*************************** Forward Declarations **************************

using std::cerr;
using std::endl;
using std::string;

//***************************************************************************

static const char* usage_summary =
"[options] <whirl-file>\n";

static const char* usage_details =
"Given a WHIRL file, translates the 'numerical core' into XAIF.  By default,\n"
"output is sent to stdout.\n"
"\n"
"Options:\n"
"  -h, --help          print this help and exit\n"
"  -n, --noFilter      do not filter ud/du chains by current basic block\n"
"  -N, --noTimeStamp   do not print a time stamp into the output\n"
"  -o, --output <file> send output to <file> instead of stdout\n"
"      --prefix <pfx>  Set the temporary variable prefix to <pfx>. Default\n"
"                      is 'OpenAD_'\n"
"  -s, --simpleLoop    force simple loop property on all loop constructs\n"
"  -v, --variedOnly    do not require active data to also be 'useful'\n"
"      --uniformCBact  if any variable in a given common block is active\n"
"                      then activate all of them\n"
"      --allActive     all floating point variables are made active\n"
"      --debug [lvl]   debug mode at level `lvl'\n";

bool Args::ourSimpleLoopFlag=false;   // default: done't force it
bool Args::ourDoNotFilterFlag=false;  // default: filter it
bool Args::ourNoTimeStampFlag=false;  // default: dump a time stamp
bool Args::ourVariedOnlyFlag=false;   // default: require both usefull and varied
bool Args::ourUniformCBactFlag=false; // default: no uniform activation
bool Args::ourAllActiveFlag=false;    // default: no global activation

#define CLP CmdLineParser

CmdLineParser::OptArgDesc Args::optArgs[] = {
  // Options
  { 'o', "output",      CLP::ARG_REQ , CLP::DUPOPT_ERR,  NULL },
  {  0 , "prefix",      CLP::ARG_OPT,  CLP::DUPOPT_CLOB, NULL },
  { 'v', "variedOnly",  CLP::ARG_NONE, CLP::DUPOPT_CLOB, NULL },
  { 'h', "help",        CLP::ARG_NONE, CLP::DUPOPT_CLOB, NULL },
  { 's', "simpleLoop",  CLP::ARG_NONE, CLP::DUPOPT_ERR,  NULL },
  { 'n', "noFilter",    CLP::ARG_NONE, CLP::DUPOPT_ERR,  NULL },
  { 'N', "noTimeStamp", CLP::ARG_NONE, CLP::DUPOPT_ERR,  NULL },
  {  0 , "uniformCBact",CLP::ARG_NONE, CLP::DUPOPT_ERR,  NULL },
  {  0 , "allActive",   CLP::ARG_NONE, CLP::DUPOPT_ERR,  NULL },
  {  0 , "debug",       CLP::ARG_OPT,  CLP::DUPOPT_CLOB, NULL },
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
  ourVariedOnlyFlag=false;
  tmpVarPrefix = "OpenAD_"; // default prefix
  debug = 0;                // default: 0 (off)
}

Args::~Args()
{
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
    if (parser.IsOpt("variedOnly")) { 
      ourVariedOnlyFlag=true;
    }
    if (parser.IsOpt("output")) { 
      xaifFileNm = parser.GetOptArg("output");
    }    
    if (parser.IsOpt("prefix")) { 
      tmpVarPrefix = parser.GetOptArg("prefix");
    }
    if (parser.IsOpt("simpleLoop")) { 
      ourSimpleLoopFlag = true; 
    }
    if (parser.IsOpt("noFilter")) { 
      ourDoNotFilterFlag = true; 
    }
    if (parser.IsOpt("noTimeStamp")) { 
      ourNoTimeStampFlag= true; 
    }
    if (parser.IsOpt("uniformCBact")) { 
      ourUniformCBactFlag = true; 
    }
    if (parser.IsOpt("allActive")) { 
      ourAllActiveFlag = true; 
    }
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

