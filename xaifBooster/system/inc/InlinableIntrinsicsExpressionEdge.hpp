#ifndef _INLINABLEINTRINSICSEXPRESSIONEDGE_INCLUDE_
#define _INLINABLEINTRINSICSEXPRESSIONEDGE_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/boostWrapper/inc/Edge.hpp"
#include "xaifBooster/utils/inc/ObjectWithId.hpp"

namespace xaifBooster { 

  

  class InlinableIntrinsicsExpressionEdge : public Edge, 
					    public ObjectWithId { 
  public:

    InlinableIntrinsicsExpressionEdge () : myPosition(0) {};
    ~InlinableIntrinsicsExpressionEdge(){};

    void setPosition(const std::string&);
    unsigned int getPosition() const;

    virtual std::string debug() const;

    static const std::string ourXAIFName;
    static const std::string our_myId_XAIFName;
    static const std::string our_source_XAIFName;
    static const std::string our_target_XAIFName;
    static const std::string our_myPosition_XAIFName;

  private:

    unsigned int myPosition;

  }; // end of class InlinableIntrinsicsExpressionEdge
 
} // end of namespace xaifBooster
                                                                     
#endif
