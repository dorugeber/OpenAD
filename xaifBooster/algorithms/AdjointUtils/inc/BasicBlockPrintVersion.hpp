#ifndef _XAIFBOOSTERADJOINTUTILS_BASICBLOCKPRINTVERSION_INCLUDE_
#define _XAIFBOOSTERADJOINTUTILS_BASICBLOCKPRINTVERSION_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/ForLoopReversalType.hpp"

using namespace xaifBooster;

namespace xaifBoosterAdjointUtils {  

  /** 
   * class to implement a selection between 
   * explicit and anonymous reversal for preaccumulation
   */
  class BasicBlockPrintVersion {
  public:
    
    static void set(ForLoopReversalType::ForLoopReversalType_E aReversalType);

    static ForLoopReversalType::ForLoopReversalType_E get(); 

  private:

    /**
     * we have data for both versions. The control flow reversal variant
     * decides which one we should take at any given time. Since 
     * we have both strict anonymous and also possible explicit reversal 
     * to generate we cannot decide per instance. 
     * so for each BasicBlock we set this to either ANONYMOUS or EPLICIT
     * upon printing time. 
     */
    static ForLoopReversalType::ForLoopReversalType_E ourPrintVersion; 

  };
 
} 
                                                                     
#endif
