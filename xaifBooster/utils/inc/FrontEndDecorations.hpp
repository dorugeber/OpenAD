#ifndef _FRONTENDDECORATIONS_INCLUDE_
#define _FRONTENDDECORATIONS_INCLUDE_
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
  class FrontEndDecorations { 

  public:
    
    /**
     * enumeration setting bits in an integer 
     * for easy selection an combination of 
     * categories
     */
    enum FrontEndDecorations_E {OPEN64_STYLE  = 0,
				NO_STYLE      = 1};

    static std::string toString(const FrontEndDecorations_E& aKind) throw (PrintingIntException);
    
    static const FrontEndDecorations_E fromString(const std::string& aName);

    static std::string printAll() throw (PrintingIntException);

  }; // end of class FrontEndDecorations
  
} // end of namespace xaifBooster
                                                                     
#endif
