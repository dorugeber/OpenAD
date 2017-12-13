#ifndef _NONINLINABLEINTRINSICSPARSERHELPER_INCLUDE_
#define _NONINLINABLEINTRINSICSPARSERHELPER_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/utils/inc/XMLParserHelper.hpp"

namespace xaifBooster { 

#define HELPER_DECLARATION(x)  \
public: \
void set##x(x& whatToSet); \
x& get##x() const ; \
private: \
x* my##x##_p;

  class NonInlinableIntrinsicsCatalogue;
  class ExplicitJacobian;
  
  class NonInlinableIntrinsicsParserHelper : public XMLParserHelper { 
  public: 
    NonInlinableIntrinsicsParserHelper();
    ~NonInlinableIntrinsicsParserHelper(){};

    HELPER_DECLARATION(NonInlinableIntrinsicsCatalogue)
    HELPER_DECLARATION(ExplicitJacobian)

  };

} 

#endif
