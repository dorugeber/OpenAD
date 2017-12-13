#ifndef _XAIFBOOSTERPUSHPOP_BASICBLOCKALG_INCLUDE_
#define _XAIFBOOSTERPUSHPOP_BASICBLOCKALG_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/algorithms/AddressArithmetic/inc/BasicBlockAlg.hpp"

#include "xaifBooster/algorithms/RequiredValues/inc/RequiredValue.hpp"
#include "xaifBooster/algorithms/RequiredValues/inc/RequiredValueSet.hpp"

using namespace xaifBooster;

namespace xaifBoosterPushPop {  

  /** 
   * class to implement determination of which values to push/pop,
   * along with the placement of the pushes and pops themselves
   */
  class BasicBlockAlg : public xaifBoosterAddressArithmetic::BasicBlockAlg {
  public:

    BasicBlockAlg(BasicBlock& theContaining);

    virtual ~BasicBlockAlg();

    virtual std::string debug() const;

    virtual void traverseToChildren(const GenericAction::GenericAction_E anAction_c);

    virtual void algorithm_action_6_TD();

    /**
     * creates a push for a value from \p aRequiredValuePList that occurs no later than any other,
     * where the resolution is currently on the level of basic block elements and sequences
     */
    void pushSupremum(xaifBoosterRequiredValues::RequiredValueSet::RequiredValuePList aRequiredValuePList);

  private:

    /// no def
    BasicBlockAlg();

    /// no def
    BasicBlockAlg(const BasicBlockAlg&);

    /// no def
    BasicBlockAlg& operator=(const BasicBlockAlg&);

    /**
     * determine the order relation between \p firstExpression and \p secondExpression by
     * stepping through all basic block elements.  We check the data inside each sequence
     * after we've reached the last element in that sequence.
     */
    xaifBoosterRequiredValues::RequiredValueSet::ComparisonResult_E
    compareExpressions(const Expression& firstExpression,
                       const Expression& secondExpression) const;

    /**
     * add a push call for \p aRequiredValue in the appropriate place,
     * which is currently defined as the push block associated with
     * the subroutine call or sequence that this value is associated with
     * \todo take another parameter which is an upper limit to the push range, and possibly a lower limit as well
     */
    void pushRequiredValue(const xaifBoosterRequiredValues::RequiredValue& aRequiredValue);

    const BasicBlock& getContaining() const;

  }; // end class xaifBoosterPushPop::BasicBlockAlg
 
} // end namespace xaifBoosterPushPop
                                                                     
#endif

