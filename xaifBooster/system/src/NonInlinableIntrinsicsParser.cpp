// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/system/inc/NonInlinableIntrinsicsParser.hpp"
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/TransService.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include "xaifBooster/utils/inc/XMLParserErrorHandler.hpp"
#include "xaifBooster/utils/inc/XMLParserMessage.hpp"
#include "xaifBooster/utils/inc/LogicException.hpp"
#include "xaifBooster/utils/inc/DbgLoggerManager.hpp"
#include <sstream>

namespace xaifBooster {
  
  HashTable<NonInlinableIntrinsicsParser::ActionItem> NonInlinableIntrinsicsParser::ourActionCatalogue;
  bool NonInlinableIntrinsicsParser::ourStaticInitFlag;

  NonInlinableIntrinsicsParser::NonInlinableIntrinsicsParser(NonInlinableIntrinsicsCatalogue& theNonInlinableIntrinsicsCatalogue_r) : 
    myNonInlinableIntrinsicsCatalogue(theNonInlinableIntrinsicsCatalogue_r){
  }
  
  void NonInlinableIntrinsicsParser::setExternalSchemaLocation(const std::string& theSchemaLocation) { 
    if (theSchemaLocation.size()) { 
      std::string aUrl("http://www.mcs.anl.gov/XAIFNonInlinableIntrinsics ");
      XMLParser::setExternalSchemaLocation(aUrl+theSchemaLocation+"/xaif_NonInlinable_intrinsics.xsd");
    } 
  } 
  
  void NonInlinableIntrinsicsParser::initialize() { 
    try { 

      if (!ourStaticInitFlag)
	staticInitialize();
      XMLParser::initialize(true);
      myParser_p->setContentHandler(this);
      static XMLParserErrorHandler anErrorHandler;
      myParser_p->setErrorHandler(&anErrorHandler);

    } catch (const xercesc::XMLException& e) {
      THROW_LOGICEXCEPTION_MACRO("NonInlinableIntrinsicsParser::initialize: XMLException: "
				 << XMLParserMessage(e.getMessage()));
    }
  } // end of NonInlinableIntrinsicsParser::initialize

  void 
  NonInlinableIntrinsicsParser::staticInitialize() { 
    if (ourStaticInitFlag)
      THROW_LOGICEXCEPTION_MACRO("NonInlinableIntrinsicsParser::staticInitialize: already initialized");
    ourStaticInitFlag=true;
    ourActionCatalogue.addElement(NonInlinableIntrinsicsCatalogue::ourXAIFName,
				  ActionItem(&NonInlinableIntrinsicsParser::onNonInlinableIntrinsicsCatalogue));
    ourActionCatalogue.addElement(ExplicitJacobian::ourXAIFName,
				  ActionItem(&NonInlinableIntrinsicsParser::onExplicitJacobian));
    ourActionCatalogue.addElement(ExplicitJacobianPartial::ourXAIFName,
				  ActionItem(&NonInlinableIntrinsicsParser::onPartial));
    ourActionCatalogue.addElement(DirectAction::ourXAIFName,
				  ActionItem(&NonInlinableIntrinsicsParser::onDirectAction));
    ourActionCatalogue.addElement("#comment",
				  ActionItem(&NonInlinableIntrinsicsParser::onDummy));
    ourActionCatalogue.addElement("#document",
				  ActionItem(&NonInlinableIntrinsicsParser::onDummy));
    ourActionCatalogue.addElement("#text",
				  ActionItem(&NonInlinableIntrinsicsParser::onDummy));
  } // end of NonInlinableIntrinsicsParser::staticInitialize

  /*
   * UN: Implementation of startElement handler
   */

  void NonInlinableIntrinsicsParser::startElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname, const XERCES_CPP_NAMESPACE::Attributes& attributes) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in NonInlinableIntrinsicsParser::startElement" );
    XMLParser::ourAttributes_p=&attributes;
    actionInvocation(qname); 
  }

  /*
   * UN: Implementation of endElement handler
   */

  void NonInlinableIntrinsicsParser::endElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in NonInlinableIntrinsicsParser::endElement" );
    XMLParserHelper* myDummyHelper=myXMLParserHelperStack.top();
    myXMLParserHelperStack.pop();
    delete myDummyHelper;
  }

  /*
   * UN: Implementation of startDocument handler
   */

  void NonInlinableIntrinsicsParser::startDocument() {
    DBG_MACRO(DbgGroup::CALLSTACK, "in NonInlinableIntrinsicsParser::startDocument" );
    NonInlinableIntrinsicsParserHelper* myDummyHelper=new NonInlinableIntrinsicsParserHelper;
    myXMLParserHelperStack.push(myDummyHelper);
  }

  /*
   * UN: Implementation of endDocument handler
   */

  void NonInlinableIntrinsicsParser::endDocument() {
    DBG_MACRO(DbgGroup::CALLSTACK, "in NonInlinableIntrinsicsParser::endDocument" );
    XMLParserHelper* myDummyHelper=myXMLParserHelperStack.top();
    myXMLParserHelperStack.pop();
    delete myDummyHelper;
  }

    /*
   * UN: takes only the name of the current element and
   * calls the appropriate handler routine
   */
                                                           
  void NonInlinableIntrinsicsParser::actionInvocation(const XMLCh* const
qname) {
    NonInlinableIntrinsicsParserHelper* passingFromParent=dynamic_cast<NonInlinableIntrinsicsParserHelper*>(myXMLParserHelperStack.top());
    NonInlinableIntrinsicsParserHelper* passingToChild=new NonInlinableIntrinsicsParserHelper;
    myXMLParserHelperStack.push(passingToChild);
    try {
      ActionItem& theAction=ourActionCatalogue.getElement(XMLParserMessage(qname).toString());
      (this->*(theAction.myAction))(*passingFromParent,
                                    *passingToChild) ;
    }
    catch (LogicException& e) {
      DBG_MACRO(DbgGroup::ERROR,
                "XMLParser::actionInvocation: caught: "
                << e.getReason().c_str());
      throw e;
    }
  } // end of NonInlinableIntrinsicsParser::actionInvocation

} // end of namespace 

