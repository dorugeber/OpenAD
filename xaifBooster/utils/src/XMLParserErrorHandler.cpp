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
#include "xercesc/sax/SAXParseException.hpp"
#include "xaifBooster/utils/inc/XMLParserErrorHandler.hpp"
#include "xaifBooster/utils/inc/XMLParserMessage.hpp"
#include "xaifBooster/utils/inc/LogicException.hpp"

namespace xaifBooster { 

  XMLParserErrorHandler::XMLParserErrorHandler() {}

  XMLParserErrorHandler::~XMLParserErrorHandler() {}

  void 
  XMLParserErrorHandler::warning(const xercesc::SAXParseException&) {
  } 

  void 
  XMLParserErrorHandler::error(const xercesc::SAXParseException& e) {
    std::ostringstream err;
    err << "XMLParser error: at file \""
     	<< XMLParserMessage(e.getSystemId())
	<< "\", line "
	<< e.getLineNumber()
	<< ", column " 
	<< e.getColumnNumber()
	<< " Message: " 
      	<< XMLParserMessage(e.getMessage()) 
	<< std::ends;
    THROW_LOGICEXCEPTION_MACRO(err.str());
  }

  void 
  XMLParserErrorHandler::fatalError(const xercesc::SAXParseException& e) {
    error(e);
  }

}
