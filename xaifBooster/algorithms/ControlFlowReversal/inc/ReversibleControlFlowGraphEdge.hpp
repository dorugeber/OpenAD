#ifndef _XAIFBOOSTERCONTROLFLOWREVERSAL_REVERSIBLECONTROLFLOWGRAPHEDGE_INCLUDE_
#define _XAIFBOOSTERCONTROLFLOWREVERSAL_REVERSIBLECONTROLFLOWGRAPHEDGE_INCLUDE_
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

namespace xaifBoosterControlFlowReversal {  

  class ReversibleControlFlowGraph;

  class ReversibleControlFlowGraphEdge : public EdgeTraversable, 
					 public ObjectWithId {

  public:
    
    ReversibleControlFlowGraphEdge(const ControlFlowGraphEdge*);
    ReversibleControlFlowGraphEdge();
    ~ReversibleControlFlowGraphEdge();

    void printXMLHierarchy(std::ostream& os, const ReversibleControlFlowGraph&) const;
                                                                                
    virtual std::string debug() const ;

    virtual void traverseToChildren(const GenericAction::GenericAction_E anAction_c);

    bool isOriginal() const;

    const ControlFlowGraphEdge& getOriginalEdge() const;

    bool hasConditionValue() const;

    void setConditionValue(int);

    int getConditionValue() const;
    
    /** 
     * if the edge has a condition 
     * value of '1'
     */ 
    bool leadsToLoopBody() const;

    bool hasRevConditionValue() const;

    void setRevConditionValue(int);

    int getRevConditionValue() const;
    
  private:

    /** 
     * no def
     */
    ReversibleControlFlowGraphEdge(const ReversibleControlFlowGraphEdge&);

    /** 
     * no def
     */
    ReversibleControlFlowGraphEdge& operator=(const ReversibleControlFlowGraphEdge&);
    
    bool myConditionValueFlag;
    
    /**
     * if myConditionValueFlag is false this can be set
     * otherwise it has the value of the original edge
     * holds the condition value
     */
    int myConditionValue;

    bool myRevConditionValueFlag;
    
    /**
     * if myRevConditionValueFlag is false this can be set
     * it holds the marker value to be pushed and popped
     */
    int myRevConditionValue;

    /** 
     * pointer to original ControlFlowGraphEdge
     * the edge is owned by the original ControlFlowGraph
     */
    const ControlFlowGraphEdge* myOriginalEdge_p;

  };  // end of class

} // end of namespace 
                                                                     
#endif
