#ifndef _BASICBLOCK_INCLUDE_
#define _BASICBLOCK_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/FindDefinitionResult.hpp"
#include "xaifBooster/system/inc/PlainBasicBlock.hpp"
#include "xaifBooster/system/inc/BasicBlockAlgBase.hpp"
#include "xaifBooster/system/inc/ControlFlowGraphVertex.hpp"

namespace xaifBooster { 

  /**
   * representation for basic block with pointer to 
   * BasicBlockAlgBase
   */
  class BasicBlock : public PlainBasicBlock, public ControlFlowGraphVertex {
  public:
    
    BasicBlock (Scope& theScope,
		bool makeAlgorithm=true);

    ~BasicBlock();

    /**
     * calls virtual routine in BasicBlockAlgBase
     * that gets resolved to either printing the
     * actual expression using PlainBasicBlock::printXMLHierarchy
     * or to printing the transformed copy depending
     * on the algorithm used.
     */
    virtual void printXMLHierarchy(std::ostream& os) const;
    void printXMLHierarchyImpl(std::ostream& os) const;

    std::string debug() const ;

    /** 
     * get algorithm
     */
    BasicBlockAlgBase& getBasicBlockAlgBase() const;

    virtual void traverseToChildren(const GenericAction::GenericAction_E anAction_c); 

    virtual bool hasStatement(const ObjectWithId::Id& aStatementId) const; 

    /**
     * Searches for \p anExpression by first calling the PlainBasicBlock version of this method,
     * which checks each of the elements.  If it is not found there, it checks the associated algorithm (if any).
     */
    virtual bool hasExpression(const Expression& anExpression) const;

    /**
     * for aStatementId get the definition
     */
    virtual FindDefinitionResult findDefinition(const ObjectWithId::Id& aStatementId) const;

    virtual ControlFlowGraphVertexKind::ControlFlowGraphVertexKind_E getKind() const { return ControlFlowGraphVertexKind::BASICBLOCK_VKIND;}

  }; // end of class BasicBlock
 
} // end of namespace xaifBooster
                                                                     
#endif
