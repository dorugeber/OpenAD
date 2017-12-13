// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/PrivateLinearizedComputationalGraphEdge.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/PrivateLinearizedComputationalGraph.hpp"

#include "xaifBooster/algorithms/CrossCountryInterface/inc/Elimination.hpp"

using namespace xaifBoosterBasicBlockPreaccumulation;

namespace xaifBoosterCrossCountryInterface {

  // defaults for command-line configurable settings
  AwarenessLevel::AwarenessLevel_E Elimination::ourAwarenessLevel = AwarenessLevel::UNIT_AWARENESS;

  bool Elimination::ourAllowMaintainingFlag = false;

  Elimination::Elimination (LinearizedComputationalGraph& lcg) : myType (UNSET_ELIMTYPE),
								 myLCG_p (&lcg),
								 myNumIterations (0),
								 myGamma (0) {
  }

  void Elimination::setAwarenessLevel(AwarenessLevel::AwarenessLevel_E anAwarenessLevel) {
    ourAwarenessLevel = anAwarenessLevel;
  }
  
  void Elimination::setAllowMaintainingFlag() {
    ourAllowMaintainingFlag = true;
  }

  void Elimination::initAsOperations() {
    myType = OPS_ELIMTYPE;
    myDescription = "default min. ops vertex / edge elimination";
  }

  void Elimination::initAsOperationsRandom() {
    myType = OPS_RANDOM_ELIMTYPE;
    myDescription = "random min. ops edge elimination";
  } // end Elimination::initAsOperationsRandom()

  void Elimination::initAsLSAVertex(int i, double g) {
    myType = OPS_LSA_VERTEX_ELIMTYPE;
    myDescription = "min. ops LSA vertex elimination";
    myNumIterations = i;
    myGamma = g;
  }

  void Elimination::initAsLSAFace(int i, double g) {
    myType = OPS_LSA_FACE_ELIMTYPE;
    myDescription = "min. ops LSA face elimination";
    myNumIterations = i;
    myGamma = g;
  }

  void Elimination::initAsScarceElimination() {
    myType = SCARCE_ELIMTYPE;
    myDescription = "scarcity edge elimination";
  }

  void Elimination::initAsScarceRandomElimination() {
    myType = SCARCE_RANDOM_ELIMTYPE;
    myDescription = "randomized scarcity edge elimination";
  } // end Elimination::initAsScarceRandomElimination()

  void Elimination::initAsScarceTransformation() {
    myType = SCARCE_TRANSFORMATIONTYPE;
    myDescription = "scarcity edge elimination and rerouting";
  }

  void Elimination::initAsScarceRandomTransformation() {
    myType = SCARCE_RANDOM_TRANSFORMATIONTYPE;
    myDescription = "randomized scarcity edge elimination and rerouting";
  } // end Elimination::initAsScarceRandomTransformation()

  std::string Elimination::getDescription() const {
    if (myType == UNSET_ELIMTYPE)
      THROW_LOGICEXCEPTION_MACRO("Elimination::getDescription() : cannot return description - Elimination never init'ed");
    return myDescription;
  }

  const LinearizedComputationalGraph&
  Elimination::getRemainderLCG() const {
    return myRemainderLCG;
  } // end Elimination::getRemainderLCG() const

  LinearizedComputationalGraph&
  Elimination::getRemainderLCG() {
    return myRemainderLCG;
  } // end Elimination::getRemainderLCG()

  const AccumulationGraph&
  Elimination::getAccumulationGraph() const {
    return myAccumulationGraph;
  } // end Elimination::getAccumulationGraph() const

  AccumulationGraph&
  Elimination::getAccumulationGraph() {
    return myAccumulationGraph;
  } // end Elimination::getAccumulationGraph()

  const Elimination::RemainderEdge2AccumulationVertexMap&
  Elimination::getRemainderEdge2AccumulationVertexMap() const {
    return myRemainderEdge2AccumulationVertexMap;
  } // end Elimination::getRemainderEdge2AccumulationVertex_map()

  const PreaccumulationCounter& Elimination::getCounter() const { 
    if (!myCountedFlag) { 
      countPreaccumulationOperations();
      countRemainderGraphEdges();
      if (myCounter.getNumMultiplications()+myCounter.getNumAdditions()+myCounter.getNumEdges()==0)
	THROW_LOGICEXCEPTION_MACRO("Elimination::EliminationResult::getCounter() nothing counted");
      myCountedFlag=true;
    }
    return myCounter; 
  }

  unsigned int Elimination::getNumReroutings() const {
    return myNumReroutings;
  } // end Elimination::getNumReroutings()

  const LinearizedComputationalGraphVertex&
  Elimination::rVertex2oVertex (const LinearizedComputationalGraphVertex& theRemainderVertex) const {
  VertexCorrelationList::const_iterator vCorrListI;
    for (vCorrListI = myVertexCorrelationList.begin(); vCorrListI != myVertexCorrelationList.end(); ++vCorrListI)
      if ((*vCorrListI).myRemainderVertex_p == &theRemainderVertex) break;
    if (vCorrListI == myVertexCorrelationList.end()) { 
      THROW_LOGICEXCEPTION_MACRO("Elimination::rVertex2oVertex: correlation could not be found for remainder graph vertex");
    }
    return *(vCorrListI->myOriginalVertex_p);
  }

  void
  Elimination::buildAccumulationGraph() {
    myAccumulationGraph.clear();

    LCGe_to_ACCv_map theLCGe_to_ACCv_map;

    // First iterate through the original edges and create corresponding accumulation graph vertices 
    const PrivateLinearizedComputationalGraph& theOriginalPLCG (dynamic_cast<const PrivateLinearizedComputationalGraph&>(*myLCG_p));
    PrivateLinearizedComputationalGraph::ConstEdgeIteratorPair aPLCGeIPair (theOriginalPLCG.edges());
    for (PrivateLinearizedComputationalGraph::ConstEdgeIterator PLCGeI (aPLCGeIPair.first), PLCGeIEnd (aPLCGeIPair.second);
	 PLCGeI != PLCGeIEnd; ++PLCGeI) {
      const PrivateLinearizedComputationalGraphEdge& currentPLCGEdge (dynamic_cast<const PrivateLinearizedComputationalGraphEdge&>(*PLCGeI));
      // handle regular edge (no parallels)
      if (!currentPLCGEdge.getParallels().size()) {
	AccumulationGraphVertex* theNewAccVertex_p (NULL);
	if (currentPLCGEdge.isDirectCopyEdge())
	  theNewAccVertex_p = new AccumulationGraphVertex;
	else
	  theNewAccVertex_p = new AccumulationGraphVertex (currentPLCGEdge.getLinearizedExpressionEdge());
	// add the new AccVertex to the graph and map this PLCG edge to it
	myAccumulationGraph.supplyAndAddVertexInstance(*theNewAccVertex_p);
	theLCGe_to_ACCv_map.addElement(&*PLCGeI, theNewAccVertex_p);
      } // end regular edge (no parallels)
      // handle edge with parallels
      else {
	//std::cout << "There are parallel edges!" << std::endl;
	// deal with the initial edge
	AccumulationGraphVertex* theNewAccVertex_p = new AccumulationGraphVertex (currentPLCGEdge.getLinearizedExpressionEdge());
	myAccumulationGraph.supplyAndAddVertexInstance(*theNewAccVertex_p);
	// create the add vertex and connect the initial vertex to it.  also map current PLCGedge to the add vertex
	AccumulationGraphVertex* theNewAccAddVertex_p = new AccumulationGraphVertex(JacobianAccumulationExpressionVertex::ADD_OP);
	myAccumulationGraph.supplyAndAddVertexInstance(*theNewAccAddVertex_p);
	myAccumulationGraph.addEdge(*theNewAccVertex_p, *theNewAccAddVertex_p);
	theLCGe_to_ACCv_map.addElement(&*PLCGeI, theNewAccAddVertex_p);
	// iterate over parallels: create the new vertex for the parallel edge and connect it up
	for (PrivateLinearizedComputationalGraphEdge::ExpressionEdgePList::const_iterator parallelI = currentPLCGEdge.getParallels().begin();
	     parallelI != currentPLCGEdge.getParallels().end(); ++parallelI) {
	  AccumulationGraphVertex* theNewParallelAccVertex_p = new AccumulationGraphVertex (**parallelI);
	  myAccumulationGraph.supplyAndAddVertexInstance(*theNewParallelAccVertex_p);
	  myAccumulationGraph.addEdge(*theNewParallelAccVertex_p, *theNewAccAddVertex_p);
	} // end iterate over parallel edges
      } // end handle edge with parallels
    } // end all edges in theOriginalPLCG

    // used for references that occur across different JAEs and for resolving remainder graph references
    JAEv_to_ACCv_map theInterJAEvertexMap;

    // iterate over all JAEs
    for(JacobianAccumulationExpressionList::GraphList::const_iterator JAEit = myJAEList.getGraphList().begin();
	JAEit != myJAEList.getGraphList().end(); ++JAEit) {
      const JacobianAccumulationExpression& theCurrentJAE (*(*JAEit));
      JAEv_to_ACCv_map theIntraJAEvertexMap; // used for creating the edges that occur within this JAE

      // iterate over all vertices in this JAE
      JacobianAccumulationExpression::ConstVertexIteratorPair aJAEvPair (theCurrentJAE.vertices());
      for (JacobianAccumulationExpression::ConstVertexIterator aJAEvI(aJAEvPair.first), aJAEvIEnd(aJAEvPair.second);
	   aJAEvI != aJAEvIEnd; ++aJAEvI) {
	const JacobianAccumulationExpressionVertex& theCurrentJAEVertex (*aJAEvI);

	switch (theCurrentJAEVertex.getReferenceType()) {
	  case JacobianAccumulationExpressionVertex::INTERNAL_REF: {
	    // find the source vertex using theInterJAEvertexMap
	    if (!theInterJAEvertexMap.hasElement(&theCurrentJAEVertex.getInternalReference()))
	      THROW_LOGICEXCEPTION_MACRO("Elimination::buildAccumulationGraph: could not find AccumulationGraphVertex for internal JAE vertex reference in theInterJAEvertexMap");
	    // simply map to the corresponding vertex.  No need to create a new one...
	    theIntraJAEvertexMap.addElement(&theCurrentJAEVertex,theInterJAEvertexMap.getElement(&theCurrentJAEVertex.getInternalReference()));
	    break;
	  } // end INTERNAL_REF
	  case xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionVertex::EXTERNAL_REF: {
	    // find the corresponding AccVertex (it must have been created in the first step
	    if (!theLCGe_to_ACCv_map.hasElement(&theCurrentJAEVertex.getExternalReference()))
	      THROW_LOGICEXCEPTION_MACRO("Elimination::buildAccumulationGraph: could not find AccumulationGraphVertex for PLCG edge referred to by internal JAE vertex reference");
	    theIntraJAEvertexMap.addElement(&theCurrentJAEVertex,theLCGe_to_ACCv_map.getElement(&theCurrentJAEVertex.getExternalReference()));
	    // handle maximal JAE vertex
	    if (!theCurrentJAE.numOutEdgesOf(theCurrentJAEVertex))
	      theInterJAEvertexMap.addElement(&theCurrentJAEVertex,theLCGe_to_ACCv_map.getElement(&theCurrentJAEVertex.getExternalReference()));
	    break;
	  } // end EXTERNAL_REF
	  case JacobianAccumulationExpressionVertex::OPERATION: {
	    AccumulationGraphVertex* theNewAccVertex_p = new AccumulationGraphVertex (theCurrentJAEVertex.getOperation());
	    // create the new AccumulationGraphVertex and add it to the map
	    myAccumulationGraph.supplyAndAddVertexInstance(*theNewAccVertex_p);
	    theIntraJAEvertexMap.addElement(&theCurrentJAEVertex, theNewAccVertex_p);
	    // add to the inter map if this is the maximal vertex
	    if (!theCurrentJAE.numOutEdgesOf(theCurrentJAEVertex))
	      theInterJAEvertexMap.addElement(&theCurrentJAEVertex, theNewAccVertex_p);
	    break;
	  } // end OPERATION
	  default :
	    THROW_LOGICEXCEPTION_MACRO("Elimination::buildAccumulationGraph: unknown JAE vertex type");
	    break;
	} // end switch on (*aJAEvI).getReferenceType()
      } // end iterate over JAE vertices

      // create edges resulting from connections within this JAE (INTRA)
      JacobianAccumulationExpression::ConstEdgeIteratorPair aJAEePair (theCurrentJAE.edges());
      for (JacobianAccumulationExpression::ConstEdgeIterator aJAEeI(aJAEePair.first), aJAEeIEnd(aJAEePair.second);
	   aJAEeI != aJAEeIEnd; ++aJAEeI) {
	// find the source and target of the new edge from theIntraJAEvertexMap
	if (!theIntraJAEvertexMap.hasElement(&theCurrentJAE.getSourceOf(*aJAEeI)))
	  THROW_LOGICEXCEPTION_MACRO("Elimination::buildAccumulationGraph: could not find AccumulationGraphVertex for internal JAE vertex reference in theInterJAEvertexMap");
	const AccumulationGraphVertex& theSourceAccVertex (*theIntraJAEvertexMap.getElement(&theCurrentJAE.getSourceOf(*aJAEeI)));
	if (!theIntraJAEvertexMap.hasElement(&theCurrentJAE.getTargetOf(*aJAEeI)))
	  THROW_LOGICEXCEPTION_MACRO("Elimination::buildAccumulationGraph: could not find AccumulationGraphVertex for internal JAE vertex reference in theInterJAEvertexMap");
	const AccumulationGraphVertex& theTargetAccVertex (*theIntraJAEvertexMap.getElement(&theCurrentJAE.getTargetOf(*aJAEeI)));
	// create the edge in the accumulation graph
	myAccumulationGraph.addEdge(theSourceAccVertex, theTargetAccVertex);
      } // end iterate over JAE edges

    } // end iterate over JAEs

    // Populate remainder graph map
    for (EdgeCorrelationList::const_iterator ecI = myEdgeCorrelationList.begin(); ecI != myEdgeCorrelationList.end(); ecI++) {
      AccumulationGraphVertex* theAccVertex_p (NULL);
      if ((*ecI).myType == EdgeCorrelationEntry::LCG_EDGE) {
	// find the corresponding AccVertex in thePLCGe_to_ACCv_map
	if (!theLCGe_to_ACCv_map.hasElement((*ecI).myEliminationReference.myOriginalEdge_p))
	  THROW_LOGICEXCEPTION_MACRO("Elimination::buildAccumulationGraph: could not find AccumulationGraphVertex for external JAE vertex reference (to original PLCG edge) in thePLCGe_to_ACCv_map");
	theAccVertex_p = theLCGe_to_ACCv_map.getElement((*ecI).myEliminationReference.myOriginalEdge_p);
      } // end LCG_EDGE ref
      else if ((*ecI).myType == EdgeCorrelationEntry::JAE_VERT) {
	// find the corresponding AccVertex in theInterJAEvertexMap
	if (!theInterJAEvertexMap.hasElement((*ecI).myEliminationReference.myJAEVertex_p))
	  THROW_LOGICEXCEPTION_MACRO("Elimination::buildAccumulationGraph: could not find AccumulationGraphVertex for internal JAE vertex reference in theInterJAEvertexMap");
	theAccVertex_p = theInterJAEvertexMap.getElement((*ecI).myEliminationReference.myJAEVertex_p);
      } // end JAE_VERT ref
      else THROW_LOGICEXCEPTION_MACRO("Elimination::buildAccumulationGraph: EdgeCorrelationEntry has invalid type");

      myRemainderEdge2AccumulationVertexMap.addElement((*ecI).myRemainderGraphEdge_p, theAccVertex_p);
      theAccVertex_p->setRemainderGraphEdge(*(*ecI).myRemainderGraphEdge_p);
    } // end iterate over myEdgeCorrelationList

  } // end Elimination::buildAccumulationGraph()

  void
  Elimination::evaluateCostRecursively(const AccumulationGraphVertex& theAccVertex,
                                       AccVertexIsUnitMap& theAccVertexIsUnitMap) const {
    if (theAccVertexIsUnitMap.find(&theAccVertex) != theAccVertexIsUnitMap.end())
      THROW_LOGICEXCEPTION_MACRO("Elimination::EliminationResult::evaluateCostRecursively(): re-called on an AccVertex that has already been visited");

    // LEAVES: mark them if unit, then return
    if (myAccumulationGraph.numInEdgesOf(theAccVertex) == 0) {
      if (theAccVertex.getPartialDerivativeKind() == PartialDerivativeKind::LINEAR_ONE
       || theAccVertex.getPartialDerivativeKind() == PartialDerivativeKind::LINEAR_MINUS_ONE)
        theAccVertexIsUnitMap[&theAccVertex] = true;
      else
        theAccVertexIsUnitMap[&theAccVertex] = false;
      return;
    }

    // visit unvisited children recursively
    bool allChildrenUnit = true;
    bool hasUnitChild = false;
    AccumulationGraph::ConstInEdgeIteratorPair iei_pair (myAccumulationGraph.getInEdgesOf(theAccVertex));
    for (AccumulationGraph::ConstInEdgeIterator iei (iei_pair.first), ie_end (iei_pair.second); iei != ie_end; ++iei) {
      const AccumulationGraphVertex& theChildAccVertex (myAccumulationGraph.getSourceOf(*iei));
      // visit the child if it isn't already visited
      if (theAccVertexIsUnitMap.find(&theChildAccVertex) == theAccVertexIsUnitMap.end())
        evaluateCostRecursively(theChildAccVertex, theAccVertexIsUnitMap);
      if (theAccVertexIsUnitMap[&theChildAccVertex])
        hasUnitChild = true;
      else
        allChildrenUnit = false;
    } // end iterate over children

    // at this point we know all children are visited
    switch (theAccVertex.getOperation()) {
      case JacobianAccumulationExpressionVertex::ADD_OP: {
        // no addition can result in a unit value
        theAccVertexIsUnitMap[&theAccVertex] = false;
        // any add will always cost us
	myCounter.numAdditionsInc();
        break;
      } // end ADD_OP
      case JacobianAccumulationExpressionVertex::MULT_OP: {
        // only unit if all children unit
        theAccVertexIsUnitMap[&theAccVertex] = allChildrenUnit;
        // only increment if there's a unit child
        if (!hasUnitChild)
          myCounter.numMultiplicationsInc();
        break;
      } // end MULT_OP
//      case JacobianAccumulationExpressionVertex::DIV_OP: {
//        break;
//      } // end DIV_OP
//      case JacobianAccumulationExpressionVertex::SUB_OP: {
//        break;
//      } // end SUB_OP
      default: {
        THROW_LOGICEXCEPTION_MACRO("Elimination::EliminationResult::evaluateCostRecursively() : unknown operation type for non-leaf AccVertex");
	break;
      } // end default
    } // end switch (theAccVertex.getOperation())
  } // end Elimination::evaluateCostRecursively()

  void Elimination::countPreaccumulationOperations() const {
    AccVertexIsUnitMap theAccVertexIsUnitMap;
    // visit each maximal AccumulationGraphVertex recursively
    AccumulationGraph::ConstVertexIteratorPair aAccVitPair (myAccumulationGraph.vertices());
    for (AccumulationGraph::ConstVertexIterator aAccVit (aAccVitPair.first), aAccVitEnd (aAccVitPair.second); aAccVit != aAccVitEnd; ++aAccVit)
      if (myAccumulationGraph.numOutEdgesOf(*aAccVit) == 0)
        evaluateCostRecursively(*aAccVit, theAccVertexIsUnitMap);
  } // end Elimination::countPreaccumulationOperations()

  void Elimination::countRemainderGraphEdges() const {
    if (myRemainderLCG.numEdges() == 0)
	THROW_LOGICEXCEPTION_MACRO("Elimination::EliminationResult::countRemainderGraphEdges(): The remainder graph is empty!");

    xaifBoosterCrossCountryInterface::LinearizedComputationalGraph::ConstEdgeIteratorPair anLCGeIPair (myRemainderLCG.edges());
    for (xaifBoosterCrossCountryInterface::LinearizedComputationalGraph::ConstEdgeIterator LCGeI (anLCGeIPair.first), LCGeIEnd (anLCGeIPair.second);
         LCGeI != LCGeIEnd;
         ++LCGeI) {
      myCounter.numEdgesInc();
      if ((*LCGeI).getEdgeLabelType() == xaifBoosterCrossCountryInterface::LinearizedComputationalGraphEdge::VARIABLE_LABEL)
        myCounter.numNonconstantEdgesInc();
      if ((*LCGeI).getEdgeLabelType() != xaifBoosterCrossCountryInterface::LinearizedComputationalGraphEdge::UNIT_LABEL)
        myCounter.numNonunitEdgesInc();
    } // end iterate over edges
  } // end Elimination::countRemainderGraphEdges()

} // end of namespace xaifBoosterCrossCountryInterface
