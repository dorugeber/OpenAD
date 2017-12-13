// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include <sstream>
#include "xaifBooster/utils/inc/PrintManager.hpp"
#include "xaifBooster/utils/inc/LogicException.hpp"
#include "xaifBooster/utils/inc/DbgLoggerManager.hpp"
#include "xaifBooster/system/inc/AliasRange.hpp"

namespace xaifBooster { 
  
  const std::string AliasRange::ourXAIFName("xaif:AliasRange");
  const std::string AliasRange::our_myLowerAddress_XAIFName("from_virtual_address");
  const std::string AliasRange::our_myUpperAddress_XAIFName("to_virtual_address");
  const std::string AliasRange::our_myPartial_XAIFName("partial");

  AliasRange::AliasRange(unsigned int lower,
			 unsigned int upper,
			 bool aPartial): 
    myLowerAddress(lower),
    myUpperAddress(upper),
    myPartial(aPartial){
    // JU: fix this, don't like excception in ctor
    if (lower>upper)
      THROW_LOGICEXCEPTION_MACRO("AliasRange::AliasRange: lower larger than upper");
  } 
 
  std::string AliasRange::debug() const {
    std::ostringstream out;
    out << "AliasRange[" << this 
	<< ", myLowerAddress=" << myLowerAddress
	<< ", myUpperAddress=" << myUpperAddress
	<< ", myPartial=" << myPartial
	<< "]" << std::ends; 
    return out.str();
  } // end of AliasRange::debug
  
  void AliasRange::printXMLHierarchy(std::ostream& os) const {
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent() 
       << "<" 
       << ourXAIFName.c_str() 
       << " " 
       << our_myLowerAddress_XAIFName.c_str()
       << "=\""
       << myLowerAddress
       << "\" " 
       << our_myUpperAddress_XAIFName.c_str()
       << "=\""
       << myUpperAddress
       << "\" " 
       << our_myPartial_XAIFName.c_str()
       << "=\""
       << myPartial
       << "\"/>" 
       << std::endl;
    pm.releaseInstance();
  } 

  unsigned int AliasRange::max() const { 
    return myUpperAddress;
  }

  unsigned int AliasRange::min() const {
    return myLowerAddress;
  }

  void AliasRange::max(unsigned int newUpper) { 
    if (newUpper<myUpperAddress)
      THROW_LOGICEXCEPTION_MACRO("AliasRange::max: lowering of max not permitted");
    myUpperAddress=newUpper;
  }

  void AliasRange::min(unsigned int newLower) {
    if (newLower>myLowerAddress)
      THROW_LOGICEXCEPTION_MACRO("AliasRange::min: raising of min not permitted");
    myLowerAddress=newLower;
  }

  bool AliasRange::sameAs(const AliasRange& theOther) const { 
    if (min()==theOther.min() && 
	max()==theOther.max()) { 
      if (isPartial()!=theOther.isPartial())
	DBG_MACRO(DbgGroup::ERROR,"AliasRange::sameAs: identical ranges with different partial settings");
      return true;
    }
    return false;
  } // end of AliasRange::sameAs

  bool AliasRange::isGreaterThan(const AliasRange& theOther) const { 
    return (max()<theOther.min() ? true:false);
  } // end of AliasRange::isGreaterThan

  bool AliasRange::isContainedIn(const AliasRange& theOther) const { 
    return (max()<=theOther.max() && min()>=theOther.min() ?
	    true:false);
  } // end of AliasRange::isGreaterThan

  bool AliasRange::overlapsWith(const AliasRange& theOther) const { 
    if (((min()<=theOther.max() && max()>theOther.max())
	 || 
	 (max()>=theOther.min() && min()<theOther.min())) 
	&&
	min()!=max())
      return true;
    return false;
  } // end of AliasRange::overlapsWith

  bool AliasRange::bordersWith(const AliasRange& theOther) const { 
    if (min()==theOther.max()+1
	|| 
	max()+1==theOther.min())
      return true;
    return false;
  } // end of AliasRange::bordersWith

  void AliasRange::absorb(const AliasRange& theOther) { 
    if (theOther.min()<min())
      min(theOther.min());
    if (theOther.max()>max())
      max(theOther.max());
  } // end of AliasRange::absorb

  bool AliasRange::isPartial()const { 
    return myPartial;
  }

}
