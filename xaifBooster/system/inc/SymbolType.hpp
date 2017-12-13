#ifndef _SYMBOLTYPE_INCLUDE_
#define _SYMBOLTYPE_INCLUDE_
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
#include <set>
#include "xaifBooster/utils/inc/PrintingIntException.hpp"

namespace xaifBooster { 
  class SymbolType { 
  public:

    /**
     * keep the order!
     */
    enum SymbolType_E {
      INTEGER_STYPE=0,
      REAL_STYPE=1,
      COMPLEX_STYPE=2,
      VOID_STYPE=3,
      BOOL_STYPE=4,
      CHAR_STYPE=5,
      STRING_STYPE=6,
      OPAQUE_STYPE=7
    };


    static std::string toString(const SymbolType_E& aType) throw (PrintingIntException);

    static const SymbolType_E fromString(const std::string& aName);
    
    static const std::string our_attribute_XAIFName;
    
    /** 
     * for pairs of types that can be promoted or are equal
     * \return the resulting type
     * or throws an exception
     */
    static SymbolType_E genericPromotion(const SymbolType_E& aType,
					 const SymbolType_E& anotherType);

    /** 
     * the types that can't be automatically promoted 
     * in binary operations
     */
    static const std::set<SymbolType_E> ourNonPromotables;

  }; // end of class SymbolType

} // end of namespace xaifBooster
                                                                     
#endif
