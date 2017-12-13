#ifndef _XAIFBOOSTERBASICBLOCKPREACCUMULATION_BASICCONTROLFLOWGRAPHVERTEX_INCLUDE_
#define _XAIFBOOSTERBASICBLOCKPREACCUMULATION_BASICCONTROLFLOWGRAPHVERTEX_INCLUDE_
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

using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulation {  

  /** 
   * class to implement vertices that are used in
   * the BasicControlFlowGraph
   */
  class BasicControlFlowGraphVertex : public ControlFlowGraphVertex {

  public:
    
    BasicControlFlowGraphVertex(const ControlFlowGraphVertex*);
    BasicControlFlowGraphVertex();
    ~BasicControlFlowGraphVertex();

    const ControlFlowGraphVertexAlgBase&
    getOriginalControlFlowGraphVertexAlg() const;

    ControlFlowGraphVertexAlgBase&
    getOriginalControlFlowGraphVertexAlg();

    const ControlFlowGraphVertexAlgBase&
    getNewControlFlowGraphVertexAlg() const;

    virtual void printXMLHierarchy(std::ostream& os) const;
                                                                                
    virtual std::string debug() const ;

    virtual void traverseToChildren(const GenericAction::GenericAction_E anAction_c);

    int getIndex() const;
    void setIndex(int);

    ControlFlowGraphVertexKind::ControlFlowGraphVertexKind_E getKind() const;

    bool isOriginal() const { return original; }

    

    const ControlFlowGraphVertex& getOriginalVertex() const { 
      return *myOriginalVertex_p; 
    }

    ControlFlowGraphVertex& getNewVertex() { 
      return *myNewVertex_p; 
    }

    const ControlFlowGraphVertex& getNewVertex() const { 
      return *myNewVertex_p; 
    }

    void supplyAndAddNewVertex(ControlFlowGraphVertex& theNewVertex);

  private:

    /** 
     * no def
     */
    BasicControlFlowGraphVertex(const BasicControlFlowGraphVertex&);

    /** 
     * no def
     */
    BasicControlFlowGraphVertex& operator=(const BasicControlFlowGraphVertex&);

    /** 
     * indicates if this is a reference to an original ControlFlowGraphVertex
     * or if it got created as a new ControlFlowGraphVertex
     */
    bool original;

    /** 
     * pointer to original ControlFlowGraphVertex
     */
    const ControlFlowGraphVertex* myOriginalVertex_p;

    /** 
     * pointer to new ControlFlowGraphVertex
     * these vertices are never added to any graph 
     * themselves, we own them so we need to delete them
     */
    ControlFlowGraphVertex* myNewVertex_p;

    /** 
     * index for topological sort
     */
    int myIndex;

  };  // end of class

} // end of namespace 
                                                                     
#endif
