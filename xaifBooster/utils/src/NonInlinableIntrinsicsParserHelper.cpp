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
#include "xaifBooster/utils/inc/NonInlinableIntrinsicsParserHelper.hpp"

namespace xaifBooster { 

#define NONINLINABLEINTRINSICSPARSERHELPER_INITIALIZATION(x)  \
my##x##_p(0)

#define NONINLINABLEINTRINSICSPARSERHELPER_DEFINITION(x)  \
void \
NonInlinableIntrinsicsParserHelper::set##x(x& whatToSet){ \
if( my##x##_p ) \
THROW_LOGICEXCEPTION_MACRO("NonInlinableIntrinsicsParserHelper::set" << #x << " item already set"); \
my##x##_p=&whatToSet; \
} \
x& \
NonInlinableIntrinsicsParserHelper::get##x() const { \
if(! my##x##_p ) \
THROW_LOGICEXCEPTION_MACRO("NonInlinableIntrinsicsParserHelper::get" << #x << " item not set"); \
return *my##x##_p; \
}

  NonInlinableIntrinsicsParserHelper::NonInlinableIntrinsicsParserHelper() :
    NONINLINABLEINTRINSICSPARSERHELPER_INITIALIZATION(NonInlinableIntrinsicsCatalogue),
    NONINLINABLEINTRINSICSPARSERHELPER_INITIALIZATION(ExplicitJacobian)
  {} // end of   NonInlinableIntrinsicsParserHelper::NonInlinableIntrinsicsParserHelper

    
  NONINLINABLEINTRINSICSPARSERHELPER_DEFINITION(NonInlinableIntrinsicsCatalogue)
  NONINLINABLEINTRINSICSPARSERHELPER_DEFINITION(ExplicitJacobian)

} // end of namespace

