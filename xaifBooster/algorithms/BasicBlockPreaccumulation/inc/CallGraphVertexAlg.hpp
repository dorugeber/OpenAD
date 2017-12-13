#ifndef _XAIFBOOSTERBASICBLOCKPREACCUMULATION_CALLGRAPHVERTEXALG_INCLUDE_
#define _XAIFBOOSTERBASICBLOCKPREACCUMULATION_CALLGRAPHVERTEXALG_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/CallGraphVertexAlgBase.hpp"
#include "xaifBooster/system/inc/CallGraphVertex.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/BasicControlFlowGraph.hpp"

using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulation {  

  /** 
   * class to add basic block and active variable derivative initialization
   * subroutines to a modified BasicControlFlowGraph version of the contained 
   * control flow graph
   */
  class CallGraphVertexAlg : public CallGraphVertexAlgBase {

  public:
    
    CallGraphVertexAlg(const CallGraphVertex& theContaining);

    virtual ~CallGraphVertexAlg();

    virtual void printXMLHierarchy(std::ostream& os) const;

    virtual void algorithm_action_1();
                                                                                
    virtual std::string debug() const ;

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

    BasicControlFlowGraph* myBasicControlFlowGraph_p;

    /** 
     * no def
     */
    CallGraphVertexAlg& operator=(const CallGraphVertexAlg&);

  }; // end of class CallGraphVertexAlg

}

#endif
