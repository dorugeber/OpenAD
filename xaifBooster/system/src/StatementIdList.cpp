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

#include "xaifBooster/system/inc/StatementIdList.hpp"

namespace xaifBooster { 

  std::string StatementIdList::debug() const {
    std::ostringstream out;
    out << "StatementIdList[" << this 
	<< ":";
    for (const_iterator i = begin();
	 i!=end();) {
      out << i->c_str(); 
      ++i; 
      if (i!=end())
	out << ",";
    }
    out << "]" << std::ends; 
    return out.str();
  }

} // end of namespace  
