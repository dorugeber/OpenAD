#ifndef _CONTROLFLOWGRAPHEDGE_INCLUDE_
#define _CONTROLFLOWGRAPHEDGE_INCLUDE_
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
  
  class ControlFlowGraphBase;

  class ControlFlowGraphEdge : public EdgeTraversable, 
			       public ObjectWithId {
  public:

    ControlFlowGraphEdge() : 
      myConditionValueFlag(false), 
      myConditionValue(0) {
    };
    
    ~ControlFlowGraphEdge(){};
    
    void printXMLHierarchy(std::ostream& os,
			   const ControlFlowGraphBase& theGraph) const;

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

    /**
     * name for hasConditionValue as represented in XAIF schema
     */
    static const std::string our_myConditionValueFlag_XAIFName;

    /**
     * name for condition_value as represented in XAIF schema
     */
    static const std::string our_myConditionValue_XAIFName;



    bool hasConditionValue() const;

    void setConditionValue(int cv);

    int getConditionValue() const;

    bool leadsToLoopBody() const;

  private:

    bool myConditionValueFlag;
    
    int myConditionValue;  

  }; // end of class ControlFlowGraphEdge
 
} // end of namespace xaifBooster
                                                                     
#endif
