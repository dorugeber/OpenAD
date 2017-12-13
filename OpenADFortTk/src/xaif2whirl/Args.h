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

#include "CmdLineParser.h"
#include "xaif2whirl.h"

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
  xaif2whirl::AlgorithmType algorithm; // default: ALG_NULL
  int debug;                           // default: 0 (off)
  
  // Parsed Data: arguments
  std::string inWhirlFileNm;
  std::string outWhirlFileNm;
  std::string xaifFileNm;
  static std::string ourActiveTypeNm;
  static const unsigned short ourActiveTypeNmLength=27;
  bool validate;

  static TYPE_ID ourDefaultMTypeInt;
  static TYPE_ID ourDefaultMTypeUInt;
  static TYPE_ID ourDefaultMTypeReal;

  bool myNoCleanUpFlag;

private:
  void Ctor();
  
private:
  static CmdLineParser::OptArgDesc optArgs[];
  CmdLineParser parser;
}; 

#endif
