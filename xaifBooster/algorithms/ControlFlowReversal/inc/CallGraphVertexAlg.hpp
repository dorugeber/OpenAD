#ifndef _XAIFBOOSTERCONTROLFLOWREVERSAL_CALLGRAPHVERTEXALG_INCLUDE_
#define _XAIFBOOSTERCONTROLFLOWREVERSAL_CALLGRAPHVERTEXALG_INCLUDE_
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
#include "xaifBooster/algorithms/ControlFlowReversal/inc/ReversibleControlFlowGraph.hpp"

using namespace xaifBooster;

namespace xaifBoosterControlFlowReversal {  

  /** 
   * class to implement algorithms relevant for the 
   * reversal of the control flow
   */
  class CallGraphVertexAlg : virtual public CallGraphVertexAlgBase {
  public:
    
    CallGraphVertexAlg(CallGraphVertex& theContaining);

    ~CallGraphVertexAlg();

    /**
     * control flow reversal
     */
    virtual void algorithm_action_4();
                                                                                
    virtual void printXMLHierarchy(std::ostream& os) const;
                                                                                
    virtual std::string debug() const ;

    virtual void traverseToChildren(const GenericAction::GenericAction_E anAction_c);

    bool hasTapingControlFlowGraph() const;
    ReversibleControlFlowGraph& getTapingControlFlowGraph();
    const ReversibleControlFlowGraph& getTapingControlFlowGraph() const;
 
    bool hasAdjointControlFlowGraph() const;
    ReversibleControlFlowGraph& getAdjointControlFlowGraph();
    const ReversibleControlFlowGraph& getAdjointControlFlowGraph() const;

    bool hasStrictAnonymousTapingControlFlowGraph() const;
    ReversibleControlFlowGraph& getStrictAnonymousTapingControlFlowGraph();
    const ReversibleControlFlowGraph& getStrictAnonymousTapingControlFlowGraph() const;
 
    bool hasStrictAnonymousAdjointControlFlowGraph() const;
    ReversibleControlFlowGraph& getStrictAnonymousAdjointControlFlowGraph();
    const ReversibleControlFlowGraph& getStrictAnonymousAdjointControlFlowGraph() const;

    static void changeIntent();

    static bool changesIntent();

  private:
    
    /** 
     * no def
     */
    CallGraphVertexAlg();

    /** 
     * no def
     */
    CallGraphVertexAlg(const CallGraphVertexAlg&);

    void structuredReversal();

    void unstructuredReversal();

    /** 
     * no def
     */
    CallGraphVertexAlg& operator=(const CallGraphVertexAlg&);

    /**
     * CFG copy that tapes for partially explicit reversal
     * the graph structure is the same as for strictly anonymous
     * reversal but the contents of the basic blocks differ from the ones in 
     * the strictly anonymous version
     */
    ReversibleControlFlowGraph* myTapingControlFlowGraph_p;
    /**
     * CFG copy that performs partially explicit reversal 
     * the graph structure is the same as for strictly anonymous
     * reversal but the contents of the basic blocks and the 
     * adjoint control flow expressions differ from the ones in 
     * the strictly anonymous version
     */
    ReversibleControlFlowGraph* myAdjointControlFlowGraph_p;
    /**
     * CFG copy that tapes for strictly anonymous reversal 
     */
    ReversibleControlFlowGraph* myStrictAnonymousTapingControlFlowGraph_p;
    /**
     * CFG copy that performs strictly anonymous reversal 
     */
    ReversibleControlFlowGraph* myStrictAnonymousAdjointControlFlowGraph_p;
    
    /** 
     * change argument intent, this information is needed 
     * to determine which if any arguments need to be initialized
     * the intent change is needed for checkpointing; 
     * there can be cases where intents are specified only for constant arguments 
     * which explains why the change is not done by default. 
     */
    static bool ourChangeIntentFlag;

  };  // end of class

} // end of namespace 
                                                                     
#endif
