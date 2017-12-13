#ifndef _CONCEPTUALLYSTATICINSTANCES_INCLUDE_
#define _CONCEPTUALLYSTATICINSTANCES_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/utils/inc/NameCreator.hpp"

#include "xaifBooster/system/inc/InlinableIntrinsicsCatalogue.hpp"
#include "xaifBooster/system/inc/NonInlinableIntrinsicsCatalogue.hpp"
#include "xaifBooster/system/inc/PrintVersion.hpp"
#include "xaifBooster/system/inc/TraversalStack.hpp"

namespace xaifBooster { 

  class CallGraph;

  /** 
   * class to contain 'globally static'
   * instances. This can later be extended 
   * to instances that are threadspecific 
   * if we decide to opt for a multithreaded 
   * implementation
   */
  class ConceptuallyStaticInstances { 
 
  public : 

    static ConceptuallyStaticInstances* instance();

    ~ConceptuallyStaticInstances();

    CallGraph& getCallGraph();

    InlinableIntrinsicsCatalogue& getInlinableIntrinsicsCatalogue();
    NonInlinableIntrinsicsCatalogue& getNonInlinableIntrinsicsCatalogue();

    void createCallGraph(const std::string& aSchemaInstance,
			 const std::string& anXAIFInstance,
			 const std::string& aSchemaLocation,
			 const std::string& aPrefix);

    /**
     * instantiated as "VIRTUAL" meaning 
     * printXMLHierarchy uses the normal virtual 
     * invocation mechanism
     * modifiable through setPrintVersion
     */
    PrintVersion::PrintVersion_E getPrintVersion() const;

    /**
     * set PrintVersion to either 
     * VIRTUAL or SYSTEM_ONLY
     */
    void setPrintVersion(PrintVersion::PrintVersion_E aPrintVersion);

    const NameCreator& getUniversalNameCreator() const;
    const NameCreator& getTemporaryVariableNameCreator() const;
    const NameCreator& getTypeChangeVariableNameCreator() const;
    const NameCreator& getDelayVariableNameCreator() const;
    const NameCreator& getAuxiliaryVariableNameCreator() const;
    const NameCreator& getLinearizationVariableNameCreator() const;
    const NameCreator& getAccumulationVariableNameCreator() const;
    const NameCreator& getPropagationVariableNameCreator() const;

    const TraversalStack& getTraversalStack() const;
    TraversalStack& getTraversalStack();

  private: 
    
    ConceptuallyStaticInstances();
    
    /// no def
    ConceptuallyStaticInstances(const ConceptuallyStaticInstances&);

    /// no def
    ConceptuallyStaticInstances& operator=(const ConceptuallyStaticInstances&);

    static ConceptuallyStaticInstances* ourInstance_p;
    
    /**
     * this class owns the CallGraph
     * it is created with createCallGraph
     * and deleted in the dtor
     */
    CallGraph* myCallGraph_p;

    InlinableIntrinsicsCatalogue myInlinableIntrinsicsCatalogue;
    NonInlinableIntrinsicsCatalogue myNonInlinableIntrinsicsCatalogue;

    /**
     * initialized to 
     * VIRTUAL 
     */
    PrintVersion::PrintVersion_E myPrintVersion;

    /// used to give unique names for variables created for some purpose not covered by those below
    NameCreator myUniversalNameCreator;

    /// used to give unique names to temporary variables
    NameCreator myTemporaryVariableNameCreator;

    /**
     * used to give unique names to variables created for type change
     * (see ****)
     */
    NameCreator myTypeChangeVariableNameCreator;

    /**
     * used to delay an assignment
     * (see xaifBoosterLinearization::AssignmentAlg::makeSSACodeList)
     */
    NameCreator myDelayVariableNameCreator;

    /**
     * used to give unique names to variables that are assigned the value of some RHS subexpression
     * for reasons including
     * - evaluation of a local partial, and
     * - taping as part of a push pop strategy
     */
    NameCreator myAuxiliaryVariableNameCreator;

    /// used to give unique names to variables that get assigned the value of a local partial during linearization
    NameCreator myLinearizationVariableNameCreator;

    /// used to give unique names to variables created for the propagation
    NameCreator myAccumulationVariableNameCreator;

    /// used to give unique names to variables created for the propagation
    NameCreator myPropagationVariableNameCreator;

    /**
     * the stack for pointers that allow one to access different parts of the AST
     */
    TraversalStack myTraversalStack;

  }; // end of ConceptuallyStaticInstances
  
} // end namespace xaifBooster

#endif 
