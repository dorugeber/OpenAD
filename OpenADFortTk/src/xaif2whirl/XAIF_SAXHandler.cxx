#include <iostream>

#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>

#include "XAIF_SAXHandler.h"
#include "XAIF_SAXErrorHandler.h"
#include "XAIFStrings.h"
#include "XAIF_DOMFilters.h"
#include "PUXlationContext.h"
#include "xaif2whirl.h"

#include "Open64IRInterface/IFDiagnostics.h"

namespace xaif2whirl { 

  XAIF_SAXHandler::XAIF_SAXHandler(PU_Info* pu_forest, const XMLCh* implementationFeatures) :
	myPUForest_p (pu_forest),
	myPUXlationContext ("TranslateIR", pu_forest),
	myParser_p (xercesc::XMLReaderFactory::createXMLReader()),
	myDOMImplementation_p (xercesc::DOMImplementationRegistry::getDOMImplementation(implementationFeatures)),
	myDOMDocument_p (NULL),
	inDOMMode (false) {
  }

  XAIF_SAXHandler::~XAIF_SAXHandler() {
  }

  void XAIF_SAXHandler::initialize(bool validateAgainstSchema) {

    if (validateAgainstSchema)
      FORTTK_MSG(1, "progress: parsing with schema validation");

    // set features for SAX2 parser (copied from xaifBooster - XMLParser.cpp)
    myParser_p->setFeature(xercesc::XMLUni::fgSAX2CoreValidation,validateAgainstSchema);
    myParser_p->setFeature(xercesc::XMLUni::fgXercesDynamic,false);
    // we need the following for schema processing:
    myParser_p->setFeature(xercesc::XMLUni::fgSAX2CoreNameSpaces,true);
    myParser_p->setFeature(xercesc::XMLUni::fgSAX2CoreNameSpacePrefixes,true);
    // we need the following to support default values coming from the schema:
    myParser_p->setFeature(xercesc::XMLUni::fgXercesSchema,true);
    myParser_p->setFeature(xercesc::XMLUni::fgXercesSchemaFullChecking,validateAgainstSchema);

    // set content and error handlers
    myParser_p->setContentHandler(this);
    static XAIF_SAXErrorHandler anErrorHandler;
    myParser_p->setErrorHandler(&anErrorHandler);

    Diag_Set_Phase("XAIF to WHIRL: translate IR");

/* some stuff from TranslateIR()
    //IntrinsicTable.DDump();
    if (!pu_forest) { return; }
*/
    WNIdToWNTableMap.Create(myPUForest_p); // Note: could make this local

    DeclareActiveTypes(); // sets a static variable (global) in PUXlation...

  } // end XAIF_SAXHandler::initialize()

  void XAIF_SAXHandler::parse(std::string theXMLFileName) {
    myParser_p->parse(theXMLFileName.c_str());
  } // end XAIF_SAXHandler::parse()

  bool XAIF_SAXHandler::getParserErrorStatus() { 
    return (dynamic_cast<const XAIF_SAXErrorHandler*>(myParser_p->getErrorHandler()))->getErrors();
  } 

  void XAIF_SAXHandler::startElement(const XMLCh* const uri,
				     const XMLCh* const localname,
				     const XMLCh* const qname,
				     const xercesc::Attributes& attrs) {
    if (inDOMMode) {
      if (XMLString::equals(qname, XAIFStrings.elem_CFG_x())) {
	FORTTK_DIE("start of CFG element while already in DOM mode");
      }
      // create new DOMElement, set its attributes, and add it to the document tree and the stack
      xercesc::DOMElement* theNewDOMElement = myDOMDocument_p->createElement(qname);
      copyAttributes(theNewDOMElement, attrs);
      myElementStack.top()->appendChild(theNewDOMElement);
      myElementStack.push(theNewDOMElement);
    } // end DOM mode

    else { // SAX mode
      // start of callgraph element: extract the prefix attribute
      if (XMLString::equals(qname, XAIFStrings.elem_CallGraph_x())) {
	const XMLCh* prefixX = attrs.getValue(XAIFStrings.attr_prefix_x());
	PUXlationContext::setPrefix(XercesStrX(prefixX).c_str());
      }

      // start of ScopeHierarchy or CFG element: switch to DOM mode
      else if (XMLString::equals(qname, XAIFStrings.elem_ScopeHierarchy_x())
	       || 
	       XMLString::equals(qname, XAIFStrings.elem_CFG_x())
	       ||
	       XMLString::equals(qname, XAIFStrings.elem_ReplaceList_x())) {
	inDOMMode = true;
	// create new DOMDocument with root element
	myDOMDocument_p = myDOMImplementation_p->createDocument(uri,	// root element namespace URI.
								//0,	// root element namespace URI.
								qname,	// root element name
								0);	// document type object (DTD).
	// set attributes for the root element of the new document and push it to the stack
	copyAttributes(myDOMDocument_p->getDocumentElement(), attrs);
	myElementStack.push(myDOMDocument_p->getDocumentElement());
      }
    } // end SAX mode

  } // end XAIF_SAXHandler::startElement()

  void XAIF_SAXHandler::endElement(const XMLCh* const uri,
				   const XMLCh* const localname,
				   const XMLCh* const qname) {
    if (inDOMMode) {
      myElementStack.pop(); //remove the element from the stack

      // end of a CFG / ReplacementList element
      if (XMLString::equals(qname, XAIFStrings.elem_CFG_x())
	  ||
	  XMLString::equals(qname, XAIFStrings.elem_ReplaceList_x())) {
	if (!myElementStack.empty()) FORTTK_DIE("stack not empty after popping CFG element");
	inDOMMode = false;
        // translate the DOM subtree for the CFG
	TranslateCFG (myPUForest_p, myDOMDocument_p->getDocumentElement(), myPUXlationContext);
	delete myDOMDocument_p;
      } // end CFG element

      // end of a ScopeHierarchy element
      else if (XMLString::equals(qname, XAIFStrings.elem_ScopeHierarchy_x())) {
	if (!myElementStack.empty()) FORTTK_DIE("stack not empty after popping ScopeHierarchy element");
        inDOMMode = false;
	// populate the symbol map for myPUXlationContext
	XAIF_ScopeElemFilter filt;
	for (xercesc::DOMElement* elem = GetChildElement(myDOMDocument_p->getDocumentElement(), &filt); (elem); elem = GetNextSiblingElement(elem, &filt))
	  xlate_Scope(elem, myPUXlationContext);
	delete myDOMDocument_p;
      } // end ScopeHierarchy element

    } // end DOM mode
    else { // in SAX mode
      if (XMLString::equals(qname, XAIFStrings.elem_CFG_x()) || XMLString::equals(qname, XAIFStrings.elem_ScopeHierarchy_x()))
	FORTTK_DIE("end of CFG reached while not in DOM mode");
    } // end SAX mode

  } // end XAIF_SAXHandler::endElement()

  void XAIF_SAXHandler::deleteParser() {
    delete myParser_p;
  }

  void XAIF_SAXHandler::copyAttributes(xercesc::DOMElement* theElement, const xercesc::Attributes& theAttributes) {
    unsigned int n = theAttributes.getLength();
    for (int i = 0; i < n; i++)
      theElement->setAttributeNS(theAttributes.getURI(i),
				 theAttributes.getQName(i),
				 theAttributes.getValue(i));
  } // end XAIF_SAXHandler::copyAttributes()

} // end namespace xaif2whirl

