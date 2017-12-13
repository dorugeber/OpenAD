#ifndef _XAIFBOOSTERCONTROLFLOWREVERSAL_BASICBLOCKALG_INCLUDE_
#define _XAIFBOOSTERCONTROLFLOWREVERSAL_BASICBLOCKALG_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/BasicBlockAlgBase.hpp"
#include "xaifBooster/system/inc/BasicBlock.hpp"
#include "xaifBooster/system/inc/ForLoopReversalType.hpp"

#include "xaifBooster/algorithms/ControlFlowReversal/inc/ControlFlowGraphVertexAlg.hpp"

using namespace xaifBooster;

namespace xaifBoosterControlFlowReversal {  

  /** 
   * class to provide algorithm for BasicBlock
   */
  class BasicBlockAlg : virtual public BasicBlockAlgBase,
			public ControlFlowGraphVertexAlg {
  public:
    
    BasicBlockAlg(BasicBlock& theContaining);
                                                                                
    ~BasicBlockAlg();

    std::string debug() const;

    ControlFlowGraphVertexAlg::ControlFlowGraphVertexKind_E getKind() const {
      return ControlFlowGraphVertexAlg::BASICBLOCK;
    };

    void setEnumVal(unsigned short);

    unsigned short getEnumVal() const;

  private:
    
    /** 
     * no def
     */
    BasicBlockAlg();

    /** 
     * no def
     */
    BasicBlockAlg(const BasicBlockAlg&);

    /** 
     * no def
     */
    BasicBlockAlg& operator=(const BasicBlockAlg&);

    /** 
     * used to enumerate BasicBlocks for unstructured control flow 
     * reversal
     */
    unsigned short myEnumVal;

  };  // end of class

} // end of namespace 
                                                                     
#endif
