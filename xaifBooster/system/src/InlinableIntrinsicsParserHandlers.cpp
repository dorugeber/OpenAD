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
#include "xaifBooster/system/inc/InlinableIntrinsicsParserHandlers.hpp"
#include "xaifBooster/system/inc/XMLParser.hpp"
#include <sstream>

namespace xaifBooster {
  
  void 
  InlinableIntrinsicsParserHandlers::onInlinableIntrinsicsCatalogue(const InlinableIntrinsicsParserHelper& passingIn,
								    InlinableIntrinsicsParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in InlinableIntrinsicsParserHandlers::onInlinableIntrinsicDefinitions");
    InlinableIntrinsicsCatalogue& theInlinableIntrinsicsCatalogue_r(ConceptuallyStaticInstances::instance()->getInlinableIntrinsicsCatalogue());
    passingOut.setInlinableIntrinsicsCatalogue(theInlinableIntrinsicsCatalogue_r);
  }

  void 
  InlinableIntrinsicsParserHandlers::onInlinableIntrinsicsCatalogueItem(const InlinableIntrinsicsParserHelper& passingIn,
									InlinableIntrinsicsParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in InlinableIntrinsicsParserHandlers::onInlinableIntrinsic");
    InlinableIntrinsicsCatalogue& theInlinableIntrinsicsCatalogue_r(passingIn.getInlinableIntrinsicsCatalogue());
    InlinableIntrinsicsCatalogueItem& theItem_r(theInlinableIntrinsicsCatalogue_r.
						addCatalogueItem(XMLParser::getAttributeValueByName(InlinableIntrinsicsCatalogueItem::our_myName_XAIFName), 
								 XMLParser::getAttributeValueByName(InlinableIntrinsicsCatalogueItem::our_myNrArgs_XAIFName),
								 XMLParser::convertToBoolean(XMLParser::getAttributeValueByName(InlinableIntrinsicsCatalogueItem::our_myNonSmoothFlag_XAIFName)),
								 ShapeChange::fromString(XMLParser::getAttributeValueByName(InlinableIntrinsicsCatalogueItem::our_myShapeChange_XAIFName))));
    passingOut.setInlinableIntrinsicsCatalogueItem(theItem_r);
  }

  void 
  InlinableIntrinsicsParserHandlers::onInlinableIntrinsicsFunction(const InlinableIntrinsicsParserHelper& passingIn,
								   InlinableIntrinsicsParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in InlinableIntrinsicsParserHandlers::onInlinableIntrinsicsFunction");
    InlinableIntrinsicsCatalogueItem& theItem_r(passingIn.getInlinableIntrinsicsCatalogueItem());
    InlinableIntrinsicsExpression& theFunction_r(theItem_r.getFunction());
    if (XMLParser::getAttributeValueByName(InlinableIntrinsicsCatalogueItem::our_myFunctionType_XAIFName)=="builtin") {
      theFunction_r.setBuiltinFunction();
      theFunction_r.setBuiltinFunctionName(XMLParser::getAttributeValueByName(InlinableIntrinsicsCatalogueItem::our_myFunctionBuiltinName_XAIFName) );
    }
    passingOut.setInlinableIntrinsicsExpression(theFunction_r);
  }

  void 
  InlinableIntrinsicsParserHandlers::onInlinableIntrinsicsPartial(const InlinableIntrinsicsParserHelper& passingIn,
								  InlinableIntrinsicsParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in InlinableIntrinsicsParserHandlers::onInlinableIntrinsicsPartial" ); 
    InlinableIntrinsicsCatalogueItem& theItem_r(passingIn.getInlinableIntrinsicsCatalogueItem());
    PartialDerivativeKind::PartialDerivativeKind_E pdk_e(PartialDerivativeKind::fromString(XMLParser::getAttributeValueByName(InlinableIntrinsicsCatalogueItem::our_myPartialType_XAIFName)));
    InlinableIntrinsicsExpression& thePartial_r(theItem_r.addPartial(StringConversions::convertToInt(XMLParser::getAttributeValueByName(InlinableIntrinsicsCatalogueItem::our_myPartialId_XAIFName)),
								     pdk_e));
    if (XMLParser::convertToBoolean(XMLParser::getAttributeValueByName(InlinableIntrinsicsCatalogueItem::our_myNonValueInquiryFlag_XAIFName)))
      thePartial_r.setNonValueInquiry();
    passingOut.setInlinableIntrinsicsExpression(thePartial_r);
  }

  void 
  InlinableIntrinsicsParserHandlers::onInlinableIntrinsicsArgumentReference(const InlinableIntrinsicsParserHelper& passingIn,
									    InlinableIntrinsicsParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in InlinableIntrinsicsParserHandlers::onArgumentReference" ); 
    InlinableIntrinsicsExpression& theExpression_r(passingIn.getInlinableIntrinsicsExpression());
    int argref(StringConversions::convertToInt(XMLParser::getAttributeValueByName(InlinableIntrinsicsArgumentReference::our_myArgRef_XAIFName)));
    InlinableIntrinsicsArgumentReference* aReference_p=new InlinableIntrinsicsArgumentReference(argref);
    aReference_p->setId(XMLParser::getAttributeValueByName(InlinableIntrinsicsArgumentReference::our_myId_XAIFName));
    theExpression_r.supplyAndAddVertexInstance(*aReference_p);
    theExpression_r.setPartialArgument(*aReference_p,argref);
  }

  void 
  InlinableIntrinsicsParserHandlers::onInlinableIntrinsicsConstant(const InlinableIntrinsicsParserHelper& passingIn,
								   InlinableIntrinsicsParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in InlinableIntrinsicsParserHandlers::onInlinableIntrinsicsConstant" ); 
    InlinableIntrinsicsExpression& theExpression_r(passingIn.getInlinableIntrinsicsExpression());
    InlinableIntrinsicsConstant* aConstant_p=new InlinableIntrinsicsConstant(
									     SymbolType::fromString(XMLParser::getAttributeValueByName(InlinableIntrinsicsConstant::our_myType_XAIFName)));
    aConstant_p->setId(XMLParser::getAttributeValueByName(InlinableIntrinsicsConstant::our_myId_XAIFName));
    aConstant_p->setFromString(
			       XMLParser::getAttributeValueByName(InlinableIntrinsicsConstant::our_myValue_XAIFName));
    theExpression_r.supplyAndAddVertexInstance(*aConstant_p);
  }

  void 
  InlinableIntrinsicsParserHandlers::onInlinableIntrinsicsIntrinsic(const InlinableIntrinsicsParserHelper& passingIn,
								    InlinableIntrinsicsParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in InlinableIntrinsicsParserHandlers::onInlinableIntrinsicsIntrinsic" ); 
    InlinableIntrinsicsExpression& theExpression_r(passingIn.getInlinableIntrinsicsExpression());
    InlinableIntrinsicsIntrinsic* aIntrinsic_p=new InlinableIntrinsicsIntrinsic(
										XMLParser::getAttributeValueByName(InlinableIntrinsicsIntrinsic::our_myName_XAIFName));
    aIntrinsic_p->setId(XMLParser::getAttributeValueByName(InlinableIntrinsicsConstant::our_myId_XAIFName));
    theExpression_r.supplyAndAddVertexInstance(*aIntrinsic_p);
  }

  void 
  InlinableIntrinsicsParserHandlers::onInlinableIntrinsicsExpressionEdge(const InlinableIntrinsicsParserHelper& passingIn,
									 InlinableIntrinsicsParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in InlinableIntrinsicsParserHandlers::onInlinableIntrinsicsExpressionEdge" ); 
    InlinableIntrinsicsExpression& theExpression_r(passingIn.getInlinableIntrinsicsExpression());
    std::string sourceId=XMLParser::getAttributeValueByName(
							    InlinableIntrinsicsExpressionEdge::our_source_XAIFName);
    std::string targetId=XMLParser::getAttributeValueByName(
							    InlinableIntrinsicsExpressionEdge::our_target_XAIFName);
    InlinableIntrinsicsExpression::VertexIteratorPair p=theExpression_r.vertices();
    InlinableIntrinsicsExpression::VertexIterator beginIt(p.first),endIt(p.second);
    InlinableIntrinsicsExpressionVertex *theSource_p(0), *theTarget_p(0);
    for (; (beginIt!=endIt)
	   && !(theSource_p && theTarget_p)
	   ;++beginIt) { 
      if (!theSource_p && sourceId==(*beginIt).getId())
	theSource_p=&(*beginIt);
      if (!theTarget_p && targetId==(*beginIt).getId())
	theTarget_p=&(*beginIt);
    }
    if (!theTarget_p || 
 	!theSource_p)
      THROW_LOGICEXCEPTION_MACRO("XAIFInlinableIntrinsicsParser::onInlinableIntrinsicsExpressionEdge: can't find source  " 
				 << sourceId.c_str()
				 << " or target "
				 << targetId.c_str());
    InlinableIntrinsicsExpressionEdge& theInlinableIntrinsicsExpressionEdge(theExpression_r.addEdge(*theSource_p, *theTarget_p));
    theInlinableIntrinsicsExpressionEdge.setId(XMLParser::getAttributeValueByName(InlinableIntrinsicsExpressionEdge::our_myId_XAIFName));
    theInlinableIntrinsicsExpressionEdge.setPosition(XMLParser::getAttributeValueByName(InlinableIntrinsicsExpressionEdge::our_myPosition_XAIFName));
  }

  void 
  InlinableIntrinsicsParserHandlers::onDummy(const InlinableIntrinsicsParserHelper& passingIn,
					     InlinableIntrinsicsParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK,
	      "InlinableIntrinsicsParserHandlers::onDummy");
    // pass all set elements through
    passingOut=passingIn;
    return;
  }

} // end of namespace 

