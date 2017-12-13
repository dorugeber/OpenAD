#ifndef _XAIFBOOSTERBASICBLOCKPREACCUMULATIONREVERSE_CALLGRAPHVERTEXALG_INCLUDE_
#define _XAIFBOOSTERBASICBLOCKPREACCUMULATIONREVERSE_CALLGRAPHVERTEXALG_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include <list>

#include "xaifBooster/utils/inc/MemCounter.hpp"

#include "xaifBooster/algorithms/CodeReplacement/inc/ReplacementList.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/PreaccumulationCounter.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulationReverse/inc/ReplacementId.hpp"

#include "xaifBooster/algorithms/PushPop/inc/CallGraphVertexAlg.hpp"

using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulationReverse {  

  /** 
   * class to implement algorithms relevant for ... 
   */
  class CallGraphVertexAlg : public xaifBoosterPushPop::CallGraphVertexAlg {
  public:
    
    CallGraphVertexAlg(CallGraphVertex& theContaining);

    virtual ~CallGraphVertexAlg();

    virtual void printXMLHierarchy(std::ostream& os) const;

    virtual std::string debug() const ;

    virtual void traverseToChildren(const GenericAction::GenericAction_E anAction_c);

    virtual void algorithm_action_4();

    static void checkPointToFiles();

    static void forceAllArgumentCheckpoints();
    
    /**
     * Sets flag to insert runtime conuters into the code.
     */
    static void setRuntimeCounters();

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

    xaifBoosterCodeReplacement::ReplacementList* myReplacementList_p;

    /** 
     * we own this
     */
    ControlFlowGraph* myCFGStoreArguments_p;

    /** 
     * we own this
     */
    ControlFlowGraph* myCFGStoreResults_p;

    /** 
     * we own this
     */
    ControlFlowGraph* myCFGRestoreArguments_p;

    /** 
     * we own this
     */
    ControlFlowGraph* myCFGRestoreResults_p;

    /** 
     * we own this
     */
    ControlFlowGraph* myCFGTimeStepStoreArguments_p;
    
    /** 
     * we own this
     */
    ControlFlowGraph* myCFGTimeStepRestoreArguments_p;

    /**
     * we own this
     */
    ControlFlowGraph* myCFGTimeStepStoreResults_p;

    /**
     * we own this
     */
    ControlFlowGraph* myCFGTimeStepRestoreResults_p;

    /**
     * static memory counter
     */
    MemCounter myMemCounter;

    /**
     * static preaccumulation counter
     */
    PreaccumulationCounter myPreaccumulationCounter;

    /**
     * make entry, exit and a basic block which is returned
     */
    BasicBlock& initCheckPointCFG(ControlFlowGraph& aCheckPointCFG); 

    /**
     * Says whether runtime counter information should be output
     */
    static bool runtimeCounters;

    xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall* theSubroutineCall_p;//IK
        PlainBasicBlock::BasicBlockElementList myBasicBlockElementList;//IK

    
    /** 
     * append type and shape strings to aSubroutineNameBase 
     * add the checkpointing calls to theBasicBlock
     */
    void handleCheckPointing(const std::string& aSubroutineNameBase,
			     SideEffectListType::SideEffectListType_E theSideEffectListType,
			     BasicBlock& theBasicBlock,
			     bool reverse, MemCounter& count, bool storeDerivatives);

    /** 
     * called by handleCheckPointing to deal with one argument
     */
    void handleCheckPoint(const std::string& aSubroutineNameBase,
			  BasicBlock& theBasicBlock,
			  const Variable& theVariable, MemCounter& count, bool storeDerivatives); 
    /** 
     * add the InlinableSubroutineCall with name  aSubroutineName
     * to theBasicBlock which pushes or pops to/from a Variable 
     * with name as specified by theSymbol and theScope
     */
    void addCheckPointingInlinableSubroutineCall(const std::string& aSubroutineName,
						 BasicBlock& theBasicBlock,
						 const Symbol& theSymbol,
						 const Scope& theScope, bool storeDerivatives);

    /** 
     * used to control the read order for 
     * checkpoints written to individual files, 
     * in which case it is easiest to read in the same
     * sequence as the data was written, as opposed 
     * to the stack model
     */
    static bool ourCheckPointToFilesFlag;

    /** 
     * used to control if checkpoints should 
     * forcibly be written for all formal arguments 
     * including ones for routines that are  never called
     * or only called with with constant actual arguments 
     */
    static bool ourForceAllArgCheckPointFlag;
    
  };
 
} // end of namespace xaifBoosterAngelInterfaceAlgorithms
                                                                     
#endif
