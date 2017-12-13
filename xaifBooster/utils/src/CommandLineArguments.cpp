// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/utils/inc/CommandLineArguments.hpp"

namespace xaifBooster { 

  CommandLineArguments::CommandLineSwitchArgument::CommandLineSwitchArgument() : 
    mySetFlag(false) { 
  } // end of CommandLineArguments::CommandLineSwitchArgument::CommandLineSwitchArgument

  void CommandLineArguments::CommandLineSwitchArgument::setArgument(const std::string& theArgument) { 
    if (mySetFlag)
      THROW_LOGICEXCEPTION_MACRO("CommandLineArguments::CommandLineSwitchArgument::setArgument: argument already set");
    mySetFlag=true;
    myArgument=theArgument;
  } 

  const std::string& CommandLineArguments::CommandLineSwitchArgument::getArgument() const { 
    if (!mySetFlag)
      THROW_LOGICEXCEPTION_MACRO("CommandLineArguments::CommandLineSwitchArgument::getArgument: not set");
    return myArgument;
  } 

  bool CommandLineArguments::CommandLineSwitchArgument::isSet() const { 
    return mySetFlag;
  } 

  CommandLineArguments::CommandLineArguments() { 
  } 
 
  CommandLineArguments::~CommandLineArguments() {
  }

  void
  CommandLineArguments::addSwitches(const std::string& theSwitches){
    for (std::string::const_iterator i=theSwitches.begin();
	 i!=theSwitches.end();
	 ++i) { 
      checkAllowedAsSwitch(*i);
      mySwitchArgumentMap.insert(std::pair<const char,CommandLineSwitchArgument>(*i,CommandLineSwitchArgument()));
    } // endfor 
  }  

  void 
  CommandLineArguments::checkAllowedAsSwitch(char a){
    if ( (a < 'A') || (a > 'z' ) || ((a > 'Z') && (a < 'a'))) 
      THROW_LOGICEXCEPTION_MACRO("CommandLineSegment::checkAllowedAsSwitch: \""
				 << a 
				 << "\" is not allowed as switch character");
  } // end CommandLineArguments::checkAllowedAsSwitch

  void
  CommandLineArguments::setSegment(const CommandLineSegment& theSegment){
    std::string theSwitches(theSegment.getSwitches());
    for(unsigned int i=0;
	i<theSwitches.length()-1;
	++i) { 
      SwitchArgumentMap::iterator mapI=
	mySwitchArgumentMap.find(theSwitches[i]);
      if (mapI==mySwitchArgumentMap.end()) 
	THROW_LOGICEXCEPTION_MACRO("CommandLineArguments::setSegment: character \""
				   << theSwitches[i]
				   << "\" is not in the set of defined switches");
      if (i+1==theSwitches.length()-1) 
	// only the last switch has the 
	// argument as its value
	(*mapI).second.setArgument(theSegment.getArgument());
      else
	// all the switches except for the last are set to ""
	(*mapI).second.setArgument("");
    } // end for 
  } // end addArgument 
  
  const std::string&
  CommandLineArguments::getArgument(char theSwitch) const {
    SwitchArgumentMap::const_iterator mapI=
      mySwitchArgumentMap.find(theSwitch);
    if (mapI==mySwitchArgumentMap.end()) 
      THROW_LOGICEXCEPTION_MACRO("CommandLineArguments::getArgument: character \""
				 << theSwitch
				 << "\" is not in the set of defined switches");
    if (!(*mapI).second.isSet())
      THROW_LOGICEXCEPTION_MACRO("CommandLineArguments::getArgument: \""
				 << theSwitch
				 << "\" was not set");
    return ((*mapI).second.getArgument());
  } // end getArgument 
    
  bool
  CommandLineArguments::isSet(char theSwitch) const {
    SwitchArgumentMap::const_iterator mapI=
      mySwitchArgumentMap.find(theSwitch);
    if (mapI==mySwitchArgumentMap.end()) 
      THROW_LOGICEXCEPTION_MACRO("CommandLineArguments::isSet: character \""
				 << theSwitch
				 << "\" is not in the set of defined switches");
    return (*mapI).second.isSet();
  } // end isSet 

  std::string CommandLineArguments::dump() const { 
    std::string returnString;
    for(SwitchArgumentMap::const_iterator mapI=
	  mySwitchArgumentMap.begin();
	mapI!=mySwitchArgumentMap.end();
	++mapI) { 
      returnString+="[";
      returnString+=(*mapI).first;
      returnString+="][";
      if ((*mapI).second.isSet()) 
	returnString+="+]["+(*mapI).second.getArgument()+"]";
      else 
	returnString+="-][]";
    }
    return returnString;
  }  

} // end of namespace xaifBooster
