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

#include "xaifBooster/utils/inc/DbgLoggerManager.hpp"

#include "xaifBooster/system/inc/Assignment.hpp"
#include "xaifBooster/system/inc/Argument.hpp"
#include "xaifBooster/system/inc/AliasMap.hpp"
#include "xaifBooster/system/inc/ConceptuallyStaticInstances.hpp"
#include "xaifBooster/system/inc/CallGraph.hpp"
#include "xaifBooster/system/inc/VariableSymbolReference.hpp"
#include "xaifBooster/system/inc/InlinableIntrinsicsCatalogueItem.hpp"
#include "xaifBooster/system/inc/Intrinsic.hpp"
#include "xaifBooster/system/inc/InlinableIntrinsicsExpression.hpp"
#include "xaifBooster/system/inc/InlinableIntrinsicsExpressionVertex.hpp"
#include "xaifBooster/system/inc/GraphVizDisplay.hpp"

#include "xaifBooster/algorithms/CodeReplacement/inc/ConceptuallyStaticInstances.hpp"

#include "xaifBooster/algorithms/InlinableXMLRepresentation/inc/InlinableSubroutineCall.hpp"

#include "xaifBooster/algorithms/TraceDiff/inc/AssignmentAlg.hpp"
#include "xaifBooster/algorithms/TraceDiff/inc/Helpers.hpp"

namespace xaifBoosterTraceDiff { 

  AssignmentAlg::AssignmentAlg(Assignment& theContainingAssignment) : 
    AssignmentAlgBase(theContainingAssignment) { 
  }

  std::string AssignmentAlg::debug() const { 
    std::ostringstream out;
    out << "xaifBoosterTraceDiff::AssignmentAlg[" 
	<< this 
	<< "]" << std::ends;  
    return out.str();
  } // end of AssignmentAlg::debug

  void
  AssignmentAlg::printXMLHierarchy(std::ostream& os) const { 
    if (xaifBoosterCodeReplacement::ConceptuallyStaticInstances::instance()->getPrintVersion()
	!=
	xaifBoosterCodeReplacement::PrintVersion::ORIGINAL) { 
      if (!myTracingCalls.empty()) { 
	for (PlainBasicBlock::BasicBlockElementList::const_iterator li=myTracingCalls.begin();
	     li!=myTracingCalls.end();
	     ++li) { 
	  (*li)->printXMLHierarchy(os);
	}
      }
    }
    getContaining().printXMLHierarchyImpl(os);
  }

  void 
  AssignmentAlg::traverseToChildren(const GenericAction::GenericAction_E anAction_c) { 
  } 

  void 
  AssignmentAlg::trace() { 
    const Expression& theRHS=getContainingAssignment().getRHS();
    Expression::ConstVertexIteratorPair p(theRHS.vertices());
    Expression::ConstVertexIterator it(p.first),endIt(p.second);
    for (;it!=endIt ;++it) {
      if (theRHS.numInEdgesOf(*it)>0 && (*it).isIntrinsic()) {
        const Intrinsic& theIntrinsic(dynamic_cast<const Intrinsic&>(*it));
	if (theIntrinsic.isNonSmooth()) {
	  // trace the  call: 
	  xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall* theIntrinsicCall_p;
	  theIntrinsicCall_p=new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall("oad_trace_call");
	  theIntrinsicCall_p->setId("TraceDiff::AssignmentAlg::trace");
	  myTracingCalls.push_back(theIntrinsicCall_p);
	  ConcreteArgument& theName(theIntrinsicCall_p->addConcreteArgument(1));
	  theName.makeConstant(SymbolType::STRING_STYPE);
	  theName.getConstant().setFromString(theIntrinsic.getName());
	  ConcreteArgument& theOriginalLine(theIntrinsicCall_p->addConcreteArgument(2));
	  theOriginalLine.makeConstant(SymbolType::INTEGER_STYPE);
	  theOriginalLine.getConstant().setint(getContainingAssignment().getLineNumber());
	  Expression aSubExpression;
	  theRHS.copySubExpressionInto(aSubExpression,*it,false,false);
	  Expression::CArgumentPList activeArgPList;
	  aSubExpression.appendActiveArguments(activeArgPList);
	  for(Expression::CArgumentPList::const_iterator argIt=activeArgPList.begin();
	      argIt!=activeArgPList.end();
	      ++argIt) { 
	    Helpers::traceArgument(*((*argIt).first),
				   myTracingCalls,
                                   ConceptuallyStaticInstances::instance()->getTraversalStack().getCurrentBasicBlockInstance().getScope());
	  }
	  theIntrinsicCall_p=new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall("oad_trace_ecall");
	  theIntrinsicCall_p->setId("TraceDiff::SubroutineCallAlg::algorithm_action_2");
	  myTracingCalls.push_back(theIntrinsicCall_p);
	}
      }
    }
  }
}
