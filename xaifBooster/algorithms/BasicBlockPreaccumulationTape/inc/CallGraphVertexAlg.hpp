#ifndef _XAIFBOOSTERBASICBLOCKPREACCUMULATIONTAPE_CALLGRAPHVERTEXALG_INCLUDE_
#define _XAIFBOOSTERBASICBLOCKPREACCUMULATIONTAPE_CALLGRAPHVERTEXALG_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include <list>

#include "xaifBooster/system/inc/CallGraphVertexAlgBase.hpp"
#include "xaifBooster/system/inc/CallGraphVertex.hpp"

#include "xaifBooster/algorithms/RequiredValues/inc/RequiredValueSet.hpp"

using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulationTape {  

  /** 
   * class to implement algorithms relevant for ...
   */
  class CallGraphVertexAlg : virtual public CallGraphVertexAlgBase {
  public:

    CallGraphVertexAlg(CallGraphVertex& theContaining);

    ~CallGraphVertexAlg();

    virtual std::string debug() const;

    virtual void traverseToChildren(const GenericAction::GenericAction_E anAction_c);

    void markRequiredValue(const Expression& theExpression,
                           const ControlFlowGraphVertex& theControlFlowGraphVertex,
                           const std::string theOriginStr);

  protected:

    /// this is where we keep the set of values that are marked as required by various algorithms
    xaifBoosterRequiredValues::RequiredValueSet myRequiredValueSet;

  private:

    /// no def
    CallGraphVertexAlg();

    /// no def
    CallGraphVertexAlg(const CallGraphVertexAlg&);

    /// no def
    CallGraphVertexAlg& operator=(const CallGraphVertexAlg&);

  }; // end class xaifBoosterBasicBlockPreaccumulationTape::CallGraphVertexAlg

} // end namespace xaifBoosterBasicBlockPreaccumulationTape

#endif

