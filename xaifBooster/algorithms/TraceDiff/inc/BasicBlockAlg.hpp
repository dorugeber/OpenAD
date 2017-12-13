#ifndef _XAIFBOOSTERTRACEDIFF_BASICBLOCKALG_INCLUDE_
#define _XAIFBOOSTERTRACEDIFF_BASICBLOCKALG_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/algorithms/ControlFlowReversal/inc/BasicBlockAlg.hpp"

using namespace xaifBooster;

namespace xaifBoosterTraceDiff {  

  /** 
   * class to ...
   */
  class BasicBlockAlg : public xaifBoosterControlFlowReversal::BasicBlockAlg { 

  public:
    
    BasicBlockAlg(BasicBlock& theContaining);

    virtual ~BasicBlockAlg() {};

    virtual std::string debug() const ;

    virtual void traverseToChildren(const GenericAction::GenericAction_E anAction_c);

    /**
     * referring to xaifBoosterBasicBlockPreaccumulation::BasicBlockAlg::algorithm_action_2
     */
    virtual void algorithm_action_2();

  private:

    /** 
     * no def
     */
    BasicBlockAlg();

    /** 
     * no def
     */
    BasicBlockAlg(const BasicBlockAlg&);

    /** 
     * no def
     */
    BasicBlockAlg& operator=(const BasicBlockAlg&);

  };
 
} // end of namespace 
                                                                     
#endif
