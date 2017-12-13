#ifndef _XAIFBOOSTERBASICBLOCKPREACCUMULATIONTAPEADJOINT_ASSIGNMENTALG_INCLUDE_
#define _XAIFBOOSTERBASICBLOCKPREACCUMULATIONTAPEADJOINT_ASSIGNMENTALG_INCLUDE_
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

#include "xaifBooster/algorithms/BasicBlockPreaccumulationTape/inc/AssignmentAlg.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulationTapeAdjoint/inc/BasicBlockElementAlg.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulationTapeAdjoint/inc/AdjointAssembly.hpp"

using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulationTapeAdjoint {  

  /** 
   * class to implement reversal of BasicBlockElements
   */
  class AssignmentAlg : public xaifBoosterBasicBlockPreaccumulationTape::AssignmentAlg,
			public BasicBlockElementAlg,
			public AdjointAssembly  {

  public:
    
    AssignmentAlg(Assignment& theContainingAssignment);

    virtual ~AssignmentAlg();

    virtual void printXMLHierarchy(std::ostream& os) const;

    virtual std::string debug() const ;

    virtual void traverseToChildren(const GenericAction::GenericAction_E anAction_c);

    virtual void insertYourself(const BasicBlock& theBasicBlock);

    virtual Scope& getScope();

  private: 

    /** 
     * no def
     */
    AssignmentAlg();

    /** 
     * no def
     */
    AssignmentAlg(const AssignmentAlg&);

    /** 
     * no def
     */
    AssignmentAlg& operator=(const AssignmentAlg&);

    /** 
     * inserts inlined restores for index values
     */
    void handleArrayAccessIndices(AssignmentAlg& orignalAssignmentAlg,
                                  ForLoopReversalType::ForLoopReversalType_E aReversalType);

    /**
     * inserts myPlaceholderDummy_p for non-inlinable assignments
     * @param theBasicBlock is the original basic block
     * @param aReversalType is the reversal type
     */
    void insertYourself(const BasicBlock& theBasicBlock,
			ForLoopReversalType::ForLoopReversalType_E aReversalType);

    /**
     * performs the adjoint actions on the algorithm associated with myPlaceholderDummy_p
     *
     */
    void adjointAssembly(const AssignmentAlg& theOrigAlg,
                         const BasicBlock& theBasicBlock,
			 ForLoopReversalType::ForLoopReversalType_E aReversalType );

    /**
     * dummies to place in the reverse basic block and hold the position
     * because it is placed there the basic block assumes ownership
     * and we don't delete it
     */
    Assignment* myPlaceholderDummyAnonymous_p;
    Assignment* myPlaceholderDummyExplicit_p;

  }; // end of class AssignmentAlg
 
} 
                                                                     
#endif
