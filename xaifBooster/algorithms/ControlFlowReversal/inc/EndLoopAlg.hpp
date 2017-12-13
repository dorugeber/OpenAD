#ifndef _XAIFBOOSTERCONTROLFLOWREVERSAL_ENDLOOPALG_INCLUDE_
#define _XAIFBOOSTERCONTROLFLOWREVERSAL_ENDLOOPALG_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/algorithms/ControlFlowReversal/inc/ControlFlowGraphVertexAlg.hpp"
#include "xaifBooster/system/inc/EndLoopAlgBase.hpp"
#include "xaifBooster/system/inc/EndLoop.hpp"

using namespace xaifBooster;

namespace xaifBoosterControlFlowReversal {  

  /** 
   * class to provide algorithm for EndLoop
   */
  class EndLoopAlg : public EndLoopAlgBase, public ControlFlowGraphVertexAlg {
  public:
    
    EndLoopAlg(EndLoop& theContaining);
                                                                                
    ~EndLoopAlg();

    std::string debug() const;

    ControlFlowGraphVertexAlg::ControlFlowGraphVertexKind_E getKind() const {
      return ControlFlowGraphVertexAlg::ENDLOOP;
    };


  private:
    
    /** 
     * no def
     */
    EndLoopAlg();

    /** 
     * no def
     */
    EndLoopAlg(const EndLoopAlg&);

    /** 
     * no def
     */
    EndLoopAlg& operator=(const EndLoopAlg&);

  };  // end of class

} // end of namespace 
                                                                     
#endif
