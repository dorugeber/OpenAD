#ifndef _PARTIALDERIVATIVEKIND_INCLUDE_
#define _PARTIALDERIVATIVEKIND_INCLUDE_
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
  class PartialDerivativeKind { 
public:
    enum PartialDerivativeKind_E {NOT_SET,
				  PASSIVE,
				  LINEAR_ONE,
				  LINEAR_MINUS_ONE,
				  LINEAR,
				  NONLINEAR};

    static std::string toString(const PartialDerivativeKind_E& aKind) throw (PrintingIntException);

    static const PartialDerivativeKind_E fromString(const std::string& aName);
    
    static const std::string our_attribute_XAIFName;
    
    /** 
     * dependency comparison with min being PASSIVE
     * and max being NONLINEAR
     * this is a partial order
     */
    static PartialDerivativeKind_E leastDependent(PartialDerivativeKind_E aKind,
						  PartialDerivativeKind_E anotherKind);

  }; // end of class PartialDerivativeKind

} // end of namespace xaifBooster
                                                                     
#endif
