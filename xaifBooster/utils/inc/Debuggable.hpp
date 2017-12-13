#ifndef _DEBUGGABLE_INCLUDE_
#define _DEBUGGABLE_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include <string>

namespace xaifBooster { 

  /**
   *  an abstract base class for debugging
   */
  class Debuggable {
  public:

    Debuggable() {};

    virtual ~Debuggable() {};
    
    /** 
     * one line debug information of the object instance
     * without descend into the hierarchy. This should 
     * facilitate grep/awk through a large number of debugged 
     * instances. 
     */
    virtual std::string debug() const =0; 

  };  // end  of class Debuggable 

} // end of namespace xaifBooster
                                                                     
#endif

