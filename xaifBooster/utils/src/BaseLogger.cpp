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

  Mutex BaseLogger::ourMutex;

  BaseLogger::BaseLogger(std::ostream& anOstream_r) :
    myOstream_r(anOstream_r) {
  } // end of BaseLogger::BaseLogger

  void
  BaseLogger::intLogMessage(std::ostringstream& aMessage_r) { 
    ourMutex.lock();
    try { 
      aMessage_r << std::endl; 
      myOstream_r << aMessage_r.str().c_str();
      myOstream_r.flush();
    } // end try 
    catch(...) { 
      std::cerr << "BaseLogger::log: unexpected exception thrown" << std::endl; 
    } // end catch 
    ourMutex.unlock();
  } // end of BaseLogger::log

  BaseLogger::~BaseLogger() {
    myOstream_r << std::endl; 
    myOstream_r.flush();
  } // end of BaseLogger::~BaseLogger

}




