#ifndef _XAIFBOOSTERPUSHPOP_CALLGRAPHVERTEXALG_INCLUDE_
#define _XAIFBOOSTERPUSHPOP_CALLGRAPHVERTEXALG_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/algorithms/AddressArithmetic/inc/CallGraphVertexAlg.hpp"

#include "xaifBooster/algorithms/RequiredValues/inc/RequiredValueSet.hpp"

using namespace xaifBooster;

namespace xaifBoosterPushPop {  

  /**
   * class to implement algorithms relevant for coordinating
   * pushes and pops for values needed in the adjoint
   */
  class CallGraphVertexAlg : public xaifBoosterAddressArithmetic::CallGraphVertexAlg {
  public:

    CallGraphVertexAlg(CallGraphVertex& theContaining);

    ~CallGraphVertexAlg();

    virtual std::string debug() const;

    virtual void traverseToChildren(const GenericAction::GenericAction_E anAction_c);

    /**
     * Place pushes and pops
     */
    virtual void algorithm_action_6_TD();

  private:

    typedef std::list<xaifBoosterRequiredValues::RequiredValueSet::RequiredValuePList*> RequiredValuePListPList;

    typedef std::map<const ControlFlowGraphVertex*,
                     xaifBoosterRequiredValues::RequiredValueSet::RequiredValuePList>
       CFGVertexP2RequiredValuePListMap;

    /**
     * no def
     */
    CallGraphVertexAlg();

    /**
     * no def
     */
    CallGraphVertexAlg(const CallGraphVertexAlg&);

    /**
     * no def
     */
    CallGraphVertexAlg& operator=(const CallGraphVertexAlg&);

  };  // end class CallGraphVertexAlg

} // end namespace xaifBoosterPushPop

#endif

