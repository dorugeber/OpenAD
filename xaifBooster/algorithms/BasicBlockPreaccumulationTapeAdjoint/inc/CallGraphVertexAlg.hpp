#ifndef _XAIFBOOSTERBASICBLOCKPREACCUMULATIONTAPEADJOINT_CALLGRAPHVERTEXALG_INCLUDE_
#define _XAIFBOOSTERBASICBLOCKPREACCUMULATIONTAPEADJOINT_CALLGRAPHVERTEXALG_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/algorithms/BasicBlockPreaccumulationTape/inc/CallGraphVertexAlg.hpp"

using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulationTapeAdjoint {

  /** 
   * A class to inherit CallGraphVertexAlg from BasicBlockPreaccumulationTape,
   * which has a facility for marking values needed for the adjoint
   */
  class CallGraphVertexAlg : public xaifBoosterBasicBlockPreaccumulationTape::CallGraphVertexAlg {
  public:

    CallGraphVertexAlg(CallGraphVertex& theContaining);

    ~CallGraphVertexAlg();

    virtual void traverseToChildren(const GenericAction::GenericAction_E anAction_c);

  private:

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
    CallGraphVertexAlg operator=(const CallGraphVertexAlg&);

  };  // end class xaifBoosterBasicBlockPreaccumulationTapeAdjoint::CallGraphVertexAlg

} // end namespace xaifBoosterBasicBlockPreaccumulationTapeAdjoint

#endif

