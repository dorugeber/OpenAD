// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include <cctype>
#include <algorithm>

#include "xaifBooster/utils/inc/DbgLoggerManager.hpp"
#include "xaifBooster/utils/inc/PrintManager.hpp"

#include "xaifBooster/system/inc/DimensionBounds.hpp"
#include "xaifBooster/system/inc/Scope.hpp"
#include "xaifBooster/system/inc/Constant.hpp"
#include "xaifBooster/system/inc/Intrinsic.hpp"
#include "xaifBooster/system/inc/InlinableIntrinsicsCatalogueItem.hpp"
#include "xaifBooster/system/inc/VariableSymbolReference.hpp"

#include "xaifBooster/algorithms/TypeChange/inc/TemporariesHelper.hpp"

namespace xaifBoosterTypeChange{

  TemporariesHelper::TemporariesHelper(const std::string& contextAnnotation,
				       const Expression& theExpression,
				       const ExpressionVertex& theTopVertex):
    myContextAnnnotation(contextAnnotation),
    myExpression_p(&theExpression),
    myTopVertex_p(&theTopVertex),
    myVariable_p(NULL),
    myShape(SymbolShape::SCALAR),
    myType(SymbolType::INTEGER_STYPE),
    myTypeInfo(false){
  }

  TemporariesHelper::TemporariesHelper(const std::string& contextAnnotation,
				       const Variable& variableToMatch):
    myContextAnnnotation(contextAnnotation),
    myExpression_p(NULL),
    myTopVertex_p(NULL),
    myVariable_p(&variableToMatch),
    myShape(SymbolShape::SCALAR),
    myType(SymbolType::INTEGER_STYPE),
    myTypeInfo(false),
    myAllocationModel_p(NULL) {
  }

  TemporariesHelper::~TemporariesHelper(){
    for(DimensionBoundsPVector::iterator it=myDimensionBoundsPVector.begin();
	it!=myDimensionBoundsPVector.end();
	++it) {
      if(*it)
	delete (*it);
    }
  }

  Symbol& TemporariesHelper::makeTempSymbol(Scope& aScope, bool activeType){
    setTypeInfo();
    Symbol&theNewVariableSymbol(aScope.getSymbolTable().
				addUniqueAuxSymbol(SymbolKind::VARIABLE,
						   myType,
						   myShape,
						   activeType));
    theNewVariableSymbol.setFrontEndType(myFrontEndType);
    if(myShape!=SymbolShape::SCALAR) {
      setDimensionBounds(theNewVariableSymbol);
    }
    return theNewVariableSymbol;
  } 

  SymbolShape::SymbolShape_E TemporariesHelper::getSymbolShape() { 
    setTypeInfo();
    return myShape;
  } 

  Symbol& TemporariesHelper::makeTempSymbol(Scope& aScope,
                                            const NameCreator& aNameCreator,
                                            bool isActive){
    setTypeInfo();
    Symbol& theNewVariableSymbol (aScope.getSymbolTable().addUniqueSymbol(aNameCreator,
                                                                          SymbolKind::VARIABLE,
                                                                          myType,
                                                                          myShape,
                                                                          isActive));
    theNewVariableSymbol.setFrontEndType(myFrontEndType);
    if(myShape!=SymbolShape::SCALAR) {
      setDimensionBounds(theNewVariableSymbol);
    }
    return theNewVariableSymbol;
  } 

  void TemporariesHelper::setDimensionBounds(Symbol& aNewSymbol){
    unsigned short found=0;
    for(DimensionBoundsPVector::iterator it=myDimensionBoundsPVector.begin();
	it!=myDimensionBoundsPVector.end();
	++it) {
      if(*it)
	++found;
    }
    if(found==myShape&&found==myDimensionBoundsPVector.size()) {
      switch(DimensionBounds::getIndexOrder()) {
      case IndexOrder::ROWMAJOR: // c and c++
	for(DimensionBoundsPVector::iterator it=myDimensionBoundsPVector.begin();
	    it!=myDimensionBoundsPVector.end();
	    ++it) {
	  if(*it)
	    aNewSymbol.addDimensionBounds((*it)->getLower(), (*it)->getUpper());
	}
	break;
      case IndexOrder::COLUMNMAJOR:  // fortran
	for(DimensionBoundsPVector::reverse_iterator it=myDimensionBoundsPVector.rbegin();
	    it!=myDimensionBoundsPVector.rend();
	    ++it) {
	  if(*it)
	    aNewSymbol.addDimensionBounds((*it)->getLower(), (*it)->getUpper());
	}
	break;
      }
    }
  }

  void TemporariesHelper::typeInfo(const ExpressionVertex & theTopVertex){
    DBG_TAG_MACRO(DbgGroup::DATA,"temporaries","typeInfo("<< theTopVertex.debug().c_str() << ")")
    if(theTopVertex.isArgument()) {
      typeInfo(dynamic_cast<const Argument&>(theTopVertex).getVariable());
    }
    else {
      Expression::ConstInEdgeIteratorPair theInEdgesP=myExpression_p->getInEdgesOf(theTopVertex);
      Expression::ConstInEdgeIterator inEdgeIt=theInEdgesP.first, inEdgeEndIt=theInEdgesP.second;
      if(inEdgeIt==inEdgeEndIt) { // must be a constant
	typeInfo(dynamic_cast<const Constant&>(theTopVertex));
      }
      else { 
	for(; inEdgeIt!=inEdgeEndIt; ++inEdgeIt)
	  typeInfo(myExpression_p->getSourceOf(*inEdgeIt));
      }
    }
  }

  void TemporariesHelper::typeInfo(const Constant& aConstant){
    DBG_TAG_MACRO(DbgGroup::DATA,"temporaries","typeInfo("<< aConstant.debug().c_str() << ")")
    if (!myTypeInfo) {
      myTypeInfo=true;
      myType=aConstant.getType();
      myFrontEndType=aConstant.getFrontEndType();
    }
    else { 
      SymbolType::SymbolType_E aCType=aConstant.getType();
      SymbolType::SymbolType_E promotedType=SymbolType::genericPromotion(myType,aCType);
      if (aCType==promotedType && myType!=promotedType)
	myFrontEndType=aConstant.getFrontEndType();
      myType=promotedType;
    }
  }

  void TemporariesHelper::typeInfo(const Variable & theVariable){
    DBG_TAG_MACRO(DbgGroup::DATA,"temporaries","typeInfo("<< theVariable.debug().c_str() << ")")
    if (!myTypeInfo) {
      myTypeInfo=true;
      myType=theVariable.getVariableSymbolReference().getSymbol().getSymbolType();
      myFrontEndType=theVariable.getVariableSymbolReference().getSymbol().getFrontEndType();
    }
    else {
      SymbolType::SymbolType_E aVType=theVariable.getVariableSymbolReference().getSymbol().getSymbolType();
      SymbolType::SymbolType_E promotedType=SymbolType::genericPromotion(myType,aVType);
      if (aVType==promotedType && myType!=promotedType)
	myFrontEndType=theVariable.getVariableSymbolReference().getSymbol().getFrontEndType();
      myType=promotedType;
    }
    SymbolShape::SymbolShape_E argShape=theVariable.getEffectiveShape();
    if(myShape!=SymbolShape::SCALAR&&argShape!=SymbolShape::SCALAR&&myShape!=argShape)
      THROW_LOGICEXCEPTION_MACRO("TemporariesHelper::typeInfo: effective shape change between "
				 <<SymbolShape::toString(myShape)
				 <<" and "
				 <<SymbolShape::toString(argShape));
    myShape=(myShape>argShape)?myShape:argShape;
    if(myShape==argShape) {
      if(!myDimensionBoundsPVector.size())
	myDimensionBoundsPVector.resize(myShape, NULL);
      populateDimensionBounds(theVariable);
      myAllocationModel_p=&theVariable;
    }
  }

  void TemporariesHelper::populateDimensionBounds(const Variable & aVariable){
    const Symbol::DimensionBoundsPList *symbolDimensionBoundsP=NULL;
    if(aVariable.getVariableSymbolReference().getSymbol().hasDimensionBounds()) {
      symbolDimensionBoundsP= &(aVariable.getVariableSymbolReference().getSymbol().getDimensionBoundsPList());
    }
    if(aVariable.hasArrayAccess()) {
      unsigned short theDimension=0;
      const ArrayAccess::IndexTripletListType&theIndexTripletList(aVariable.getArrayAccess().getIndexTripletList());
      DimensionBounds *aDimensionBounds_p=NULL;
      Symbol::DimensionBoundsPList::const_iterator symbolDBIt;
      Symbol::DimensionBoundsPList::const_reverse_iterator symbolDBRIt;
      if(symbolDimensionBoundsP) {
	switch(DimensionBounds::getIndexOrder()) {
	case IndexOrder::ROWMAJOR: // c and c++
	  symbolDBIt=symbolDimensionBoundsP->begin();
	  aDimensionBounds_p=*symbolDBIt;
	  break;
	case IndexOrder::COLUMNMAJOR:  // fortran
	  symbolDBRIt=symbolDimensionBoundsP->rbegin();
	  aDimensionBounds_p=*symbolDBRIt;
	  break;
	}
      }
      for(ArrayAccess::IndexTripletListType::const_iterator it=theIndexTripletList.begin();
	  it!=theIndexTripletList.end();
	  ++it) {
	if(!((*it)->isScalarDeref())) {
	  ++theDimension; // 1-based counting
	  const Expression *indexExprP=NULL, *boundExprP=NULL, *strideExprP=NULL;
	  Expression indexExpr, boundExpr;
	  // determine size if possible
	  if((*it)->hasExpression(IndexTriplet::IT_INDEX)) {
	    const Expression& theExpr=(*it)->getExpression(IndexTriplet::IT_INDEX);
	    if(theExpr.isConstant()) {
	      indexExprP= &theExpr;
	    }
	  }
	  if(!indexExprP) {
	    if(aDimensionBounds_p) {
	      indexExpr.supplyAndAddVertexInstance(*(new Constant(aDimensionBounds_p->getLower()))).setId(1);
	      indexExprP= &indexExpr;
	    }
	  }

	  if((*it)->hasExpression(IndexTriplet::IT_BOUND)) {
	    const Expression& theExpr=(*it)->getExpression(IndexTriplet::IT_BOUND);
	    if(theExpr.isConstant()) {
	      boundExprP= &theExpr;
	    }
	  }
	  if(!boundExprP) {
	    if(aDimensionBounds_p) {
	      boundExpr.supplyAndAddVertexInstance(*(new Constant(aDimensionBounds_p->getUpper()))).setId(1);
	      boundExprP= &boundExpr;
	    }
	  }
	  if((*it)->hasExpression(IndexTriplet::IT_STRIDE)) {
	    const Expression& theExpr=(*it)->getExpression(IndexTriplet::IT_STRIDE);
	    if(theExpr.isConstant()) {
	      strideExprP= &theExpr;
	    }
	  }
	  if(indexExprP&&boundExprP) {
	    Expression effectiveBoundExpression;
	    // make the actual expression
	    Constant* theOne_p=new Constant(1);
	    effectiveBoundExpression.supplyAndAddVertexInstance(*theOne_p);
	    ExpressionVertex& theNewIndex=indexExprP->copyMyselfInto(effectiveBoundExpression, true, false);
	    ExpressionVertex& theNewBound=boundExprP->copyMyselfInto(effectiveBoundExpression, true, false);
	    ExpressionVertex& theNewMinus=effectiveBoundExpression.addBinaryOpByName("minus_scal_scal", theNewBound, theNewIndex);
	    ExpressionVertex& theNewPlus=effectiveBoundExpression.addBinaryOpByName("plus_scal_scal", theNewMinus, *theOne_p);
	    if(strideExprP) {
	      if(strideExprP->constIntEval()!=1) {
		ExpressionVertex& theNewStride=strideExprP->copyMyselfInto(effectiveBoundExpression, true, false);
		effectiveBoundExpression.addBinaryOpByName("div_scal_scal", theNewPlus, theNewStride);
	      }

	    }
	    int theBoundVal=effectiveBoundExpression.constIntEval();
	    if(myDimensionBoundsPVector[theDimension-1]
	       &&
	       myDimensionBoundsPVector[theDimension-1]->getUpper()!=theBoundVal) {
	      THROW_LOGICEXCEPTION_MACRO("TemporariesHelper::populateEffectiveDimensionBounds: conflicting bounds ("
					 <<myDimensionBoundsPVector[theDimension]
					 <<" vs. "
					 <<theBoundVal
					 <<" for dimension "
					 <<theDimension);
	    }
	    myDimensionBoundsPVector[theDimension-1]=new DimensionBounds(1, theBoundVal);
	  }
	}
	if(symbolDimensionBoundsP) { 
	  switch(DimensionBounds::getIndexOrder()) {
	  case IndexOrder::ROWMAJOR: // c and c++
	    ++symbolDBIt;
	    aDimensionBounds_p=*symbolDBIt;
	    break;
	  case IndexOrder::COLUMNMAJOR:  // fortran
	    ++symbolDBRIt;
	    aDimensionBounds_p=*symbolDBRIt;
	    break;
	  }
	}
      }
    }
    else { // no array access
      if(symbolDimensionBoundsP) {
	unsigned short theDimension=0;
	switch(DimensionBounds::getIndexOrder()) {
	case IndexOrder::ROWMAJOR: // c and c++
	  for(Symbol::DimensionBoundsPList::const_iterator symbolDBIt=symbolDimensionBoundsP->begin();
	     symbolDBIt!=symbolDimensionBoundsP->end();
	     ++symbolDBIt) {
	    ++theDimension; // 1-based counting
	    myDimensionBoundsPVector[theDimension-1]=new DimensionBounds((*symbolDBIt)->getLower(), (*symbolDBIt)->getUpper());
	  }
	  break;
	case IndexOrder::COLUMNMAJOR:  // fortran
	  for(Symbol::DimensionBoundsPList::const_reverse_iterator symbolDBIt=symbolDimensionBoundsP->rbegin();
	     symbolDBIt!=symbolDimensionBoundsP->rend();
	     ++symbolDBIt) {
	    ++theDimension; // 1-based counting
	    myDimensionBoundsPVector[theDimension-1]=new DimensionBounds((*symbolDBIt)->getLower(), (*symbolDBIt)->getUpper());
	  }
	  break;
	}
      }
    }
  }

  bool TemporariesHelper::needsAllocation() {
    setTypeInfo();
    unsigned short found=0;
    for(DimensionBoundsPVector::iterator it=myDimensionBoundsPVector.begin();
	it!=myDimensionBoundsPVector.end();
	++it) {
      if(*it)
	++found;
    }
    return !(found==myShape&&found==myDimensionBoundsPVector.size());
  }

  void TemporariesHelper::setTypeInfo() {
    if(!myTypeInfo) {
      if(myTopVertex_p) {
	typeInfo(*myTopVertex_p);
	if (myTopVertex_p->isIntrinsic()) { 
	  switch( (dynamic_cast<const Intrinsic&>(*myTopVertex_p)).getInlinableIntrinsicsCatalogueItem().getShapeChange()) {
	  case ShapeChange::RANK : { 
	    unsigned short myRank=myShape;
	    myShape=SymbolShape::VECTOR;
	    myType=SymbolType::INTEGER_STYPE;
	    myFrontEndType=FrontEndType(); // unset it
	    for(DimensionBoundsPVector::iterator it=myDimensionBoundsPVector.begin();
		it!=myDimensionBoundsPVector.end();
		++it) {
	      if(*it)
		delete (*it);
	    }
	    myDimensionBoundsPVector.resize(1,0);
	    myDimensionBoundsPVector[0]=new DimensionBounds(1, myRank);
	    break; 
	  }
	  case ShapeChange::SCALAR : { 
	    myShape=SymbolShape::SCALAR;
	    myType=SymbolType::INTEGER_STYPE;
	    myFrontEndType=FrontEndType(); // unset it
	    for(DimensionBoundsPVector::iterator it=myDimensionBoundsPVector.begin();
		it!=myDimensionBoundsPVector.end();
		++it) {
	      if(*it)
		delete (*it);
	    }
	    myDimensionBoundsPVector.resize(0,0);
	    break; 
	  }
	  default: 
	    // do nothing
	    break;
	  } 
	}
      }
      else {
	typeInfo(*myVariable_p);
      }
    }
    if(!myTypeInfo) {
      if(DbgLoggerManager::instance()->isSelected(DbgGroup::GRAPHICS)&&DbgLoggerManager::instance()->wantTag("expr"))
	myExpression_p->show("ExpressionForMakeTempSymbol");
      THROW_LOGICEXCEPTION_MACRO("TemporariesHelper::setTypeInfo: no type info found");
    }
  }

  const Variable& TemporariesHelper::allocationModel() { 
    setTypeInfo();
    if(!myAllocationModel_p) {
      THROW_LOGICEXCEPTION_MACRO("TemporariesHelper::allocationModel: no allocation model found");
    }
    return *myAllocationModel_p;
  } 

}
