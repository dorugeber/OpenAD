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
#include "xaifBooster/utils/inc/PrintManager.hpp"
#include "xaifBooster/utils/inc/DbgLoggerManager.hpp"

#include "xaifBooster/system/inc/ConceptuallyStaticInstances.hpp"
#include "xaifBooster/system/inc/ForLoop.hpp"
#include "xaifBooster/system/inc/BooleanOperation.hpp"

#include "xaifBooster/algorithms/AdjointUtils/inc/BasicBlockPrintVersion.hpp"

#include "xaifBooster/algorithms/ControlFlowReversal/inc/ReversibleControlFlowGraphVertex.hpp"

using namespace xaifBooster;

namespace xaifBoosterControlFlowReversal { 

  ReversibleControlFlowGraphVertex::ReversibleControlFlowGraphVertex() :
    original(false), 
    adjoint(false), 
    myOriginalVertex_p(0), 
    myNewVertex_p(0),
    myIndex(0), 
    myReversalType(ForLoopReversalType::ANONYMOUS), 
    myCounterPart_p(0),
    myTopExplicitLoop_p(0),
    myTopExplicitLoopAddressArithmetic_p(0),
    myStorePlaceholder_p(0),
    myRestorePlaceholder_p(0),
    myEnclosingControlFlow_p(0) {
  }

  ReversibleControlFlowGraphVertex::ReversibleControlFlowGraphVertex(const ControlFlowGraphVertex* theOriginal) : 
    original(true),
    adjoint(false),
    myOriginalVertex_p(theOriginal),
    myNewVertex_p(0),
    myIndex(0),
    myReversalType(ForLoopReversalType::ANONYMOUS), 
    myCounterPart_p(0),
    myTopExplicitLoop_p(0),
    myTopExplicitLoopAddressArithmetic_p(0),
    myStorePlaceholder_p(0),
    myRestorePlaceholder_p(0),
    myEnclosingControlFlow_p(0) {
  }

  ReversibleControlFlowGraphVertex::~ReversibleControlFlowGraphVertex() {
    if (myNewVertex_p)
      delete myNewVertex_p;
  }

  const ControlFlowGraphVertexAlg&
  ReversibleControlFlowGraphVertex::getOriginalControlFlowGraphVertexAlg() const {
    return dynamic_cast<const ControlFlowGraphVertexAlg&>(myOriginalVertex_p->getControlFlowGraphVertexAlgBase());
  }

  ControlFlowGraphVertexAlg&
  ReversibleControlFlowGraphVertex::getOriginalControlFlowGraphVertexAlg() {
    return dynamic_cast<ControlFlowGraphVertexAlg&>(myOriginalVertex_p->getControlFlowGraphVertexAlgBase());
  }

  const ControlFlowGraphVertexAlg&
  ReversibleControlFlowGraphVertex::getNewControlFlowGraphVertexAlg() const {
    if (!myNewVertex_p)
      THROW_LOGICEXCEPTION_MACRO("ReversibleControlFlowGraphVertex::getNewControlFlowGraphVertexAlg: not set");
    return dynamic_cast<const ControlFlowGraphVertexAlg&>(myNewVertex_p->getControlFlowGraphVertexAlgBase());
  }

  void 
  ReversibleControlFlowGraphVertex::supplyAndAddNewVertex(ControlFlowGraphVertex& theNewVertex) { 
    if (myNewVertex_p)
      THROW_LOGICEXCEPTION_MACRO("ReversibleControlFlowGraphVertex::supplyAndAddNewVertex: already set");
    myNewVertex_p=&theNewVertex; 
  }

  void
  ReversibleControlFlowGraphVertex::printXMLHierarchy(std::ostream& os) const {
    if (!original) { 
      // JU: this is not nice! These things have Alg objects which  when 
      // inherited in a different algorithm override printing which shouldn't 
      // happen here. So we have to forcibly override it. 
      xaifBooster::PrintVersion::PrintVersion_E aPrintVersion(xaifBooster::ConceptuallyStaticInstances::instance()->
							      getPrintVersion());
      xaifBooster::ConceptuallyStaticInstances::instance()->setPrintVersion(xaifBooster::PrintVersion::SYSTEM_ONLY);
      myNewVertex_p->printXMLHierarchy(os);
      xaifBooster::ConceptuallyStaticInstances::instance()->setPrintVersion(aPrintVersion);
    }
    else {
      // pick the right version since the 
      // BasicBlockAlg objects have data for both:
      xaifBoosterAdjointUtils::BasicBlockPrintVersion::set(myReversalType); 
      if (adjoint)
        myOriginalVertex_p->printXMLHierarchy(os);
      else
        myOriginalVertex_p->printXMLHierarchy(os);
      // reset the print version to the default:
      xaifBoosterAdjointUtils::BasicBlockPrintVersion::set(ForLoopReversalType::ANONYMOUS); 
    }
  }

  std::string
  ReversibleControlFlowGraphVertex::debug() const {
    std::ostringstream out;
    out << "xaifBoosterControlFlowReversal::ReversibleControlFlowGraphVertex["
	<< this
	<< ",original="
	<< original
	<< ",adjoint="
	<< adjoint
	<< ",myIndex="
	<< myIndex
	<< ",getKind():"
	<< ControlFlowGraphVertexAlg::kindToString(getKind()).c_str()
	<< ",myReversalType="
	<< ForLoopReversalType::toString(myReversalType).c_str()
	<< ",myCounterPart_p="
	<< myCounterPart_p
	<< ",myTopExplicitLoop_p="
	<< myTopExplicitLoop_p
	<< ",myTopExplicitLoopAddressArithmetic_p="
	<< myTopExplicitLoopAddressArithmetic_p
	<< ",myStorePlaceholder_p="
	<< myStorePlaceholder_p
	<< ",myRestorePlaceholder_p="
	<< myRestorePlaceholder_p
	<< ",myEnclosingControlFlow_p="
	<< myEnclosingControlFlow_p
	<< ",myKnownLoopVariables[";
      for (ControlFlowGraphVertex::VariablePList::const_iterator knownListI= myKnownLoopVariables.begin();
	   knownListI!= myKnownLoopVariables.end();
	   ++knownListI) { 
	out << (*knownListI)->debug().c_str();
      }
      out << "]"
	  << "]" << std::ends;
    return out.str();
  }

  void ReversibleControlFlowGraphVertex::traverseToChildren(const GenericAction::GenericAction_E anAction_c) {
  }

  void ReversibleControlFlowGraphVertex::setIndex(int i) {
    myIndex=i;
  } 

  void ReversibleControlFlowGraphVertex::setAdjointIndex(int i) {
    myIndex=-i;
  } 

  int ReversibleControlFlowGraphVertex::getIndex() const {
    return myIndex;
  }
                                                                    
  ControlFlowGraphVertexAlg::ControlFlowGraphVertexKind_E 
  ReversibleControlFlowGraphVertex::getKind() const {
    if (original)
      return getOriginalControlFlowGraphVertexAlg().getKind();
    else
      return getNewControlFlowGraphVertexAlg().getKind();
  }

  void ReversibleControlFlowGraphVertex::setReversalType(ForLoopReversalType::ForLoopReversalType_E aReversalType) { 
    myReversalType=aReversalType;
  }

  ForLoopReversalType::ForLoopReversalType_E ReversibleControlFlowGraphVertex::getReversalType() const { 
    return myReversalType;
  }

  void ReversibleControlFlowGraphVertex::setCounterPart(ReversibleControlFlowGraphVertex& theCounterPart) { 
    if(myCounterPart_p)
      THROW_LOGICEXCEPTION_MACRO("ControlFlowGraphVertexAlg::setCounterPart: already set");
    myCounterPart_p=&theCounterPart;
  }
      
  ReversibleControlFlowGraphVertex& ReversibleControlFlowGraphVertex::getCounterPart() { 
    if (!myCounterPart_p)
      THROW_LOGICEXCEPTION_MACRO("ControlFlowGraphVertexAlg::getCounterPart: not set");
    return *myCounterPart_p;
  }

  const ControlFlowGraphVertex::VariablePList& 
  ReversibleControlFlowGraphVertex::getKnownLoopVariables()const { 
    return myKnownLoopVariables;
  } 
    
  void 
  ReversibleControlFlowGraphVertex::inheritLoopVariables(const ReversibleControlFlowGraphVertex& aParent) {
    const ControlFlowGraphVertex::VariablePList& aParentList(aParent.getKnownLoopVariables());
    if (myKnownLoopVariables.size())
      THROW_LOGICEXCEPTION_MACRO("ReversibleControlFlowGraphVertex::inheritLoopVariables: already inherited once");
    for(ControlFlowGraphVertex::VariablePList::const_iterator i=aParentList.begin();
	i!=aParentList.end();
	++i) { 
      myKnownLoopVariables.push_back(*i);
    }
  } 

  void 
  ReversibleControlFlowGraphVertex::addLoopVariable(const Variable& aLoopVariable) { 
    myKnownLoopVariables.push_back(&aLoopVariable);
  }

  void 
  ReversibleControlFlowGraphVertex::setLoopVariables(const ControlFlowGraphVertex::VariablePList& loopVariables) { 
    myKnownLoopVariables=loopVariables;
  }

  ReversibleControlFlowGraphVertex& 
  ReversibleControlFlowGraphVertex::getTopExplicitLoop() { 
    if (!myTopExplicitLoop_p)
      THROW_LOGICEXCEPTION_MACRO("ReversibleControlFlowGraphVertex::getTopExplicitLoop: not set for "
				 << myOriginalVertex_p->debug().c_str());
    return *myTopExplicitLoop_p;
  } 

  const ReversibleControlFlowGraphVertex& 
  ReversibleControlFlowGraphVertex::getTopExplicitLoop() const { 
    if (!myTopExplicitLoop_p)
      THROW_LOGICEXCEPTION_MACRO("ReversibleControlFlowGraphVertex::getTopExplicitLoop: not set for "
				 << myOriginalVertex_p->debug().c_str());
    return *myTopExplicitLoop_p;
  } 

  void 
  ReversibleControlFlowGraphVertex::setTopExplicitLoop(ReversibleControlFlowGraphVertex& theTopExplicitLoop) { 
    if (!(myTopExplicitLoop_p && myTopExplicitLoop_p!=&theTopExplicitLoop) )
      myTopExplicitLoop_p=&theTopExplicitLoop;	
  } 

  ReversibleControlFlowGraphVertex& 
  ReversibleControlFlowGraphVertex::getTopExplicitLoopAddressArithmetic() { 
    if (!myTopExplicitLoopAddressArithmetic_p)
      THROW_LOGICEXCEPTION_MACRO("ReversibleControlFlowGraphVertex::getTopExplicitLoopAddressArithmetic: not set for "
				 << myOriginalVertex_p->debug().c_str());
    return *myTopExplicitLoopAddressArithmetic_p;
  } 

  void 
  ReversibleControlFlowGraphVertex::setTopExplicitLoopAddressArithmetic(ReversibleControlFlowGraphVertex& theTopExplicitLoopAddressArithmetic) { 
    if (myTopExplicitLoopAddressArithmetic_p)
      THROW_LOGICEXCEPTION_MACRO("ReversibleControlFlowGraphVertex::setTopExplicitLoopAddressArithmetic: already set");
    myTopExplicitLoopAddressArithmetic_p=&theTopExplicitLoopAddressArithmetic;
  } 

  bool
  ReversibleControlFlowGraphVertex::hasStorePlaceholder() const { 
    return (myStorePlaceholder_p!=0);
  } 

  ReversibleControlFlowGraphVertex& 
  ReversibleControlFlowGraphVertex::getStorePlaceholder() { 
    if (!myStorePlaceholder_p)
      THROW_LOGICEXCEPTION_MACRO("ReversibleControlFlowGraphVertex::getStorePlaceholder: not set for "
				 << debug().c_str());
    return *myStorePlaceholder_p;
  } 

  void 
  ReversibleControlFlowGraphVertex::setStorePlaceholder(ReversibleControlFlowGraphVertex& theStorePlaceholder) { 
    if (myStorePlaceholder_p)
      THROW_LOGICEXCEPTION_MACRO("ReversibleControlFlowGraphVertex::setStorePlaceholder: already set")
    myStorePlaceholder_p=&theStorePlaceholder;	
  } 

  bool
  ReversibleControlFlowGraphVertex::hasRestorePlaceholder() const { 
    return (myRestorePlaceholder_p!=0);
  } 

  ReversibleControlFlowGraphVertex& 
  ReversibleControlFlowGraphVertex::getRestorePlaceholder() { 
    if (!myRestorePlaceholder_p)
      THROW_LOGICEXCEPTION_MACRO("ReversibleControlFlowGraphVertex::getRestorePlaceholder: not set for "
				 << myOriginalVertex_p->debug().c_str());
    return *myRestorePlaceholder_p;
  } 

  void 
  ReversibleControlFlowGraphVertex::setRestorePlaceholder(ReversibleControlFlowGraphVertex& theRestorePlaceholder) { 
    if (myRestorePlaceholder_p)
      THROW_LOGICEXCEPTION_MACRO("ReversibleControlFlowGraphVertex::setRestorePlaceholder: already set")
    if (!(myRestorePlaceholder_p && myRestorePlaceholder_p!=&theRestorePlaceholder) )
      myRestorePlaceholder_p=&theRestorePlaceholder;	
  } 

  bool
  ReversibleControlFlowGraphVertex::hasEnclosingControlFlow() const { 
    return (myEnclosingControlFlow_p!=0);
  } 

  ReversibleControlFlowGraphVertex& 
  ReversibleControlFlowGraphVertex::getEnclosingControlFlow() { 
    if (!myEnclosingControlFlow_p)
      THROW_LOGICEXCEPTION_MACRO("ReversibleControlFlowGraphVertex::getEnclosingControlFlow: not set for "
				 << myOriginalVertex_p->debug().c_str());
    return *myEnclosingControlFlow_p;
  } 

  void 
  ReversibleControlFlowGraphVertex::setEnclosingControlFlow(ReversibleControlFlowGraphVertex& theEnclosingControlFlow) { 
    if (myEnclosingControlFlow_p)
      THROW_LOGICEXCEPTION_MACRO("ReversibleControlFlowGraphVertex::setEnclosingControlFlow: already set")
    if (!(myEnclosingControlFlow_p && myEnclosingControlFlow_p!=&theEnclosingControlFlow) )
      myEnclosingControlFlow_p=&theEnclosingControlFlow;	
  } 

  ForLoopDirection::ForLoopDirection_E ReversibleControlFlowGraphVertex::simpleCountUp() const { 
    if (myReversalType!=ForLoopReversalType::EXPLICIT) { 
      THROW_LOGICEXCEPTION_MACRO("ReversibleControlFlowGraphVertex::simpleCountUp: the vertex is not explicit");
    } 
    if (getKind()!=ControlFlowGraphVertexAlg::FORLOOP) { 
      THROW_LOGICEXCEPTION_MACRO("ReversibleControlFlowGraphVertex::simpleCountUp: the vertex is a " << ControlFlowGraphVertexAlg::kindToString(getKind()).c_str());
    } 
    ForLoopDirection::ForLoopDirection_E dir=ForLoopDirection::COUNT_UP;
    const ForLoop& theForLoop(dynamic_cast<const ForLoop&>(getOriginalVertex()));
    const Expression& theConditionExpr(theForLoop.getCondition().getExpression());
    const ExpressionVertex& theConditionMaxVertex(theConditionExpr.getMaxVertex());
    const BooleanOperation* theConditionBooleanOperation_p(dynamic_cast<const BooleanOperation*>(&theConditionMaxVertex));
    if (!theConditionBooleanOperation_p) { 
      THROW_LOGICEXCEPTION_MACRO("ReversibleControlFlowGraphVertex::simpleCountUp: could not find loop condition boolean operation");
    }
    switch(theConditionBooleanOperation_p->getType()) { 
    case BooleanOperationType::LESS_THAN_OTYPE :
    case BooleanOperationType::LESS_OR_EQUAL_OTYPE: 
      dir=ForLoopDirection::COUNT_UP;
      break;
    case BooleanOperationType::GREATER_THAN_OTYPE :
    case BooleanOperationType::GREATER_OR_EQUAL_OTYPE: 
      dir=ForLoopDirection::COUNT_DOWN;
      break; 
    case BooleanOperationType::NOT_EQUAL_OTYPE :
      dir=ForLoopDirection::COUNT_UNDECIDED;
      break; 
    default:
      THROW_LOGICEXCEPTION_MACRO("ReversibleControlFlowGraphVertex::simpleCountUp: don't know what to do with operation "
				 << BooleanOperationType::toString(theConditionBooleanOperation_p->getType()));
      break;
    }
    return dir;
  } 

} // end of namespace

