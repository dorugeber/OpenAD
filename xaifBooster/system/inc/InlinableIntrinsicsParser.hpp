#ifndef _INLINABLEINTRINSICSPARSER_INCLUDE_
#define _INLINABLEINTRINSICSPARSER_INCLUDE_
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
                                                           
#include "xaifBooster/system/inc/XMLParser.hpp"
#include "xaifBooster/system/inc/InlinableIntrinsicsParserHandlers.hpp"
#include "xaifBooster/utils/inc/InlinableIntrinsicsParserHelper.hpp"

namespace xaifBooster {

  class InlinableIntrinsicsParser : public XERCES_CPP_NAMESPACE::DefaultHandler, public XMLParser, public InlinableIntrinsicsParserHandlers {
  public:
    InlinableIntrinsicsParser(InlinableIntrinsicsCatalogue& theInlinableIntrinsicsCatalogue_r);

    void initialize();

    void startDocument();
    void endDocument();
    void startElement(const XMLCh*, const XMLCh*, const XMLCh*, const XERCES_CPP_NAMESPACE::Attributes&);
    void endElement(const XMLCh*, const XMLCh*, const XMLCh*);
 
    virtual void setExternalSchemaLocation(const std::string& theSchemaLocation);

  private:

    InlinableIntrinsicsCatalogue& myInlinableIntrinsicsCatalogue;

    struct ActionItem { 
      typedef 
      void (InlinableIntrinsicsParser::* ActionFPType)(
				       const InlinableIntrinsicsParserHelper& passingIn,
				       InlinableIntrinsicsParserHelper& passingOut);
      ActionFPType myAction;
      ActionItem(const ActionFPType& anAction): myAction(anAction){};
    }; 

    /**
     * the action catalogue 
     * keyed by the DOM node names as specified 
     * in the XAIF schema
     */
    static HashTable<ActionItem> ourActionCatalogue;
    
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
     * method used by * startElement.
     * selects action method appropriate for node name
     * from ourActionCatalogue
     */
    void actionInvocation(const XMLCh* const);

  }; // end of class InlinableIntrinsicsParser

} // end of namespace 

#endif
