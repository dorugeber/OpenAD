#ifndef XAIF_SAXErrorHandler_INCLUDED_h
#define XAIF_SAXErrorHandler_INCLUDED_h

#include <iostream>

#include "xercesc/util/XercesDefs.hpp"
#include "xercesc/sax/ErrorHandler.hpp"

#include "XercesStrX.h"

namespace xaif2whirl { 

  class XAIF_SAXErrorHandler : public xercesc::ErrorHandler {
  public:
    XAIF_SAXErrorHandler() 
      : errors(false) { }
  
    ~XAIF_SAXErrorHandler() { }
  
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

} // end namespace xaif2whirl

#endif

