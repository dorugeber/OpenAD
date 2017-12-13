// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/system/inc/InlinableIntrinsicsExpression.hpp"

namespace xaifBooster { 

  InlinableIntrinsicsExpression::InlinableIntrinsicsExpression(unsigned int aNumberOfArguments,
							       PartialDerivativeKind::PartialDerivativeKind_E aPartialDerivativeKind) :
    myPartialArgumentSignature(aNumberOfArguments+1),
    myNumberOfArguments(aNumberOfArguments),
    myIsBuiltinFunction(false), 
    myPartialDerivativeKind(aPartialDerivativeKind),
    myNonValueInquiryFlag(false) {
    for (unsigned int i=0;i<aNumberOfArguments+1;i++)
      myPartialArgumentSignature[i]=0;
  }

  void 
  InlinableIntrinsicsExpression::setPartialArgument(const InlinableIntrinsicsExpressionVertex& theVertex,
						    unsigned int thePosition) { 
    if (thePosition>myNumberOfArguments) 
      THROW_LOGICEXCEPTION_MACRO("InlinableIntrinsicsExpression::setPartialArgument: position out of range");
    if (myPartialArgumentSignature[thePosition])
      THROW_LOGICEXCEPTION_MACRO("InlinableIntrinsicsExpression::setPartialArgument: trying to reset position >"
				 << thePosition
				 << "<; there can only be at most one argument in the partial per argument in the intrinsic.");
    myPartialArgumentSignature[thePosition]=&theVertex;
    myUsedPositionalArguments.add(thePosition);
  } 

  bool 
  InlinableIntrinsicsExpression::hasPartialArgumentAt(unsigned int aPosition) const { 
    if (aPosition>myNumberOfArguments) 
      THROW_LOGICEXCEPTION_MACRO("InlinableIntrinsicsExpression::setPartialArgument: position out of range");
    return (myPartialArgumentSignature[aPosition]!=0);
  } 

  const InlinableIntrinsicsExpressionVertex& 
  InlinableIntrinsicsExpression::getPartialArgumentAt(unsigned int aPosition) const { 
    if (!hasPartialArgumentAt(aPosition)) 
      THROW_LOGICEXCEPTION_MACRO("InlinableIntrinsicsExpression::setPartialArgument: not set");
    return *(myPartialArgumentSignature[aPosition]);
  } 

  bool InlinableIntrinsicsExpression::isBuiltinFunction() const {
    return myIsBuiltinFunction;
  } 
  
  void InlinableIntrinsicsExpression::setBuiltinFunction() {
    myIsBuiltinFunction=true;
  }

  void InlinableIntrinsicsExpression::setBuiltinFunctionName(std::string name) {
    myBuiltinFunctionName=name;
  } 
  
  const std::string& InlinableIntrinsicsExpression::getBuiltinFunctionName() const {
    return myBuiltinFunctionName;
  }

  const PositionSet& 
  InlinableIntrinsicsExpression::getUsedPositionalArguments() const {    
    return myUsedPositionalArguments;
  } 

  PartialDerivativeKind::PartialDerivativeKind_E 
  InlinableIntrinsicsExpression::getPartialDerivativeKind() const { 
    return myPartialDerivativeKind;
  } 

  void InlinableIntrinsicsExpression::setNonValueInquiry() {
    myNonValueInquiryFlag=true;
  }

  bool InlinableIntrinsicsExpression::isNonValueInquiry() const {
    return myNonValueInquiryFlag;
  }

} 
