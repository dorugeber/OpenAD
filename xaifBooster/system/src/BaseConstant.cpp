// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include <iomanip>
#include "xaifBooster/utils/inc/LogicException.hpp"
#include "xaifBooster/utils/inc/StringConversions.hpp"
#include "xaifBooster/system/inc/BaseConstant.hpp"

namespace xaifBooster { 

  const std::string BaseConstant::ourBooleanTrueString("true");
  const std::string BaseConstant::ourBooleanFalseString("false");

  std::string BaseConstant::debug () const { 
    std::ostringstream out;
    out << "BaseConstant[" << this 
	<< ",myType=" << SymbolType::toString(myType)
	<< ",myFrontEndType=" << myFrontEndType.c_str()
	<< ",myValue=" << toString().c_str()
	<< "]" << std::ends;  
    return out.str();
  } // end of ExpressionVertex::debug

  double BaseConstant::getdouble()const { 
    if (myType!=SymbolType::REAL_STYPE)
      THROW_LOGICEXCEPTION_MACRO("BaseConstant::getdouble typs is " + 
				 SymbolType::toString(myType));
    return myValue.f;
  } 

  void BaseConstant::setdouble(double f) { 
    if (myType!=SymbolType::REAL_STYPE)
      THROW_LOGICEXCEPTION_MACRO("BaseConstant::setdouble typs is " + 
				 SymbolType::toString(myType));
    myValue.f=f;
  } 

  int BaseConstant::getint()const { 
    if (myType!=SymbolType::INTEGER_STYPE)
      THROW_LOGICEXCEPTION_MACRO("BaseConstant::getint typs is " + 
				 SymbolType::toString(myType));
    return myValue.i;
  } 

  void BaseConstant::setint(int i) { 
    if (myType!=SymbolType::INTEGER_STYPE)
      THROW_LOGICEXCEPTION_MACRO("BaseConstant::setint typs is " + 
				 SymbolType::toString(myType));
    myValue.i=i;
  } 

  void BaseConstant::setbool(bool b) { 
    if (myType!=SymbolType::BOOL_STYPE)
      THROW_LOGICEXCEPTION_MACRO("BaseConstant::setint typs is " + 
				 SymbolType::toString(myType));
    myValue.b=b;
  } 

  bool BaseConstant::getbool()const { 
    if (myType!=SymbolType::BOOL_STYPE)
      THROW_LOGICEXCEPTION_MACRO("BaseConstant::getint typs is " + 
				 SymbolType::toString(myType));
    return myValue.b;
  } 

  void BaseConstant::setchar(char c) { 
    if (myType!=SymbolType::CHAR_STYPE)
      THROW_LOGICEXCEPTION_MACRO("BaseConstant::setint typs is " + 
				 SymbolType::toString(myType));
    myValue.c=c;
  } 

  char BaseConstant::getchar()const { 
    if (myType!=SymbolType::CHAR_STYPE)
      THROW_LOGICEXCEPTION_MACRO("BaseConstant::getint typs is " + 
				 SymbolType::toString(myType));
    return myValue.c;
  } 

  void 
  BaseConstant::setFromString(const std::string& aValue) { 
    myOriginalStringRep=aValue;
    switch(myType) { 
    case SymbolType::INTEGER_STYPE : 
      setint(StringConversions::convertToInt(aValue.c_str()));
      break;
    case SymbolType::REAL_STYPE : 
      setdouble(StringConversions::convertToDouble(aValue.c_str()));
      break;
    case SymbolType::BOOL_STYPE : 
      { 
	if (aValue==ourBooleanTrueString) 
	  setbool(true);
	else if (aValue==ourBooleanFalseString)
	  setbool(false);
	else 
	  THROW_LOGICEXCEPTION_MACRO("BaseConstant::setFromString: boolean value expected to be " 
				     + ourBooleanTrueString + " or " + ourBooleanFalseString);
	break;
      }
    case SymbolType::STRING_STYPE : 
      break;
    case SymbolType::CHAR_STYPE : 
      { 
	if (aValue.size()!=1)
	  THROW_LOGICEXCEPTION_MACRO("BaseConstant::setFromString: char value expected to be a string of length 1");
	setchar(aValue[0]);
	break;
      }
    default: 
      THROW_LOGICEXCEPTION_MACRO("BaseConstant::setFromString: cannot handle " 
				 + SymbolType::toString(myType));
      break;
    } // end switch
  } 

  std::string
  BaseConstant::toString() const {
    std::ostringstream oss;
    switch(myType) { 
    case SymbolType::INTEGER_STYPE : 
      oss << getint();
      break;
    case SymbolType::REAL_STYPE : 
      oss << std::scientific << std::setprecision(20) << getdouble();
      break;
    case SymbolType::BOOL_STYPE : 
      if (myValue.b)
	oss << ourBooleanTrueString.c_str();
      else
	oss << ourBooleanFalseString.c_str();
      break;
    case SymbolType::STRING_STYPE : 
      oss << myOriginalStringRep.c_str();
      break;
    case SymbolType::CHAR_STYPE : 
      oss << getchar();
      break;
    default: 
      THROW_LOGICEXCEPTION_MACRO("BaseConstant::toString: cannot handle " 
				 + SymbolType::toString(myType));
      break;
    } // end switch
    return oss.str();
  } 

  SymbolType::SymbolType_E  
  BaseConstant::getType() const {
    return myType;
  }

  const FrontEndType& BaseConstant::getFrontEndType() const { 
    return myFrontEndType;
  }
  
  void BaseConstant::setFrontEndType(const FrontEndType& aFrontEndType) {
    if (!myFrontEndType.empty())
      THROW_LOGICEXCEPTION_MACRO("BaseConstant::setFrontEndType: already set");
    myFrontEndType=aFrontEndType;
  }

} 
