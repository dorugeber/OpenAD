#ifndef _XAIFBOOSTERTRACEDIFF_CALLGRAPHVERTEXALG_INCLUDE_
#define _XAIFBOOSTERTRACEDIFF_CALLGRAPHVERTEXALG_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/algorithms/ControlFlowReversal/inc/CallGraphVertexAlg.hpp"

#include "xaifBooster/algorithms/CodeReplacement/inc/ReplacementList.hpp"

#include "xaifBooster/algorithms/TraceDiff/inc/TraceableControlFlowGraph.hpp"

using namespace xaifBooster;

namespace xaifBoosterTraceDiff {  

  /** 
   * class to implement algorithms relevant for the 
   * reversal of the control flow
   */
  class CallGraphVertexAlg : public xaifBoosterControlFlowReversal::CallGraphVertexAlg {
  public:
    
    CallGraphVertexAlg(CallGraphVertex& theContaining);

    ~CallGraphVertexAlg();

    /**
     * control flow reversal
     */
    virtual void algorithm_action_2();
                                                                                
    virtual void printXMLHierarchy(std::ostream& os) const;
                                                                                
    virtual std::string debug() const ;

    virtual void traverseToChildren(const GenericAction::GenericAction_E anAction_c);

    xaifBoosterControlFlowReversal::ReversibleControlFlowGraph& getTracingControlFlowGraph();

    const xaifBoosterControlFlowReversal::ReversibleControlFlowGraph& getTracingControlFlowGraph() const;

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
    CallGraphVertexAlg& operator=(const CallGraphVertexAlg&);

    /**
     * CFG copy that traces for detecting potential non-smooth behavior
     */
    TraceableControlFlowGraph* myTracingControlFlowGraph_p;

    xaifBoosterCodeReplacement::ReplacementList* myReplacementList_p;

  }; 

} // end of namespace 
                                                                     
#endif
