#ifndef _MUTEX_INCLUDE_
#define _MUTEX_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

// For POSIX mutex
// which is for now the thread implementation 
// If another thread implementation is selected 
// the appropriate calls have to be added in the code
#ifdef POSIX_THREADS
#include <pthread.h>
#endif

namespace xaifBooster { 

  /** 
   * our own wrapper class for the 
   * the different thread libraries
   * currently only for Posix threads, 
   * is empty if POSIX_THREADS
   * is not defined
   * for now provides only lock and unlock
   * will be extended as needed
   */
  class Mutex { 

  public: 

    Mutex();

    virtual ~Mutex();
    
    void lock();
    
    void unlock();

  private: 

#ifdef POSIX_THREADS
    pthread_mutex_t myMutex;
#endif

  }; // end of class Mutex 

}

#endif
