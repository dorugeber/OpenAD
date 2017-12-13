// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/utils/inc/PrintManager.hpp"
#include "xaifBooster/utils/inc/DbgLoggerManager.hpp"
#include "xaifBooster/system/inc/GraphVizDisplay.hpp"
#include "xaifBooster/algorithms/MemOpsTradeoffPreaccumulation/inc/FaceElim.hpp"

using namespace MemOpsTradeoffPreaccumulation;

namespace xaifBoosterMemOpsTradeoffPreaccumulation { 

  void FaceElim::absorbMode_f(DualGraph& theDual,
			      DualGraph::FacePointerList& theOldFaceList,
			      const DualGraph::VertexPointerList& thePredList,
			      const DualGraph::VertexPointerList& theSuccList,
			      DualGraphVertex* newOrAbsorb){

    DualGraph::FacePointerList theNewFaceList;

    //spath iterates through each path in the list
    DualGraph::PathList::const_iterator spath;
    //svertex iterates through each vertex in the path
    DualGraphPath::Path::iterator svertex;
    DualGraph::FacePointerList::iterator sface;

    for(sface = theOldFaceList.begin(); sface != theOldFaceList.end(); sface++){

      DualGraph::InEdgeIteratorPair die (theDual.getInEdgesOf(theDual.getSourceOf(**sface)));
      DualGraph::InEdgeIterator diei (die.first), die_end (die.second);
      DualGraph::OutEdgeIteratorPair doe (theDual.getOutEdgesOf(theDual.getTargetOf(**sface)));
      DualGraph::OutEdgeIterator doei (doe.first), doe_end (doe.second);
      bool loopcheck = true;

      for(; (diei != die_end) && loopcheck; ++diei){
	if(theDual.numOutEdgesOf(theDual.getSourceOf(*diei)) > 1){
	  for(; (doei != doe_end) && loopcheck; ++doei){
	    if(theDual.numInEdgesOf(theDual.getTargetOf(*doei)) > 1){
	      for(spath = (theDual.myPathList).begin(); (spath != (theDual.myPathList).end()) && loopcheck; spath++){
		for(svertex = ((**spath).myPath).begin(); svertex != ((**spath).myPath).end(); svertex++){
		  if(*svertex == &theDual.getSourceOf(*diei)){
		    //if path contains the vertex, go to next vertex in the path
		    svertex++;
		    //if the next vertex is the source of the face, go to the next path
		    if(*svertex == &theDual.getSourceOf(**sface)){break;}
		    svertex++;
		    if(*svertex == &theDual.getTargetOf(*doei)){
		      theNewFaceList.push_back(*sface);
		      loopcheck = false;
		      break;
		    }// end if succ matches
		    break;
		  }// end if vertex is the pred
		}// end for each vertex in the path
	      }// end for all paths
	    }// end if succ has alternate paths
	  }// end for all out edges
	}// end if pred has alternate path
      }// end for all inedges
    }// end for each face in the old list

    if(!theNewFaceList.empty()){
      theDual.absum++;
      theOldFaceList = theNewFaceList;
    }// end if

  }// end absorbMode_f

} // end of namespace
