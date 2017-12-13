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
#include "xaifBooster/utils/inc/DbgLoggerManager.hpp"

#include "xaifBooster/system/inc/GraphVizDisplay.hpp"
#include "xaifBooster/system/inc/CallGraphVertex.hpp"

#include "xaifBooster/algorithms/CodeReplacement/inc/Replacement.hpp"

#include "xaifBooster/algorithms/TypeChange/inc/SymbolAlg.hpp"
#include "xaifBooster/algorithms/TraceDiff/inc/CallGraphVertexAlg.hpp"


using namespace xaifBooster;

namespace xaifBoosterTraceDiff { 

  CallGraphVertexAlg::CallGraphVertexAlg(CallGraphVertex& theContaining) : 
    CallGraphVertexAlgBase(theContaining),
    xaifBoosterControlFlowReversal::CallGraphVertexAlg(theContaining),
    myTracingControlFlowGraph_p(0),
    myReplacementList_p(0) {
  }

  CallGraphVertexAlg::~CallGraphVertexAlg() {
    if (myTracingControlFlowGraph_p) delete myTracingControlFlowGraph_p;
    if (myReplacementList_p) delete myReplacementList_p;
  }

  xaifBoosterControlFlowReversal::ReversibleControlFlowGraph&
  CallGraphVertexAlg::getTracingControlFlowGraph() {
    if (!myTracingControlFlowGraph_p) 
      THROW_LOGICEXCEPTION_MACRO("CallGraphVertexAlg::getTracingControlFlowGraph: not set");
    return *myTracingControlFlowGraph_p;
  }
                                                                                
  const xaifBoosterControlFlowReversal::ReversibleControlFlowGraph&
  CallGraphVertexAlg::getTracingControlFlowGraph() const {
    if (!myTracingControlFlowGraph_p) 
      THROW_LOGICEXCEPTION_MACRO("CallGraphVertexAlg::getTracingControlFlowGraph: not set");
    return *myTracingControlFlowGraph_p;
  }
                                                                                
  class ControlFlowGraphVertexLabelWriter {
  public:
    ControlFlowGraphVertexLabelWriter(const xaifBoosterControlFlowReversal::ReversibleControlFlowGraph& g) : myG(g) {}
    template <class BoostIntenalVertexDescriptor>
    void operator()(std::ostream& out, const BoostIntenalVertexDescriptor& v) const {
      xaifBoosterControlFlowReversal::ReversibleControlFlowGraphVertex* theReversibleControlFlowGraphVertex_p=boost::get(boost::get(BoostVertexContentType(),myG.getInternalBoostGraph()),v);
      std::string theVertexKind;
      std::string theIdInfo; // <xaif id>[s<store placeholder>][r<restore placeholder>][t<top level explicit loop>][e<enclosing control flow start>]
      if (theReversibleControlFlowGraphVertex_p->isOriginal()) {
	const xaifBoosterControlFlowReversal::ControlFlowGraphVertexAlg& va(dynamic_cast<const xaifBoosterControlFlowReversal::ControlFlowGraphVertexAlg&>(theReversibleControlFlowGraphVertex_p->getOriginalVertex().getControlFlowGraphVertexAlgBase()));
        theVertexKind=va.kindToString();
	const ControlFlowGraphVertex& v(dynamic_cast<const ControlFlowGraphVertex&>(theReversibleControlFlowGraphVertex_p->getOriginalVertex()));
        theIdInfo=v.getId();
	if (theReversibleControlFlowGraphVertex_p->hasStorePlaceholder())
	  theIdInfo+="s"+theReversibleControlFlowGraphVertex_p->getStorePlaceholder().getNewVertex().getId();
	if (theReversibleControlFlowGraphVertex_p->hasRestorePlaceholder())
	  theIdInfo+="r"+theReversibleControlFlowGraphVertex_p->getRestorePlaceholder().getNewVertex().getId();
      }
      else {
	const xaifBoosterControlFlowReversal::ControlFlowGraphVertexAlg& va(dynamic_cast<const xaifBoosterControlFlowReversal::ControlFlowGraphVertexAlg&>(theReversibleControlFlowGraphVertex_p->getNewVertex().getControlFlowGraphVertexAlgBase()));
        theVertexKind=va.kindToString();
	const ControlFlowGraphVertex& v(dynamic_cast<const ControlFlowGraphVertex&>(theReversibleControlFlowGraphVertex_p->getNewVertex()));
        theIdInfo=v.getId();
      }
      if (theReversibleControlFlowGraphVertex_p->getReversalType()==ForLoopReversalType::EXPLICIT) { 
	theIdInfo+="t"+theReversibleControlFlowGraphVertex_p->getTopExplicitLoop().getOriginalVertex().getId();
      }
      if (theReversibleControlFlowGraphVertex_p->hasEnclosingControlFlow()) { 
	theIdInfo+="e"+theReversibleControlFlowGraphVertex_p->getEnclosingControlFlow().getOriginalVertex().getId();
      }
      out << "[label=\"" << boost::get(boost::get(BoostVertexContentType(), myG.getInternalBoostGraph()), v)->getIndex() << " (" << theIdInfo.c_str() << "): " << theVertexKind.c_str() << "\"]";
    }
    const xaifBoosterControlFlowReversal::ReversibleControlFlowGraph& myG;
  };

  class ControlFlowGraphEdgeLabelWriter {
  public:
    ControlFlowGraphEdgeLabelWriter(const xaifBoosterControlFlowReversal::ReversibleControlFlowGraph& g) : myG(g) {}
    template <class BoostIntenalEdgeDescriptor>
    void operator()(std::ostream& out, const BoostIntenalEdgeDescriptor& v) const {
      xaifBoosterControlFlowReversal::ReversibleControlFlowGraphEdge* theReversibleControlFlowGraphEdge_p=boost::get(boost::get(BoostEdgeContentType(),myG.getInternalBoostGraph()),v);
      if (theReversibleControlFlowGraphEdge_p->hasConditionValue() ||
	  theReversibleControlFlowGraphEdge_p->hasRevConditionValue()) { 
	out << "[label=\"";
	if (theReversibleControlFlowGraphEdge_p->hasConditionValue())
	  out << theReversibleControlFlowGraphEdge_p->getConditionValue();
	if (theReversibleControlFlowGraphEdge_p->hasRevConditionValue())
	  out << "r" << theReversibleControlFlowGraphEdge_p->getRevConditionValue();
	out << "\"]";
      }
    }
    const xaifBoosterControlFlowReversal::ReversibleControlFlowGraph& myG;
  };

  void CallGraphVertexAlg::algorithm_action_2() {
    DBG_MACRO(DbgGroup::CALLSTACK,
              "xaifBoosterControlFlowReversal::CallGraphVertexAlg::algorithm_action_2(tracing) called for: "
              << debug().c_str());
    if (getContaining().getControlFlowGraph().numVertices()) { 
      // for instance modules may have an empty CallGraph
      myTracingControlFlowGraph_p=new TraceableControlFlowGraph(getContaining().getControlFlowGraph());
      myTracingControlFlowGraph_p->makeThisACopyOfOriginalControlFlowGraph();
      if (DbgLoggerManager::instance()->isSelected(DbgGroup::GRAPHICS) && DbgLoggerManager::instance()->wantTag("")) {     
    	GraphVizDisplay::show(*myTracingControlFlowGraph_p,"cfg_copy", ControlFlowGraphVertexLabelWriter(*myTracingControlFlowGraph_p),ControlFlowGraphEdgeLabelWriter(*myTracingControlFlowGraph_p));
      }
      myTracingControlFlowGraph_p->markBranchExitEdges();
      if (DbgLoggerManager::instance()->isSelected(DbgGroup::GRAPHICS) && DbgLoggerManager::instance()->wantTag("")) {     
	GraphVizDisplay::show(*myTracingControlFlowGraph_p,"cfg_branch_marked", ControlFlowGraphVertexLabelWriter(*myTracingControlFlowGraph_p),ControlFlowGraphEdgeLabelWriter(*myTracingControlFlowGraph_p));
      }
      myTracingControlFlowGraph_p->traceActiveControlFlow();
      if (DbgLoggerManager::instance()->isSelected(DbgGroup::GRAPHICS) && DbgLoggerManager::instance()->wantTag("")) {     
	GraphVizDisplay::show(*myTracingControlFlowGraph_p,"cfg_tracing", ControlFlowGraphVertexLabelWriter(*myTracingControlFlowGraph_p),ControlFlowGraphEdgeLabelWriter(*myTracingControlFlowGraph_p));
      }
    }
    const xaifBoosterTypeChange::SymbolAlg& 
      theSymbolAlg(dynamic_cast<const xaifBoosterTypeChange::SymbolAlg&>(getContaining().
									 getControlFlowGraph().
									 getSymbolReference().
									 getSymbol().
									 getSymbolAlgBase()));
    const SymbolReference* theSymbolReference_p;
    if (theSymbolAlg.hasReplacementSymbolReference()) 
      theSymbolReference_p=&(theSymbolAlg.getReplacementSymbolReference());
    else
      theSymbolReference_p=&(getContaining().getControlFlowGraph().getSymbolReference());
    // make the replacement list
    if (!myReplacementList_p) 
      myReplacementList_p=
	new xaifBoosterCodeReplacement::ReplacementList(theSymbolReference_p->getSymbol(),
							theSymbolReference_p->getScope(),
							getContaining().getControlFlowGraph().getScope(),
							"tracing_subroutine_template",
							getContaining().getControlFlowGraph().getArgumentList());
    myReplacementList_p->setAnnotation(getContaining().getControlFlowGraph().getAnnotation());
    myReplacementList_p->setId(getContaining().getControlFlowGraph().getId());
    // fill in replacements:
    xaifBoosterCodeReplacement::Replacement& originalCFG(myReplacementList_p->addReplacement(1)); 
    originalCFG.setControlFlowGraphBase(getContaining().getControlFlowGraph());
    originalCFG.setPrintVersion(xaifBoosterCodeReplacement::PrintVersion::ORIGINAL);
    xaifBoosterCodeReplacement::Replacement& tracingCFG(myReplacementList_p->addReplacement(2)); 
    if (myTracingControlFlowGraph_p) { 
      tracingCFG.setReversibleControlFlowGraph(*myTracingControlFlowGraph_p);
      tracingCFG.setPrintVersion(xaifBoosterCodeReplacement::PrintVersion::AUGMENTED);
    }
    else { // if we don't have anything to trace 
    tracingCFG.setControlFlowGraphBase(getContaining().getControlFlowGraph());
    tracingCFG.setPrintVersion(xaifBoosterCodeReplacement::PrintVersion::ORIGINAL);
    }
  } 


  void
  CallGraphVertexAlg::printXMLHierarchy(std::ostream& os) const {
    if (!myReplacementList_p)
      THROW_LOGICEXCEPTION_MACRO("CallGraphVertexAlg::printXMLHierarchy: no replacement list ");
    myReplacementList_p->printXMLHierarchy(os);
  }

  std::string
  CallGraphVertexAlg::debug() const {
    std::ostringstream out;
    out << "xaifBoosterControlFlowReversal::CallGraphVertexAlg["
        << this
	<< ", containing="
	<< getContaining().debug().c_str()
        << "]" << std::ends;
    return out.str();
  }

  void CallGraphVertexAlg::traverseToChildren(const GenericAction::GenericAction_E anAction_c) {
  }

} // end of namespace

