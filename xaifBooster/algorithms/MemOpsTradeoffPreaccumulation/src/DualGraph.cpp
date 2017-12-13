// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/algorithms/MemOpsTradeoffPreaccumulation/inc/DualGraph.hpp"
#include <algorithm>
using namespace MemOpsTradeoffPreaccumulation;

namespace MemOpsTradeoffPreaccumulation {

  DualGraph::DualGraph(const LinearizedComputationalGraph& theOriginal){

    absum = 0;
    opsum = 0;
    
    //Create Dual Graph Vertices
    xaifBoosterCrossCountryInterface::LinearizedComputationalGraph::ConstEdgeIteratorPair oep (theOriginal.edges());
    xaifBoosterCrossCountryInterface::LinearizedComputationalGraph::ConstEdgeIterator oei (oep.first), oe_end (oep.second);
    //iterate through edges in the original graph, and create a vertex in the dual for each one
    for(; oei != oe_end; ++oei){
      DualGraphVertex& DV = addVertex();
      DV.setOriginalRef(*oei);
    }// end for original edges

    //iterate through independant and dependant vertices and create assumed in and out edges, respectively
    //declaration of iterators
    const xaifBoosterCrossCountryInterface::LinearizedComputationalGraph::VertexPointerList& originalIndependents = theOriginal.getIndependentList();
    const xaifBoosterCrossCountryInterface::LinearizedComputationalGraph::VertexPointerList& originalDependents = theOriginal.getDependentList();
    xaifBoosterCrossCountryInterface::LinearizedComputationalGraph::VertexPointerList::const_iterator oIndepi, oDepi;

    for(oIndepi = originalIndependents.begin(); oIndepi != originalIndependents.end(); oIndepi++) {
      DualGraphVertex& DV = addVertex();
      DV.setAssumedInEdgeRef(**oIndepi);

      // for each outedge of the vertex, make an edge from that outedge vertex to the assumed inedge vertex
      xaifBoosterCrossCountryInterface::LinearizedComputationalGraph::ConstOutEdgeIteratorPair doe (theOriginal.getOutEdgesOf(**oIndepi));
      xaifBoosterCrossCountryInterface::LinearizedComputationalGraph::ConstOutEdgeIterator doei (doe.first), doe_end (doe.second);
      for(; doei != doe_end; ++doei){
	addEdge(DV, getDualVertex(*doei));
      }// end for outedges
    }// end for original indep list
      
    for(oDepi = originalDependents.begin(); oDepi != originalDependents.end(); oDepi++) {
      DualGraphVertex& DV = addVertex();
      DV.setAssumedOutEdgeRef(**oDepi);

      // for each inedge of the vertex, make an edge from the inedge to the assumed inedge vertex
      xaifBoosterCrossCountryInterface::LinearizedComputationalGraph::ConstInEdgeIteratorPair die (theOriginal.getInEdgesOf(**oDepi));
      xaifBoosterCrossCountryInterface::LinearizedComputationalGraph::ConstInEdgeIterator diei (die.first), die_end (die.second);
      for(; diei != die_end; ++diei){
	addEdge(getDualVertex(*diei), DV);
      }// end for inedges
    }// end for original indep list

    //generate edges in dual graph
    //for all vertices in the original graph
    xaifBoosterCrossCountryInterface::LinearizedComputationalGraph::ConstVertexIteratorPair dv2 (theOriginal.vertices());
    xaifBoosterCrossCountryInterface::LinearizedComputationalGraph::ConstVertexIterator dv2i (dv2.first), dv2_end (dv2.second);
    for(; dv2i != dv2_end; ++dv2i){
      //for all in edges
      xaifBoosterCrossCountryInterface::LinearizedComputationalGraph::ConstInEdgeIteratorPair die2 (theOriginal.getInEdgesOf(*dv2i));
      xaifBoosterCrossCountryInterface::LinearizedComputationalGraph::ConstInEdgeIterator die2i (die2.first), die2_end (die2.second);
      for(; die2i != die2_end; ++die2i){
	//for all out edges
	xaifBoosterCrossCountryInterface::LinearizedComputationalGraph::ConstOutEdgeIteratorPair doe2 (theOriginal.getOutEdgesOf(*dv2i));
	xaifBoosterCrossCountryInterface::LinearizedComputationalGraph::ConstOutEdgeIterator doe2i (doe2.first), doe2_end (doe2.second);
	for(; doe2i != doe2_end; ++doe2i){
	  addEdge(getDualVertex(*die2i),getDualVertex(*doe2i));
	}// end for outedges
      }// end for inedges
    }// end for original vertices

    //mark all final vertices as such
    DualGraph::VertexIteratorPair fip (vertices());
    DualGraph::VertexIterator fipi (fip.first), fip_end (fip.second);
    for(; fipi != fip_end; ++fipi){
      (*fipi).final = isFinal(*fipi);
    }// end for all vertices in the dual graph

  }// end constructor

  DualGraph::~DualGraph() {
    clearPathList();
  }

  const DualGraphVertex& DualGraph::getDualVertex(const xaifBoosterCrossCountryInterface::LinearizedComputationalGraphEdge& theEdge) const {

    DualGraph::ConstVertexIteratorPair dvip (vertices());
    DualGraph::ConstVertexIterator dvi (dvip.first), dv_end (dvip.second);
    for(; dvi != dv_end; ++dvi){
      if((*dvi).getRefType() == DualGraphVertex::TO_ORIGINAL_EDGE){
	if(&(*dvi).getOriginalRef() == &theEdge){
	  return *dvi;
	}
      }
    }
    THROW_LOGICEXCEPTION_MACRO("ERROR: Attempt to determine corresponding dual vertex for an edge which does not have one"); 
  }// end getDualVertex

  void DualGraph::populatePathList() {

    //create paths for independent vertices
    DualGraph::VertexIteratorPair dvip (vertices());
    DualGraph::VertexIterator dvi (dvip.first), dv_end (dvip.second);
    for(; dvi != dv_end; ++dvi){
      if(numInEdgesOf(*dvi) == 0){
	DualGraphPath* thenewpath_p = new DualGraphPath;
	((*thenewpath_p).myPath).push_back(&*dvi);
	(*thenewpath_p).pathNum = 0;
	myPathList.push_front(thenewpath_p);
	if(numOutEdgesOf(*dvi) == 0){
	  (*thenewpath_p).setComplete();
	}// end if
	else{
	  copyPath(thenewpath_p);
	}// end else
      }// end if
    }// end for

    DualGraph::PathList::iterator pathi;
    for(pathi = myPathList.begin(); pathi != myPathList.end(); pathi++){
      while(!(**pathi).isComplete()){
	//iterate through successors of the last vertex in the path
        DualGraph::OutEdgeIteratorPair doe (getOutEdgesOf(*((**pathi).myPath).back()));
	DualGraph::OutEdgeIterator doei (doe.first), doe_end (doe.second);
	unsigned int i = 0;
	//loop finds the correct successor to expand the path to
	for(; doei != doe_end; ++doei){
	  if((**pathi).pathNum == i){
	    break;
	  }// end if
	  i++;
	}// end for outedges
	if(doei == doe_end){
	  THROW_LOGICEXCEPTION_MACRO("Error: No match between pathnum and out edge num could be found");
	}// end if
	((**pathi).myPath).push_back(&getTargetOf(*doei));
	(**pathi).pathNum = 0;
	if(numOutEdgesOf(getTargetOf(*doei)) == 0) {
	  (**pathi).setComplete();
	}// end if
	else{
	  copyPath(*pathi);
	}// end else
	pathi = myPathList.begin();
      }// end while not complete
    }// end for all paths

  }// end populate path list

  void DualGraph::copyPath(DualGraphPath* thepath_p) {
    
    unsigned int i;
    for(i = 1; i < numOutEdgesOf(*((*thepath_p).myPath).back()); i++){      
       DualGraphPath* thenewpath_p= new DualGraphPath;
       (*thenewpath_p).myPath = (*thepath_p).myPath;
       (*thenewpath_p).pathNum = i;
       myPathList.push_front(thenewpath_p);
    }// end for

  }// end copyPath

  void DualGraph::clearPathList() {

    DualGraph::PathList::iterator lclear;
    for(lclear = myPathList.begin(); lclear != myPathList.end(); lclear++) {
      delete *lclear;
    }// end for
    
    myPathList.clear();

  }// end clear path list 

  DualGraph::FacePointerList DualGraph::populateElimList() {

    FacePointerList myElimList;

    //iterate over all faces in the dual graph
    EdgeIteratorPair deip (edges());
    EdgeIterator dei (deip.first), de_end (deip.second);
    for(; dei != de_end; ++dei){
      //if the face is an intermediate and has no path conflicts
      if((numInEdgesOf(getSourceOf(*dei)) > 0) && (numOutEdgesOf(getTargetOf(*dei)) > 0)){
	if((getSourceOf(*dei).final) || (getTargetOf(*dei).final)){
	  myElimList.push_back(&*dei);
	}// end if
      }// end if
    }// end for edges

    return myElimList;
  }// end populateElimlist

  bool DualGraph::isFinal(DualGraphVertex& theVertex) const {

    //spath iterates through each path in the list
    DualGraph::PathList::const_iterator spath;
    //svertex iterates through each vertex in the path
    DualGraphPath::Path::iterator svertex;

    //if there is any path from a pred to a succ not containing theVertex, return false

    DualGraph::ConstInEdgeIteratorPair die (getInEdgesOf(theVertex));
    DualGraph::ConstInEdgeIterator diei (die.first), die_end (die.second);
    DualGraph::ConstOutEdgeIteratorPair doe (getOutEdgesOf(theVertex));
    DualGraph::ConstOutEdgeIterator doei (doe.first), doe_end (doe.second);

    for(; diei != die_end; ++diei){
      if(numOutEdgesOf(getSourceOf(*diei)) > 1){
	for(; doei != doe_end; ++doei){
	  if(numInEdgesOf(getTargetOf(*doei)) > 1){
	    for(spath = myPathList.begin(); spath != myPathList.end(); spath++){
	      for(svertex = ((**spath).myPath).begin(); svertex != ((**spath).myPath).end(); svertex++){
		if(*svertex == &getSourceOf(*diei)){
		  //if path contains the vertex, go to next path
		  svertex++;
		  if(*svertex == &theVertex){break;}
		  svertex++;
		  svertex++;
		  for(; svertex != ((**spath).myPath).end(); svertex++){
		    if(*svertex == &getTargetOf(*doei)){
		      return false;
		    }// end if we find 
		  }// end for all vertices after the pred
		  //if we dont find the succ, go to next path
		  break;
		}// end if vertex is the pred
	      }// end for each vertex in the path
	    }// end for all paths
	  }// end if succ has alternate paths
	}// end for all out edges
      }// end if pred has alternate paths
    }// end for all inedges

    return true;
  }// end isFinal

  DualGraphVertex* DualGraph::elim_face(DualGraphEdge& theFace,
					const DualGraph::VertexPointerList& thePredList,
					const DualGraph::VertexPointerList& theSuccList,
					xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionList& theJacobianAccumulationExpressionList){

    opsum ++;

    //create a new JAE graph
    JacobianAccumulationExpressionCopy* theExpression = new JacobianAccumulationExpressionCopy(theJacobianAccumulationExpressionList.addExpression());

    //add first minimal vertex: the source of the face
    xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionVertex& minisource = ((*theExpression).myExpression).addVertex();
    if((getSourceOf(theFace)).getRefType() == DualGraphVertex::TO_INTERNAL_EXPRESSION){
      minisource.setInternalReference(((getSourceOf(theFace)).getJacobianRef()).getMaximal());
    }
    else{
      minisource.setExternalReference((getSourceOf(theFace)).getOriginalRef());
    }
    //add second minimal vertex: the target of the face
    xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionVertex& minitarget = ((*theExpression).myExpression).addVertex();    
    if((getTargetOf(theFace)).getRefType() == DualGraphVertex::TO_INTERNAL_EXPRESSION){
      minitarget.setInternalReference(((getTargetOf(theFace)).getJacobianRef()).getMaximal());
    }// end if
    else{
      minitarget.setExternalReference((getTargetOf(theFace)).getOriginalRef());
    }// end else

    //create "mult" vertex and connect it to minisource and minitarget
    xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionVertex& mult = ((*theExpression).myExpression).addVertex();
    mult.setOperation(xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionVertex::MULT_OP);
    ((*theExpression).myExpression).addEdge(minisource, mult);
    ((*theExpression).myExpression).addEdge(minitarget, mult);

    //create a new vertex and set its jacobian ref to the new JAE
    DualGraphVertex& theNewVertex = addVertex();
    theNewVertex.setJacobianRef(theExpression);

    // iterate over predecessors of the source and successors of the target, connect them all to the new vertex
    DualGraph::InEdgeIteratorPair die (getInEdgesOf(getSourceOf(theFace)));
    DualGraph::InEdgeIterator diei (die.first), die_end (die.second);
    for(; diei != die_end; ++diei){
      addEdge(getSourceOf(*diei), theNewVertex);
    }// end for inedges
    DualGraph::OutEdgeIteratorPair doe (getOutEdgesOf(getTargetOf(theFace)));
    DualGraph::OutEdgeIterator doei (doe.first), doe_end (doe.second);
    for(; doei != doe_end; ++doei){
      addEdge(theNewVertex, getTargetOf(*doei));
    }// end for outedges
    
    //check for direct vertex
    DualGraphVertex* directVertex_pt = NULL;
    //spath iterates through each path in the list
    DualGraph::PathList::const_iterator spath;
    //svertex iterates through each vertex in the path
    DualGraphPath::Path::iterator svertex;
    //predi and succi iterate over the predecessor and successor lists of the face being eliminated
    DualGraph::VertexPointerList::const_iterator predi, succi;
    bool loopcheck = true;
    for(predi = thePredList.begin(); (predi != thePredList.end()) && loopcheck; ++predi){
      if(numOutEdgesOf(**predi) > 1){
	for(succi = theSuccList.begin(); (succi != theSuccList.end()) && loopcheck; ++succi){
	  if(numInEdgesOf(**succi) > 1){
	    for(spath = myPathList.begin(); (spath != myPathList.end()) && loopcheck; spath++){
	      for(svertex = ((**spath).myPath).begin(); svertex != ((**spath).myPath).end(); svertex++){
		if(*svertex == *predi){
		  //if path contains the vertex, go to next vertex in the path
		  svertex++;
		  if(numOutEdgesOf(**svertex) == 0){
		    break; //break out of this path
		  }
		  svertex++;
		  if(*svertex == *succi){
		    svertex--;
		    directVertex_pt = *svertex;
		    loopcheck = false;
		    break; //break out of this path
		  }// end if succ matches
		  break;  //break out of this path
		}// end if vertex is the pred
	      }// end for each vertex in the path
	    }// end for all paths
	  }// end if succ has alternate paths
	}// end for all out edges
      }// end if pred has alternate path
    }// end for all inedges

    //if there was a direct vertex from predecessors to successors
    if(directVertex_pt != NULL){
      //create vertices for direct edge and "add" vertex
      xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionVertex& minidirect = ((*theExpression).myExpression).addVertex();
      xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionVertex& add = ((*theExpression).myExpression).addVertex();
      add.setOperation(xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionVertex::ADD_OP);

      //connect "add" vertex to minidirect and mult, point the new vertex to add
      ((*theExpression).myExpression).addEdge(mult, add);
      ((*theExpression).myExpression).addEdge(minidirect, add);
      (*theExpression).setMaximal(add);

      if((*directVertex_pt).getRefType() == DualGraphVertex::TO_INTERNAL_EXPRESSION){
	minidirect.setInternalReference(((*directVertex_pt).getJacobianRef()).getMaximal());
      }
      else if((*directVertex_pt).getRefType() == DualGraphVertex::TO_ORIGINAL_EDGE){
	minidirect.setExternalReference((*directVertex_pt).getOriginalRef());
      }
      else{
	THROW_LOGICEXCEPTION_MACRO("Error: Direct vertex ref is neither internal nor external");
      }

      //delete the old vertex
      removeAndDeleteVertex(*directVertex_pt);
      (*theExpression).setMaximal(add);
    }// end if
    else{      //point the new vertex to "mult"
      (*theExpression).setMaximal(mult);
    }// end else
  
    //if the face elimination isolates both the source and the target, delete them both (which automatically deletes the face)
    if((numOutEdgesOf(getSourceOf(theFace)) == 1) && (numInEdgesOf(getTargetOf(theFace)) == 1)){
      removeAndDeleteVertex(getSourceOf(theFace));
      removeAndDeleteVertex(getTargetOf(theFace));
    }// end if
    //if the face elimination isolates the source, delete the source (which automatically deletes the face)
    else if(numOutEdgesOf(getSourceOf(theFace)) == 1){
      removeAndDeleteVertex(getSourceOf(theFace));
    }
    //if the face elimination isolates the target, delete the target (which automatically deletes the face)
    else if(numInEdgesOf(getTargetOf(theFace)) == 1){
      removeAndDeleteVertex(getTargetOf(theFace));
    }
    // if the elimination does not isolate either the source or the target, we simply delete the face
    else{
      removeAndDeleteEdge(theFace);
    }

    //set final for the new vertex
    theNewVertex.final = isFinal(theNewVertex);
    return &theNewVertex;
  }// end elim_face

}// end namespace MemOpsTradeoffPreaccumulation
