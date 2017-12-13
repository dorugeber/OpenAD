// -*-Mode: C++;-*-
// $Header: /m_home/m_utkej/Argonne/cvs2svn/cvs/OpenADFortTk/src/xaif2whirl/XAIF_DOMErrorHandler.cxx,v 1.2 2006-05-12 16:12:23 utke Exp $

#include <iostream>

#include <stdlib.h>
#include <string.h>

#include "xercesc/sax/SAXParseException.hpp"

#include "XAIF_DOMErrorHandler.h"

namespace xaif2whirl { 

  void XAIF_DOMErrorHandler::warning(const xercesc::SAXParseException&) {
    // Ignore all warnings.
  }

  void XAIF_DOMErrorHandler::error(const xercesc::SAXParseException& toCatch) {
    errors = true;
    std::cerr << "Error at file \"" 
	      << XercesStrX(toCatch.getSystemId())
	      << "\", line " << toCatch.getLineNumber()
	      << ", column " << toCatch.getColumnNumber()
	      << std::endl 
	      << "   Message: " << XercesStrX(toCatch.getMessage()) 
	      << std::endl;
  }

  void XAIF_DOMErrorHandler::fatalError(const xercesc::SAXParseException& toCatch) {
    errors = true;
    std::cerr << "Fatal Error at file \"" << XercesStrX(toCatch.getSystemId())
	      << "\", line " << toCatch.getLineNumber()
	      << ", column " << toCatch.getColumnNumber()
	      << std::endl
	      << "   Message: " << XercesStrX(toCatch.getMessage()) 
	      << std::endl;
  }

  void XAIF_DOMErrorHandler::resetErrors() {
    errors = false;
  }

}
