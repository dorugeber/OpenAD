// -*-Mode: C++;-*-
// $Header: /m_home/m_utkej/Argonne/cvs2svn/cvs/OpenADFortTk/src/lib/support/Diagnostics.cxx,v 1.1 2007-10-08 18:28:32 utke Exp $

#include "Diagnostics.h"

namespace fortTkSupport { 

  int Diagnostics::ourUserDebugLevel = 0;
  const char* Diagnostics::Unimplemented = "Unimplemented feature: ";
  const char* Diagnostics::UnexpectedInput = "Unexpected input: ";
  const char* Diagnostics::UnexpectedOpr = "Unexpected operator: ";

  void Diagnostics::setDiagnosticFilterLevel(int lvl) {
    ourUserDebugLevel = lvl;
  }

  int Diagnostics::getDiagnosticFilterLevel() {
    return ourUserDebugLevel;
  }

  void Diagnostics::theMostVisitedBreakpointInHistory(const char* filenm, unsigned int lineno) {
    // Prevent this routine from ever being inlined
    static unsigned count = 0;
    count++;
  }

}
