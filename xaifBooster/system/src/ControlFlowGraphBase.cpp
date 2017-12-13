// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/ControlFlowGraphBase.hpp"

namespace xaifBooster { 

  const std::string ControlFlowGraphBase::our_myStructuredFlag_XAIFName("structured");


  ControlFlowGraphBase::ControlFlowGraphBase () :
    myStructuredFlag(true) { 
  } 

  void ControlFlowGraphBase::setStructured(bool aFlag) { 
    myStructuredFlag=aFlag;
  }
  
  bool ControlFlowGraphBase::isStructured() const { 
    return myStructuredFlag;
  } 


} // end of namespace xaifBooster 
