// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================


#include "xaifBooster/utils/inc/DbgLoggerManager.hpp"
#include "xaifBooster/system/inc/SymbolReference.hpp"
#include "xaifBooster/algorithms/TypeChange/inc/SignaturePattern.hpp"

namespace xaifBoosterTypeChange { 

  const unsigned short SignaturePattern::ourMaxParameterCount;

  SignaturePattern::SignaturePattern() : mySize(-1) {
  } 

  void SignaturePattern::trackAt(unsigned short aPosition) { 
    if (aPosition<1 || aPosition>getSize())
      THROW_LOGICEXCEPTION_MACRO("SignaturePattern::setActive: aPosition "
				 << aPosition
				 << " is out of range [1,"
				 << getSize()
				 << "]");
    myPattern.set(aPosition-1);
  }
  
  void SignaturePattern::setSize(unsigned short aSize) { 
    if (aSize>ourMaxParameterCount)
      THROW_LOGICEXCEPTION_MACRO("SignaturePattern::setSize: "
				 << aSize
				 << " is out of range [0,"
				 << ourMaxParameterCount
				 << "]");
    mySize=aSize;
  }
  

  unsigned short SignaturePattern::getSize()const { 
    if (mySize<0)
      THROW_LOGICEXCEPTION_MACRO("SignaturePattern::getSize: not set ");
    return mySize;
  }

  bool SignaturePattern::initialized() const { 
    return (mySize>=0);
  } 
  
  bool SignaturePattern::operator == (const SignaturePattern& anotherPattern) const { 
    return (myPattern==anotherPattern.myPattern);
  } 
  
  bool SignaturePattern::operator != (const SignaturePattern& anotherPattern) const{ 
    return (myPattern!=anotherPattern.myPattern);
  } 

  std::string SignaturePattern::discrepancyPositions(const SignaturePattern& anotherPattern) const { 
    if (getSize()!=anotherPattern.getSize()) 
      THROW_LOGICEXCEPTION_MACRO("SignaturePattern::discrepancyPositions: size mismatch ");
    std::ostringstream out;
    for (unsigned short i=0; i<mySize-1; ++i) { 
      if (myPattern.test(i)!=anotherPattern.myPattern.test(i)) 
	out << i+1 << " ";
    }
    return out.str();
  } 
  
  bool SignaturePattern::isTracked(unsigned short aPosition) const { 
    if (aPosition<1 || aPosition>getSize())
      THROW_LOGICEXCEPTION_MACRO("SignaturePattern::isTracked: aPosition "
				 << aPosition
				 << " is out of range [1,"
				 << getSize()
				 << "]");
    return myPattern.test(aPosition-1);
  } 

}

