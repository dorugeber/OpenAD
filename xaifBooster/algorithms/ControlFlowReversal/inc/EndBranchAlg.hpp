#ifndef _XAIFBOOSTERCONTROLFLOWREVERSAL_ENDBRANCHALG_INCLUDE_
#define _XAIFBOOSTERCONTROLFLOWREVERSAL_ENDBRANCHALG_INCLUDE_
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
#include "xaifBooster/system/inc/EndBranchAlgBase.hpp"
#include "xaifBooster/system/inc/EndBranch.hpp"

using namespace xaifBooster;

namespace xaifBoosterControlFlowReversal {  

  /** 
   * class to provide algorithm for EndBranch
   */
  class EndBranchAlg : public EndBranchAlgBase, public ControlFlowGraphVertexAlg {
  public:
    
    EndBranchAlg(EndBranch& theContaining);
                                                                                
    ~EndBranchAlg();

    std::string debug() const;

    ControlFlowGraphVertexAlg::ControlFlowGraphVertexKind_E getKind() const {
      return ControlFlowGraphVertexAlg::ENDBRANCH;
    };


  private:
    
    /** 
     * no def
     */
    EndBranchAlg();

    /** 
     * no def
     */
    EndBranchAlg(const EndBranchAlg&);

    /** 
     * no def
     */
    EndBranchAlg& operator=(const EndBranchAlg&);

  };  // end of class

} // end of namespace 
                                                                     
#endif
