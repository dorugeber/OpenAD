// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/utils/inc/Mutex.hpp"

namespace xaifBooster { 
  
  Mutex::Mutex() 
#ifdef POSIX_THREADS
    // always assume a fast mutex... otherwise 
    // we need to be more specific in the error handling
    : myMutex(PTHREAD_MUTEX_INITIALIZER)
#endif
  { }

  Mutex::~Mutex() {
#ifdef POSIX_THREADS
    int rValue=pthread_mutex_destroy(&myMutex);
    if (rValue)
      THROW_LOGICEXCEPTION_MACRO("Mutex::~Mutex(): unexpected failure, still locked");
#endif
  }
    
  void Mutex::lock() { 
#ifdef POSIX_THREADS
    int rValue=pthread_mutex_lock(&myMutex);
    if (rValue)
      THROW_LOGICEXCEPTION_MACRO("Mutex::lock(): unexpected failure");
#endif
  } 
    
  void Mutex::unlock() { 
#ifdef POSIX_THREADS
    int rValue=pthread_mutex_unlock(&myMutex);
    if (rValue)
      THROW_LOGICEXCEPTION_MACRO("Mutex::unlock(): unexpected failure");
#endif
  } 

  
} // end of namespace xaifBooster

