#ifndef _XAIFBOOSTER_PRINTVERSION_INCLUDE_
#define _XAIFBOOSTER_PRINTVERSION_INCLUDE_
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
   * version of printing 
   * to be chosen
   * either VIRTUAL (the default) 
   * meaning that printing of the elements 
   * uses virtual invocations of printXMLHierarchy 
   * OR set to SYSTEM_ONLY
   * which means we prevent the virtual invocation 
   * and just print the system version only. 
   */
  class PrintVersion { 

  public:
    
    enum PrintVersion_E {VIRTUAL,
			 SYSTEM_ONLY};

    static std::string toString(const PrintVersion_E& aKind) throw (PrintingIntException);

    static const PrintVersion_E fromString(const std::string& aName);
    
  }; // end of class PrintVersion

} 
                                                                     
#endif
