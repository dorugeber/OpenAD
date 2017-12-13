#ifndef _CONTROLFLOWGRAPHBASE_INCLUDE_
#define _CONTROLFLOWGRAPHBASE_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/GraphWrapperTraversable.hpp"
#include "xaifBooster/system/inc/ControlFlowGraphVertex.hpp"
#include "xaifBooster/system/inc/ControlFlowGraphEdge.hpp"

namespace xaifBooster { 

  class ControlFlowGraphBase : public GraphWrapperTraversable<ControlFlowGraphVertex, ControlFlowGraphEdge> {  
  public:

    ControlFlowGraphBase();

    /**
     * name for member myStructuredFlag as represented in XAIF schema
     */
    static const std::string our_myStructuredFlag_XAIFName;

    void setStructured(bool aFlag);

    bool isStructured() const; 

  private:

    /** 
     * structured control flow
     * (every control path through loop/branch
     * needs to also go through the (unique) corresponding 
     * endloop/endbranch vertex
     */
    bool myStructuredFlag; 

  }; 

} // end of namespace xaifBooster
                                                                     
#endif
