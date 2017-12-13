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

#include "xaifBooster/system/inc/VariableSymbolReference.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/PrivateLinearizedComputationalGraphEdge.hpp"

using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulation { 

  PrivateLinearizedComputationalGraphEdge::PrivateLinearizedComputationalGraphEdge() : 
    myLinearizedExpressionEdge_p(0),
    myDirectCopyEdgeFlag(false) {
  }

  std::string PrivateLinearizedComputationalGraphEdge::debug() const {
    std::ostringstream out;
    out << "PrivateLinearizedComputationalGraphEdge[" << Edge::debug().c_str()
        << ",myDirectCopyEdgeFlag=" << myDirectCopyEdgeFlag
        << ",myLinearizedExpressionEdge_p=" << myLinearizedExpressionEdge_p
        << ",myParallelEdges.size()=" << myParallelEdges.size()
        << "]" << std::ends;
    return out.str();
  } 

  std::string
  PrivateLinearizedComputationalGraphEdge::getLabelString() const {
    if (hasLinearizedExpressionEdge()) {
      switch(getLinearizedExpressionEdgeAlg().getPartialDerivativeKind()) {
        case PartialDerivativeKind::LINEAR_ONE:
          return "";//1";
          break;
        case PartialDerivativeKind::LINEAR_MINUS_ONE:
          return "-1";
          break;
        case PartialDerivativeKind::LINEAR:
          return getLinearizedExpressionEdgeAlg().hasConcretePartialAssignment()
                 ? getAssignmentFromEdge().getLHS().getVariableSymbolReference().getSymbol().getId()
                 : getLinearizedExpressionEdgeAlg().getConcreteConstant().toString();
          break;
        case PartialDerivativeKind::NONLINEAR:
          return getLinearizedExpressionEdgeAlg().hasConcretePartialAssignment()
                  ? getAssignmentFromEdge().getLHS().getVariableSymbolReference().getSymbol().getId()
                  : getLinearizedExpressionEdgeAlg().getConcreteConstant().toString();
          break;
        case PartialDerivativeKind::PASSIVE:
          return "PASSIVE";
          break;
        default:
          THROW_LOGICEXCEPTION_MACRO("PrivateLinearizedComputationalGraphEdge::getLabelString: unexpected PDK");
          break;
      } // end switch
    } // end if
    else {
      switch(getEdgeLabelType()) {
        case xaifBoosterCrossCountryInterface::LinearizedComputationalGraphEdge::UNIT_LABEL:
          return "";
          break;
        case xaifBoosterCrossCountryInterface::LinearizedComputationalGraphEdge::CONSTANT_LABEL:
          return getLinearizedExpressionEdgeAlg().getConcreteConstant().toString();
          break;
        default:
          return getLinearizedExpressionEdgeAlg().hasConcretePartialAssignment()
                 ? getAssignmentFromEdge().getLHS().getVariableSymbolReference().getSymbol().getId()
                 : "";
          break;
      } // end switch
    } // end else
  }

  std::string
  PrivateLinearizedComputationalGraphEdge::getColorString() const {
    if (hasLinearizedExpressionEdge()) {
      switch(getLinearizedExpressionEdgeAlg().getPartialDerivativeKind()) {
        case PartialDerivativeKind::LINEAR_ONE:
          return "red";
          break;
        case PartialDerivativeKind::LINEAR_MINUS_ONE:
          return "pink";
          break;
        case PartialDerivativeKind::LINEAR:
          return "blue";
          break;
        default:
          return "black";
          break;
      }
    }
    else {
      switch(getEdgeLabelType()) {
        case xaifBoosterCrossCountryInterface::LinearizedComputationalGraphEdge::UNIT_LABEL:
          return "red";
          break;
        case xaifBoosterCrossCountryInterface::LinearizedComputationalGraphEdge::CONSTANT_LABEL:
          return "blue";
          break;
        default:
          return "black";
          break;
      }
    }
  }

  std::string
  PrivateLinearizedComputationalGraphEdge::getStyleString() const {
    return isDirectCopyEdge() ? "dashed"
                              : "solid";
  }

  void PrivateLinearizedComputationalGraphEdge::setLinearizedExpressionEdge(const ExpressionEdge& anExpressionEdge) {
    if (myLinearizedExpressionEdge_p || myDirectCopyEdgeFlag) 
      THROW_LOGICEXCEPTION_MACRO("PrivateLinearizedComputationalGraphEdge::setLinearizedExpressionEdge: already set");
    myLinearizedExpressionEdge_p=&anExpressionEdge;
  }

  void PrivateLinearizedComputationalGraphEdge::setDirectCopyEdge() {
    if (myLinearizedExpressionEdge_p || myDirectCopyEdgeFlag) 
      THROW_LOGICEXCEPTION_MACRO("PrivateLinearizedComputationalGraphEdge::setDirectCopyEdge: already set");
    myDirectCopyEdgeFlag=true;
    setEdgeLabelType(UNIT_LABEL);
  }

  bool PrivateLinearizedComputationalGraphEdge::isDirectCopyEdge() const {
    return myDirectCopyEdgeFlag;
  }

  const ExpressionEdge& 
  PrivateLinearizedComputationalGraphEdge::getLinearizedExpressionEdge() const { 
    if (!myLinearizedExpressionEdge_p) 
      THROW_LOGICEXCEPTION_MACRO("PrivateLinearizedComputationalGraphEdge::getLinearizedExpressionEdge: not set");
    return *myLinearizedExpressionEdge_p;
  } 

  bool
  PrivateLinearizedComputationalGraphEdge::hasLinearizedExpressionEdge() const { 
    if (!myLinearizedExpressionEdge_p) return false;
    return true;
  } 

  void PrivateLinearizedComputationalGraphEdge::addParallel(const ExpressionEdge& theParallelEdge) {
    myParallelEdges.push_back(&theParallelEdge);
  } 

  const PrivateLinearizedComputationalGraphEdge::ExpressionEdgePList& 
  PrivateLinearizedComputationalGraphEdge::getParallels() const { 
    return myParallelEdges;
  } 

} 
