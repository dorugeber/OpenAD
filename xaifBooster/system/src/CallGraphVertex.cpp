// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include <sstream>
#include "xaifBooster/utils/inc/PrintManager.hpp"
#include "xaifBooster/system/inc/ConceptuallyStaticInstances.hpp"
#include "xaifBooster/system/inc/CallGraphVertex.hpp"
#include "xaifBooster/system/inc/CallGraphVertexAlgFactory.hpp"

namespace xaifBooster { 

  bool CallGraphVertex::ourInitializeDerivativeComponentsFlag=false;

  CallGraphVertex::CallGraphVertex(const Symbol& theSymbol,
				   const Scope& theScope,
				   const Scope& theCFGScope,
				   const bool activeFlag,
				   bool makeAlgorithm) : 
    myControlFlowGraph(theSymbol, theScope, theCFGScope, activeFlag),
    myCallGraphVertexAlgBase_p(0) { 
    if (makeAlgorithm)
      myCallGraphVertexAlgBase_p=CallGraphVertexAlgFactory::instance()->makeNewAlg(*this); 
  }

  CallGraphVertex::~CallGraphVertex() {
    if (myCallGraphVertexAlgBase_p)
      delete myCallGraphVertexAlgBase_p;
  } 

  const std::string&
  CallGraphVertex::getSubroutineName() const { 
    return myControlFlowGraph.getSymbolReference().getSymbol().getId(); 
  }

  CallGraphVertexAlgBase& 
  CallGraphVertex::getCallGraphVertexAlgBase() const { 
    if (!myCallGraphVertexAlgBase_p)
      THROW_LOGICEXCEPTION_MACRO("CallGraphVertex::getCallGraphVertexAlgBase: not set");
    return *myCallGraphVertexAlgBase_p;
  }

  void
  CallGraphVertex::printXMLHierarchy(std::ostream& os) const { 
    if (myCallGraphVertexAlgBase_p
	&& 
	! ConceptuallyStaticInstances::instance()->getPrintVersion()==PrintVersion::SYSTEM_ONLY)
      getCallGraphVertexAlgBase().printXMLHierarchy(os);
    else 
      myControlFlowGraph.printXMLHierarchy(os);
  } // end of CallGraphVertex::printXMLHierarchy

  void
  CallGraphVertex::printXMLHierarchyImpl(std::ostream& os) const { 
    myControlFlowGraph.printXMLHierarchy(os);
  } // end of CallGraphVertex::printXMLHierarchyImpl

  std::string CallGraphVertex::debug () const { 
    std::ostringstream out;
    out << "CallGraphVertex[" << this 
	<< Vertex::debug().c_str()
	<< ",myControlFlowGraph=" 
	<< myControlFlowGraph.debug().c_str()
	<< ","
	<< "myCallGraphVertexAlgBase_p"
	<< "="
	<< myCallGraphVertexAlgBase_p
	<< "]" << std::ends;  
    return out.str();
  } // end of CallGraphVertex::debug

  ControlFlowGraph&
  CallGraphVertex::getControlFlowGraph() { 
    return myControlFlowGraph;
  } // end of CallGraphVertex::getControlFlowGraph

  const ControlFlowGraph&
  CallGraphVertex::getControlFlowGraph() const { 
    return myControlFlowGraph;
  } // end of CallGraphVertex::getControlFlowGraph

  void CallGraphVertex::traverseToChildren(const GenericAction::GenericAction_E anAction_c) { 
    ConceptuallyStaticInstances::instance()->getTraversalStack().setCurrentCallGraphVertexInstance(*this);
    getCallGraphVertexAlgBase().genericTraversal(anAction_c);
    myControlFlowGraph.genericTraversal(anAction_c);
    ConceptuallyStaticInstances::instance()->getTraversalStack().resetCurrentCallGraphVertexInstance();
  } 

  void CallGraphVertex::setInitializeDerivativeComponentsFlag() {
    ourInitializeDerivativeComponentsFlag = true;
  }
  bool CallGraphVertex::getInitializeDerivativeComponentsFlag() const {
    return ourInitializeDerivativeComponentsFlag;
  }

} // end of namespace xaifBooster 
