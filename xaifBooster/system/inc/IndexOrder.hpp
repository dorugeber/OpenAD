#ifndef _INDEXORDER_INCLUDE_
#define _INDEXORDER_INCLUDE_
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
  class IndexOrder { 
public:
    enum IndexOrder_E {
	    ROWMAJOR=0,
	    COLUMNMAJOR=1};
    
    static std::string toString(const IndexOrder_E& aShape) throw (PrintingIntException);

    static const IndexOrder_E fromString(const std::string& aName);
    
  }; // end of class IndexOrder
} // end of namespace xaifBooster
                                                                     
#endif
