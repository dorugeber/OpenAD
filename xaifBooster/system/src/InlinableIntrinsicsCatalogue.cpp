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
#include "xaifBooster/system/inc/InlinableIntrinsicsCatalogue.hpp"

namespace xaifBooster { 

  const std::string InlinableIntrinsicsCatalogue::ourXAIFName("xaifii:InlinableIntrinsicDefinitions");

  InlinableIntrinsicsCatalogue::InlinableIntrinsicsCatalogue(){
    
  }

  InlinableIntrinsicsCatalogue::~InlinableIntrinsicsCatalogue(){
    // JU need to implement the deletion
  }

  InlinableIntrinsicsCatalogueItem& 
  InlinableIntrinsicsCatalogue::addCatalogueItem(const std::string& theKey,
						 const std::string& theNumberOfArguments,
						 bool aNonSmoothFlag,
						 ShapeChange::ShapeChange_E aShapeChange) { 
    InlinableIntrinsicsCatalogueItem* theItem_p=
      new InlinableIntrinsicsCatalogueItem(StringConversions::convertToUInt(theNumberOfArguments.c_str()),
					   aNonSmoothFlag,
					   aShapeChange);
    myHashTable.addElement(theKey,
			   theItem_p);
    return *theItem_p;
  }

  const InlinableIntrinsicsCatalogueItem& 
  InlinableIntrinsicsCatalogue::getElement(const std::string& theKey) const { 
    return *(myHashTable.getElement(theKey));
  } 

} 
