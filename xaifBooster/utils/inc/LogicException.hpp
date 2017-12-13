#ifndef _LOGICEXCEPTION_INCLUDE_
#define _LOGICEXCEPTION_INCLUDE_
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
#include "xaifBooster/utils/inc/BaseException.hpp"

namespace xaifBooster { 

  /** 
   * class to be used for any generic exception
   * we expect complete failure upon any such exception
   * Exceptions that point to non-fatal problem 
   * should get their own type to allow selective 
   * handling by the calling environment
   */
  class LogicException : public BaseException { 

  public:

    /** 
     * param what should be one line, 
     * used in THROW_LOGICEXCEPTION_MACRO
     */
    LogicException(std::string aFileName,
		   int aLineNumber,
		   std::string what);
    ~LogicException(){};
  }; // end of class LogicException 
  
#ifndef THROW_LOGICEXCEPTION_MACRO
#define THROW_LOGICEXCEPTION_MACRO(StreamArgs) THROW_EXCEPTION_MACRO(LogicException,StreamArgs)
#else
#error macro name for THROW_LOGICEXCEPTION_MACRO already in use
#endif

} // end of namespace xaifBooster
                                                                     
#endif
