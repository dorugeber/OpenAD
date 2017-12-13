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

#include "xaifBooster/system/inc/ExpressionEdge.hpp"
#include "xaifBooster/system/inc/Assignment.hpp"

#include "xaifBooster/algorithms/Linearization/inc/ExpressionEdgeAlg.hpp"

namespace xaifBoosterLinearization { 

  ExpressionEdgeAlg::ExpressionEdgeAlg(ExpressionEdge& theContainingExpressionEdge) : 
    ExpressionEdgeAlgBase(theContainingExpressionEdge),
    myPartialDerivative_p(0),
    myConcretePartialAssignment_p(0),
    myPartialAllocation_p(0),
    myConcreteConstant_p(0),
    myConcretePartialDerivativeKind(PartialDerivativeKind::NONLINEAR) { 
  }

  ExpressionEdgeAlg::~ExpressionEdgeAlg() { 
    if (myConcretePartialAssignment_p) delete myConcretePartialAssignment_p;
    if (myPartialAllocation_p) delete myPartialAllocation_p;
    if (myConcreteConstant_p) delete myConcreteConstant_p;
  }

  std::string ExpressionEdgeAlg::debug () const { 
    std::ostringstream out;
    out << "xaifBoosterLinearization::ExpressionEdgeAlg[" << this
        << ",myConcretePartialDerivativeKind=" << PartialDerivativeKind::toString(myConcretePartialDerivativeKind)
        << ",myPartialDerivative_p=" << myPartialDerivative_p
        << ",myConcretePartialAssignment_p=" << myConcretePartialAssignment_p
        << ",myPartialAllocation_p=" << myPartialAllocation_p
        << ",myConcreteConstant_p=" << myConcreteConstant_p
 	<< "]" << std::ends;
    return out.str();
  } // end of ExpressionEdgeAlg::debug

  void
  ExpressionEdgeAlg::printXMLHierarchy(std::ostream& os) const { 
    getContaining().printXMLHierarchyImpl(os);
  }

  void ExpressionEdgeAlg::traverseToChildren(const GenericAction::GenericAction_E anAction_c) { 
  } 

  bool ExpressionEdgeAlg::hasPartial() const { 
    return (myPartialDerivative_p!=0);
  }

  const InlinableIntrinsicsExpression&  
  ExpressionEdgeAlg::getPartial() const { 
    if (!myPartialDerivative_p)
      THROW_LOGICEXCEPTION_MACRO("ExpressionEdgeAlg::getPartial: not set");
    return *myPartialDerivative_p;
  } 

  void 
  ExpressionEdgeAlg::setPartial(const InlinableIntrinsicsExpression& thePartial) { 
    if (myPartialDerivative_p)
      THROW_LOGICEXCEPTION_MACRO("ExpressionEdgeAlg::setPartial: already set");
    myPartialDerivative_p=&thePartial;
    myConcretePartialDerivativeKind=PartialDerivativeKind::leastDependent(myConcretePartialDerivativeKind, 
									  thePartial.getPartialDerivativeKind());
  } 

  void 
  ExpressionEdgeAlg::addArgumentsConcretizationPair(const ExpressionVertex& aConcreteArgument,
						    const InlinableIntrinsicsExpressionVertex& anAbstractArgument) { 
    // for safety check uniqueness
    for (VertexPairList::iterator it=myConcreteArgumentInstancesList.begin();
	 it!=myConcreteArgumentInstancesList.end();
	 ++it) { 
      if (((*it).first==&aConcreteArgument 
	   &&
	   (*it).second==&anAbstractArgument))
	// both are the same, i.e. an allowed parallel 
	// edge that has already been added
	return;
      if (((*it).first==&aConcreteArgument 
	   ||
	   (*it).second==&anAbstractArgument))
	// if they are not both the same but just one of them matches, 
	THROW_LOGICEXCEPTION_MACRO("ExpressionEdgeAlg::addArgumentsConcretizationPair: ambiguous pairs");
    } // end for
    myConcreteArgumentInstancesList.push_back(VertexPair(&aConcreteArgument,&anAbstractArgument));
  }

  void ExpressionEdgeAlg::makeConcretePartialAssignment() { 
    if (myConcreteConstant_p)
      THROW_LOGICEXCEPTION_MACRO("ExpressionEdgeAlg::makeConcretePartialAssignment: myConcreteConstant_p is already set, and we cannot have both");
    if (myConcretePartialAssignment_p)
      THROW_LOGICEXCEPTION_MACRO("ExpressionEdgeAlg::makeConcretePartialAssignment: already set");
    myConcretePartialAssignment_p=new Assignment(false);
  } 

  Assignment& ExpressionEdgeAlg::getConcretePartialAssignment() { 
    if (!myConcretePartialAssignment_p)
      THROW_LOGICEXCEPTION_MACRO("ExpressionEdgeAlg::getConcretePartialAssignment: not set");
    return *myConcretePartialAssignment_p;
  } 

  const Assignment& ExpressionEdgeAlg::getConcretePartialAssignment() const { 
    if (!myConcretePartialAssignment_p)
      THROW_LOGICEXCEPTION_MACRO("ExpressionEdgeAlg::getConcretePartialAssignment: not set");
    return *myConcretePartialAssignment_p;
  } 

  bool ExpressionEdgeAlg::hasConcretePartialAssignment() const { 
    return (myConcretePartialAssignment_p!=0);
  } 

  bool
  ExpressionEdgeAlg::hasPartialAllocation() const {
    return (myPartialAllocation_p != NULL);
  }

  void
  ExpressionEdgeAlg::makePartialAllocation(xaifBoosterTypeChange::TemporariesHelper& aHelper) {
    if (hasPartialAllocation())
      THROW_LOGICEXCEPTION_MACRO("ExpressionEdgeAlg::makePartialAllocation: already set");
    if (!hasConcretePartialAssignment())
      THROW_LOGICEXCEPTION_MACRO("ExpressionEdgeAlg::makePartialAllocation: no partial assignment");
    myPartialAllocation_p = new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall("oad_AllocateMatching");
    myPartialAllocation_p->setId("xaifBoosterLinearization::ExpressionEdgeAlg::makePartialAllocation");
    myConcretePartialAssignment_p->getLHS().copyMyselfInto(myPartialAllocation_p->addConcreteArgument(1).getArgument().getVariable());
    aHelper.allocationModel().copyMyselfInto(myPartialAllocation_p->addConcreteArgument(2).getArgument().getVariable());
  }

  const xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall& 
  ExpressionEdgeAlg::getPartialAllocation() const { 
    if (!hasPartialAllocation())
      THROW_LOGICEXCEPTION_MACRO("ExpressionEdgeAlg::getPartialAllocation: no partial allocation has been created");
    return *myPartialAllocation_p;
  }

  void
  ExpressionEdgeAlg::setConcreteConstant(const Constant& aConstant) {
    if (myConcretePartialAssignment_p)
      THROW_LOGICEXCEPTION_MACRO("ExpressionEdgeAlg::setConcreteConstant: myConcretePartialAssignment_p is already set, and we cannot have both");
    myConcreteConstant_p = &aConstant;
  } // end ExpressionEdgeAlg::setConcreteConstant()

  const Constant&
  ExpressionEdgeAlg::getConcreteConstant() const {
    if (!myConcreteConstant_p)
      THROW_LOGICEXCEPTION_MACRO("ExpressionEdgeAlg::getConcreteConstant: not set, " <<
                                 "where the PDK is currently set to " << PartialDerivativeKind::toString(myConcretePartialDerivativeKind));
    return *myConcreteConstant_p;
  } // end ExpressionEdgeAlg::getConcreteConstant()

  void ExpressionEdgeAlg::passivate() { 
    myConcretePartialDerivativeKind=PartialDerivativeKind::PASSIVE;
  } 
  
  void ExpressionEdgeAlg::linearate() { 
    myConcretePartialDerivativeKind = PartialDerivativeKind::LINEAR;
  } 
  
  const ExpressionEdgeAlg::VertexPairList& ExpressionEdgeAlg::getConcreteArgumentInstancesList() const { 
    return myConcreteArgumentInstancesList;
  }

  PartialDerivativeKind::PartialDerivativeKind_E ExpressionEdgeAlg::getPartialDerivativeKind() const { 
    return myConcretePartialDerivativeKind;
  } 

  void
  ExpressionEdgeAlg::mapPartialEV2OriginalEV(const ExpressionVertex& aPartialExpressionVertex,
                                             const ExpressionVertex& aOriginalExpressionVertex) {
    myPartialEVP2OriginalEVPMap.addElement(&aPartialExpressionVertex, &aOriginalExpressionVertex);
  }

  const ExpressionVertex&
  ExpressionEdgeAlg::getOriginalExpressionVertex4ConcretePartialArgument(const ExpressionVertex& aConcretePartialEV) const {
    if (!myPartialEVP2OriginalEVPMap.hasElement(&aConcretePartialEV))
      THROW_LOGICEXCEPTION_MACRO("ExpressionEdgeAlg::getOriginalExpressionVertex4ConcretePartialArgument: " <<
                                 "no original ExpressionVertex associated with " << aConcretePartialEV.debug().c_str());
    return *(myPartialEVP2OriginalEVPMap.getElement(&aConcretePartialEV));
  }

}
