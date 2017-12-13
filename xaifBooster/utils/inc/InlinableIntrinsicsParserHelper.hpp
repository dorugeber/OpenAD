#ifndef _INLINABLEINTRINSICSPARSERHELPER_INCLUDE_
#define _INLINABLEINTRINSICSPARSERHELPER_INCLUDE_
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

  class InlinableIntrinsicsCatalogue;
  class InlinableIntrinsicsCatalogueItem;
  class InlinableIntrinsicsExpression;
  class InlinableIntrinsicsArgumentReference;
  class InlinableIntrinsicsIntrinsic;
  class InlinableIntrinsicsConstant;
  class InlinableIntrinsicsExpressionEdge;
  
  class InlinableIntrinsicsParserHelper : public XMLParserHelper { 
  public: 
    InlinableIntrinsicsParserHelper();
    ~InlinableIntrinsicsParserHelper(){};

    HELPER_DECLARATION(InlinableIntrinsicsCatalogue)
    HELPER_DECLARATION(InlinableIntrinsicsCatalogueItem)
    HELPER_DECLARATION(InlinableIntrinsicsExpression)
    HELPER_DECLARATION(InlinableIntrinsicsArgumentReference)
    HELPER_DECLARATION(InlinableIntrinsicsIntrinsic)
    HELPER_DECLARATION(InlinableIntrinsicsConstant)
    HELPER_DECLARATION(InlinableIntrinsicsExpressionEdge)

  }; // end of class InlinableIntrinsicsParserHelper

} // end of namespace

#endif
