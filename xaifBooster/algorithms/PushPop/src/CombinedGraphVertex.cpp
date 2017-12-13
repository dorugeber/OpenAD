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

#include "xaifBooster/utils/inc/LogicException.hpp"

#include "xaifBooster/system/inc/ExpressionVertex.hpp"

#include "xaifBooster/algorithms/Linearization/inc/ExpressionEdgeAlg.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/PrivateLinearizedComputationalGraphVertex.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/RemainderGraphVertex.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/RemainderGraphEdge.hpp"

#include "xaifBooster/algorithms/CrossCountryInterface/inc/AccumulationGraphVertex.hpp"

#include "xaifBooster/algorithms/PushPop/inc/CombinedGraphVertex.hpp"

namespace xaifBoosterPushPop {

  CombinedGraphVertex::CombinedGraphVertex() :
   myLCGVertex_p(NULL),
   myLinearizationExpressionIntrinsic_p(NULL),
   myLinearizationExpressionConstant_p(NULL),
   myLinearizedExpressionEdgeAlg_p(NULL),
   myRemainderGraphVertex_p(NULL) {
  }

  std::string
  CombinedGraphVertex::debug() const { 
    std::ostringstream out;
    out << "CombinedGraphVertex[" << Vertex::debug().c_str()
        << ",myLCGVertex_p=" << myLCGVertex_p
        << ",myLinearizationExpressionIntrinsic_p=" << myLinearizationExpressionIntrinsic_p;
    // myLinearizationExpressionArguments
    out << ",myLinearizationExpressionArgumentPSet={";
    for (CArgumentPSet::const_iterator argI = myLinearizationExpressionArgumentPSet.begin(); argI != myLinearizationExpressionArgumentPSet.end(); ++argI)
      out << (*argI)->debug().c_str() << ",";
    out << "}";
    out << ",myLinearizationExpressionConstant_p=" << myLinearizationExpressionConstant_p
        << ",myLinearizedExpressionEdgeAlg_p=" << myLinearizedExpressionEdgeAlg_p
        << ",myRemainderGraphVertex_p=" << myRemainderGraphVertex_p;
    // myRemainderGraphEdges
    out << ",myRemainderGraphEdgePSet={";
    for (xaifBoosterBasicBlockPreaccumulation::CRemainderGraphEdgePSet::const_iterator rgeI = myRemainderGraphEdgePSet.begin(); rgeI != myRemainderGraphEdgePSet.end(); ++rgeI)
      out << (*rgeI)->debug().c_str() << ",";
    out << "}";
    // LHSvariables
    out << ",myLHSCVariablePSet={";
    for (CVariablePSet::const_iterator setI = myLHSCVariablePSet.begin(); setI != myLHSCVariablePSet.end(); ++setI)
      out << (*setI)->debug().c_str() << ",";
    out << "}";
    out << "]" << std::ends;
    return out.str();
  }

  void
  CombinedGraphVertex::addLHSVariable(const Variable& aVariable) {
  //if (myLHSCVariablePSet.find(&aVariable) != myLHSCVariablePSet.end())
  //  THROW_LOGICEXCEPTION_MACRO("CombinedGraphVertex::addLHSVariable: " << aVariable.debug().c_str() << " already in set");
    myLHSCVariablePSet.insert(&aVariable);
  }

  const CVariablePSet&
  CombinedGraphVertex::getLHSCVariablePSet() const {
    return myLHSCVariablePSet;
  }

  const bool
  CombinedGraphVertex::hasComputationalGraphVertex() const {
    return (myLCGVertex_p) ? true : false;
  }

  const xaifBoosterBasicBlockPreaccumulation::PrivateLinearizedComputationalGraphVertex&
  CombinedGraphVertex::getComputationalGraphVertex() const {
    if (!hasComputationalGraphVertex())
      THROW_LOGICEXCEPTION_MACRO("CombinedGraphVertex::getComputationalGraphVertex: has no such vertex");
    return *myLCGVertex_p;
  }

  void
  CombinedGraphVertex::setComputationalGraphVertex(const xaifBoosterBasicBlockPreaccumulation::PrivateLinearizedComputationalGraphVertex& aPLCGVertex) {
    if (myLCGVertex_p)
      THROW_LOGICEXCEPTION_MACRO("CombinedGraphVertex::setComputationalGraphVertex:"
                                 << " already set to " << myLCGVertex_p
                                 << " while trying to set to " << &aPLCGVertex);
    myLCGVertex_p = &aPLCGVertex;
  }

  const bool
  CombinedGraphVertex::hasLinearizationExpressionConstant() const {
    return (myLinearizationExpressionConstant_p) ? true : false;
  }

  const bool
  CombinedGraphVertex::hasLinearizationExpressionIntrinsic() const {
    return (myLinearizationExpressionIntrinsic_p) ? true : false;
  }

  const bool
  CombinedGraphVertex::hasLinearizationExpressionArgument() const {
    return (!myLinearizationExpressionArgumentPSet.empty()) ? true : false;
  }

  const bool
  CombinedGraphVertex::hasLinearizationExpressionVertex() const {
    return (hasLinearizationExpressionConstant()
         || hasLinearizationExpressionIntrinsic()
         || hasLinearizationExpressionArgument());
  }

  void
  CombinedGraphVertex::associateLinearizationExpressionVertex(const ExpressionVertex& aExpressionVertex) {
    if (hasLinearizationExpressionConstant())
      THROW_LOGICEXCEPTION_MACRO("CombinedGraphVertex::associateLinearizationExpressionVertex: conflicts with previously associated constant " << myLinearizationExpressionConstant_p->debug().c_str());
    if (hasLinearizationExpressionIntrinsic())
      THROW_LOGICEXCEPTION_MACRO("CombinedGraphVertex::associateLinearizationExpressionVertex: conflicts with previously associated intrinsic " << myLinearizationExpressionIntrinsic_p->debug().c_str());

    if (aExpressionVertex.isIntrinsic())
      myLinearizationExpressionIntrinsic_p = dynamic_cast<const Intrinsic*>(&aExpressionVertex);
    else if (aExpressionVertex.isArgument())
      myLinearizationExpressionArgumentPSet.insert(dynamic_cast<const Argument*>(&aExpressionVertex));
    else // otherwise it's a constant
      myLinearizationExpressionConstant_p = dynamic_cast<const Constant*>(&aExpressionVertex);
  }

  const bool
  CombinedGraphVertex::hasLinearizedExpressionEdgeAlg() const {
    return (myLinearizedExpressionEdgeAlg_p) ? true : false;
  }

  void
  CombinedGraphVertex::setLinearizedExpressionEdgeAlg(const xaifBoosterLinearization::ExpressionEdgeAlg& aLinEEAlg) {
    if (hasComputationalGraphVertex())
      THROW_LOGICEXCEPTION_MACRO("CombinedGraphVertex::setLinearizedExpressionEdgeAlg: already associated with a LCG vertex");
    if (hasLinearizationExpressionVertex())
      THROW_LOGICEXCEPTION_MACRO("CombinedGraphVertex::setLinearizedExpressionEdgeAlg: already associated with a Linearization Expression vertex");
    if (hasLinearizedExpressionEdgeAlg())
      THROW_LOGICEXCEPTION_MACRO("CombinedGraphVertex::setLinearizedExpressionEdgeAlg: already set");
    myLinearizedExpressionEdgeAlg_p = &aLinEEAlg;
  }

  const xaifBoosterLinearization::ExpressionEdgeAlg&
  CombinedGraphVertex::getLinearizedExpressionEdgeAlg() const {
    if (!hasLinearizedExpressionEdgeAlg())
      THROW_LOGICEXCEPTION_MACRO("CombinedGraphVertex::getLinearizedExpressionEdgeAlg: not set");
    return *myLinearizedExpressionEdgeAlg_p;
  }

  const bool
  CombinedGraphVertex::hasAccumulationGraphVertex() const {
    return (!myAccumulationGraphVertexPSet.empty()) ? true : false;
  }

  void
  CombinedGraphVertex::associateAccumulationGraphVertex(const xaifBoosterCrossCountryInterface::AccumulationGraphVertex& aAccumulationGraphVertex) {
    myAccumulationGraphVertexPSet.insert(&aAccumulationGraphVertex);
    if (aAccumulationGraphVertex.hasLHSVariable())
      addLHSVariable(aAccumulationGraphVertex.getLHSVariable());
  }

  const xaifBoosterCrossCountryInterface::AccumulationGraphVertex&
  CombinedGraphVertex::getAccumulationGraphVertex() const {
    if (!hasAccumulationGraphVertex())
      THROW_LOGICEXCEPTION_MACRO("CombinedGraphVertex::getAccumulationGraphVertex: has none");
    // \TODO FIXME: revisit this
    return **myAccumulationGraphVertexPSet.begin();
  }

  const bool
  CombinedGraphVertex::hasRemainderGraphVertex() const {
    return (myRemainderGraphVertex_p) ? true : false;
  }

  void
  CombinedGraphVertex::setRemainderGraphVertex(const xaifBoosterBasicBlockPreaccumulation::RemainderGraphVertex& aRGV) {
    if (hasRemainderGraphVertex())
      THROW_LOGICEXCEPTION_MACRO("CombinedGraphVertex::setRemainderGraphVertex: already set to " << myRemainderGraphVertex_p << " while trying to set to " << &aRGV);
    myRemainderGraphVertex_p = &aRGV;
  }

  const xaifBoosterBasicBlockPreaccumulation::RemainderGraphVertex&
  CombinedGraphVertex::getRemainderGraphVertex() const {
    if (!hasRemainderGraphVertex())
      THROW_LOGICEXCEPTION_MACRO("CombinedGraphVertex::getRemainderGraphVertex: not set");
    return *myRemainderGraphVertex_p;
  }

  const bool
  CombinedGraphVertex::hasRemainderGraphEdge() const {
    return (!myRemainderGraphEdgePSet.empty()) ? true : false;
  }

  void
  CombinedGraphVertex::associateRemainderGraphEdge(const xaifBoosterBasicBlockPreaccumulation::RemainderGraphEdge& aRGE) {
    myRemainderGraphEdgePSet.insert(&aRGE);
  }

  std::string
  CombinedGraphVertex::getShapeString() const {
    if (hasComputationalGraphVertex())
      return "ellipse"; //getComputationalGraphVertex().getShapeString();
    else if (hasLinearizationExpressionVertex() || hasLinearizedExpressionEdgeAlg())
      return "diamond";
    else
      return "box";
  }

  std::string
  CombinedGraphVertex::getColorString() const {
    if (hasComputationalGraphVertex())
      return "black";
    else if (hasAccumulationGraphVertex())
      return getAccumulationGraphVertex().getColorString();
    else if (hasLinearizationExpressionConstant())
      return "blue";
    else if (hasLinearizationExpressionArgument())
      return "black";
    else
      return "orange";
  }

  std::string
  CombinedGraphVertex::getLabelString() const {
    if (hasLinearizedExpressionEdgeAlg()) { // trivial partial expression
      if (getLinearizedExpressionEdgeAlg().getPartialDerivativeKind() == PartialDerivativeKind::LINEAR_ONE)
        return "1";
      else if (getLinearizedExpressionEdgeAlg().getPartialDerivativeKind() == PartialDerivativeKind::LINEAR_MINUS_ONE)
        return "-1";
      else
        THROW_LOGICEXCEPTION_MACRO("CombinedGraphVertex::getLabelString: trivial partial expression has invalid PDK");
    }
    else {
      std::ostringstream out;
      // LHS Variables (formerly included propagation variables)
      for (CVariablePSet::const_iterator setI = getLHSCVariablePSet().begin(); setI != getLHSCVariablePSet().end(); ++setI)
        out << (*setI)->getVariableSymbolReference().getSymbol().getId().c_str() << "=";

      if (hasComputationalGraphVertex())
        out << getComputationalGraphVertex().getLabelString().c_str();
      else if (hasLinearizationExpressionIntrinsic())
        out << myLinearizationExpressionIntrinsic_p->getName();
      else if (hasLinearizationExpressionConstant())
        out << myLinearizationExpressionConstant_p->toString();
      else if (hasLinearizationExpressionArgument()) {
        for (CArgumentPSet::const_iterator argI = myLinearizationExpressionArgumentPSet.begin(); argI != myLinearizationExpressionArgumentPSet.end(); ++argI)
          out << (*argI)->getVariable().getVariableSymbolReference().getSymbol().getId().c_str();
      }
      else if (hasAccumulationGraphVertex()) {
        out << getAccumulationGraphVertex().getRHSString();
      }
      else
        THROW_LOGICEXCEPTION_MACRO("CombinedGraphVertex::getLabelString: could not determine what type of vertex this is");

      return out.str();
    }
  }

}

