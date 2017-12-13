#ifndef _CALLGRAPHEDGE_INCLUDE_
#define _CALLGRAPHEDGE_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/utils/inc/ObjectWithId.hpp"
#include "xaifBooster/system/inc/EdgeTraversable.hpp"

namespace xaifBooster { 

  class CallGraph;


  /** 
   * A CallGraphEdge represents  a (subroutine) call being made
   * there are as of now no further data associated with 
   * this kind of call
   */
  class CallGraphEdge : public EdgeTraversable, 
			public ObjectWithId {
  public:

    CallGraphEdge (){};

    ~CallGraphEdge(){};

    void printXMLHierarchy(std::ostream& os,
			   const CallGraph& theGraph) const;

    std::string debug() const ;

    /**
     * name for this class as represented in XAIF schema
     */
    static const std::string ourXAIFName;

    /**
     * name for inherited member myId as represented in XAIF schema
     */
    static const std::string our_myId_XAIFName;

    /**
     * name for source as represented in XAIF schema
     */
    static const std::string our_source_XAIFName;

    /**
     * name for target as represented in XAIF schema
     */
    static const std::string our_target_XAIFName;

  }; // end of class CallGraphEdge
 
} // end of namespace xaifBooster
                                                                     
#endif
