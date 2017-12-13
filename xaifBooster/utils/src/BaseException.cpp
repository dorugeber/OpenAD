// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/utils/inc/BaseException.hpp"

namespace xaifBooster { 

  void BaseException::convertToOneLineReason(std::ostringstream& err) { 
    err << std::ends; 
    myReason=err.str(); 
  } // end of BaseException::convertToOneLineReason
  
} // end of namespace xaifBooster

