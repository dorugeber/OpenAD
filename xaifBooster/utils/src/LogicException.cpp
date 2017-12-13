// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/utils/inc/LogicException.hpp"

namespace xaifBooster { 

  LogicException::LogicException(std::string aFileName,
				 int aLineNumber,
				 std::string what) { 
    std::ostringstream reason;
    reason << "(" 
	   << aFileName.c_str()
	   << ":"
	   << aLineNumber
	   << ")LogicException: " << what ;
    convertToOneLineReason(reason);
  } // end of LogicException::LogicException
  
} // end of namespace xaifBooster
                                                                     


