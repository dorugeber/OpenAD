#ifndef _PLAINBASICBLOCK_INCLUDE_
#define _PLAINBASICBLOCK_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

//UN #include "xaifBooster/system/inc/ControlFlowGraphVertex.hpp"
#include <list>

#include "xaifBooster/system/inc/Expression.hpp"
#include "xaifBooster/system/inc/BasicBlockElement.hpp"
#include "xaifBooster/system/inc/Scope.hpp"

namespace xaifBooster { 

  /**
   * class PlainBasicBlock describes a block of statements.
   * I.e. think of statements within {}
   */
  // UNclass PlainBasicBlock : public ControlFlowGraphVertex {
  class PlainBasicBlock {
  public:
    
    PlainBasicBlock (Scope& theScope);

    virtual ~PlainBasicBlock();

    std::string debug() const;

    /**
     * name for this class as represented in XAIF schema
     */
    static const std::string ourXAIFName;

    /**
     * name for inherited member myId as represented in XAIF schema
     */
    static const std::string our_myId_XAIFName;

    /**
     * name for member myScopeId as represented in XAIF schema
     */
    static const std::string our_myScopeId_XAIFName;

    /**
     * adds an element  to the basic block,
     * however the element instance has to 
     * be DYNAMICALLY ALLOCATED and the 
     * OWNERSHIP IS RELINQUISHED to the basic block.
     */
    void supplyAndAddBasicBlockElementInstance(BasicBlockElement& theBasicBlockElement);
    void supplyAndAddBasicBlockElementInstanceAtBeginning(BasicBlockElement& theBasicBlockElement);
    
    typedef std::list<BasicBlockElement*> BasicBlockElementList;
    
    const BasicBlockElementList& getBasicBlockElementList() const;

    BasicBlockElementList& getBasicBlockElementList();

    /**
     * get the scope, here not constant yet
     * because right now we don't have 
     * a derive ScopeAlg object but rather add 
     * new symbols right away in the original scope
     * which is contrary to the rest of the design 
     * where we keep the original constant... 
     * \todo JU: revisit the const Scope issue
     */
    Scope& getScope() const;

    /**
     * Checks each element in the element list for \p anExpression.
     * We don't check the respective algorithm because this class has none
     */ 
    virtual bool hasExpression(const Expression& anExpression) const;

  protected:
    
    /**
     * the (ordered) list of basic block elements
     * which are instances of the derived classes 
     * assignment, subroutine call etc..
     * we own the elements in this list and delete them
     * at destruction time
     */
    BasicBlockElementList myElementList;

    /**
     * the scope as specified by the scope id
     */
    Scope& myScope_r;

  }; // end of class PlainBasicBlock
 
} // end of namespace xaifBooster
                                                                     
#endif
