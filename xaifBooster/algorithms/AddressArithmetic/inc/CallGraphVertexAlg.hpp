#ifndef _XAIFBOOSTERADDRESSARITHMETIC_CALLGRAPHVERTEXALG_INCLUDE_
#define _XAIFBOOSTERADDRESSARITHMETIC_CALLGRAPHVERTEXALG_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/Expression.hpp"

#include "xaifBooster/algorithms/InlinableXMLRepresentation/inc/InlinableSubroutineCall.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulationTapeAdjoint/inc/CallGraphVertexAlg.hpp"
#include "xaifBooster/algorithms/ControlFlowReversal/inc/CallGraphVertexAlg.hpp"

using namespace xaifBooster;

namespace xaifBoosterAddressArithmetic {  

  /** 
   * class to implement algorithms relevant for the 
   * address arithmetic but so far only for 
   * the explicit reversal case. 
   * taping and restoring of non-explicit reversal
   * is handled in BasicBlockPreaccumulationTape and 
   * BasicBlockPreaccumulationTapeAdjoint resp.
   */
  class CallGraphVertexAlg : public xaifBoosterControlFlowReversal::CallGraphVertexAlg,
                             public xaifBoosterBasicBlockPreaccumulationTapeAdjoint::CallGraphVertexAlg {
  public:
    
    CallGraphVertexAlg(CallGraphVertex& theContaining);

    ~CallGraphVertexAlg();

    /**
     * fix addresses
     * this is for now just applicable 
     * to explicit loop reversal
     * it modifies the control flow 
     * by storing additional integers 
     * occuring in index expressions
     */
    virtual void algorithm_action_5();

    /**
     * TEMPORARY: sort out values violating
     * the simple loop assumption
     */
    virtual void algorithm_action_6();

    virtual std::string debug() const ;

    virtual void traverseToChildren(const GenericAction::GenericAction_E anAction_c);
    
    static void setUserDecides();
    
    static void setIgnorance();

    static void setTopLevelRoutine(const std::string& theName);

  private:
    
    /** 
     * no def
     */
    CallGraphVertexAlg();

    /** 
     * no def
     */
    CallGraphVertexAlg(const CallGraphVertexAlg&);

    /** 
     * no def
     */
    CallGraphVertexAlg& operator=(const CallGraphVertexAlg&);

    /**
     * information on the unknown variables
     */
    struct UnknownVarInfo { 
      /** 
       * the unknown variable, 
       * just a reference, do not delete
       */
      const Variable * myVariable_p;
      /** 
       * the vertex in which we refer to myVariable_p, 
       * just a reference, do not delete
       */
      const xaifBoosterControlFlowReversal::ReversibleControlFlowGraphVertex * myContainingVertex_p;
      /** 
       * the vertex determining the taping point (either the top level loop 
       * or a control flow vertex (not a basic block) under the top level loop
       * just a reference, do not delete
       */
      xaifBoosterControlFlowReversal::ReversibleControlFlowGraphVertex * myTapingVertex_p;
      /** 
       * how many references have we encountered
       */
      unsigned int myRefCount;
      /**
       * TEMPORARY: optional info who requires this variable
       */
      const BasicBlockElement* myRequiredBy_p;

      UnknownVarInfo() : 
	myVariable_p(0),
	myContainingVertex_p(0),
	myTapingVertex_p(0),
	myRefCount(0),
	myRequiredBy_p(0){ };
      UnknownVarInfo(const Variable& theVariable,
		     const xaifBoosterControlFlowReversal::ReversibleControlFlowGraphVertex& theContainingVertex,
		     xaifBoosterControlFlowReversal::ReversibleControlFlowGraphVertex& theTapingVertex) : 
	myVariable_p(&theVariable),
	myContainingVertex_p(&theContainingVertex),
	myTapingVertex_p(&theTapingVertex),
	myRefCount(0),
	myRequiredBy_p(0){ };

    }; 

    typedef std::list<UnknownVarInfo> UnknownVarInfoList;

    /** 
     * find variables used in anExpression that 
     * do not occur in theKnownVariables and 
     * put them in the theUnknownVariables
     */
    void findUnknownVariablesInExpression(const Expression& anExpression,
					  const ControlFlowGraphVertex::VariablePList& theKnownVariables,
					  UnknownVarInfoList& theUnknownVariables,
					  bool thisIsCF,
					  xaifBoosterControlFlowReversal::ReversibleControlFlowGraphVertex& theContainingVertex);

    /** 
     * find variables used in anArgument that 
     * do not occur in theKnownVariables and 
     * put them in the theUnknownVariables
     */
    void findUnknownVariablesInArgument(const Argument& anArgument,
                                        bool nonValueInquiryUse,
					const ControlFlowGraphVertex::VariablePList& theKnownVariables,
					UnknownVarInfoList& theUnknownVariables,
					bool thisIsCF,
					xaifBoosterControlFlowReversal::ReversibleControlFlowGraphVertex& theContainingVertex);

    /** 
     * find variables used in anArrayAccess
     */
    void findUnknownVariablesInArrayAccess(const ArrayAccess& anArrayAccess,
					   const ControlFlowGraphVertex::VariablePList& theKnownVariables,
					   UnknownVarInfoList& theUnknownVariables,
					   bool thisIsCF,
					   xaifBoosterControlFlowReversal::ReversibleControlFlowGraphVertex& theContainingVertex);

    /** 
     * find variables used in index expressions
     * occuring in the DerivativePropagator 
     * instances and subroutine calls for a given BasicBlock
     */
    void findUnknownIndexVariablesInBasicBlockElements(xaifBoosterControlFlowReversal::ReversibleControlFlowGraphVertex& theOriginalBasicBlock,
						       const ControlFlowGraphVertex::VariablePList& theKnownVariables,
						       UnknownVarInfoList& theUnknownVariables);

    /** 
     * find variables used in aReversibleControlFlowGraphVertex 
     */
    void findUnknownVariablesInReversibleControlFlowGraphVertex( xaifBoosterControlFlowReversal::ReversibleControlFlowGraphVertex& aReversibleControlFlowGraphVertex,
								 const ControlFlowGraphVertex::VariablePList& theKnownVariables,
								 UnknownVarInfoList& theUnknownVariables);

    /** 
     * push anUnknownVariable
     * with statement inserted into the storeplaceholder 
     * associated with aTapingVertex
     */
    void pushUnknownVariable(const Variable& anUnknownVariable,
			     xaifBoosterControlFlowReversal::ReversibleControlFlowGraphVertex& aTapingVertex);

    /** 
     * push all theUnknownVariables
     * with statements inserted into aBasicBlock_r
     */
    void pushUnknownVariables(const UnknownVarInfoList& theUnknownVariables);

    /** 
     * there is aPushCall
     * that we use to create a pop 
     * inserted into aBasicBlock_r
     */
    void createPopFromPush(const xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall& aPushCall,
			   BasicBlock& aBasicBlock_r);
    
    /** 
     * let the user decide individually, default false
     */
    static bool ourUserDecidesFlag;

    /** 
     * force ignorance (nothing gets pushed), default false
     */
    static bool ourIgnoranceFlag;

    /** 
     * the name of the top level routine 
     * which we refer to to test for quasi-constness of 
     * variables used in simple loop constructs
     */
    static std::string ourTopLevelRoutineName;

    /**
     * has ourTopLevelRoutineName been set
     */ 
    bool static haveTopLevelRoutineName();

    /**
     * defined in getTopLevelRoutine ;
     * we don't own the instance pointed to by
     * this attribute
     */
    static const CallGraphVertex* ourTopLevelRoutine_p;

    /** 
     * have we shown that this routine is called 
     * only under the top level routine
     */
    bool myOnlyUnderTopLevelRoutineFlag;

    /** 
     * determines if a this routine is called 
     * only under the top level routine
     */
    bool isOnlyUnderTopLevelRoutine(); 

    /**
     * gets the routine associated with ourTopLevelRoutineName
     */
    static const CallGraphVertex& getTopLevelRoutine();  

    /**
     * check if the variable is quasi-constant
     */
    bool isQuasiConstant(const Variable& theVariable);

    /**
     * list of unknown variables not resolved for explicit reversal
     */
    UnknownVarInfoList myExplicitReversalUnresolvedVariablesList;

    /**
     * TEMPORARY: handle required values not resolved in the simple loop case
     */
    void handleExplicitUnresolved();

  };  // end of class

} // end of namespace 
                                                                     
#endif
