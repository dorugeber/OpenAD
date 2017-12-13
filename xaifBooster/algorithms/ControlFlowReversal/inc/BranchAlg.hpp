#ifndef _XAIFBOOSTERCONTROLFLOWREVERSAL_BRANCHALG_INCLUDE_
#define _XAIFBOOSTERCONTROLFLOWREVERSAL_BRANCHALG_INCLUDE_
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
#include "xaifBooster/system/inc/BranchAlgBase.hpp"
#include "xaifBooster/system/inc/Branch.hpp"

using namespace xaifBooster;

namespace xaifBoosterControlFlowReversal {  

  /** 
   * class to provide algorithm for Branch
   */
  class BranchAlg : public BranchAlgBase, public ControlFlowGraphVertexAlg {
  public:
    
    BranchAlg(Branch& theContaining);
                                                                                
    ~BranchAlg();

    std::string debug() const;

    ControlFlowGraphVertexAlg::ControlFlowGraphVertexKind_E getKind() const {
      return ControlFlowGraphVertexAlg::BRANCH;
    };


  private:
    
    /** 
     * no def
     */
    BranchAlg();

    /** 
     * no def
     */
    BranchAlg(const BranchAlg&);

    /** 
     * no def
     */
    BranchAlg& operator=(const BranchAlg&);

  };  // end of class

} // end of namespace 
                                                                     
#endif
