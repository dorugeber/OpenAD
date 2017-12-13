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
#include <string>
#include <iostream>
#include <fstream>
#include <list>
#include "xaifBooster/utils/inc/PrintManager.hpp"
#include "xaifBooster/utils/inc/DbgLoggerManager.hpp"
#include "xaifBooster/system/inc/GraphVizDisplay.hpp"
#include "xaifBooster/system/inc/BasicBlock.hpp"
#include "xaifBooster/algorithms/MemOpsTradeoffPreaccumulation/inc/ConceptuallyStaticInstances.hpp"
#include "xaifBooster/algorithms/MemOpsTradeoffPreaccumulation/inc/BasicBlockAlg.hpp"
#include "xaifBooster/algorithms/MemOpsTradeoffPreaccumulation/inc/EdgeElim.hpp"
#include "xaifBooster/algorithms/MemOpsTradeoffPreaccumulation/inc/VertexElim.hpp"
#include "xaifBooster/algorithms/MemOpsTradeoffPreaccumulation/inc/FaceElim.hpp"
#include "xaifBooster/algorithms/CrossCountryInterface/inc/Elimination.hpp"

using namespace xaifBoosterMemOpsTradeoffPreaccumulation;

namespace xaifBoosterCrossCountryInterface {

/**
 * First, a copy of the graph is made, so that the original can remain unmodified.
 * At the moment, vertex elimination can only be used on graphs where the independent
 * vertices are those vertices with no inedges, and the dependent vertices are the ones
 * with no outedges, rather than the distinctions specified by the independent and
 * dependent lists that come as a part of the original graph.  The first heuristic is
 * run on the set of possible eliminations, if it can't decide on a unique elimination
 * target, the second heuristic is run on the remaining elements, and so on, until
 * either forward mode or reverse mode is called.  These two heuristics always decide
 * on a single element.  The JAE list is updated as eliminations are made.  After
 * execution, theOriginal remains the same, and the JAE list is full of the
 * eliminations required to reduce the original to a bipartite graph.
 */
	
void Elimination::eliminate() {
  if (myType == REGULAR_ELIMTYPE) {

    ConceptuallyStaticInstances::HeuristicList HeuristicSequence = ConceptuallyStaticInstances::instance()->getList();

    //if no heuristics are passed on the command line, we use defaults as defined here
    if(HeuristicSequence.empty()){
      if(DbgLoggerManager::instance()->isSelected(DbgGroup::WARNING)){
        std::cout << "Error parsing command line heuristic arguments, using default mode and heuristics" << std::endl;
      }
      HeuristicSequence.push_back(ConceptuallyStaticInstances::EDGE);
      HeuristicSequence.push_back(ConceptuallyStaticInstances::MARKOWITZ);
      HeuristicSequence.push_back(ConceptuallyStaticInstances::SIBLING2);
      HeuristicSequence.push_back(ConceptuallyStaticInstances::REVERSE);
    }

    //FACE ELIMINATION
    if(HeuristicSequence.front() == ConceptuallyStaticInstances::FACE){

      DualGraph theDual (getLCG());

      //display the dual graph
      if(DbgLoggerManager::instance()->isSelected(DbgGroup::GRAPHICS)) {
	GraphVizDisplay::show(theDual,"Dual");
      }

      typedef void(*faceHeuristicFunc) (DualGraph&,
					DualGraph::FacePointerList&,
					const DualGraph::VertexPointerList&,
				        const DualGraph::VertexPointerList&,
					DualGraphVertex*);

      std::list<faceHeuristicFunc> faceHeuristicSequence;

      HeuristicSequence.pop_front();
      //generate a list of function pointers that point to each heuristic that is to be used
      while(!HeuristicSequence.empty()){
	switch(HeuristicSequence.front()){
	  case ConceptuallyStaticInstances::ABSORB:
	    faceHeuristicSequence.push_back(&FaceElim::absorbMode_f);
	    break;
	  default:
	    THROW_LOGICEXCEPTION_MACRO("Error: unknown face heuristic passed");
	}// end switch HeuristicSequence
	HeuristicSequence.pop_front();
      }// end while

      faceHeuristicFunc func_pt;
      std::list<faceHeuristicFunc>::iterator fhiter;
      DualGraph::VertexPointerList thePredList, theSuccList;
      DualGraphVertex* newOrAbsorb = NULL;

      //populate the dual path and elim lists
      theDual.populatePathList();
      DualGraph::FacePointerList theElimList = theDual.populateElimList();

      while(!theElimList.empty()){

 	//this loop runs the list through each heuristic
 	for(fhiter = faceHeuristicSequence.begin(); fhiter != faceHeuristicSequence.end(); fhiter++){
 	  func_pt = *fhiter;
 	  func_pt(theDual, theElimList, thePredList, theSuccList, newOrAbsorb);
 	}// end for

 	//if(theElimList.size() == 1){//if the heuristics have decided on one single face

	//the predlist and thesucclist hold the predecessors and successors respectively of the face about to be eliminated.
	//this information is stored in case one of the heuristics needs to use it to make it's determinations.
	thePredList.clear();
	theSuccList.clear();
	DualGraph::InEdgeIteratorPair newpreds (theDual.getInEdgesOf(theDual.getSourceOf(*theElimList.back())));
	DualGraph::InEdgeIterator newpredi (newpreds.first), newprede (newpreds.second);
	//go through predecessors and add them to the list of predecessors
	for(; newpredi != newprede; ++newpredi){
	  thePredList.push_back(&theDual.getSourceOf(*newpredi));
	}// end for
	DualGraph::OutEdgeIteratorPair newsuccs (theDual.getOutEdgesOf(theDual.getTargetOf(*theElimList.back())));
	DualGraph::OutEdgeIterator newsucci (newsuccs.first), newsucce (newsuccs.second);
	//go through successors and add them to the list of successors
	for(; newsucci != newsucce; ++newsucci){
	  theSuccList.push_back(&theDual.getTargetOf(*newsucci));
	}// end for

	//eliminate the face
	newOrAbsorb = theDual.elim_face(*theElimList.back(), thePredList, theSuccList, getEliminationResult().myJAEList);
	//}
	//else{
	//THROW_LOGICEXCEPTION_MACRO("Error: Heuristics could not decide on a single face");
	//}

	if(DbgLoggerManager::instance()->isSelected(DbgGroup::GRAPHICS)) {
	  GraphVizDisplay::show(theDual,"intermediate");
	}

	//regenerate path list and list of eliminatable faces
	theDual.clearPathList();
 	theDual.populatePathList();
	theElimList = theDual.populateElimList();

      }// end while

      //iterate through remaining intermediate vertices and set corresponding expressions as jacobian entries
      DualGraph::VertexIteratorPair jvip (theDual.vertices());
      DualGraph::VertexIterator jvi (jvip.first), jv_end (jvip.second);
      for(; jvi != jv_end; ++jvi){

	//check to see if the edge was an original edge, if so, create a JAE comprised of one vertex for it
	if((*jvi).getRefType() == DualGraphVertex::TO_ORIGINAL_EDGE){

	  JacobianAccumulationExpressionCopy* theNewExpression = new JacobianAccumulationExpressionCopy(getEliminationResult().myJAEList.addExpression());

	  xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionVertex& orig = ((*theNewExpression).myExpression).addVertex();
	  orig.setExternalReference((*jvi).getOriginalRef());
	  (*theNewExpression).setMaximal(orig);
	  (*jvi).setJacobianRef(theNewExpression);

	  (((*jvi).getJacobianRef()).myExpression).setJacobianEntry(getLCG().getTargetOf((*jvi).getOriginalRef()), getLCG().getSourceOf((*jvi).getOriginalRef()));
	}// end if
	else if((*jvi).getRefType() == DualGraphVertex::TO_INTERNAL_EXPRESSION){

	  DualGraph::InEdgeIteratorPair iep (theDual.getInEdgesOf(*jvi));
	  DualGraph::InEdgeIterator ie (iep.first);
	  DualGraph::OutEdgeIteratorPair oep (theDual.getOutEdgesOf(*jvi));
	  DualGraph::OutEdgeIterator oe (oep.first);

	  (((*jvi).getJacobianRef()).myExpression).setJacobianEntry((theDual.getTargetOf(*oe)).getAssumedRef(), (theDual.getSourceOf(*ie)).getAssumedRef());

	}// end else if
      }// end for

      if(DbgLoggerManager::instance()->isSelected(DbgGroup::GRAPHICS)) {
	GraphVizDisplay::show(theDual,"tripartite");
      }

      DBG_MACRO(DbgGroup::CALLSTACK, "Heuristic Metrics: ab sum: " << theDual.absum);
      DBG_MACRO(DbgGroup::CALLSTACK, "Heuristic Metrics: op sum: " << theDual.opsum);

    }// end if face
    else{

      LinearizedComputationalGraphCopy theCopy;

      theCopy.sdsum = 0;
      theCopy.spsum = 0;
      theCopy.opsum = 0;

      struct vertexMap {
	const xaifBoosterCrossCountryInterface::LinearizedComputationalGraphVertex* original;
	xaifBoosterCrossCountryInterface::LinearizedComputationalGraphVertex* copy;
	bool topsorted;
      };
      //copymap maps the copy vertices to the original graph vertices
      vertexMap* copymap=new vertexMap[getLCG().numVertices()];
      unsigned int i = 0, s = 0, t = 0;

      //declaration of lists to hold information about the last vertex/edge eliminated
      LinearizedComputationalGraphCopy::VertexPointerList thePredList;
      LinearizedComputationalGraphCopy::VertexPointerList theSuccList;
    
      // Copy Vertices
      //declaration of iterators
      const xaifBoosterCrossCountryInterface::LinearizedComputationalGraph::VertexPointerList& originalIndependents = getLCG().getIndependentList();
      const xaifBoosterCrossCountryInterface::LinearizedComputationalGraph::VertexPointerList& originalDependents = getLCG().getDependentList();
      xaifBoosterCrossCountryInterface::LinearizedComputationalGraph::VertexPointerList::const_iterator oIndeps, oDeps;

      xaifBoosterCrossCountryInterface::LinearizedComputationalGraph::ConstVertexIteratorPair vip (getLCG().vertices());
      xaifBoosterCrossCountryInterface::LinearizedComputationalGraph::ConstVertexIterator vi (vip.first), v_end (vip.second);
      //iterate through vertices in the original graph, and create a vertex in the copy for each one
      for(; vi != v_end; ++vi){
	copymap[i].original = &*vi;
	copymap[i].copy = &theCopy.addVertex();
	copymap[i].topsorted = false;  //flag used for topsorting
    
	//Copy dependent and independent lists
	//go through indeps, if its in the indep list, add to the copy indep list
	for(oIndeps=originalIndependents.begin(); oIndeps != originalIndependents.end(); oIndeps++) {
	  if(copymap[i].original == *oIndeps){
	    theCopy.addToIndependentList(*copymap[i].copy);
	    break;
	  }// end if
	}// end for original indep list
      
	//go through deps, if its in the dep list, add to the copy dep list
	for(oDeps=originalDependents.begin(); oDeps != originalDependents.end(); oDeps++) {
	  if(copymap[i].original == *oDeps){
	    theCopy.addToDependentList(*copymap[i].copy);
	    break;
	  }// end if
	}// end for original indep list
	i++;
      }// end for

      //create a file and output a list of edges, which represents the graph
      std::ofstream graphdump;
      if(DbgLoggerManager::instance()->isSelected(DbgGroup::TEMPORARY)){
	graphdump.open("graphdump.txt");
      }// end if TEMPORARY

      // Copy Edges
      xaifBoosterCrossCountryInterface::LinearizedComputationalGraph::ConstEdgeIteratorPair eip (getLCG().edges());
      xaifBoosterCrossCountryInterface::LinearizedComputationalGraph::ConstEdgeIterator ei (eip.first), e_end (eip.second);
      for(; ei != e_end; ++ei){
	//set the source and target indices for each edge
	for(s = 0; copymap[s].original != &getLCG().getSourceOf(*ei); s++){}// end for
	for(t = 0; copymap[t].original != &getLCG().getTargetOf(*ei); t++){}// end for
	theCopy.addEdge(*(copymap[s].copy), *(copymap[t].copy)).setOriginalRef(*ei);

	if(DbgLoggerManager::instance()->isSelected(DbgGroup::TEMPORARY)){
	  graphdump << s << " " << t << std::endl;
	}// end if TEMPORARY

      }// end for

      if(DbgLoggerManager::instance()->isSelected(DbgGroup::TEMPORARY)){
	graphdump.close();
      }// end if TEMPORARY

      //create a topsorted vertex list
      i = 0;
      while(i < getLCG().numVertices()){  
	if(!copymap[i].topsorted){
	  LinearizedComputationalGraphCopy::InEdgeIteratorPair tsp (theCopy.getInEdgesOf(*copymap[i].copy));
	  LinearizedComputationalGraphCopy::InEdgeIterator ti (tsp.first), t_end (tsp.second);
	  for(; ti != t_end; ++ti){//go through sources and make sure they have been topsorted
	    unsigned int topindex = 0;
	    for(; copymap[topindex].copy != &theCopy.getSourceOf(*ti); topindex++){ //set topindex to the index of the source of ti
	    }// end for
	    if(!copymap[topindex].topsorted){//if there is an edge from a non-topsorted vertex, break and move on
	      break;
	    }// end if
	  }// end for
	  if(ti == t_end){//if all of the vertex's predecessors are topsorted, it can be topsorted next
	    theCopy.addToVertexList(*copymap[i].copy);
	    copymap[i].topsorted = true;
	    i = 0; //reset the loop
	  }// end if
	  else{
	    i++;
	  }// end else
	}// end if
	else{
	  i++;
	}// end else
      }// end while

      //VERTEX ELIMINATION
      if(HeuristicSequence.front() == ConceptuallyStaticInstances::VERTEX){

	typedef void(*vertexHeuristicFunc) (LinearizedComputationalGraphCopy&,
					    LinearizedComputationalGraphCopy::VertexPointerList&,
					    const LinearizedComputationalGraphCopy::VertexPointerList&,
					    const LinearizedComputationalGraphCopy::VertexPointerList&);

	std::list<vertexHeuristicFunc> vertexHeuristicSequence;

	HeuristicSequence.pop_front();
	//generate a list of function pointers that point to each heuristic that is to be used
	while(!HeuristicSequence.empty()){
	  switch(HeuristicSequence.front()){
	  case ConceptuallyStaticInstances::FORWARD:
	    vertexHeuristicSequence.push_back(&VertexElim::forwardMode_v);
	    break;
	  case ConceptuallyStaticInstances::REVERSE:
	    vertexHeuristicSequence.push_back(&VertexElim::reverseMode_v);
	    break;
	  case ConceptuallyStaticInstances::MARKOWITZ:
	    vertexHeuristicSequence.push_back(&VertexElim::markowitzMode_v);
	    break;
	  case ConceptuallyStaticInstances::SIBLING:
	    vertexHeuristicSequence.push_back(&VertexElim::siblingMode_v);
	    break;
	  case ConceptuallyStaticInstances::SIBLING2:
	    vertexHeuristicSequence.push_back(&VertexElim::sibling2Mode_v);
	    break;
	  case ConceptuallyStaticInstances::SUCCPRED:
	    vertexHeuristicSequence.push_back(&VertexElim::succPredMode_v);
	    break;
	  default:
	    THROW_LOGICEXCEPTION_MACRO("Error: unknown heuristic passed");
	  }// end switch HeuristicSequence
	  HeuristicSequence.pop_front();
	}// end while

	//the vertex list holds all the vertices that are candidates for elimination
	LinearizedComputationalGraphCopy::VertexPointerList theVertexList = theCopy.getVertexList();
	vertexHeuristicFunc func_pt;
	std::list<vertexHeuristicFunc>::iterator vhiter;
    
	//output list of eliminations to a file
	std::ofstream sequencedump;
	if(DbgLoggerManager::instance()->isSelected(DbgGroup::TEMPORARY)){
	  sequencedump.open("f.txt");
	}// end if TEMPORARY

	//this is the elimination loop, it eliminates one vertex per iteration
	while(!theVertexList.empty()){

	  theVertexList = theCopy.getVertexList();

	  //all vertices that are minimal or maximal are removed from the list, they cannot be eliminated
	  LinearizedComputationalGraphCopy::VertexIteratorPair cvip (theCopy.vertices());
	  LinearizedComputationalGraphCopy::VertexIterator cvi (cvip.first), cv_end (cvip.second);
	  for(; cvi != cv_end; ++cvi){
	    if((theCopy.numInEdgesOf(*cvi) == 0) || (theCopy.numOutEdgesOf(*cvi) == 0)){
	      theVertexList.remove(&*cvi);
	    }// end if
	  }// end for

	  //this loop runs the list through each heuristic
	  if(!theVertexList.empty()){
	    for(vhiter=vertexHeuristicSequence.begin(); vhiter!=vertexHeuristicSequence.end(); vhiter++){
	      func_pt = *vhiter;
	      func_pt(theCopy, theVertexList, thePredList, theSuccList);
	    }// end for
	  }// end if

	  if(theVertexList.size() == 1){
	    //the predlist and thesucclist hold the predecessors and successors respectively of the vertex about to be eliminated.
	    //this information is stored in case one of the heuristics needs to use it to make it's determinations.
	    thePredList.clear();
	    theSuccList.clear();
	    LinearizedComputationalGraphCopy::InEdgeIteratorPair newpreds (theCopy.getInEdgesOf(*theVertexList.front()));
	    LinearizedComputationalGraphCopy::InEdgeIterator newpredi (newpreds.first), newprede (newpreds.second);
	    //go through predecessors and add them to the list of predecessors
	    for(; newpredi != newprede; ++newpredi){
	      thePredList.push_back(&theCopy.getSourceOf(*newpredi));
	    }// end for
	    LinearizedComputationalGraphCopy::OutEdgeIteratorPair newsuccs (theCopy.getOutEdgesOf(*theVertexList.front()));
	    LinearizedComputationalGraphCopy::OutEdgeIterator newsucci (newsuccs.first), newsucce (newsuccs.second);
	    //go through successors and add them to the list of successors
	    for(; newsucci != newsucce; ++newsucci){
	      theSuccList.push_back(&theCopy.getTargetOf(*newsucci));
	    }// end for
	
	    if(DbgLoggerManager::instance()->isSelected(DbgGroup::TEMPORARY)){
	      for(s = 0; copymap[s].copy != theVertexList.front(); s++){}// end for
	      //output vertex index
	      sequencedump << s << std::endl;
	    }// end if TEMPORARY

	    theCopy.elim_vertex(*theVertexList.front(), getEliminationResult().myJAEList);

	    if(DbgLoggerManager::instance()->isSelected(DbgGroup::GRAPHICS)) {
	      GraphVizDisplay::show(theCopy,"intermediate");
	    }

	  }// end if
	  else if(theVertexList.size() > 1){
	    THROW_LOGICEXCEPTION_MACRO("Error: More than one vertex in list of possible eliminations");
	  }// end else

	}// end while

	if(DbgLoggerManager::instance()->isSelected(DbgGroup::TEMPORARY)){
	  sequencedump.close();
	}// end if TEMPORARY

      }// end if

      //EDGE ELIMINATION
      else if(HeuristicSequence.front() == ConceptuallyStaticInstances::EDGE){

	typedef void(*edgeHeuristicFunc) (LinearizedComputationalGraphCopy&,
					  LinearizedComputationalGraphCopy::EdgePointerList&,
					  const LinearizedComputationalGraphCopy::VertexPointerList&,
					  const LinearizedComputationalGraphCopy::VertexPointerList&);
	std::list<edgeHeuristicFunc> edgeHeuristicSequence;

	HeuristicSequence.pop_front();
	//generate a list of function pointers that point to each heuristic that is to be used
	while(!HeuristicSequence.empty()){
	  switch(HeuristicSequence.front()){
	  case ConceptuallyStaticInstances::FORWARD:
	    edgeHeuristicSequence.push_back(&EdgeElim::forwardMode_e);
	    break;
	  case ConceptuallyStaticInstances::REVERSE:
	    edgeHeuristicSequence.push_back(&EdgeElim::reverseMode_e);
	    break;
	  case ConceptuallyStaticInstances::MARKOWITZ:
	    edgeHeuristicSequence.push_back(&EdgeElim::markowitzMode_e);
	    break;
	  case ConceptuallyStaticInstances::SIBLING2:
	    edgeHeuristicSequence.push_back(&EdgeElim::sibling2Mode_e);
	    break;
	  case ConceptuallyStaticInstances::SUCCPRED:
	    edgeHeuristicSequence.push_back(&EdgeElim::succPredMode_e);
	    break;
	  default:
	    THROW_LOGICEXCEPTION_MACRO("Error: Unknown heuristic passed");
	  }// end switch HeuristicSequence
	  HeuristicSequence.pop_front();
	}// end while
      
	edgeHeuristicFunc func_pt;
	std::list<edgeHeuristicFunc>::iterator ehiter;

	//output list of eliminations to a file
	std::ofstream sequencedump;
	if(DbgLoggerManager::instance()->isSelected(DbgGroup::TEMPORARY)){
	  sequencedump.open("f.txt");
	}// end if TEMPORARY

	//populate a list of edge eliminations
	LinearizedComputationalGraphCopy::EdgePointerList theEdgeList = theCopy.populateEdgeList();

	//this is the elimination loop, it eliminates one edge per iteration
	while(!theEdgeList.empty()){
	
	  //send theEdgeList to each heuristic
	  for(ehiter = edgeHeuristicSequence.begin(); ehiter != edgeHeuristicSequence.end(); ehiter++){
	    func_pt = *ehiter;
	    func_pt(theCopy, theEdgeList, thePredList, theSuccList);
	  }// end for

	  if(theEdgeList.size() == 1){//if the heuristics have decided on one single edge

	    //reset pred and succ lists and eliminate the edge as specified
	    thePredList.clear();
	    theSuccList.clear();
	    if(theEdgeList.front().direction == LinearizedComputationalGraphCopy::FRONT){
	      thePredList.push_back(&theCopy.getSourceOf(*(theEdgeList.front().edge_p)));
	      LinearizedComputationalGraphCopy::OutEdgeIteratorPair succs (theCopy.getOutEdgesOf(theCopy.getTargetOf(*(theEdgeList.front().edge_p))));
	      LinearizedComputationalGraphCopy::OutEdgeIterator succi (succs.first), succ_end (succs.second);
	      for(; succi != succ_end; ++succi){//go through successors and add them to the list of successors
		theSuccList.push_back(&theCopy.getTargetOf(*succi));
	      }// end for

	      if(DbgLoggerManager::instance()->isSelected(DbgGroup::TEMPORARY)){
		for(s = 0; copymap[s].copy != &theCopy.getSourceOf(*(theEdgeList.front()).edge_p); s++){}// end for
		for(t = 0; copymap[t].copy != &theCopy.getTargetOf(*(theEdgeList.front()).edge_p); t++){}// end for
		//output source and target indices
		sequencedump << s << " " << t << std::endl;
	      }// end if TEMPORARY

	      theCopy.front_elim_edge(*(theEdgeList.front().edge_p), getEliminationResult().myJAEList);
	    }// end if
	    else if((theEdgeList.front()).direction == LinearizedComputationalGraphCopy::BACK){
	      theSuccList.push_back(&theCopy.getTargetOf(*(theEdgeList.front().edge_p)));
	      LinearizedComputationalGraphCopy::InEdgeIteratorPair preds (theCopy.getInEdgesOf(theCopy.getSourceOf(*((theEdgeList.front()).edge_p))));
	      LinearizedComputationalGraphCopy::InEdgeIterator predi (preds.first), pred_end (preds.second);
	      for(; predi != pred_end; ++predi){//go through predecessors and add them to the list of predecessors
		thePredList.push_back(&theCopy.getSourceOf(*predi));
	      }// end for

	      if(DbgLoggerManager::instance()->isSelected(DbgGroup::TEMPORARY)){
		for(s = 0; copymap[s].copy != &theCopy.getSourceOf(*(theEdgeList.front()).edge_p); s++){}// end for
		for(t = 0; copymap[t].copy != &theCopy.getTargetOf(*(theEdgeList.front()).edge_p); t++){}// end for
		//output source and target indices
		sequencedump << s << " " << t << std::endl;
	      }// end if TEMPORARY

	      theCopy.back_elim_edge(*(theEdgeList.front().edge_p), getEliminationResult().myJAEList);
	    }// end else if
	    else{
	      THROW_LOGICEXCEPTION_MACRO("Error: Edge has no elimination direction specified");
	    }
	
	    if(DbgLoggerManager::instance()->isSelected(DbgGroup::GRAPHICS)) {
	      GraphVizDisplay::show(theCopy,"intermediate");
	    }

	  }// end if
	  else{
	    THROW_LOGICEXCEPTION_MACRO("Error: More than one edge in list of possible eliminations");
	  }// end else

	  //clear edge list and pupolate a new new one
	  theCopy.clearEdgeList();
	  theEdgeList = theCopy.populateEdgeList();

	}// end while
      
	if(DbgLoggerManager::instance()->isSelected(DbgGroup::TEMPORARY)){
	  sequencedump.close();
	}// end if TEMPORARY

      }// end else if

      //iterate through remaining edges and set corresponding expressions as jacobian entries
      LinearizedComputationalGraphCopy::EdgeIteratorPair jeip (theCopy.edges());
      LinearizedComputationalGraphCopy::EdgeIterator ci (jeip.first), c_end (jeip.second);
      for(; ci != c_end; ++ci){
	unsigned int indepindex = 0;
	for(; copymap[indepindex].copy != &theCopy.getSourceOf(*ci); indepindex++){
	}
	unsigned int depindex = 0;
	for(; copymap[depindex].copy != &theCopy.getTargetOf(*ci); depindex++){
	}
	//check to see if the edge was an original edge, if so, create a JAE comprised of one vertex for it
	if((*ci).getRefType() == LinearizedComputationalGraphCopyEdge::TO_ORIGINAL_EDGE){
	  JacobianAccumulationExpressionCopy* theNewExpression = new JacobianAccumulationExpressionCopy(getEliminationResult().myJAEList.addExpression());
	
	  xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionVertex& orig = ((*theNewExpression).myExpression).addVertex();
	  orig.setExternalReference((*ci).getOriginalRef());
	  (*theNewExpression).setMaximal(orig);
	  (*ci).setJacobianRef(theNewExpression);
	}// end if

	(((*ci).getJacobianRef()).myExpression).setJacobianEntry(*(copymap[depindex].original), *(copymap[indepindex].original));
      }// end for
      if (DbgLoggerManager::instance()->isSelected(DbgGroup::GRAPHICS)) {
	GraphVizDisplay::show(theCopy,"bipartite");
      }

      DBG_MACRO(DbgGroup::CALLSTACK, "Heuristic Metrics: sd sum: " << theCopy.sdsum);
      DBG_MACRO(DbgGroup::CALLSTACK, "Heuristic Metrics: sp sum: " << theCopy.spsum);
      DBG_MACRO(DbgGroup::CALLSTACK, "Heuristic Metrics: op sum: " << theCopy.opsum);

      delete[] copymap;

    }// end else (vertex or edge)
  } // end if elim type is regular
  else
    THROW_LOGICEXCEPTION_MACRO("Error: eliminate() called from within MemOps with invalid elimination type");
} // end of eliminate()

} // end of namespace xaifBoosterCrossCountryInterface
 
