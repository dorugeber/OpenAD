#ifndef _XAIFBOOSTERBASICBLOCKPREACCUMULATIONTAPE_SUBROUTINECALLALG_INCLUDE_
#define _XAIFBOOSTERBASICBLOCKPREACCUMULATIONTAPE_SUBROUTINECALLALG_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/system/inc/SubroutineCall.hpp"

#include "xaifBooster/algorithms/TypeChange/inc/SubroutineCallAlg.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulationTape/inc/BasicBlockElementAlg.hpp"

using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulationTape {  

  /** 
   * class to implement taping array access in arguments
   */
  class SubroutineCallAlg : virtual public xaifBoosterTypeChange::SubroutineCallAlg,
                            public BasicBlockElementAlg {

  public:
    
    SubroutineCallAlg(const SubroutineCall& theContainingSubroutineCall);

    virtual ~SubroutineCallAlg();

    virtual void printXMLHierarchy(std::ostream& os) const;

    virtual std::string debug() const ;

    virtual void traverseToChildren(const GenericAction::GenericAction_E anAction_c);

    /**
     * \todo : distinction between constant and non-constant index expressions is simplified
     * or we have this fixed by a proper TBR analysis
     */
    virtual void algorithm_action_4();

    void checkAndPush(const Variable& theVariable,ForLoopReversalType::ForLoopReversalType_E aReversalType);

    /**
     * check myAfterCallIndexPushes,myOnEntryFormalExpressionPList, and myOnEntryNonFormalExpressionPList for \p anExpression
     */
    virtual bool hasExpression(const Expression& theExpression) const;

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
     * This is a list of formal parameters that are required on entry.
     * this class owns the dummy expressions that were created for the purpose of marking them
     * as required for the push/pop algorithm(s), and this is where they reside.
     * They are deleted in the destructor.
     */
    std::list<const Expression*> myOnEntryFormalExpressionPList;

    /**
     * Values required on entry that are not formal parameters.
     * These may also have to be restored, but they cannot be restored inside
     * the called context (they must be restored in the calling context)
     * We create fake expressions for these (for the sake of pointing to as required values).
     * We own these fake expressions and delete them in the destructor
     */
    std::list<const Expression*> myOnEntryNonFormalExpressionPList;

    /** 
     * inserts inlined stores for index values
     */
    void handleArrayAccessIndices(ConcreteArgument& theConcreteArgument);
    
  }; // end of class SubroutineCallAlg
 
} 
                                                                     
#endif
