// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/RemainderGraphEdge.hpp"

#include "xaifBooster/algorithms/CrossCountryInterface/inc/AccumulationGraphVertex.hpp"

namespace xaifBoosterBasicBlockPreaccumulation { 

  RemainderGraphEdge::RemainderGraphEdge(const xaifBoosterCrossCountryInterface::AccumulationGraphVertex& aAccumulationGraphVertex) :
   myAccumulationGraphVertex_p(&aAccumulationGraphVertex) {
  }

  std::string
  RemainderGraphEdge::debug() const {
    std::ostringstream out;
    out << "RemainderGraphEdge[" << Edge::debug().c_str()
        << ",myAccumulationGraphVertex_p=>" << myAccumulationGraphVertex_p->debug().c_str()
        << "]" << std::ends;
    return out.str();
  }

  std::string
  RemainderGraphEdge::getColorString() const {
    return myAccumulationGraphVertex_p->getColorString();
  }

  std::string
  RemainderGraphEdge::getLabelString() const {
    std::ostringstream out;
    if (getAccumulationGraphVertex().getPartialDerivativeKind() == PartialDerivativeKind::LINEAR)
      out << getAccumulationGraphVertex().getValueString();
    else if (getAccumulationGraphVertex().getPartialDerivativeKind() == PartialDerivativeKind::NONLINEAR)
      out << getAccumulationGraphVertex().getLHSString();
    return out.str();
  }

  std::string
  RemainderGraphEdge::getStyleString() const {
    // \TODO \FIXME should return "dashed" for direct copy edges
    return "solid";
  }

  const xaifBoosterCrossCountryInterface::AccumulationGraphVertex&
  RemainderGraphEdge::getAccumulationGraphVertex() const {
    return *myAccumulationGraphVertex_p;
  }

}

