#ifndef _BASELOOP_INCLUDE_
#define _BASELOOP_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/Condition.hpp"
#include "xaifBooster/system/inc/ControlFlowGraphVertex.hpp"
#include "xaifBooster/system/inc/ObjectWithLineNumber.hpp"

namespace xaifBooster { 

  /**
   * base class for loops
   */
  class BaseLoop : public ControlFlowGraphVertex,
  public ObjectWithLineNumber {
    public:

    BaseLoop();

    ~BaseLoop();

    /**
     * print debug information
     */
    std::string debug() const ;

    Condition& makeCondition();
    Condition& getCondition();
    const Condition& getCondition() const;

    private:

    /** 
     * the condition 
     */
    Condition* myCondition_p;

  };

}

#endif
