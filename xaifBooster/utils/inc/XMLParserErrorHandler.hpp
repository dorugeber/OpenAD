#ifndef _XMLPARSERERRORHANDLER_INCLUDE_
#define _XMLPARSERERRORHANDLER_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xercesc/util/XercesDefs.hpp"
#include "xercesc/sax/ErrorHandler.hpp"

namespace xaifBooster { 

  /** 
   * an error handler class required by xerces
   */
  class XMLParserErrorHandler : public xercesc::ErrorHandler {

  public:

    XMLParserErrorHandler();

    ~XMLParserErrorHandler();

    /** 
     * pure virtual method declared in ErrorHandler to be defined 
     * for now we ignore all warnings
     */
    void warning(const xercesc::SAXParseException& e);

    /** 
     * pure virtual method declared in ErrorHandler to be defined 
     * we throw a LogicException
     */
    void error(const xercesc::SAXParseException& e);

    /** 
     * pure virtual method declared in ErrorHandler to be defined 
     * no distinction, same treatment as error
     */
    void fatalError(const xercesc::SAXParseException& e);

    /** 
     * pure virtual method declared in ErrorHandler to be defined 
     */
    void resetErrors(){};

  private :

    /**
     * disallowed (no implementation)
     */
    XMLParserErrorHandler(const XMLParserErrorHandler&);

    /**
     * disallowed (no implementation)
     */
     XMLParserErrorHandler& operator=(const XMLParserErrorHandler&);

  }; // end of class XMLParserErrorHandler

}
#endif
