#ifndef _THREAD_INCLUDE_
#define _THREAD_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

namespace xaifBooster { 

  /** 
   * our own wrapper class for the 
   * the different thread libraries
   * for now provides only an identifier
   */
  class Thread { 

  public: 

    /**
     * unless we have threads
     * this just returns 0
     */
    static unsigned int threadId();

  }; // end of class Thread 

}

#endif
