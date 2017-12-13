#ifndef _BRANCH_INCLUDE_
#define _BRANCH_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/ControlFlowGraphVertex.hpp"
#include "xaifBooster/system/inc/Condition.hpp"
#include "xaifBooster/system/inc/BranchAlgBase.hpp"
#include "xaifBooster/system/inc/ObjectWithLineNumber.hpp"

namespace xaifBooster { 

  /**
   * this class describes 
   * a preloop
   */
  class Branch : public ControlFlowGraphVertex,
		 public ObjectWithLineNumber {
  public:

    Branch ();

    ~Branch();

    /**
     * print XML hierarchy
     */
    void printXMLHierarchy(std::ostream& os) const;

    /**
     * print debug information
     */
    std::string debug() const ;

    /**
     * name for this class as represented in XAIF schema
     */
    static const std::string ourXAIFName;

    static const std::string our_myId_XAIFName;
    
    Condition& getCondition();

    const Condition& getCondition() const;

    /**
     * get algorithm
     */
    BranchAlgBase& getBranchAlgBase() const;

    virtual ControlFlowGraphVertexKind::ControlFlowGraphVertexKind_E getKind() const { return ControlFlowGraphVertexKind::BRANCH_VKIND;}

  private:
    
    /** 
     * the condition for the if statement
     */
    Condition myCondition;

  };
 
} // end of namespace xaifBooster
                                                                     
#endif
