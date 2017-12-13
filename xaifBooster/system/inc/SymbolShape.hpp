#ifndef _SYMBOLSHAPE_INCLUDE_
#define _SYMBOLSHAPE_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include <string>
#include "xaifBooster/utils/inc/PrintingIntException.hpp"

namespace xaifBooster { 
  class SymbolShape { 
public:
    enum SymbolShape_E {
      // keep these numbers in order so we can compute differences
      // even though differences with enumerated names should normally be frowned upon. 
	    SCALAR=0,
	    VECTOR=1,
	    MATRIX=2,
	    THREE_TENSOR=3,
	    FOUR_TENSOR=4,
	    FIVE_TENSOR=5,
	    SIX_TENSOR=6,
	    SEVEN_TENSOR=7,
            VOID=8};
    
    static std::string toString(const SymbolShape_E& aShape) throw (PrintingIntException);

    static std::string toShortString(const SymbolShape_E& aShape) throw (PrintingIntException);

    static const SymbolShape_E fromString(const std::string& aName);
    
    static const std::string our_attribute_XAIFName;

    /**
     * this is the result of 
     * oneShape-minusTheOtherShape
     */
    static int difference(const SymbolShape_E& oneShape, 
			  const SymbolShape_E& minusTheOtherShape);

    /**
     * the shape that is offset by offset
     */
    static const SymbolShape_E offset(const SymbolShape_E& aShape, 
				      int offset);

  }; // end of class SymbolShape

} // end of namespace xaifBooster
                                                                     
#endif
