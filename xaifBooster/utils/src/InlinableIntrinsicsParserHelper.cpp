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
#include "xaifBooster/utils/inc/InlinableIntrinsicsParserHelper.hpp"

namespace xaifBooster { 

#define INLINABLEINTRINSICSPARSERHELPER_INITIALIZATION(x)  \
my##x##_p(0)

#define INLINABLEINTRINSICSPARSERHELPER_DEFINITION(x)  \
void \
InlinableIntrinsicsParserHelper::set##x(x& whatToSet){ \
if( my##x##_p ) \
THROW_LOGICEXCEPTION_MACRO("InlinableIntrinsicsParserHelper::set" << #x << " item already set"); \
my##x##_p=&whatToSet; \
} \
x& \
InlinableIntrinsicsParserHelper::get##x() const { \
if(! my##x##_p ) \
THROW_LOGICEXCEPTION_MACRO("InlinableIntrinsicsParserHelper::get" << #x << " item not set"); \
return *my##x##_p; \
}

  InlinableIntrinsicsParserHelper::InlinableIntrinsicsParserHelper() :
    INLINABLEINTRINSICSPARSERHELPER_INITIALIZATION(InlinableIntrinsicsCatalogue),
    INLINABLEINTRINSICSPARSERHELPER_INITIALIZATION(InlinableIntrinsicsCatalogueItem),
    INLINABLEINTRINSICSPARSERHELPER_INITIALIZATION(InlinableIntrinsicsExpression),
    INLINABLEINTRINSICSPARSERHELPER_INITIALIZATION(InlinableIntrinsicsArgumentReference),
    INLINABLEINTRINSICSPARSERHELPER_INITIALIZATION(InlinableIntrinsicsIntrinsic),
    INLINABLEINTRINSICSPARSERHELPER_INITIALIZATION(InlinableIntrinsicsConstant),
    INLINABLEINTRINSICSPARSERHELPER_INITIALIZATION(InlinableIntrinsicsExpressionEdge)
  {} // end of   InlinableIntrinsicsParserHelper::InlinableIntrinsicsParserHelper

    
  INLINABLEINTRINSICSPARSERHELPER_DEFINITION(InlinableIntrinsicsCatalogue)
  INLINABLEINTRINSICSPARSERHELPER_DEFINITION(InlinableIntrinsicsCatalogueItem)
  INLINABLEINTRINSICSPARSERHELPER_DEFINITION(InlinableIntrinsicsExpression)
  INLINABLEINTRINSICSPARSERHELPER_DEFINITION(InlinableIntrinsicsArgumentReference)
  INLINABLEINTRINSICSPARSERHELPER_DEFINITION(InlinableIntrinsicsIntrinsic)
  INLINABLEINTRINSICSPARSERHELPER_DEFINITION(InlinableIntrinsicsConstant)
  INLINABLEINTRINSICSPARSERHELPER_DEFINITION(InlinableIntrinsicsExpressionEdge)

} // end of namespace

