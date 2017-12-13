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

#include "xaifBooster/system/inc/ConceptuallyStaticInstances.hpp"
#include "xaifBooster/system/inc/CallGraph.hpp"

#include "xaifBooster/system/inc/GraphVizDisplay.hpp"
#include "xaifBooster/system/inc/Argument.hpp"
#include "xaifBooster/system/inc/ArgumentList.hpp"
#include "xaifBooster/system/inc/Intrinsic.hpp"
#include "xaifBooster/system/inc/VariableSymbolReference.hpp"
#include "xaifBooster/system/inc/Constant.hpp"
#include "xaifBooster/system/inc/SubroutineCall.hpp"
#include "xaifBooster/system/inc/BooleanOperation.hpp"

#include "xaifBooster/algorithms/ControlFlowReversal/inc/CallGraphVertexAlg.hpp"
#include "xaifBooster/algorithms/ControlFlowReversal/inc/ReversibleControlFlowGraph.hpp"
#include "xaifBooster/algorithms/ControlFlowReversal/inc/ControlFlowGraphVertexAlg.hpp"
#include "xaifBooster/algorithms/ControlFlowReversal/inc/CallGraphAlg.hpp"
#include "xaifBooster/algorithms/ControlFlowReversal/inc/BasicBlockAlg.hpp"

#include "xaifBooster/algorithms/InlinableXMLRepresentation/inc/InlinableSubroutineCall.hpp"

using namespace xaifBooster;

namespace xaifBoosterControlFlowReversal { 

  ReversibleControlFlowGraph::ReversibleControlFlowGraph(const ControlFlowGraph& theOriginal_r) : 
    myOriginalGraph_r(theOriginal_r),
    myRetainUserReversalFlag(true),
    myStructuredFlag(true) {
  }

  class ReversibleControlFlowGraphVertexLabelWriter {
  public:
    ReversibleControlFlowGraphVertexLabelWriter(const ReversibleControlFlowGraph& g) : myG(g) {};
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
      if (theReversibleControlFlowGraphVertex_p->getReversalType()==ForLoopReversalType::EXPLICIT) { 
	std::ostringstream temp;
	temp << theXaifId.c_str() << ".e" << std::ends;
	theXaifId=temp.str();
      }
      out << "[label=\"" << boost::get(boost::get(BoostVertexContentType(), myG.getInternalBoostGraph()), v)->getIndex() << " (" << theXaifId.c_str() << "): " << theVertexKind.c_str() << "\"]";
    }
    const ReversibleControlFlowGraph& myG;
  };

  class ReversibleControlFlowGraphEdgeLabelWriter {
  public:
    ReversibleControlFlowGraphEdgeLabelWriter(const ReversibleControlFlowGraph& g) : myG(g) {};
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

  void
  ReversibleControlFlowGraph::makeThisACopyOfOriginalControlFlowGraph() {
    class GetReversibleFromOriginal {
    public : 
      ReversibleControlFlowGraphVertex& operator() (std::list<std::pair<const ControlFlowGraphVertex*,ReversibleControlFlowGraphVertex*> > vertexCopy_l,
						    const ControlFlowGraphVertex& theOriginalVertex) { 
	std::list<std::pair<const ControlFlowGraphVertex*,ReversibleControlFlowGraphVertex*> >::iterator vertexCopyListIt;
	for (vertexCopyListIt=vertexCopy_l.begin();
	     vertexCopyListIt!=vertexCopy_l.end();
	     ++vertexCopyListIt) {
	  if (&theOriginalVertex==(*vertexCopyListIt).first)
	    return *((*vertexCopyListIt).second);
	}
	THROW_LOGICEXCEPTION_MACRO("getReversibleFromOriginal: not found"); 
      };
    } getReversibleFromOriginal;
    std::list<std::pair<const ControlFlowGraphVertex*,ReversibleControlFlowGraphVertex*> > vertexCopy_l;
    ControlFlowGraph::ConstVertexIteratorPair p(myOriginalGraph_r.vertices());
    ControlFlowGraph::ConstVertexIterator vertexIt(p.first),endIt(p.second);
    for (;vertexIt!=endIt ;++vertexIt) {
      ReversibleControlFlowGraphVertex* anOriginalVertex=new ReversibleControlFlowGraphVertex(&(*vertexIt));
      vertexCopy_l.push_back(std::make_pair(&(*vertexIt),anOriginalVertex));  
      supplyAndAddVertexInstance(*anOriginalVertex);
      // copy simple augmented info: 
      anOriginalVertex->setIndex((*vertexIt).getIndex());
      anOriginalVertex->setReversalType((myRetainUserReversalFlag)?(*vertexIt).getReversalType():ForLoopReversalType::ANONYMOUS);
    } 
    // copy the augmented vertex references now that we have all reversible vertices created: 
    std::list<std::pair<const ControlFlowGraphVertex*,ReversibleControlFlowGraphVertex*> >::iterator vertexCopyListIt;
    for (vertexCopyListIt=vertexCopy_l.begin();
	 vertexCopyListIt!=vertexCopy_l.end();
	 ++vertexCopyListIt) {
      ControlFlowGraphVertexAlg::ControlFlowGraphVertexKind_E theKind((*vertexCopyListIt).second->getKind());
      if (myOriginalGraph_r.isStructured()
	  && 
	  (
	   theKind==ControlFlowGraphVertexAlg::FORLOOP 
	   || 
	   theKind==ControlFlowGraphVertexAlg::PRELOOP
	   ||
	   theKind==ControlFlowGraphVertexAlg::BRANCH
	   ||
	   theKind==ControlFlowGraphVertexAlg::ENDLOOP
	   ||
	   theKind==ControlFlowGraphVertexAlg::ENDBRANCH
	   ))
	(*vertexCopyListIt).second->setCounterPart(getReversibleFromOriginal(vertexCopy_l,
									     (*vertexCopyListIt).first->getCounterPart()));
      (*vertexCopyListIt).second->setLoopVariables((*vertexCopyListIt).first->getKnownLoopVariables());
      if ((*vertexCopyListIt).first->hasTopExplicitLoop())
	(*vertexCopyListIt).second->setTopExplicitLoop(getReversibleFromOriginal(vertexCopy_l,
										 (*vertexCopyListIt).first->getTopExplicitLoop()));
      if ((*vertexCopyListIt).first->hasEnclosingControlFlow())
	(*vertexCopyListIt).second->setEnclosingControlFlow(getReversibleFromOriginal(vertexCopy_l,
										      (*vertexCopyListIt).first->getEnclosingControlFlow()));
    }
    // the sorted vertex list
    const std::list<const ControlFlowGraphVertex*>& theList(myOriginalGraph_r.getSOrtedVertexList());
    for (std::list<const ControlFlowGraphVertex*>::const_iterator sortListIter=theList.begin();
	 sortListIter!=theList.end();
	 ++sortListIter) { 
      mySortedVertices_p_l.push_back(&(getReversibleFromOriginal(vertexCopy_l,
								 **sortListIter)));
    }
    ControlFlowGraph::ConstEdgeIteratorPair pe(myOriginalGraph_r.edges());
    ControlFlowGraph::ConstEdgeIterator beginIte(pe.first),endIte(pe.second);
    for (;beginIte!=endIte ;++beginIte) {
      const ControlFlowGraphVertex& theOriginalSource_r(myOriginalGraph_r.getSourceOf(*beginIte));
      const ControlFlowGraphVertex& theOriginalTarget_r(myOriginalGraph_r.getTargetOf(*beginIte));
      std::list<std::pair<const ControlFlowGraphVertex*,ReversibleControlFlowGraphVertex*> >::iterator vertexCopyListSrcIt, vertexCopyListTgtIt;
      for (vertexCopyListSrcIt=vertexCopy_l.begin();
           vertexCopyListSrcIt!=vertexCopy_l.end();vertexCopyListSrcIt++) {
        if ((*vertexCopyListSrcIt).first==&theOriginalSource_r) {
          for (vertexCopyListTgtIt=vertexCopy_l.begin();
               vertexCopyListTgtIt!=vertexCopy_l.end();vertexCopyListTgtIt++) {
            if ((*vertexCopyListTgtIt).first==&theOriginalTarget_r) {
              ReversibleControlFlowGraphEdge* theEdgeCopy_p=new ReversibleControlFlowGraphEdge(&(*beginIte));
	      theEdgeCopy_p->setId(makeUniqueEdgeId());
              supplyAndAddEdgeInstance(*theEdgeCopy_p,*(*vertexCopyListSrcIt).second,*(*vertexCopyListTgtIt).second);
              break;
            }
          }
          break;
        }
      }
    }
  }

  ReversibleControlFlowGraphVertex& ReversibleControlFlowGraph::getEntry() {
    ReversibleControlFlowGraph::VertexIteratorPair p(vertices());
    ReversibleControlFlowGraph::VertexIterator beginIt(p.first),endIt(p.second);
    for (;beginIt!=endIt ;++beginIt) 
      if ((*beginIt).isOriginal()) 
        if ((*beginIt).getOriginalControlFlowGraphVertexAlg().getKind()==ControlFlowGraphVertexAlg::ENTRY) 
          return *beginIt;
    THROW_LOGICEXCEPTION_MACRO("Missing ENTRY node in control flow graph"); 
  }
  
  ReversibleControlFlowGraphVertex& ReversibleControlFlowGraph::getExit() {
    ReversibleControlFlowGraph::VertexIteratorPair p(vertices());
    ReversibleControlFlowGraph::VertexIterator beginIt(p.first),endIt(p.second);
    for (;beginIt!=endIt ;++beginIt) 
      if ((*beginIt).isOriginal()) 
        if ((*beginIt).getOriginalControlFlowGraphVertexAlg().getKind()==ControlFlowGraphVertexAlg::EXIT) 
          return *beginIt;
    THROW_LOGICEXCEPTION_MACRO("Missing EXIT node in control flow graph"); 
  }

  // this does not guarantee uniqueness in the case of an existing
  // graph getting expanded
  // depends on prefix
  std::string ReversibleControlFlowGraph::makeUniqueVertexId() {
    std::ostringstream oss;
    oss << dynamic_cast<const CallGraphAlg&>(ConceptuallyStaticInstances::instance()->getCallGraph().getCallGraphAlgBase()).
      getAlgorithmSignature().c_str() << "v_" << getNextVertexId() << std::ends;
    return (oss.str());
  }

  std::string ReversibleControlFlowGraph::makeUniqueEdgeId() {
    std::ostringstream oss;
    oss << dynamic_cast<const CallGraphAlg&>(ConceptuallyStaticInstances::instance()->getCallGraph().getCallGraphAlgBase()).
      getAlgorithmSignature().c_str() << "e_" << getNextEdgeId() << std::ends;
    return (oss.str());
  }

  // direction indicates if the characteristics of the replaceEdge should
  // be preserved by the new in (false) or outedge (true)
  ReversibleControlFlowGraphVertex& 
  ReversibleControlFlowGraph::insertBasicBlock(const ReversibleControlFlowGraphVertex& after, 
					       const ReversibleControlFlowGraphVertex& before, 
					       const ReversibleControlFlowGraphEdge& replacedEdge_r, 
					       bool direction) {
    ReversibleControlFlowGraphVertex* aNewReversibleControlFlowGraphVertex_p=new ReversibleControlFlowGraphVertex();
    aNewReversibleControlFlowGraphVertex_p->setIndex(numVertices()+1);
    supplyAndAddVertexInstance(*aNewReversibleControlFlowGraphVertex_p);
    // myOriginalGraph_r does not have a scope yet since nobody sets it
    // use the global scope for the time being
    //    aNewReversibleControlFlowGraphVertex->myNewVertex_p=new BasicBlock(myOriginalGraph_r.getScope());
    BasicBlock* theNewBasicBlock=new BasicBlock(ConceptuallyStaticInstances::instance()->getCallGraph().getScopeTree().getGlobalScope());
    aNewReversibleControlFlowGraphVertex_p->supplyAndAddNewVertex(*theNewBasicBlock);
    //    aNewReversibleControlFlowGraphVertex_p->getNewVertex().setId(makeUniqueVertexId());
    aNewReversibleControlFlowGraphVertex_p->getNewVertex().setAnnotation(dynamic_cast<const CallGraphAlg&>(ConceptuallyStaticInstances::instance()->getCallGraph().getCallGraphAlgBase()).getAlgorithmSignature());
    ReversibleControlFlowGraphEdge* aNewReversibleControlFlowGraphInEdge_p=new ReversibleControlFlowGraphEdge();
    aNewReversibleControlFlowGraphInEdge_p->setId(makeUniqueEdgeId());
    if (!direction && replacedEdge_r.hasConditionValue()) {
      aNewReversibleControlFlowGraphInEdge_p->setConditionValue(replacedEdge_r.getConditionValue());
    }
    if (!direction && replacedEdge_r.hasRevConditionValue()) {
      aNewReversibleControlFlowGraphInEdge_p->setRevConditionValue(replacedEdge_r.getRevConditionValue());
    }
    ReversibleControlFlowGraphEdge* aNewReversibleControlFlowGraphOutEdge_p=new ReversibleControlFlowGraphEdge();
    aNewReversibleControlFlowGraphOutEdge_p->setId(makeUniqueEdgeId());
    if (direction && replacedEdge_r.hasConditionValue()) {
      aNewReversibleControlFlowGraphOutEdge_p->setConditionValue(replacedEdge_r.getConditionValue());
    }
    if (direction && replacedEdge_r.hasRevConditionValue()) {
      aNewReversibleControlFlowGraphOutEdge_p->setRevConditionValue(replacedEdge_r.getRevConditionValue());
    }
    supplyAndAddEdgeInstance(*aNewReversibleControlFlowGraphInEdge_p,after,*aNewReversibleControlFlowGraphVertex_p);
    supplyAndAddEdgeInstance(*aNewReversibleControlFlowGraphOutEdge_p,*aNewReversibleControlFlowGraphVertex_p,before);
    return *aNewReversibleControlFlowGraphVertex_p;
  }

  /** Add a new subroutine call with the specified name to the specified Basic Block*/
  xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall& 
  ReversibleControlFlowGraph::addInlinableSubroutineCall(const std::string& aSubroutineName,BasicBlock* theBasicBlock) {
    xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall* aNewCall_p(new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall(aSubroutineName));
    theBasicBlock->supplyAndAddBasicBlockElementInstance(*aNewCall_p);
    return *aNewCall_p;									     
  }

  /** Add a ZeroDeriv subroutine call for the specified variable in the specified Basic Block */
  void ReversibleControlFlowGraph::addZeroDeriv(const Variable& theTarget,BasicBlock* theBasicBlock) {
    xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall& 
      theSubroutineCall(ReversibleControlFlowGraph::addInlinableSubroutineCall("ZeroDeriv",theBasicBlock));
    theSubroutineCall.setId("inline_zeroderiv");
    theTarget.copyMyselfInto(theSubroutineCall.addConcreteArgument(1).getArgument().getVariable());
  }

  /** Create ZeroDeriv initialization subroutines for all active variables which are not paramenters with intent IN or intent INOUT */
  void ReversibleControlFlowGraph::initializeActiveVariables(BasicBlock* derivInitBasicBlock) {
    DBG_MACRO(DbgGroup::CALLSTACK,
	      "xaifBoosterControlFlowReversal::CallGraphVertexAlg::algorithm_action_5(initialize derivative components) called for: "
	      << debug().c_str());

    /** add all active variable symbols (and not subroutines) to init_symbols list */
    std::list<Symbol*> active_symbols = myOriginalGraph_r.getScope().getSymbolTable().getActiveSymbols();
    std::list<Symbol*> init_symbols;
    std::list<Symbol*>::const_iterator activeSymbol;
    std::list<Symbol*>::const_iterator initSymbol;
    for (activeSymbol = active_symbols.begin();
	 activeSymbol!=active_symbols.end();++activeSymbol) {
      if ((*activeSymbol)->getSymbolKind() == SymbolKind::VARIABLE)
	init_symbols.insert(init_symbols.end(),*activeSymbol);
    }
    /** remove all parameters which have intent INOUT or intent IN from the init_symbols list */
    const ArgumentList& origArgList = myOriginalGraph_r.getArgumentList();
    for (ArgumentList::ArgumentSymbolReferencePList::const_iterator anArgumentSymbolReferencePListI=
	   origArgList.getArgumentSymbolReferencePList().begin();
	 anArgumentSymbolReferencePListI!=origArgList.getArgumentSymbolReferencePList().end();
	 ++anArgumentSymbolReferencePListI) { 
      for (initSymbol = init_symbols.begin();
	   initSymbol != init_symbols.end(); ++initSymbol) {
	if ((*initSymbol) == &((*anArgumentSymbolReferencePListI)->getSymbol())) {
	  if ((*anArgumentSymbolReferencePListI)->getIntent() != IntentType::OUT_ITYPE 
	      ||
	      CallGraphVertexAlg::changesIntent()
	      ||	
              (*initSymbol)->getActiveTypeFlag()) {  // this also triggers intent change for the adjoint propagation
	    init_symbols.remove(*initSymbol);
	  }
	  break;
	}
      }
    }
    /** create deriv initialization routines for all symbols in init_symbols list */
    for (initSymbol = init_symbols.begin();
	 initSymbol != init_symbols.end(); ++initSymbol) {
      VariableSymbolReference* activeVarSym = 
	new VariableSymbolReference(*(*initSymbol),myOriginalGraph_r.getScope());
      activeVarSym->setId((*initSymbol)->getId());
      activeVarSym->setAnnotation((*initSymbol)->getAnnotation());
      
      Variable* activeVar = new Variable();
      activeVar->supplyAndAddVertexInstance(*(dynamic_cast<VariableVertex*>(activeVarSym)));
      activeVar->setDerivFlag();
      activeVar->getAliasMapKey().setTemporary();
      activeVar->getDuUdMapKey().setTemporary();
      
      addZeroDeriv(*activeVar,derivInitBasicBlock);
    }
  } // end ReversibleControlFlowGraph::initializeActiveVariables

  ReversibleControlFlowGraphVertex* ReversibleControlFlowGraph::old_basic_block(const BasicBlock& theOriginalBasicBlock) {
    ReversibleControlFlowGraphVertex* aNewReversibleControlFlowGraphVertex_p=new ReversibleControlFlowGraphVertex(&theOriginalBasicBlock);
    supplyAndAddVertexInstance(*aNewReversibleControlFlowGraphVertex_p);
    return aNewReversibleControlFlowGraphVertex_p;
  }

  ReversibleControlFlowGraphVertex* ReversibleControlFlowGraph::new_entry() {
    ReversibleControlFlowGraphVertex* aNewReversibleControlFlowGraphVertex_p=new ReversibleControlFlowGraphVertex();
    supplyAndAddVertexInstance(*aNewReversibleControlFlowGraphVertex_p);
    aNewReversibleControlFlowGraphVertex_p->supplyAndAddNewVertex(*(new Entry()));
    //    aNewReversibleControlFlowGraphVertex_p->getNewVertex().setId(makeUniqueVertexId());
    aNewReversibleControlFlowGraphVertex_p->getNewVertex().setAnnotation(dynamic_cast<const CallGraphAlg&>(ConceptuallyStaticInstances::instance()->getCallGraph().getCallGraphAlgBase()).getAlgorithmSignature());
    return aNewReversibleControlFlowGraphVertex_p;
  }

  ReversibleControlFlowGraphVertex* ReversibleControlFlowGraph::new_exit() {
    ReversibleControlFlowGraphVertex* aNewReversibleControlFlowGraphVertex_p=new ReversibleControlFlowGraphVertex();
    supplyAndAddVertexInstance(*aNewReversibleControlFlowGraphVertex_p);
    aNewReversibleControlFlowGraphVertex_p->supplyAndAddNewVertex(*(new Exit()));
    //    aNewReversibleControlFlowGraphVertex_p->getNewVertex().setId(makeUniqueVertexId());
    aNewReversibleControlFlowGraphVertex_p->getNewVertex().setAnnotation(dynamic_cast<const CallGraphAlg&>(ConceptuallyStaticInstances::instance()->getCallGraph().getCallGraphAlgBase()).getAlgorithmSignature());
    return aNewReversibleControlFlowGraphVertex_p;
  }

  ReversibleControlFlowGraphVertex* ReversibleControlFlowGraph::new_branch() {
    ReversibleControlFlowGraphVertex* aNewReversibleControlFlowGraphVertex_p=new ReversibleControlFlowGraphVertex();
    supplyAndAddVertexInstance(*aNewReversibleControlFlowGraphVertex_p);
    aNewReversibleControlFlowGraphVertex_p->supplyAndAddNewVertex(*(new Branch()));
    //    aNewReversibleControlFlowGraphVertex_p->getNewVertex().setId(makeUniqueVertexId());
    aNewReversibleControlFlowGraphVertex_p->getNewVertex().setAnnotation(dynamic_cast<const CallGraphAlg&>(ConceptuallyStaticInstances::instance()->getCallGraph().getCallGraphAlgBase()).getAlgorithmSignature());
    return aNewReversibleControlFlowGraphVertex_p;
  }

  ReversibleControlFlowGraphVertex* ReversibleControlFlowGraph::new_if() {
    ReversibleControlFlowGraphVertex* aNewReversibleControlFlowGraphVertex_p=new ReversibleControlFlowGraphVertex();
    supplyAndAddVertexInstance(*aNewReversibleControlFlowGraphVertex_p);
    aNewReversibleControlFlowGraphVertex_p->supplyAndAddNewVertex(*(new IfStatement()));
    //    aNewReversibleControlFlowGraphVertex_p->getNewVertex().setId(makeUniqueVertexId());
    aNewReversibleControlFlowGraphVertex_p->getNewVertex().setAnnotation(dynamic_cast<const CallGraphAlg&>(ConceptuallyStaticInstances::instance()->getCallGraph().getCallGraphAlgBase()).getAlgorithmSignature());
    return aNewReversibleControlFlowGraphVertex_p;
  }

  ReversibleControlFlowGraphVertex* ReversibleControlFlowGraph::new_forloop(ForLoopReversalType::ForLoopReversalType_E aForLoopReversalType,
									    const ForLoop& theOriginalForLoop) {
    ReversibleControlFlowGraphVertex* aNewReversibleControlFlowGraphVertex_p=new ReversibleControlFlowGraphVertex();
    supplyAndAddVertexInstance(*aNewReversibleControlFlowGraphVertex_p);
    ForLoop* aNewForLoop_p=new ForLoop(aForLoopReversalType);
    aNewReversibleControlFlowGraphVertex_p->supplyAndAddNewVertex(*aNewForLoop_p);
    aNewForLoop_p->makeInitialization().getAssignment().setId(dynamic_cast<const CallGraphAlg&>(ConceptuallyStaticInstances::instance()->getCallGraph().getCallGraphAlgBase()).getAlgorithmSignature()
							     +
							     theOriginalForLoop.getInitialization().getAssignment().getId());
    aNewForLoop_p->makeCondition();
    aNewForLoop_p->makeUpdate().getAssignment().setId(dynamic_cast<const CallGraphAlg&>(ConceptuallyStaticInstances::instance()->getCallGraph().getCallGraphAlgBase()).getAlgorithmSignature()
						     +
						     theOriginalForLoop.getUpdate().getAssignment().getId());
    aNewReversibleControlFlowGraphVertex_p->getNewVertex().setAnnotation(dynamic_cast<const CallGraphAlg&>(ConceptuallyStaticInstances::instance()->getCallGraph().getCallGraphAlgBase()).getAlgorithmSignature());
    return aNewReversibleControlFlowGraphVertex_p;
  }

  ReversibleControlFlowGraphVertex* ReversibleControlFlowGraph::new_preloop(const std::string& theOriginalPreLoopId) {
    ReversibleControlFlowGraphVertex* aNewReversibleControlFlowGraphVertex_p=new ReversibleControlFlowGraphVertex();
    supplyAndAddVertexInstance(*aNewReversibleControlFlowGraphVertex_p);
    // a preloop is handled by an anonymous FORLOOP
    ForLoop* aNewForLoop_p=new ForLoop(ForLoopReversalType::ANONYMOUS);
    aNewReversibleControlFlowGraphVertex_p->supplyAndAddNewVertex(*aNewForLoop_p);
    aNewForLoop_p->makeInitialization().getAssignment().setId(dynamic_cast<const CallGraphAlg&>(ConceptuallyStaticInstances::instance()->getCallGraph().getCallGraphAlgBase()).getAlgorithmSignature()
							     +
							     theOriginalPreLoopId
							     +
							     "_init");
    aNewForLoop_p->makeCondition();
    aNewForLoop_p->makeUpdate().getAssignment().setId(dynamic_cast<const CallGraphAlg&>(ConceptuallyStaticInstances::instance()->getCallGraph().getCallGraphAlgBase()).getAlgorithmSignature()
						     +
						     theOriginalPreLoopId
						     +
						     "_update");
    aNewReversibleControlFlowGraphVertex_p->getNewVertex().setAnnotation(dynamic_cast<const CallGraphAlg&>(ConceptuallyStaticInstances::instance()->getCallGraph().getCallGraphAlgBase()).getAlgorithmSignature());
    return aNewReversibleControlFlowGraphVertex_p;
  }

  ReversibleControlFlowGraphVertex* ReversibleControlFlowGraph::new_endbranch() {
    ReversibleControlFlowGraphVertex* aNewReversibleControlFlowGraphVertex_p=new ReversibleControlFlowGraphVertex();
    supplyAndAddVertexInstance(*aNewReversibleControlFlowGraphVertex_p);
    aNewReversibleControlFlowGraphVertex_p->supplyAndAddNewVertex(*(new EndBranch()));
    //    aNewReversibleControlFlowGraphVertex_p->getNewVertex().setId(makeUniqueVertexId());
    aNewReversibleControlFlowGraphVertex_p->getNewVertex().setAnnotation(dynamic_cast<const CallGraphAlg&>(ConceptuallyStaticInstances::instance()->getCallGraph().getCallGraphAlgBase()).getAlgorithmSignature());
    return aNewReversibleControlFlowGraphVertex_p;
  }

  ReversibleControlFlowGraphVertex* ReversibleControlFlowGraph::new_endloop() {
    ReversibleControlFlowGraphVertex* aNewReversibleControlFlowGraphVertex_p=new ReversibleControlFlowGraphVertex();
    supplyAndAddVertexInstance(*aNewReversibleControlFlowGraphVertex_p);
    aNewReversibleControlFlowGraphVertex_p->supplyAndAddNewVertex(*(new EndLoop()));
    //   aNewReversibleControlFlowGraphVertex_p->getNewVertex().setId(makeUniqueVertexId());
    aNewReversibleControlFlowGraphVertex_p->getNewVertex().setAnnotation(dynamic_cast<const CallGraphAlg&>(ConceptuallyStaticInstances::instance()->getCallGraph().getCallGraphAlgBase()).getAlgorithmSignature());
    return aNewReversibleControlFlowGraphVertex_p;
  }

  const Symbol* ReversibleControlFlowGraph::makeAuxiliaryIntegerLHS(Assignment& theAssignment, 
								     PlainBasicBlock& theBasicBlock_r) {
    const Symbol& theLhsSymbol_r(theBasicBlock_r.getScope().getSymbolTable().
				 addUniqueAuxSymbol(SymbolKind::VARIABLE,SymbolType::INTEGER_STYPE,SymbolShape::SCALAR,false));
    VariableSymbolReference* theVariableSymbolReference_p=new VariableSymbolReference(theLhsSymbol_r,theBasicBlock_r.getScope());
    theVariableSymbolReference_p->setId("1"); 
    theVariableSymbolReference_p->setAnnotation("xaifBoosterControlFlowReversal::ReversibleControlFlowGraph::makeAuxiliaryIntegerLHS"); 
    theAssignment.getLHS().supplyAndAddVertexInstance(*theVariableSymbolReference_p);
    theAssignment.getLHS().getAliasMapKey().setTemporary();
    theAssignment.getLHS().getDuUdMapKey().setTemporary();
    return &theLhsSymbol_r;
  }


  void ReversibleControlFlowGraph::insert_increment_integer(const Symbol* theIntegerSymbol_p,
							    BasicBlock& theBasicBlock_r) {
    // not active, no algorithm
    Assignment* theAssignment_p=new Assignment(false);
    theAssignment_p->setId(dynamic_cast<const CallGraphAlg&>(ConceptuallyStaticInstances::instance()->getCallGraph().
							     getCallGraphAlgBase()).getAlgorithmSignature() + "increment");
    // set lhs
    VariableSymbolReference* theVariableSymbolReference_p=new VariableSymbolReference(*theIntegerSymbol_p,theBasicBlock_r.getScope());
    theVariableSymbolReference_p->setId("1");
    theVariableSymbolReference_p->setAnnotation("xaifBoosterControlFlowReversal::ReversibleControlFlowGraph::insert_increment_integer(lhs)");
    theAssignment_p->getLHS().supplyAndAddVertexInstance(*theVariableSymbolReference_p);
    theAssignment_p->getLHS().getAliasMapKey().setTemporary();
    theAssignment_p->getLHS().getDuUdMapKey().setTemporary();
    // set rhs
    // counter
    Argument* theUse_p=new Argument(false); // no algorithm
    theUse_p->setId("2");
    theVariableSymbolReference_p=new VariableSymbolReference(*theIntegerSymbol_p,theBasicBlock_r.getScope());
    theVariableSymbolReference_p->setId("1");
    theVariableSymbolReference_p->setAnnotation("xaifBoosterControlFlowReversal::ReversibleControlFlowGraph::insert_increment_integer(counter)");
    theUse_p->getVariable().supplyAndAddVertexInstance(*theVariableSymbolReference_p);
    theUse_p->getVariable().getAliasMapKey().setTemporary();
    theUse_p->getVariable().getDuUdMapKey().setTemporary();
    theAssignment_p->getRHS().supplyAndAddVertexInstance(*theUse_p);
    // 1
    Constant* theOne_p=new Constant(SymbolType::INTEGER_STYPE,false);
    theOne_p->setint(1);
    theOne_p->setId("3");
    theAssignment_p->getRHS().supplyAndAddVertexInstance(*theOne_p);
    // +
    Intrinsic* theAddition_p=new Intrinsic(std::string("add_scal_scal"),false);
    theAddition_p->setId("1");
    theAssignment_p->getRHS().supplyAndAddVertexInstance(*theAddition_p);
    // edges
    ExpressionEdge& theFirstExpressionEdge(theAssignment_p->getRHS().addEdge(*theUse_p,*theAddition_p));
    theFirstExpressionEdge.setId(1);
    theFirstExpressionEdge.setPosition(1);
    ExpressionEdge& theSecondExpressionEdge(theAssignment_p->getRHS().addEdge(*theOne_p,*theAddition_p));
    theSecondExpressionEdge.setId(2);
    theSecondExpressionEdge.setPosition(2);
    // add assignment to basic block
    theBasicBlock_r.supplyAndAddBasicBlockElementInstance(*theAssignment_p); 
  }

  const Symbol* ReversibleControlFlowGraph::insert_init_integer(int value, 
								PlainBasicBlock& theBasicBlock_r) {
    // not active, no algorithm
    Assignment* theAssignment_p=new Assignment(false);
    theAssignment_p->setId(dynamic_cast<const CallGraphAlg&>(ConceptuallyStaticInstances::instance()->getCallGraph().
							     getCallGraphAlgBase()).getAlgorithmSignature() + "init");
    // set lhs
    const Symbol* theLhsSymbol=makeAuxiliaryIntegerLHS(*theAssignment_p,theBasicBlock_r);
    // set rhs
    Constant* theOne=new Constant(SymbolType::INTEGER_STYPE,false);
    theOne->setint(value);
    theOne->setId("1");
    theAssignment_p->getRHS().supplyAndAddVertexInstance(*theOne);
    theBasicBlock_r.supplyAndAddBasicBlockElementInstance(*theAssignment_p); 
    return theLhsSymbol;
  }

  void ReversibleControlFlowGraph::insert_push_integer(const Symbol* theIntegerSymbol_p, 
						       PlainBasicBlock& theBasicBlock_r) {
    xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall* theInlinableSubroutineCall_p = new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall("push_i_"+SymbolShape::toShortString(theIntegerSymbol_p->getSymbolShape()));
    theInlinableSubroutineCall_p->setId(dynamic_cast<const CallGraphAlg&>(ConceptuallyStaticInstances::instance()->getCallGraph().
									  getCallGraphAlgBase()).getAlgorithmSignature() + "push_i");
    Variable theSubstitutionArgument;
    VariableSymbolReference* theVariableSymbolReference_p=new VariableSymbolReference(*theIntegerSymbol_p,theBasicBlock_r.getScope());
    theVariableSymbolReference_p->setId("1");
    theVariableSymbolReference_p->setAnnotation("xaifBoosterControlFlowReversal::ReversibleControlFlowGraph::insert_push_integer");
    theSubstitutionArgument.supplyAndAddVertexInstance(*theVariableSymbolReference_p);
    theSubstitutionArgument.getAliasMapKey().setTemporary();
    theSubstitutionArgument.getDuUdMapKey().setTemporary();
    theSubstitutionArgument.copyMyselfInto(theInlinableSubroutineCall_p->addConcreteArgument(1).getArgument().getVariable());
    theBasicBlock_r.supplyAndAddBasicBlockElementInstance(*theInlinableSubroutineCall_p);
  }

  const Symbol& ReversibleControlFlowGraph::insert_pop_integer(BasicBlock& theBasicBlock_r) {
    xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall* theInlinableSubroutineCall_p = new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall("pop_i_"+SymbolShape::toShortString(SymbolShape::SCALAR));
    theInlinableSubroutineCall_p->setId(dynamic_cast<const CallGraphAlg&>(ConceptuallyStaticInstances::instance()->getCallGraph().
									  getCallGraphAlgBase()).getAlgorithmSignature() + "pop_i");
    const Symbol& theIntegerSymbol(theBasicBlock_r.getScope().getSymbolTable().
				   addUniqueAuxSymbol(SymbolKind::VARIABLE,
						      SymbolType::INTEGER_STYPE,
						      SymbolShape::SCALAR,
						      false));
    VariableSymbolReference* theVariableSymbolReference_p=new VariableSymbolReference(theIntegerSymbol,theBasicBlock_r.getScope());
    theVariableSymbolReference_p->setId("1");
    theVariableSymbolReference_p->setAnnotation("xaifBoosterControlFlowReversal::ReversibleControlFlowGraph::insert_pop_integer");
    Variable theSubstitutionArgument;
    theSubstitutionArgument.supplyAndAddVertexInstance(*theVariableSymbolReference_p);
    theSubstitutionArgument.getAliasMapKey().setTemporary();
    theSubstitutionArgument.getDuUdMapKey().setTemporary();
    theSubstitutionArgument.copyMyselfInto(theInlinableSubroutineCall_p->addConcreteArgument(1).getArgument().getVariable());
    theBasicBlock_r.supplyAndAddBasicBlockElementInstance(*theInlinableSubroutineCall_p);
    return theIntegerSymbol;
  }

  void 
  ReversibleControlFlowGraph::storeControlFlow() {
    std::stack<const Symbol*> theLoopCounterSymbolStack_r;
    std::list<ReversibleControlFlowGraphVertex*>::iterator the_mySortedVertices_p_l_it;
    for (the_mySortedVertices_p_l_it=mySortedVertices_p_l.begin(); 
	 the_mySortedVertices_p_l_it!=mySortedVertices_p_l.end(); 
	 ++the_mySortedVertices_p_l_it) {
      switch ((*the_mySortedVertices_p_l_it)->getOriginalControlFlowGraphVertexAlg().getKind()) {
	/*
	  Both FORLOOP and PRELOOP are control flow merge points,
	  that is they have two predecessors. One of the indeges is a
	  back edge with source of kind ENDLOOP.
	*/
      case ControlFlowGraphVertexAlg::FORLOOP : 
      case ControlFlowGraphVertexAlg::PRELOOP : {
	int endLoopIndex;
	// insert "counter=0" before theCurrentVertex_r
	// push counter symbol onto theLoopCounterSymbolStack_r
	InEdgeIteratorPair pie(getInEdgesOf(*(*the_mySortedVertices_p_l_it)));
	InEdgeIterator beginItie(pie.first),endItie(pie.second);
	/*
	  We need to work on a copy of the inedges because the
	  InEdgeIterator gets messed up when deleting inedges inside
	  of it.
	*/
	std::list<InEdgeIterator> ieil;
	const Symbol* theLoopCounterSymbol_p;
	for (;beginItie!=endItie ;++beginItie) ieil.push_back(beginItie);
	std::list<InEdgeIterator>::iterator ieilIt;
	for (ieilIt=ieil.begin();ieilIt!=ieil.end();++ieilIt) {
// 	  if (DbgLoggerManager::instance()->isSelected(DbgGroup::GRAPHICS)) 
// 	    GraphVizDisplay::show(*this,
// 				  "storeConstruction", 
// 				  ReversibleControlFlowGraphVertexLabelWriter(*this),
// 				  ReversibleControlFlowGraphEdgeLabelWriter(*this));
	  if (&((*the_mySortedVertices_p_l_it)->getCounterPart())!=&(getSourceOf(*(*ieilIt)))) {
	    // this is not the inedge from the corresponding ENDLOOP
	    BasicBlock& theBasicBlock_r(dynamic_cast<BasicBlock&>(insertBasicBlock(getSourceOf(*(*ieilIt)),
										   getTargetOf(*(*ieilIt)),
										   (*(*ieilIt)),
										   false).getNewVertex()));
	    removeAndDeleteEdge(*(*ieilIt));
	    theBasicBlock_r.setId(std::string("_aug_")+makeUniqueVertexId());
	    if ((*the_mySortedVertices_p_l_it)->getReversalType()==ForLoopReversalType::ANONYMOUS
		|| 
		(*the_mySortedVertices_p_l_it)->getOriginalControlFlowGraphVertexAlg().getKind()==ControlFlowGraphVertexAlg::PRELOOP) { 
	      theLoopCounterSymbol_p=insert_init_integer(0,theBasicBlock_r);
	      theLoopCounterSymbolStack_r.push(theLoopCounterSymbol_p);
	    }
	  }  
	  else {
	    // store index of corresponding ENDLOOP node
	    endLoopIndex=getSourceOf(*(*ieilIt)).getIndex();
	  }
	}
	// insert "call push_cfg(counter)" after end of the loop
	// to find the correct outedge we check if the index is 
	// greater than the one of the corresponding ENDLOOP node for
	// all targets. 
	ReversibleControlFlowGraphVertex* thePushBlock_p=0;
	OutEdgeIteratorPair poe(getOutEdgesOf(*(*the_mySortedVertices_p_l_it)));
	OutEdgeIterator beginItoe(poe.first),endItoe(poe.second);
	std::list<OutEdgeIterator> oeil;
	for (;beginItoe!=endItoe ;++beginItoe) oeil.push_back(beginItoe);
	std::list<OutEdgeIterator>::iterator oeilIt;
	for (oeilIt=oeil.begin();oeilIt!=oeil.end();++oeilIt) { 
	  if (getTargetOf(*(*oeilIt)).getIndex()>endLoopIndex) {
	    thePushBlock_p=&(insertBasicBlock(getSourceOf(*(*oeilIt)),
					      getTargetOf(*(*oeilIt)),
					      (*(*oeilIt)),
					      true));
	    BasicBlock& theBasicBlock_r(dynamic_cast<BasicBlock&>(thePushBlock_p->getNewVertex()));
	    (*the_mySortedVertices_p_l_it)->setStorePlaceholder(*thePushBlock_p);
	    removeAndDeleteEdge(*(*oeilIt));
	    theBasicBlock_r.setId(std::string("_aug_")+makeUniqueVertexId());
	    if ((*the_mySortedVertices_p_l_it)->getReversalType()==ForLoopReversalType::ANONYMOUS
		|| 
		(*the_mySortedVertices_p_l_it)->getOriginalControlFlowGraphVertexAlg().getKind()==ControlFlowGraphVertexAlg::PRELOOP) { 
	      insert_push_integer(theLoopCounterSymbol_p,theBasicBlock_r);
	    }
	    break;
	  }
	}
	if ((*the_mySortedVertices_p_l_it)->getReversalType()==ForLoopReversalType::EXPLICIT) { 
	  if (&((*the_mySortedVertices_p_l_it)->getTopExplicitLoop())==(*the_mySortedVertices_p_l_it)) {
	    // this is the top level loop:
	    // associate the placeholder to push address arithmetic variables. 
	    (*the_mySortedVertices_p_l_it)->setTopExplicitLoopAddressArithmetic(*thePushBlock_p);
	  } 
	}
	break;
      } 
      case ControlFlowGraphVertexAlg::ENDLOOP : {
	// pop counter symbol from theLoopCounterSymbolStack_r
	// insert "counter=counter+1" before *(*the_mySortedVertices_p_l_it)
	InEdgeIteratorPair pie(getInEdgesOf(*(*the_mySortedVertices_p_l_it)));
	InEdgeIterator beginItie(pie.first);
	BasicBlock& theBasicBlock_r(dynamic_cast<BasicBlock&>(insertBasicBlock(getSourceOf((*beginItie)),
									       getTargetOf((*beginItie)),
									       (*beginItie),
									       false).getNewVertex()));
	removeAndDeleteEdge((*beginItie));
	theBasicBlock_r.setId(std::string("_aug_")+makeUniqueVertexId());
	if ((*the_mySortedVertices_p_l_it)->getReversalType()==ForLoopReversalType::ANONYMOUS) { 
	  insert_increment_integer(theLoopCounterSymbolStack_r.top(),theBasicBlock_r);
	  theLoopCounterSymbolStack_r.pop();
	}
	break;
      }
      case ControlFlowGraphVertexAlg::ENDBRANCH : {
	if ((*the_mySortedVertices_p_l_it)->getReversalType()==ForLoopReversalType::ANONYMOUS) { 
	  InEdgeIteratorPair pie(getInEdgesOf(*(*the_mySortedVertices_p_l_it)));
	  InEdgeIterator beginItie(pie.first),endItie(pie.second);
	  /*
	    We need to work on a copy of the inedges because the
	    InEdgeIterator gets messed up when deleting inedges inside
	    of it.
	  */
	  std::list<InEdgeIterator> ieil;
	  for (;beginItie!=endItie ;++beginItie) ieil.push_back(beginItie);
	  std::list<InEdgeIterator>::iterator ieilIt;
	  int branch_idx=1;
	  for (ieilIt=ieil.begin();ieilIt!=ieil.end();++ieilIt,branch_idx++) {
	    ReversibleControlFlowGraphVertex& theSource_r(getSourceOf(*(*ieilIt)));
	    ReversibleControlFlowGraphVertex& thePushBlock(insertBasicBlock(theSource_r,
									    getTargetOf(*(*ieilIt)),
									    (*(*ieilIt)),
									    true));
	    // postpone deleting the edge, we still need it
	    BasicBlock& theBasicBlock_r(dynamic_cast<BasicBlock&>(thePushBlock.getNewVertex()));
	    theBasicBlock_r.setId(std::string("_aug_")+makeUniqueVertexId());
	    const Symbol* theLhsSymbol;
	    if (ieil.size()==2) {
	      // this is a true if-then-else
	      if ((*(*ieilIt)).hasRevConditionValue())
		theLhsSymbol=insert_init_integer(1,theBasicBlock_r);
	      else 
		theLhsSymbol=insert_init_integer(0,theBasicBlock_r);
	    }
	    else { 
	      // this is a switch, all of them should have a value assigned now
	      if ((*(*ieilIt)).hasRevConditionValue())
		theLhsSymbol=insert_init_integer((*(*ieilIt)).getRevConditionValue(),theBasicBlock_r);
	      else
		THROW_LOGICEXCEPTION_MACRO("ReversibleControlFlowGraph::storeControlFlow: switch branch exit without condition value");
	    }
	    insert_push_integer(theLhsSymbol,theBasicBlock_r);
	    // here comes the postponed delete of the edge
	    removeAndDeleteEdge(*(*ieilIt));
	  }
	}
	// insert the placeholder for storing miscellaneous items
	// get the one outedge of this vertex
	ReversibleControlFlowGraphEdge& theOutEdge(*(getOutEdgesOf(*(*the_mySortedVertices_p_l_it)).first));
	ReversibleControlFlowGraphVertex& thePushBlock(insertBasicBlock(*(*the_mySortedVertices_p_l_it),
									getTargetOf(theOutEdge),
									theOutEdge,
									true));
	// set the store placeholder in the forloop
	(*the_mySortedVertices_p_l_it)->getCounterPart().setStorePlaceholder(thePushBlock);
	BasicBlock& theBasicBlock_r(dynamic_cast<BasicBlock&>(thePushBlock.getNewVertex()));
	theBasicBlock_r.setId(std::string("_aug_")+makeUniqueVertexId());
	removeAndDeleteEdge(theOutEdge);
	break;
      }
      default : 
	break;
      }
      DBG_MACRO(DbgGroup::DATA,"ReversibleControlFlowGraph::storeControlFlow: ran for vertex "
		<< (*the_mySortedVertices_p_l_it)->debug().c_str());
    }
    // do a second iteration to deal with the enclosed basicblocks
    for (the_mySortedVertices_p_l_it=mySortedVertices_p_l.begin(); 
	 the_mySortedVertices_p_l_it!=mySortedVertices_p_l.end(); 
	 ++the_mySortedVertices_p_l_it) {
      if((*the_mySortedVertices_p_l_it)->getOriginalControlFlowGraphVertexAlg().getKind()== ControlFlowGraphVertexAlg::BASICBLOCK) {
	if ((*the_mySortedVertices_p_l_it)->hasEnclosingControlFlow())
	  (*the_mySortedVertices_p_l_it)->setStorePlaceholder((*the_mySortedVertices_p_l_it)->getEnclosingControlFlow().getStorePlaceholder());
      }
    }
  } 

  void ReversibleControlFlowGraph::storeEnumeratedBB() {
    setUnstructured();
    unsigned short bbEnum=1; // 0 is reserved for the ENTRY
    ReversibleControlFlowGraph::VertexIteratorPair p(vertices());
    ReversibleControlFlowGraph::VertexIterator it(p.first),endIt(p.second);
    for (;it!=endIt ;++it) { 
      if (!(*it).isOriginal())
	continue;
      if((*it).getOriginalControlFlowGraphVertexAlg().getKind()==ControlFlowGraphVertexAlg::ENTRY) {
	// get the out edge from the ENTRY
	ReversibleControlFlowGraphEdge& theEntryOutEdge(*(getOutEdgesOf(*it).first));
	BasicBlock& thePushBasicBlock_r(dynamic_cast<BasicBlock&>(insertBasicBlock(getSourceOf(theEntryOutEdge),
										   getTargetOf(theEntryOutEdge),
										   theEntryOutEdge,
										   false).getNewVertex()));
	removeAndDeleteEdge(theEntryOutEdge);
	thePushBasicBlock_r.setId(std::string("_aug_")+makeUniqueVertexId());
	const Symbol* enumSymb_p=insert_init_integer(0,thePushBasicBlock_r);
	insert_push_integer(enumSymb_p,thePushBasicBlock_r);
      }
      if((*it).getOriginalControlFlowGraphVertexAlg().getKind()==ControlFlowGraphVertexAlg::BASICBLOCK) {
	BasicBlockAlg& theBasicBlockAlg=dynamic_cast<BasicBlockAlg&>((*it).getOriginalControlFlowGraphVertexAlg());
	theBasicBlockAlg.setEnumVal(bbEnum++);
	// get the out edge from the BasicBlock
	ReversibleControlFlowGraphEdge& theOutEdge(*(getOutEdgesOf(*it).first));
	BasicBlock& thePushBasicBlock_r(dynamic_cast<BasicBlock&>(insertBasicBlock(getSourceOf(theOutEdge),
										   getTargetOf(theOutEdge),
										   theOutEdge,
										   false).getNewVertex()));
	removeAndDeleteEdge(theOutEdge);
	thePushBasicBlock_r.setId(std::string("_aug_")+makeUniqueVertexId());
	const Symbol* enumSymb_p=insert_init_integer(theBasicBlockAlg.getEnumVal(),thePushBasicBlock_r);
	insert_push_integer(enumSymb_p,thePushBasicBlock_r);
      }
    }
  } 

  void ReversibleControlFlowGraph::reverseFromEnumeratedBB(ReversibleControlFlowGraph& theAdjointGraph) { 
    // set up structure 
    // new entry
    ReversibleControlFlowGraphVertex* theAdjointEntry_p=theAdjointGraph.new_entry();
    theAdjointEntry_p->getNewVertex().setId(std::string("_adj_")+theAdjointGraph.makeUniqueVertexId());
    // new exit
    ReversibleControlFlowGraphVertex* theAdjointExit_p=theAdjointGraph.new_exit();
    theAdjointExit_p->getNewVertex().setId(std::string("_adj_")+theAdjointGraph.makeUniqueVertexId());
    // new pre loop
    ReversibleControlFlowGraphVertex* theAdjointLoop_p=new ReversibleControlFlowGraphVertex();
    theAdjointGraph.supplyAndAddVertexInstance(*theAdjointLoop_p);
    PreLoop* aNewPreLoop_p=new PreLoop();
    theAdjointLoop_p->supplyAndAddNewVertex(*aNewPreLoop_p);
    aNewPreLoop_p->setAnnotation(dynamic_cast<const CallGraphAlg&>(ConceptuallyStaticInstances::instance()->getCallGraph().getCallGraphAlgBase()).getAlgorithmSignature());
    theAdjointLoop_p->getNewVertex().setId(std::string("_adj_")+theAdjointGraph.makeUniqueVertexId());
    // new end loop
    ReversibleControlFlowGraphVertex* theAdjointEndLoop_p=theAdjointGraph.new_endloop();
    theAdjointEndLoop_p->getNewVertex().setId(std::string("_adj_")+theAdjointGraph.makeUniqueVertexId());
    // new initial pop block
    ReversibleControlFlowGraphVertex* initialPopBlockVertex_p=new ReversibleControlFlowGraphVertex();
    theAdjointGraph.supplyAndAddVertexInstance(*initialPopBlockVertex_p);
    BasicBlock* initialPopBlock_p=new BasicBlock(myOriginalGraph_r.getScope());
    initialPopBlock_p->setId(std::string("_adj_")+theAdjointGraph.makeUniqueVertexId());
    initialPopBlockVertex_p->supplyAndAddNewVertex(*initialPopBlock_p);
    initialPopBlockVertex_p->getNewVertex().setAnnotation(dynamic_cast<const CallGraphAlg&>(ConceptuallyStaticInstances::instance()->getCallGraph().getCallGraphAlgBase()).getAlgorithmSignature());
    const Symbol& thePopSymb=insert_pop_integer(*initialPopBlock_p);
    // set up pre loop condition which is thePopSymb!=0
    Expression& theConditionExpression=aNewPreLoop_p->makeCondition().getExpression();
    BooleanOperation* notEqualOp_p=new BooleanOperation(BooleanOperationType::NOT_EQUAL_OTYPE,false);
    notEqualOp_p->setId(theConditionExpression.getNextVertexId());
    theConditionExpression.supplyAndAddVertexInstance(*notEqualOp_p);
    Argument* theArg_p=new Argument(false);
    theArg_p->setId(theConditionExpression.getNextVertexId());
    theConditionExpression.supplyAndAddVertexInstance(*theArg_p);
    VariableSymbolReference* newVarSymRef_p=new VariableSymbolReference(thePopSymb,
									initialPopBlock_p->getScope());
    theArg_p->getVariable().supplyAndAddVertexInstance(*newVarSymRef_p);
    newVarSymRef_p->setAnnotation(dynamic_cast<const CallGraphAlg&>(ConceptuallyStaticInstances::instance()->getCallGraph().getCallGraphAlgBase()).getAlgorithmSignature());
    newVarSymRef_p->setId(1);
    Constant* theConst_p=new Constant(SymbolType::INTEGER_STYPE,false);
    theConst_p->setId(theConditionExpression.getNextVertexId());
    theConditionExpression.supplyAndAddVertexInstance(*theConst_p);
    theConst_p->setint(0);
    ExpressionEdge* aNewEdge_p=0;
    aNewEdge_p=new ExpressionEdge(false);
    aNewEdge_p->setPosition(1);
    aNewEdge_p->setId(theConditionExpression.getNextEdgeId());
    theConditionExpression.supplyAndAddEdgeInstance(*aNewEdge_p,
						    *theArg_p,
						    *notEqualOp_p);
    aNewEdge_p=new ExpressionEdge(false);
    aNewEdge_p->setPosition(2);
    aNewEdge_p->setId(theConditionExpression.getNextEdgeId());
    theConditionExpression.supplyAndAddEdgeInstance(*aNewEdge_p,
						    *theConst_p,
						    *notEqualOp_p);
    // updating pop block at the end of the loop body
    ReversibleControlFlowGraphVertex* updatingPopBlockVertex_p=new ReversibleControlFlowGraphVertex();
    updatingPopBlockVertex_p->setIndex(theAdjointGraph.numVertices()+1);
    theAdjointGraph.supplyAndAddVertexInstance(*updatingPopBlockVertex_p);
    BasicBlock* updatingPopBlock_p=new BasicBlock(myOriginalGraph_r.getScope());
    updatingPopBlock_p->setId(std::string("_adj_")+theAdjointGraph.makeUniqueVertexId());
    updatingPopBlockVertex_p->supplyAndAddNewVertex(*updatingPopBlock_p);
    updatingPopBlockVertex_p->getNewVertex().setAnnotation(dynamic_cast<const CallGraphAlg&>(ConceptuallyStaticInstances::instance()->getCallGraph().getCallGraphAlgBase()).getAlgorithmSignature());
    // replicate the pop call now as an update
    xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall* theInlinableSubroutineCall_p = new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall("pop_i_"+SymbolShape::toShortString(thePopSymb.getSymbolShape()));
    theInlinableSubroutineCall_p->setId(dynamic_cast<const CallGraphAlg&>(ConceptuallyStaticInstances::instance()->getCallGraph().
									  getCallGraphAlgBase()).getAlgorithmSignature() + "pop_i");
    VariableSymbolReference* theVariableSymbolReference_p=new VariableSymbolReference(thePopSymb,updatingPopBlock_p->getScope());
    theVariableSymbolReference_p->setId("1");
    theVariableSymbolReference_p->setAnnotation("xaifBoosterControlFlowReversal::ReversibleControlFlowGraph::insert_pop_integer");
    Variable theSubstitutionArgument;
    theSubstitutionArgument.supplyAndAddVertexInstance(*theVariableSymbolReference_p);
    theSubstitutionArgument.getAliasMapKey().setTemporary();
    theSubstitutionArgument.getDuUdMapKey().setTemporary();
    theSubstitutionArgument.copyMyselfInto(theInlinableSubroutineCall_p->addConcreteArgument(1).getArgument().getVariable());
    updatingPopBlock_p->supplyAndAddBasicBlockElementInstance(*theInlinableSubroutineCall_p);
    // construct the select statement that is the loop body
    // branch node
    ReversibleControlFlowGraphVertex* theAdjointBranch_p=theAdjointGraph.new_branch();
    theAdjointBranch_p->getNewVertex().setId(std::string("_adj_")+theAdjointGraph.makeUniqueVertexId());
    Expression& theBranchConditionExpression=dynamic_cast<Branch&>(theAdjointBranch_p->getNewVertex()).getCondition().getExpression();
    Argument* theBranchArg_p=new Argument(false);
    theBranchArg_p->setId(theBranchConditionExpression.getNextVertexId());
    theBranchConditionExpression.supplyAndAddVertexInstance(*theBranchArg_p);
    VariableSymbolReference* newBranchVarSymRef_p=new VariableSymbolReference(thePopSymb,
									      initialPopBlock_p->getScope());
    theBranchArg_p->getVariable().supplyAndAddVertexInstance(*newBranchVarSymRef_p);
    newBranchVarSymRef_p->setAnnotation(dynamic_cast<const CallGraphAlg&>(ConceptuallyStaticInstances::instance()->getCallGraph().getCallGraphAlgBase()).getAlgorithmSignature());
    newBranchVarSymRef_p->setId(1);
    // end branch node
    ReversibleControlFlowGraphVertex* theAdjointEndBranch_p=theAdjointGraph.new_endbranch();
    theAdjointEndBranch_p->getNewVertex().setId(std::string("_adj_")+theAdjointGraph.makeUniqueVertexId());
    // take care of all basic blocks now and connect then to branch and end branch
    ReversibleControlFlowGraph::VertexIteratorPair p(vertices());
    ReversibleControlFlowGraph::VertexIterator it(p.first),endIt(p.second);
    for (;it!=endIt ;++it) { 
      if (!(*it).isOriginal())
	continue;
      if((*it).getOriginalControlFlowGraphVertexAlg().getKind()==ControlFlowGraphVertexAlg::BASICBLOCK) {
	BasicBlockAlg& aBasicBlockAlg(dynamic_cast<BasicBlockAlg&>((*it).getOriginalControlFlowGraphVertexAlg()));
	ReversibleControlFlowGraphVertex* adjointBlock_p=
	  theAdjointGraph.old_basic_block(dynamic_cast<const BasicBlock&>(aBasicBlockAlg.BasicBlockAlgBase::getContaining()));
	// branch to block
	ReversibleControlFlowGraphEdge* aNewReversibleControlFlowGraphEdge_p=new ReversibleControlFlowGraphEdge();
	aNewReversibleControlFlowGraphEdge_p->setId(theAdjointGraph.makeUniqueEdgeId());
	aNewReversibleControlFlowGraphEdge_p->setConditionValue(aBasicBlockAlg.getEnumVal());
	theAdjointGraph.supplyAndAddEdgeInstance(*aNewReversibleControlFlowGraphEdge_p,
						 *theAdjointBranch_p,
						 *adjointBlock_p);
	// block to end branch
	aNewReversibleControlFlowGraphEdge_p=new ReversibleControlFlowGraphEdge();
	aNewReversibleControlFlowGraphEdge_p->setId(theAdjointGraph.makeUniqueEdgeId());
	theAdjointGraph.supplyAndAddEdgeInstance(*aNewReversibleControlFlowGraphEdge_p,
						 *adjointBlock_p,
						 *theAdjointEndBranch_p);
      }
    }
    // connect the other CFG vertices
    // entry to initial pop block
    ReversibleControlFlowGraphEdge* aNewReversibleControlFlowGraphEdge_p=new ReversibleControlFlowGraphEdge();
    aNewReversibleControlFlowGraphEdge_p->setId(theAdjointGraph.makeUniqueEdgeId());
    theAdjointGraph.supplyAndAddEdgeInstance(*aNewReversibleControlFlowGraphEdge_p,
					     *theAdjointEntry_p,
					     *initialPopBlockVertex_p);
    // initial pop block to loop
    aNewReversibleControlFlowGraphEdge_p=new ReversibleControlFlowGraphEdge();
    aNewReversibleControlFlowGraphEdge_p->setId(theAdjointGraph.makeUniqueEdgeId());
    theAdjointGraph.supplyAndAddEdgeInstance(*aNewReversibleControlFlowGraphEdge_p,
					     *initialPopBlockVertex_p,
					     *theAdjointLoop_p);
    // loop to branch
    aNewReversibleControlFlowGraphEdge_p=new ReversibleControlFlowGraphEdge();
    aNewReversibleControlFlowGraphEdge_p->setId(theAdjointGraph.makeUniqueEdgeId());
    aNewReversibleControlFlowGraphEdge_p->setConditionValue(1);
    theAdjointGraph.supplyAndAddEdgeInstance(*aNewReversibleControlFlowGraphEdge_p,
					     *theAdjointLoop_p,
					     *theAdjointBranch_p);
    // end branch to update pop
    aNewReversibleControlFlowGraphEdge_p=new ReversibleControlFlowGraphEdge();
    aNewReversibleControlFlowGraphEdge_p->setId(theAdjointGraph.makeUniqueEdgeId());
    theAdjointGraph.supplyAndAddEdgeInstance(*aNewReversibleControlFlowGraphEdge_p,
					     *theAdjointEndBranch_p,
					     *updatingPopBlockVertex_p);
    // update pop to end loop
    aNewReversibleControlFlowGraphEdge_p=new ReversibleControlFlowGraphEdge();
    aNewReversibleControlFlowGraphEdge_p->setId(theAdjointGraph.makeUniqueEdgeId());
    theAdjointGraph.supplyAndAddEdgeInstance(*aNewReversibleControlFlowGraphEdge_p,
					     *updatingPopBlockVertex_p,
					     *theAdjointEndLoop_p);
    // end loop to loop
    aNewReversibleControlFlowGraphEdge_p=new ReversibleControlFlowGraphEdge();
    aNewReversibleControlFlowGraphEdge_p->setId(theAdjointGraph.makeUniqueEdgeId());
    theAdjointGraph.supplyAndAddEdgeInstance(*aNewReversibleControlFlowGraphEdge_p,
					     *theAdjointEndLoop_p,
					     *theAdjointLoop_p);
    // loop to exit
    aNewReversibleControlFlowGraphEdge_p=new ReversibleControlFlowGraphEdge();
    aNewReversibleControlFlowGraphEdge_p->setId(theAdjointGraph.makeUniqueEdgeId());
    aNewReversibleControlFlowGraphEdge_p->setConditionValue(0);
    theAdjointGraph.supplyAndAddEdgeInstance(*aNewReversibleControlFlowGraphEdge_p,
					     *theAdjointLoop_p,
					     *theAdjointExit_p);
  } 

  const ReversibleControlFlowGraphEdge& ReversibleControlFlowGraph::find_corresponding_branch_entry_edge_rec(const ReversibleControlFlowGraphEdge& theCurrentEdge_r, 
													     int& nesting_depth) const {
    const ReversibleControlFlowGraphVertex& theSource_cr(getSourceOf(theCurrentEdge_r));
    if (theSource_cr.isOriginal()&&theSource_cr.getOriginalControlFlowGraphVertexAlg().getKind()==ControlFlowGraphVertexAlg::BRANCH) {
      if (nesting_depth==0) 
        return theCurrentEdge_r;
      else 
        nesting_depth--;
    }
    if (theSource_cr.isOriginal()&&theSource_cr.getOriginalControlFlowGraphVertexAlg().getKind()==ControlFlowGraphVertexAlg::ENDBRANCH) {
      nesting_depth++;
    }
    ConstInEdgeIteratorPair pie(getInEdgesOf(theSource_cr));
    ConstInEdgeIterator beginItie(pie.first),endItie(pie.second);
    for (;beginItie!=endItie ;++beginItie) {
      // We do not want the inedge that emanates from an ENDLOOP to avoid
      // endless looping
      if (getSourceOf(*beginItie).isOriginal()&&getSourceOf(*beginItie).getOriginalControlFlowGraphVertexAlg().getKind()
	  ==
	  ControlFlowGraphVertexAlg::ENDLOOP) 
	continue;
      return find_corresponding_branch_entry_edge_rec(*beginItie, nesting_depth);
    }
    // should never get here, just to calm the compiler...
    return find_corresponding_branch_entry_edge_rec(theCurrentEdge_r, nesting_depth);
  }

  /* 
     The branch entry edges are marked by hasConditionValue()==true and
     a corresponding integer getConditionValue().
     This information is projected onto the branch exit edges.
  */
  void ReversibleControlFlowGraph::markBranchExitEdges() {
    ReversibleControlFlowGraph::VertexIteratorPair p(vertices());
    ReversibleControlFlowGraph::VertexIterator beginIt(p.first),endIt(p.second);
    for (;beginIt!=endIt ;++beginIt) { 
      if ((*beginIt).isOriginal()&&(*beginIt).getOriginalControlFlowGraphVertexAlg().getKind()==ControlFlowGraphVertexAlg::ENDBRANCH) {
        InEdgeIteratorPair pie(getInEdgesOf(*beginIt));
        InEdgeIterator beginItie(pie.first),endItie(pie.second);
	int branchIdx=1;
        for (;beginItie!=endItie ;++beginItie,branchIdx++) {
          int nesting_depth=0; 
          const ReversibleControlFlowGraphEdge& theEntryEdge_cr(find_corresponding_branch_entry_edge_rec((*beginItie),nesting_depth));
	  if (numInEdgesOf(*beginIt)==2) { 
	    // this is a if-then-else branch
	    if (theEntryEdge_cr.hasConditionValue())
	      (*beginItie).setRevConditionValue(theEntryEdge_cr.getConditionValue());
	  }
	  else { 
	    (*beginItie).setRevConditionValue(branchIdx);
	  } 
        }
      }
    }
  }

  const ReversibleControlFlowGraphEdge& ReversibleControlFlowGraph::find_corresponding_branch_exit_edge_rec(const ReversibleControlFlowGraphEdge& theCurrentEdge_r, 
													    int& nesting_depth) const {
    const ReversibleControlFlowGraphVertex& theTarget_cr(getTargetOf(theCurrentEdge_r));
    if (!(theTarget_cr.isOriginal())&&theTarget_cr.getNewControlFlowGraphVertexAlg().getKind()==ControlFlowGraphVertexAlg::ENDBRANCH) {
      if (nesting_depth==0)
        return theCurrentEdge_r;
      else
        nesting_depth--;
    }
    if (!(theTarget_cr.isOriginal())&&theTarget_cr.getNewControlFlowGraphVertexAlg().getKind()==ControlFlowGraphVertexAlg::BRANCH) {
      nesting_depth++;
    }
    ConstOutEdgeIteratorPair pie(getOutEdgesOf(theTarget_cr));
    ConstOutEdgeIterator beginItie(pie.first),endItie(pie.second);
    for (;beginItie!=endItie ;++beginItie) {
      // We do not want the outedge that emanates from an LOOP into its
      // body to avoid endless looping
      if (!(getSourceOf(*beginItie)).isOriginal()&&(
						    getSourceOf(*beginItie).getNewControlFlowGraphVertexAlg().getKind()==ControlFlowGraphVertexAlg::PRELOOP||
						    getSourceOf(*beginItie).getNewControlFlowGraphVertexAlg().getKind()==ControlFlowGraphVertexAlg::FORLOOP)&&
          (*beginItie).hasConditionValue()) continue;
      return find_corresponding_branch_exit_edge_rec(*beginItie, nesting_depth);
    }
    // should never get here, just to calm the compiler...
    return find_corresponding_branch_exit_edge_rec(theCurrentEdge_r, nesting_depth);
  }

  void 
  ReversibleControlFlowGraph::inheritLoopVariables(ForLoopReversalType::ForLoopReversalType_E aReversalType,
						   ReversibleControlFlowGraphVertex& theCurrentVertex_r) { 
    // just ensure the proper logic, i.e. the kind of 
    // vertices we get the inheritance from.
    // this is not intended to be done for the 'end' bits 
    // ENDBRANCH etc. 
    switch (theCurrentVertex_r.getKind()) { 
    case ControlFlowGraphVertexAlg::FORLOOP: 
    case ControlFlowGraphVertexAlg::PRELOOP: 
    case ControlFlowGraphVertexAlg::BASICBLOCK: 
    case ControlFlowGraphVertexAlg::BRANCH: 
      if ( aReversalType==ForLoopReversalType::EXPLICIT) { 
	// iterate through the in edges
	InEdgeIteratorPair anInEdgeIteratorPair(getInEdgesOf(theCurrentVertex_r));
	InEdgeIterator anInEdgeI(anInEdgeIteratorPair.first),anInEdgeIEnd(anInEdgeIteratorPair.second);
	for (;anInEdgeI!=anInEdgeIEnd;++anInEdgeI) { 
	  if ((theCurrentVertex_r.getKind()==ControlFlowGraphVertexAlg::FORLOOP 
	       || 
	       theCurrentVertex_r.getKind()==ControlFlowGraphVertexAlg::PRELOOP)
	      && 
	      getSourceOf(*anInEdgeI).getKind()!=ControlFlowGraphVertexAlg::ENDLOOP) { 
	    theCurrentVertex_r.inheritLoopVariables(getSourceOf(*anInEdgeI));
	    break;
	  } 
	  else { 
	    // all others that we allow after the switch above should have a single in-edge 
	    // always assuming structured graphs
	    theCurrentVertex_r.inheritLoopVariables(getSourceOf(*anInEdgeI));
	  }
	}
      } 
      break;
    case ControlFlowGraphVertexAlg::ENTRY: 
    case ControlFlowGraphVertexAlg::EXIT: 
    case ControlFlowGraphVertexAlg::LABEL: 
    case ControlFlowGraphVertexAlg::GOTO: 
      // do nothing
      break;
    default: 
      THROW_LOGICEXCEPTION_MACRO("ReversibleControlFlowGraph::inheritLoopVariables: cannot use this for "
				 << ControlFlowGraphVertexAlg::kindToString(theCurrentVertex_r.getKind()).c_str());
      break;
    } 
  } 

  /**
   * assign index to current vertex and call on successors
   */
  void
  ReversibleControlFlowGraph::topologicalSortRecursively(ReversibleControlFlowGraphVertex& theCurrentVertex_r, 
							 int& idx,
							 std::stack<ReversibleControlFlowGraphVertex*>& endNodes_p_s_r, 
							 ForLoopReversalType::ForLoopReversalType_E aReversalType,
							 ReversibleControlFlowGraphVertex* aTopExplicitLoopVertex_p,
							 ReversibleControlFlowGraphVertex* enclosingControlFlowVertex_p) {
    if (theCurrentVertex_r.wasVisited()) return;
    theCurrentVertex_r.setVisited();
    // push current node to stack if ENDBRANCH and return
    if (theCurrentVertex_r.getKind()==ControlFlowGraphVertexAlg::ENDBRANCH) {
      endNodes_p_s_r.push(&theCurrentVertex_r);
      return;
    } 
    // set index of current node and increment
    theCurrentVertex_r.setIndex(idx++);
    theCurrentVertex_r.setReversalType(aReversalType);
    if (aTopExplicitLoopVertex_p)
      theCurrentVertex_r.setTopExplicitLoop(*aTopExplicitLoopVertex_p);
    mySortedVertices_p_l.push_back(&theCurrentVertex_r);
    // return if ENDLOOP
    if (theCurrentVertex_r.getKind()==ControlFlowGraphVertexAlg::ENDLOOP) { 
      // the end loop should have exactly one out edge to the loop node:
      if (numOutEdgesOf(theCurrentVertex_r)>1) 
	THROW_LOGICEXCEPTION_MACRO("ReversibleControlFlowGraph::topologicalSortRecursively: ENDLOOP with more than 1 out edge");
      ReversibleControlFlowGraphVertex& theCounterPart(getTargetOf(*(getOutEdgesOf(theCurrentVertex_r).first)));
      theCounterPart.setCounterPart(theCurrentVertex_r);
      theCurrentVertex_r.setCounterPart(theCounterPart);
      theCurrentVertex_r.inheritLoopVariables(theCounterPart);
      if (theCounterPart.hasEnclosingControlFlow())
	theCurrentVertex_r.setEnclosingControlFlow(theCounterPart.getEnclosingControlFlow());
      // the following is particularly for PRELOOPs: 
      theCurrentVertex_r.setReversalType(theCurrentVertex_r.getCounterPart().getReversalType());
      return;
    }
    if (enclosingControlFlowVertex_p)
      theCurrentVertex_r.setEnclosingControlFlow(*enclosingControlFlowVertex_p);
    inheritLoopVariables(aReversalType,theCurrentVertex_r);
    // for loops make sure that loop body is tranversed first
    if (theCurrentVertex_r.getKind()==ControlFlowGraphVertexAlg::PRELOOP||theCurrentVertex_r.getKind()==ControlFlowGraphVertexAlg::FORLOOP) {
      ReversibleControlFlowGraphVertex* aNewEnclosingControlFlowVertex_p=&theCurrentVertex_r;
      ForLoopReversalType::ForLoopReversalType_E aNewReversalType(aReversalType);
      ReversibleControlFlowGraphVertex* aNewTopExplicitLoopVertex_p=aTopExplicitLoopVertex_p;
      // we only require explicit reversal to be specified at the top loop 
      // construct and have to hand it down to all sub graphs
      if (theCurrentVertex_r.getKind()==ControlFlowGraphVertexAlg::FORLOOP 
	  &&
	  myRetainUserReversalFlag
	  &&
	  dynamic_cast<const ForLoop&>(theCurrentVertex_r.getOriginalVertex()).getReversalType()==ForLoopReversalType::EXPLICIT
	  && 
	  !aNewTopExplicitLoopVertex_p) { 
	aNewReversalType=ForLoopReversalType::EXPLICIT;
	aNewTopExplicitLoopVertex_p=&theCurrentVertex_r;
      }
      // reset the reversal type
      if (theCurrentVertex_r.getKind()==ControlFlowGraphVertexAlg::PRELOOP) 
	theCurrentVertex_r.setReversalType(ForLoopReversalType::ANONYMOUS);
      else { 
	theCurrentVertex_r.setReversalType(aNewReversalType);
	if (aNewTopExplicitLoopVertex_p)
	  theCurrentVertex_r.setTopExplicitLoop(*aNewTopExplicitLoopVertex_p);
      }
      if (aNewReversalType==ForLoopReversalType::EXPLICIT 
	  && 
	  theCurrentVertex_r.getKind()==ControlFlowGraphVertexAlg::FORLOOP) { 
	theCurrentVertex_r.addLoopVariable(dynamic_cast<const ForLoop&>(theCurrentVertex_r.getOriginalVertex()).
					   getInitialization().
					   getAssignment().
					   getLHS());
      }
      OutEdgeIteratorPair theCurrentVertex_oeip(getOutEdgesOf(theCurrentVertex_r));
      // sort loop body
      OutEdgeIterator begin_oei_toLoopBody(theCurrentVertex_oeip.first),end_oei_toLoopBody(theCurrentVertex_oeip.second);
      for (;begin_oei_toLoopBody!=end_oei_toLoopBody;++begin_oei_toLoopBody) 
        if ((*begin_oei_toLoopBody).leadsToLoopBody()) 
          topologicalSortRecursively(getTargetOf(*begin_oei_toLoopBody),
				     idx,
				     endNodes_p_s_r,
				     aNewReversalType,
				     aNewTopExplicitLoopVertex_p,
				     aNewEnclosingControlFlowVertex_p); 
      // sort nodes after loop
      OutEdgeIterator begin_oei_toAfterLoop(theCurrentVertex_oeip.first),end_oei_toAfterLoop(theCurrentVertex_oeip.second);
      for (;begin_oei_toAfterLoop!=end_oei_toAfterLoop;++begin_oei_toAfterLoop) 
        if (!(*begin_oei_toAfterLoop).leadsToLoopBody()) 
          topologicalSortRecursively(getTargetOf(*begin_oei_toAfterLoop),
				     idx,
				     endNodes_p_s_r,
				     aReversalType,
				     aTopExplicitLoopVertex_p,
				     enclosingControlFlowVertex_p); 
    }
    else { // go for all successors otherwise
      ReversibleControlFlowGraphVertex* aNewEnclosingControlFlowVertex_p=enclosingControlFlowVertex_p;
      if (theCurrentVertex_r.getKind()==ControlFlowGraphVertexAlg::BRANCH) 
	aNewEnclosingControlFlowVertex_p=&theCurrentVertex_r;
      OutEdgeIteratorPair theCurrentVertex_oeip(getOutEdgesOf(theCurrentVertex_r));
      OutEdgeIterator begin_oei(theCurrentVertex_oeip.first),end_oei(theCurrentVertex_oeip.second);
      for (;begin_oei!=end_oei;++begin_oei) 
        topologicalSortRecursively(getTargetOf(*begin_oei),
				   idx,
				   endNodes_p_s_r,
				   aReversalType,
				   aTopExplicitLoopVertex_p,
				   aNewEnclosingControlFlowVertex_p); 
    }
    // if branch node then handle corresponding end node
    if (theCurrentVertex_r.getKind()==ControlFlowGraphVertexAlg::BRANCH) {
      ReversibleControlFlowGraphVertex* the_endBranch_p=endNodes_p_s_r.top();
      endNodes_p_s_r.pop();
      the_endBranch_p->setIndex(idx++);
      the_endBranch_p->setReversalType(aReversalType);
      the_endBranch_p->setCounterPart(theCurrentVertex_r);
      the_endBranch_p->inheritLoopVariables(theCurrentVertex_r);
      if (aReversalType==ForLoopReversalType::EXPLICIT) { 
	the_endBranch_p->setTopExplicitLoop(theCurrentVertex_r.getTopExplicitLoop());
      }
      if (enclosingControlFlowVertex_p)
	the_endBranch_p->setEnclosingControlFlow(*enclosingControlFlowVertex_p);
      theCurrentVertex_r.setCounterPart(*the_endBranch_p);
      mySortedVertices_p_l.push_back(the_endBranch_p);
      // sort successor  
      OutEdgeIteratorPair theCurrentVertex_oeip(getOutEdgesOf(*(the_endBranch_p)));
      topologicalSortRecursively(getTargetOf(*(theCurrentVertex_oeip.first)),
 				 idx,
 				 endNodes_p_s_r,
 				 aReversalType,
 				 aTopExplicitLoopVertex_p,
				 enclosingControlFlowVertex_p); 
    }
  }

  void ReversibleControlFlowGraph::topologicalSort() {
    initVisit();
    mySortedVertices_p_l.clear();
    int idx=1;
    std::stack<ReversibleControlFlowGraphVertex*> endNodes_p_s;
    topologicalSortRecursively(getEntry(),idx,endNodes_p_s, ForLoopReversalType::ANONYMOUS,0,0);
    finishVisit();
  }

  /*
    Insert edge from theAdjointSource_cr to theAdjointTarget_cr.
    Return reference to the newly created ReversibleControlFlowGraphEdge.
  */
  ReversibleControlFlowGraphEdge&
  ReversibleControlFlowGraph::insertAdjointControlFlowGraphEdge(ReversibleControlFlowGraph& theAdjointControlFlowGraph_r, 
								const ReversibleControlFlowGraphVertex& theAdjointSource_cr, 
								const ReversibleControlFlowGraphVertex& theAdjointTarget_cr) {
    ReversibleControlFlowGraphEdge* aNewReversibleControlFlowGraphEdge_p=new ReversibleControlFlowGraphEdge();
    aNewReversibleControlFlowGraphEdge_p->setId(theAdjointControlFlowGraph_r.makeUniqueEdgeId());
    theAdjointControlFlowGraph_r.supplyAndAddEdgeInstance(*aNewReversibleControlFlowGraphEdge_p,theAdjointSource_cr,theAdjointTarget_cr);
    return *aNewReversibleControlFlowGraphEdge_p;
  }

  ReversibleControlFlowGraphEdge&
  ReversibleControlFlowGraph::addAdjointControlFlowGraphEdge(ReversibleControlFlowGraph& theAdjointControlFlowGraph_r, 
							     const ReversibleControlFlowGraphEdge& theOriginalEdge_cr, 
							     const VertexPPairList& theVertexCorrespondence_ppl) {
    VertexPPairList::const_iterator theVertexCorrespondence_ppl_cit;
    // source and target of adjoint edge to be inserted
    // source is fixed, target depends on kind of original source
    ReversibleControlFlowGraphVertex *theAdjointSource_p, *theAdjointTarget_p;
    // initialize source and target of adjoint edge to be inserted
    bool foundAdjointSource=false;
    bool foundAdjointTarget=false;
    for (theVertexCorrespondence_ppl_cit=theVertexCorrespondence_ppl.begin();
	 theVertexCorrespondence_ppl_cit!=theVertexCorrespondence_ppl.end()
	   &&
	   !(foundAdjointSource&&foundAdjointTarget);
	 theVertexCorrespondence_ppl_cit++) {
      if ((*theVertexCorrespondence_ppl_cit).first==&getTargetOf(theOriginalEdge_cr)) {
        theAdjointSource_p=(*theVertexCorrespondence_ppl_cit).second; 
        foundAdjointSource=true;
      } 
      if ((*theVertexCorrespondence_ppl_cit).first==&getSourceOf(theOriginalEdge_cr)) {
	theAdjointTarget_p=(*theVertexCorrespondence_ppl_cit).second; 
        foundAdjointTarget=true;
      }
    }
    // if this is the original  edge from a loop  vertex to loop construct successor, i.e. not the loop body
    // reset the target to the corresponding  adjoint FORLOOP
    // this is because the FORLOOP turns into an ENDLOOP but that is not the target of the successor in the 
    // reverse.
    if ((getSourceOf(theOriginalEdge_cr).getOriginalControlFlowGraphVertexAlg().getKind()==ControlFlowGraphVertexAlg::FORLOOP
	 ||
	 getSourceOf(theOriginalEdge_cr).getOriginalControlFlowGraphVertexAlg().getKind()==ControlFlowGraphVertexAlg::PRELOOP)
	&&
	!theOriginalEdge_cr.hasConditionValue()) {
      // the original ENDLOOP vertex that matches the original LOOP vertex is the counterPart
      ReversibleControlFlowGraphVertex& theOriginalEndLoop=getSourceOf(theOriginalEdge_cr).getCounterPart();
      // find the adjoint FORLOOP node that corresponds to original ENDLOOP node
      for (theVertexCorrespondence_ppl_cit=theVertexCorrespondence_ppl.begin();
	   theVertexCorrespondence_ppl_cit!=theVertexCorrespondence_ppl.end();
	   ++theVertexCorrespondence_ppl_cit) {
	if ((*theVertexCorrespondence_ppl_cit).first==&theOriginalEndLoop) {
	  theAdjointTarget_p=(*theVertexCorrespondence_ppl_cit).second; 
	  break;
	}
      }
    }
    // insert edge from *theAdjointSource_p to *theAdjointTarget_p
    ReversibleControlFlowGraphEdge& theNewReversibleControlFlowGraphEdge_r(insertAdjointControlFlowGraphEdge(theAdjointControlFlowGraph_r,
													     *theAdjointSource_p,
													     *theAdjointTarget_p));
    // this gives us values on edges that normally don't have conditions but these values 
    // are needed for the branch matches:
    if (theOriginalEdge_cr.hasRevConditionValue())
      theNewReversibleControlFlowGraphEdge_r.setConditionValue(theOriginalEdge_cr.getRevConditionValue());
    //     if (DbgLoggerManager::instance()->isSelected(DbgGroup::GRAPHICS)) {     
    //       GraphVizDisplay::show(theAdjointControlFlowGraph_r,
    // 			    "adjoint_edge_insert", 
    // 			    ReversibleControlFlowGraphVertexLabelWriter(theAdjointControlFlowGraph_r),
    // 			    ReversibleControlFlowGraphEdgeLabelWriter(theAdjointControlFlowGraph_r));
    //     }
    return theNewReversibleControlFlowGraphEdge_r;
  }

  //
  //  Construct the adjoint control flow graph from this graph
  //
  void
  ReversibleControlFlowGraph::buildAdjointControlFlowGraph(ReversibleControlFlowGraph& theAdjointControlFlowGraph_r, bool ourInitializeDerivativeComponentsFlag) {
    //    std::list<std::pair<ReversibleControlFlowGraphVertex*,ReversibleControlFlowGraphVertex*> > theVertexCorrespondence_ppl;
    std::list<ReversibleControlFlowGraphVertex*>::reverse_iterator the_mySortedVertices_p_l_rit;
    for (the_mySortedVertices_p_l_rit=mySortedVertices_p_l.rbegin(); 
	 the_mySortedVertices_p_l_rit!=mySortedVertices_p_l.rend(); 
	 ++the_mySortedVertices_p_l_rit) {
      ReversibleControlFlowGraphVertex* theAdjointVertex_p=0;
      switch ((*the_mySortedVertices_p_l_rit)->getOriginalControlFlowGraphVertexAlg().getKind()) {
      case ControlFlowGraphVertexAlg::EXIT : {
	theAdjointVertex_p=theAdjointControlFlowGraph_r.new_entry();
	theAdjointVertex_p->getNewVertex().setId(std::string("_adj_")+makeUniqueVertexId());
	break;
      }
      case ControlFlowGraphVertexAlg::ENTRY : {
	theAdjointVertex_p=theAdjointControlFlowGraph_r.new_exit();
	theAdjointVertex_p->getNewVertex().setId(std::string("_adj_")+makeUniqueVertexId());
	break;
      }
      case ControlFlowGraphVertexAlg::BRANCH : {
	theAdjointVertex_p=theAdjointControlFlowGraph_r.new_endbranch();
	theAdjointVertex_p->getNewVertex().setId(std::string("_adj_")+makeUniqueVertexId());
	break;
      }
      case ControlFlowGraphVertexAlg::BASICBLOCK : {
	BasicBlockAlg& aBasicBlockAlg(dynamic_cast<BasicBlockAlg&>((*the_mySortedVertices_p_l_rit)->getOriginalControlFlowGraphVertexAlg()));
	theAdjointVertex_p=
	  theAdjointControlFlowGraph_r.old_basic_block(dynamic_cast<const BasicBlock&>(aBasicBlockAlg.BasicBlockAlgBase::getContaining()));
	break;
      }
      case ControlFlowGraphVertexAlg::ENDBRANCH : {
	theAdjointVertex_p=theAdjointControlFlowGraph_r.new_branch();
	theAdjointVertex_p->getNewVertex().setId(std::string("_adj_")+makeUniqueVertexId());
	break;
      }
      case ControlFlowGraphVertexAlg::PRELOOP : 
      case ControlFlowGraphVertexAlg::FORLOOP : {
	theAdjointVertex_p=theAdjointControlFlowGraph_r.new_endloop();
	theAdjointVertex_p->getNewVertex().setId(std::string("_adj_")+makeUniqueVertexId());
	break;
      }
      case ControlFlowGraphVertexAlg::ENDLOOP : {
	switch ((*the_mySortedVertices_p_l_rit)->getCounterPart().getKind()) { 
	case ControlFlowGraphVertexAlg::PRELOOP : { 
	  theAdjointVertex_p=theAdjointControlFlowGraph_r.new_preloop(dynamic_cast<const PreLoop&>((*the_mySortedVertices_p_l_rit)->getCounterPart().getOriginalVertex()).getId());
	  break;
	}
	case ControlFlowGraphVertexAlg::FORLOOP : { 
	  theAdjointVertex_p=theAdjointControlFlowGraph_r.new_forloop((*the_mySortedVertices_p_l_rit)->getReversalType(),
											 dynamic_cast<const ForLoop&>((*the_mySortedVertices_p_l_rit)->getCounterPart().getOriginalVertex()));
	  break; 
	}
	default: 	
	  THROW_LOGICEXCEPTION_MACRO("ReversibleControlFlowGraph::buildAdjointControlFlowGraph: missing logic to handle ENDLOOP counterpart ControlFlowGraphVertex of kind "
				     << ControlFlowGraphVertexAlg::kindToString((*the_mySortedVertices_p_l_rit)->getCounterPart().getKind()).c_str());
	  break; 
	}
	theAdjointVertex_p->getNewVertex().setId(std::string("_adj_")+makeUniqueVertexId());
	break;
      }
      default: 
	THROW_LOGICEXCEPTION_MACRO("ReversibleControlFlowGraph::buildAdjointControlFlowGraph: missing logic to handle ControlFlowGraphVertex of kind "
				   << ControlFlowGraphVertexAlg::kindToString((*the_mySortedVertices_p_l_rit)->getOriginalControlFlowGraphVertexAlg().getKind()).c_str());
	break;
      }
      myOriginalReverseVertexPPairList.push_back(std::make_pair(*the_mySortedVertices_p_l_rit,theAdjointVertex_p)); 
      DBG_MACRO(DbgGroup::DATA, "copying data for index " 
		<< (*the_mySortedVertices_p_l_rit)->getIndex()
		<< " and type "
		<< (*the_mySortedVertices_p_l_rit)->getReversalType());
      theAdjointVertex_p->setAdjointIndex((*the_mySortedVertices_p_l_rit)->getIndex());
      theAdjointVertex_p->setReversalType((*the_mySortedVertices_p_l_rit)->getReversalType());
    }
    // add reversed edges
    VertexPPairList::const_iterator myOriginalReverseVertexPPairList_cit;
    for (myOriginalReverseVertexPPairList_cit=myOriginalReverseVertexPPairList.begin();
	 myOriginalReverseVertexPPairList_cit!=myOriginalReverseVertexPPairList.end();
	 ++myOriginalReverseVertexPPairList_cit) {
      // we switch on the kind of the *adjoint* vertices:
      switch ((*myOriginalReverseVertexPPairList_cit).second->getKind()) {
      case ControlFlowGraphVertexAlg::ENTRY : 
      case ControlFlowGraphVertexAlg::ENDBRANCH : 
      case ControlFlowGraphVertexAlg::BASICBLOCK : {
	InEdgeIteratorPair ieitp(getInEdgesOf(*((*myOriginalReverseVertexPPairList_cit).first)));
	InEdgeIterator begin_ieit(ieitp.first),end_ieit(ieitp.second);
	for (;begin_ieit!=end_ieit ;++begin_ieit) 
	  addAdjointControlFlowGraphEdge(theAdjointControlFlowGraph_r,
					 *begin_ieit,
					 myOriginalReverseVertexPPairList);
	break;
      }
      case ControlFlowGraphVertexAlg::BRANCH : {
	// regardless of the reversal - insert a placeholder block
	InEdgeIteratorPair singleInEdge_ieitp(getInEdgesOf(*((*myOriginalReverseVertexPPairList_cit).second)));
	ReversibleControlFlowGraphVertex& thePopBlock(theAdjointControlFlowGraph_r.insertBasicBlock(getSourceOf(*(singleInEdge_ieitp.first)),
												    *((*myOriginalReverseVertexPPairList_cit).second),
												    *(singleInEdge_ieitp.first),
												    false));
	removeAndDeleteEdge(*(singleInEdge_ieitp.first));
	(*((*myOriginalReverseVertexPPairList_cit).second)).setRestorePlaceholder(thePopBlock);
	BasicBlock& theNewBasicBlock_r(dynamic_cast<BasicBlock&>(thePopBlock.getNewVertex()));
	theNewBasicBlock_r.setId(std::string("_adj_")+makeUniqueVertexId());
	if ((*myOriginalReverseVertexPPairList_cit).second->getReversalType()==ForLoopReversalType::ANONYMOUS) { 
	  // insert pop() in front
	  const Symbol& thePoppedIntegerSymbol_cr(theAdjointControlFlowGraph_r.insert_pop_integer(theNewBasicBlock_r));
	  // add thePoppedIntegerSymbol_cr to branch condition as select value
	  Expression& theSelectExpression_r(dynamic_cast<Branch&>((*myOriginalReverseVertexPPairList_cit).second->getNewVertex()).getCondition().getExpression());
	  Argument* theArgument_p=new Argument();
	  theArgument_p->setId("1");
	  theSelectExpression_r.supplyAndAddVertexInstance(*theArgument_p);  
	  theArgument_p->getVariable().getAliasMapKey().setTemporary();
	  theArgument_p->getVariable().getDuUdMapKey().setTemporary();
	  VariableSymbolReference* theSelectVariableSymbolReference_p=new VariableSymbolReference(thePoppedIntegerSymbol_cr,theNewBasicBlock_r.getScope());
	  theArgument_p->getVariable().supplyAndAddVertexInstance(*theSelectVariableSymbolReference_p);
	  theSelectVariableSymbolReference_p->setId("1");
	  theSelectVariableSymbolReference_p->setAnnotation("xaifBoosterControlFlowReversal::ReversibleControlFlowGraph::buildAdjointControlFlowGraph(BRANCH)");
	} 
	else { // explicit reversal
	  // maintain the original condition.
	  // this is the adjoint of the ENDBRANCH, i.e. we need to go to the 
	  // counterpart of the original vertex
	  // retrieve the condition expression of the new vertex:
	  Expression& theConditionExpression(dynamic_cast<Branch&>((*myOriginalReverseVertexPPairList_cit).second->
								   getNewVertex()).getCondition().getExpression());
	  dynamic_cast<const Branch&>((*myOriginalReverseVertexPPairList_cit).first->getCounterPart().getOriginalVertex()).
	    getCondition().getExpression().copyMyselfInto(theConditionExpression,false,false);
	}
	// insert adjoint edges
	InEdgeIteratorPair ieitp(getInEdgesOf(*((*myOriginalReverseVertexPPairList_cit).first)));
	InEdgeIterator begin_ieit(ieitp.first),end_ieit(ieitp.second);
	for (;begin_ieit!=end_ieit ; ++begin_ieit) 
	  addAdjointControlFlowGraphEdge(theAdjointControlFlowGraph_r,*begin_ieit,myOriginalReverseVertexPPairList);
	break;
      }
      case ControlFlowGraphVertexAlg::FORLOOP : {
	// regardless of the reversal - insert a placeholder block
	InEdgeIteratorPair singleInEdge_ieitp(getInEdgesOf(*((*myOriginalReverseVertexPPairList_cit).second)));
	if (singleInEdge_ieitp.first==singleInEdge_ieitp.second) { 
	  THROW_LOGICEXCEPTION_MACRO("ReversibleControlFlowGraph::buildAdjointControlFlowGraph: bad CFG (likely not well structured)");
	}
	ReversibleControlFlowGraphVertex& thePopBlock(theAdjointControlFlowGraph_r.insertBasicBlock(getSourceOf(*(singleInEdge_ieitp.first)),
												    *((*myOriginalReverseVertexPPairList_cit).second),
												    *(singleInEdge_ieitp.first),
												    false));
	removeAndDeleteEdge(*(singleInEdge_ieitp.first));
	(*((*myOriginalReverseVertexPPairList_cit).second)).setRestorePlaceholder(thePopBlock);
	BasicBlock& theNewBasicBlock_r(dynamic_cast<BasicBlock&>(thePopBlock.getNewVertex()));
	theNewBasicBlock_r.setId(std::string("_adj_")+makeUniqueVertexId());
	if ((*myOriginalReverseVertexPPairList_cit).second->getReversalType()==ForLoopReversalType::ANONYMOUS) {
	  // 	  if (DbgLoggerManager::instance()->isSelected(DbgGroup::GRAPHICS)) {     
	  // 	    GraphVizDisplay::show(theAdjointControlFlowGraph_r,
	  // 				  "adjoint_beforeEntryMark", 
	  // 				  ReversibleControlFlowGraphVertexLabelWriter(theAdjointControlFlowGraph_r),
	  // 				  ReversibleControlFlowGraphEdgeLabelWriter(theAdjointControlFlowGraph_r));
	  // 	  }
	  // insert pop() in front
	  const Symbol& thePoppedIntegerSymbol_cr(theAdjointControlFlowGraph_r.insert_pop_integer(theNewBasicBlock_r));
	  // fill ForLoop  
	  ForLoop& theForLoop_r(dynamic_cast<ForLoop&>((*myOriginalReverseVertexPPairList_cit).second->getNewVertex()));
	  // initialization
	  // set lhs
	  const Symbol* theLoopCounterSymbol_p=makeAuxiliaryIntegerLHS(theForLoop_r.getInitialization().getAssignment(),theNewBasicBlock_r);
	  // set rhs
	  Constant* theOne=new Constant(SymbolType::INTEGER_STYPE,false);
	  theOne->setint(1);
	  theOne->setId("1");
	  theForLoop_r.getInitialization().getAssignment().getRHS().supplyAndAddVertexInstance(*theOne);
	  // condition
	  // counter
	  Argument* theLoopCounterUse_p=new Argument(false); // no algorithm
	  theLoopCounterUse_p->setId("2");
	  VariableSymbolReference* theVariableSymbolReference_p=new VariableSymbolReference(*theLoopCounterSymbol_p,theNewBasicBlock_r.getScope());
	  theVariableSymbolReference_p->setId("1");
	  theVariableSymbolReference_p->setAnnotation("xaifBoosterControlFlowReversal::ReversibleControlFlowGraph::buildAdjointControlFlowGraph(FORLOOP/COUNTER)");
	  theLoopCounterUse_p->getVariable().supplyAndAddVertexInstance(*theVariableSymbolReference_p);
	  theLoopCounterUse_p->getVariable().getAliasMapKey().setTemporary();
	  theLoopCounterUse_p->getVariable().getDuUdMapKey().setTemporary();
	  theForLoop_r.getCondition().getExpression().supplyAndAddVertexInstance(*theLoopCounterUse_p);
	  // the popped integer symbol
	  Argument* thePoppedIntegerUse_p=new Argument(false); // no algorithm
	  thePoppedIntegerUse_p->setId("3");
	  theVariableSymbolReference_p=new VariableSymbolReference(thePoppedIntegerSymbol_cr,theNewBasicBlock_r.getScope());
	  theVariableSymbolReference_p->setId("1");
	  theVariableSymbolReference_p->setAnnotation("xaifBoosterControlFlowReversal::ReversibleControlFlowGraph::buildAdjointControlFlowGraph(FORLOOP/POP_INTEGER)");
	  thePoppedIntegerUse_p->getVariable().supplyAndAddVertexInstance(*theVariableSymbolReference_p);
	  thePoppedIntegerUse_p->getVariable().getAliasMapKey().setTemporary();
	  thePoppedIntegerUse_p->getVariable().getDuUdMapKey().setTemporary();
	  theForLoop_r.getCondition().getExpression().supplyAndAddVertexInstance(*thePoppedIntegerUse_p);
	  // <=
	  BooleanOperation* theLessThanOrEqualOperator_p=new BooleanOperation(BooleanOperationType::LESS_OR_EQUAL_OTYPE,false);
	  theLessThanOrEqualOperator_p->setId("1");
	  theForLoop_r.getCondition().getExpression().supplyAndAddVertexInstance(*theLessThanOrEqualOperator_p);
	  // edges
	  ExpressionEdge& theFirstExpressionEdge(theForLoop_r.getCondition().getExpression().addEdge(*theLoopCounterUse_p,*theLessThanOrEqualOperator_p));
	  theFirstExpressionEdge.setId(1);
	  theFirstExpressionEdge.setPosition(1);
	  ExpressionEdge& theSecondExpressionEdge(theForLoop_r.getCondition().getExpression().addEdge(*thePoppedIntegerUse_p,*theLessThanOrEqualOperator_p));
	  theSecondExpressionEdge.setId(2);
	  theSecondExpressionEdge.setPosition(2);
	  // update
	  // set lhs
	  theVariableSymbolReference_p=new VariableSymbolReference(*theLoopCounterSymbol_p,theNewBasicBlock_r.getScope());
	  theVariableSymbolReference_p->setId("1");
	  theVariableSymbolReference_p->setAnnotation("xaifBoosterControlFlowReversal::ReversibleControlFlowGraph::buildAdjointControlFlowGraph(FORLOOP/LHS)");
	  theForLoop_r.getUpdate().getAssignment().getLHS().supplyAndAddVertexInstance(*theVariableSymbolReference_p);
	  theForLoop_r.getUpdate().getAssignment().getLHS().getAliasMapKey().setTemporary();
	  theForLoop_r.getUpdate().getAssignment().getLHS().getDuUdMapKey().setTemporary();
	  // set rhs
	  // counter
	  Argument* theUse_p=new Argument(false); // no algorithm
	  theUse_p->setId("2");
	  theVariableSymbolReference_p=new VariableSymbolReference(*theLoopCounterSymbol_p,theNewBasicBlock_r.getScope());
	  theVariableSymbolReference_p->setId("1");
	  theVariableSymbolReference_p->setAnnotation("xaifBoosterControlFlowReversal::ReversibleControlFlowGraph::buildAdjointControlFlowGraph(FORLOOP/RHS)");
	  theUse_p->getVariable().supplyAndAddVertexInstance(*theVariableSymbolReference_p);
	  theUse_p->getVariable().getAliasMapKey().setTemporary();
	  theUse_p->getVariable().getDuUdMapKey().setTemporary();
	  theForLoop_r.getUpdate().getAssignment().getRHS().supplyAndAddVertexInstance(*theUse_p);
	  // 1
	  Constant* theOne_p=new Constant(SymbolType::INTEGER_STYPE,false);
	  theOne_p->setint(1);
	  theOne_p->setId("3");
	  theForLoop_r.getUpdate().getAssignment().getRHS().supplyAndAddVertexInstance(*theOne_p);
	  // +
	  Intrinsic* theAddition_p=new Intrinsic(std::string("add_scal_scal"),false);
	  theAddition_p->setId("1");
	  theForLoop_r.getUpdate().getAssignment().getRHS().supplyAndAddVertexInstance(*theAddition_p);
	  // edges
	  ExpressionEdge& theFirstUpdateEdge(theForLoop_r.getUpdate().getAssignment().getRHS().addEdge(*theUse_p,*theAddition_p));
	  theFirstUpdateEdge.setId(1);
	  theFirstUpdateEdge.setPosition(1);
	  ExpressionEdge& theSecondUpdateEdge(theForLoop_r.getUpdate().getAssignment().getRHS().addEdge(*theOne_p,*theAddition_p));
	  theSecondUpdateEdge.setId(2);
	  theSecondUpdateEdge.setPosition(2);
	} 
	else { // explicit reversal  of a FORLOOP
	  // the old ForLoop is the counter part of the ENDLOOP that is the original to this vertex 
	  if (&((*myOriginalReverseVertexPPairList_cit).first->getCounterPart().getTopExplicitLoop())
	      ==
	      &((*myOriginalReverseVertexPPairList_cit).first->getCounterPart())) { 
	    // this is the top level one, insert a placeholder for address arithmetic pops
	    InEdgeIteratorPair singleInEdge_ieitp(getInEdgesOf(*((*myOriginalReverseVertexPPairList_cit).second)));
	    (*myOriginalReverseVertexPPairList_cit).second->
	      setTopExplicitLoopAddressArithmetic(theAdjointControlFlowGraph_r.insertBasicBlock(getSourceOf(*(singleInEdge_ieitp.first)),
												*((*myOriginalReverseVertexPPairList_cit).second),
												*(singleInEdge_ieitp.first),
												false));
	    BasicBlock& theBasicBlock_r(dynamic_cast<BasicBlock&>((*myOriginalReverseVertexPPairList_cit).second->getTopExplicitLoopAddressArithmetic().getNewVertex()));
	    theBasicBlock_r.setId(std::string("_adj__AddressArithmetic_")+makeUniqueVertexId());
	    removeAndDeleteEdge(*(singleInEdge_ieitp.first));
	  }     
	  // the old ForLoop is the counter part of the ENDLOOP that is the original to this vertex 
	  const ForLoop& theOldForLoop(dynamic_cast<const ForLoop&>((*myOriginalReverseVertexPPairList_cit).first->getCounterPart().getOriginalVertex()));
	  // figure out if we count up or down
	  const Expression& theOldUpdateRHS(theOldForLoop.getUpdate().getAssignment().getRHS());
	  // require simple expression of the form i+c or i-c where c is a constant
	  // needs 3 vertices
	  if (theOldUpdateRHS.numVertices()!=3) 
	    THROW_LOGICEXCEPTION_MACRO("ReversibleControlFlowGraph::buildAdjointControlFlowGraph: an explicit reversal loop update should be of the form i+c or i-c");
	  // find the intrinsic
	  const Intrinsic* theOldUpdateIntrinsic_p(dynamic_cast<const Intrinsic*>(&(theOldUpdateRHS.getMaxVertex())));
	  if (!theOldUpdateIntrinsic_p) { 
	    THROW_LOGICEXCEPTION_MACRO("ReversibleControlFlowGraph::buildAdjointControlFlowGraph: could not find loop update intrinsic");
	  }
	  // what is it?
	  ForLoopDirection::ForLoopDirection_E loopDir=
	    (*myOriginalReverseVertexPPairList_cit).first->getCounterPart().simpleCountUp();
	  // the new ForLoop  
	  ForLoop& theNewForLoop(dynamic_cast<ForLoop&>((*myOriginalReverseVertexPPairList_cit).second->getNewVertex()));
	  makeLoopExplicitReversalInitialization(theOldForLoop,
						 theNewForLoop,
						 loopDir);
	  makeLoopExplicitReversalCondition(theOldForLoop,
					    theNewForLoop,
					    loopDir);
	  makeLoopExplicitReversalUpdate(theOldForLoop,
					 theNewForLoop);
	}
	// insert adjoint edges
	InEdgeIteratorPair theEndLoop_ieitp(getInEdgesOf(*((*myOriginalReverseVertexPPairList_cit).first)));
	ReversibleControlFlowGraphEdge& theNewReversibleControlFlowGraphEdge_r(addAdjointControlFlowGraphEdge(theAdjointControlFlowGraph_r,
													      *(theEndLoop_ieitp.first),
													      myOriginalReverseVertexPPairList));
	// set condition value to true because this is a loop body entry edge
	theNewReversibleControlFlowGraphEdge_r.setConditionValue(1);

	// insert edge from current adjoint node to adjoint of the
	// predecessor of the LOOP node that matches the original
	// (LOOP) of the original (ENDLOOP) of the current adjoint node 
	ReversibleControlFlowGraphVertex *theAdjointSource_p, *theAdjointTarget_p;
	VertexPPairList::const_iterator myOriginalReverseVertexPPairList_cit_1;
	for (myOriginalReverseVertexPPairList_cit_1=myOriginalReverseVertexPPairList.begin();myOriginalReverseVertexPPairList_cit_1!=myOriginalReverseVertexPPairList.end();myOriginalReverseVertexPPairList_cit_1++) 
	  if ((*myOriginalReverseVertexPPairList_cit_1).first==(*myOriginalReverseVertexPPairList_cit).first) {
	    theAdjointSource_p=(*myOriginalReverseVertexPPairList_cit_1).second; 
	    break;
	  } 
	// get the outedges of the original ENDLOOP of which there is only one and that leads to the 
	// original FORLOOP
	OutEdgeIteratorPair toLoop_oeitp(getOutEdgesOf(*((*myOriginalReverseVertexPPairList_cit).first)));
	// the "second" in the pair is the adjoint FORLOOP, 
	// the "first" in the pair is the original ENDLOOP which has the original FORLOOP as counterpart
	// we get the in edges of the original corresponding FORLOOP
	InEdgeIteratorPair ieitp(getInEdgesOf((*myOriginalReverseVertexPPairList_cit).first->getCounterPart()));
	InEdgeIterator begin_ieit(ieitp.first),end_ieit(ieitp.second);
	for (;begin_ieit!=end_ieit ;++begin_ieit) { 
	  if ((*myOriginalReverseVertexPPairList_cit).first!=&(getSourceOf(*begin_ieit))) {
	    for (myOriginalReverseVertexPPairList_cit_1=myOriginalReverseVertexPPairList.begin();myOriginalReverseVertexPPairList_cit_1!=myOriginalReverseVertexPPairList.end();myOriginalReverseVertexPPairList_cit_1++) { 
	      if ((*myOriginalReverseVertexPPairList_cit_1).first==&getSourceOf(*begin_ieit)) {
		theAdjointTarget_p=(*myOriginalReverseVertexPPairList_cit_1).second; 
		break;
	      } 
	    }
	    break;
	  } 
	}
	insertAdjointControlFlowGraphEdge(theAdjointControlFlowGraph_r,*theAdjointSource_p,*theAdjointTarget_p);
	break;
      }
      case ControlFlowGraphVertexAlg::ENDLOOP : {
	// Insert back-edge from this adjoint ENDLOOP to the matching adjoint FORLOOP vertex.
	// The original vertex corresponding to this adjoint ENDLOOP is a LOOP. 
	// We need to find the adjoint vertex corresponding to the
	// original ENDLOOP counterPart. 
	// find adjoint target as the adjoint to the counterpart
	ReversibleControlFlowGraphVertex* theAdjointTarget_p(0);
	ReversibleControlFlowGraphVertex& theOriginalEndLoop((*myOriginalReverseVertexPPairList_cit).first->getCounterPart());
	for (VertexPPairList::const_iterator myOriginalReverseVertexPPairList_cit_1=myOriginalReverseVertexPPairList.begin();
	     myOriginalReverseVertexPPairList_cit_1!=myOriginalReverseVertexPPairList.end();
	     myOriginalReverseVertexPPairList_cit_1++) { 
	  if ((*myOriginalReverseVertexPPairList_cit_1).first==&theOriginalEndLoop) {
	    theAdjointTarget_p=(*myOriginalReverseVertexPPairList_cit_1).second; 
	    break;
	  } 
	}
	if (!theAdjointTarget_p)
	  THROW_LOGICEXCEPTION_MACRO("ReversibleControlFlowGraph::buildAdjointControlFlowGraph: adjoint ENDLOOP counterpart not found");
	insertAdjointControlFlowGraphEdge(theAdjointControlFlowGraph_r,
					  *(*myOriginalReverseVertexPPairList_cit).second,
					  *theAdjointTarget_p);
	break;
      }
      case ControlFlowGraphVertexAlg::EXIT : 
	// do nothing
	break;
      default: 
	THROW_LOGICEXCEPTION_MACRO("ReversibleControlFlowGraph::buildAdjointControlFlowGraph: missing logic to handle ControlFlowGraphVertex of kind "
				   << ControlFlowGraphVertexAlg::kindToString((*myOriginalReverseVertexPPairList_cit).second->getKind()).c_str());
	break; 
      }
      if (ourInitializeDerivativeComponentsFlag) {
	if ((*myOriginalReverseVertexPPairList_cit).second->getKind() == ControlFlowGraphVertexAlg::ENTRY) {
	  try {
	    ReversibleControlFlowGraphVertex& entryVertex = *((*myOriginalReverseVertexPPairList_cit).second);
	    ReversibleControlFlowGraphEdge& theOutEdge(*(getOutEdgesOf(entryVertex).first));	  
	    ReversibleControlFlowGraphVertex& theTargetVertex = getTargetOf(theOutEdge);
	    ReversibleControlFlowGraphVertex& theInitBlock = 
	      theAdjointControlFlowGraph_r.insertBasicBlock(entryVertex,
							    theTargetVertex,
							    theOutEdge,
							    false);
	    removeAndDeleteEdge(theOutEdge);
	    (*((*myOriginalReverseVertexPPairList_cit).second)).setRestorePlaceholder(theInitBlock);
	    BasicBlock& theNewBasicBlock_r(dynamic_cast<BasicBlock&>(theInitBlock.getNewVertex()));
	    theNewBasicBlock_r.setId(std::string("_adj_")+makeUniqueVertexId());
	    
	    ReversibleControlFlowGraph::initializeActiveVariables(&theNewBasicBlock_r);
	  } 
	  catch (LogicException) {
	  }
	}
      }
    }
    // go once more through the basic blocks
    for (myOriginalReverseVertexPPairList_cit=myOriginalReverseVertexPPairList.begin();
	 myOriginalReverseVertexPPairList_cit!=myOriginalReverseVertexPPairList.end();
	 ++myOriginalReverseVertexPPairList_cit) {
      if((*myOriginalReverseVertexPPairList_cit).second->getKind()==ControlFlowGraphVertexAlg::BASICBLOCK) {
	if ((*myOriginalReverseVertexPPairList_cit).first->hasEnclosingControlFlow()) { 
	  ReversibleControlFlowGraphVertex& theOriginalCounterPart((*myOriginalReverseVertexPPairList_cit).first->getEnclosingControlFlow().getCounterPart());
	  for (VertexPPairList::const_iterator myOriginalReverseVertexPPairList_cit_1=myOriginalReverseVertexPPairList.begin();
	       myOriginalReverseVertexPPairList_cit_1!=myOriginalReverseVertexPPairList.end();
	       myOriginalReverseVertexPPairList_cit_1++) { 
	    if ((*myOriginalReverseVertexPPairList_cit_1).first==&theOriginalCounterPart) {
	      (*myOriginalReverseVertexPPairList_cit).second->setRestorePlaceholder((*myOriginalReverseVertexPPairList_cit_1).second->getRestorePlaceholder());
	      break;
	    } 
	  }
	}
      }
    }
  }

  void ReversibleControlFlowGraph::printXMLHierarchy(std::ostream& os) const {
    ReversibleControlFlowGraph::ConstVertexIteratorPair p(vertices());
    ReversibleControlFlowGraph::ConstVertexIterator beginIt(p.first),endIt(p.second);
    for (;beginIt!=endIt ;++beginIt)
      (*beginIt).printXMLHierarchy(os);
    ReversibleControlFlowGraph::ConstEdgeIteratorPair pe=edges();
    ReversibleControlFlowGraph::ConstEdgeIterator beginIte(pe.first),endIte(pe.second);
    for (;beginIte!=endIte ;++beginIte)
      (*beginIte).printXMLHierarchy(os,*this);
  }

  std::string
  ReversibleControlFlowGraph::debug() const {
    std::ostringstream out;
    out << "xaifBoosterControlFlowReversal::ControlFlowGraph["
	<< this
	<< "]" << std::ends;
    return out.str();
  }

  void ReversibleControlFlowGraph::traverseToChildren(const GenericAction::GenericAction_E anAction_c) {
  }

  void ReversibleControlFlowGraph::makeLoopExplicitReversalInitialization(const ForLoop& theOldForLoop,
									  ForLoop& theNewForLoop,
									  ForLoopDirection::ForLoopDirection_E loopDir) { 
    // initialization depends on the original condition which should be of the form i <operator> e with e as a expression of known variables
    // some setup 
    const Expression& theOldUpdateRHS(theOldForLoop.getUpdate().getAssignment().getRHS());
    const Intrinsic* theOldUpdateIntrinsic_p(dynamic_cast<const Intrinsic*>(&(theOldUpdateRHS.getMaxVertex())));
    if (!theOldUpdateIntrinsic_p) { 
      THROW_LOGICEXCEPTION_MACRO("ReversibleControlFlowGraph::makeLoopExplicitReversalInitialization: could not find loop update intrinsic");
    }
    const Expression& theOldConditionExpr(theOldForLoop.getCondition().getExpression());
    // find the maximal vertex in the old condition which should be the boolean operation 
    // which determines the behavior
    const ExpressionVertex& theOldConditionMaxVertex(theOldConditionExpr.getMaxVertex());
    // find the BooleanOperation
    const BooleanOperation* theOldConditionBooleanOperation_p(dynamic_cast<const BooleanOperation*>(&theOldConditionMaxVertex));
    if (!theOldConditionBooleanOperation_p) { 
      THROW_LOGICEXCEPTION_MACRO("ReversibleControlFlowGraph::makeLoopExplicitReversalInitialization: could not find loop condition boolean operation");
    }
    // copy the init assignment LHS from the old one
    theOldForLoop.getInitialization().getAssignment().getLHS().copyMyselfInto(theNewForLoop.getInitialization().getAssignment().getLHS());
    // get the new RHS
    Expression& theNewInitializationRHS(theNewForLoop.getInitialization().getAssignment().getRHS());
    // copy the old one into it
    theOldForLoop.getInitialization().getAssignment().getRHS().copyMyselfInto(theNewInitializationRHS, true, false);
    ExpressionVertex& theNewInitMaxVertex(theNewInitializationRHS.getMaxVertex());
    // add the looping amount which is bound plus/minus init divided by update
    // e.g. reverseInit=oldInit+oldUpdate*((oldBound-oldInit)/update)
    Intrinsic* newInitTop_p;
    if (loopDir==ForLoopDirection::COUNT_UP || loopDir==ForLoopDirection::COUNT_UNDECIDED )
      newInitTop_p=new Intrinsic("add_scal_scal");
    else 
      newInitTop_p=new Intrinsic("sub_scal_scal");
    newInitTop_p->setId(theNewInitializationRHS.getNextVertexId());
    theNewInitializationRHS.supplyAndAddVertexInstance(*newInitTop_p);
    // add the edge from the max of the old init to the top
    ExpressionEdge* newEdge_p=new ExpressionEdge(false);
    newEdge_p->setId(theNewInitializationRHS.getNextEdgeId());
    newEdge_p->setPosition(1);
    theNewInitializationRHS.supplyAndAddEdgeInstance(*newEdge_p,theNewInitMaxVertex,*newInitTop_p);
    // the multiplication op
    Intrinsic& aMult(*(new Intrinsic("mul_scal_scal")));
    aMult.setId(theNewInitializationRHS.getNextVertexId());
    theNewInitializationRHS.supplyAndAddVertexInstance(aMult);
    // add an edge from the multiplication to the top
    newEdge_p=new ExpressionEdge(false);
    newEdge_p->setId(theNewInitializationRHS.getNextEdgeId());
    newEdge_p->setPosition(2);
    theNewInitializationRHS.supplyAndAddEdgeInstance(*newEdge_p,aMult,*newInitTop_p);
    // 1st argument to the multiplication is the constant in the old update which we expect to be 
    // the second argument of the top level intrinsic:
    // copy this into the new init
    ExpressionVertex& 
      theTopOftheUpdate(theOldUpdateRHS.copySubExpressionInto(theNewInitializationRHS,
							      theOldUpdateRHS.findPositionalSubExpressionOf(theOldUpdateRHS.getMaxVertex(),2),
							      true,
							      false));
    // make the edge to the multiplication op
    newEdge_p=new ExpressionEdge(false);
    newEdge_p->setId(theNewInitializationRHS.getNextEdgeId());
    newEdge_p->setPosition(1);
    theNewInitializationRHS.supplyAndAddEdgeInstance(*newEdge_p,theTopOftheUpdate,aMult);
    // 2nd argument to the multiplication is the integer division operation
    Intrinsic& aMod(*(new Intrinsic("div_scal_scal")));
    aMod.setId(theNewInitializationRHS.getNextVertexId());
    theNewInitializationRHS.supplyAndAddVertexInstance(aMod);
    if (loopDir==ForLoopDirection::COUNT_UNDECIDED ) { 
      // wrap the division part into an ABS call
      Intrinsic& anAbs(*(new Intrinsic("abs_scal")));
      anAbs.setId(theNewInitializationRHS.getNextVertexId());
      theNewInitializationRHS.supplyAndAddVertexInstance(anAbs);
      // add an edge from the division to the abs
      newEdge_p=new ExpressionEdge(false);
      newEdge_p->setId(theNewInitializationRHS.getNextEdgeId());
      newEdge_p->setPosition(1);
      theNewInitializationRHS.supplyAndAddEdgeInstance(*newEdge_p,aMod,anAbs);
      // add an edge from the abs to the multiplication
      newEdge_p=new ExpressionEdge(false);
      newEdge_p->setId(theNewInitializationRHS.getNextEdgeId());
      newEdge_p->setPosition(2);
      theNewInitializationRHS.supplyAndAddEdgeInstance(*newEdge_p,anAbs,aMult);
    } 
    else { 
      // add an edge from the division to the multiplication
      newEdge_p=new ExpressionEdge(false);
      newEdge_p->setId(theNewInitializationRHS.getNextEdgeId());
      newEdge_p->setPosition(2);
      theNewInitializationRHS.supplyAndAddEdgeInstance(*newEdge_p,aMod,aMult);
    }
    // 1st argument to division operation involves initializer and bound
    Intrinsic& aMinus(*(new Intrinsic("sub_scal_scal")));
    aMinus.setId(theNewInitializationRHS.getNextVertexId());
    theNewInitializationRHS.supplyAndAddVertexInstance(aMinus);
    // add an edge from the minus to the division
    newEdge_p=new ExpressionEdge(false);
    newEdge_p->setId(theNewInitializationRHS.getNextEdgeId());
    newEdge_p->setPosition(1);
    theNewInitializationRHS.supplyAndAddEdgeInstance(*newEdge_p,aMinus,aMod);
    // 2nd argument to division operation is the update which we have already
    // add an edge from the update to the division
    newEdge_p=new ExpressionEdge(false);
    newEdge_p->setId(theNewInitializationRHS.getNextEdgeId());
    newEdge_p->setPosition(2);
    theNewInitializationRHS.supplyAndAddEdgeInstance(*newEdge_p,theTopOftheUpdate,aMod);
    // one argument to the minus is the initializer which we have already, the 
    // other the boundary which we expect to be the second argument of the boolean 
    // in the old condition
    // copy this into the new init
    ExpressionVertex& 
      theTopOftheBound(theOldConditionExpr.copySubExpressionInto(theNewInitializationRHS,
								 theOldConditionExpr.findPositionalSubExpressionOf(theOldConditionMaxVertex,
														   2),
								 true,
								 false));
    newEdge_p=new ExpressionEdge(false);
    newEdge_p->setId(theNewInitializationRHS.getNextEdgeId());
    theNewInitializationRHS.supplyAndAddEdgeInstance(*newEdge_p,theTopOftheBound,aMinus);
    if (loopDir==ForLoopDirection::COUNT_UP)
      newEdge_p->setPosition(1);
    else 
      newEdge_p->setPosition(2);
    newEdge_p=new ExpressionEdge(false);
    newEdge_p->setId(theNewInitializationRHS.getNextEdgeId());
    theNewInitializationRHS.supplyAndAddEdgeInstance(*newEdge_p,theNewInitMaxVertex,aMinus);
    if (loopDir==ForLoopDirection::COUNT_UP)
      newEdge_p->setPosition(2);
    else 
      newEdge_p->setPosition(1);
    // find out if we need to adjust the initialization
    bool adjustUp=false;
    bool adjustDown=false;
    switch(theOldConditionBooleanOperation_p->getType()) { 
    case BooleanOperationType::LESS_THAN_OTYPE :
      if (loopDir==ForLoopDirection::COUNT_UP)  
	adjustDown=true;
      else
	THROW_LOGICEXCEPTION_MACRO("ReversibleControlFlowGraph::makeLoopExplicitReversalInitialization: loop reversal count down and < condition logic goof up");
      break;
    case BooleanOperationType::NOT_EQUAL_OTYPE :
      if (loopDir==ForLoopDirection::COUNT_UP)  
	adjustDown=true;
      else if (loopDir==ForLoopDirection::COUNT_UP) 
	adjustUp=true;
      break;
    case BooleanOperationType::GREATER_THAN_OTYPE :
      if (loopDir==ForLoopDirection::COUNT_UP) { 
	THROW_LOGICEXCEPTION_MACRO("ReversibleControlFlowGraph::makeLoopExplicitReversalInitialization: loop reversal count up and > condition logic goof up");
      }
      else
	adjustDown=true;
      break;
    case BooleanOperationType::LESS_OR_EQUAL_OTYPE: 
      if (!loopDir==ForLoopDirection::COUNT_UP)  
	THROW_LOGICEXCEPTION_MACRO("ReversibleControlFlowGraph::makeLoopExplicitReversalInitialization: loop reversal count down and <= condition logic goof up");
      break;
    case BooleanOperationType::GREATER_OR_EQUAL_OTYPE: 
      break;
    default:
      THROW_LOGICEXCEPTION_MACRO("ReversibleControlFlowGraph::makeLoopExplicitReversalInitialization: don't know what to do with operation "
				 << BooleanOperationType::toString(theOldConditionBooleanOperation_p->getType()));
      break;
    } 
    if (adjustDown || adjustUp) { 
      Constant& theOne(*(new Constant(SymbolType::INTEGER_STYPE,false)));
      theOne.setint(1);
      theOne.setId(theNewInitializationRHS.getNextVertexId());
      theNewInitializationRHS.supplyAndAddVertexInstance(theOne);
      Intrinsic* theAdjustmentIntrinsic_p;
      if (adjustDown) 
	theAdjustmentIntrinsic_p=new Intrinsic("sub_scal_scal");
      else 
	theAdjustmentIntrinsic_p=new Intrinsic("add_scal_scal");
      theAdjustmentIntrinsic_p->setId(theNewInitializationRHS.getNextVertexId());
      theNewInitializationRHS.supplyAndAddVertexInstance(*theAdjustmentIntrinsic_p);
      // add the 2 edges
      newEdge_p=new ExpressionEdge(false);
      newEdge_p->setId(theNewInitializationRHS.getNextEdgeId());
      newEdge_p->setPosition(1);
      theNewInitializationRHS.supplyAndAddEdgeInstance(*newEdge_p,*newInitTop_p,*theAdjustmentIntrinsic_p);
      newEdge_p=new ExpressionEdge(false);
      newEdge_p->setId(theNewInitializationRHS.getNextEdgeId());
      newEdge_p->setPosition(2);
      theNewInitializationRHS.supplyAndAddEdgeInstance(*newEdge_p,theOne,*theAdjustmentIntrinsic_p);
    }
  } 

  /**
   *strictly local wrapper to reduce code bloat
   */
  ExpressionVertex& undecidedDirectionCondition(const ForLoop& theOldForLoop,
						ForLoop& theNewForLoop,
						bool updGTZero) { 
    Expression& theNewConditionExpression(theNewForLoop.getCondition().getExpression());
    // copy 'i' for i . init
    ExpressionVertex& 
      theLoopVarRef(theOldForLoop.getCondition().getExpression().
			 copySubExpressionInto(theNewForLoop.getCondition().getExpression(),
					       theOldForLoop.getCondition().getExpression().
					       findPositionalSubExpressionOf(theOldForLoop.getCondition().
									     getExpression().getMaxVertex(),1),
					       true,
					       false));
    // copy  init for i. init
    ExpressionVertex& 
      theTopOftheCopiedInit(theOldForLoop.getInitialization().getAssignment().getRHS().
				 copySubExpressionInto(theNewForLoop.getCondition().getExpression(),
						       theOldForLoop.getInitialization().getAssignment().getRHS().getMaxVertex(),
						       true, 
						       false));
    BooleanOperation *compInit_p;
    if (updGTZero) 
      compInit_p=new BooleanOperation(BooleanOperationType::GREATER_OR_EQUAL_OTYPE,false);
    else 
      compInit_p=new BooleanOperation(BooleanOperationType::LESS_OR_EQUAL_OTYPE,false);
    compInit_p->setId(theNewForLoop.getCondition().getExpression().getNextVertexId());
    theNewForLoop.getCondition().getExpression().supplyAndAddVertexInstance(*compInit_p);
    // add the edges
    ExpressionEdge* newEdge_p=new ExpressionEdge(false);
    newEdge_p->setId(theNewConditionExpression.getNextEdgeId());
    newEdge_p->setPosition(1);
    theNewConditionExpression.supplyAndAddEdgeInstance(*newEdge_p,theLoopVarRef,*compInit_p);
    newEdge_p=new ExpressionEdge(false);
    newEdge_p->setId(theNewConditionExpression.getNextEdgeId());
    newEdge_p->setPosition(2);
    theNewConditionExpression.supplyAndAddEdgeInstance(*newEdge_p,theTopOftheCopiedInit,*compInit_p);
    // copy 'upd' for upd . 0
    // the update statement is typically of the form i=i+upd
    const ExpressionVertex* topOfOldUpd_p=0;
    const ExpressionVertex& oldUpdPlus=theOldForLoop.getUpdate().getAssignment().getRHS().getMaxVertex();
    // see which child is the same as the loop var:
    const ExpressionVertex& oldPos1Child=theOldForLoop.getUpdate().getAssignment().getRHS().findPositionalSubExpressionOf(oldUpdPlus,1);
    if (oldPos1Child.isArgument() && 
	(dynamic_cast<const Argument&>(oldPos1Child)).getVariable().getVariableSymbolReference().refersToSameSymbolAs((dynamic_cast<const Argument&>(theLoopVarRef)).getVariable().getVariableSymbolReference()))
      topOfOldUpd_p=&(theOldForLoop.getUpdate().getAssignment().getRHS().findPositionalSubExpressionOf(oldUpdPlus,2));
    else
      topOfOldUpd_p=&oldPos1Child;
    ExpressionVertex& 
      theTopOftheCopiedUpdate(theOldForLoop.getUpdate().getAssignment().getRHS().
				   copySubExpressionInto(theNewForLoop.getCondition().getExpression(),
							 *topOfOldUpd_p,
							 true, 
							 false));
    Constant* theZero_p=new Constant(SymbolType::REAL_STYPE,false);
    theZero_p->setdouble(0.0);
    theZero_p->setId(theNewForLoop.getCondition().getExpression().getNextVertexId());
    theNewForLoop.getCondition().getExpression().supplyAndAddVertexInstance(*theZero_p);
    BooleanOperation *updComp_p;
    if (updGTZero)
      updComp_p=new BooleanOperation(BooleanOperationType::GREATER_THAN_OTYPE,false);
    else
      updComp_p=new BooleanOperation(BooleanOperationType::LESS_THAN_OTYPE,false);
    updComp_p->setId(theNewForLoop.getCondition().getExpression().getNextVertexId());
    theNewForLoop.getCondition().getExpression().supplyAndAddVertexInstance(*updComp_p);
    // add the edges
    newEdge_p=new ExpressionEdge(false);
    newEdge_p->setId(theNewConditionExpression.getNextEdgeId());
    newEdge_p->setPosition(1);
    theNewConditionExpression.supplyAndAddEdgeInstance(*newEdge_p,theTopOftheCopiedUpdate,*updComp_p);
    newEdge_p=new ExpressionEdge(false);
    newEdge_p->setId(theNewConditionExpression.getNextEdgeId());
    newEdge_p->setPosition(2);
    theNewConditionExpression.supplyAndAddEdgeInstance(*newEdge_p,*theZero_p,*updComp_p);
    BooleanOperation *and_p;
    and_p=new BooleanOperation(BooleanOperationType::AND_OTYPE,false);
    and_p->setId(theNewForLoop.getCondition().getExpression().getNextVertexId());
    theNewForLoop.getCondition().getExpression().supplyAndAddVertexInstance(*and_p);
    // add the edges
    newEdge_p=new ExpressionEdge(false);
    newEdge_p->setId(theNewConditionExpression.getNextEdgeId());
    newEdge_p->setPosition(1);
    theNewConditionExpression.supplyAndAddEdgeInstance(*newEdge_p,*updComp_p,*and_p);
    newEdge_p=new ExpressionEdge(false);
    newEdge_p->setId(theNewConditionExpression.getNextEdgeId());
    newEdge_p->setPosition(2);
    theNewConditionExpression.supplyAndAddEdgeInstance(*newEdge_p,*compInit_p,*and_p);
    return *and_p;
  }

  void ReversibleControlFlowGraph::makeLoopExplicitReversalCondition(const ForLoop& theOldForLoop,
								     ForLoop& theNewForLoop,
								     ForLoopDirection::ForLoopDirection_E loopDir) { 
    Expression& theNewConditionExpression(theNewForLoop.getCondition().getExpression());
    if (loopDir!=ForLoopDirection::COUNT_UNDECIDED) { 
      // the stop condition is determined by the first argument to the boolean of the old condition, 
      // the old initialization and the up or down count 
      // find the first argument to the boolean
      // copy this into the new condition
      ExpressionVertex& 
	theTopOfConditionVarRef(theOldForLoop.getCondition().getExpression().
				copySubExpressionInto(theNewForLoop.getCondition().getExpression(),
						      theOldForLoop.getCondition().getExpression().
						      findPositionalSubExpressionOf(theOldForLoop.getCondition().
										    getExpression().getMaxVertex(),1),
						      true,
						      false));
      // copy the old initialization:
      ExpressionVertex& theTopOftheInit(theOldForLoop.getInitialization().getAssignment().getRHS().
					copySubExpressionInto(theNewForLoop.getCondition().getExpression(),
							      theOldForLoop.getInitialization().getAssignment().getRHS().getMaxVertex(),
							      true, 
							      false));
      BooleanOperation* theNewBooleanOp_p;
      if(loopDir==ForLoopDirection::COUNT_UP)
	theNewBooleanOp_p=new BooleanOperation(BooleanOperationType::GREATER_OR_EQUAL_OTYPE,false);
      else if (loopDir==ForLoopDirection::COUNT_DOWN) 
	theNewBooleanOp_p=new BooleanOperation(BooleanOperationType::LESS_OR_EQUAL_OTYPE,false);
      theNewBooleanOp_p->setId(theNewForLoop.getCondition().getExpression().getNextVertexId());
      theNewForLoop.getCondition().getExpression().supplyAndAddVertexInstance(*theNewBooleanOp_p);
      // add the 2 edges
      ExpressionEdge* newEdge_p=new ExpressionEdge(false);
      newEdge_p->setId(theNewConditionExpression.getNextEdgeId());
      newEdge_p->setPosition(1);
      theNewConditionExpression.supplyAndAddEdgeInstance(*newEdge_p,theTopOfConditionVarRef,*theNewBooleanOp_p);
      newEdge_p=new ExpressionEdge(false);
      newEdge_p->setId(theNewConditionExpression.getNextEdgeId());
      newEdge_p->setPosition(2);
      theNewConditionExpression.supplyAndAddEdgeInstance(*newEdge_p,theTopOftheInit,*theNewBooleanOp_p);
    }
    else { 
      // do i=init,cond,upd -> if s is variable we have to have extra tests:  (upd>0 && i>=init) || (upd<0 && i<=init)
      ExpressionVertex& theGTPart=undecidedDirectionCondition(theOldForLoop,
							      theNewForLoop,
							      true);
      ExpressionVertex& theLTPart=undecidedDirectionCondition(theOldForLoop,
							      theNewForLoop,
							      false);
      BooleanOperation *topLevelOr_p=new BooleanOperation(BooleanOperationType::OR_OTYPE,false);
      topLevelOr_p->setId(theNewForLoop.getCondition().getExpression().getNextVertexId());
      theNewForLoop.getCondition().getExpression().supplyAndAddVertexInstance(*topLevelOr_p);
      // add the 2 edges
      ExpressionEdge* newEdge_p=new ExpressionEdge(false);
      newEdge_p->setId(theNewConditionExpression.getNextEdgeId());
      newEdge_p->setPosition(1);
      theNewConditionExpression.supplyAndAddEdgeInstance(*newEdge_p,theGTPart,*topLevelOr_p);
      newEdge_p=new ExpressionEdge(false);
      newEdge_p->setId(theNewConditionExpression.getNextEdgeId());
      newEdge_p->setPosition(2);
      theNewConditionExpression.supplyAndAddEdgeInstance(*newEdge_p,theLTPart,*topLevelOr_p);
    }
  }

  void ReversibleControlFlowGraph::makeLoopExplicitReversalUpdate(const ForLoop& theOldForLoop,
								  ForLoop& theNewForLoop) { 
    // LHS
    theOldForLoop.getUpdate().getAssignment().getLHS().copyMyselfInto(theNewForLoop.getUpdate().getAssignment().getLHS());
    // get the new RHS
    Expression& theNewUpdateRHS(theNewForLoop.getUpdate().getAssignment().getRHS());
    const Expression& theOldUpdateRHS(theOldForLoop.getUpdate().getAssignment().getRHS());
    // copy the old update left operand which we expect to be the varref into the new update
    ExpressionVertex& 
      theTopOftheNewUpdateVarRef(theOldUpdateRHS.copySubExpressionInto(theNewUpdateRHS,
								       theOldUpdateRHS.findPositionalSubExpressionOf(theOldUpdateRHS.getMaxVertex(),1),
								       true,
								       false));
    // copy the right operand which we expect to be the update constant subexpression
    // into the new update
    ExpressionVertex& 
      theTopOftheNewUpdateConstant(theOldUpdateRHS.copySubExpressionInto(theNewUpdateRHS,
									 theOldUpdateRHS.findPositionalSubExpressionOf(theOldUpdateRHS.getMaxVertex(),2),
									 true,
									 false));
    
    const Intrinsic* theOldUpdateTopIntrinsic_p(dynamic_cast<const Intrinsic*>(&(theOldUpdateRHS.getMaxVertex())));
    if (!theOldUpdateTopIntrinsic_p)
      THROW_LOGICEXCEPTION_MACRO("ReversibleControlFlowGraph::makeLoopExplicitReversalUpdate: cannot find proper top level intrinsic operation in update expression");
    Intrinsic* theNewUpdateOp_p;
    if (&(theOldUpdateTopIntrinsic_p->getInlinableIntrinsicsCatalogueItem())==
	&(ConceptuallyStaticInstances::instance()->getInlinableIntrinsicsCatalogue().getElement("add_scal_scal"))) 
                                                                theNewUpdateOp_p=new Intrinsic("sub_scal_scal",false);
    else if (&(theOldUpdateTopIntrinsic_p->getInlinableIntrinsicsCatalogueItem())==
	     &(ConceptuallyStaticInstances::instance()->getInlinableIntrinsicsCatalogue().getElement("sub_scal_scal")))
                                                              theNewUpdateOp_p=new Intrinsic("add_scal_scal",false);
    else
      THROW_LOGICEXCEPTION_MACRO("ReversibleControlFlowGraph::makeLoopExplicitReversalUpdate: don't know what to do with operation "
                                 << theOldUpdateTopIntrinsic_p->debug().c_str());
    theNewUpdateOp_p->setId(theNewForLoop.getCondition().getExpression().getNextVertexId());
    theNewUpdateRHS.supplyAndAddVertexInstance(*theNewUpdateOp_p);
    ExpressionEdge* newEdge_p=new ExpressionEdge(false);
    newEdge_p->setId(theNewUpdateRHS.getNextEdgeId());
    newEdge_p->setPosition(1);
    theNewUpdateRHS.supplyAndAddEdgeInstance(*newEdge_p,theTopOftheNewUpdateVarRef,*theNewUpdateOp_p);
    newEdge_p=new ExpressionEdge(false);
    newEdge_p->setId(theNewUpdateRHS.getNextEdgeId());
    newEdge_p->setPosition(2);
    theNewUpdateRHS.supplyAndAddEdgeInstance(*newEdge_p,theTopOftheNewUpdateConstant,*theNewUpdateOp_p);
  }

  ReversibleControlFlowGraph::VertexPPairList& ReversibleControlFlowGraph::getOriginalReverseVertexPPairList() { 
    return myOriginalReverseVertexPPairList; 
  } 

  const ReversibleControlFlowGraph::VertexPPairList& ReversibleControlFlowGraph::getOriginalReverseVertexPPairList() const { 
    return myOriginalReverseVertexPPairList; 
  } 

  void ReversibleControlFlowGraph::donotRetainUserReversalFlag() { 
    myRetainUserReversalFlag=false;
  } 

  void ReversibleControlFlowGraph::setUnstructured() { 
    myStructuredFlag=false;
  }

  bool ReversibleControlFlowGraph::isStructured() const { 
    return myStructuredFlag;
  } 

} // end of namespace

