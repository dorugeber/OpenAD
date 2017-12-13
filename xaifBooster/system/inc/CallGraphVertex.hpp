#ifndef _CALLGRAPHVERTEX_INCLUDE_
#define _CALLGRAPHVERTEX_INCLUDE_
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
#include "xaifBooster/system/inc/VertexTraversable.hpp"
#include "xaifBooster/system/inc/ControlFlowGraph.hpp"
#include "xaifBooster/system/inc/CallGraphVertexAlgBase.hpp"

namespace xaifBooster { 

  /** 
   * A CallGraphVertex represents the target of a call 
   * in a code represented by XAIF. 
   * One can think of subroutines as being represented 
   * as vertices in the call graph. 
   */
  class CallGraphVertex : public VertexTraversable, 
			  public XMLPrintable{
  public:

    CallGraphVertex (const Symbol& theSymbol,
		     const Scope& theScope,
		     const Scope& theCFGScope,
		     const bool activeFlag,
		     bool makeAlgorithm=true);

    ~CallGraphVertex();

    void printXMLHierarchy(std::ostream& os) const;

    void printXMLHierarchyImpl(std::ostream& os) const;

    std::string debug() const ;

    /**
     * returns a reference to the ControlFlowGraph
     */
    ControlFlowGraph& getControlFlowGraph();

    /**
     * returns a reference to the ControlFlowGraph
     */
    const ControlFlowGraph& getControlFlowGraph() const;

    /**
     * override default behavior
     */
    virtual void traverseToChildren(const GenericAction::GenericAction_E anAction_c); 

    static void setInitializeDerivativeComponentsFlag();
    
    bool getInitializeDerivativeComponentsFlag() const;

    /** 
     * algorithm access where the CallGraphVertex may 
     * be const but in difference to the 
     * internal representation (wich is always 
     * const for the algorithms) the algorithm 
     * instances will always be modifiable.
     */
    CallGraphVertexAlgBase& getCallGraphVertexAlgBase()const;

    /** 
     * get name of subroutine associated with myControlFlowGraph
     */
    const std::string& getSubroutineName() const;

  private: 

    /**
     * no def
     */
    CallGraphVertex ();

    /** each call graph has a control flow 
     * graph 
     * @see ControlFlowGraph
     */
    ControlFlowGraph myControlFlowGraph;

    /** 
     * this will be set to point a dynamically instance
     * during construction and deleted during 
     * destruction
     */
    CallGraphVertexAlgBase* myCallGraphVertexAlgBase_p;

    static bool ourInitializeDerivativeComponentsFlag;

  }; // end of class CallGraphVertex
 
} // end of namespace xaifBooster
                                                                     
#endif
