#ifndef _XAIFBOOSTERLINEARIZATION_EXPRESSIONALG_INCLUDE_
#define _XAIFBOOSTERLINEARIZATION_EXPRESSIONALG_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/utils/inc/ObjectWithId.hpp"

#include "xaifBooster/system/inc/ExpressionAlgBase.hpp"
#include "xaifBooster/system/inc/Argument.hpp"

#include "xaifBooster/algorithms/Linearization/inc/AssignmentAlg.hpp"

using namespace xaifBooster; 

namespace xaifBoosterLinearization { 

  /**
   * class to implement linearization steps
   */
  class ExpressionAlg : public ExpressionAlgBase {

  public:
    
    ExpressionAlg(Expression& theContainingExpression);

    virtual ~ExpressionAlg(){};

    virtual void printXMLHierarchy(std::ostream& os) const;

    virtual std::string debug() const ;

    virtual void traverseToChildren(const GenericAction::GenericAction_E anAction_c);

    /**
     * mark all passive vertices/edges bottom up
     */
    void activityAnalysis();

    /**
     * after the  activityAnalysis we can 
     * create the exprssions to calculate the partials
     */
    void createPartialExpressions();

    bool
    needsAuxiliaryExtraction() const;

    /// prints the aux assignments and their allocations (recursively, bottom-up)
    /**
     * goes from the maximal node straight down to the 'leaf' nodes without 
     * any modification and then searches back up to the first 
     * node requiring the creation of an auxiliary assignment.
     * It creates the top node of the auxiliary assignment
     * and then calls populateSSASubexpressionRecursively for each inedge
     * which in turn recursively builds the rest of the auxiliary assignment.
     * The required ordering is obtained via
     * a postorder traversal of the expression.
     */
    void printAuxiliaryAssignmentsSSA(const ObjectWithId::Id&,
                                      std::ostream&) const;

    /**
     * this method is called by printAuxiliaryAssignmentsSSA for each 
     * top node of a subexpression that needs a replacement assignment constructed,
     * and by the AssignmentAlg to produce the top-level replacement.
     * This method adds the respective
     * source for the input edge to the auxiliary assignment RHS expression, 
     * and the edge itself and recursively invokes itself for all 
     * input edges of the source vertex.
     */
    void populateSSASubexpressionRecursively(const ExpressionEdge&,
                                             Expression&,
                                             const ExpressionVertex&) const;

    /// initiates a recursive descent of printLocalPartialAssignmentsRecursively from the maximal vertex in the expression
    void printLocalPartialAssignments(std::ostream&) const;

  private: 

    /** 
     * no def
     */
    ExpressionAlg();

    /** 
     * no def
     */
    ExpressionAlg(const ExpressionAlg&);

    /** 
     * no def
     */
    ExpressionAlg& operator=(const ExpressionAlg&);

    /**
     * mark theVertex and all outgoing edges bottom up
     */
    void activityAnalysisBottomUpPass(const ExpressionVertex& theVertex);

    /**
     * mark all passive vertices/edges top down
     * we assume activityAnalysisBottomUpPass has run first
     */
    void activityAnalysisTopDownPass(const ExpressionVertex& theVertex);

    void
    extractAuxiliary(const ExpressionVertex&);

    void printAuxiliaryAssignmentsSSARecursively(const ExpressionVertex&,
                                                 const ObjectWithId::Id&,
                                                 std::ostream&) const;

    /**
     * currently prints bottom-up, which isn't technically necessary because
     * all of the local partial assignments follow all of the auxiliary assignments
     */
    void
    printLocalPartialAssignmentsRecursively(const ExpressionEdge&,
                                            std::ostream&) const;

    /// used to make unique statement ids for auxiliary assignments (see makeAuxiliaryAssignmentSSAId)
    mutable
    unsigned int myAuxiliaryAssignmentCounter;

    /// see myAuxiliaryAssignmentCounter
    std::string
    makeAuxiliaryAssignmentSSAId(const ObjectWithId::Id&) const; 

    /** 
     * to satisfy schema uniqueness constraints
     */
    static std::string makePartialAssignmentId(); 
    
  };
 
} 
                                                                     
#endif
