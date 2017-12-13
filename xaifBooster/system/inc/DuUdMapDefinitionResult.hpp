#ifndef _DUUDMAPDEFINITIONRESULT_INCLUDE_
#define _DUUDMAPDEFINITIONRESULT_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include <list>

#include "xaifBooster/utils/inc/ObjectWithId.hpp"

namespace xaifBooster { 

  class DuUdMapDefinitionResult {
    
  public: 
      enum DuUdMapDefinition_E { UNIQUE_INSIDE,
				 UNIQUE_OUTSIDE,
				 AMBIGUOUS_INSIDE,
				 AMBIGUOUS_OUTSIDE,
				 AMBIGUOUS_BOTHSIDES,
				 DONT_KNOW };
    DuUdMapDefinitionResult();
    
    ~DuUdMapDefinitionResult();
    
    DuUdMapDefinition_E myAnswer;
    
    ObjectWithId::Id myStatementId;
    
  };

} // end of namespace 
                                                                     
#endif
