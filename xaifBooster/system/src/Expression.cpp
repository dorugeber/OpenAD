// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include <iostream>
#include <sstream>

#include "xaifBooster/utils/inc/DbgLoggerManager.hpp"

#include "xaifBooster/system/inc/GraphVizDisplay.hpp"
#include "xaifBooster/system/inc/Expression.hpp"
#include "xaifBooster/system/inc/ExpressionAlgBase.hpp"
#include "xaifBooster/system/inc/ExpressionAlgFactory.hpp"
#include "xaifBooster/system/inc/InlinableIntrinsicsCatalogueItem.hpp"
#include "xaifBooster/system/inc/ConceptuallyStaticInstances.hpp"
#include "xaifBooster/system/inc/Assignment.hpp"
#include "xaifBooster/system/inc/CallGraph.hpp"
#include "xaifBooster/system/inc/VariableSymbolReference.hpp"
#include "xaifBooster/system/inc/Constant.hpp"
#include "xaifBooster/system/inc/Intrinsic.hpp"
#include "xaifBooster/system/inc/BooleanOperation.hpp"
#include "xaifBooster/system/inc/BooleanOperationType.hpp"

namespace xaifBooster{

  class ExpressionVertexLabelWriter{
  public:

    ExpressionVertexLabelWriter(const Expression& g): myG(g){
    };

    template <class BoostInternalDescriptor>
    void operator()(std::ostream& out,
		    const BoostInternalDescriptor& v) const{
      const ExpressionVertex* theExprVertex_p=
	dynamic_cast<const ExpressionVertex*>(boost::get(boost::get(BoostVertexContentType(),
								    myG.getInternalBoostGraph()),
							 v));
      if(theExprVertex_p->isArgument()) { 
        const Argument& theArgument(dynamic_cast<const Argument&>(*theExprVertex_p));
	out<<"[label=\""<<theArgument.getVariable().getVariableSymbolReference().getSymbol().getId().c_str()<<"\"]";
      }
      else {
	if(myG.numInEdgesOf(*theExprVertex_p)) { // is an intrinsic or boolean op
          if (theExprVertex_p->isIntrinsic()){
	    const Intrinsic& theIntrinsic(dynamic_cast<const Intrinsic&>(*theExprVertex_p));
	    out<<"[label=\""<<theIntrinsic.getInlinableIntrinsicsCatalogueItem().getFunction().getBuiltinFunctionName().c_str()<<"\"]";
          }
          else { // boolean op
	    const BooleanOperation& theBO(dynamic_cast<const BooleanOperation&>(*theExprVertex_p));
	    out<<"[label=\""<<BooleanOperationType::toString(theBO.getType()).c_str()<<"\"]";
          }
	}
	else { // must be a constant then
          const Constant& theConstant(dynamic_cast<const Constant&>(*theExprVertex_p));
	  out<<"[label=\""<<theConstant.toString().c_str()<<"\"]";
	}
      }
    }
    const Expression& myG;
  };

  class ExpressionEdgeLabelWriter{
  public:

    ExpressionEdgeLabelWriter(const Expression& g): myG(g){
    };

    template <class BoostInternalDescriptor>
    void operator()(std::ostream& out, const BoostInternalDescriptor& v) const{
      const ExpressionEdge* theExprEdge_p=
	dynamic_cast<const ExpressionEdge*>(boost::get(boost::get(BoostEdgeContentType(),
								  myG.getInternalBoostGraph()),
						       v));
      out<<"[label=\""<<theExprEdge_p->getPosition()<<"\"]";
    }
    const Expression& myG;
  };

  class ExpressionPropertiesWriter{
  public:

    ExpressionPropertiesWriter(const Expression& g): myG(g){
    };

    void operator()(std::ostream& out) const{
      out<<"rankdir=BT;"<<std::endl;
    }
    const Expression& myG;
  };

  void Expression::show(const std::string& outputName) const{
    GraphVizDisplay::show(*this,
			  outputName,
			  ExpressionVertexLabelWriter(*this),
			  ExpressionEdgeLabelWriter(*this),
			  ExpressionPropertiesWriter(*this));
  }

  Expression::Expression(bool hasAlgorithm):
    myExpressionAlgBase_p(0){
    if(hasAlgorithm)
      myExpressionAlgBase_p=ExpressionAlgFactory::instance()->makeNewAlg(*this);
  }

  Expression::~Expression(){
    if(myExpressionAlgBase_p)
      delete myExpressionAlgBase_p;
  }

  ExpressionAlgBase&
  Expression::getExpressionAlgBase() const{
    if(!myExpressionAlgBase_p)
      THROW_LOGICEXCEPTION_MACRO("Expression::getExpressionAlgBase: not set");
    return *myExpressionAlgBase_p;
  }

  void
  Expression::printXMLHierarchy(std::ostream& os) const{
    if(myExpressionAlgBase_p
       &&
       !ConceptuallyStaticInstances::instance()->getPrintVersion()==PrintVersion::SYSTEM_ONLY)
      getExpressionAlgBase().printXMLHierarchy(os);
    else
      printXMLHierarchyImpl(os);
  } // end of Expression::printXMLHierarchy

  void
  Expression::printXMLHierarchyImpl(std::ostream& os) const{
    ConstVertexIteratorPair p(vertices());
    ConstVertexIterator beginIt(p.first), endIt(p.second);
    for(; beginIt!=endIt; ++beginIt)
      (*beginIt).printXMLHierarchy(os);
    ConstEdgeIteratorPair pe=edges();
    ConstEdgeIterator beginIte(pe.first), endIte(pe.second);
    for(; beginIte!=endIte; ++beginIte)
      (*beginIte).printXMLHierarchy(os, *this);
  } // end of Expression::printXMLHierarchyImpl

  std::string Expression::debug() const{
    std::ostringstream out;
    out<<"Expression["<<this
       <<", numVertices="<<numVertices()
       <<"]"<<std::ends;
    return out.str();
  } // end of Expression::debug

  void Expression::traverseToChildren(const GenericAction::GenericAction_E anAction_c){
    getExpressionAlgBase().genericTraversal(anAction_c);
    GraphWrapperTraversable<ExpressionVertex, ExpressionEdge>::traverseToChildren(anAction_c);
  } // end traversalToChildren

  ExpressionVertex&
  Expression::copyMyselfInto(Expression& theTarget,
			     bool withNewId,
			     bool withAlgorithm) const{
    ConstVertexIteratorPair p(vertices());
    ConstVertexIterator beginIt(p.first), endIt(p.second);
    typedef std::pair<const ExpressionVertex*, const ExpressionVertex*> PointerPair;
    const ExpressionVertex *maxOrigVertexP;
    ExpressionVertex *maxCopiedVertexP;
    maxOrigVertexP= &(getMaxVertex());
    typedef std::list<PointerPair> PointerPairList;
    PointerPairList theList; // first original, second copy
    for(; beginIt!=endIt; ++beginIt) {
      ExpressionVertex&theCopy((*beginIt).createCopyOfMyself(withAlgorithm));
      if(withNewId)
	theCopy.overWriteId(theTarget.getNextVertexId());
      theTarget.supplyAndAddVertexInstance(theCopy);
      if(maxOrigVertexP== &(*beginIt))
	maxCopiedVertexP= &theCopy;
      theList.push_back(PointerPair(&(*beginIt), &theCopy));
    }
    ConstEdgeIteratorPair pe=edges();
    ConstEdgeIterator beginIte(pe.first), endIte(pe.second);
    for(; beginIte!=endIte; ++beginIte) {
      const ExpressionVertex
	*theOriginalSource_p(&(getSourceOf(*beginIte))),
	*theOriginalTarget_p(&(getTargetOf(*beginIte)));
      const ExpressionVertex
	*theCopySource_p(0),
	*theCopyTarget_p(0);
      for(PointerPairList::const_iterator li=theList.begin();
	  li!=theList.end()
	    &&
	    !(theCopySource_p&&theCopyTarget_p);
	  ++li) {
	if(!theCopySource_p&&(*li).first==theOriginalSource_p)
	  theCopySource_p=(*li).second;
	if(!theCopyTarget_p&&(*li).first==theOriginalTarget_p)
	  theCopyTarget_p=(*li).second;
      } // end for 
      if(!theCopySource_p|| !theCopyTarget_p)
	THROW_LOGICEXCEPTION_MACRO("Expression::copyMyselfInto: couldn't find source or target");
      ExpressionEdge* theCopy=new ExpressionEdge(withAlgorithm);
      (*beginIte).copyMyselfInto(*theCopy);
      if(withNewId)
	(*theCopy).overWriteId(theTarget.getNextEdgeId());
      theTarget.supplyAndAddEdgeInstance(*theCopy, *theCopySource_p, *theCopyTarget_p);
    } // end for 
    return *maxCopiedVertexP;
  } // end of Expression::copyMyselfInto

  ExpressionVertex&
  Expression::copySubExpressionInto(Expression& theTarget,
				    const ExpressionVertex& theTopVertex,
				    bool withNewId,
				    bool withAlgorithm) const{
    typedef std::list<const ExpressionVertex*> ExpressionVertexPList;
    ExpressionVertexPList theList;
    ExpressionVertex&theTopCopy(theTopVertex.createCopyOfMyself(withAlgorithm));
    if(withNewId)
      theTopCopy.overWriteId(theTarget.getNextVertexId());
    theTarget.supplyAndAddVertexInstance(theTopCopy);
    theList.push_back(&theTopVertex);
    copySubExpressionInto(theTarget,
			  theTopVertex,
			  theTopCopy,
			  withNewId,
			  withAlgorithm,
			  theList);
    return theTopCopy;
  }

  void
  Expression::copySubExpressionInto(Expression& theTarget,
				    const ExpressionVertex& theTopVertex,
				    ExpressionVertex& theTopCopy,
				    bool withNewId,
				    bool withAlgorithm,
				    Expression::ExpressionVertexPList theList) const{
    ConstInEdgeIteratorPair pe=getInEdgesOf(theTopVertex);
    ConstInEdgeIterator beginIte(pe.first), endIte(pe.second);
    for(; beginIte!=endIte; ++beginIte) {
      const ExpressionVertex&theOriginalSource(getSourceOf(*beginIte));
      // check if we created the target already
      bool haveIt=false;
      for(ExpressionVertexPList::iterator i=theList.begin();
	  i!=theList.end();
	  ++i) {
	if(&theOriginalSource==*i) {
	  haveIt=true;
	  break;
	}
      }
      if(haveIt)
	continue;
      ExpressionVertex&theCopy(theOriginalSource.createCopyOfMyself(withAlgorithm));
      if(withNewId)
	theCopy.overWriteId(theTarget.getNextVertexId());
      theTarget.supplyAndAddVertexInstance(theCopy);
      theList.push_back(&theOriginalSource);
      ExpressionEdge* theEdgeCopy=new ExpressionEdge(withAlgorithm);
      (*beginIte).copyMyselfInto(*theEdgeCopy);
      if(withNewId)
	(*theEdgeCopy).overWriteId(theTarget.getNextEdgeId());
      theTarget.supplyAndAddEdgeInstance(*theEdgeCopy, theCopy, theTopCopy);
      copySubExpressionInto(theTarget,
			    theOriginalSource,
			    theCopy,
			    withNewId,
			    withAlgorithm,
			    theList);
    } // end for 
  }

  const ExpressionVertex& Expression::findPositionalSubExpressionOf(const ExpressionVertex& aVertex,
								    unsigned int aPosition) const{
    ConstInEdgeIteratorPair p=getInEdgesOf(aVertex);
    ConstInEdgeIterator vI(p.first), vIE(p.second);
    for(; vI!=vIE; ++vI)
      if(aPosition==(*vI).getPosition())
	return getSourceOf(*vI);
    THROW_LOGICEXCEPTION_MACRO("Expression::findPositionalSubExpressionOf: no such subexpression");
    return aVertex;
  }

  void Expression::appendArguments(Expression::ArgumentPList& listToBeAppended){
    Expression::VertexIteratorPair expVertItPair(vertices());
    Expression::VertexIterator expVertIt(expVertItPair.first), expVertItEnd(expVertItPair.second);
    for(; expVertIt!=expVertItEnd; ++expVertIt) {
      Expression::InEdgeIteratorPair expInEdgeItPair(getInEdgesOf(*expVertIt));
      if(expInEdgeItPair.first==expInEdgeItPair.second // no in edges
	 &&
	 (*expVertIt).isArgument()) {
        // see if it is non-value inquiry
        bool nonValueInquiry=false;
        Expression::OutEdgeIteratorPair expOutEdgeItPair(getOutEdgesOf(*expVertIt));
        Expression::OutEdgeIterator expOutEdgeIt(expOutEdgeItPair.first), expOutEdgeItEnd(expOutEdgeItPair.second);
        for(; expOutEdgeIt!=expOutEdgeItEnd; ++expOutEdgeIt) {
          if (getTargetOf(*expOutEdgeIt).isIntrinsic()){
            const Intrinsic& theIntrinsic(dynamic_cast<const Intrinsic&>(getTargetOf(*expOutEdgeIt)));
            if (!(theIntrinsic.getInlinableIntrinsicsCatalogueItem().getExpressionVectorElement((*expOutEdgeIt).getPosition()).isNonValueInquiry())) {
              nonValueInquiry=false;
              break;
            }
	    else  
	      nonValueInquiry=true;  // but it needs to be true for all outedges, so we can't break here
          }
        }
	listToBeAppended.push_back(std::make_pair(&(dynamic_cast<Argument&>(*expVertIt)),nonValueInquiry));
      }
    }
  }

  void Expression::appendArguments(Expression::CArgumentPList& listToBeAppended)const{
    Expression::ConstVertexIteratorPair expVertItPair(vertices());
    Expression::ConstVertexIterator expVertIt(expVertItPair.first), expVertItEnd(expVertItPair.second);
    for(; expVertIt!=expVertItEnd; ++expVertIt) {
      Expression::ConstInEdgeIteratorPair expInEdgeItPair(getInEdgesOf(*expVertIt));
      if(expInEdgeItPair.first==expInEdgeItPair.second // no in edges
	 &&
	 (*expVertIt).isArgument()) {
	// see if it is non-value inquiry
        bool nonValueInquiry=false;
        Expression::ConstOutEdgeIteratorPair expOutEdgeItPair(getOutEdgesOf(*expVertIt));
        Expression::ConstOutEdgeIterator expOutEdgeIt(expOutEdgeItPair.first), expOutEdgeItEnd(expOutEdgeItPair.second);
        for(; expOutEdgeIt!=expOutEdgeItEnd; ++expOutEdgeIt) {
          if (getTargetOf(*expOutEdgeIt).isIntrinsic()) {
	    const Intrinsic& theIntrinsic(dynamic_cast<const Intrinsic&>(getTargetOf(*expOutEdgeIt)));
	    if (theIntrinsic.isInlinable() 
		&& 
		!(theIntrinsic.getInlinableIntrinsicsCatalogueItem().getExpressionVectorElement((*expOutEdgeIt).getPosition()).isNonValueInquiry())) {
	      nonValueInquiry=false;
	      break;
	    }
	    else  
	      nonValueInquiry=true;  // but it needs to be true for all outedges, so we can't break here
          }
        }
	listToBeAppended.push_back(std::make_pair(&(dynamic_cast<const Argument&>(*expVertIt)),nonValueInquiry));
      }
    }
  }

  void Expression::replaceVariables(const Expression::VariablePVariableSRPPairList& replacementList){
    ArgumentPList listToBeAppended;
    appendArguments(listToBeAppended);
    for(ArgumentPList::iterator argumentI=listToBeAppended.begin();
	argumentI!=listToBeAppended.end();
	++argumentI) {
      for(VariablePVariableSRPPairList::const_iterator replacementI=replacementList.begin();
	  replacementI!=replacementList.end();
	  ++replacementI) {
	if((*replacementI).first->equivalentTo((*argumentI).first->getVariable())) {
	  DBG_MACRO(DbgGroup::DATA, "Expression::replaceVariables: replacing :"
		    <<(*argumentI).first->getVariable().debug().c_str());
	  // make the replacement vertex in this expression
	  Argument* newArgument_p=new Argument(false);
	  newArgument_p->getVariable().supplyAndAddVertexInstance((*replacementI).second->createCopyOfMyself());
	  supplyAndAddVertexInstance(*newArgument_p);
	  Expression::OutEdgeIteratorPair expOutEdgeItPair(getOutEdgesOf(*((*argumentI).first)));
	  Expression::OutEdgeIterator expOutEdgeIt(expOutEdgeItPair.first), expOutEdgeItEnd(expOutEdgeItPair.second);
	  typedef std::pair<ExpressionEdge*, ExpressionVertex*> TargetPair;
	  typedef std::list<TargetPair> TargetPairList;
	  TargetPairList targetList;
	  // save the targets separately because insertion 
	  // etc. confuses the graph iterators
	  for(; expOutEdgeIt!=expOutEdgeItEnd; ++expOutEdgeIt) {
	    targetList.push_back(TargetPair(&(*expOutEdgeIt), &getTargetOf(*expOutEdgeIt)));
	  }
	  // now make the new edges
	  for(TargetPairList::iterator it=targetList.begin();
	      it!=targetList.end();
	      ++it) {
	    DBG_MACRO(DbgGroup::DATA, "Expression::replaceVariables: replacing edge "
		      <<(*it).first->debug().c_str()
		      <<" with target "
		      <<(*it).second->debug().c_str());
	    supplyAndAddEdgeInstance((*it).first->createCopyOfMyself(),
				     *newArgument_p,
				     *((*it).second));
	  }
	  newArgument_p->setId((*argumentI).first->getId());
	  removeAndDeleteVertex(*((*argumentI).first));
	  break;
	}
      }
    }
  }

  bool Expression::isConstantR(const ExpressionVertex& theTopVertex) const{
    ConstInEdgeIteratorPair theInEdgesP=getInEdgesOf(theTopVertex);
    ConstInEdgeIterator inEdgeIt=theInEdgesP.first, inEdgeEndIt=theInEdgesP.second;
    if(inEdgeIt==inEdgeEndIt) {
      return !(theTopVertex.isArgument());
    }
    for(; inEdgeIt!=inEdgeEndIt; ++inEdgeIt) {
      if(!isConstantR(getSourceOf(*inEdgeIt)))
	return false;
    }
    return true;
  }

  bool Expression::isConstant() const{
    return isConstantR(getMaxVertex());
  }

  int Expression::constIntEvalR(const ExpressionVertex& theTopVertex) const{
    ConstInEdgeIteratorPair theInEdgesP=getInEdgesOf(theTopVertex);
    ConstInEdgeIterator inEdgeIt=theInEdgesP.first, inEdgeEndIt=theInEdgesP.second;
    if(inEdgeIt==inEdgeEndIt) {
      const Constant *theConstant_p=dynamic_cast<const Constant*>(&theTopVertex);
      if(theConstant_p) {
	return theConstant_p->getint();
      }
    }
    const Intrinsic* theIntrinsic_p=dynamic_cast<const Intrinsic*>(&theTopVertex);
    if(!theIntrinsic_p)
      THROW_LOGICEXCEPTION_MACRO("Expression::constIntEvalR: non-leaf vertex "<<theTopVertex.debug().c_str()<<" is not an intrinsic");
    int arg[2];
    for(; inEdgeIt!=inEdgeEndIt; ++inEdgeIt) {
      arg[(*inEdgeIt).getPosition()-1]=constIntEvalR(getSourceOf(*inEdgeIt));
    }
    if(theIntrinsic_p->getName()=="plus_scal_scal") {
      return arg[0]+arg[1];
    }
    else if(theIntrinsic_p->getName()=="minus_scal_scal") {
      return arg[0]-arg[1];
    }
    else if(theIntrinsic_p->getName()=="div_scal_scal") {
      return arg[0]/arg[1];
    }
    else if(theIntrinsic_p->getName()=="unary_minus") {
      return -arg[0];
    }
    else {
      THROW_LOGICEXCEPTION_MACRO("Expression::constIntEvalR: no logic implemented for "<<theIntrinsic_p->getName().c_str());
    }
    return 0;
  }

  int Expression::constIntEval() const{
    if(!isConstant())
      THROW_LOGICEXCEPTION_MACRO("Expression::constIntEval: not a constant");
    const ExpressionVertex& theMaxVertex=getMaxVertex();
    return constIntEvalR(theMaxVertex);
  }

  void Expression::appendActiveArguments(CArgumentPList& listToBeAppended) const{
    CArgumentPList aList;
    appendArguments(aList);
    for(CArgumentPList::const_iterator argumentI=aList.begin();
	argumentI!=aList.end();
	++argumentI) {
      if((*argumentI).first->getVariable().getActiveType())
	listToBeAppended.push_back(*argumentI);
    }
  }

  ExpressionVertex& Expression::addBinaryOpByName(const std::string& opName,
						  const ExpressionVertex& arg1TopVertex,
						  const ExpressionVertex& arg2TopVertex){
    Intrinsic* theNewInt_p=new Intrinsic(opName);
    supplyAndAddVertexInstance(*theNewInt_p);
    if(!has(arg1TopVertex))
      THROW_LOGICEXCEPTION_MACRO("Expression::addBinaryOpByName: arg1TopVertex "<<arg1TopVertex.debug().c_str()<<" is not in the Expression");
    if(!has(arg2TopVertex))
      THROW_LOGICEXCEPTION_MACRO("Expression::addBinaryOpByName: arg2TopVertex "<<arg2TopVertex.debug().c_str()<<" is not in the Expression");
    ExpressionEdge* aNewEdge_p=new ExpressionEdge(false);
    aNewEdge_p->setId(getNextEdgeId());
    supplyAndAddEdgeInstance(*aNewEdge_p,
			     arg1TopVertex,
			     *theNewInt_p);
    aNewEdge_p->setPosition(1);
    aNewEdge_p=new ExpressionEdge(false);
    aNewEdge_p->setId(getNextEdgeId());
    supplyAndAddEdgeInstance(*aNewEdge_p,
			     arg2TopVertex,
			     *theNewInt_p);
    aNewEdge_p->setPosition(2);
    return *theNewInt_p;
  }

  bool
  Expression::hasExpression(const Expression& anExpression) const {
    if (&anExpression == this)
      return true;
    CArgumentPList theArguments;
    appendArguments(theArguments);
    for (CArgumentPList::iterator argI = theArguments.begin(); argI != theArguments.end(); ++argI)
      if ((*argI).first->getVariable().hasExpression(anExpression))
        return true;
    if (myExpressionAlgBase_p)
      return myExpressionAlgBase_p->hasExpression(anExpression);
    else
      return false;
  }

  Expression::ReturnOfSingleVariable Expression::singleVariable() const {
	  Expression::ConstVertexIteratorPair itP=vertices();
	  const Variable* theVar_p=0;
	  if (itP.first!=itP.second) {
		  if ((*(itP.first)).isArgument()) {
			  theVar_p=&(dynamic_cast<const Argument&>((*(itP.first))).getVariable());
			  ++(itP.first);
		  }
	  }
	  if (theVar_p && itP.first==itP.second)
		  return ReturnOfSingleVariable(theVar_p);
	  return ReturnOfSingleVariable(NULL);
  }

  Expression::ReturnOfSingleConstant Expression::singleConstant() const {
	  Expression::ConstVertexIteratorPair itP=vertices();
	  const Constant* theConst_p=0;
	  if (itP.first!=itP.second) {
		  if (!(*(itP.first)).isArgument()) {
			  // if not an Argument and the only vertex this should be a constant
			  // if it is not (intrinsic w/o arguments?)  then the case will return a 0 and we are still ok
			  theConst_p=dynamic_cast<const Constant*>(&(*(itP.first)));
			  ++(itP.first);
		  }
	  }
	  if (theConst_p && itP.first==itP.second)
		  return ReturnOfSingleConstant(theConst_p);
	  return ReturnOfSingleConstant(NULL);
  }

}
