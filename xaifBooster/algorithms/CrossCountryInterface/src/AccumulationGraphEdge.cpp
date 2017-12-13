// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/algorithms/CrossCountryInterface/inc/AccumulationGraphEdge.hpp"

using namespace xaifBooster;

namespace xaifBoosterCrossCountryInterface {

  AccumulationGraphEdge::AccumulationGraphEdge() {}
  AccumulationGraphEdge::~AccumulationGraphEdge() {}

  std::string AccumulationGraphEdge::debug() const {
    std::ostringstream out;
    out << "AccumulationGraphEdge[" << this
	<< "]" << std::ends;  
    return out.str();
  } // end AccumulationGraphEdge::debug()

} // end namespace xaifBoosterCrossCountryInterface

