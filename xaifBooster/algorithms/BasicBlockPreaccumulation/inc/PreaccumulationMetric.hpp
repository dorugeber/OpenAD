#ifndef _XAIFBOOSTERBASICBLOCKPREACCUMULATION_PREACCUMULATIONMETRIC_INCLUDE_
#define _XAIFBOOSTERBASICBLOCKPREACCUMULATION_PREACCUMULATIONMETRIC_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include <string>

#include "xaifBooster/utils/inc/PrintingIntException.hpp"

using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulation {

  class PreaccumulationMetric {
  public:

    enum PreaccumulationMetric_E {
      OPERATIONS_METRIC = 0,
      SCARCITY_METRIC = 1
    };

    static void
    checkValid(const PreaccumulationMetric_E& aMetric);
    
    static std::string
    toString(const PreaccumulationMetric_E& aMetric) throw (PrintingIntException);

  }; // end class PreaccumulationMetric

} // end namespace xaifBoosterBasicBlockPreaccumulation

#endif

