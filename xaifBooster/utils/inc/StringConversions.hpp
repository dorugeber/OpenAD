#ifndef _STRINGCONVERSIONS_INCLUDE_
#define _STRINGCONVERSIONS_INCLUDE_
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

  class StringConversions { 

  public:
    
    static double convertToDouble(const std::string& aValue);

    static float convertToFloat(const std::string& aValue);

    static int convertToInt(const std::string& aValue);

    static unsigned int convertToUInt(const std::string& aValue);
    
  }; // end of class StringConversions
  
} // end of namespace xaifBooster
                                                                     
#endif
