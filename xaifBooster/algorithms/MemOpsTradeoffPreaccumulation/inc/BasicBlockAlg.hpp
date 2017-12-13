#ifndef _XAIFBOOSTERMEMOPSTRADEOFFPREACCUMULATION_BASICBLOCKALG_INCLUDE_
#define _XAIFBOOSTERMEMOPSTRADEOFFPREACCUMULATION_BASICBLOCKALG_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/algorithms/MemOpsTradeoffPreaccumulation/inc/LinearizedComputationalGraphCopyEdge.hpp"
#include "xaifBooster/algorithms/MemOpsTradeoffPreaccumulation/inc/LinearizedComputationalGraphCopy.hpp"
#include "xaifBooster/algorithms/MemOpsTradeoffPreaccumulation/inc/JacobianAccumulationExpressionCopy.hpp"
#include "xaifBooster/algorithms/MemOpsTradeoffPreaccumulation/inc/DualGraph.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/BasicBlockAlg.hpp"

using namespace xaifBoosterCrossCountryInterface;

namespace xaifBoosterMemOpsTradeoffPreaccumulation {  

  /**
   * class to implement algorithms relevant for the 
   * elimination methods for memory-operations-tradeoffs
   */
  class BasicBlockAlg : public xaifBoosterBasicBlockPreaccumulation::BasicBlockAlg {
  public:

    BasicBlockAlg(BasicBlock& theContaining);
                                                                                
    virtual ~BasicBlockAlg() {};

    /**
     * calls algorithm_action_2() from 
     * xaifBoosterBasicBlockPreaccumulation::BasicBlockAlg 
     */
    virtual void algorithm_action_2();
                                                                                
    /**
     * calls algorithm_action_3() from 
     * xaifBoosterBasicBlockPreaccumulation::BasicBlockAlg 
     */

    virtual void algorithm_action_3();

    virtual void printXMLHierarchy(std::ostream& os) const;
                                                                                
    virtual std::string debug() const;

    virtual void traverseToChildren(const GenericAction::GenericAction_E anAction_c);

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

    /**
     * run the algorithm for creating the elminated graphs using thisMode
     */
    virtual void runElimination(Sequence& aSequence,
				SequenceHolder& aSequenceHolder,
				xaifBoosterBasicBlockPreaccumulation::PreaccumulationMode::PreaccumulationMode_E thisMode);
  };  // end of class

} // end of namespace 
                                                                     
#endif
