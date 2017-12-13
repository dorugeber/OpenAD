#ifndef _INLINABLEINTRINSICSINTRINSIC_INCLUDE_
#define _INLINABLEINTRINSICSINTRINSIC_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/InlinableIntrinsicsExpressionVertex.hpp"

namespace xaifBooster { 

  /**
   * a class for a vertex representing an argument in an 
   * intrinsic
   */
  class InlinableIntrinsicsIntrinsic : public InlinableIntrinsicsExpressionVertex  { 
  
  public:

    InlinableIntrinsicsIntrinsic(const std::string& aName) :
      myName(aName) {};

    ~InlinableIntrinsicsIntrinsic(){};

    virtual ExpressionVertex& createCopyOfMyself() const;

    static const std::string ourXAIFName;
    static const std::string our_myName_XAIFName;
    static const std::string our_myId_XAIFName;

  private:
    
    /**
     * intially for readout from the XML file only, 
     * refers to the key in 
     * InlinableIntrinsicsCatalogue
     */
    const std::string myName;

  }; // end of class InlinableIntrinsicsIntrinsic
 
} // end of namespace xaifBooster
                                                                     
#endif
