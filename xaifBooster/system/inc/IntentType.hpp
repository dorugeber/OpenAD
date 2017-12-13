#ifndef _INTENTTYPE_INCLUDE_
#define _INTENTTYPE_INCLUDE_
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
  class IntentType { 
  public:
    enum IntentType_E {IN_ITYPE,
		       OUT_ITYPE,
		       INOUT_ITYPE};
    static std::string toString(const IntentType_E& aType) throw (PrintingIntException);

    static const IntentType_E fromString(const std::string& aName);
    
    static const std::string our_attribute_XAIFName;

  }; // end of class IntentType

} // end of namespace xaifBooster
                                                                     
#endif
