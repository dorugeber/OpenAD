// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/utils/inc/LogicException.hpp"
#include "xaifBooster/system/inc/NonInlinableIntrinsicsCatalogueItem.hpp"

namespace xaifBooster { 

  const std::string NonInlinableIntrinsicsCatalogueItem::our_myName_XAIFName("name");
  const std::string NonInlinableIntrinsicsCatalogueItem::our_myNonSmoothFlag_XAIFName("nonSmooth");

  NonInlinableIntrinsicsCatalogueItem::NonInlinableIntrinsicsCatalogueItem(bool aNonSmoothFlag) :
    myNonSmoothFlag(aNonSmoothFlag),
    myExplicitJacobian_p(0),
    myDirectAction_p(0) { 
  } 

  NonInlinableIntrinsicsCatalogueItem::~NonInlinableIntrinsicsCatalogueItem() { 
    if (myExplicitJacobian_p)
      delete myExplicitJacobian_p;
    if (myDirectAction_p)
      delete myDirectAction_p;
  } 

  bool 
  NonInlinableIntrinsicsCatalogueItem::isNonSmooth() const { 
    return myNonSmoothFlag;
  }

  bool NonInlinableIntrinsicsCatalogueItem::isExplicitJacobian() const { 
    return myExplicitJacobian_p!=0;
  }

  bool NonInlinableIntrinsicsCatalogueItem::isDirectAction() const { 
    return myDirectAction_p!=0;
  } 

  ExplicitJacobian& NonInlinableIntrinsicsCatalogueItem::getExplicitJacobian() { 
    if (!myExplicitJacobian_p)
      THROW_LOGICEXCEPTION_MACRO("NonInlinableIntrinsicsCatalogueItem::getExplicitJacobian: not set");
    return *myExplicitJacobian_p;
  }

  DirectAction& NonInlinableIntrinsicsCatalogueItem::getDirectAction() { 
    if (!myDirectAction_p)
      THROW_LOGICEXCEPTION_MACRO("NonInlinableIntrinsicsCatalogueItem::getDirectAction: not set");
    return *myDirectAction_p;
  }

  const DirectAction& NonInlinableIntrinsicsCatalogueItem::getDirectAction() const {
    if (!myDirectAction_p)
      THROW_LOGICEXCEPTION_MACRO("NonInlinableIntrinsicsCatalogueItem::getDirectAction: not set");
    return *myDirectAction_p;
  }

  ExplicitJacobian& NonInlinableIntrinsicsCatalogueItem::makeExplicitJacobian() { 
    if (myExplicitJacobian_p)
      THROW_LOGICEXCEPTION_MACRO("NonInlinableIntrinsicsCatalogueItem::makeExplicitJacobian: already set as ExplicitJacobian");
    if (myDirectAction_p)
      THROW_LOGICEXCEPTION_MACRO("NonInlinableIntrinsicsCatalogueItem::makeExplicitJacobian: already set as DirectAction");
    myExplicitJacobian_p=new ExplicitJacobian();
    return *myExplicitJacobian_p;
  }

  DirectAction& NonInlinableIntrinsicsCatalogueItem::makeDirectAction() { 
    if (myExplicitJacobian_p)
      THROW_LOGICEXCEPTION_MACRO("NonInlinableIntrinsicsCatalogueItem::makeExplicitJacobian: already set as ExplicitJacobian");
    if (myDirectAction_p)
      THROW_LOGICEXCEPTION_MACRO("NonInlinableIntrinsicsCatalogueItem::makeExplicitJacobian: already set as DirectAction");
    myDirectAction_p=new DirectAction();
    return *myDirectAction_p;
  }

} 
