// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/algorithms/MemOpsTradeoffPreaccumulation/inc/LinearizedComputationalGraphCopy.hpp"

namespace MemOpsTradeoffPreaccumulation{

  void LinearizedComputationalGraphCopy::front_elim_edge(LinearizedComputationalGraphCopyEdge& theEdge,
							 xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionList& theJacobianAccumulationExpressionList){

    //iterator over outedges of theTarget
    LinearizedComputationalGraphCopy::OutEdgeIteratorPair outedges (getOutEdgesOf(getTargetOf(theEdge)));
    LinearizedComputationalGraphCopy::OutEdgeIterator oe=outedges.first, oend=outedges.second;
    for(; oe != oend; ++oe){

      opsum++;

      JacobianAccumulationExpressionCopy* theExpression = new JacobianAccumulationExpressionCopy(theJacobianAccumulationExpressionList.addExpression());

      //add first minimal vertex: the edge being eliminated, set its pointer appropriately
      xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionVertex& miniin = ((*theExpression).myExpression).addVertex();
      if(theEdge.getRefType() == LinearizedComputationalGraphCopyEdge::TO_INTERNAL_EXPRESSION){
	miniin.setInternalReference((theEdge.getJacobianRef()).getMaximal());
      }
      else{
        miniin.setExternalReference(theEdge.getOriginalRef());
      }
      //add second minimal vertex: edge out of theVertex
      xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionVertex& miniout = ((*theExpression).myExpression).addVertex();    
      if((*oe).getRefType() == LinearizedComputationalGraphCopyEdge::TO_INTERNAL_EXPRESSION){
        miniout.setInternalReference(((*oe).getJacobianRef()).getMaximal());
      }// end if
      else{
        miniout.setExternalReference((*oe).getOriginalRef());
      }// end else
      //create "mult" vertex and connect it to miniin and miniout
      xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionVertex& mult = ((*theExpression).myExpression).addVertex();
      mult.setOperation(xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionVertex::MULT_OP);
      ((*theExpression).myExpression).addEdge(miniin, mult);
      ((*theExpression).myExpression).addEdge(miniout, mult);
      
      //iterator used to check for a direct edge from source to target
      LinearizedComputationalGraphCopy::OutEdgeIteratorPair srcoutedges (getOutEdgesOf(getSourceOf(theEdge)));
      LinearizedComputationalGraphCopy::OutEdgeIterator srcoe = srcoutedges.first, srcoend = srcoutedges.second;
      LinearizedComputationalGraphCopyEdge* directEdge_pt = NULL;
      for(; srcoe != srcoend; ++srcoe){
	//if there already was a direct edge from the source vertex to the target vertex, directEdge_pt is set to point to that edge
        if(&getTargetOf(*srcoe) == &getTargetOf(*oe)){
          directEdge_pt = &*srcoe;
          break;
        } //endif
      }//end while
      
      LinearizedComputationalGraphCopyEdge& theNewEdge = addEdge(getSourceOf(theEdge), getTargetOf(*oe));
      
      theNewEdge.setJacobianRef(theExpression);
      if(directEdge_pt != NULL){        //create vertex for direct edge, and "add" vertex, point new edge to it
        xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionVertex& add = ((*theExpression).myExpression).addVertex();
        add.setOperation(xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionVertex::ADD_OP);
        //add vertex for direct edge
        xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionVertex& minidirect = ((*theExpression).myExpression).addVertex();
        if((*directEdge_pt).getRefType() == LinearizedComputationalGraphCopyEdge::TO_INTERNAL_EXPRESSION){
          minidirect.setInternalReference(((*directEdge_pt).getJacobianRef()).getMaximal());
        }
        else{
          minidirect.setExternalReference((*directEdge_pt).getOriginalRef());
        }
	//connect "add" vertex to minidirect and mult
        ((*theExpression).myExpression).addEdge(mult, add);
        ((*theExpression).myExpression).addEdge(minidirect, add);
        removeAndDeleteEdge(*directEdge_pt);
        (*theExpression).setMaximal(add);
      }// end if
      else{      //point the new edge to "mult"
        (*theExpression).setMaximal(mult);	  
      }// end else
    }// end outedge iteration

    //if the front elimination isolates the vertex, delete the vertex (which automatically deletes the edge)
    if(numInEdgesOf(getTargetOf(theEdge)) == 1){
      removeFromVertexList(getTargetOf(theEdge));
      removeAndDeleteVertex(getTargetOf(theEdge));
    }// end if
    // if the elimination does not isolate the target vertex, we simply delete the edge
    else{
      removeAndDeleteEdge(theEdge);
    }

  }// end front_elim_edge

  void LinearizedComputationalGraphCopy::back_elim_edge(LinearizedComputationalGraphCopyEdge& theEdge,
							xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionList& theJacobianAccumulationExpressionList){

    //iterator over inedges of theVertex
    LinearizedComputationalGraphCopy::InEdgeIteratorPair inedges (getInEdgesOf(getSourceOf(theEdge)));
    LinearizedComputationalGraphCopy::InEdgeIterator ie=inedges.first, iend=inedges.second;
    for(; ie != iend; ++ie){

      opsum++;

      JacobianAccumulationExpressionCopy* theExpression = new JacobianAccumulationExpressionCopy(theJacobianAccumulationExpressionList.addExpression());

      //add first minimal vertex: edge into theVertex
      xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionVertex& miniin = ((*theExpression).myExpression).addVertex();
      if((*ie).getRefType() == LinearizedComputationalGraphCopyEdge::TO_INTERNAL_EXPRESSION){
        miniin.setInternalReference(((*ie).getJacobianRef()).getMaximal());
      }
      else{
        miniin.setExternalReference((*ie).getOriginalRef());
      }
      //add second minimal vertex: the edge being eliminated
      xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionVertex& miniout = ((*theExpression).myExpression).addVertex();    
      if(theEdge.getRefType() == LinearizedComputationalGraphCopyEdge::TO_INTERNAL_EXPRESSION){
        miniout.setInternalReference((theEdge.getJacobianRef()).getMaximal());
      }// end if
      else{
        miniout.setExternalReference(theEdge.getOriginalRef());
      }// end else
      //create "mult" vertex and connect it to miniin and miniout
      xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionVertex& mult = ((*theExpression).myExpression).addVertex();
      mult.setOperation(xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionVertex::MULT_OP);
      ((*theExpression).myExpression).addEdge(miniin, mult);
      ((*theExpression).myExpression).addEdge(miniout, mult);
      
      //iterator used to check for a direct edge from source to target
      LinearizedComputationalGraphCopy::OutEdgeIteratorPair srcoutedges (getOutEdgesOf(getSourceOf(*ie)));
      LinearizedComputationalGraphCopy::OutEdgeIterator srcoe = srcoutedges.first, srcoend = srcoutedges.second;
      LinearizedComputationalGraphCopyEdge* directEdge_pt = NULL;
      for(; srcoe != srcoend; ++srcoe){
	//if there already was a direct edge from the source vertex to the target vertex, directEdge_pt is set to point to that edge
        if(&getTargetOf(*srcoe) == &getTargetOf(theEdge)){
          directEdge_pt = &*srcoe;
          break;
        } //endif
      }//end while

      LinearizedComputationalGraphCopyEdge& theNewEdge = addEdge(getSourceOf(*ie), getTargetOf(theEdge));

      theNewEdge.setJacobianRef(theExpression);
      if(directEdge_pt != NULL){        //create vertex for direct edge, and "add" vertex, point new edge to it
        xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionVertex& add = ((*theExpression).myExpression).addVertex();
        add.setOperation(xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionVertex::ADD_OP);
        //add vertex for direct edge
        xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionVertex& minidirect = ((*theExpression).myExpression).addVertex();
        if((*directEdge_pt).getRefType() == LinearizedComputationalGraphCopyEdge::TO_INTERNAL_EXPRESSION){
          minidirect.setInternalReference(((*directEdge_pt).getJacobianRef()).getMaximal());
        }
        else{
          minidirect.setExternalReference((*directEdge_pt).getOriginalRef());
        }
	//connect "add" vertex to minidirect and mult
        ((*theExpression).myExpression).addEdge(mult, add);
        ((*theExpression).myExpression).addEdge(minidirect, add);
        removeAndDeleteEdge(*directEdge_pt);
        (*theExpression).setMaximal(add);
      }// end if
      else{      //point the new edge to "mult"
        (*theExpression).setMaximal(mult);	  
      }// end else
    }// end inedge iteration

    if(numOutEdgesOf(getSourceOf(theEdge)) == 1){
      const LinearizedComputationalGraphCopy::ConstVertexPointerList& copyDependents = getDependentList();
      // if the now isolated vertex is not a dependent
      if(find(copyDependents.begin(), copyDependents.end(), &getSourceOf(theEdge)) == copyDependents.end()){
	removeFromVertexList(getSourceOf(theEdge));
	removeAndDeleteVertex(getSourceOf(theEdge));
      }// end if
      // is the source of the edge is a dependent vertex
      else{
	removeAndDeleteEdge(theEdge);
      }// end else

    }// end if
    // if the vertex is not now isolated
    else{
      removeAndDeleteEdge(theEdge);
    }// end else

  }// end back_elim_edge

  
  void LinearizedComputationalGraphCopy::elim_vertex(xaifBoosterCrossCountryInterface::LinearizedComputationalGraphVertex& theVertex,
						     xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionList& theJacobianAccumulationExpressionList){
    
    //iterator over inedges of theVertex
    LinearizedComputationalGraphCopy::InEdgeIteratorPair inedges (getInEdgesOf(theVertex));
    LinearizedComputationalGraphCopy::InEdgeIterator ie=inedges.first, iend=inedges.second;
    //iterator over outedges of theVertex
    LinearizedComputationalGraphCopy::OutEdgeIteratorPair outedges (getOutEdgesOf(theVertex));
    
    for(; ie != iend; ++ie){
      LinearizedComputationalGraphCopy::OutEdgeIterator oe=outedges.first, oend=outedges.second;
      for(; oe != oend; ++oe){

	opsum++;

	JacobianAccumulationExpressionCopy* theExpression = new JacobianAccumulationExpressionCopy(theJacobianAccumulationExpressionList.addExpression());

	//add first minimal vertex: edge into theVertex
       	xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionVertex& miniin = ((*theExpression).myExpression).addVertex();
	//if the edge points to a JAE rather than an original vertex, the new vertex is set to point to the maximal vertex of the JAE
	//that the edge points to
	if((*ie).getRefType() == LinearizedComputationalGraphCopyEdge::TO_INTERNAL_EXPRESSION){
	  miniin.setInternalReference(((*ie).getJacobianRef()).getMaximal());
	}
	//if the edge points to an original vertex, it is set as an external ref, and points to the original edge that the copy edge pointed to
	else{
	  miniin.setExternalReference((*ie).getOriginalRef());
	}
	//add second minimal vertex: edge out of theVertex
	xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionVertex& miniout = ((*theExpression).myExpression).addVertex();    
	if((*oe).getRefType() == LinearizedComputationalGraphCopyEdge::TO_INTERNAL_EXPRESSION){
	  miniout.setInternalReference(((*oe).getJacobianRef()).getMaximal());
	}// end if
	else{
	  miniout.setExternalReference((*oe).getOriginalRef());
	}// end else
	//create "mult" vertex and connect it to miniin and miniout
	xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionVertex& mult = ((*theExpression).myExpression).addVertex();
	mult.setOperation(xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionVertex::MULT_OP);
	((*theExpression).myExpression).addEdge(miniin, mult);
	((*theExpression).myExpression).addEdge(miniout, mult);
	
	//iterator used to check for a direct edge from source to target
	LinearizedComputationalGraphCopy::OutEdgeIteratorPair srcoutedges (getOutEdgesOf(getSourceOf(*ie)));
	LinearizedComputationalGraphCopy::OutEdgeIterator srcoe = srcoutedges.first, srcoend = srcoutedges.second;
	LinearizedComputationalGraphCopyEdge* directEdge_pt = NULL;
	for(; srcoe != srcoend; ++srcoe){
	  //if there already was a direct edge from the source vertex to the target vertex, directEdge_pt is set to point to that edge
	  if(&getTargetOf(*srcoe) == &getTargetOf(*oe)){
	    directEdge_pt = &*srcoe;
	    break;
	  } //endif
	}//end while

	LinearizedComputationalGraphCopyEdge& theNewEdge = addEdge(getSourceOf(*ie), getTargetOf(*oe));
	theNewEdge.setJacobianRef(theExpression);
	if(directEdge_pt != NULL){        //create vertex for direct edge, and "add" vertex, point new edge to it
	  xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionVertex& add = ((*theExpression).myExpression).addVertex();
	  add.setOperation(xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionVertex::ADD_OP);
	  //add vertex for direct edge
	  xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionVertex& minidirect = ((*theExpression).myExpression).addVertex();
	  if((*directEdge_pt).getRefType() == LinearizedComputationalGraphCopyEdge::TO_INTERNAL_EXPRESSION){
	    minidirect.setInternalReference(((*directEdge_pt).getJacobianRef()).getMaximal());
	  }
	  else{
	    minidirect.setExternalReference((*directEdge_pt).getOriginalRef());
	  }
	  //connect "add" vertex to minidirect and mult
	  ((*theExpression).myExpression).addEdge(mult, add);
	  ((*theExpression).myExpression).addEdge(minidirect, add);
	  removeAndDeleteEdge(*directEdge_pt);
	  (*theExpression).setMaximal(add);
	}// end if
	else{      //point the new edge to "mult"
	  (*theExpression).setMaximal(mult);	  
	}// end else
      }// end outedge iteration
    }// end inedge iteration

    removeFromVertexList(theVertex);
    removeAndDeleteVertex(theVertex);
  }// end elim_vertex

  const LinearizedComputationalGraphCopy::ConstVertexPointerList& LinearizedComputationalGraphCopy::getIndependentList() const{ 
    return myIndependentList;
  }
  
  const LinearizedComputationalGraphCopy::ConstVertexPointerList& LinearizedComputationalGraphCopy::getDependentList() const{
    return myDependentList;
  }

  bool LinearizedComputationalGraphCopy::isDep(xaifBoosterCrossCountryInterface::LinearizedComputationalGraphVertex& theVertex) const {

    bool isdep = false;

    const ConstVertexPointerList& depList = getDependentList();
    ConstVertexPointerList::const_iterator depCheck;
    for(depCheck = depList.begin(); depCheck != depList.end(); depCheck++){
      if(*depCheck == &theVertex){
	isdep = true;
	break;
      }// end if
    }// end for

    return isdep;
  }

  const unsigned int LinearizedComputationalGraphCopy::numIndeps() const{
    return myIndependentList.size();
  }

  const unsigned int LinearizedComputationalGraphCopy::numDeps() const{
    return myDependentList.size();
  }

  LinearizedComputationalGraphCopy::VertexPointerList LinearizedComputationalGraphCopy::getVertexList() const{
    return myVertexList;
  }

  void LinearizedComputationalGraphCopy::addToIndependentList(const xaifBoosterCrossCountryInterface::LinearizedComputationalGraphVertex& theIndepdentVertex){ 
    myIndependentList.push_back(&theIndepdentVertex);
  } 

  void LinearizedComputationalGraphCopy::addToDependentList(const xaifBoosterCrossCountryInterface::LinearizedComputationalGraphVertex& theDepdentVertex){ 
    myDependentList.push_back(&theDepdentVertex);
  }

  void LinearizedComputationalGraphCopy::addToVertexList(xaifBoosterCrossCountryInterface::LinearizedComputationalGraphVertex& theVertex){ 
    myVertexList.push_back(&theVertex);
  }

  void LinearizedComputationalGraphCopy::removeFromVertexList(xaifBoosterCrossCountryInterface::LinearizedComputationalGraphVertex& theVertex){ 
    myVertexList.remove(&theVertex);
  }

  LinearizedComputationalGraphCopy::EdgePointerList LinearizedComputationalGraphCopy::populateEdgeList() {

    EdgeIteratorPair edgelist (edges());
    EdgeIterator eli (edgelist.first), ele (edgelist.second);
    for(; eli != ele; ++eli){
      if((numOutEdgesOf(getTargetOf(*eli)) > 0) && (!isDep(getTargetOf(*eli)))){
	// if the target of the edge is not a dependent and has inedges

	edgeEntry newFrontElim;
	newFrontElim.edge_p = &*eli;
	newFrontElim.direction = FRONT;
	myEdgeList.push_front(newFrontElim);

      }// end if 
      //if the source has inedges
      if(numInEdgesOf(getSourceOf(*eli)) > 0){//can be back eliminated

	edgeEntry newBackElim;
	newBackElim.edge_p = &*eli;
	newBackElim.direction = BACK;
	myEdgeList.push_front(newBackElim);

      }// end if
    }// end for

    return myEdgeList;
  }// end populateEdgeList

  void LinearizedComputationalGraphCopy::clearEdgeList(){
    myEdgeList.clear();
  }// end clearEdgeList

}// end namespace MemOpsTradeoffPreaccumulation
