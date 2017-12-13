// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/Intrinsic.hpp"

#include "xaifBooster/algorithms/Linearization/inc/ExpressionVertexAlg.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/PrivateLinearizedComputationalGraphVertex.hpp"

namespace xaifBoosterBasicBlockPreaccumulation { 

  PrivateLinearizedComputationalGraphVertex::PrivateLinearizedComputationalGraphVertex() : myOriginatingExpressionVertex_p(NULL),
                                                                                           myOriginatingAssignment_p(NULL),
                                                                                           myOriginalVariable_p(NULL),
                                                                                           myAuxiliaryVariable_p(NULL) {
  }

  std::string
  PrivateLinearizedComputationalGraphVertex::debug() const {
    std::ostringstream out;
    out << "PrivateLinearizedComputationalGraphVertex[" << Vertex::debug().c_str()
        << ",myOriginatingExpressionVertex_p=" << myOriginatingExpressionVertex_p
        << ",myOriginatingAssignment_p=" << myOriginatingAssignment_p;
    out << ",myOriginalVariable_p=";
    if (myOriginalVariable_p)
      out << ">" << myOriginalVariable_p->debug().c_str();
    else
      out << myOriginalVariable_p;
    out << ",myAuxiliaryVariable_p=";
    if (myAuxiliaryVariable_p)
      out << ">" << myAuxiliaryVariable_p->debug().c_str();
    else
      out << myAuxiliaryVariable_p;
    out << ",myOriginalExpressionVertexPSet={";
    for (CExpressionVertexPSet::const_iterator setI = myOriginalExpressionVertexPSet.begin(); setI != myOriginalExpressionVertexPSet.end(); ++setI)
      out << (*setI)->debug().c_str();
    out << "}";
    out << "]" << std::ends;
    return out.str();
  } 

  void
  PrivateLinearizedComputationalGraphVertex::setOrigin(const ExpressionVertex& aExpressionVertex,
                                                       const Assignment& aAssignment) {
    if (myOriginatingExpressionVertex_p)
      THROW_LOGICEXCEPTION_MACRO("PrivateLinearizedComputationalGraphVertex::setOrigin:"
                                 << " already set to " << myOriginatingExpressionVertex_p->debug().c_str()
                                 << " from assignment " << myOriginatingAssignment_p);
    myOriginatingExpressionVertex_p = &aExpressionVertex;
    myOriginatingAssignment_p = &aAssignment;
    // set the original variable
    // we assume here that this is NOT a direct copy assignment (i.e., if the originating EV is an argument, then it is not the maximal RHS EV)
    if (getOriginatingExpressionVertex().isArgument())
      myOriginalVariable_p = &(dynamic_cast<const Argument&>(getOriginatingExpressionVertex()).getVariable());
    // set the auxiliary variable
    const xaifBoosterLinearization::ExpressionVertexAlg& theOriginatingEVAlg(
     dynamic_cast<const xaifBoosterLinearization::ExpressionVertexAlg&>(getOriginatingExpressionVertex().getExpressionVertexAlgBase())
    );
    if (theOriginatingEVAlg.hasAuxiliaryVariable())
      myAuxiliaryVariable_p = &theOriginatingEVAlg.getAuxiliaryVariable();
  }

  void
  PrivateLinearizedComputationalGraphVertex::setOrigin(const Assignment& aAssignment) {
    if (myOriginatingAssignment_p)
      THROW_LOGICEXCEPTION_MACRO("PrivateLinearizedComputationalGraphVertex::setOrigin:"
                                 << " originating assignment already set to " << myOriginatingAssignment_p->debug().c_str());
    myOriginatingAssignment_p = &aAssignment;
    myOriginalVariable_p = &aAssignment.getLHS();
    // used to get auxiliary variable; should be the ONLY RHS vertex
    myOriginatingExpressionVertex_p = &aAssignment.getRHS().getMaxVertex(); // used to get auxiliary variable
  }
                                                       
  const ExpressionVertex&
  PrivateLinearizedComputationalGraphVertex::getOriginatingExpressionVertex() const {
    if (!myOriginatingExpressionVertex_p)
      THROW_LOGICEXCEPTION_MACRO("PrivateLinearizedComputationalGraphVertex::getOriginatingExpressionVertex: myOriginatingExpressionVertex_p not set");
    return *myOriginatingExpressionVertex_p;
  }

  const CExpressionVertexPSet&
  PrivateLinearizedComputationalGraphVertex::getAssociatedExpressionVertexPSet() const {
    return myOriginalExpressionVertexPSet;
  }

  void
  PrivateLinearizedComputationalGraphVertex::associateExpressionVertex(const ExpressionVertex& aExpressionVertex) {
    if (myOriginalExpressionVertexPSet.find(&aExpressionVertex) != myOriginalExpressionVertexPSet.end())
      THROW_LOGICEXCEPTION_MACRO("PrivateLinearizedComputationalGraphVertex::associateExpressionVertex: "
                                 << aExpressionVertex.debug().c_str() << "already associated");
    myOriginalExpressionVertexPSet.insert(&aExpressionVertex);
  }

  bool PrivateLinearizedComputationalGraphVertex::hasOriginalVariable() const {
    return (myOriginalVariable_p) ? true : false;
  } 

  const Variable& PrivateLinearizedComputationalGraphVertex::getOriginalVariable() const {
    if (!myOriginalVariable_p)
      THROW_LOGICEXCEPTION_MACRO("PrivateLinearizedComputationalGraphVertex::getOriginalVariable: not set");
    return *myOriginalVariable_p;
  } 

  const ObjectWithId::Id& PrivateLinearizedComputationalGraphVertex::getStatementId() const {
    if (!myOriginatingAssignment_p)
      THROW_LOGICEXCEPTION_MACRO("PrivateLinearizedComputationalGraphVertex::getStatementId: myOriginatingAssignment_p not set");
    return myOriginatingAssignment_p->getId();
  } 

  bool
  PrivateLinearizedComputationalGraphVertex::hasAuxiliaryVariable() const {
    if (!myOriginatingExpressionVertex_p)
      THROW_LOGICEXCEPTION_MACRO("PrivateLinearizedComputationalGraphVertex::hasAuxiliaryVariable: myOriginatingExpressionVertex_p not set");
    return (myAuxiliaryVariable_p) ? true : false;
  }

  const Variable&
  PrivateLinearizedComputationalGraphVertex::getAuxiliaryVariable() const {
    if (!hasAuxiliaryVariable())
      THROW_LOGICEXCEPTION_MACRO("PrivateLinearizedComputationalGraphVertex::getAuxiliaryVariable: has none");
    return *myAuxiliaryVariable_p;
  }

  std::string
  PrivateLinearizedComputationalGraphVertex::getLabelString() const {
    std::ostringstream out;

    // auxiliary variable
    if (hasAuxiliaryVariable())
      out << getAuxiliaryVariable().getVariableSymbolReference().getSymbol().getId().c_str() << "=";

    // search for a (LHS) variable symbol reference
    if (hasOriginalVariable()) {
      out << getOriginalVariable().getVariableSymbolReference().getSymbol().getId().c_str();
    }

    // print the operation
    if (getOriginatingExpressionVertex().isIntrinsic()) {
      if (hasOriginalVariable())
        out << "=";
      out << dynamic_cast<const Intrinsic&>(getOriginatingExpressionVertex()).getInlinableIntrinsicsCatalogueItem().getFunction().getBuiltinFunctionName();
    }

    // print DuUdMapKey
    if (hasOriginalVariable()) {
      if (getOriginalVariable().getDuUdMapKey().getKind() == InfoMapKey::SET)
        out  << "\\n[k=" << getOriginalVariable().getDuUdMapKey().getKey() << "]";
    }
    return out.str();
  }

} // end namespace xaifBoosterBasicBlockPreaccumulation

