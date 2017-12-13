// -*-Mode: C++;-*-
// $Header: /m_home/m_utkej/Argonne/cvs2svn/cvs/OpenADFortTk/src/xaif2whirl/XAIF_DOMErrorHandler.h,v 1.2 2006-05-12 16:12:23 utke Exp $


#ifndef XAIF_DOMErrorHandler_INCLUDED_h
#define XAIF_DOMErrorHandler_INCLUDED_h


#include <iostream>

#include "xercesc/util/XercesDefs.hpp"
#include "xercesc/sax/ErrorHandler.hpp"

#include "XercesStrX.h"

namespace xaif2whirl { 

  class XAIF_DOMErrorHandler : public xercesc::ErrorHandler
  {
  public:
    XAIF_DOMErrorHandler() 
      : errors(false) { }
  
    ~XAIF_DOMErrorHandler() { }
  
    // -----------------------------------------------------------------------
    //  Implementation of the error handler interface
    // -----------------------------------------------------------------------
    void warning(const xercesc::SAXParseException& toCatch);
    void error(const xercesc::SAXParseException& toCatch);
    void fatalError(const xercesc::SAXParseException& toCatch);
    void resetErrors();

    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    bool getErrors() const { return errors; }
  
  private:

    bool errors; // Set if we find any errors

  };

}

#endif 
