// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/utils/inc/PrintingIntException.hpp"

namespace xaifBooster { 

  PrintingIntException::PrintingIntException(std::string what, 
					     int aValue) { 
    std::ostringstream reason;
    reason << "PrintingIntException: " << what << " (" << aValue << ")";
    convertToOneLineReason(reason);
  } // end of PrintingIntException::PrintingIntException
  
} // end of namespace xaifBooster
                                                                     


