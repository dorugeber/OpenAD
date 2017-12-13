#ifndef _DIRECTACTION_INCLUDE_
#define _DIRECTACTION_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include <string>

namespace xaifBooster { 

  class DirectAction { 
    
  public: 

    DirectAction();

    ~DirectAction();

    static const std::string ourXAIFName;
    static const std::string our_myDerivAction_XAIFName;

    void setDerivAction(const std::string& aDerivAction); 
    
    const std::string& getDerivAction() const; 

  private:
    
    /**
     * no def
     */
    DirectAction(const DirectAction&);

    /**
     * no def
     */
    DirectAction& operator=(const DirectAction&);

    /** 
     * the call to be made on the derivative
     */
    std::string myDerivAction;

  }; 
  
} 

#endif
