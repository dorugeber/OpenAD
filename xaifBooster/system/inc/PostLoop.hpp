#ifndef _POSTLOOP_INCLUDE_
#define _POSTLOOP_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/BaseLoop.hpp"

namespace xaifBooster { 

  /**
   * this class describes 
   * a loop with a post condition 
   */
  class PostLoop : public BaseLoop {

    public:

    PostLoop (){};

    ~PostLoop(){};

    /**
     * print XML hierarchy
     */
    void printXMLHierarchy(std::ostream& os) const;

    /**
     * print debug information
     */
    std::string debug() const ;

    /**
     * name for this class as represented in XAIF schema
     */
    static const std::string ourXAIFName;

    /**
     * name for inherited member myId as represented in XAIF schema
     */
    static const std::string our_myId_XAIFName;
    

    virtual ControlFlowGraphVertexKind::ControlFlowGraphVertexKind_E getKind() const { return ControlFlowGraphVertexKind::POSTLOOP_VKIND;}

  };
 
}
                                                                     
#endif
