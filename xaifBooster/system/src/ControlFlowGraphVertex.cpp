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
#include "xaifBooster/system/inc/ControlFlowGraphVertex.hpp"
#include "xaifBooster/system/inc/ForLoopReversalType.hpp"
#include "xaifBooster/system/inc/ControlFlowGraphVertexAlgFactory.hpp"

namespace xaifBooster { 

  ControlFlowGraphVertex::ControlFlowGraphVertex() : 
    myControlFlowGraphVertexAlgBase_p(0), 
    myReversalType(ForLoopReversalType::ANONYMOUS),
    myIndex(0),
    myCounterPart_p(0),
    myTopExplicitLoop_p(0),
    myEnclosingControlFlow_p(0) {
  }

  ControlFlowGraphVertex::~ControlFlowGraphVertex() {
    if (myControlFlowGraphVertexAlgBase_p) delete myControlFlowGraphVertexAlgBase_p;
  }


  void
  ControlFlowGraphVertex::printXMLHierarchy(std::ostream& os) const { 
  } // end of ControlFlowGraphVertex::printXMLHierarchy

  void
  ControlFlowGraphVertex::printXMLHierarchyImpl(std::ostream& os) const { 
  } // end of ControlFlowGraphVertex::printXMLHierarchyImpl

  std::string ControlFlowGraphVertex::debug () const { 
    std::ostringstream out;
    out << "ControlFlowGraphVertex[" << this 
	<< Vertex::debug().c_str()
	<< ObjectWithId::debug().c_str()
	<< ObjectWithAnnotation::debug().c_str()
	<< "]" << std::ends;  
    return out.str();
  } // end of ControlFlowGraphVertex::debug

  ControlFlowGraphVertexAlgBase&
  ControlFlowGraphVertex::getControlFlowGraphVertexAlgBase() const {
    if (!myControlFlowGraphVertexAlgBase_p)
      THROW_LOGICEXCEPTION_MACRO("ControlFlowGraphVertex::getControlFlowGraphVertexAlgBase: not set");
    return *myControlFlowGraphVertexAlgBase_p;
  }

  bool ControlFlowGraphVertex::hasStatement(const ObjectWithId::Id& aStatementId) const { 
    return false;
  }

  bool
  ControlFlowGraphVertex::hasExpression(const Expression& anExpression) const {
    if (myControlFlowGraphVertexAlgBase_p)
      return myControlFlowGraphVertexAlgBase_p->hasExpression(anExpression);
    else
      return false;
  } // end ControlFlowGraphVertex::containsExpression()

  FindDefinitionResult ControlFlowGraphVertex::findDefinition(const ObjectWithId::Id& aStatementId) const { 
    return FindDefinitionResult(0);
  }

  void ControlFlowGraphVertex::setIndex(int i) {
    myIndex=i;
  } 

  int ControlFlowGraphVertex::getIndex() const {
    return myIndex;
  }
  
  void ControlFlowGraphVertex::setReversalType(ForLoopReversalType::ForLoopReversalType_E aReversalType) { 
    myReversalType=aReversalType;
  }

  void ControlFlowGraphVertex::setCounterPart(ControlFlowGraphVertex& theCounterPart) { 
    if(myCounterPart_p)
      THROW_LOGICEXCEPTION_MACRO("ControlFlowGraphVertex::setCounterPart: already set");
    myCounterPart_p=&theCounterPart;
  }
      
  ControlFlowGraphVertex& ControlFlowGraphVertex::getCounterPart() { 
    if (!myCounterPart_p)
      THROW_LOGICEXCEPTION_MACRO("ControlFlowGraphVertex::getCounterPart: not set");
    return *myCounterPart_p;
  }

  ControlFlowGraphVertex& ControlFlowGraphVertex::getCounterPart() const { 
    if (!myCounterPart_p)
      THROW_LOGICEXCEPTION_MACRO("ControlFlowGraphVertex::getCounterPart: not set");
    return *myCounterPart_p;
  }

  const ControlFlowGraphVertex::VariablePList& 
  ControlFlowGraphVertex::getKnownLoopVariables()const { 
    return myKnownLoopVariables;
  } 
    
  void 
  ControlFlowGraphVertex::inheritLoopVariables(const ControlFlowGraphVertex& aParent) {
    const ControlFlowGraphVertex::VariablePList& aParentList(aParent.getKnownLoopVariables());
    if (myKnownLoopVariables.size())
      THROW_LOGICEXCEPTION_MACRO("ControlFlowGraphVertex::inheritLoopVariables: already inherited once");
    for(VariablePList::const_iterator i=aParentList.begin();
	i!=aParentList.end();
	++i) { 
      myKnownLoopVariables.push_back(*i);
    }
  } 

  bool
  ControlFlowGraphVertex::hasTopExplicitLoop() const { 
    return (myTopExplicitLoop_p!=0);
  } 

  ControlFlowGraphVertex& 
  ControlFlowGraphVertex::getTopExplicitLoop() { 
    if (!myTopExplicitLoop_p)
      THROW_LOGICEXCEPTION_MACRO("ControlFlowGraphVertex::getTopExplicitLoop: not set for "
				 << debug().c_str());
    return *myTopExplicitLoop_p;
  } 

  const ControlFlowGraphVertex& 
  ControlFlowGraphVertex::getTopExplicitLoop() const { 
    if (!myTopExplicitLoop_p)
      THROW_LOGICEXCEPTION_MACRO("ControlFlowGraphVertex::getTopExplicitLoop: not set for "
				 << debug().c_str());
    return *myTopExplicitLoop_p;
  } 

  void 
  ControlFlowGraphVertex::setTopExplicitLoop(ControlFlowGraphVertex& theTopExplicitLoop) { 
    if (!(myTopExplicitLoop_p && myTopExplicitLoop_p!=&theTopExplicitLoop) )
      myTopExplicitLoop_p=&theTopExplicitLoop;	
  } 

  bool
  ControlFlowGraphVertex::hasEnclosingControlFlow() const { 
    return (myEnclosingControlFlow_p!=0);
  } 

  ControlFlowGraphVertex& 
  ControlFlowGraphVertex::getEnclosingControlFlow() { 
    if (!myEnclosingControlFlow_p)
      THROW_LOGICEXCEPTION_MACRO("ControlFlowGraphVertex::getEnclosingControlFlow: not set for "
				 << debug().c_str());
    return *myEnclosingControlFlow_p;
  } 

  const ControlFlowGraphVertex& 
  ControlFlowGraphVertex::getEnclosingControlFlow() const { 
    if (!myEnclosingControlFlow_p)
      THROW_LOGICEXCEPTION_MACRO("ControlFlowGraphVertex::getEnclosingControlFlow: not set for "
				 << debug().c_str());
    return *myEnclosingControlFlow_p;
  } 

  void 
  ControlFlowGraphVertex::setEnclosingControlFlow(ControlFlowGraphVertex& theEnclosingControlFlow) { 
    if (myEnclosingControlFlow_p)
      THROW_LOGICEXCEPTION_MACRO("ControlFlowGraphVertex::setEnclosingControlFlow: already set")
    if (!(myEnclosingControlFlow_p && myEnclosingControlFlow_p!=&theEnclosingControlFlow) )
      myEnclosingControlFlow_p=&theEnclosingControlFlow;	
  } 

  ForLoopReversalType::ForLoopReversalType_E ControlFlowGraphVertex::getReversalType() const { 
    return myReversalType;
  }

} // end of namespace xaifBooster 
