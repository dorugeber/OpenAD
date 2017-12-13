
#ifndef _GENERICACTION_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#define _GENERICACTION_INCLUDE_

#include <string>
#include "xaifBooster/utils/inc/PrintingIntException.hpp"

namespace xaifBooster { 

  /** 
   * enum class to identify an action
   * to be performed by an algorithm.
   */
  class GenericAction { 
public:

    enum GenericAction_E {ALGORITHM_INIT,
			  ALGORITHM_ACTION_1,
			  ALGORITHM_ACTION_2,
			  ALGORITHM_ACTION_3,
			  ALGORITHM_ACTION_4,
			  ALGORITHM_ACTION_5,
			  ALGORITHM_ACTION_6,
			  ALGORITHM_ACTION_7,
			  ALGORITHM_ACTION_8};

    static std::string toString(const GenericAction_E& anAction) throw (PrintingIntException);

    static const GenericAction_E fromString(const std::string& aName);
    
    static const std::string our_attribute_XAIFName;

  }; // end of class GenericAction

} // end of namespace xaifBooster
                                                                     
#endif
