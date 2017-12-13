#ifndef _XAIFBOOSTERLINEARIZATION_ASSIGNMENTALG_INCLUDE_
#define _XAIFBOOSTERLINEARIZATION_ASSIGNMENTALG_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/AssignmentAlgBase.hpp"
#include "xaifBooster/system/inc/Assignment.hpp"

#include "xaifBooster/algorithms/InlinableXMLRepresentation/inc/InlinableSubroutineCall.hpp"

using namespace xaifBooster; 

namespace xaifBoosterLinearization { 

  /**
   * class to implement linearization steps
   */
  class AssignmentAlg : public AssignmentAlgBase {

  public:
    
    AssignmentAlg(Assignment& theContainingAssignment);

    virtual ~AssignmentAlg(){};

    virtual void printXMLHierarchy(std::ostream& os) const;

    virtual std::string debug() const ;

    virtual void traverseToChildren(const GenericAction::GenericAction_E anAction_c);

    /** 
     * activity analysis 
     */
    virtual void algorithm_action_1();

    /** 
     * code generation
     */
    virtual void algorithm_action_2();

    /** 
     * statement local activity analysis
     * can be rerun
     * Note: if this discovers that an assignment is passive 
     * it may remove any associated code created.
     */
    void activityAnalysis();

    const Expression& getLinearizedRHS() const;

    bool hasLinearizedRHS() const;

    bool getActiveFlag() const; 

  private: 

    /** 
     * no def
     */
    AssignmentAlg();

    /** 
     * no def
     */
    AssignmentAlg(const AssignmentAlg&);

    /** 
     * no def
     */
    AssignmentAlg& operator=(const AssignmentAlg&);


    /**
     * constructs and then (XML) prints the actual assignment to the LHS,
     * which must occur after all assignments of subexpressions to auxiliaries.
     * intended to be called immediately before (during??) XML printing,
     * at which point we hopefully know exactly which subexpressions are assigned to aux variables
     */
    void printReplacementAssignmentSSA(std::ostream&) const;

    /** 
     * this indicates that the linearization has 
     * happened
     */
    bool myHaveLinearizedRHSFlag;

    /** 
     * this determines if the assignment is active or not
     * it has a delayed initialization from the active informantion 
     * if the containing assignments LHS.
     */
    mutable bool myActiveFlag;

    /** 
     * determines whether myActiveFlag has been initialized 
     */
    mutable bool myActiveFlagInit;

  };
 
} 
                                                                     
#endif
