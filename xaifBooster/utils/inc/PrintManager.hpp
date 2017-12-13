#ifndef _PRINTMANAGER_INCLUDE_
#define _PRINTMANAGER_INCLUDE_
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
   * this class deals with indentation and verbosity
   */
  class PrintManager {
  public: 

    /**
     * get an instance and increase the indent
     * to be used in a pair with releaseInstance
     */
    static PrintManager& getInstance();

    /**
     * release the instance and decrease the indent
     * to be used in a pair with getInstance
     */
    void releaseInstance();

    /** 
     * produces a string with spaces according to the 
     * current indentation
     */
    std::string indent();

    /** 
     * produces a string with spaces according to the 
     * current indentation + additionalIndent spances
     */
    std::string indent(unsigned int additionalIndent);

    static const unsigned int ourDefaultIndentation;

    /** 
     * turns on verbose output (default off)
     */
    static void setVerbose();

    /** 
     * queries verbosity
     */
    static bool isVerbose();

  private:

    static PrintManager* ourPrintManagerInstance_p;

    unsigned int my_indentation;

    static bool ourVerbosity;

    PrintManager();

    /** 
     * no def
     */
    PrintManager(const PrintManager&);

    /** 
     * no def
     */
    PrintManager& operator=(const PrintManager&);

    ~PrintManager(){};

  }; // end of class PrintManager

} // end of namespace xaifBooster

#endif
