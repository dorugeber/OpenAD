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
#include "xaifBooster/utils/inc/PrintManager.hpp"
#include "xaifBooster/system/inc/AliasMapEntry.hpp"

namespace xaifBooster { 

  const std::string AliasMapEntry::ourXAIFName("xaif:AliasSet");
  const std::string AliasMapEntry::our_myKey_XAIFName("key");

  std::string AliasMapEntry::debug() const {
    std::ostringstream out;
    out << "AliasMapEntry[" << this 
	<< ", myAliasSet=" << myAliasSet.debug().c_str()
	<< "]" << std::ends; 
    return out.str();
  } // end of AliasMapEntry::debug
  
  void AliasMapEntry::printXMLHierarchy(std::ostream& os) const {
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent() 
       << "<" 
       << ourXAIFName 
       << " " 
       << our_myKey_XAIFName
       << "=\""
       << myKey
       << "\"" 
       << ">" 
       << std::endl;
    myAliasSet.printXMLHierarchy(os);       
    os << pm.indent() 
       << "</"
       << ourXAIFName
       << ">" << std::endl;
    pm.releaseInstance();
  } 

  const AliasSet& AliasMapEntry::getAliasSet() const { 
    return myAliasSet;
  }

  AliasSet& AliasMapEntry::getAliasSet() { 
    return myAliasSet;
  }

  bool AliasMapEntry::disjointFrom(const AliasMapEntry& theOtherAliastActivity) const { 
    return (!myAliasSet.sharesAliasWith(theOtherAliastActivity.myAliasSet));
  } 

  bool AliasMapEntry::mustAlias(const AliasMapEntry& theOtherAliastActivity) const { 
    return (!myAliasSet.mustAlias(theOtherAliastActivity.myAliasSet));
  } 

} 
