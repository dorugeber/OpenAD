// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/utils/inc/LogicException.hpp"
#include "xaifBooster/utils/inc/StringConversions.hpp"
#include "xaifBooster/system/inc/InlinableIntrinsicsExpressionEdge.hpp"

namespace xaifBooster { 

  const std::string InlinableIntrinsicsExpressionEdge::ourXAIFName("xaif:ExpressionEdge");
  const std::string InlinableIntrinsicsExpressionEdge::our_myId_XAIFName("edge_id");
  const std::string InlinableIntrinsicsExpressionEdge::our_source_XAIFName("source");
  const std::string InlinableIntrinsicsExpressionEdge::our_target_XAIFName("target");
  const std::string InlinableIntrinsicsExpressionEdge::our_myPosition_XAIFName("position");


  void
  InlinableIntrinsicsExpressionEdge::setPosition(const std::string& i) {
    if (myPosition) THROW_LOGICEXCEPTION_MACRO("InlinableIntrinsicsExpressionEdge::setPosition: already set");
    myPosition=StringConversions::convertToUInt(i);
  }

  unsigned int
  InlinableIntrinsicsExpressionEdge::getPosition() const {
    if (!myPosition) THROW_LOGICEXCEPTION_MACRO("InlinableIntrinsicsExpressionEdge::getPosition: not set");
    return myPosition;
  }

  std::string
  InlinableIntrinsicsExpressionEdge::debug() const {
    return ObjectWithId::debug();
  }

}

