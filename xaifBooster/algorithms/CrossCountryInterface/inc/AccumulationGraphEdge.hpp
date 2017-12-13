#ifndef _XAIFBOOSTERCROSSCOUNTRYINTERFACE_ACCUMULATIONGRAPHEDGE_INCLUDE_
#define _XAIFBOOSTERCROSSCOUNTRYINTERFACE_ACCUMULATIONGRAPHEDGE_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/boostWrapper/inc/Edge.hpp"

using namespace xaifBooster;

namespace xaifBoosterCrossCountryInterface {

  /**
   *
   */
  class AccumulationGraphEdge : public Edge {

  public:

    AccumulationGraphEdge();
    ~AccumulationGraphEdge();

    std::string debug() const;

  }; // end of class AccumulationGraphEdge
 
} // end namespace xaifBoosterCrossCountryInterface

#endif

