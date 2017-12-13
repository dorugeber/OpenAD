#ifndef _CONTROLFLOWGRAPHVERTEX_INCLUDE_
#define _CONTROLFLOWGRAPHVERTEX_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/utils/inc/XMLPrintable.hpp"
#include "xaifBooster/utils/inc/ObjectWithId.hpp"

#include "xaifBooster/system/inc/Expression.hpp"
#include "xaifBooster/system/inc/FindDefinitionResult.hpp"
#include "xaifBooster/system/inc/ObjectWithAnnotation.hpp"
#include "xaifBooster/system/inc/VertexTraversable.hpp"
#include "xaifBooster/system/inc/ForLoopReversalType.hpp"
#include "xaifBooster/system/inc/ControlFlowGraphVertexAlgBase.hpp"
#include "xaifBooster/system/inc/ControlFlowGraphVertexKind.hpp"
#include "xaifBooster/system/inc/Variable.hpp"

namespace xaifBooster { 

  class Assignment;

  /**
   * the ControlFlowGraphVertex is a base class for elements 
   * of a control flow such a if statements, basic blocks, loops etc.
   */
  class ControlFlowGraphVertex : public VertexTraversable, 
				 public XMLPrintable,
				 public ObjectWithId,
				 public ObjectWithAnnotation {
  public:

    /**
     * ctor
     */
    ControlFlowGraphVertex ();

    /** 
     * dtor
     */
    ~ControlFlowGraphVertex();

    /**
     * print XML hierarchy
     */
    virtual void printXMLHierarchy(std::ostream& os) const;
    virtual void printXMLHierarchyImpl(std::ostream& os) const;

    /**
     * print debug information
     */
    std::string debug() const ;


    /**
     * get algorithm
     */
    ControlFlowGraphVertexAlgBase& getControlFlowGraphVertexAlgBase() const;
    
    virtual bool hasStatement(const ObjectWithId::Id& aStatementId) const; 

    /**
     * checks the corresponding algorithm (if any) for \p anExpression
     */
    virtual bool hasExpression(const Expression& anExpression) const;

    /**
     * for aStatementId get the Assignment if it exists
     */
    virtual FindDefinitionResult  findDefinition(const ObjectWithId::Id& aStatementId) const;

    virtual ControlFlowGraphVertexKind::ControlFlowGraphVertexKind_E getKind() const = 0;

    ForLoopReversalType::ForLoopReversalType_E getReversalType() const; 

    void setReversalType(ForLoopReversalType::ForLoopReversalType_E aReversalType); 


    int getIndex() const;

    void setIndex(int);

    void setCounterPart(ControlFlowGraphVertex& theCounterPart);

    ControlFlowGraphVertex& getCounterPart();

    ControlFlowGraphVertex& getCounterPart() const;

    typedef std::list<const Variable*> VariablePList;

    const VariablePList& getKnownLoopVariables()const;
    void inheritLoopVariables(const ControlFlowGraphVertex& aParent);
    virtual void addLoopVariable(){} // overwritten in Loop vertices

    bool hasTopExplicitLoop() const;
    ControlFlowGraphVertex& getTopExplicitLoop();
    const ControlFlowGraphVertex& getTopExplicitLoop() const;
    void setTopExplicitLoop(ControlFlowGraphVertex& theTopExplicitLoop);


    bool hasEnclosingControlFlow() const;
    ControlFlowGraphVertex& getEnclosingControlFlow();
    const ControlFlowGraphVertex& getEnclosingControlFlow() const;
    void setEnclosingControlFlow(ControlFlowGraphVertex& theEnclosingControlFlow);

  protected:

    /**
     * this will be set to point a dynamically constructed instance
     * during construction and deleted during
     * destruction
     */
    ControlFlowGraphVertexAlgBase* myControlFlowGraphVertexAlgBase_p;

    /** 
     * the attribute indicating  
     * the reversal option as specified by 
     * a user directive for a top level loop
     */
    ForLoopReversalType::ForLoopReversalType_E myReversalType;

    /** 
     * index for topological sort
     */
    int myIndex;

    /** 
     * list of variable known to be loop variables
     * in loops for explicit reversal collected from 
     * the respective top loop down
     * we don't own the Variables we are pointing to
     */ 
    VariablePList myKnownLoopVariables; 

  private: 

    /** 
     * pointer to corresponding vertex for 
     * LOOP-ENDLOOP and BRANCH-ENDBRANCH pairs
     * is initialized during the topological sort
     */
    ControlFlowGraphVertex* myCounterPart_p;

    /** 
     * pointer to the top level 
     * explicit loop for explicit reversals
     * this is just a reference not to be deleted by 
     * the dtor
     */
    ControlFlowGraphVertex* myTopExplicitLoop_p;

    /** 
     * pointer to the enclosing Loop or Branch vertex
     * if it exists; only set for original vertices
     * this is just a reference not to be deleted by 
     * the dtor
     */ 
    ControlFlowGraphVertex* myEnclosingControlFlow_p;

  }; // end of class ControlFlowGraphVertex
 
} // end of namespace xaifBooster
                                                                     
#endif
