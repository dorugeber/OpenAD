// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/utils/inc/LogicException.hpp"
#include "xaifBooster/system/inc/ObjectWithLineNumber.hpp"

namespace xaifBooster { 

  const std::string ObjectWithLineNumber::our_myLineNumber_XAIFName("lineNumber");

  ObjectWithLineNumber::ObjectWithLineNumber() : myLineNumber(0) { 
  } // end of ObjectWithLineNumber::ObjectWithLineNumber

  std::string ObjectWithLineNumber::debug () const { 
    std::ostringstream out;
    out << "ObjectWithLineNumber[" << this 
	<< ",myLineNumber=" << myLineNumber 
	<< "]" << std::ends;  
    return out.str();
  } // end of ObjectWithLineNumber::debug

  bool ObjectWithLineNumber::lineNumberIsSet()const { 
    return ((myLineNumber)?true:false);
  }

  void ObjectWithLineNumber::setLineNumber(unsigned int aLineNumber) { 
    if (myLineNumber) 
      THROW_LOGICEXCEPTION_MACRO("ObjectWithLineNumber::setLineNumber: already set");
    myLineNumber=aLineNumber;
  }

  unsigned int ObjectWithLineNumber::getLineNumber() const { 
    return myLineNumber;
  }

} 
