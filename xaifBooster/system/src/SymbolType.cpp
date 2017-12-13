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
#include "xaifBooster/system/inc/SymbolType.hpp"

namespace xaifBooster { 
  
  const std::string SymbolType::our_attribute_XAIFName("type");

  static const SymbolType::SymbolType_E typesList[]={
    SymbolType::INTEGER_STYPE,
    SymbolType::REAL_STYPE,
    SymbolType::COMPLEX_STYPE,
    SymbolType::VOID_STYPE,
    SymbolType::BOOL_STYPE,
    SymbolType::CHAR_STYPE,
    SymbolType::STRING_STYPE,
    SymbolType::OPAQUE_STYPE
  };
  
  const std::set<SymbolType::SymbolType_E> SymbolType::ourNonPromotables(&(typesList[SymbolType::VOID_STYPE]),&(typesList[SymbolType::OPAQUE_STYPE])+1);

  std::string SymbolType::toString(const SymbolType_E& aType)
    throw (PrintingIntException) { 
    std::string returnString;
    switch(aType) {
    case INTEGER_STYPE: 
      returnString="integer";
      break;
    case REAL_STYPE: 
      returnString="real";
      break;
    case COMPLEX_STYPE:
      returnString="complex";
      break;
    case VOID_STYPE:
      returnString="void";
      break;
    case BOOL_STYPE: 
      returnString="bool";
      break;
    case CHAR_STYPE:
      returnString="char";
      break;
    case STRING_STYPE: 
      returnString="string";
      break;
    case OPAQUE_STYPE: 
      returnString="opaque";
      break;
    default: 
      throw PrintingIntException("SymbolType::toString: unknown value",aType);
      break;
    } // end switch
    return returnString;
  } // end of std::string SymbolType::toString

  const SymbolType::SymbolType_E
  SymbolType::fromString(const std::string& aName) { 
    SymbolType_E returnValue;
    if (aName=="integer")
      returnValue=INTEGER_STYPE;
    else if (aName=="real")
      returnValue=REAL_STYPE;
    else if (aName=="complex")
      returnValue=COMPLEX_STYPE;
    else if (aName=="void")
      returnValue=VOID_STYPE;
    else if (aName=="bool")
      returnValue=BOOL_STYPE;
    else if (aName=="char")
      returnValue=CHAR_STYPE;
    else if (aName=="string")
      returnValue=STRING_STYPE;
    else if (aName=="opaque")
      returnValue=OPAQUE_STYPE;
    else  
      THROW_LOGICEXCEPTION_MACRO("SymbolType::fromString: unknown value >" << aName.c_str() << "<");
    return returnValue;
  } // end of std::string SymbolType::fromString

  SymbolType::SymbolType_E SymbolType::genericPromotion(const SymbolType::SymbolType_E& aType,
							const SymbolType::SymbolType_E& anotherType) {
    if (aType==anotherType)
      return aType;
    if (ourNonPromotables.find(aType)!=ourNonPromotables.end()
	||
	ourNonPromotables.find(anotherType)!=ourNonPromotables.end()) {
      THROW_LOGICEXCEPTION_MACRO("SymbolType::genericPromotion: not between " << toString(aType) << " and " << toString(anotherType));
    }
    return (aType>anotherType)?aType:anotherType;
  }

} // end of namespace xaifBooster
