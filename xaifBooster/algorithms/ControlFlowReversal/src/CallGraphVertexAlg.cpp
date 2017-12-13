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

#include "xaifBooster/algorithms/CodeReplacement/inc/ConceptuallyStaticInstances.hpp"
#include "xaifBooster/algorithms/ControlFlowReversal/inc/CallGraphVertexAlg.hpp"


using namespace xaifBooster;

namespace xaifBoosterControlFlowReversal { 

  bool CallGraphVertexAlg::ourChangeIntentFlag=false;

  CallGraphVertexAlg::CallGraphVertexAlg(CallGraphVertex& theContaining) : 
    CallGraphVertexAlgBase(theContaining), 
    myTapingControlFlowGraph_p(NULL), 
    myAdjointControlFlowGraph_p(NULL),
    myStrictAnonymousTapingControlFlowGraph_p(NULL), 
    myStrictAnonymousAdjointControlFlowGraph_p(NULL) {
  }

  CallGraphVertexAlg::~CallGraphVertexAlg() {
    if (myTapingControlFlowGraph_p) delete myTapingControlFlowGraph_p;
    if (myAdjointControlFlowGraph_p) delete myAdjointControlFlowGraph_p;
    if (myStrictAnonymousTapingControlFlowGraph_p) delete myStrictAnonymousTapingControlFlowGraph_p;
    if (myStrictAnonymousAdjointControlFlowGraph_p) delete myStrictAnonymousAdjointControlFlowGraph_p;
  }

  bool CallGraphVertexAlg::hasTapingControlFlowGraph() const { 
    return (myTapingControlFlowGraph_p?true:false);
  }
  
  ReversibleControlFlowGraph&
  CallGraphVertexAlg::getTapingControlFlowGraph() {
    if (!myTapingControlFlowGraph_p) 
      THROW_LOGICEXCEPTION_MACRO("CallGraphVertexAlg::getTapingControlFlowGraph: not set");
    return *myTapingControlFlowGraph_p;
  }
                                                                                
  const ReversibleControlFlowGraph&
  CallGraphVertexAlg::getTapingControlFlowGraph() const {
    if (!myTapingControlFlowGraph_p) 
      THROW_LOGICEXCEPTION_MACRO("CallGraphVertexAlg::getTapingControlFlowGraph: not set");
    return *myTapingControlFlowGraph_p;
  }

  bool CallGraphVertexAlg::hasAdjointControlFlowGraph() const { 
    return (myAdjointControlFlowGraph_p?true:false);
  }

  ReversibleControlFlowGraph&
  CallGraphVertexAlg::getAdjointControlFlowGraph() {
    if (!myAdjointControlFlowGraph_p) 
      THROW_LOGICEXCEPTION_MACRO("CallGraphVertexAlg::getAdjointControlFlowGraph: not set");
    return *myAdjointControlFlowGraph_p;
  }
                                                                                
  const ReversibleControlFlowGraph&
  CallGraphVertexAlg::getAdjointControlFlowGraph() const {
    if (!myAdjointControlFlowGraph_p) 
      THROW_LOGICEXCEPTION_MACRO("CallGraphVertexAlg::getAdjointControlFlowGraph: not set");
    return *myAdjointControlFlowGraph_p;
  }

  bool CallGraphVertexAlg::hasStrictAnonymousTapingControlFlowGraph() const { 
    return (myStrictAnonymousTapingControlFlowGraph_p?true:false);
  }
  
  ReversibleControlFlowGraph&
  CallGraphVertexAlg::getStrictAnonymousTapingControlFlowGraph() {
    if (!myStrictAnonymousTapingControlFlowGraph_p) 
      THROW_LOGICEXCEPTION_MACRO("CallGraphVertexAlg::getStrictAnonymousTapingControlFlowGraph: not set");
    return *myStrictAnonymousTapingControlFlowGraph_p;
  }
                                                                                
  const ReversibleControlFlowGraph&
  CallGraphVertexAlg::getStrictAnonymousTapingControlFlowGraph() const {
    if (!myStrictAnonymousTapingControlFlowGraph_p) 
      THROW_LOGICEXCEPTION_MACRO("CallGraphVertexAlg::getStrictAnonymousTapingControlFlowGraph: not set");
    return *myStrictAnonymousTapingControlFlowGraph_p;
  }

  bool CallGraphVertexAlg::hasStrictAnonymousAdjointControlFlowGraph() const { 
    return (myStrictAnonymousAdjointControlFlowGraph_p?true:false);
  }

  ReversibleControlFlowGraph&
  CallGraphVertexAlg::getStrictAnonymousAdjointControlFlowGraph() {
    if (!myStrictAnonymousAdjointControlFlowGraph_p) 
      THROW_LOGICEXCEPTION_MACRO("CallGraphVertexAlg::getStrictAnonymousAdjointControlFlowGraph: not set");
    return *myStrictAnonymousAdjointControlFlowGraph_p;
  }
                                                                                
  const ReversibleControlFlowGraph&
  CallGraphVertexAlg::getStrictAnonymousAdjointControlFlowGraph() const {
    if (!myStrictAnonymousAdjointControlFlowGraph_p) 
      THROW_LOGICEXCEPTION_MACRO("CallGraphVertexAlg::getStrictAnonymousAdjointControlFlowGraph: not set");
    return *myStrictAnonymousAdjointControlFlowGraph_p;
  }

  class ControlFlowGraphVertexLabelWriter {
  public:
    ControlFlowGraphVertexLabelWriter(const ReversibleControlFlowGraph& g) : myG(g) {}
    template <class BoostIntenalVertexDescriptor>
    void operator()(std::ostream& out, const BoostIntenalVertexDescriptor& v) const {
      ReversibleControlFlowGraphVertex* theReversibleControlFlowGraphVertex_p=boost::get(boost::get(BoostVertexContentType(),myG.getInternalBoostGraph()),v);
      std::string theVertexKind;
      std::string theIdInfo; // <xaif id>[s<store placeholder>][r<restore placeholder>][t<top level explicit loop>][e<enclosing control flow start>]
      if (theReversibleControlFlowGraphVertex_p->isOriginal()) {
	const ControlFlowGraphVertexAlg& va(dynamic_cast<const ControlFlowGraphVertexAlg&>(theReversibleControlFlowGraphVertex_p->getOriginalVertex().getControlFlowGraphVertexAlgBase()));
        theVertexKind=va.kindToString();
	const ControlFlowGraphVertex& v(dynamic_cast<const ControlFlowGraphVertex&>(theReversibleControlFlowGraphVertex_p->getOriginalVertex()));
        theIdInfo=v.getId();
	if (theReversibleControlFlowGraphVertex_p->hasStorePlaceholder())
	  theIdInfo+="s"+theReversibleControlFlowGraphVertex_p->getStorePlaceholder().getNewVertex().getId();
	if (theReversibleControlFlowGraphVertex_p->hasRestorePlaceholder())
	  theIdInfo+="r"+theReversibleControlFlowGraphVertex_p->getRestorePlaceholder().getNewVertex().getId();
      }
      else {
	const ControlFlowGraphVertexAlg& va(dynamic_cast<const ControlFlowGraphVertexAlg&>(theReversibleControlFlowGraphVertex_p->getNewVertex().getControlFlowGraphVertexAlgBase()));
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
    const ReversibleControlFlowGraph& myG;
  };

  class AdjointControlFlowGraphVertexLabelWriter {
  public:
    AdjointControlFlowGraphVertexLabelWriter(const ReversibleControlFlowGraph& g) : myG(g) {}
    template <class BoostIntenalVertexDescriptor>
    void operator()(std::ostream& out, const BoostIntenalVertexDescriptor& v) const {
      ReversibleControlFlowGraphVertex* theReversibleControlFlowGraphVertex_p=boost::get(boost::get(BoostVertexContentType(),myG.getInternalBoostGraph()),v);
      std::string theVertexKind;
      std::string theXaifId;
      if (theReversibleControlFlowGraphVertex_p->isOriginal()) {
	const ControlFlowGraphVertexAlg& va(dynamic_cast<const ControlFlowGraphVertexAlg&>(theReversibleControlFlowGraphVertex_p->getOriginalVertex().getControlFlowGraphVertexAlgBase()));
        theVertexKind=va.kindToString();
	const ControlFlowGraphVertex& v(dynamic_cast<const ControlFlowGraphVertex&>(theReversibleControlFlowGraphVertex_p->getOriginalVertex()));
        theXaifId=v.getId();
      }
      else {
	const ControlFlowGraphVertexAlg& va(dynamic_cast<const ControlFlowGraphVertexAlg&>(theReversibleControlFlowGraphVertex_p->getNewVertex().getControlFlowGraphVertexAlgBase()));
        theVertexKind=va.kindToString();
	const ControlFlowGraphVertex& v(dynamic_cast<const ControlFlowGraphVertex&>(theReversibleControlFlowGraphVertex_p->getNewVertex()));
        theXaifId=v.getId();
      }
      if (theReversibleControlFlowGraphVertex_p->hasStorePlaceholder())
	theXaifId+="s:"+theReversibleControlFlowGraphVertex_p->getStorePlaceholder().getNewVertex().getId();
      if (theReversibleControlFlowGraphVertex_p->hasRestorePlaceholder())
	theXaifId+="r:"+theReversibleControlFlowGraphVertex_p->getRestorePlaceholder().getNewVertex().getId();
      if (theReversibleControlFlowGraphVertex_p->getReversalType()==ForLoopReversalType::EXPLICIT) { 
	std::ostringstream temp;
	temp << theXaifId.c_str() << ".e";
	temp << std::ends;
	theXaifId=temp.str();
      }
      out << "[label=\"" << boost::get(boost::get(BoostVertexContentType(), myG.getInternalBoostGraph()), v)->getIndex() << " (" << theXaifId.c_str() << "): " << theVertexKind.c_str() << "\"]";
    }
    const ReversibleControlFlowGraph& myG;
  };

  class ControlFlowGraphEdgeLabelWriter {
  public:
    ControlFlowGraphEdgeLabelWriter(const ReversibleControlFlowGraph& g) : myG(g) {}
    template <class BoostIntenalEdgeDescriptor>
    void operator()(std::ostream& out, const BoostIntenalEdgeDescriptor& v) const {
      ReversibleControlFlowGraphEdge* theReversibleControlFlowGraphEdge_p=boost::get(boost::get(BoostEdgeContentType(),myG.getInternalBoostGraph()),v);
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
    const ReversibleControlFlowGraph& myG;
  };

  void CallGraphVertexAlg::algorithm_action_4() {
    DBG_MACRO(DbgGroup::CALLSTACK,
              "xaifBoosterControlFlowReversal::CallGraphVertexAlg::algorithm_action_4(reverse control flow) called for: "
              << debug().c_str());
    if (!getContaining().getControlFlowGraph().numVertices())
      // for instance modules may have an empty CallGraph
      return; 
    myTapingControlFlowGraph_p=new ReversibleControlFlowGraph(getContaining().getControlFlowGraph());
    myAdjointControlFlowGraph_p=new ReversibleControlFlowGraph(getContaining().getControlFlowGraph());
    myTapingControlFlowGraph_p->makeThisACopyOfOriginalControlFlowGraph();
    // do the same steps for the strictly anonymous version where we ignore the user supplied flags
    myStrictAnonymousTapingControlFlowGraph_p=new ReversibleControlFlowGraph(getContaining().getControlFlowGraph());
    myStrictAnonymousAdjointControlFlowGraph_p=new ReversibleControlFlowGraph(getContaining().getControlFlowGraph());
    // this flag causes strict anonymity
    myStrictAnonymousTapingControlFlowGraph_p->donotRetainUserReversalFlag();
    myStrictAnonymousTapingControlFlowGraph_p->makeThisACopyOfOriginalControlFlowGraph();
    if (getContaining().getControlFlowGraph().isStructured())
      structuredReversal();
    else
      unstructuredReversal();
  } // end CallGraphVertexAlg::algorithm_action_4() 

  void CallGraphVertexAlg::structuredReversal() {
    if (DbgLoggerManager::instance()->isSelected(DbgGroup::GRAPHICS) && DbgLoggerManager::instance()->wantTag("cfg")) {     
      GraphVizDisplay::show(*myTapingControlFlowGraph_p,"cfg_topologically_sorted", ControlFlowGraphVertexLabelWriter(*myTapingControlFlowGraph_p),ControlFlowGraphEdgeLabelWriter(*myTapingControlFlowGraph_p));
    }
    myTapingControlFlowGraph_p->markBranchExitEdges();
    if (DbgLoggerManager::instance()->isSelected(DbgGroup::GRAPHICS) && DbgLoggerManager::instance()->wantTag("cfg")) {     
      GraphVizDisplay::show(*myTapingControlFlowGraph_p,"cfg_branch_marked", ControlFlowGraphVertexLabelWriter(*myTapingControlFlowGraph_p),ControlFlowGraphEdgeLabelWriter(*myTapingControlFlowGraph_p));
    }
    // buildAdjointControlFlowGraph() should always be based on the
    // original CFG, that is, it should precede the call to 
    // storeControlFlow()
    // but we should have found out how to label branch edges...
    myTapingControlFlowGraph_p->buildAdjointControlFlowGraph(*myAdjointControlFlowGraph_p,getContaining().getInitializeDerivativeComponentsFlag());
    if (DbgLoggerManager::instance()->isSelected(DbgGroup::GRAPHICS) && DbgLoggerManager::instance()->wantTag("cfg")) {     
      GraphVizDisplay::show(*myAdjointControlFlowGraph_p,"cfg_adjoint", AdjointControlFlowGraphVertexLabelWriter(*myAdjointControlFlowGraph_p),ControlFlowGraphEdgeLabelWriter(*myAdjointControlFlowGraph_p));
    }
    myTapingControlFlowGraph_p->storeControlFlow();
    if (DbgLoggerManager::instance()->isSelected(DbgGroup::GRAPHICS) && DbgLoggerManager::instance()->wantTag("cfg")) {     
      GraphVizDisplay::show(*myTapingControlFlowGraph_p,"cfg_taping", ControlFlowGraphVertexLabelWriter(*myTapingControlFlowGraph_p),ControlFlowGraphEdgeLabelWriter(*myTapingControlFlowGraph_p));
    }
    // GraphVizDisplay::show(*myStrictAnonymousTapingControlFlowGraph_p,"cfg_copy", ControlFlowGraphVertexLabelWriter(*myStrictAnonymousTapingControlFlowGraph_p),ControlFlowGraphEdgeLabelWriter(*myStrictAnonymousTapingControlFlowGraph_p));
    //    myStrictAnonymousTapingControlFlowGraph_p->topologicalSort();
    if (DbgLoggerManager::instance()->isSelected(DbgGroup::GRAPHICS) && DbgLoggerManager::instance()->wantTag("cfg")) {     
      GraphVizDisplay::show(*myStrictAnonymousTapingControlFlowGraph_p,"cfg_strict_anonymous_topologically_sorted", ControlFlowGraphVertexLabelWriter(*myStrictAnonymousTapingControlFlowGraph_p),ControlFlowGraphEdgeLabelWriter(*myStrictAnonymousTapingControlFlowGraph_p));
    }
    myStrictAnonymousTapingControlFlowGraph_p->markBranchExitEdges();
    if (DbgLoggerManager::instance()->isSelected(DbgGroup::GRAPHICS) && DbgLoggerManager::instance()->wantTag("cfg")) {     
      GraphVizDisplay::show(*myStrictAnonymousTapingControlFlowGraph_p,"cfg_strict_anonymous_branch_marked", ControlFlowGraphVertexLabelWriter(*myStrictAnonymousTapingControlFlowGraph_p),ControlFlowGraphEdgeLabelWriter(*myStrictAnonymousTapingControlFlowGraph_p));
    }
    // buildAdjointControlFlowGraph() should always be based on the
    // original CFG, that is, it should precede the call to 
    // storeControlFlow()
    // but we should have found out how to label branch edges...
    myStrictAnonymousTapingControlFlowGraph_p->buildAdjointControlFlowGraph(*myStrictAnonymousAdjointControlFlowGraph_p,getContaining().getInitializeDerivativeComponentsFlag());
    if (DbgLoggerManager::instance()->isSelected(DbgGroup::GRAPHICS) && DbgLoggerManager::instance()->wantTag("cfg")) {     
      GraphVizDisplay::show(*myStrictAnonymousAdjointControlFlowGraph_p,"cfg_strict_anonymous_adjoint", AdjointControlFlowGraphVertexLabelWriter(*myStrictAnonymousAdjointControlFlowGraph_p),ControlFlowGraphEdgeLabelWriter(*myStrictAnonymousAdjointControlFlowGraph_p));
    }
    myStrictAnonymousTapingControlFlowGraph_p->storeControlFlow();
    if (DbgLoggerManager::instance()->isSelected(DbgGroup::GRAPHICS) && DbgLoggerManager::instance()->wantTag("cfg")) {     
      GraphVizDisplay::show(*myStrictAnonymousTapingControlFlowGraph_p,"cfg_strict_anonymous_taping", ControlFlowGraphVertexLabelWriter(*myStrictAnonymousTapingControlFlowGraph_p),ControlFlowGraphEdgeLabelWriter(*myStrictAnonymousTapingControlFlowGraph_p));
    }
  }

  void CallGraphVertexAlg::unstructuredReversal() {
    if (DbgLoggerManager::instance()->isSelected(DbgGroup::GRAPHICS) && DbgLoggerManager::instance()->wantTag("cfg")) {     
      GraphVizDisplay::show(*myTapingControlFlowGraph_p,
			    "cfg_unstructured", 
			    ControlFlowGraphVertexLabelWriter(*myTapingControlFlowGraph_p),
			    ControlFlowGraphEdgeLabelWriter(*myTapingControlFlowGraph_p));
    }
    myTapingControlFlowGraph_p->storeEnumeratedBB();
    if (DbgLoggerManager::instance()->isSelected(DbgGroup::GRAPHICS) && DbgLoggerManager::instance()->wantTag("cfg")) {     
      GraphVizDisplay::show(*myTapingControlFlowGraph_p,
			    "cfg_unstructured_taping", 
			    ControlFlowGraphVertexLabelWriter(*myTapingControlFlowGraph_p),
			    ControlFlowGraphEdgeLabelWriter(*myTapingControlFlowGraph_p));
    }
    myTapingControlFlowGraph_p->reverseFromEnumeratedBB(*myAdjointControlFlowGraph_p);
    if (DbgLoggerManager::instance()->isSelected(DbgGroup::GRAPHICS) && DbgLoggerManager::instance()->wantTag("cfg")) {     
      GraphVizDisplay::show(*myAdjointControlFlowGraph_p,
			    "cfg_unstructured_adjoint", 
			    ControlFlowGraphVertexLabelWriter(*myAdjointControlFlowGraph_p),
			    ControlFlowGraphEdgeLabelWriter(*myAdjointControlFlowGraph_p));
    }
    myStrictAnonymousTapingControlFlowGraph_p->storeEnumeratedBB();
    myStrictAnonymousTapingControlFlowGraph_p->reverseFromEnumeratedBB(*myStrictAnonymousAdjointControlFlowGraph_p);
  }

  void
  CallGraphVertexAlg::printXMLHierarchy(std::ostream& os) const {
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent()
       << "<"
       << getContaining().getControlFlowGraph().ourXAIFName.c_str()
       << " ";
    getContaining().getControlFlowGraph().printAttributes(os,
							  getContaining().
							  getControlFlowGraph().
							  getSymbolReference());
    os << " "
       << getContaining().getControlFlowGraph().our_myActiveFlag_XAIFName.c_str()
       << "=\""
       << getContaining().getControlFlowGraph().getActiveFlag()
       << "\">"
       << std::endl;
    getContaining().getControlFlowGraph().getArgumentList().printXMLHierarchy(os);

    // This is for testing purposes only,
    // if the following graphs have been created we have multiple ENTRIES and EXITS in one cfg                              
    // they may not have been created for empty graphs as e.g. possible for modules
    if (myTapingControlFlowGraph_p) { 
      xaifBoosterCodeReplacement::ConceptuallyStaticInstances::instance()->setPrintVersion(xaifBoosterCodeReplacement::PrintVersion::AUGMENTED);
      myTapingControlFlowGraph_p->printXMLHierarchy(os);
    }
    if (myAdjointControlFlowGraph_p) { 
      xaifBoosterCodeReplacement::ConceptuallyStaticInstances::instance()->setPrintVersion(xaifBoosterCodeReplacement::PrintVersion::ADJOINT);
      myAdjointControlFlowGraph_p->printXMLHierarchy(os);
    }
    if (myStrictAnonymousTapingControlFlowGraph_p) { 
      xaifBoosterCodeReplacement::ConceptuallyStaticInstances::instance()->setPrintVersion(xaifBoosterCodeReplacement::PrintVersion::AUGMENTED);
      myStrictAnonymousTapingControlFlowGraph_p->printXMLHierarchy(os);
    }
    if (myStrictAnonymousAdjointControlFlowGraph_p) { 
      xaifBoosterCodeReplacement::ConceptuallyStaticInstances::instance()->setPrintVersion(xaifBoosterCodeReplacement::PrintVersion::ADJOINT);
      myStrictAnonymousAdjointControlFlowGraph_p->printXMLHierarchy(os);
    }                          
    os << pm.indent()
       << "</"
       << getContaining().getControlFlowGraph().ourXAIFName
       << ">"
       << std::endl;
    pm.releaseInstance();
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

  void CallGraphVertexAlg::changeIntent() { 
    ourChangeIntentFlag=true;
  }

  bool CallGraphVertexAlg::changesIntent() { 
    return ourChangeIntentFlag;
  }

} // end of namespace

