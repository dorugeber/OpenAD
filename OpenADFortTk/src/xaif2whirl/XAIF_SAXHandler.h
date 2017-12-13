#ifndef XAIF_SAXhandler_INCLUDED_h
#define XAIF_SAXHandler_INCLUDED_h

#include <stack>

#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/util/PlatformUtils.hpp>

#include <xercesc/dom/DOM.hpp>

#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>

#include "PUXlationContext.h"

namespace xaif2whirl {

  class XAIF_SAXHandler : public xercesc::DefaultHandler {

  public:

    XAIF_SAXHandler(PU_Info* pu_forest, const XMLCh* implementationFeatures);
    ~XAIF_SAXHandler();

    void initialize(bool validateAgainstSchema);

    void parse(std::string theXMLFileName);

    void startElement(const XMLCh* const uri,
		      const XMLCh* const localname,
		      const XMLCh* const qname,
		      const xercesc::Attributes& attrs);

    void endElement(const XMLCh* const uri,
		    const XMLCh* const localname,
		    const XMLCh* const qname);

    bool getParserErrorStatus(); 

    void deleteParser();

  private:

    /**
     * no def
     */
    XAIF_SAXHandler();

    /**
     * no def
     */
    XAIF_SAXHandler& operator=(const XAIF_SAXHandler& x);

    PU_Info* myPUForest_p;

    PUXlationContext myPUXlationContext;

    xercesc::SAX2XMLReader* myParser_p;

    xercesc::DOMImplementation* myDOMImplementation_p;

    xercesc::DOMDocument* myDOMDocument_p;

    std::stack<xercesc::DOMElement*> myElementStack;

    bool inDOMMode;

    void copyAttributes(xercesc::DOMElement* theElement, const xercesc::Attributes& theAttributes);

  }; // end class XAIF_SAXHandler

} // end namespace xaif2whirl

#endif 
