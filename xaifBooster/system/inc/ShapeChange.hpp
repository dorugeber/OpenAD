#ifndef _SHAPECHANGE_INCLUDE_
#define _SHAPECHANGE_INCLUDE_
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
  class ShapeChange { 
public:
    enum ShapeChange_E {NO_CHANGE=0,
			RANK=1,
			SCALAR=2};

    static std::string toString(const ShapeChange_E& aKind) throw (PrintingIntException);

    static const ShapeChange_E fromString(const std::string& aName);
    
    static const std::string our_attribute_XAIFName;

  };

} 
                                                                     
#endif
