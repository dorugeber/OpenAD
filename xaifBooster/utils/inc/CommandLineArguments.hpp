#ifndef _COMMANDLINEARGUMENTS_INCLUDE_
#define _COMMANDLINEARGUMENTS_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/utils/inc/CommandLineSegment.hpp"
#include "xaifBooster/utils/inc/LogicException.hpp"
#include <map>

namespace xaifBooster { 


  /** 
   * the class holding the command line 
   * arguments 
   */
  class CommandLineArguments {
  private: 

    /** 
     * class holds the argument and 
     * a flag indicating if it has been 
     * set
     */
    class CommandLineSwitchArgument { 
    public: 
      CommandLineSwitchArgument(); 
      ~CommandLineSwitchArgument(){};
      void setArgument(const std::string& theArgument);
      const std::string& getArgument() const;
      bool isSet() const;
    private: 
      std::string myArgument;
      bool mySetFlag;
    }; // end of class CommandLineSwitchArgument

  public: 

    CommandLineArguments();

    ~CommandLineArguments(); 

    /** 
     * supply all the potential switches
     */
    void addSwitches(const std::string& theSwitches); 

    /** 
     * set a switch/argument pair
     */
    void setSegment(const CommandLineSegment& theSegment);
 
    /**
     * retrieve an argument from the parsed command line
     */
    const std::string& getArgument(char theSwitch) const ;

    /** 
     * has a switch been set?
     */
    bool isSet(char theSwitch) const ;

    typedef std::map<char, CommandLineSwitchArgument> 
    SwitchArgumentMap;

    /** 
     * throws an exception for any non-alpha 
     * character
     */
    static void checkAllowedAsSwitch(char a);

    std::string dump() const;
    
  private:

    /** 
     * the argument map
     * is constructed with all potential 
     * switches and the CommandLineSwitchArgument 
     * members are set during parsing
     */
    SwitchArgumentMap mySwitchArgumentMap; 
    
  }; // end class CommandLineArguments

} // end of namespace xaifBooster
   
#endif







