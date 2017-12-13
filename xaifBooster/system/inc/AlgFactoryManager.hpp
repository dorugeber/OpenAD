#ifndef _ALGFACTORYMANAGER_INCLUDE_
#define _ALGFACTORYMANAGER_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/utils/inc/Mutex.hpp"
#include "xaifBooster/utils/inc/Debuggable.hpp"

/** 
 * to be used in in all AlgFactoryManager declarations
 */
#define ALG_CONFIG_ACCESS_DECL_MACRO				\
  AlgConfig* makeAlgConfig(int argc,				\
			   char** argv,				\
			   const std::string& buildStamp);	\
								\
  AlgConfig* getAlgConfig();

/** 
 * to be used in in all AlgFactoryManager definitions
 */
#define ALG_CONFIG_ACCESS_DEF_MACRO					\
  AlgConfig* AlgFactoryManager::makeAlgConfig(int argc,			\
					      char** argv,		\
					      const std::string& buildStamp) { \
    if (ourAlgConfig_p)							\
      THROW_LOGICEXCEPTION_MACRO("AlgFactoryManager::makeAlgConfig(): already made"); \
    return dynamic_cast<AlgConfig*>(ourAlgConfig_p=new AlgConfig(argc,argv,buildStamp)); \
  }									\
									\
  AlgConfig* AlgFactoryManager::getAlgConfig() {			\
    if (!ourAlgConfig_p)						\
      THROW_LOGICEXCEPTION_MACRO("AlgFactoryManager::getAlgConfig(): not set"); \
    return dynamic_cast<AlgConfig*>(ourAlgConfig_p);			\
  } 

namespace xaifBooster { 

  class ArgumentAlgFactory;
  class ArgumentSymbolReferenceAlgFactory;
  class AssignmentAlgFactory;
  class BasicBlockAlgFactory;
  class BooleanOperationAlgFactory;
  class BranchAlgFactory;
  class CallGraphAlgFactory;
  class CallGraphVertexAlgFactory;
  class ConcreteArgumentAlgFactory;
  class ConstantAlgFactory;
  class ControlFlowGraphAlgFactory;
  class ControlFlowGraphVertexAlgFactory;
  class EndBranchAlgFactory;
  class EndLoopAlgFactory;
  class EntryAlgFactory;
  class ExitAlgFactory;
  class ExpressionAlgFactory;
  class ExpressionEdgeAlgFactory;
  class ForLoopAlgFactory;
  class GotoAlgFactory;
  class IfStatementAlgFactory;
  class IntrinsicAlgFactory;
  class LabelAlgFactory;
  class MarkerAlgFactory;
  class PreLoopAlgFactory;
  class SubroutineCallAlgFactory;
  class SymbolAlgFactory;

  class AlgConfig;

  /** 
   * the singleton class for 
   * setting algorithm factory pointers
   * and the config instance
   */
  class AlgFactoryManager : public Debuggable { 

  public: 

    static AlgFactoryManager* instance();

    /** 
     * uses reset and the proper 
     * settings from the respective 
     * parent manager(s)
     */
    virtual void init(); 

    /** 
     * set a selection 
     * of factories 
     * overwriting earlier settings 
     * using the reset methods. 
     */
    virtual void resets(); 

    ArgumentAlgFactory* getArgumentAlgFactory() const ;
    ArgumentSymbolReferenceAlgFactory* getArgumentSymbolReferenceAlgFactory() const ;
    AssignmentAlgFactory* getAssignmentAlgFactory() const ;
    BasicBlockAlgFactory* getBasicBlockAlgFactory() const ;
    BooleanOperationAlgFactory* getBooleanOperationAlgFactory() const ;
    BranchAlgFactory* getBranchAlgFactory() const ;
    CallGraphAlgFactory* getCallGraphAlgFactory() const ;
    CallGraphVertexAlgFactory* getCallGraphVertexAlgFactory() const ;
    ConcreteArgumentAlgFactory* getConcreteArgumentAlgFactory() const ;
    ConstantAlgFactory* getConstantAlgFactory() const ;
    ControlFlowGraphAlgFactory* getControlFlowGraphAlgFactory() const ;
    ControlFlowGraphVertexAlgFactory* getControlFlowGraphVertexAlgFactory() const ;
    EndBranchAlgFactory* getEndBranchAlgFactory() const ;
    EndLoopAlgFactory* getEndLoopAlgFactory() const ;
    EntryAlgFactory* getEntryAlgFactory() const ;
    ExitAlgFactory* getExitAlgFactory() const ;
    ExpressionAlgFactory* getExpressionAlgFactory() const ;
    ExpressionEdgeAlgFactory* getExpressionEdgeAlgFactory() const ;
    ForLoopAlgFactory* getForLoopAlgFactory() const ;
    GotoAlgFactory* getGotoAlgFactory() const ;
    IfStatementAlgFactory* getIfStatementAlgFactory() const ;
    IntrinsicAlgFactory* getIntrinsicAlgFactory() const ;
    LabelAlgFactory* getLabelAlgFactory() const ;
    MarkerAlgFactory* getMarkerAlgFactory() const ;
    PreLoopAlgFactory* getPreLoopAlgFactory() const ;
    SubroutineCallAlgFactory* getSubroutineCallAlgFactory() const ;
    SymbolAlgFactory* getSymbolAlgFactory() const ;

    virtual std::string debug() const; 

    ALG_CONFIG_ACCESS_DECL_MACRO

  protected: 
    
    void resetArgumentAlgFactory(ArgumentAlgFactory*);
    void resetArgumentSymbolReferenceAlgFactory(ArgumentSymbolReferenceAlgFactory*);
    void resetAssignmentAlgFactory(AssignmentAlgFactory*);
    void resetBasicBlockAlgFactory(BasicBlockAlgFactory*);
    void resetBooleanOperationAlgFactory(BooleanOperationAlgFactory*);
    void resetBranchAlgFactory(BranchAlgFactory*);
    void resetCallGraphAlgFactory(CallGraphAlgFactory*);
    void resetCallGraphVertexAlgFactory(CallGraphVertexAlgFactory*);
    void resetConcreteArgumentAlgFactory(ConcreteArgumentAlgFactory*);
    void resetConstantAlgFactory(ConstantAlgFactory*);
    void resetControlFlowGraphAlgFactory(ControlFlowGraphAlgFactory*);
    void resetControlFlowGraphVertexAlgFactory(ControlFlowGraphVertexAlgFactory*);
    void resetEndBranchAlgFactory(EndBranchAlgFactory*);
    void resetEndLoopAlgFactory(EndLoopAlgFactory*);
    void resetEntryAlgFactory(EntryAlgFactory*);
    void resetExitAlgFactory(ExitAlgFactory*);
    void resetExpressionAlgFactory(ExpressionAlgFactory*);
    void resetExpressionEdgeAlgFactory(ExpressionEdgeAlgFactory*);
    void resetForLoopAlgFactory(ForLoopAlgFactory*);
    void resetGotoAlgFactory(GotoAlgFactory*);
    void resetIfStatementAlgFactory(IfStatementAlgFactory*);
    void resetIntrinsicAlgFactory(IntrinsicAlgFactory*);
    void resetLabelAlgFactory(LabelAlgFactory*);
    void resetMarkerAlgFactory(MarkerAlgFactory*);
    void resetPreLoopAlgFactory(PreLoopAlgFactory*);
    void resetSubroutineCallAlgFactory(SubroutineCallAlgFactory*);
    void resetSymbolAlgFactory(SymbolAlgFactory*);
    
    AlgFactoryManager();

    virtual ~AlgFactoryManager();

    static AlgFactoryManager* ourInstance_p;

    /**
     * the mutex to protect the instance
     */
    static Mutex ourInstanceMutex;

    /** 
     * to hold the quasi-static (because the Manager is a singleton)
     * configuration instance
     */
    AlgConfig* ourAlgConfig_p;

  private: 

    ArgumentAlgFactory* ourArgumentAlgFactory_p;
    ArgumentSymbolReferenceAlgFactory* ourArgumentSymbolReferenceAlgFactory_p;
    AssignmentAlgFactory* ourAssignmentAlgFactory_p;
    BasicBlockAlgFactory* ourBasicBlockAlgFactory_p;
    BooleanOperationAlgFactory* ourBooleanOperationAlgFactory_p;
    BranchAlgFactory* ourBranchAlgFactory_p;
    CallGraphAlgFactory* ourCallGraphAlgFactory_p;
    CallGraphVertexAlgFactory* ourCallGraphVertexAlgFactory_p;
    ConcreteArgumentAlgFactory* ourConcreteArgumentAlgFactory_p;
    ConstantAlgFactory* ourConstantAlgFactory_p;
    ControlFlowGraphAlgFactory* ourControlFlowGraphAlgFactory_p;
    ControlFlowGraphVertexAlgFactory* ourControlFlowGraphVertexAlgFactory_p;
    EndBranchAlgFactory* ourEndBranchAlgFactory_p;
    EndLoopAlgFactory* ourEndLoopAlgFactory_p;
    EntryAlgFactory* ourEntryAlgFactory_p;
    ExitAlgFactory* ourExitAlgFactory_p;
    ExpressionAlgFactory* ourExpressionAlgFactory_p;
    ExpressionEdgeAlgFactory* ourExpressionEdgeAlgFactory_p;
    ForLoopAlgFactory* ourForLoopAlgFactory_p;
    GotoAlgFactory* ourGotoAlgFactory_p;
    IfStatementAlgFactory* ourIfStatementAlgFactory_p;
    IntrinsicAlgFactory* ourIntrinsicAlgFactory_p;
    LabelAlgFactory* ourLabelAlgFactory_p;
    MarkerAlgFactory* ourMarkerAlgFactory_p;
    PreLoopAlgFactory* ourPreLoopAlgFactory_p;
    SubroutineCallAlgFactory* ourSubroutineCallAlgFactory_p;
    SymbolAlgFactory* ourSymbolAlgFactory_p;

    /**
     * no def
     */
    AlgFactoryManager(const AlgFactoryManager&);

    /**
     * no def
     */
    AlgFactoryManager& operator=(const AlgFactoryManager&);

  }; // end of class AlgFactoryManager

}

#endif
