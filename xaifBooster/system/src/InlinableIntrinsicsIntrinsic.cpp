// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/system/inc/InlinableIntrinsicsIntrinsic.hpp"
#include "xaifBooster/system/inc/Intrinsic.hpp"

namespace xaifBooster { 

  const std::string InlinableIntrinsicsIntrinsic::ourXAIFName("xaif:Intrinsic");
  const std::string InlinableIntrinsicsIntrinsic::our_myName_XAIFName("name");
  const std::string InlinableIntrinsicsIntrinsic::our_myId_XAIFName("vertex_id");

  ExpressionVertex& 
  InlinableIntrinsicsIntrinsic::createCopyOfMyself() const {
    Intrinsic* aNewIntrinsic_p=new Intrinsic(myName);
    return *aNewIntrinsic_p;
  } 

} 
