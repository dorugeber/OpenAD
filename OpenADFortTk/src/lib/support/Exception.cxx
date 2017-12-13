// -*-Mode: C++;-*-
// $Header: /m_home/m_utkej/Argonne/cvs2svn/cvs/OpenADFortTk/src/lib/support/Exception.cxx,v 1.4 2008-05-21 15:13:01 utke Exp $

#include "Exception.h"
#include "Diagnostics.h"

namespace fortTkSupport { 

  Exception::Exception(const char* m,
		       const char* filenm, unsigned int lineno) {
    std::string str = m;
    Ctor(str, filenm, lineno);
    Diagnostics::theMostVisitedBreakpointInHistory(filenm, lineno);
  }


  Exception::Exception(std::string m,
		       const char* filenm, unsigned int lineno) {
    Ctor(m, filenm, lineno);
    Diagnostics::theMostVisitedBreakpointInHistory(filenm, lineno);
  }


  Exception::~Exception() {
  }


  void 
  Exception::Ctor(std::string& m, 
		  const char* filenm, unsigned int lineno) {
    if (filenm && lineno != 0) {
      std::ostringstream os;
      os << "[" << filenm << ":" << lineno << "]: " << m.c_str();
      msg = os.str();
    } 
    else {
      msg = m;
    }
  }

  //***************************************************************************
    // FatalException
    //***************************************************************************

    FatalException::FatalException(const char* m,
				   const char* filenm, unsigned int lineno)
      : Exception(m, filenm, lineno) {
    }


  FatalException::FatalException(std::string m,
				 const char* filenm, unsigned int lineno)
    : Exception(m, filenm, lineno) {
  }


  FatalException::~FatalException() {
  }
  
}
