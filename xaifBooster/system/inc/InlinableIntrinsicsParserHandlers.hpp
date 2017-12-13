#ifndef _INLINABLEINTRINSICSPARSERHANDLERS_INCLUDE_
#define _INLINABLEINTRINSICSPARSERHANDLERS_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/utils/inc/InlinableIntrinsicsParserHelper.hpp"
#include "xaifBooster/system/inc/InlinableIntrinsicsArgumentReference.hpp"
#include "xaifBooster/system/inc/InlinableIntrinsicsCatalogue.hpp"
#include "xaifBooster/system/inc/InlinableIntrinsicsCatalogueItem.hpp"
#include "xaifBooster/system/inc/InlinableIntrinsicsConstant.hpp"
#include "xaifBooster/system/inc/InlinableIntrinsicsExpressionEdge.hpp"
#include "xaifBooster/system/inc/InlinableIntrinsicsExpression.hpp"
#include "xaifBooster/system/inc/InlinableIntrinsicsExpressionVertex.hpp"
#include "xaifBooster/system/inc/InlinableIntrinsicsIntermediateReference.hpp"
#include "xaifBooster/system/inc/InlinableIntrinsicsIntrinsic.hpp"


namespace xaifBooster {

  class InlinableIntrinsicsParserHandlers {
  public:
    /** 
     * method for InlinableIntrinsicDefinitions which is 
     * added to ourActionCatalogue
     */  
    void onInlinableIntrinsicsCatalogue(
		      const InlinableIntrinsicsParserHelper& passingIn,
		      InlinableIntrinsicsParserHelper& passingOut);

    /** 
     * method for InlinableIntrinsic which is 
     * added to ourActionCatalogue
     */  
    void onInlinableIntrinsicsCatalogueItem(
		      const InlinableIntrinsicsParserHelper& passingIn,
		      InlinableIntrinsicsParserHelper& passingOut);

    /** 
     * method for Function which is 
     * added to ourActionCatalogue
     */  
    void onInlinableIntrinsicsFunction(
		      const InlinableIntrinsicsParserHelper& passingIn,
		      InlinableIntrinsicsParserHelper& passingOut);

    /** 
     * method for Partial which is 
     * added to ourActionCatalogue
     */  
    void onInlinableIntrinsicsPartial(
		      const InlinableIntrinsicsParserHelper& passingIn,
		      InlinableIntrinsicsParserHelper& passingOut);

    /** 
     * method for ArgumentReference which is 
     * added to ourActionCatalogue
     */  
    void onInlinableIntrinsicsArgumentReference(
		       const InlinableIntrinsicsParserHelper& passingIn,
		       InlinableIntrinsicsParserHelper& passingOut);

    /** 
     * method for Constant which is 
     * added to ourActionCatalogue
     */  
    void onInlinableIntrinsicsConstant(
		    const InlinableIntrinsicsParserHelper& passingIn,
		    InlinableIntrinsicsParserHelper& passingOut);

    /** 
     * method for Intrinsic which is 
     * added to ourActionCatalogue
     */  
    void onInlinableIntrinsicsIntrinsic(
		     const InlinableIntrinsicsParserHelper& passingIn,
		     InlinableIntrinsicsParserHelper& passingOut);

    /** 
     * method for ExpressionEdge which is 
     * added to ourActionCatalogue
     */  
    void onInlinableIntrinsicsExpressionEdge(
			  const InlinableIntrinsicsParserHelper& passingIn,
			  InlinableIntrinsicsParserHelper& passingOut);

    /*
     * currently don't deal with 
     * added to ourActionCatalogue
     */  
    void onDummy(
		 const InlinableIntrinsicsParserHelper& passingIn,
		 InlinableIntrinsicsParserHelper& passingOut);

  }; // end of class InlinableIntrinsicsParserHandlers

} // end of namespace 

#endif
