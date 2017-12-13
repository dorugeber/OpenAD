#ifndef _FORLOOPREVERSALTYPE_INCLUDE_
#define _FORLOOPREVERSALTYPE_INCLUDE_
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
#include <list>
#include "xaifBooster/utils/inc/PrintingIntException.hpp"

namespace xaifBooster { 
  class ForLoopReversalType { 
  public:
    enum ForLoopReversalType_E {EXPLICIT,
				ANONYMOUS,
				HEURISTIC};

    static std::string toString(const ForLoopReversalType_E& aType) throw (PrintingIntException);

    static const ForLoopReversalType_E fromString(const std::string& aName);
    
    static const std::string our_attribute_XAIFName;

    typedef std::list<ForLoopReversalType::ForLoopReversalType_E> TypeList;

    static const TypeList ourTypeList;

  }; // end of class ForLoopReversalType

} // end of namespace xaifBooster
                                                                     
#endif
