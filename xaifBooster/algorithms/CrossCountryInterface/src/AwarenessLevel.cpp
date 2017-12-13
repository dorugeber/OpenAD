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

#include "xaifBooster/algorithms/CrossCountryInterface/inc/AwarenessLevel.hpp"

using namespace xaifBooster;

namespace xaifBoosterCrossCountryInterface { 

  std::string
  AwarenessLevel::toString(const AwarenessLevel_E& anAwarenessLevel)
  throw (PrintingIntException) {
    std::string returnString;
    switch(anAwarenessLevel) {
      case NO_AWARENESS:
        returnString = "no awareness";
        break;
      case UNIT_AWARENESS:
        returnString = "unit awareness";
        break;
      case CONSTANT_AWARENESS:
        returnString = "constant awareness";
        break;
      default: 
        throw PrintingIntException("AwarenessLevel::toString: unknown value", anAwarenessLevel);
        break;
    } // end switch(anAwarenessLevel)
    return returnString;
  } // end AwarenessLevel::toString()

  void
  AwarenessLevel::checkValid(const AwarenessLevel_E& anAwarenessLevel) {
    switch(anAwarenessLevel) {
      case NO_AWARENESS:
      case UNIT_AWARENESS:
      case CONSTANT_AWARENESS:
        break;
      default: 
        THROW_LOGICEXCEPTION_MACRO("AwarenessLevel::checkValid: unknown value " << anAwarenessLevel);
        break;
    } // end switch(anAwarenessLevel)
  } // end AwarenessLevel::checkValid()

} // end namespace xaifBoosterCrossCountryInterface

