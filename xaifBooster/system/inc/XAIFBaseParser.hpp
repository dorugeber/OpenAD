#ifndef _XAIFBASEPARSER_INCLUDE_
#define _XAIFBASEPARSER_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include  <xercesc/sax2/DefaultHandler.hpp>

#include "xaifBooster/utils/inc/XAIFBaseParserHelper.hpp"
#include "xaifBooster/system/inc/XMLParser.hpp"
#include "xaifBooster/system/inc/XAIFBaseParserHandlers.hpp"

namespace xaifBooster {

  class XAIFBaseParser :  public XERCES_CPP_NAMESPACE::DefaultHandler, public XMLParser, public XAIFBaseParserHandlers {
  public:
    void initialize(bool validateAgainstSchema);

    /**
     * UN: main handler routines for SAX2 parsing
     */
    void startDocument();
    void endDocument();
    void startElement(const XMLCh*, const XMLCh*, const XMLCh*, const XERCES_CPP_NAMESPACE::Attributes&);
    void endElement(const XMLCh*, const XMLCh*, const XMLCh*);

    virtual void setExternalSchemaLocation(const std::string& theSchemaLocation);

  private:

    const XERCES_CPP_NAMESPACE::Locator * myLocator_p;
    void setDocumentLocator(const XERCES_CPP_NAMESPACE::Locator* const locator); 
    /**
     * action held in ourActionCatalogue
     * keyed by node specs
     */
    struct ActionItem { 
      typedef 
      void (XAIFBaseParserHandlers::* ActionFPType)(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut);
      /** 
       * the actions are function pointers to XAIFBaseParserHandlers member 
       * functions with a signature as specified above
       */
      ActionFPType myAction;
      ActionItem(const ActionFPType& anAction): myAction(anAction){};
    }; 

    /**
     * the action catalogue for start elements 
     */
    static HashTable<ActionItem> ourStartActionCatalogue;
    
    /**
     * the action catalogue for end elements
     */
    static HashTable<ActionItem> ourEndActionCatalogue;
    
    /**
     * flag guarding the onetime initialization of
     * ourActionCatalogue
     */
    static bool ourStaticInitFlag;

    /**
     * method that contains calls which 
     * add individual actions to 
     * ourActionCatalogue
     */
    static void staticInitialize();

    /**
     * method used by startElement.
     * selects action method appropriate for node name
     * from ourStartActionCatalogue
     */
    void startActionInvocation(const XMLCh* const);

    /**
     * method used by endElement.
     * selects action method appropriate for node name
     * from ourEndActionCatalogue
     */
    void endActionInvocation(const XMLCh* const);

  }; // end of class XAIFBaseParser

} // end of namespace 

#endif
