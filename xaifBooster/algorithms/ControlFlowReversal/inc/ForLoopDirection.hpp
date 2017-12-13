#ifndef _XAIFBOOSTERCONTROLFLOWREVERSAL_FORLOOPDIRECTION_INCLUDE_
#define _XAIFBOOSTERCONTROLFLOWREVERSAL_FORLOOPDIRECTION_INCLUDE_
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

using namespace xaifBooster;

namespace xaifBoosterControlFlowReversal {  

  class ForLoopDirection { 
  public:
    enum ForLoopDirection_E {COUNT_UP,
			     COUNT_DOWN,
			     COUNT_UNDECIDED};
    static std::string toString(const ForLoopDirection_E& aType) throw (PrintingIntException);

    static const ForLoopDirection_E fromString(const std::string& aName);
    
    static const std::string our_attribute_XAIFName;

  }; 

} 
                                                                     
#endif
