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

#include "xaifBooster/system/inc/Expression.hpp"
#include "xaifBooster/system/inc/Scope.hpp"
#include "xaifBooster/system/inc/ConceptuallyStaticInstances.hpp"
#include "xaifBooster/system/inc/CallGraph.hpp"
#include "xaifBooster/system/inc/Intrinsic.hpp"
#include "xaifBooster/system/inc/InlinableIntrinsicsCatalogueItem.hpp"
#include "xaifBooster/system/inc/VariableSymbolReference.hpp"

#include "xaifBooster/algorithms/TypeChange/inc/TemporariesHelper.hpp"

#include "xaifBooster/algorithms/Linearization/inc/ExpressionAlg.hpp"
#include "xaifBooster/algorithms/Linearization/inc/ExpressionVertexAlg.hpp"
#include "xaifBooster/algorithms/Linearization/inc/ExpressionEdgeAlg.hpp"

namespace xaifBoosterLinearization{

  ExpressionAlg::ExpressionAlg(Expression& theContainingExpression):
   ExpressionAlgBase(theContainingExpression),
   myAuxiliaryAssignmentCounter(0) { 
  }

  std::string ExpressionAlg::debug() const{
    std::ostringstream out;
    out << "xaifBoosterLinearization::ExpressionAlg[" << ExpressionAlgBase::debug().c_str()
        << ",needsAuxiliaryExtraction=" << needsAuxiliaryExtraction()
        << "]" << std::ends;
    return out.str();
  } // end of ExpressionAlg::debug

  void
  ExpressionAlg::printXMLHierarchy(std::ostream& os) const{
    getContaining().printXMLHierarchyImpl(os);
  }

  void ExpressionAlg::traverseToChildren(const GenericAction::GenericAction_E anAction_c){
  }

  bool
  ExpressionAlg::needsAuxiliaryExtraction() const {
    Expression::ConstVertexIteratorPair vip(getContaining().vertices());
    for(Expression::ConstVertexIterator aEVi(vip.first); aEVi != vip.second; ++aEVi)
      if (dynamic_cast<const ExpressionVertexAlg&>((*aEVi).getExpressionVertexAlgBase()).hasAuxiliaryVariable())
        return true;
    return false;
  }

  void
  ExpressionAlg::extractAuxiliary(const ExpressionVertex& theExpressionVertex) {
    ExpressionVertexAlg& theEVAlg(
     dynamic_cast<ExpressionVertexAlg&>(theExpressionVertex.getExpressionVertexAlgBase())
    );
    if (!theEVAlg.hasAuxiliaryVariable()) {
      Scope& theCurrentCfgScope(
       ConceptuallyStaticInstances::instance()->getTraversalStack().getCurrentCallGraphVertexInstance().getControlFlowGraph().getScope()
      );
      xaifBoosterTypeChange::TemporariesHelper aHelper("xaifBoosterLinearization::ExpressionAlg::extractAuxiliary",
                                                       getContaining(),
                                                       theExpressionVertex);
      theEVAlg.makeAuxiliaryVariable(aHelper.makeTempSymbol(theCurrentCfgScope,
                                                             ConceptuallyStaticInstances::instance()->getAuxiliaryVariableNameCreator(),
                                                             false),
                                      theCurrentCfgScope);
      if (aHelper.needsAllocation())
        theEVAlg.makeAuxiliaryAllocation(aHelper);
    }
  }

  void
  ExpressionAlg::printAuxiliaryAssignmentsSSA(const ObjectWithId::Id& theTopLevelAssignmentId,
                                              std::ostream& os) const {
    myAuxiliaryAssignmentCounter = 0;
    printAuxiliaryAssignmentsSSARecursively(getContaining().getMaxVertex(),
                                            theTopLevelAssignmentId,
                                            os);
  }

  void
  ExpressionAlg::printAuxiliaryAssignmentsSSARecursively(const ExpressionVertex& theEV,
                                                         const ObjectWithId::Id& theTopLevelAssignmentId,
                                                         std::ostream& os) const {
    // recursive descent (postorder traversal)
    Expression::ConstInEdgeIteratorPair iep(getContaining().getInEdgesOf(theEV));
    for (Expression::ConstInEdgeIterator aEEi(iep.first); aEEi != iep.second; ++aEEi)
      if (getContaining().numInEdgesOf(getContaining().getSourceOf(*aEEi)) > 0)
        printAuxiliaryAssignmentsSSARecursively(getContaining().getSourceOf(*aEEi),
                                                theTopLevelAssignmentId,
                                                os);
    const ExpressionVertexAlg& theEVAlg(dynamic_cast<const ExpressionVertexAlg&>(theEV.getExpressionVertexAlgBase()));
    if (theEVAlg.hasAuxiliaryVariable()) {
      Assignment theNewAuxiliaryAssignment(false);
      theNewAuxiliaryAssignment.setId(makeAuxiliaryAssignmentSSAId(theTopLevelAssignmentId));
      // set the LHS
      theEVAlg.getAuxiliaryVariable().copyMyselfInto(theNewAuxiliaryAssignment.getLHS());
      // populate the RHS
      ExpressionVertex& theNewMaxEV(theEV.createCopyOfMyself());
      theNewAuxiliaryAssignment.getRHS().supplyAndAddVertexInstance(theNewMaxEV);
      for (Expression::ConstInEdgeIterator aEEi(iep.first); aEEi != iep.second; ++aEEi)
        populateSSASubexpressionRecursively(*aEEi,
                                            theNewAuxiliaryAssignment.getRHS(),
                                            theNewMaxEV);
      if (theEVAlg.hasAuxiliaryAllocation())
        theEVAlg.getAuxiliaryAllocation().printXMLHierarchy(os);
      theNewAuxiliaryAssignment.printXMLHierarchy(os);
    } // end if aux variable
  }

  void
  ExpressionAlg::populateSSASubexpressionRecursively(const ExpressionEdge& theEE,
                                                     Expression& theExpressionToPopulate,
                                                     const ExpressionVertex& theNewParentEV) const {
    DBG_MACRO(DbgGroup::CALLSTACK,"xaifBoosterLinearization::ExpressionAlg::populateSSASubexpressionRecursively"
                               << " called for " << theEE.debug().c_str());
    const ExpressionVertex& theSourceEV(getContaining().getSourceOf(theEE));
    const ExpressionVertexAlg& theSourceEVAlg(dynamic_cast<const ExpressionVertexAlg&>(theSourceEV.getExpressionVertexAlgBase()));
    if (theSourceEVAlg.hasAuxiliaryVariable()) {
      Argument& theNewAuxiliaryArgument(*new Argument());
      theSourceEVAlg.copyAuxiliaryInto(theNewAuxiliaryArgument);
      // add the new aux argument to the extraction subexpression
      theExpressionToPopulate.supplyAndAddVertexInstance(theNewAuxiliaryArgument);
      theEE.copyMyselfInto(theExpressionToPopulate.addEdge(theNewAuxiliaryArgument,
                                                           theNewParentEV));
    }
    else {
      ExpressionVertex& theNewEV(theSourceEV.createCopyOfMyself());
      theExpressionToPopulate.supplyAndAddVertexInstance(theNewEV);
      theEE.copyMyselfInto(theExpressionToPopulate.addEdge(theNewEV,
                                                           theNewParentEV));
      Expression::ConstInEdgeIteratorPair iep(getContaining().getInEdgesOf(theSourceEV));
      for (Expression::ConstInEdgeIterator aEEi(iep.first); aEEi != iep.second; ++aEEi)
        populateSSASubexpressionRecursively(*aEEi,
                                            theExpressionToPopulate,
                                            theNewEV);
    }
  }

  void
  ExpressionAlg::printLocalPartialAssignments(std::ostream& os) const {
    if (dynamic_cast<const ExpressionVertexAlg&>(getContaining().getMaxVertex().getExpressionVertexAlgBase()).isActive()) {
      Expression::ConstInEdgeIteratorPair iep(getContaining().getInEdgesOf(getContaining().getMaxVertex()));
      for (Expression::ConstInEdgeIterator aEEi(iep.first); aEEi != iep.second; ++aEEi)
        if (dynamic_cast<const ExpressionVertexAlg&>(getContaining().getSourceOf(*aEEi).getExpressionVertexAlgBase()).isActive())
          printLocalPartialAssignmentsRecursively(*aEEi,
                                                  os);
    } // end if the max vertex is active
  }

  void
  ExpressionAlg::printLocalPartialAssignmentsRecursively(const ExpressionEdge& theExpressionEdge,
                                                         std::ostream& os) const {
    const ExpressionEdgeAlg& theEEAlg(dynamic_cast<const ExpressionEdgeAlg&>(theExpressionEdge.getExpressionEdgeAlgBase()));
    DBG_MACRO(DbgGroup::CALLSTACK,"xaifBoosterLinearization::ExpressionAlg::printLocalPartialAssignmentsRecursively:"
                               << " entered for " << theEEAlg.debug().c_str() 
                               << " in " << debug().c_str());
    // recurse
    Expression::ConstInEdgeIteratorPair iep(getContaining().getInEdgesOf(getContaining().getSourceOf(theExpressionEdge)));
    for (Expression::ConstInEdgeIterator aEEi(iep.first); aEEi != iep.second; ++aEEi)
      if (dynamic_cast<const ExpressionVertexAlg&>(getContaining().getSourceOf(*aEEi).getExpressionVertexAlgBase()).isActive())
        printLocalPartialAssignmentsRecursively(*aEEi,
                                                os);
    // print for this edge
    if (theEEAlg.hasConcretePartialAssignment()
     && theEEAlg.getPartialDerivativeKind()!=PartialDerivativeKind::PASSIVE) {
      DBG_MACRO(DbgGroup::CALLSTACK,"xaifBoosterLinearization::ExpressionAlg::printLocalPartialAssignmentsRecursively:"
                                 << " printing partial assignment for " << theExpressionEdge.debug().c_str() 
                                 << " in " << debug().c_str());
      if (theEEAlg.hasPartialAllocation())
        theEEAlg.getPartialAllocation().printXMLHierarchy(os);
      theEEAlg.getConcretePartialAssignment().printXMLHierarchy(os);
    }
  }

  void ExpressionAlg::activityAnalysis(){
    activityAnalysisBottomUpPass(getContaining().getMaxVertex());
    activityAnalysisTopDownPass(getContaining().getMaxVertex());
  } // end of  ExpressionAlg::activityAnalysis

  void ExpressionAlg::createPartialExpressions(){
    DBG_MACRO(DbgGroup::CALLSTACK, "ExpressionAlg::createPartialExpressions: for "
	      <<debug().c_str());
    Scope& theCurrentCfgScope (ConceptuallyStaticInstances::instance()->getTraversalStack().getCurrentCallGraphVertexInstance().getControlFlowGraph().getScope());
    Expression::ConstVertexIteratorPair p(getContaining().vertices());
    Expression::ConstVertexIterator anExpressionVertexIEnd(p.second);
    // outer loop over all vertices
    for(Expression::ConstVertexIterator anExpressionVertexI(p.first); anExpressionVertexI!=anExpressionVertexIEnd; ++anExpressionVertexI) {
      if(!getContaining().numInEdgesOf(*anExpressionVertexI)
	 ||
	 !dynamic_cast<ExpressionVertexAlg&>((*anExpressionVertexI).getExpressionVertexAlgBase()).isActive())
	// nothing to be done for leafs
	// or passive vertices
	continue;
      const InlinableIntrinsicsCatalogueItem&theCatalogueItem((*anExpressionVertexI).getInlinableIntrinsicsCatalogueItem());
      // find all Positions with active Arguments
      PositionSet theActivePositions;
      Expression::ConstInEdgeIteratorPair pE=getContaining().getInEdgesOf(*anExpressionVertexI);
      Expression::ConstInEdgeIterator anExpressionEdgeI(pE.first), anExpressionEdgeIEnd(pE.second);
      for(; anExpressionEdgeI!=anExpressionEdgeIEnd; ++anExpressionEdgeI) {
	if(dynamic_cast<ExpressionVertexAlg&>(getContaining().getSourceOf(*anExpressionEdgeI).
					getExpressionVertexAlgBase()).isActive())
	  theActivePositions.add((*anExpressionEdgeI).getPosition());
      }
      PositionSet theUsedPositions(theCatalogueItem.getUsedPositionalArguments(theActivePositions));
      DBG_MACRO(DbgGroup::DATA, "ExpressionAlg::createPartialExpressions: for " << (dynamic_cast<const Intrinsic&>(*anExpressionVertexI)).getName().c_str() << " all used positions: "
		<<theUsedPositions.debug().c_str());
      // the first one points to the function value, i.e. this vertex
      // however we may not have a special expression specified for 
      // the function
      if(theUsedPositions.has(0)) {
        extractAuxiliary(*anExpressionVertexI);
      } // end if
      // now we need to loop over all arguments to determine auxiliaries
      for(Expression::ConstInEdgeIterator anExpressionEdgeI2(pE.first); anExpressionEdgeI2!=anExpressionEdgeIEnd; ++anExpressionEdgeI2) {
	const ExpressionVertex&theSource(getContaining().getSourceOf(*anExpressionEdgeI2));
	if(theUsedPositions.has((*anExpressionEdgeI2).getPosition())
        && getContaining().numInEdgesOf(theSource)) {
          extractAuxiliary(theSource);
        } // end if  

	const InlinableIntrinsicsExpression&
		thePartialExpression(theCatalogueItem.getExpressionVectorElement((*anExpressionEdgeI2).getPosition()));
	ExpressionEdgeAlg&theI2EdgeAlg(dynamic_cast<ExpressionEdgeAlg&>((*anExpressionEdgeI2).getExpressionEdgeAlgBase()));
	// match the abstract arguments in thePartialExpression
	// with the concrete vertices in this Expression
	// by position
	const PositionSet&usedByThisPartial(thePartialExpression.getUsedPositionalArguments());
	DBG_MACRO(DbgGroup::DATA, "xaifBoosterLinearization::ExpressionAlg::createPartialExpressions:"
                               << " for argument " << (*anExpressionEdgeI2).getPosition()
                               << " the set of used positions is " << theUsedPositions.debug().c_str());
	Expression::ConstInEdgeIterator anExpressionEdgeI2_1(pE.first);
	// first add the result (position 0) if needed:
	if(usedByThisPartial.has(0))
	  theI2EdgeAlg.addArgumentsConcretizationPair(getContaining().getTargetOf(*anExpressionEdgeI2_1),
						  thePartialExpression.getPartialArgumentAt(0));
	// then go to all arguments:
	for(; anExpressionEdgeI2_1!=anExpressionEdgeIEnd; ++anExpressionEdgeI2_1) { // inner inner loop over all arguments to determine
	  // matches for this partial expression: 
	  if(usedByThisPartial.has((*anExpressionEdgeI2_1).getPosition()))
	    theI2EdgeAlg.addArgumentsConcretizationPair(getContaining().getSourceOf(*anExpressionEdgeI2_1),
						    thePartialExpression.getPartialArgumentAt((*anExpressionEdgeI2_1).getPosition()));
	} // end for 
      } // end inner loop

      // in the first pass through all arguments we made sure that we make all necessary auxiliary variables, 
      // in a second pass we can now deal with the partial expressions, but we had to make sure that all potential
      // arguments are already auxiliarized, therefore two passes are needed. 
      // In this pass we also determine whether the ExpressionEdgeAlg has a constant PDK (the default is nonlinear)
      // second inner loop over all arguments to create concrete expressions
      for(Expression::ConstInEdgeIterator anExpressionEdgeI3(pE.first); anExpressionEdgeI3!=anExpressionEdgeIEnd; ++anExpressionEdgeI3) {
	ExpressionEdgeAlg&theI3ExpressionEdgeAlg(dynamic_cast<ExpressionEdgeAlg&>((*anExpressionEdgeI3).getExpressionEdgeAlgBase()));
	// don't make a partial assignment for unit or passive partials
	if(theI3ExpressionEdgeAlg.getPartialDerivativeKind()==PartialDerivativeKind::LINEAR_ONE
	   ||
	   theI3ExpressionEdgeAlg.getPartialDerivativeKind()==PartialDerivativeKind::LINEAR_MINUS_ONE
	   ||
	   theI3ExpressionEdgeAlg.getPartialDerivativeKind()==PartialDerivativeKind::PASSIVE)
	  continue;

	// now copy the expression for the partial, i.e. make a concrete 
	// Expression for the abstract Expression given as InlinableIntrinsicsExpression
	// make a list for vertex mapping
	// from abstract to concrete vertices 
	const InlinableIntrinsicsExpression&
		thePartialExpression(theCatalogueItem.getExpressionVectorElement((*anExpressionEdgeI3).getPosition()));
        // maintain a mapping from abstract to concrete vertices to facilitate the expression duplication
        GuardedMap<const InlinableIntrinsicsExpressionVertex*,
                 const ExpressionVertex*> theAbstract2ConcreteMap;
	const ExpressionEdgeAlg::VertexPairList&theConcreteArgumentInstancesList(theI3ExpressionEdgeAlg.getConcreteArgumentInstancesList());

	// we don't make an assignment if there's only one partial expression vertex and it's constant
	if(thePartialExpression.numVertices()==1) {
	  // if we find it in the list of vertices known to be arguments to the partial expressions, then the following would be a vertex in 'this' expression
	  const ExpressionVertex*theArgumentVertex_p(NULL);
	  for(ExpressionEdgeAlg::VertexPairList::const_iterator li=theConcreteArgumentInstancesList.begin(); li!=theConcreteArgumentInstancesList.end(); ++li) {
	    if((*li).second== &thePartialExpression.getMaxVertex()) {
	      theArgumentVertex_p=(*li).first;
	      break;
	    }
	  }
	  if(theArgumentVertex_p) {
	    // now we know that this is either an argument, a constant, or any kind of vertex with an auxiliary Argument
	    ExpressionVertexAlg&theArgumentVertexAlg(dynamic_cast<ExpressionVertexAlg&>(theArgumentVertex_p->getExpressionVertexAlgBase()));
	    if(!theArgumentVertexAlg.hasAuxiliaryVariable()) { // this is an Argument or a Constant
	      // if the following cast fails then we have a constant which we can just skip:
	      const Argument* theArgument_p=dynamic_cast<const Argument*>(theArgumentVertex_p);
	      if(!theArgument_p) {
		theI3ExpressionEdgeAlg.linearate();
		theI3ExpressionEdgeAlg.setConcreteConstant(dynamic_cast<const Constant&>(*theArgumentVertex_p));
		continue;
	      } // end if constant
	    } // end argument or constant
	  } // end if
	} // end if there's only one vertex in the partial expression
	theI3ExpressionEdgeAlg.makeConcretePartialAssignment();
	Expression&theNewConcretePartial(theI3ExpressionEdgeAlg.getConcretePartialAssignment().getRHS());
	theI3ExpressionEdgeAlg.getConcretePartialAssignment().setId(makePartialAssignmentId());
	bool allConst=true;
	InlinableIntrinsicsExpression::ConstVertexIteratorPair anAbstractvertexPair(thePartialExpression.vertices());
	for(InlinableIntrinsicsExpression::ConstVertexIterator abstractVertexIt(anAbstractvertexPair.first), abstractVertexEndIt(anAbstractvertexPair.second);
	abstractVertexIt!=abstractVertexEndIt; ++abstractVertexIt) {
	  // if we find it in the list of vertices known to be 
	  // arguments to the partial expressions,
	  // then the following would be a vertex in 'this' expression
	  const ExpressionVertex*theArgumentVertex_p(0);
	  for(ExpressionEdgeAlg::VertexPairList::const_iterator li=theConcreteArgumentInstancesList.begin();
	  li!=theConcreteArgumentInstancesList.end();
	  ++li) {
	    if((*li).second==&*abstractVertexIt) {
	      theArgumentVertex_p=(*li).first;
	      break;
	    }
	  } // end for
	  ExpressionVertex*theNewVertex_p(0);
	  if(theArgumentVertex_p) {
	    // now we know that this is either
	    // a Argument or a Constant
	    // or any kind of vertex with an auxiliary Argument
	    ExpressionVertexAlg&theArgumentVertexAlg(dynamic_cast<ExpressionVertexAlg&>(theArgumentVertex_p->getExpressionVertexAlgBase()));
	    if(theArgumentVertexAlg.hasAuxiliaryVariable()) {
	      Argument* aNewArgument_p=new Argument();
	      theNewVertex_p=aNewArgument_p;
	      theArgumentVertexAlg.copyAuxiliaryInto(*aNewArgument_p);
	      allConst=false;
	    }
	    else { // this is an Argument or a Constant
	      theNewVertex_p=&(theArgumentVertex_p->createCopyOfMyself());
              if (theArgumentVertex_p->isArgument()) {
                allConst=false;
	      } // else it must be a constant
	    } // end argument or constant
            theI3ExpressionEdgeAlg.mapPartialEV2OriginalEV(*theNewVertex_p,
                                                           *theArgumentVertex_p);
	  } // end if
	  else {
	    // this is not referring to 
	    // a vertex in 'this'
	    // we just create a copy of the 
	    // abstract vertex
	    theNewVertex_p=&((*abstractVertexIt).createCopyOfMyself());
            //\TODO FIXME revisit this
	  } // end else
	  // in this new graph the potentially copied Ids are useless.
	  theNewVertex_p->overWriteId(theNewConcretePartial.getNextVertexId());
	  theNewConcretePartial.supplyAndAddVertexInstance(*theNewVertex_p);
          theAbstract2ConcreteMap.addElement(&(*abstractVertexIt), theNewVertex_p);
	} // end for all abstract vertices

	// only make it LINEAR if all vertices were constants and it's currently NONLINEAR (don't want to undo a passivate!)
	if(allConst&&theI3ExpressionEdgeAlg.getPartialDerivativeKind()==PartialDerivativeKind::NONLINEAR)
	  theI3ExpressionEdgeAlg.linearate();

	// now take care of all abstract edges
	InlinableIntrinsicsExpression::ConstEdgeIteratorPair anAbstractEdgePair(thePartialExpression.edges());
	InlinableIntrinsicsExpression::ConstEdgeIterator abstractEdgeIt(anAbstractEdgePair.first), abstractEdgeEndIt(anAbstractEdgePair.second);
	for(; abstractEdgeIt!=abstractEdgeEndIt; ++abstractEdgeIt) {
          ExpressionEdge& theNewEdge(theNewConcretePartial.addEdge(*theAbstract2ConcreteMap.getElement(&thePartialExpression.getSourceOf(*abstractEdgeIt)),
								   *theAbstract2ConcreteMap.getElement(&thePartialExpression.getTargetOf(*abstractEdgeIt))));
	  theNewEdge.setId(theNewConcretePartial.getNextEdgeId());
	  theNewEdge.setPosition((*abstractEdgeIt).getPosition());
	} // end for all abstract edges
	// make a left hand side: 
	Variable&theLHS(theI3ExpressionEdgeAlg.getConcretePartialAssignment().getLHS());
	xaifBoosterTypeChange::TemporariesHelper aHelper("xaifBoosterLinearization::ExpressionAlg::createPartialExpressions(for the LHS)",
							 theNewConcretePartial,
							 theNewConcretePartial.getMaxVertex());	
	VariableSymbolReference* theVariableSymbolReference_p =
	  new VariableSymbolReference(aHelper.makeTempSymbol(theCurrentCfgScope,
							     ConceptuallyStaticInstances::instance()->getLinearizationVariableNameCreator(),
							     false),
				      theCurrentCfgScope);
	// JU: this assignment of the vertex Id might have to change 
	// if we create vector assignments as auxiliary variables...
	theVariableSymbolReference_p->setId("1");
	theVariableSymbolReference_p->setAnnotation("xaifBoosterLinearization::ExpressionAlg::createPartialExpressions");
	theLHS.supplyAndAddVertexInstance(*theVariableSymbolReference_p);
	theLHS.getAliasMapKey().setTemporary();
	theLHS.getDuUdMapKey().setTemporary();
	if (aHelper.needsAllocation()) { 
          theI3ExpressionEdgeAlg.makePartialAllocation(aHelper);
	}
      } // end for
    } // end for all expression vertices (outer loop)
  } // end of ExpressionAlg::createPartialExpressions()

  void ExpressionAlg::activityAnalysisBottomUpPass(const ExpressionVertex& theVertex){
    Expression::ConstInEdgeIteratorPair pE(getContaining().getInEdgesOf(theVertex));
    Expression::ConstInEdgeIterator iE(pE.first), iEe(pE.second);
    for(; iE!=iEe; ++iE) {
      // go down first
      activityAnalysisBottomUpPass(getContaining().getSourceOf(*iE));
    }
    ExpressionVertexAlg&theExpressionVertexAlg(dynamic_cast<ExpressionVertexAlg&>(theVertex.getExpressionVertexAlgBase()));
    if(theExpressionVertexAlg.isActive()) {
      bool makePassive=false;
      if(theVertex.isArgument()) {
	// this may be an argument with a passive type 
	// whose flag defaulted to 'active'
	if(!dynamic_cast<const Argument&>(theVertex).getVariable().getActiveType())
	  makePassive=true;
      } // end if 
      else {
	// assume we want to passivate
	makePassive=true;
	// look at all incoming edges
	Expression::ConstInEdgeIterator iE2(pE.first);
	for(; iE2!=iEe; ++iE2) {
	  ExpressionEdgeAlg&theEdgeAlg(dynamic_cast<ExpressionEdgeAlg&>((*iE2).getExpressionEdgeAlgBase()));
	  if(theEdgeAlg.getPartialDerivativeKind()!=PartialDerivativeKind::PASSIVE) {
	    // see if we are properly initialized from the catalogue
	    if(!theEdgeAlg.hasPartial()) {
	      // no, have to do it here to learn about our partial definitions
	      theEdgeAlg.setPartial(theVertex.getInlinableIntrinsicsCatalogueItem().getExpressionVectorElement((*iE2).getPosition()));
	    }
	    if(theEdgeAlg.getPartialDerivativeKind()!=PartialDerivativeKind::PASSIVE) {
	      // don't passivate if we have an active edge
	      makePassive=false;
	    } // end if 
	  } // end if 
	} // end for 
      } // end else 
      if(makePassive)
	theExpressionVertexAlg.passivate();
    } // end if (vertex is active)
    if(!theExpressionVertexAlg.isActive()) {
      Expression::ConstOutEdgeIteratorPair pE(getContaining().getOutEdgesOf(theVertex));
      Expression::ConstOutEdgeIterator oE(pE.first), oEe(pE.second);
      for(; oE!=oEe; ++oE)
	dynamic_cast<ExpressionEdgeAlg&>((*oE).getExpressionEdgeAlgBase()).passivate();
    }
  } // end of ExpressionAlg::activityAnalysisBottomUpPass

  void ExpressionAlg::activityAnalysisTopDownPass(const ExpressionVertex& theVertex){
    Expression::ConstOutEdgeIteratorPair pE(getContaining().getOutEdgesOf(theVertex));
    Expression::ConstOutEdgeIterator oE(pE.first), oEe(pE.second);
    bool makePassive=false;
    if(oE!=oEe) {
      // we have out edges
      // assume we want to passivate
      makePassive=true;
    }
    for(; oE!=oEe; ++oE) {
      ExpressionEdgeAlg&theEdgeAlg(dynamic_cast<ExpressionEdgeAlg&>((*oE).getExpressionEdgeAlgBase()));
      if(theEdgeAlg.getPartialDerivativeKind()!=PartialDerivativeKind::PASSIVE) {
	makePassive=false;
	break;
      }
    }
    if(makePassive) {
      // take care of the vertex
      dynamic_cast<ExpressionVertexAlg&>(theVertex.getExpressionVertexAlgBase()).passivate();
    }
    // and all its in-edges
    Expression::ConstInEdgeIteratorPair pE2(getContaining().getInEdgesOf(theVertex));
    Expression::ConstInEdgeIterator iE(pE2.first), iEe(pE2.second);
    for(; iE!=iEe; ++iE) {
      if(makePassive)
	dynamic_cast<ExpressionEdgeAlg&>((*iE).getExpressionEdgeAlgBase()).passivate();
      activityAnalysisTopDownPass(getContaining().getSourceOf(*iE));
    }
  } // end of ExpressionAlg::activityAnalysisTopDownPass

  std::string
  ExpressionAlg::makeAuxiliaryAssignmentSSAId(const ObjectWithId::Id& theTopLevelAssignmentId) const {
    std::ostringstream ostr;
    ostr << "_replacement_" << ++myAuxiliaryAssignmentCounter
         << "_for_"  << theTopLevelAssignmentId << std::ends;
    return ostr.str();
  }

  std::string
  ExpressionAlg::makePartialAssignmentId(){
    static unsigned anId=0;
    std::ostringstream ostr;
    ostr<<"_elementary_partial_"<<anId++ <<std::ends;
    return ostr.str();
  }

}
