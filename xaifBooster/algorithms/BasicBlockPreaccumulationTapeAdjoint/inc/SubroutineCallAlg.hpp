#ifndef _XAIFBOOSTERBASICBLOCKPREACCUMULATIONTAPEADJOINT_SUBROUTINECALLALG_INCLUDE_
#define _XAIFBOOSTERBASICBLOCKPREACCUMULATIONTAPEADJOINT_SUBROUTINECALLALG_INCLUDE_
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
#include "xaifBooster/system/inc/SubroutineCall.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulationTape/inc/SubroutineCallAlg.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulationTapeAdjoint/inc/BasicBlockElementAlg.hpp"


using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulationTapeAdjoint {  

  /** 
   * class to implement reversal of BasicBlockElements
   * NOTE: once the push/pop stuff is complete, 
   * this possibly should no longer inherit from xaifBoosterBasicBlockPreaccumulationTape::SubroutineCallAlg
   */
  class SubroutineCallAlg : virtual public xaifBoosterBasicBlockPreaccumulationTape::SubroutineCallAlg,
			    public BasicBlockElementAlg {

  public:
    
    SubroutineCallAlg(const SubroutineCall& theContainingSubroutineCall);

    virtual ~SubroutineCallAlg();

    virtual void printXMLHierarchy(std::ostream& os) const;

    virtual std::string debug() const ;

    virtual void traverseToChildren(const GenericAction::GenericAction_E anAction_c);

    virtual void insertYourself(const BasicBlock& theBasicBlock);

    bool isExternal() const;

    SubroutineCallAlg& getAdjointCounterPart();

    SubroutineCallAlg& getOriginalCounterPart();

    /**
     * inserts inlined restores for index values
     */
    void handleArrayAccessIndices(SubroutineCallAlg& orignalCallAlg,
                                  ForLoopReversalType::ForLoopReversalType_E aReversalType);

  private: 

    /** 
     * no def
     */
    SubroutineCallAlg();

    /** 
     * no def
     */
    SubroutineCallAlg(const SubroutineCallAlg&);

    /** 
     * no def
     */
    SubroutineCallAlg& operator=(const SubroutineCallAlg&);

    /** 
     * for anonymous reversals we need to restore 
     * any array indices occurring in formal arguments
     * this has to happen to the the call; populated for
     * the adjoint instance
     */
    PlainBasicBlock::BasicBlockElementList myPops;

    void insertYourself(const BasicBlock& theBasicBlock,
			ForLoopReversalType::ForLoopReversalType_E aReversalType);

    /**
     * set by insertYourself if this is the algorithm of the original instance
     */
    SubroutineCallAlg* myAdjointCounterpart_p;

    /**
     * set by insertYourself if this is the algorithm of the adjoint counterpart of an original instance
     */
    SubroutineCallAlg* myOriginalCounterpart_p;

  }; // end of class SubroutineCallAlg
 
} 
                                                                     
#endif
