#ifndef _COMMANDLINESEGMENT_INCLUDE_
#define _COMMANDLINESEGMENT_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include <string>

namespace xaifBooster { 

  /** 
   * class parses the command line into one pair of 
   * switch(es) followed by an argument(block)
   * each step is a parse after 
   * which the respective switch/argument pair
   * can be retrieved
   */
  class CommandLineSegment {

  public: 

    /** 
     * argc/argv are the arguments from 'main'
     */
    CommandLineSegment(int argc, char** argv);

    ~CommandLineSegment(); 

    /**
     * parse the next switch/argument pair
     */ 
    void parse();
  
    /** 
     * is there something left to parse
     */
    bool notDone() const;

    /** 
     * get the switch(es) 
     * with the leading '-'
     */
    const std::string& getSwitches() const;

    /** 
     * get the argumen(block) 
     * without escape characters
     */
    const std::string& getArgument() const;

  private:

    /**
     * no def
     */
    CommandLineSegment();

    /**
     * resets for the next pair
     */
    void reset(); 

    /** 
     * parses switch(es)
     */
    void parseSwitch();
  
    /** 
     * parses a single argument
     */
    void parseArgument();

    /** 
     * parses a block of arguments, 
     * i.e. arguments delimited by single quotes
     */
    void parseArgumentBlock();

    /** 
     * the switches found in the current 
     * context
     */
    std::string mySwitches;

    /** 
     * the arguments found in the current 
     * context
     */
    std::string myArgument;

    /** 
     * my copy of argv;
     * not a deep copy
     */
    char** myArgv;

    /** 
     * my copy of argc
     */
    const int myArgc;

    /** 
     * my current index into argv
     */
    int myArgvIndex;

    /**
     * while parsing we find escapes '\' 
     * that need to be dropped
     */
    std::string copyDroppingEscapes(char* argument,int numberOfEscapes);

    /** 
     * the current parsing context
     */
    enum Context {SWITCH_BLOCK,
		  ARG_BLOCK, 
		  DONE} myContext;

  }; // end class CommandLineSegment
   
} // end of namespace xaifBooster

#endif








