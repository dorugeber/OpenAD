// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/ConcreteArgument.hpp"
#include "xaifBooster/system/inc/VariableSymbolReference.hpp"
#include "xaifBooster/system/inc/ArrayAccess.hpp"
#include "xaifBooster/system/inc/Assignment.hpp"

#include "xaifBooster/algorithms/InlinableXMLRepresentation/inc/InlinableSubroutineCall.hpp"

#include "xaifBooster/algorithms/TraceDiff/inc/Helpers.hpp"

namespace xaifBoosterTraceDiff {  
  
  bool Helpers::argumentsHaveArrayAccess(const Expression::CArgumentPList& arguments) {
    for (Expression::CArgumentPList::const_iterator i=arguments.begin();
	 i!=arguments.end();
	 ++i) { 
      if ((*i).first->getVariable().hasArrayAccess())
	return true; 
    }
    return false; 
  }

  void Helpers::traceArguments(const Expression::CArgumentPList& arguments,
			       BasicBlock& aBasicBlock) { 
    for (Expression::CArgumentPList::const_iterator i=arguments.begin();
	 i!=arguments.end();
	 ++i) { 
      PlainBasicBlock::BasicBlockElementList aList;
      traceArgument(*((*i).first), aList,aBasicBlock.getScope());
      for (PlainBasicBlock::BasicBlockElementList::iterator i=aList.begin();
	   i!=aList.end();
	   ++i) { 
	aBasicBlock.supplyAndAddBasicBlockElementInstance(**i);
      }
    }
  } 

  void Helpers::traceArgument(const Argument& anArgument,
			      PlainBasicBlock::BasicBlockElementList& aTraceList,
			      Scope& aScope) { 
    // if there are no array indices we can just look at the line in question
    if (anArgument.getVariable().hasArrayAccess()) {
      xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall* theSubroutineCall_p=
	new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall("oad_trace_arg");
      theSubroutineCall_p->setId("TraceDiff::Helpers::traceArgument");
      aTraceList.push_back(theSubroutineCall_p);
      ConcreteArgument& theName(theSubroutineCall_p->addConcreteArgument(1));
      theName.makeConstant(SymbolType::STRING_STYPE);
      theName.getConstant().setFromString(anArgument.getVariable().getVariableSymbolReference().getSymbol().plainName());
      handleArrayAccessIndices(anArgument,
			       aTraceList,
			       aScope); 
      theSubroutineCall_p=new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall("oad_trace_earg");
      theSubroutineCall_p->setId("TraceDiff::SubroutineCallAlg::algorithm_action_2");
      aTraceList.push_back(theSubroutineCall_p);
    }
  } 

  void Helpers::handleArrayAccessIndices(const Argument& anArgument,
					 PlainBasicBlock::BasicBlockElementList& aTraceList,
					 Scope& aScope) { 
    const ArrayAccess::IndexTripletListType& theIndexTripletList(anArgument.getVariable().getArrayAccess().getIndexTripletList());
    for (ArrayAccess::IndexTripletListType::const_iterator anIndexTripletListTypeCI=theIndexTripletList.begin();
	 anIndexTripletListTypeCI!=theIndexTripletList.end();
	 ++anIndexTripletListTypeCI) { 
      for (IndexTriplet::IndexPairList::const_iterator anIndexPairListCI=(*anIndexTripletListTypeCI)->getIndexPairList().begin();
	   anIndexPairListCI!=(*anIndexTripletListTypeCI)->getIndexPairList().end();
	   ++anIndexPairListCI) { 
	xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall* theSubroutineCall_p(new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall("oad_trace_index"));
	theSubroutineCall_p->setId("TraceDiff::Helpers::handleArrayAccessIndices");
	// now we have two cases, essentially the expression is a single vertex with a constant 
	const Expression& theIndexExpression(*((*anIndexPairListCI).second));
	const Variable* theIndex_p=0;
	if (theIndexExpression.numVertices()==1) { 
	  if (!(*(theIndexExpression.vertices().first)).isArgument()) { 
	    // this must be a constant
	    ConcreteArgument& theIndexConstant(theSubroutineCall_p->addConcreteArgument(1));
	    theIndexConstant.makeConstant(SymbolType::INTEGER_STYPE);
	    theIndexConstant.getConstant().setint(dynamic_cast<const Constant&>(*(theIndexExpression.vertices().first)).getint());
	  }
	  else {  // is not a constant
	    theIndex_p=&(dynamic_cast<const Argument&>(*(theIndexExpression.vertices().first)).getVariable());
	  }
	}
	else { // is an actual expression we should save
	  Assignment* theIndexExpressionAssignment_p(new Assignment(false));
	  // save it in the list
	  aTraceList.push_back(theIndexExpressionAssignment_p);
	  theIndexExpressionAssignment_p->setId("index_expression_assignment_for_tracing");
	  // create a new symbol and add a new VariableSymbolReference in the Variable
	  VariableSymbolReference* theNewVariableSymbolReference_p=
	    new VariableSymbolReference(aScope.getSymbolTable().
					addUniqueAuxSymbol(SymbolKind::VARIABLE,
							   SymbolType::INTEGER_STYPE,
							   SymbolShape::SCALAR,
							   false),
					aScope);
	  theNewVariableSymbolReference_p->setId("1");
	  theNewVariableSymbolReference_p->setAnnotation("TraceDiff::Helpers::handleArrayAccessIndices");
	  // pass it on to the LHS and relinquish ownership
	  theIndexExpressionAssignment_p->getLHS().supplyAndAddVertexInstance(*theNewVariableSymbolReference_p);
	  theIndexExpressionAssignment_p->getLHS().getAliasMapKey().setTemporary();
	  theIndexExpressionAssignment_p->getLHS().getDuUdMapKey().setTemporary();
	  theIndex_p=&(theIndexExpressionAssignment_p->getLHS());
	  // set the RHS
	  theIndexExpression.copyMyselfInto(theIndexExpressionAssignment_p->getRHS(),false,false);
	}
	if (theIndex_p) {
	  theIndex_p->copyMyselfInto(theSubroutineCall_p->addConcreteArgument(1).getArgument().getVariable());
	}
	aTraceList.push_back(theSubroutineCall_p);
      }
    }
  }

} // end of namespace 
