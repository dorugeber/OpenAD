#ifndef _DBGLOGGER_INCLUDE_
#define _DBGLOGGER_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/utils/inc/BaseLogger.hpp"

namespace xaifBooster { 

  /** 
   * class used by DbgLoggerManager
   */
  class DbgLogger :public BaseLogger { 

  public: 
 
    DbgLogger(std::ostream& anOstream_r);

    virtual void logMessage(std::ostringstream& aMessage_r); 

    virtual ~DbgLogger();

  }; // end of class DbgLogger

}

#endif









