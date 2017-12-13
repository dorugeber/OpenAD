#ifndef _XAIFBOOSTERCROSSCOUNTRYINTERFACE_AWARENESSLEVEL_INCLUDE_
#define _XAIFBOOSTERCROSSCOUNTRYINTERFACE_AWARENESSLEVEL_INCLUDE_
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

namespace xaifBoosterCrossCountryInterface {

  class AwarenessLevel {
  public:

    enum AwarenessLevel_E {
      NO_AWARENESS = 0,
      UNIT_AWARENESS = 1,
      CONSTANT_AWARENESS = 2
    };

    static void
    checkValid(const AwarenessLevel_E& anAwarenessLevel);
    
    static std::string
    toString(const AwarenessLevel_E& anAwarenessLevel) throw (PrintingIntException);

  }; // end class AwarenessLevel

} // end namespace xaifBoosterCrossCountryInterface

#endif

