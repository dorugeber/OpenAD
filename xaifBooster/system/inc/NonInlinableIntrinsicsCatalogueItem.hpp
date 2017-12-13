#ifndef _NONINLINABLEINTRINSICSCATALOGUEITEM_INCLUDE_
#define _NONINLINABLEINTRINSICSCATALOGUEITEM_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include <vector>
#include "xaifBooster/system/inc/ExplicitJacobian.hpp"
#include "xaifBooster/system/inc/DirectAction.hpp"

namespace xaifBooster { 

  /**
   * a class that contains all the 
   * definitions for a particular 
   * intrinsic
   */
  class NonInlinableIntrinsicsCatalogueItem { 
    
  public: 

    /**
     * we preallocate the vector with the resp. size
     * and check any argument position against this size
     * the ctor will also preallocate an Expression for the 
     * function but will not fill it. 
     * \todo JU: why do we preallocate the expression for the function?
     */
    NonInlinableIntrinsicsCatalogueItem(bool aNonSmoothFlag);

    ~NonInlinableIntrinsicsCatalogueItem();

    static const std::string our_myNonSmoothFlag_XAIFName;
    static const std::string our_myName_XAIFName;

    /** 
     * returns flag value
     */
    bool isNonSmooth() const; 

    bool isExplicitJacobian() const; 

    bool isDirectAction() const; 

    ExplicitJacobian& getExplicitJacobian(); 

    DirectAction& getDirectAction();
    const DirectAction& getDirectAction()const;

    ExplicitJacobian& makeExplicitJacobian(); 

    DirectAction& makeDirectAction(); 
  private:
    
    /**
     * no def
     */
    NonInlinableIntrinsicsCatalogueItem();

    /**
     * no def
     */
    NonInlinableIntrinsicsCatalogueItem(const NonInlinableIntrinsicsCatalogueItem&);

    /**
     * no def
     */
    NonInlinableIntrinsicsCatalogueItem& operator=(const NonInlinableIntrinsicsCatalogueItem&);

    /** 
     * a flag in the intrinsics catalogue indicating a nonsmooth 
     * behavior
     */
    const bool myNonSmoothFlag;

    /** 
     * if this is an ExplicitJacobian
     * this points to it;
     * we own the instance
     */
    ExplicitJacobian *myExplicitJacobian_p;

    /** 
     * if this is a DirectAction
     * this points to it;
     * we own the instance
     */
    DirectAction *myDirectAction_p;

  }; 
  
} 

#endif
