#ifndef _XAIFBOOSTERBASICBLOCKPREACCUMULATION_BASICCONTROLFLOWGRAPHEDGE_INCLUDE_
#define _XAIFBOOSTERBASICBLOCKPREACCUMULATION_BASICCONTROLFLOWGRAPHEDGE_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/EdgeTraversable.hpp"
#include "xaifBooster/system/inc/ControlFlowGraphEdge.hpp"

using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulation {  

  class BasicControlFlowGraph;

  /** class to implement edges that are used in the BasicControlFlowGraph
  */

  class BasicControlFlowGraphEdge : public ControlFlowGraphEdge {

  public:
    
    BasicControlFlowGraphEdge(const ControlFlowGraphEdge*);
    BasicControlFlowGraphEdge();
    ~BasicControlFlowGraphEdge();

    void printXMLHierarchy(std::ostream& os, const BasicControlFlowGraph&) const;
                                                                                
    virtual std::string debug() const ;

    virtual void traverseToChildren(const GenericAction::GenericAction_E anAction_c);

    bool isOriginal() const;

    const ControlFlowGraphEdge& getOriginalEdge() const;

    bool hasConditionValue() const;

    void setConditionValue(int);

    int getConditionValue() const;
    
  private:

    /** 
     * no def
     */
    BasicControlFlowGraphEdge(const BasicControlFlowGraphEdge&);

    /** 
     * no def
     */
    BasicControlFlowGraphEdge& operator=(const BasicControlFlowGraphEdge&);
    
    bool myConditionValueFlag;
    
    /**
     * if myConditionValueFlag is false this can be set
     * otherwise it has the value of the original edge
     * holds the condition value
     */
    int myConditionValue;

    /** 
     * pointer to original ControlFlowGraphEdge
     * the edge is owned by the original ControlFlowGraph
     */
    const ControlFlowGraphEdge* myOriginalEdge_p;

  };  // end of class

} // end of namespace 
                                                                     
#endif
