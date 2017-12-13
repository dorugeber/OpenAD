#include <iostream>

#include <stdlib.h>
#include <string.h>

#include "xercesc/sax/SAXParseException.hpp"

#include "XAIF_SAXErrorHandler.h"

namespace xaif2whirl { 

  void XAIF_SAXErrorHandler::warning(const xercesc::SAXParseException&) {
    // Ignore all warnings.
  }

  void XAIF_SAXErrorHandler::error(const xercesc::SAXParseException& toCatch) {
    errors = true;
    std::cerr << "Error at file \"" 
	      << XercesStrX(toCatch.getSystemId())
	      << "\", line " << toCatch.getLineNumber()
	      << ", column " << toCatch.getColumnNumber()
	      << std::endl 
	      << "   Message: " << XercesStrX(toCatch.getMessage()) 
	      << std::endl;
  }

  void XAIF_SAXErrorHandler::fatalError(const xercesc::SAXParseException& toCatch) {
    errors = true;
    std::cerr << "Fatal Error at file \"" << XercesStrX(toCatch.getSystemId())
	      << "\", line " << toCatch.getLineNumber()
	      << ", column " << toCatch.getColumnNumber()
	      << std::endl
	      << "   Message: " << XercesStrX(toCatch.getMessage()) 
	      << std::endl;
  }

  void XAIF_SAXErrorHandler::resetErrors() {
    errors = false;
  }

} // end namespace xaif2whirl

