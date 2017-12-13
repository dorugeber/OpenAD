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

#include "xaifBooster/system/inc/ExpressionEdge.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/PrivateLinearizedComputationalGraphEdge.hpp"

#include "xaifBooster/algorithms/CrossCountryInterface/inc/AccumulationGraphEdge.hpp"

#include "xaifBooster/algorithms/PushPop/inc/CombinedGraphEdge.hpp"

using namespace xaifBooster;

namespace xaifBoosterPushPop { 

  CombinedGraphEdge::CombinedGraphEdge(const xaifBoosterBasicBlockPreaccumulation::PrivateLinearizedComputationalGraphEdge& aLCGEdge) :
    myOrigin(aLCGEdge),
    myOriginType(ORIGINAL_EDGE) {
  }

  CombinedGraphEdge::CombinedGraphEdge(const ExpressionEdge& aLinearizationExpressionEdge) :
    myOrigin(aLinearizationExpressionEdge),
    myOriginType(LINEARIZATION_EDGE) {
  }

  CombinedGraphEdge::CombinedGraphEdge(const xaifBoosterCrossCountryInterface::AccumulationGraphEdge& aAccumulationGraphEdge) :
    myOrigin(aAccumulationGraphEdge),
    myOriginType(ACCUMULATION_EDGE) {
  }

//CombinedGraphEdge::~CombinedGraphEdge(){};

  std::string
  CombinedGraphEdge::debug() const { 
    std::ostringstream out;
    out << "CombinedGraphEdge[" << Edge::debug().c_str()
      //<< ",myOriginType=" << myOriginType
      //<< ",myOrigin=" << myOrigin
        << "]" << std::ends;  
    return out.str();
  } 

  std::string
  CombinedGraphEdge::getStyleString() const {
    if (myOriginType == ORIGINAL_EDGE)
      return myOrigin.myOriginalPLCGEdge_p->getStyleString();
    else
      return "solid";
  }

  std::string
  CombinedGraphEdge::getColorString() const {
    if (myOriginType == ORIGINAL_EDGE)
      return myOrigin.myOriginalPLCGEdge_p->getColorString();
    else
      return "black";
  }

  CombinedGraphEdge::EdgeOriginType_E
  CombinedGraphEdge::getOriginType() const {
    return myOriginType;
  }

  const xaifBoosterBasicBlockPreaccumulation::PrivateLinearizedComputationalGraphEdge&
  CombinedGraphEdge::getPrivateLinearizedComputationalGraphEdge() const {
    if (myOriginType != ORIGINAL_EDGE)
      THROW_LOGICEXCEPTION_MACRO("CombinedGraphEdge::getPrivateLinearizedComputationalGraphEdge: wrong type");
    if (!myOrigin.myOriginalPLCGEdge_p)
      THROW_LOGICEXCEPTION_MACRO("CombinedGraphEdge::getPrivateLinearizedComputationalGraphEdge: not set");
    return *(myOrigin.myOriginalPLCGEdge_p);
  }

  const xaifBoosterCrossCountryInterface::AccumulationGraphEdge&
  CombinedGraphEdge::getAccumulationGraphEdge() const {
    if (myOriginType != ACCUMULATION_EDGE)
      THROW_LOGICEXCEPTION_MACRO("CombinedGraphEdge::getAccumulationGraphEdge: wrong type");
    if (!myOrigin.myAccumulationGraphEdge_p)
      THROW_LOGICEXCEPTION_MACRO("CombinedGraphEdge::getAccumulationGraphEdge: not set");
    return *(myOrigin.myAccumulationGraphEdge_p);
  }

}
