#ifndef _ELIMINATIONEXCEPTION_INCLUDE_
#define _ELIMINATIONEXCEPTION_INCLUDE_
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

namespace xaifBoosterCrossCountryInterface {

  /** 
   * as the name says we have some elimination problem 
   * here which may be a non-fatal error, hence the
   * separate exception class 
   */
  class EliminationException { 

  public:

    /** 
     * param reason should be one line
     */
    EliminationException(std::string reason):
      myReason(reason) { 
    };

    ~EliminationException(){};

    /** 
     * a one line reason text for the exception
     */
    std::string getReason() const {return myReason;};

  private:

    /** 
     * a one liner for the reason
     */
    std::string myReason;


  }; // end of class PrintingException 
  
} 
                                                                     
#endif
