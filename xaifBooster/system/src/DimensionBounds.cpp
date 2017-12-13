// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/utils/inc/PrintManager.hpp"
#include "xaifBooster/utils/inc/LogicException.hpp"
#include "xaifBooster/system/inc/DimensionBounds.hpp"

namespace xaifBooster { 

  const std::string DimensionBounds::ourXAIFName("xaif:DimensionBounds");

  const std::string DimensionBounds::our_myLower_XAIFName("lower");

  const std::string DimensionBounds::our_myUpper_XAIFName("upper");

  IndexOrder::IndexOrder_E DimensionBounds::ourIndexOrder(IndexOrder::COLUMNMAJOR);

  DimensionBounds::DimensionBounds(int aLower,
				   int anUpper) : 
    myLower(aLower),
    myUpper(anUpper) {
  }

  void DimensionBounds::printXMLHierarchy(std::ostream& os) const {
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent() 
       << "<"
       << ourXAIFName.c_str() 
       << " " 
       << our_myLower_XAIFName.c_str() 
       << "=\"" 
       << myLower
       << "\" " 
       << our_myUpper_XAIFName.c_str() 
       << "=\""
       << myUpper
       << "\"/>" 
       << std::endl; 
    pm.releaseInstance();
  } // end if DimensionBounds::printXMLHierarchy
  
  std::string DimensionBounds::debug () const { 
    std::ostringstream out;
    out << "DimensionBounds[" << this 
	<< ",myLower=" << myLower
	<< ",myUpper=" << myUpper
	<< "]" << std::ends;  
    return out.str();
  } // end of DimensionBounds::debug

  int DimensionBounds::getLower()const { 
    return myLower;
  }
  
  int DimensionBounds::getUpper()const {
    return myUpper;
  }
  
  IndexOrder::IndexOrder_E DimensionBounds::getIndexOrder() {
    return ourIndexOrder;
  }

} // end of namespace xaifBooster 
