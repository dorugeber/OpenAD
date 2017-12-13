#ifndef _XAIFBOOSTERCONTROLFLOWREVERSAL_IFSTATEMENTALG_INCLUDE_
#define _XAIFBOOSTERCONTROLFLOWREVERSAL_IFSTATEMENTALG_INCLUDE_
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
#include "xaifBooster/system/inc/IfStatementAlgBase.hpp"
#include "xaifBooster/system/inc/IfStatement.hpp"

using namespace xaifBooster;

namespace xaifBoosterControlFlowReversal {  

  /** 
   * class to provide algorithm for IfStatement
   */
  class IfStatementAlg : public IfStatementAlgBase, public ControlFlowGraphVertexAlg {
  public:
    
    IfStatementAlg(IfStatement& theContaining);
                                                                                
    ~IfStatementAlg();

    std::string debug() const;

    ControlFlowGraphVertexAlg::ControlFlowGraphVertexKind_E getKind() const {
      return ControlFlowGraphVertexAlg::IF;
    };


  private:
    
    /** 
     * no def
     */
    IfStatementAlg();

    /** 
     * no def
     */
    IfStatementAlg(const IfStatementAlg&);

    /** 
     * no def
     */
    IfStatementAlg& operator=(const IfStatementAlg&);

  };  // end of class

} // end of namespace 
                                                                     
#endif
