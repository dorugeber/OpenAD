// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/GraphVizDisplay.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/PrivateLinearizedComputationalGraph.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/PrivateLinearizedComputationalGraphVertex.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/PrivateLinearizedComputationalGraphEdge.hpp"

using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulation {  

  PrivateLinearizedComputationalGraph::PrivateLinearizedComputationalGraph() { 
  } 

  void
  PrivateLinearizedComputationalGraph::show(const std::string& outputName) const {
    if (numVertices() > 0)
      GraphVizDisplay::show(*this,
                            outputName,
                            PrivateLinearizedComputationalGraphVertexLabelWriter(*this),
                            PrivateLinearizedComputationalGraphEdgeLabelWriter(*this),
                            PrivateLinearizedComputationalGraphPropertiesWriter(*this));
  }

  void
  PrivateLinearizedComputationalGraph::mapAssignmentLHS2PLCGV(const Assignment& anAssignment,
                                                              const PrivateLinearizedComputationalGraphVertex& thePLCGV) {
    if (myAssignmentLHS2LCGVertexPMap.hasElement(&anAssignment))
      THROW_LOGICEXCEPTION_MACRO("PrivateLinearizedComputationalGraph::mapAssignmentLHS2PLCGV: already mapping " << anAssignment.debug().c_str());
    myAssignmentLHS2LCGVertexPMap.addElement(&anAssignment,&thePLCGV);
  }

  const PrivateLinearizedComputationalGraphVertex&
  PrivateLinearizedComputationalGraph::getLCGVertexForAssignmentLHS(const Assignment& anAssignment) const {
    if (!myAssignmentLHS2LCGVertexPMap.hasElement(&anAssignment))
      THROW_LOGICEXCEPTION_MACRO("PrivateLinearizedComputationalGraph::mapAssignmentLHS2PLCGV: no map entry for " << anAssignment.debug().c_str());
    return *myAssignmentLHS2LCGVertexPMap.getElement(&anAssignment);
  }

  void PrivateLinearizedComputationalGraph::addToIndependentList(const xaifBoosterCrossCountryInterface::LinearizedComputationalGraphVertex& theIndependentVertex) { 
    myIndependentList.push_back(&theIndependentVertex);
  } 

  void PrivateLinearizedComputationalGraph::addToDependentList(const xaifBoosterCrossCountryInterface::LinearizedComputationalGraphVertex& theDependentVertex,
							       const ObjectWithId::Id& aStatementId) { 
    myDependentList.push_back(&theDependentVertex);
    myDependentStatementIdList.push_back(aStatementId);
    myStatementIdList.push_back(aStatementId);
  } 

  void PrivateLinearizedComputationalGraph::removeFromIndependentList(const xaifBoosterCrossCountryInterface::LinearizedComputationalGraphVertex& theIndependentVertex) { 
    VertexPointerList::iterator aVertexPointerListI=myIndependentList.begin();
    for (;
	 aVertexPointerListI!=myIndependentList.end();
	 ++aVertexPointerListI) { 
      if ((*aVertexPointerListI)==&theIndependentVertex) { 
	myIndependentList.erase(aVertexPointerListI);
	break;
      }
    }
    if (aVertexPointerListI==myIndependentList.end()) { 
      THROW_LOGICEXCEPTION_MACRO("PrivateLinearizedComputationalGraph::removeFromIndependentList: attempt to remove a vertex that is not in the list");
    }
  } 

  void PrivateLinearizedComputationalGraph::removeFromDependentList(const xaifBoosterCrossCountryInterface::LinearizedComputationalGraphVertex& theDependentVertex) {
    VertexPointerList::iterator aVertexPointerListI=myDependentList.begin();
    StatementIdList::iterator aStatementIdListI=myDependentStatementIdList.begin();
      for (;
	   aVertexPointerListI!=myDependentList.end();
	   ++aStatementIdListI,++aVertexPointerListI) { 
	     if ((*aVertexPointerListI)==&theDependentVertex) { 
	       myDependentList.erase(aVertexPointerListI);
	       myDependentStatementIdList.erase(aStatementIdListI);
	       break;
	     }
	   }
    if (aVertexPointerListI==myDependentList.end()) { 
      THROW_LOGICEXCEPTION_MACRO("PrivateLinearizedComputationalGraph::removeFromDependentList: attempt to remove a vertex that is not in the list");
    }
  }

  const DuUdMapUseResult::StatementIdLists PrivateLinearizedComputationalGraph::getStatementIdLists()const { 
    return DuUdMapUseResult::StatementIdLists(myDependentStatementIdList, 
					      myPassiveStatementIdList,
					      myStatementIdList);
  }

  void PrivateLinearizedComputationalGraph::addToPassiveStatementIdList(const ObjectWithId::Id& aStatementId) { 
    myPassiveStatementIdList.push_back(aStatementId);
    myStatementIdList.push_back(aStatementId);
  } 

} // end of namespace 
