// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include <cstring>

#include "xaifBooster/utils/inc/CommandLineSegment.hpp"
#include "xaifBooster/utils/inc/CommandLineArguments.hpp"
#include "xaifBooster/utils/inc/LogicException.hpp"

namespace xaifBooster { 

  CommandLineSegment::CommandLineSegment(int argc, char** argv)
    : myArgv(argv),
      myArgc(argc),
      myArgvIndex(1),
      myContext(argc==1?DONE:SWITCH_BLOCK) { 
    if (!argv) { 
      THROW_LOGICEXCEPTION_MACRO("Error in CommandLineSegment::CommandLineSegment(): the passed argument array is a NULL pointer!");
    } 
  } // end CommandLineSegment::CommandLineSegment()  

  CommandLineSegment::~CommandLineSegment(){
  } // end CommandLineSegment::~CommandLineSegment()

  void
  CommandLineSegment::reset() {
    mySwitches="";
    myArgument="";
  } // end CommandLineSegment::addSwitch()

  void
  CommandLineSegment::parse(){
    reset();
    if (myArgvIndex>=myArgc) { // e.g. if we start out with no arguments
      myContext=DONE;
      return;
    } 
    char* thisArg_p=myArgv[myArgvIndex];
    if (!thisArg_p) { 
      THROW_LOGICEXCEPTION_MACRO("Error in CommandLineSegment::parse(): the passed argument is a NULL pointer!");
    } 
    if (!strlen(thisArg_p)) {
      THROW_LOGICEXCEPTION_MACRO("Error in CommandLineSegment::parse(): the passed argument is empty!");
    } 
    if (thisArg_p[0]=='-'){ 
      // because this is the first character in the argument 
      // it must have been preceeded by a space, we assume 
      // this is a switch
      parseSwitch();
      // this must be followed by an argument or another switch, which is 
      // handled in the following routine
      parseArgument();
    } // endif
    else { 
      // i.e. the first character is not a dash
      // we always assume that the first step is to find the switch
      if (myContext==ARG_BLOCK) { 
	THROW_LOGICEXCEPTION_MACRO("Error in CommandLineSegment::parse(): entered parse with context ARG_BLOCK!");
      } 
      // handle remaining error conditions
      else if (myContext==SWITCH_BLOCK) { 
	// we would require the first character to be a dash 
	if (myArgvIndex>1) { 
	  THROW_LOGICEXCEPTION_MACRO("Error in CommandLineSegment::parse(): The argument \"" 
				     << myArgv[myArgvIndex] 
				     << "\" is not preceded by a switch. Arguments that contain spaces must be delimited by single quotes!");
	} 
	else { 
	  THROW_LOGICEXCEPTION_MACRO("Error in CommandLineSegment::parse(): The first argument is not preceded by a switch!");
	} 
      } // end else if
      else // this should never happen 
	THROW_LOGICEXCEPTION_MACRO("Error in CommandLineSegment::parse(): context mismatch!");
    } // endelse  
  } // end CommandLineSegment::parse 
     
  void
  CommandLineSegment::parseSwitch(){
    if (myContext==ARG_BLOCK) {
      // we were expecting arguments but apparently found the next switch 
      // block. We assume the current context of switches does not have 
      // arguments
      myContext=SWITCH_BLOCK;
      // return; 
    } // endif 
    char* thisArg_p=myArgv[myArgvIndex];
    if (!thisArg_p) { 
      THROW_LOGICEXCEPTION_MACRO("Error in CommandLineSegment::parseSwitch(): the current argument is a NULL pointer!");
    } 
    if (!strlen(thisArg_p)) { 
      THROW_LOGICEXCEPTION_MACRO("Error in CommandLineSegment::parseSwitch(): the current argument is empty!");
    } 
    int i=1;   
    while (thisArg_p[i]!='\0') {
      CommandLineArguments::checkAllowedAsSwitch(thisArg_p[i]);
      i++;
    } // end while 
    if (i==1) { 
      THROW_LOGICEXCEPTION_MACRO("Error in CommandLineSegment::parseSwitch(): no switch letter is specified!");
    } 
    mySwitches=std::string(thisArg_p+1,strlen(thisArg_p)); 
    // do not copy the leading dash
    myContext=ARG_BLOCK;
    myArgvIndex++;
    if (myArgvIndex==myArgc) 
      myContext=DONE;
    return;
  } // end parseSwitch 

  void
  CommandLineSegment::parseArgument(){
    char* thisArg_p=myArgv[myArgvIndex];
    int escapes =0;
    if (!thisArg_p) {
      // we are probably at the end with an argumentless switch
      // dont
      return;
    } 
    if (!strlen(thisArg_p)) { 
      THROW_LOGICEXCEPTION_MACRO("Error in CommandLineSegment::parseArgument(): the current argument is empty!");
    } 
    if (thisArg_p[0]=='\'') { // this is block of arguments delimited by single quotes
      parseArgumentBlock();
      return;
    } // endif 
    if (thisArg_p[0]=='-') {
      return; // do nothing, wait to reenter parse
    } // endif 
    for(unsigned int i=0;i<strlen(thisArg_p);i++) {
      if (thisArg_p[i]=='\'') {
	if (thisArg_p[i-1]=='\\') { // we can assume i-1 because of the previous test
	  escapes++;
	} //end if
	else { 
	  THROW_LOGICEXCEPTION_MACRO("Error in CommandLineSegment::parseArgument(): The parsed argument \"" 
				     << thisArg_p 
				     << "\" contains an opening single quote in another than the first position! Use \\' to escape the delimiter property of '."); 
	} 
      } // end if  
    } // end for 
    myArgument=copyDroppingEscapes(thisArg_p,escapes);
    myArgvIndex ++;
    myContext=SWITCH_BLOCK;
    if (myArgvIndex==myArgc) 
      myContext=DONE;
    return;
  } // end parseArgument 

  void
  CommandLineSegment::parseArgumentBlock(){
    char* thisArg_p=myArgv[myArgvIndex];
    int matched=0;
    int escapes=0;
    while(!matched){
      escapes=0;
      if (!thisArg_p) 
	THROW_LOGICEXCEPTION_MACRO("Error in CommandLineSegment::parseArgumentBlock(): the current argument is a NULL pointer!");
      if (!strlen(thisArg_p))
	THROW_LOGICEXCEPTION_MACRO("Error in CommandLineSegment::parseArgumentBlock(): the current argument is empty!");
      for(unsigned int i=1;i<strlen(thisArg_p);i++) {
	if (thisArg_p[i]=='\'') {
	  if (thisArg_p[i-1]=='\\')  // we can assume i-1 because we start with 1
	    escapes++; // opening quote
	  else { 
	    if (i<strlen(thisArg_p)-1) { // the closing quote is in the middle of the word
	      THROW_LOGICEXCEPTION_MACRO("Error in CommandLineSegment::parseArgumentBlock(): The parsed argument \"" 
					 << thisArg_p << "\" contains a closing single quote in another than the last position! Use \\' to escape the delimiter property of \'."); 
	    } 
	    else  
	      matched=1; // found the closing quote
	  } // end else
	} // end if 
      } // end for 
      // the whole word is the argument, strip the quotes
      if(thisArg_p[0]=='\''){ // we start a new argument block 
	myArgument=copyDroppingEscapes(thisArg_p+1,escapes);
      } // end if 
      else { // this is a continuation of a partially parsed argument
	myArgument.append(" ");
	myArgument.append(copyDroppingEscapes(thisArg_p,escapes));
      } // end else 
      myArgvIndex ++;
      if (matched) { // argument block complete, return
	myContext=SWITCH_BLOCK;
	if (myArgvIndex==myArgc) 
	  myContext=DONE;
	return;
      }
      else { // block incomplete, remain in while
	if (myArgvIndex==myArgc) { 
	  THROW_LOGICEXCEPTION_MACRO("Error in CommandLineSegment::parseArgumentBlock(): Could not find closing single quote!");
	} 
      } // end else
      thisArg_p=myArgv[myArgvIndex]; // get the next word 
    } // end while  
  } // end getArgumentBlock 

  bool
  CommandLineSegment::notDone() const {
    return (myContext==DONE)?false:true;
  } // end CommandLineSegment::incomplete()

  const std::string& 
  CommandLineSegment::getArgument() const {
    return myArgument;
  } // end CommandLineSegment::getArgument()

  const std::string& 
  CommandLineSegment::getSwitches() const {
    return mySwitches;
  } // end CommandLineSegment::getArgument()

  std::string
  CommandLineSegment::copyDroppingEscapes(char* source, int escapes) {
    std::string returnString;
    char *temp_b, *temp_e;
    temp_b=source;
    if (escapes){
      for (int i=0;i<escapes;i++){
	temp_e=strchr(temp_b,'\\');
	returnString.append(std::string(temp_b,temp_e-temp_b)); // copy the part up to the '\'
	temp_b=temp_e+1; // jump over the '\' for the next segment
      } // end for 
      // there is always something left to be copied, at least the ' that 
      // belongs to the last escape sequence
      returnString.append(temp_b);
    } // end if 
    else {   
      returnString.append(source);
    } // end else
    return returnString;
  } // CommandLineSegment::copyDroppingEscapes

} // end of namespace xaifBooster
