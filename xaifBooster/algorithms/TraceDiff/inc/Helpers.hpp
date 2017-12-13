#ifndef _XAIFBOOSTERTRACEDIFF_HELPERS_INCLUDE_
#define _XAIFBOOSTERTRACEDIFF_HELPERS_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/BasicBlock.hpp"
#include "xaifBooster/system/inc/Argument.hpp"
#include "xaifBooster/system/inc/Expression.hpp"

using namespace xaifBooster;

namespace xaifBoosterTraceDiff {  

  /** 
   * helper class to implement tracing for arguments
   */
  class Helpers  {

  public:

    static bool argumentsHaveArrayAccess(const Expression::CArgumentPList& arguments); 
    
    static void traceArguments(const Expression::CArgumentPList& arguments,
			       BasicBlock& aBasicBlock);

    static void traceArgument(const Argument& anArgument,
			      PlainBasicBlock::BasicBlockElementList& aTraceList,
			      Scope& aScope); 

    static void handleArrayAccessIndices(const Argument& anArgument,
					 PlainBasicBlock::BasicBlockElementList& aTraceList,
					 Scope& aScope);
    
  };
 
} 
                                                                     
#endif
