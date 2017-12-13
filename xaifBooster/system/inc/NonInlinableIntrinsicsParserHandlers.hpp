#ifndef _NONINLINABLEINTRINSICSPARSERHANDLERS_INCLUDE_
#define _NONINLINABLEINTRINSICSPARSERHANDLERS_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/utils/inc/NonInlinableIntrinsicsParserHelper.hpp"
#include "xaifBooster/system/inc/NonInlinableIntrinsicsCatalogue.hpp"
#include "xaifBooster/system/inc/NonInlinableIntrinsicsCatalogueItem.hpp"

namespace xaifBooster {

  class NonInlinableIntrinsicsParserHandlers {
  public:
    /** 
     * method for NonInlinableIntrinsicDefinitions which is 
     * added to ourActionCatalogue
     */  
    void onNonInlinableIntrinsicsCatalogue(const NonInlinableIntrinsicsParserHelper& passingIn,
					   NonInlinableIntrinsicsParserHelper& passingOut);

    /** 
     * method for Function which is 
     * added to ourActionCatalogue
     */  
    void onExplicitJacobian(const NonInlinableIntrinsicsParserHelper& passingIn,
			    NonInlinableIntrinsicsParserHelper& passingOut);
    
    /** 
     * method for Partial which is 
     * added to ourActionCatalogue
     */  
    void onPartial(const NonInlinableIntrinsicsParserHelper& passingIn,
		   NonInlinableIntrinsicsParserHelper& passingOut);

    /** 
     * method for DirectAction which is 
     * added to ourActionCatalogue
     */  
    void onDirectAction(const NonInlinableIntrinsicsParserHelper& passingIn,
			NonInlinableIntrinsicsParserHelper& passingOut);

    /*
     * currently don't deal with 
     * added to ourActionCatalogue
     */  
    void onDummy(const NonInlinableIntrinsicsParserHelper& passingIn,
		 NonInlinableIntrinsicsParserHelper& passingOut);

  }; // end of class NonInlinableIntrinsicsParserHandlers

} // end of namespace 

#endif
