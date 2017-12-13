// ##########################################################
// # This file is part of OpenADFortTk.                     #
// # The full COPYRIGHT notice can be found in the top      #
// # level directory of the OpenADFortTk source tree.       #
// # For more information visit                             #
// # http://www.mcs.anl.gov/openad                          #
// ##########################################################

#ifndef Args_h
#define Args_h

#include <iostream>
#include <string>

#include <CmdLineParser.h>

class Args {
public: 
  Args(); 
  Args(int argc, const char* const argv[]);
  ~Args(); 
  
  // Parse the command line
  void Parse(int argc, const char* const argv[]);
  
  // Version and Usage information
  void PrintVersion(std::ostream& os) const;
  void PrintUsage(std::ostream& os) const;
  
  // Error
  void PrintError(std::ostream& os, const char* msg) const;
  void PrintError(std::ostream& os, const std::string& msg) const;

  // Dump
  void Dump(std::ostream& os = std::cerr) const;
  void DDump() const;

public:
  // Parsed Data: Command
  const std::string& GetCmd() const { return parser.GetCmd(); }

  // Parsed Data: optional arguments
  std::string xaifFileNm;
  std::string tmpVarPrefix;
  int debug;   // default: 0 (off)
  
  // Parsed Data: arguments
  std::string whirlFileNm;
  
  // force simple loop construct
  static bool ourSimpleLoopFlag;

  // do not filter uddu chains by basic block
  static bool ourDoNotFilterFlag; 

  // don't print a time stamp
  static bool ourNoTimeStampFlag;

  // do not require useful analysis
  static bool ourVariedOnlyFlag; 

  // uniformly activate all variables in a common block if one is active 
  static bool ourUniformCBactFlag; 

  // activate all floating point variables
  static bool ourAllActiveFlag; 

private:
  void Ctor();

private:
  static CmdLineParser::OptArgDesc optArgs[];
  CmdLineParser parser;
}; 

#endif
