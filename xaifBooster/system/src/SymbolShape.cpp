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
#include "xaifBooster/system/inc/SymbolShape.hpp"

namespace xaifBooster { 
  
  const std::string SymbolShape::our_attribute_XAIFName("shape");

  std::string SymbolShape::toString(const SymbolShape_E& aShape)
    throw (PrintingIntException) { 
    std::string returnString;
    switch(aShape) {
    case SCALAR:
      returnString="scalar";
      break;
    case VECTOR:
      returnString="vector";
      break;
    case MATRIX:
      returnString="matrix";
      break;
    case THREE_TENSOR:
      returnString="three_tensor";
      break;
    case FOUR_TENSOR:
      returnString="four_tensor";
      break;
    case FIVE_TENSOR:
      returnString="five_tensor";
      break;
    case SIX_TENSOR:
      returnString="six_tensor";
      break;
    case SEVEN_TENSOR:
      returnString="seven_tensor";
      break;
    case VOID:
      returnString="void";
      break;
    default: 
      throw PrintingIntException("SymbolShape::toString: unknown value",aShape);
      break;
    } // end switch
    return returnString;
  } // end of std::string SymbolShape::toString

  std::string SymbolShape::toShortString(const SymbolShape_E& aShape)
    throw (PrintingIntException) {
    std::string returnString;
    switch(aShape) {
    case SCALAR:
      returnString="s0";
      break;
    case VECTOR:
      returnString="s1";
      break;
    case MATRIX:
      returnString="s2";
      break;
    case THREE_TENSOR:
      returnString="s3";
      break;
    case FOUR_TENSOR:
      returnString="s4";
      break;
    case FIVE_TENSOR:
      returnString="s5";
      break;
    case SIX_TENSOR:
      returnString="s6";
      break;
    case SEVEN_TENSOR:
      returnString="s7";
      break;
    case VOID:
      returnString="s8";
      break;
    default:
      throw PrintingIntException("SymbolShape::toShortString: unknown value",aShape);
      break;
    } // end switch
    return returnString;
  }

  const SymbolShape::SymbolShape_E
  SymbolShape::fromString(const std::string& aName) { 
    SymbolShape_E returnValue;
    if (aName=="scalar")
      returnValue=SCALAR;
    else if (aName=="vector")
      returnValue=VECTOR;
    else if (aName=="matrix")
      returnValue=MATRIX;
    else if (aName=="three_tensor")
      returnValue=THREE_TENSOR;
    else if (aName=="four_tensor")
      returnValue=FOUR_TENSOR;
    else if (aName=="five_tensor")
      returnValue=FIVE_TENSOR;
    else if (aName=="six_tensor")
      returnValue=SIX_TENSOR;
    else if (aName=="seven_tensor")
      returnValue=SEVEN_TENSOR;
    else if (aName=="void")
      returnValue=VOID;
    else  
      THROW_LOGICEXCEPTION_MACRO("SymbolShape::fromString: unknown value >"
			   << aName.c_str() << "<");
    return returnValue;
  } // end of std::string SymbolShape::fromString
  
  int SymbolShape::difference(const SymbolShape::SymbolShape_E& oneShape, 
			      const SymbolShape::SymbolShape_E& minusTheOtherShape) { 
    return oneShape-minusTheOtherShape;
  } 

  const SymbolShape::SymbolShape_E SymbolShape::offset(const SymbolShape::SymbolShape_E& aShape, 
						       int offset) { 
    if ((aShape+offset)<0 || (aShape+offset)>SEVEN_TENSOR)
      THROW_LOGICEXCEPTION_MACRO("SymbolShape::offset: no shape for inputs "
				 << toString(aShape).c_str()
				 << " and  offset="
				 << offset);
    return (SymbolShape_E)(aShape+offset);
  }
  
} // end of namespace xaifBooster
