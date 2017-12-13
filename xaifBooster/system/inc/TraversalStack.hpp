#ifndef _TRAVERSALSTACK_INCLUDE_
#define _TRAVERSALSTACK_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/BasicBlock.hpp"
#include "xaifBooster/system/inc/CallGraphVertex.hpp"

namespace xaifBooster { 

  /**
   * (currently rudimentary) stack for storing reference points into the AST
   * so that progeny can have access to nodes that are higher up in the tree.
   * This process is triggered by some small additions to the definition of the traversal.
   * Currently contains only pointers to the BasicBlock and CallGraphVertex.
   */
  class TraversalStack : public Debuggable { 
  public:

    TraversalStack();
    ~TraversalStack();

    virtual std::string debug() const;

    void setCurrentCallGraphVertexInstance(CallGraphVertex& aCallGraphVertex);
    void resetCurrentCallGraphVertexInstance();
    const CallGraphVertex& getCurrentCallGraphVertexInstance() const;

    void setCurrentBasicBlockInstance(BasicBlock& aBasicBlock);
    void resetCurrentBasicBlockInstance();
    const BasicBlock& getCurrentBasicBlockInstance() const ;

  private:

    CallGraphVertex* myCurrentCallGraphVertexInstance_p;

    BasicBlock* myCurrentBasicBlockInstance_p;

    /**
     * no def
     */
    TraversalStack(const TraversalStack&);

    /**
     * no def
     */
    TraversalStack& operator=(const TraversalStack&);

  }; // end class TraversalStack

} // end namespace xaifBooster

#endif

