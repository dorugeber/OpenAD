#ifndef _XAIFBOOSTERBASICBLOCKPREACCUMULATIONTAPEADJOINT_BASICBLOCKALG_INCLUDE_
#define _XAIFBOOSTERBASICBLOCKPREACCUMULATIONTAPEADJOINT_BASICBLOCKALG_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/Assignment.hpp"
#include "xaifBooster/system/inc/BaseConstant.hpp"
#include "xaifBooster/system/inc/SubroutineCall.hpp"
#include "xaifBooster/system/inc/Symbol.hpp"
#include "xaifBooster/system/inc/PlainBasicBlock.hpp"
#include "xaifBooster/system/inc/ForLoopReversalType.hpp"

#include "xaifBooster/algorithms/DerivativePropagator/inc/DerivativePropagatorEntry.hpp"

#include "xaifBooster/algorithms/TypeChange/inc/TemporariesHelper.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulationTape/inc/BasicBlockAlg.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulationTapeAdjoint/inc/AdjointAssembly.hpp"

#include "xaifBooster/algorithms/InlinableXMLRepresentation/inc/InlinableSubroutineCall.hpp"

using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulationTapeAdjoint {  

  /** 
   * class to implement a reinterpretation of 
   * the DerivativePropagator instances as propagators in the adjoint sweep 
   */
  class BasicBlockAlg : public xaifBoosterBasicBlockPreaccumulationTape::BasicBlockAlg ,
			public AdjointAssembly {
  public:
    
    BasicBlockAlg(BasicBlock& theContaining);

    virtual ~BasicBlockAlg();

    virtual void printXMLHierarchy(std::ostream& os) const;

    virtual std::string debug() const ;

    virtual void traverseToChildren(const GenericAction::GenericAction_E anAction_c);
    
    virtual void algorithm_action_5();

    Scope& getScope();
    
  private:

    /// no def
    BasicBlockAlg();

    /// no def
    BasicBlockAlg(const BasicBlockAlg&);

    /// no def
    BasicBlockAlg& operator=(const BasicBlockAlg&);

    void replicateAllocationWrapper(const xaifBoosterBasicBlockPreaccumulation::Sequence& currentSequence,
                                    ForLoopReversalType::ForLoopReversalType_E aReversalType);

  }; 
 
} 
                                                                     
#endif
