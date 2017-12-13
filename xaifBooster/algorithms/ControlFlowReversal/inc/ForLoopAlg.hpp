#ifndef _XAIFBOOSTERCONTROLFLOWREVERSAL_PRELOOPALG_INCLUDE_
#define _XAIFBOOSTERCONTROLFLOWREVERSAL_PRELOOPALG_INCLUDE_
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
#include "xaifBooster/system/inc/ForLoopAlgBase.hpp"
#include "xaifBooster/system/inc/ForLoop.hpp"

using namespace xaifBooster;

namespace xaifBoosterControlFlowReversal {  

  /** 
   * class to provide algorithm for ForLoop
   */
  class ForLoopAlg : public ForLoopAlgBase, public ControlFlowGraphVertexAlg {
  public:
    
    ForLoopAlg(ForLoop& theContaining);
                                                                                
    ~ForLoopAlg();

    std::string debug() const;

    ControlFlowGraphVertexAlg::ControlFlowGraphVertexKind_E getKind() const {
      return ControlFlowGraphVertexAlg::FORLOOP;
    };


  private:
    
    /** 
     * no def
     */
    ForLoopAlg();

    /** 
     * no def
     */
    ForLoopAlg(const ForLoopAlg&);

    /** 
     * no def
     */
    ForLoopAlg& operator=(const ForLoopAlg&);

  };  // end of class

} // end of namespace 
                                                                     
#endif
