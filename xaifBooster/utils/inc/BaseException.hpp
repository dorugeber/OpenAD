#ifndef _BASEEXCEPTION_INCLUDE_
#define _BASEEXCEPTION_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include <sstream>
#include <string>

namespace xaifBooster { 

  /** 
   * base class for any exception thrown 
   * by xaifbooster
   */
  class BaseException { 
  public:
    
    /** 
     * a one line reason text for the exception
     */
    std::string getReason() const {return myReason;};

    virtual ~BaseException(){};

  protected:

    /** 
     * to be used by the derived classes
     * for convenience with a stream to avoid 
     * explicit conversions
     */
    void convertToOneLineReason(std::ostringstream& err);

    BaseException(){};

  private:

    /** 
     * a one liner for the reason
     */
    std::string myReason;

  };

   /**
   * use of preprocessor defined __FILE__ and __LINE__ requires
   * use of a macro. We have to define a strange name
   * for local variables to avoid accidental name matching -
   * the niceties of macros!
   * Note the enclosing {} allowing for
   * if (...)
   *   THROW_LOGICEXCEPTION_MACRO(...);
   * without extra brackets but the compiler will complain
   * about
   * if (...) {
   *   THROW_LOGICEXCEPTION_MACRO(...);
   * }
   * because it doesn't know what to do with the extra ';' in the end;
   * make your choice.
   * the  argument is an argument to an
   * ostream << operator.
   */
#ifndef THROW_EXCEPTION_MACRO
#define THROW_EXCEPTION_MACRO(className,StreamArgs) \
   { \
     std::ostringstream aLoNgAnDwEiRdLoCaLnAmeFoRtHiSmAcRoOnLy; \
     aLoNgAnDwEiRdLoCaLnAmeFoRtHiSmAcRoOnLy << StreamArgs << std::ends; \
     throw className(__FILE__,__LINE__,aLoNgAnDwEiRdLoCaLnAmeFoRtHiSmAcRoOnLy.str()); \
   }
#else
#error macro name for THROW_LOGICEXCEPTION_MACRO already in use
#endif

} // end of namespace xaifBooster
                                                                     
#endif
