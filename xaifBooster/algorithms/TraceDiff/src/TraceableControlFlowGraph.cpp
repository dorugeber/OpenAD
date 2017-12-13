// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/VariableSymbolReference.hpp"

/*
#include <sstream>
#include "xaifBooster/utils/inc/PrintManager.hpp"
#include "xaifBooster/utils/inc/DbgLoggerManager.hpp"

#include "xaifBooster/system/inc/ConceptuallyStaticInstances.hpp"
#include "xaifBooster/system/inc/CallGraph.hpp"

#include "xaifBooster/system/inc/GraphVizDisplay.hpp"
#include "xaifBooster/system/inc/Argument.hpp"
#include "xaifBooster/system/inc/Intrinsic.hpp"
#include "xaifBooster/system/inc/Constant.hpp"
#include "xaifBooster/system/inc/SubroutineCall.hpp"
#include "xaifBooster/system/inc/BooleanOperation.hpp"

#include "xaifBooster/algorithms/ControlFlowReversal/inc/ControlFlowGraphVertexAlg.hpp"
#include "xaifBooster/algorithms/ControlFlowReversal/inc/CallGraphAlg.hpp"
#include "xaifBooster/algorithms/ControlFlowReversal/inc/BasicBlockAlg.hpp"
*/

#include "xaifBooster/algorithms/InlinableXMLRepresentation/inc/InlinableSubroutineCall.hpp"

#include "xaifBooster/algorithms/TraceDiff/inc/TraceableControlFlowGraph.hpp"
#include "xaifBooster/algorithms/TraceDiff/inc/Helpers.hpp"

using namespace xaifBooster;

namespace xaifBoosterTraceDiff { 

  TraceableControlFlowGraph::TraceableControlFlowGraph(const ControlFlowGraph& theOriginal_r) : 
    xaifBoosterControlFlowReversal::ReversibleControlFlowGraph(theOriginal_r) {
  }

  std::string
  TraceableControlFlowGraph::debug() const {
    std::ostringstream out;
    out << "xaifBoosterTraceDiff::TraceableControlFlowGraph["
	<< this
	<< "]" << std::ends;
    return out.str();
  }

  void TraceableControlFlowGraph::traceActiveControlFlow() {
    std::stack<const Symbol*> theLoopCounterSymbolStack_r;
    std::list<xaifBoosterControlFlowReversal::ReversibleControlFlowGraphVertex*>::iterator the_mySortedVertices_p_l_it;
    for (the_mySortedVertices_p_l_it=mySortedVertices_p_l.begin(); 
	 the_mySortedVertices_p_l_it!=mySortedVertices_p_l.end(); 
	 ++the_mySortedVertices_p_l_it) {
      xaifBoosterControlFlowReversal::ControlFlowGraphVertexAlg::ControlFlowGraphVertexKind_E theKind=
	(*the_mySortedVertices_p_l_it)->getOriginalControlFlowGraphVertexAlg().getKind();
      switch (theKind) {
	/*
	  Both FORLOOP and PRELOOP are control flow merge points,
	  that is they have two predecessors. One of the indeges is a
	  back edge with source of kind ENDLOOP.
	*/
      case xaifBoosterControlFlowReversal::ControlFlowGraphVertexAlg::FORLOOP : 
      case xaifBoosterControlFlowReversal::ControlFlowGraphVertexAlg::PRELOOP : {
	// first check if anything here is active
	bool needToTrace=false;
	Expression::CArgumentPList initList, updateList, conditionList;
	int lineNumber=0;
	if (theKind==xaifBoosterControlFlowReversal::ControlFlowGraphVertexAlg::FORLOOP) {
	  const ForLoop& theForLoop(dynamic_cast<const ForLoop&>((*the_mySortedVertices_p_l_it)->getOriginalVertex()));
	  theForLoop.getInitialization().getAssignment().getRHS().appendActiveArguments(initList);
	  theForLoop.getUpdate().getAssignment().getRHS().appendActiveArguments(updateList);
	  theForLoop.getCondition().getExpression().appendActiveArguments(conditionList);
	  lineNumber=theForLoop.getLineNumber();
	  needToTrace=(!initList.empty() || !updateList.empty() || !conditionList.empty());
	}
	if (theKind==xaifBoosterControlFlowReversal::ControlFlowGraphVertexAlg::PRELOOP) {
	  const PreLoop& thePreLoop(dynamic_cast<const PreLoop&>((*the_mySortedVertices_p_l_it)->getOriginalVertex()));
	  thePreLoop.getCondition().getExpression().appendActiveArguments(conditionList);
	  needToTrace=!conditionList.empty();
	}
	const Symbol* theLoopCounterSymbol_p=0;
	if (needToTrace) { 
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
	  for (;beginItie!=endItie ;++beginItie) ieil.push_back(beginItie);
	  std::list<InEdgeIterator>::iterator ieilIt;
	  for (ieilIt=ieil.begin();ieilIt!=ieil.end();++ieilIt) {
	    if (&((*the_mySortedVertices_p_l_it)->getCounterPart())!=&(getSourceOf(*(*ieilIt)))) {
	      // this is not the inedge from the corresponding ENDLOOP
	      BasicBlock& theBasicBlock_r(dynamic_cast<BasicBlock&>(insertBasicBlock(getSourceOf(*(*ieilIt)),
										     getTargetOf(*(*ieilIt)),
										     (*(*ieilIt)),
										     false).getNewVertex()));
	      removeAndDeleteEdge(*(*ieilIt));
	      theBasicBlock_r.setId(std::string("_trc_")+makeUniqueVertexId());
	      theLoopCounterSymbol_p=insert_init_integer(0,theBasicBlock_r);
	      // trace the loop
	      xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall* theSubroutineCall_p;
	      theSubroutineCall_p=new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall("oad_trace_loop");
	      theSubroutineCall_p->setId("traceActiveControlFlow");
	      theBasicBlock_r.supplyAndAddBasicBlockElementInstance(*theSubroutineCall_p); 
	      ConcreteArgument& theOriginalLine(theSubroutineCall_p->addConcreteArgument(1));
	      theOriginalLine.makeConstant(SymbolType::INTEGER_STYPE);
	      theOriginalLine.getConstant().setint(lineNumber);
	      if (Helpers::argumentsHaveArrayAccess(initList)) { 
		theSubroutineCall_p=new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall("oad_trace_initialization");
		theSubroutineCall_p->setId("traceActiveControlFlow");
		theBasicBlock_r.supplyAndAddBasicBlockElementInstance(*theSubroutineCall_p); 
		Helpers::traceArguments(initList,
					theBasicBlock_r);
		theSubroutineCall_p=new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall("oad_trace_einitialization");
		theSubroutineCall_p->setId("traceActiveControlFlow");
		theBasicBlock_r.supplyAndAddBasicBlockElementInstance(*theSubroutineCall_p); 
	      }
	      if (Helpers::argumentsHaveArrayAccess(conditionList)) { 
		theSubroutineCall_p=new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall("oad_trace_condition");
		theSubroutineCall_p->setId("traceActiveControlFlow");
		theBasicBlock_r.supplyAndAddBasicBlockElementInstance(*theSubroutineCall_p); 
		Helpers::traceArguments(conditionList,
					theBasicBlock_r);
		theSubroutineCall_p=new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall("oad_trace_econdition");
		theSubroutineCall_p->setId("traceActiveControlFlow");
		theBasicBlock_r.supplyAndAddBasicBlockElementInstance(*theSubroutineCall_p); 
	      }
	      if (Helpers::argumentsHaveArrayAccess(updateList)) { 
		theSubroutineCall_p=new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall("oad_trace_update");
		theSubroutineCall_p->setId("traceActiveControlFlow");
		theBasicBlock_r.supplyAndAddBasicBlockElementInstance(*theSubroutineCall_p); 
		Helpers::traceArguments(updateList,
					theBasicBlock_r);
		theSubroutineCall_p=new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall("oad_trace_eupdate");
		theSubroutineCall_p->setId("traceActiveControlFlow");
		theBasicBlock_r.supplyAndAddBasicBlockElementInstance(*theSubroutineCall_p); 
	      }
	    }  
	    else {
	      // store index of corresponding ENDLOOP node
	      endLoopIndex=getSourceOf(*(*ieilIt)).getIndex();
	    }
	  }
	  // trace the counter after end of the loop
	  // to find the correct outedge we check if the index is 
	  // greater than the one of the corresponding ENDLOOP node for
	  // all targets. 
	  OutEdgeIteratorPair poe(getOutEdgesOf(*(*the_mySortedVertices_p_l_it)));
	  OutEdgeIterator beginItoe(poe.first),endItoe(poe.second);
	  std::list<OutEdgeIterator> oeil;
	  for (;beginItoe!=endItoe ;++beginItoe) oeil.push_back(beginItoe);
	  std::list<OutEdgeIterator>::iterator oeilIt;
	  for (oeilIt=oeil.begin();oeilIt!=oeil.end();++oeilIt) { 
	    if (getTargetOf(*(*oeilIt)).getIndex()>endLoopIndex) {
	      xaifBoosterControlFlowReversal::ReversibleControlFlowGraphVertex& thePushBlock=insertBasicBlock(getSourceOf(*(*oeilIt)),
													      getTargetOf(*(*oeilIt)),
													      (*(*oeilIt)),
													      true);
	      BasicBlock& theBasicBlock_r(dynamic_cast<BasicBlock&>(thePushBlock.getNewVertex()));
	      removeAndDeleteEdge(*(*oeilIt));
	      theBasicBlock_r.setId(std::string("_trc_")+makeUniqueVertexId());
	      xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall* theSubroutineCall_p;
	      theSubroutineCall_p=new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall("oad_trace_CFval");
	      theSubroutineCall_p->setId("traceActiveControlFlow");
	      VariableSymbolReference* aVSR_p=new VariableSymbolReference(*theLoopCounterSymbol_p,theBasicBlock_r.getScope());
	      aVSR_p->setId("1");
	      aVSR_p->setAnnotation("traceActiveControlFLow");
	      Variable aVariable;
	      aVariable.supplyAndAddVertexInstance(*aVSR_p);
	      aVariable.getAliasMapKey().setTemporary();
	      aVariable.getDuUdMapKey().setTemporary();
	      aVariable.copyMyselfInto(theSubroutineCall_p->addConcreteArgument(1).getArgument().getVariable());
	      theBasicBlock_r.supplyAndAddBasicBlockElementInstance(*theSubroutineCall_p); 
	      theSubroutineCall_p=new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall("oad_trace_eloop");
	      theSubroutineCall_p->setId("otraceActiveControlFlow");
	      theBasicBlock_r.supplyAndAddBasicBlockElementInstance(*theSubroutineCall_p); 
	      break;
	    }
	  }
	}
	theLoopCounterSymbolStack_r.push(theLoopCounterSymbol_p);
	break;
      } 
      case xaifBoosterControlFlowReversal::ControlFlowGraphVertexAlg::ENDLOOP : {
	// pop counter symbol from theLoopCounterSymbolStack_r
	if (theLoopCounterSymbolStack_r.top()) { 
	  // insert "counter=counter+1" before *(*the_mySortedVertices_p_l_it)
	  InEdgeIteratorPair pie(getInEdgesOf(*(*the_mySortedVertices_p_l_it)));
	  InEdgeIterator beginItie(pie.first);
	  BasicBlock& theBasicBlock_r(dynamic_cast<BasicBlock&>(insertBasicBlock(getSourceOf((*beginItie)),
										 getTargetOf((*beginItie)),
										 (*beginItie),
										 false).getNewVertex()));
	  removeAndDeleteEdge((*beginItie));
	  theBasicBlock_r.setId(std::string("_trc_")+makeUniqueVertexId());
	  insert_increment_integer(theLoopCounterSymbolStack_r.top(),theBasicBlock_r);
	  theLoopCounterSymbolStack_r.pop();
	}
	break;
      }
      case xaifBoosterControlFlowReversal::ControlFlowGraphVertexAlg::ENDBRANCH : {
	// get the corresponding branch node
	const Branch& theBranch(dynamic_cast<const Branch&>((*the_mySortedVertices_p_l_it)->getCounterPart().getOriginalVertex())); 
	Expression::CArgumentPList conditionList;
	theBranch.getCondition().getExpression().appendActiveArguments(conditionList);
	if (!conditionList.empty()) { 
	  // first trace the branch: 
	  // the in-edge is: 
	  InEdgeIteratorPair branchInEdgeIPair(getInEdgesOf((*the_mySortedVertices_p_l_it)->getCounterPart()));
	  xaifBoosterControlFlowReversal::ReversibleControlFlowGraphEdge& theInEdge(*(branchInEdgeIPair.first));
	  BasicBlock& theBasicBlock_r(dynamic_cast<BasicBlock&>(insertBasicBlock(getSourceOf(theInEdge),
										 (*the_mySortedVertices_p_l_it)->getCounterPart(),
										 theInEdge,
										 true).getNewVertex()));
	  theBasicBlock_r.setId(std::string("_trc_")+makeUniqueVertexId());
	  removeAndDeleteEdge(theInEdge);
	  xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall* theSubroutineCall_p;
	  theSubroutineCall_p=new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall("oad_trace_branch");
	  theSubroutineCall_p->setId("traceActiveControlFlow");
	  theBasicBlock_r.supplyAndAddBasicBlockElementInstance(*theSubroutineCall_p); 
	  ConcreteArgument& theOriginalLine(theSubroutineCall_p->addConcreteArgument(1));
	  theOriginalLine.makeConstant(SymbolType::INTEGER_STYPE);
	  theOriginalLine.getConstant().setint(theBranch.getLineNumber());
	  if (Helpers::argumentsHaveArrayAccess(conditionList)) { 
	    theSubroutineCall_p=new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall("oad_trace_condition");
	    theSubroutineCall_p->setId("traceActiveControlFlow");
	    theBasicBlock_r.supplyAndAddBasicBlockElementInstance(*theSubroutineCall_p); 
	    Helpers::traceArguments(conditionList,
				    theBasicBlock_r);
	    theSubroutineCall_p=new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall("oad_trace_econdition");
	    theSubroutineCall_p->setId("traceActiveControlFlow");
	    theBasicBlock_r.supplyAndAddBasicBlockElementInstance(*theSubroutineCall_p); 	  
	  }
	  // now trace the respective end:
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
	    xaifBoosterControlFlowReversal::ReversibleControlFlowGraphVertex& theSource_r(getSourceOf(*(*ieilIt)));
	    xaifBoosterControlFlowReversal::ReversibleControlFlowGraphVertex& thePushBlock(insertBasicBlock(theSource_r,
													    getTargetOf(*(*ieilIt)),
													    (*(*ieilIt)),
													    true));
	    // postpone deleting the edge, we still need it
	    BasicBlock& theBasicBlock_r(dynamic_cast<BasicBlock&>(thePushBlock.getNewVertex()));
	    theBasicBlock_r.setId(std::string("_trc_")+makeUniqueVertexId());
	    theSubroutineCall_p=new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall("oad_trace_CFval");
	    theSubroutineCall_p->setId("traceActiveControlFlow");
	    ConcreteArgument& theCFVal(theSubroutineCall_p->addConcreteArgument(1));
	    theCFVal.makeConstant(SymbolType::INTEGER_STYPE);
	    theBasicBlock_r.supplyAndAddBasicBlockElementInstance(*theSubroutineCall_p); 
	    if (ieil.size()==2) {
	      // this is a true if-then-else
	      if ((*(*ieilIt)).hasRevConditionValue())
		theCFVal.getConstant().setint(1);
	      else 
		theCFVal.getConstant().setint(0);
	    }
	    else { 
	      // this is a switch, all of them should have a value assigned now
	      if ((*(*ieilIt)).hasRevConditionValue())
		theCFVal.getConstant().setint((*(*ieilIt)).getRevConditionValue());
	      else
		THROW_LOGICEXCEPTION_MACRO("TraceableControlFlowGraph::storeControlFlow: switch branch exit without condition value");
	    }
	    theSubroutineCall_p=new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall("oad_trace_ebranch");
	    theSubroutineCall_p->setId("traceActiveControlFlow");
	    theBasicBlock_r.supplyAndAddBasicBlockElementInstance(*theSubroutineCall_p); 
	    // here comes the postponed delete of the edge
	    removeAndDeleteEdge(*(*ieilIt));
	  }
	}
	break;
      }
      default : 
	break;
      }
      DBG_MACRO(DbgGroup::DATA,"TraceableControlFlowGraph::storeControlFlow: ran for vertex "
		<< (*the_mySortedVertices_p_l_it)->debug().c_str());
    }
  } 

} 

