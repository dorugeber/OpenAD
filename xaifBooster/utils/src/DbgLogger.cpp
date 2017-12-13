// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/utils/inc/DbgLogger.hpp"

namespace xaifBooster { 

  DbgLogger::DbgLogger(std::ostream& anOstream_r) :
    BaseLogger(anOstream_r) {
  } // end of DbgLogger::DbgLogger

  void
  DbgLogger::logMessage(std::ostringstream& aMessage_r) { 
    intLogMessage(aMessage_r);
  } // end of DbgLogger::log

  DbgLogger::~DbgLogger() {
  } // end of DbgLogger::~DbgLogger

}
