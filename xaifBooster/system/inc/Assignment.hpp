#ifndef _ASSIGNMENT_INCLUDE_
#define _ASSIGNMENT_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include<list>

#include "xaifBooster/system/inc/BasicBlockElement.hpp"
#include "xaifBooster/system/inc/Expression.hpp"
#include "xaifBooster/system/inc/Variable.hpp"
#include "xaifBooster/system/inc/AssignmentAlgBase.hpp"
#include "xaifBooster/system/inc/ObjectWithLineNumber.hpp"

namespace xaifBooster { 

  /**
   * representation for an Assignment 
   */
  class Assignment : public BasicBlockElement,
		     public ObjectWithLineNumber {
  public:
    
    /** 
     * \param makeAlgorithm  news up an algorithm object if required
     * this is also carried through for the respective members
     */
    Assignment (bool makeAlgorithm=true);

    ~Assignment();

    /** 
     * algorithm access where the Assignment may 
     * be const but in difference to the 
     * internal representation (wich is always 
     * const for the algorithms) the algorithm 
     * instances will always be modifiable.
     */
    AssignmentAlgBase& getAssignmentAlgBase()const;

    virtual void printXMLHierarchy(std::ostream& os) const;

    /**
     * actual implementation for printing xaif
     */
    void printXMLHierarchyImpl(std::ostream& os) const;

    /**
     * unparse the left hand side only
     */
    void printXMLHierarchyLHS(std::ostream& os) const;

    /**
     * unparse the right hand side only
     */
    void printXMLHierarchyRHS(std::ostream& os) const;

    std::string debug() const;

    Variable& getLHS();

    Expression& getRHS();

    const Variable& getLHS() const;

    const Expression& getRHS() const;

    static const std::string ourXAIFName;
    static const std::string our_myId_XAIFName;
    static const std::string our_myLHS_XAIFName;
    static const std::string our_myRHS_XAIFName;
    static const std::string our_myDoMapKey_XAIFName;

    virtual void traverseToChildren(const GenericAction::GenericAction_E anAction_c);

    StatementIdSetMapKey& getDoMapKey();

    const StatementIdSetMapKey& getDoMapKey() const;

    /**
     * this assignment contains \p anExpression if and only if it is in the RHS
     * or it lies somewhere inside the array access of the LHS.
     * In addition, we also check the corresponding AssignmentAlg
     */
    virtual bool hasExpression(const Expression& anExpression) const;
    
    /**
     * @return true if this is a non-inlinable statment 
     * the required format for the RHS is the occurence of a single 
     * non-inlinable intrinsic with arguments being variable references, 
     */
    bool isNonInlinable() const;

  private: 

    /**
     * no def
     */
    Assignment(const Assignment&);

    /**
     * no def
     */
    Assignment& operator=(const Assignment&);

    /**
     * left hand side 
     */
    Variable myLHS;

    /** 
     * right hand side
     */
    Expression myRHS;

    /** 
     * key into  DoMap
     */
    StatementIdSetMapKey myDoMapKey;

    /**
     * is this the special format for a non-inlinable intrinsic?
     */
    mutable bool myNonInlinableFlag;

    /**
     * have we checked for non-inlinable
     */
    mutable bool myNonInlinableCheckedFlag;

  }; // end of class Assignment

  typedef std::list<const Assignment*> CAssignmentPList;

  typedef std::list<Assignment*> AssignmentPList;
 
 
} // end of namespace xaifBooster
                                                                     
#endif
