#ifndef _XAIFBOOSTERTRACEDIFF_REVERSIBLECONTROLFLOWGRAPH_INCLUDE_
#define _XAIFBOOSTERTRACEDIFF_REVERSIBLECONTROLFLOWGRAPH_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================


#include "xaifBooster/algorithms/ControlFlowReversal/inc/ReversibleControlFlowGraph.hpp"

using namespace xaifBooster;

namespace xaifBoosterTraceDiff {  

  /** 
   * class to implement a traceable control flow graph
   * which uses a subset the same functionality already implemented 
   * for ReversibleControlFlowGraph
   */
  class TraceableControlFlowGraph : public xaifBoosterControlFlowReversal::ReversibleControlFlowGraph {

  public:
    
    TraceableControlFlowGraph(const ControlFlowGraph&);

    void traceActiveControlFlow();

    virtual std::string debug() const ;

  private:

    /** 
     * no def
     */
    TraceableControlFlowGraph();

    /** 
     * no def
     */
    TraceableControlFlowGraph(const TraceableControlFlowGraph&);

    /** 
     * no def
     */
    TraceableControlFlowGraph& operator=(const TraceableControlFlowGraph&);

  }; 

} 
                                                                     
#endif
