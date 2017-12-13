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

#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/PreaccumulationMetric.hpp"

using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulation { 

  std::string
  PreaccumulationMetric::toString(const PreaccumulationMetric_E& aPreaccumulationMetric)
  throw (PrintingIntException) {
    std::string returnString;
    switch(aPreaccumulationMetric) {
      case OPERATIONS_METRIC:
        returnString = "operations";
        break;
      case SCARCITY_METRIC:
        returnString = "scarcity";
        break;
      default: 
        throw PrintingIntException("PreaccumulationMetric::toString: unknown value", aPreaccumulationMetric);
        break;
    } // end switch(aPreaccumulationMetric)
    return returnString;
  } // end PreaccumulationMetric::toString()

  void
  PreaccumulationMetric::checkValid(const PreaccumulationMetric_E& aPreaccumulationMetric) {
    switch(aPreaccumulationMetric) {
      case OPERATIONS_METRIC:
      case SCARCITY_METRIC:
        break;
      default: 
        THROW_LOGICEXCEPTION_MACRO("PreaccumulationMetric::checkValid: unknown value " << aPreaccumulationMetric);
        break;
    } // end switch(aPreaccumulationMetric)
  } // end PreaccumulationMetric::checkValid()

} // end namespace xaifBoosterBasicBlockPreaccumulation

