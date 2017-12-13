// -*-Mode: C++;-*-
// $Header: /Volumes/cvsrep/developer/OpenADFortTk/src/lib/support/diagnostics.h,v 1.11 2005/03/19 22:54:51 eraxxon Exp $
#ifndef Diagnostics_INCLUDED
#define Diagnostics_INCLUDED

#include "Exception.h"

namespace fortTkSupport {

  /** 
   * some standard error strings 
   * getting and setting the user debug level
   * not really restricted but should be something between 0-9
   */ 
  class Diagnostics { 
  public: 
    static void setDiagnosticFilterLevel(int lvl);
    static int getDiagnosticFilterLevel();
    static const char* Unimplemented; 
    static const char* UnexpectedInput;
    static const char* UnexpectedOpr;
    // This routine is called before an error that stops execution.  It is
    // especially useful for use with debuggers that do not have good
    // exception support.
    static void theMostVisitedBreakpointInHistory(const char* filenm = NULL, 
						  unsigned int lineno = 0);
  private: 
    /** 
     * user debug leve
     * not really restricted but should be something between 0-9
     */
    static int  ourUserDebugLevel;
  };

} // end namespace 

  // Private debugging level: messages for in-house debugging [0-9]
#define FORTTK_DBG_LVL 0

  // FORTTK_DIAGIF: If public diagnostic level is at least 'level' ...
#define FORTTK_DIAGIF(level) if (level <= fortTkSupport::Diagnostics::getDiagnosticFilterLevel())

  // FORTTK_DIAGIF_DEV: If development diagnostic level is at least 'level' ...
#define FORTTK_DIAGIF_DEV(level) if (level <= FORTTK_DBG_LVL)

  // All of these macros have a parameter named 'streamArgs' for one or
  // more ostream arguments. These macros use these arguments to create
  // a message string.  Example:
  //   if (...) FORTTK_ERR("bad val: '" << v << "'")

  // FORTTK_MSG: Print a message if level satisfies the diagnostic filter
#define FORTTK_MSG(level, streamArgs)				\
  if (level <= fortTkSupport::Diagnostics::getDiagnosticFilterLevel()) {	\
    std::cerr << "FortTk: " << streamArgs << std::endl; }

  // FORTTK_DEVMSG: Print a message if private level satisfies the
  // private diagnostic filter
#define FORTTK_DEVMSG(level, streamArgs)				\
  if (level <= FORTTK_DBG_LVL) {					\
    std::cerr << "FortTk[debugLevel=" << level << "]: " << streamArgs << std::endl; }

  // FORTTK_EMSG: Print an error message and continue.
#define FORTTK_EMSG(streamArgs)					\
  { std::cerr << "FortTk: ERROR:";					\
    if (fortTkSupport::Diagnostics::getDiagnosticFilterLevel()) {		\
      std::cerr << "[" << __FILE__ << ":" << __LINE__ << "]"; }	\
    std::cerr << ": " << streamArgs << std::endl; }

  // FORTTK_WMSG: Print an warning message and continue.
#define FORTTK_WMSG(streamArgs)					\
  { std::cerr << "FortTk: WARNING:";					\
    if (fortTkSupport::Diagnostics::getDiagnosticFilterLevel()) {		\
      std::cerr << "[" << __FILE__ << ":" << __LINE__ << "]"; }	\
    std::cerr << ": " << streamArgs << std::endl; }

  // FORTTK_ASSERT: Throw an assertion (die) if 'expr' evaluates to
  // false. Stops at 'FortTk_TheMostVisitedBreakpointInHistory'.
#define FORTTK_ASSERT(expr, streamArgs)		\
  if (!(expr)) FORTTK_THROW(streamArgs)

  // FORTTK_ASSERT_WARN: Print a warning if 'expr' evaluates to false.
  // Stops at 'FortTk_TheMostVisitedBreakpointInHistory'.
#define FORTTK_ASSERT_WARN(expr, streamArgs)	\
  if (!(expr)) FORTTK_WMSG(streamArgs)

  // FORTTK_DIE: Print an error message and die.  Stops at
  // 'FortTk_TheMostVisitedBreakpointInHistory'.
  // (Equivalent to FORTTK_THROW.)
#define FORTTK_DIE(streamArgs)			\
  FORTTK_THROW(streamArgs)

  // FORTTK_THROW: (C++ only) Throw a fatal exception.  Stops at
  // 'FortTk_TheMostVisitedBreakpointInHistory'.
  // (Equivalent to FORTTK_DIE.) Based on Jean Utke's code in xaifBooster.
#define FORTTK_THROW(streamArgs)					\
  { std::ostringstream WeIrDnAmE;					\
    WeIrDnAmE << streamArgs << std::ends;				\
    throw fortTkSupport::FatalException(WeIrDnAmE.str(), __FILE__, __LINE__); }

#endif 

