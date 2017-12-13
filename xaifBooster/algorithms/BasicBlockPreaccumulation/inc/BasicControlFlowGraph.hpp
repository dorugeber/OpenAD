#ifndef _XAIFBOOSTERBASICBLOCKPREACCUMULATION_BASICCONTROLFLOWGRAPH_INCLUDE_
#define _XAIFBOOSTERBASICBLOCKPREACCUMULATION_BASICCONTROLFLOWGRAPH_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include <stack>
#include "xaifBooster/system/inc/GraphWrapperTraversable.hpp"
#include "xaifBooster/system/inc/ControlFlowGraph.hpp"
#include "xaifBooster/system/inc/Symbol.hpp"
#include "xaifBooster/system/inc/Assignment.hpp"
#include "xaifBooster/system/inc/BasicBlock.hpp"
#include "xaifBooster/algorithms/DerivativePropagator/inc/DerivativePropagator.hpp"
#include "xaifBooster/algorithms/InlinableXMLRepresentation/inc/InlinableSubroutineCall.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/BasicControlFlowGraphVertex.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/BasicControlFlowGraphEdge.hpp"

using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulation {  

  /** 
   * class to implement a control flow graph for basic block preaccumulation.
   * creates a framework for modifying the control flow graph while keeping a pointer 
   * to the original graph
   */
  class BasicControlFlowGraph : public GraphWrapperTraversable<BasicControlFlowGraphVertex,BasicControlFlowGraphEdge> {

  public:
    
    BasicControlFlowGraph(const ControlFlowGraph&);

    std::string makeUniqueVertexId();
    std::string makeUniqueEdgeId();

    const ControlFlowGraph& myOriginalGraph_r;
 
    std::list<ControlFlowGraphVertex*> mySortedVertices_p_l;

    void makeThisACopyOfOriginalControlFlowGraph();

    virtual void printXMLHierarchy(std::ostream& os) const;
                                                                                
    virtual std::string debug() const ;

    virtual void traverseToChildren(const GenericAction::GenericAction_E anAction_c);

    BasicControlFlowGraphVertex& getEntry();

    BasicControlFlowGraphVertex& getExit();
    /** 
     * insert a new basic block between after and before and return it
     */
    BasicBlock* insertBasicBlock();

    xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall& 
    addInlinableSubroutineCall(const std::string& aSubroutineName,BasicBlock* theBasicBlock);

    void addZeroDeriv(Variable& theTarget,BasicBlock* theBasicBlock);

    void initializeDerivComponents(BasicBlock* theBasicBlock);

    const std::string& getAlgorithmSignature() const;

  protected:

    /** 
     * no def
     */
    BasicControlFlowGraph();

    /** 
     * no def
     */
    BasicControlFlowGraph(const BasicControlFlowGraph&);

    /** 
     * no def
     */
    ControlFlowGraph& operator=(const ControlFlowGraph&);

    /**
     * signature used in annotations of objects generated
     * by the algorithm
     */
    static std::string myAlgorithmSignature;

  };  // end of class

} // end of namespace 
                                                                     
#endif
