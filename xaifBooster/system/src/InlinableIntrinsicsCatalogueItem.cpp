// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/system/inc/InlinableIntrinsicsCatalogueItem.hpp"

namespace xaifBooster { 


  const std::string InlinableIntrinsicsCatalogueItem::ourXAIFName("xaifii:InlinableIntrinsic");
  const std::string InlinableIntrinsicsCatalogueItem::our_myName_XAIFName("name");
  const std::string InlinableIntrinsicsCatalogueItem::our_myNrArgs_XAIFName("nr_arguments");
  const std::string InlinableIntrinsicsCatalogueItem::our_myNonSmoothFlag_XAIFName("nonSmooth");
  const std::string InlinableIntrinsicsCatalogueItem::ourFunctionXAIFName("xaifii:Function");
  const std::string InlinableIntrinsicsCatalogueItem::our_myFunctionType_XAIFName("type");
  const std::string InlinableIntrinsicsCatalogueItem::our_myFunctionBuiltinName_XAIFName("builtin_name");
  const std::string InlinableIntrinsicsCatalogueItem::ourPartialXAIFName("xaifii:Partial");
  const std::string InlinableIntrinsicsCatalogueItem::our_myPartialId_XAIFName("partial_id");
  const std::string InlinableIntrinsicsCatalogueItem::our_myPartialType_XAIFName("partial_type");
  const std::string InlinableIntrinsicsCatalogueItem::our_myNonValueInquiryFlag_XAIFName("nonValueInquiry");
  const std::string InlinableIntrinsicsCatalogueItem::our_myShapeChange_XAIFName("shapeChange");

  InlinableIntrinsicsCatalogueItem::InlinableIntrinsicsCatalogueItem(unsigned int aNumberOfArguments,
								     bool aNonSmoothFlag,
								     ShapeChange::ShapeChange_E theShapeChange) :
    myNumberOfArguments(aNumberOfArguments),
    myExpressionVector(aNumberOfArguments+1),
    myDataComplete(false),
    myNonSmoothFlag(aNonSmoothFlag),
    myShapeChange(theShapeChange) { 
    InlinableIntrinsicsExpression* theFunction_p=new InlinableIntrinsicsExpression(myNumberOfArguments,
										   PartialDerivativeKind::PASSIVE); // this is for the function itself
    myExpressionVector[0]=theFunction_p;
    for (unsigned int i=1;i<aNumberOfArguments+1;i++)
      myExpressionVector[i]=0;
  } 

  InlinableIntrinsicsCatalogueItem::~InlinableIntrinsicsCatalogueItem() { 
    for (ExpressionVector::iterator it=myExpressionVector.begin();
	 it!=myExpressionVector.end();
	 it++)
      if ((*it)) // should always be true
	delete (*it);
  } 

  InlinableIntrinsicsExpression&
  InlinableIntrinsicsCatalogueItem::addPartial(unsigned int aPosition,
					       PartialDerivativeKind::PartialDerivativeKind_E aPartialDerivativeKind) { 
    if (myDataComplete)
      THROW_LOGICEXCEPTION_MACRO("InlinableIntrinsicsCatalogueItem::addPartial: cannot add partials after list was assumed to be complete");
    if (myExpressionVector[aPosition])
      THROW_LOGICEXCEPTION_MACRO("InlinableIntrinsicsCatalogueItem::addPartial: position already set");
    if (aPosition<1 || aPosition>myNumberOfArguments)
      THROW_LOGICEXCEPTION_MACRO("InlinableIntrinsicsCatalogueItem::addPartial: position out of range");
    InlinableIntrinsicsExpression* theExpression_p=new InlinableIntrinsicsExpression(myNumberOfArguments, aPartialDerivativeKind);
    myExpressionVector[aPosition]=theExpression_p;
    return *theExpression_p;
  } 
  
  bool
  InlinableIntrinsicsCatalogueItem::isExpressionVectorElementSet(unsigned int aPosition) const { 
    if (aPosition>myNumberOfArguments)
      THROW_LOGICEXCEPTION_MACRO("InlinableIntrinsicsCatalogueItem::isExpressionVectorElementSet: position out of range");
    return (myExpressionVector[aPosition]!=0);
  } 

  const InlinableIntrinsicsExpression& 
  InlinableIntrinsicsCatalogueItem::getExpressionVectorElement(unsigned int aPosition) const { 
    if (!isExpressionVectorElementSet(aPosition))
      THROW_LOGICEXCEPTION_MACRO("InlinableIntrinsicsCatalogueItem::getExpressionVectorElement: position not set");
    return *(myExpressionVector[aPosition]);
  } 

  InlinableIntrinsicsExpression& 
  InlinableIntrinsicsCatalogueItem::getFunction() { 
    if (!isExpressionVectorElementSet(0))
      THROW_LOGICEXCEPTION_MACRO("InlinableIntrinsicsCatalogueItem::getFunction: position 0 in ExpressionVector not set");
    return *(myExpressionVector[0]);
  } 

  const InlinableIntrinsicsExpression& 
  InlinableIntrinsicsCatalogueItem::getFunction() const { 
    if (!isExpressionVectorElementSet(0))
      THROW_LOGICEXCEPTION_MACRO("InlinableIntrinsicsCatalogueItem::getFunction: position 0 in ExpressionVector not set");
    return *(myExpressionVector[0]);
  } 

  const PositionSet&
  InlinableIntrinsicsCatalogueItem::getUsedPositionalArguments(const PositionSet&) const { 
    if (!myDataComplete) { 
      for (ExpressionVector::const_iterator it=myExpressionVector.begin();
	   it!=myExpressionVector.end();
	   it++)
	// this is really conceptually constant
	const_cast<InlinableIntrinsicsCatalogueItem*>(this)->myUsedPositionalArguments.add((*it)->getUsedPositionalArguments());
      // this is really conceptually constant
      const_cast<InlinableIntrinsicsCatalogueItem*>(this)->myDataComplete=true;
    }
    return myUsedPositionalArguments; 
  } 

  bool 
  InlinableIntrinsicsCatalogueItem::isNonSmooth() const { 
    return myNonSmoothFlag;
  }

  ShapeChange::ShapeChange_E 
  InlinableIntrinsicsCatalogueItem::getShapeChange() const { 
    return myShapeChange;
  }

} 
