#ifndef _XAIFBOOSTERADDRESSARITHMETIC_BASICBLOCKALG_INCLUDE_
#define _XAIFBOOSTERADDRESSARITHMETIC_BASICBLOCKALG_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/algorithms/BasicBlockPreaccumulationTape/inc/BasicBlockAlg.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulationTapeAdjoint/inc/BasicBlockAlg.hpp"
#include "xaifBooster/algorithms/ControlFlowReversal/inc/BasicBlockAlg.hpp"

using namespace xaifBooster;

namespace xaifBoosterAddressArithmetic {  

  /** 
   * class to pull together 
   * the taping and the adjoining 
   * view per basic block 
   * and the augmented and reversed call graph
   * This class is just here to ensure we take 
   * the proper path for the virtual method 
   * invocations as we refer to address arithmetic 
   * related to preaccumulation, taping, adjoining the tape 
   * AND we have to integrate the modifications with the control flow reversal
   * this class itself does not contribute to the address arithmetic 
   * transformation, it just ensure proper calling.
   */
  class BasicBlockAlg : public xaifBoosterControlFlowReversal::BasicBlockAlg, 
			public xaifBoosterBasicBlockPreaccumulationTapeAdjoint::BasicBlockAlg {

  public:
    
    BasicBlockAlg(BasicBlock& theContaining);

    virtual ~BasicBlockAlg() {};

    virtual std::string debug() const ;

    virtual void printXMLHierarchy(std::ostream& os) const;

    virtual void traverseToChildren(const GenericAction::GenericAction_E anAction_c);

    /**
     * referring to xaifBoosterBasicBlockPreaccumulation::BasicBlockAlg::algorithm_action_2
     */
    virtual void algorithm_action_2();

    /**
     * referring to xaifBoosterBasicBlockPreaccumulation::BasicBlockAlg::algorithm_action_3
     */
    virtual void algorithm_action_3();

    /**
     * referring to xaifBoosterBasicBlockPreaccumulationTape::BasicBlockAlg::algorithm_action_4
     */
    virtual void algorithm_action_4();

    /**
     * referring to xaifBoosterBasicBlockPreaccumulationTapeAdjoint::BasicBlockAlg::algorithm_action_5
     */
    virtual void algorithm_action_5();

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
