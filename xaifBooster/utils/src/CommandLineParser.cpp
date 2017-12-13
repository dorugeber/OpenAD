// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/utils/inc/CommandLineParser.hpp"
#include "xaifBooster/utils/inc/StringConversions.hpp"

namespace xaifBooster { 

  CommandLineParser::CommandLineParser(int argc, 
				       char** argv) :
    myArgc(argc), 
    myArgv(argv),
    myParsedFlag(false) { 
  } 

  CommandLineParser::~CommandLineParser() {
  } 
  
  void
  CommandLineParser::parse(const std::string& theSwitches) {
    if (myParsedFlag)
      THROW_LOGICEXCEPTION_MACRO("CommandLineParser::parse(): cannot parse more than once!");
    myCommandLineArguments.addSwitches(theSwitches);
    // the first piece after the program name should be a
    // switch or a group of switches. 
    CommandLineSegment theSegment(myArgc, myArgv);
    while (theSegment.notDone()){
      theSegment.parse();
      myCommandLineArguments.setSegment(theSegment); 
    } // end while 
    myParsedFlag=true; 
  }
     
  void
  CommandLineParser::displayArguments() const {
    if (!myParsedFlag)
      THROW_LOGICEXCEPTION_MACRO("CommandLineParser::displayArguments parse has not been called!");
    myCommandLineArguments.dump();
  }

  const std::string& 
  CommandLineParser::argAsString(char theSwitch) const {
    if (!myParsedFlag)
      THROW_LOGICEXCEPTION_MACRO("CommandLineParser::argAsString parse has not been called!");
    return myCommandLineArguments.getArgument(theSwitch);
  } // end CommandLineParser::displayArguments

  int
  CommandLineParser::argAsInt(char theSwitch) const {
    if (!myParsedFlag)
      THROW_LOGICEXCEPTION_MACRO("CommandLineParser::argAsInt parse has not been called!");
    int value;
    std::string temp(myCommandLineArguments.getArgument(theSwitch));
    if (temp.empty()) { // make sure that is something
      THROW_LOGICEXCEPTION_MACRO("CommandLineParser::argAsInt(): "
				 << "The argument for switch \"-" 
				 << theSwitch 
				 << "\"" 
				 << "has been set or has defaulted to an empty string which cannot " 
				 << "be converted to an integer!");
    } // end if 
    try { 
      value=StringConversions::convertToInt(temp);
    } catch (BaseException& e) {
      THROW_LOGICEXCEPTION_MACRO("CommandLineParser::argAsInt(): "  
				 << "The argument for switch \"-" 
				 << theSwitch 
				 << "\" is " 
				 << "\"" << temp << "\" "
				 << e.getReason());
    }
    return value;
  } // end CommandLineParser::argAsInteger

  double
  CommandLineParser::argAsDouble(char theSwitch) const  {
    if (!myParsedFlag)
      THROW_LOGICEXCEPTION_MACRO("CommandLineParser::argAsDouble parse has not been called!");
    double value;
    std::string temp(myCommandLineArguments.getArgument(theSwitch));
    if (temp.empty()) { // make sure that is something
      THROW_LOGICEXCEPTION_MACRO("CommandLineParser::argAsDouble(): "
				 << "The argument for switch \"-" 
				 << theSwitch 
				 << "\"" 
				 << "has been set or has defaulted to an empty string which cannot " 
				 << "be converted to a double!");
    } // end if 
    try { 
      value=StringConversions::convertToDouble(temp);
    } catch (BaseException& e) {
      THROW_LOGICEXCEPTION_MACRO("CommandLineParser::argAsDouble(): "  
				 << "The argument for switch \"-" 
				 << theSwitch 
				 << "\" is " 
				 << "\"" << temp << "\" "
				 << e.getReason());
    }
    return value;
  } // end CommandLineParser::argAsDouble

  bool
  CommandLineParser::isSet(char theSwitch) const {
    if (!myParsedFlag)
      THROW_LOGICEXCEPTION_MACRO("CommandLineParser::isSet parse has not been called!");
    return myCommandLineArguments.isSet(theSwitch);
  } // end CommandLineParser::argAsDouble

} // end of namespace xaifBooster
