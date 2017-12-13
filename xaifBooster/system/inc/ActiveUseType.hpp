#ifndef _ACTIVEUSETYPE_INCLUDE_
#define _ACTIVEUSETYPE_INCLUDE_
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
  class ActiveUseType { 
public:
    enum ActiveUseType_E {ACTIVEUSE,
			  PASSIVEUSE,
			  UNDEFINEDUSE};
    static std::string toString(const ActiveUseType_E& aKind) throw (PrintingIntException);

    static const ActiveUseType_E fromString(const std::string& aName);
    
    static const std::string our_attribute_XAIFName;

  }; // end of class ActiveUseType

} // end of namespace xaifBooster
                                                                     
#endif
