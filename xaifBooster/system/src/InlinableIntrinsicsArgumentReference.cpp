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
#include "xaifBooster/system/inc/InlinableIntrinsicsArgumentReference.hpp"
#include "xaifBooster/system/inc/Argument.hpp"

namespace xaifBooster { 

  const std::string InlinableIntrinsicsArgumentReference::ourXAIFName("xaifii:ArgumentReference");
  const std::string InlinableIntrinsicsArgumentReference::our_myArgRef_XAIFName("argument");
  const std::string InlinableIntrinsicsArgumentReference::our_myId_XAIFName("vertex_id");

  ExpressionVertex& 
  InlinableIntrinsicsArgumentReference::createCopyOfMyself() const {
    THROW_LOGICEXCEPTION_MACRO("InlinableIntrinsicsArgumentReference::createCopyOfMyself: prohibited invocation for position "
			       << myPosition);
    // not reachable but we should match return type...
    // somebody will always complain
    return *(new Argument());
  } 

} 
