// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/utils/inc/StringConversions.hpp"
#include "xaifBooster/system/inc/NonInlinableIntrinsicsCatalogue.hpp"

namespace xaifBooster { 

  const std::string NonInlinableIntrinsicsCatalogue::ourXAIFName("xaifnii:NonInlinableIntrinsicDefinitions");

  NonInlinableIntrinsicsCatalogue::NonInlinableIntrinsicsCatalogue(){
    
  }

  NonInlinableIntrinsicsCatalogue::~NonInlinableIntrinsicsCatalogue(){
  }

  NonInlinableIntrinsicsCatalogueItem& 
  NonInlinableIntrinsicsCatalogue::addCatalogueItem(const std::string& theKey,
						    bool aNonSmoothFlag) { 
    NonInlinableIntrinsicsCatalogueItem* theItem_p=
      new NonInlinableIntrinsicsCatalogueItem(aNonSmoothFlag);
    myHashTable.addElement(theKey,
			   theItem_p);
    return *theItem_p;
  }

  const NonInlinableIntrinsicsCatalogueItem& 
  NonInlinableIntrinsicsCatalogue::getElement(const std::string& theKey) const { 
    return *(myHashTable.getElement(theKey));
  } 

} 
