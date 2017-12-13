// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/CallGraph.hpp"
#include "xaifBooster/system/inc/ConceptuallyStaticInstances.hpp"
#include "xaifBooster/system/inc/Scope.hpp"
#include "xaifBooster/system/inc/VariableSymbolReference.hpp"

#include "xaifBooster/algorithms/TypeChange/inc/TemporariesHelper.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/RemainderGraphVertex.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/Sequence.hpp"

namespace xaifBoosterBasicBlockPreaccumulation {

  RemainderGraphVertex::RemainderGraphVertex(const PrivateLinearizedComputationalGraphVertex& aOriginalPLCGVertex) :
   myOriginalPLCGVertex_p(&aOriginalPLCGVertex),
   myPropagationVariable_p(NULL) {
  }

  RemainderGraphVertex::~RemainderGraphVertex() {
    if (myPropagationVariable_p)
      delete myPropagationVariable_p;
  }

  std::string RemainderGraphVertex::debug() const { 
    std::ostringstream out;
    out << "RemainderGraphVertex[" << Vertex::debug().c_str()
        << ",myOriginalPLCGVertex_p=>"<< myOriginalPLCGVertex_p->debug().c_str()
        << ",myPropagationVariable_p=";
    if (myPropagationVariable_p) out << ">" << myPropagationVariable_p->debug().c_str();
    else out << myPropagationVariable_p;
    out << "]" << std::ends;  
    return out.str();
  } 

  std::string
  RemainderGraphVertex::getLabelString() const {
    if (myPropagationVariable_p)
      return myPropagationVariable_p->getVariableSymbolReference().getSymbol().getId();
    else if (getOriginalVertex().hasOriginalVariable())
      return getOriginalVertex().getOriginalVariable().getVariableSymbolReference().getSymbol().getId();
    else
      return "";
  }
  
  const PrivateLinearizedComputationalGraphVertex&
  RemainderGraphVertex::getOriginalVertex() const {
    return *myOriginalPLCGVertex_p;
  }

  void RemainderGraphVertex::replacePropagationVariable(Sequence& theSequence) {
    if (!getOriginalVertex().hasOriginalVariable())
      THROW_LOGICEXCEPTION_MACRO("RemainderGraphVertex::replacePropagationVariable: myOriginalPLCGVertex_p has no original variable");
    Scope& theCurrentCfgScope (ConceptuallyStaticInstances::instance()->getTraversalStack().getCurrentCallGraphVertexInstance().getControlFlowGraph().getScope());
    myPropagationVariable_p  = new Variable();
    xaifBoosterTypeChange::TemporariesHelper aHelper("RemainderGraphVertex::replacePropagationVariable",
						     getOriginalVertex().getOriginalVariable());
    VariableSymbolReference* theVariableSymbolReference_p =
     new VariableSymbolReference(aHelper.makeTempSymbol(theCurrentCfgScope,
                         ConceptuallyStaticInstances::instance()->getPropagationVariableNameCreator(),
                         true),
        theCurrentCfgScope);
    theVariableSymbolReference_p->setId("1");
    theVariableSymbolReference_p->setAnnotation("xaifBoosterBasicBlockPreaccumulation::RemainderGraphVertex::replacePropagationVariable");
    myPropagationVariable_p->supplyAndAddVertexInstance(*theVariableSymbolReference_p);
    myPropagationVariable_p->getAliasMapKey().setTemporary();
    myPropagationVariable_p->getDuUdMapKey().setTemporary();
    if (aHelper.needsAllocation()){
      theSequence.addAllocation(*theVariableSymbolReference_p,
                                getOriginalVertex().getOriginalVariable(),
                                false);
    }
  } 

  void RemainderGraphVertex::createNewPropagationVariable(Sequence& theSequence, const Variable& variableToMatch) {
    Scope& theCurrentCfgScope (ConceptuallyStaticInstances::instance()->getTraversalStack().getCurrentCallGraphVertexInstance().getControlFlowGraph().getScope());
    myPropagationVariable_p  = new Variable();
    xaifBoosterTypeChange::TemporariesHelper aHelper("RemainderGraphVertex::createNewPropagationVariable",
						     variableToMatch);
    VariableSymbolReference* theVariableSymbolReference_p =
      new VariableSymbolReference(aHelper.makeTempSymbol(theCurrentCfgScope,
                         ConceptuallyStaticInstances::instance()->getPropagationVariableNameCreator(),
                         true),
        theCurrentCfgScope);
    theVariableSymbolReference_p->setId("1");
    theVariableSymbolReference_p->setAnnotation("xaifBoosterBasicBlockPreaccumulation::RemainderGraphVertex::createNewPropagationVariable");
    myPropagationVariable_p->supplyAndAddVertexInstance(*theVariableSymbolReference_p);
    myPropagationVariable_p->getAliasMapKey().setTemporary();
    myPropagationVariable_p->getDuUdMapKey().setTemporary();
    if (aHelper.needsAllocation()){
      theSequence.addAllocation(*theVariableSymbolReference_p,variableToMatch,false);
    }
  } 

  bool
  RemainderGraphVertex::hasPropagationVariable() const {
    return (myPropagationVariable_p) ? true
                                     : false;
  }

  const Variable& RemainderGraphVertex::getPropagationVariable() const {
    if (myPropagationVariable_p)
      return *myPropagationVariable_p;
    else if (getOriginalVertex().hasOriginalVariable())
      return getOriginalVertex().getOriginalVariable();
    else
      THROW_LOGICEXCEPTION_MACRO("RemainderGraphVertex::getPropagationVariable: neither myPropagationVariable_p nor myOriginalVariable_p set");
  } 

} 

