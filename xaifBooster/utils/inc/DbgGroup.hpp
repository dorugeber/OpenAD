#ifndef _DBGGROUP_INCLUDE_
#define _DBGGROUP_INCLUDE_
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
#include "xaifBooster/utils/inc/PrintingIntException.hpp"

namespace xaifBooster { 

  /** 
   * enum class for categorizing debug 
   * messages created with DbgLoggerManager
   */
  class DbgGroup { 

  public:
    
    /**
     * enumeration setting bits in an integer 
     * for easy selection an combination of 
     * categories
     */
    enum DbgGroup_E {ERROR    = 0x000000000, // anything having to do with exceptions
		     WARNING  = 0x000000001, // some potentially dangerous step
		     CALLSTACK= 0x000000002, // any message relating to entering/exiting a method
		     DATA     = 0x000000004, // any dumping of data
		     GRAPHICS = 0x000000008, // anything producing graphic output 
		     TIMING   = 0x000000010, // anything producing a time stamp 
		     TEMPORARY= 0x000000020, // the rest of the messages that can be removed at any time
		     METRIC   = 0x000000040 };

    static std::string toString(const DbgGroup_E& aKind) throw (PrintingIntException);
    
    static std::string printAll() throw (PrintingIntException);

  }; // end of class DbgGroup
  
} // end of namespace xaifBooster
                                                                     
#endif
