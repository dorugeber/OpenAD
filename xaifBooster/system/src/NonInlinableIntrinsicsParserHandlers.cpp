// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/utils/inc/XMLParserMessage.hpp"
#include "xaifBooster/utils/inc/StringConversions.hpp"
#include "xaifBooster/system/inc/NonInlinableIntrinsicsParserHandlers.hpp"
#include "xaifBooster/system/inc/XMLParser.hpp"
#include <sstream>

namespace xaifBooster {
  
  void 
  NonInlinableIntrinsicsParserHandlers::onNonInlinableIntrinsicsCatalogue(const NonInlinableIntrinsicsParserHelper& passingIn,
									  NonInlinableIntrinsicsParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in NonInlinableIntrinsicsParserHandlers::onNonInlinableIntrinsicDefinitions");
    NonInlinableIntrinsicsCatalogue& theNonInlinableIntrinsicsCatalogue_r(ConceptuallyStaticInstances::instance()->getNonInlinableIntrinsicsCatalogue());
    passingOut.setNonInlinableIntrinsicsCatalogue(theNonInlinableIntrinsicsCatalogue_r);
  }

  void 
  NonInlinableIntrinsicsParserHandlers::onExplicitJacobian(const NonInlinableIntrinsicsParserHelper& passingIn,
							   NonInlinableIntrinsicsParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in NonInlinableIntrinsicsParserHandlers::onExplicitJacobian");
    NonInlinableIntrinsicsCatalogue& theNonInlinableIntrinsicsCatalogue_r(passingIn.getNonInlinableIntrinsicsCatalogue());
    NonInlinableIntrinsicsCatalogueItem& theItem_r(theNonInlinableIntrinsicsCatalogue_r.
						   addCatalogueItem(XMLParser::getAttributeValueByName(NonInlinableIntrinsicsCatalogueItem::our_myName_XAIFName), 
								 XMLParser::convertToBoolean(XMLParser::getAttributeValueByName(NonInlinableIntrinsicsCatalogueItem::our_myNonSmoothFlag_XAIFName))));
    ExplicitJacobian & theJacobian=theItem_r.makeExplicitJacobian();
    passingOut.setExplicitJacobian(theJacobian);
  }

  void 
  NonInlinableIntrinsicsParserHandlers::onPartial(const NonInlinableIntrinsicsParserHelper& passingIn,
						  NonInlinableIntrinsicsParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in NonInlinableIntrinsicsParserHandlers::onPartial");
    ExplicitJacobian& theJacobian(passingIn.getExplicitJacobian());
    ExplicitJacobianPartial& thePartial=theJacobian.addExplicitJacobianPartial();
    thePartial.setInfo(StringConversions::convertToUInt(XMLParser::getAttributeValueByName(ExplicitJacobianPartial::our_myDep_XAIFName)),
		       StringConversions::convertToUInt(XMLParser::getAttributeValueByName(ExplicitJacobianPartial::our_myInDep_XAIFName)),
		       StringConversions::convertToUInt(XMLParser::getAttributeValueByName(ExplicitJacobianPartial::our_myPartial_XAIFName)));
  }

  void 
  NonInlinableIntrinsicsParserHandlers::onDirectAction(const NonInlinableIntrinsicsParserHelper& passingIn,
						       NonInlinableIntrinsicsParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in NonInlinableIntrinsicsParserHandlers::onDirectAction");
    NonInlinableIntrinsicsCatalogue& theNonInlinableIntrinsicsCatalogue_r(passingIn.getNonInlinableIntrinsicsCatalogue());
    NonInlinableIntrinsicsCatalogueItem& theItem_r(theNonInlinableIntrinsicsCatalogue_r.
						   addCatalogueItem(XMLParser::getAttributeValueByName(NonInlinableIntrinsicsCatalogueItem::our_myName_XAIFName), 
								    XMLParser::convertToBoolean(XMLParser::getAttributeValueByName(NonInlinableIntrinsicsCatalogueItem::our_myNonSmoothFlag_XAIFName))));
    DirectAction & theDirectAction=theItem_r.makeDirectAction();
    theDirectAction.setDerivAction(XMLParser::getAttributeValueByName(DirectAction::our_myDerivAction_XAIFName));
  }

  void 
  NonInlinableIntrinsicsParserHandlers::onDummy(
				     const NonInlinableIntrinsicsParserHelper& passingIn,
				     NonInlinableIntrinsicsParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK,
	      "NonInlinableIntrinsicsParserHandlers::onDummy");
    // pass all set elements through
    passingOut=passingIn;
    return;
  }


} // end of namespace 

