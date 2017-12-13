// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/utils/inc/DbgGroup.hpp"
#include "xaifBooster/utils/inc/LogicException.hpp"

namespace xaifBooster { 

  std::string 
  DbgGroup::toString(const DbgGroup_E& aGroup)
    throw (PrintingIntException) { 
    std::string returnString;
    switch(aGroup) {
    case ERROR:
      returnString="ERROR";
      break;
    case WARNING:
      returnString="WARNING";
      break;
    case CALLSTACK: 
      returnString="CALLSTACK";
      break;
    case DATA: 
      returnString="DATA";
      break;
    case GRAPHICS: 
      returnString="GRAPHICS";
      break;
    case TIMING: 
      returnString="TIMING";
      break;
    case TEMPORARY: 
      returnString="TEMPORARY";
      break;
    case METRIC: 
      returnString="METRIC";
      break;
    default: 
      throw PrintingIntException("DbgGroup::toString: unknown value",aGroup);
      break;
    } // end switch
    return returnString;
  } // end of std::string DbgGroup::toString

  std::string 
  DbgGroup::printAll() throw (PrintingIntException) {
    std::ostringstream os;
    os << "ERROR=" << ERROR << ","
       << "WARNING=" << WARNING << ","
       << "CALLSTACK=" << CALLSTACK << ","
       << "DATA=" << DATA << ","
       << "GRAPHICS=" << GRAPHICS << ","
       << "TIMING=" << TIMING << ","
       << "TEMPORARY=" << TEMPORARY << ","
       << "METRIC=" << METRIC 
       << std::ends; 
    return os.str();
  }

} // end of namespace xaifBooster
