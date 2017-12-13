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
#include "xaifBooster/system/inc/Intrinsic.hpp"
#include "xaifBooster/system/inc/VariableSymbolReference.hpp"
#include "xaifBooster/system/inc/Constant.hpp"
#include "xaifBooster/system/inc/SubroutineCall.hpp"
#include "xaifBooster/system/inc/BooleanOperation.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/BasicControlFlowGraph.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/BasicControlFlowGraphEdge.hpp" 
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/BasicControlFlowGraphVertex.hpp" 
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/BasicBlockAlg.hpp"

#include "xaifBooster/algorithms/InlinableXMLRepresentation/inc/InlinableSubroutineCall.hpp"

using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulation { 

  std::string BasicControlFlowGraph::myAlgorithmSignature(std::string("_bbp_"));

  BasicControlFlowGraph::BasicControlFlowGraph(const ControlFlowGraph& theOriginal_r) : 
    myOriginalGraph_r(theOriginal_r) {
  }

  // this does not guarantee uniqueness in the case of an existing
  // graph getting expanded
  // depends on prefix
  std::string BasicControlFlowGraph::makeUniqueVertexId() {
    std::ostringstream oss;
    oss << getAlgorithmSignature().c_str() << "v_" << getNextVertexId() << std::ends;
    return (oss.str());
  }

  std::string BasicControlFlowGraph::makeUniqueEdgeId() {
    std::ostringstream oss;
    oss << getAlgorithmSignature().c_str() << "e_" << getNextEdgeId() << std::ends;
    return (oss.str());
  }

  const std::string&
  BasicControlFlowGraph::getAlgorithmSignature() const {
    return myAlgorithmSignature;
  }

  void
  BasicControlFlowGraph::makeThisACopyOfOriginalControlFlowGraph() {
    class GetBasicFromOriginal {
    public : 
      BasicControlFlowGraphVertex& operator() (std::list<std::pair<const ControlFlowGraphVertex*,BasicControlFlowGraphVertex*> > vertexCopy_l,
						    const ControlFlowGraphVertex& theOriginalVertex) { 
	std::list<std::pair<const ControlFlowGraphVertex*,BasicControlFlowGraphVertex*> >::iterator vertexCopyListIt;
	for (vertexCopyListIt=vertexCopy_l.begin();
	     vertexCopyListIt!=vertexCopy_l.end();
	     ++vertexCopyListIt) {
	  if (&theOriginalVertex==(*vertexCopyListIt).first)
	    return *((*vertexCopyListIt).second);
	}
	THROW_LOGICEXCEPTION_MACRO("getBasicFromOriginal: not found"); 
      };
    } getBasicFromOriginal;
    std::list<std::pair<const ControlFlowGraphVertex*,BasicControlFlowGraphVertex*> > vertexCopy_l;
    ControlFlowGraph::ConstVertexIteratorPair p(myOriginalGraph_r.vertices());
    ControlFlowGraph::ConstVertexIterator vertexIt(p.first),endIt(p.second);
    for (;vertexIt!=endIt ;++vertexIt) {
      BasicControlFlowGraphVertex* anOriginalVertex=new BasicControlFlowGraphVertex(&(*vertexIt));
      vertexCopy_l.push_back(std::make_pair(&(*vertexIt),anOriginalVertex));  
      supplyAndAddVertexInstance(*anOriginalVertex);
      // copy simple augmented info: 
      anOriginalVertex->setIndex((*vertexIt).getIndex());
    } 
    // copy the augmented vertex references
    std::list<std::pair<const ControlFlowGraphVertex*,BasicControlFlowGraphVertex*> >::iterator vertexCopyListIt;
    for (vertexCopyListIt=vertexCopy_l.begin();
	 vertexCopyListIt!=vertexCopy_l.end();
	 ++vertexCopyListIt) {

      if ((*vertexCopyListIt).first->hasTopExplicitLoop())
	(*vertexCopyListIt).second->setTopExplicitLoop(getBasicFromOriginal(vertexCopy_l,
									    (*vertexCopyListIt).first->getTopExplicitLoop()));
      if ((*vertexCopyListIt).first->hasEnclosingControlFlow())
	(*vertexCopyListIt).second->setEnclosingControlFlow(getBasicFromOriginal(vertexCopy_l,
										 (*vertexCopyListIt).first->getEnclosingControlFlow()));
    }
    // the sorted vertex list
    const std::list<const ControlFlowGraphVertex*>& theList(myOriginalGraph_r.getSOrtedVertexList());
    for (std::list<const ControlFlowGraphVertex*>::const_iterator sortListIter=theList.begin();
	 sortListIter!=theList.end();
	 ++sortListIter) { 
      mySortedVertices_p_l.push_back(&(getBasicFromOriginal(vertexCopy_l,
							    **sortListIter)));
    }
    ControlFlowGraph::ConstEdgeIteratorPair pe(myOriginalGraph_r.edges());
    ControlFlowGraph::ConstEdgeIterator beginIte(pe.first),endIte(pe.second);
    for (;beginIte!=endIte ;++beginIte) {
      const ControlFlowGraphVertex& theOriginalSource_r(myOriginalGraph_r.getSourceOf(*beginIte));
      const ControlFlowGraphVertex& theOriginalTarget_r(myOriginalGraph_r.getTargetOf(*beginIte));
      std::list<std::pair<const ControlFlowGraphVertex*,BasicControlFlowGraphVertex*> >::iterator vertexCopyListSrcIt, vertexCopyListTgtIt;
      for (vertexCopyListSrcIt=vertexCopy_l.begin();
           vertexCopyListSrcIt!=vertexCopy_l.end();vertexCopyListSrcIt++) {
        if ((*vertexCopyListSrcIt).first==&theOriginalSource_r) {
          for (vertexCopyListTgtIt=vertexCopy_l.begin();
               vertexCopyListTgtIt!=vertexCopy_l.end();vertexCopyListTgtIt++) {
            if ((*vertexCopyListTgtIt).first==&theOriginalTarget_r) {
              BasicControlFlowGraphEdge* theEdgeCopy_p=new BasicControlFlowGraphEdge(&(*beginIte));
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

  BasicControlFlowGraphVertex& BasicControlFlowGraph::getExit() {
    BasicControlFlowGraph::VertexIteratorPair p(vertices());
    BasicControlFlowGraph::VertexIterator beginIt(p.first),endIt(p.second);
    for (;beginIt!=endIt ;++beginIt) 
      if ((*beginIt).getKind() == ControlFlowGraphVertexKind::EXIT_VKIND) 
	return *beginIt;
    THROW_LOGICEXCEPTION_MACRO("Missing EXIT node in control flow graph"); 
  }
  
  BasicControlFlowGraphVertex& BasicControlFlowGraph::getEntry() {
    BasicControlFlowGraph::VertexIteratorPair p(vertices());
    BasicControlFlowGraph::VertexIterator beginIt(p.first),endIt(p.second);
    for (;beginIt!=endIt ;++beginIt) 
      if ((*beginIt).getKind() == ControlFlowGraphVertexKind::ENTRY_VKIND) 
	return *beginIt;
    THROW_LOGICEXCEPTION_MACRO("Missing ENTRY node in control flow graph"); 
  }

  BasicBlock*
  BasicControlFlowGraph::insertBasicBlock() {

    try {
      BasicControlFlowGraphVertex& entryVertex = BasicControlFlowGraph::getEntry();
      BasicControlFlowGraphEdge& replacedEdge_r(*(getOutEdgesOf(entryVertex).first));
      BasicControlFlowGraphVertex& afterVertex = getTargetOf(replacedEdge_r);

      BasicControlFlowGraphEdge* aNewControlFlowGraphInEdge_p=new BasicControlFlowGraphEdge();
      aNewControlFlowGraphInEdge_p->setId(makeUniqueEdgeId());
      BasicControlFlowGraphEdge* aNewControlFlowGraphOutEdge_p=new BasicControlFlowGraphEdge();
      aNewControlFlowGraphOutEdge_p->setId(makeUniqueEdgeId());

      if (numOutEdgesOf(entryVertex) >= 1) {
	if (replacedEdge_r.hasConditionValue()) {
	  aNewControlFlowGraphOutEdge_p->setConditionValue(replacedEdge_r.getConditionValue());
	}
      }

      BasicBlock* theNewBasicBlock=new BasicBlock(ConceptuallyStaticInstances::instance()->getCallGraph().getScopeTree().getGlobalScope());
      BasicControlFlowGraphVertex* newVertex_p = new BasicControlFlowGraphVertex(theNewBasicBlock);
      supplyAndAddVertexInstance(*newVertex_p);
      newVertex_p->setIndex(numVertices()+1);
      newVertex_p->supplyAndAddNewVertex(*theNewBasicBlock);
      newVertex_p->getNewVertex().setId(makeUniqueVertexId());
      newVertex_p->getNewVertex().setAnnotation(getAlgorithmSignature());

      removeAndDeleteEdge(replacedEdge_r);
      supplyAndAddEdgeInstance(*aNewControlFlowGraphInEdge_p,entryVertex,*newVertex_p);
      supplyAndAddEdgeInstance(*aNewControlFlowGraphOutEdge_p,*newVertex_p,afterVertex);
      
      return theNewBasicBlock;
    } catch (LogicException){
      return NULL;
    }
  }

  void BasicControlFlowGraph::printXMLHierarchy(std::ostream& os) const {
    BasicControlFlowGraph::ConstVertexIteratorPair p(vertices());
    BasicControlFlowGraph::ConstVertexIterator beginIt(p.first),endIt(p.second);
    for (;beginIt!=endIt ;++beginIt) {
      (*beginIt).printXMLHierarchy(os);
    }
    BasicControlFlowGraph::ConstEdgeIteratorPair pe=edges();
    BasicControlFlowGraph::ConstEdgeIterator beginIte(pe.first),endIte(pe.second);
    for (;beginIte!=endIte ;++beginIte)
      (*beginIte).printXMLHierarchy(os,*this);
  }

  std::string
  BasicControlFlowGraph::debug() const {
    std::ostringstream out;
    out << "xaifBoosterBasicBlockPreaccumulation::BasicControlFlowGraph["
	<< this
	<< "]" << std::ends;
    return out.str();
  }

  void BasicControlFlowGraph::traverseToChildren(const GenericAction::GenericAction_E anAction_c) {
  }

} // end of namespace

