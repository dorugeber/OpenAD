#ifndef _XAIFBOOSTERCONTROLFLOWREVERSAL_EXITALG_INCLUDE_
#define _XAIFBOOSTERCONTROLFLOWREVERSAL_EXITALG_INCLUDE_
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
#include "xaifBooster/system/inc/ExitAlgBase.hpp"
#include "xaifBooster/system/inc/Exit.hpp"

using namespace xaifBooster;

namespace xaifBoosterControlFlowReversal {  

  /** 
   * class to provide algorithm for Exit
   */
  class ExitAlg : public ExitAlgBase, public ControlFlowGraphVertexAlg {
  public:
    
    ExitAlg(Exit& theContaining);
                                                                                
    ~ExitAlg();

    std::string debug() const;

    ControlFlowGraphVertexAlg::ControlFlowGraphVertexKind_E getKind() const {
      return ControlFlowGraphVertexAlg::EXIT;
    };


  private:
    
    /** 
     * no def
     */
    ExitAlg();

    /** 
     * no def
     */
    ExitAlg(const ExitAlg&);

    /** 
     * no def
     */
    ExitAlg& operator=(const ExitAlg&);

  };  // end of class

} // end of namespace 
                                                                     
#endif
