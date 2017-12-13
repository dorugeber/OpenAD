// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/utils/inc/Thread.hpp"

#ifdef POSIX_THREADS
#include <pthread.h>
#endif

namespace xaifBooster { 
  
  unsigned int Thread::threadId() { 
#ifdef POSIX_THREADS
    return pthread_self();
#else
    return 0;
#endif   
  } 
  
} // end of namespace xaifBooster

