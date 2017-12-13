#ifndef _HEURISTICPARSE_INCLUDE_
#define _HEURISTICPARSE_INCLUDE_
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
#include <list>
#include <boost/tokenizer.hpp>
#include "xaifBooster/algorithms/MemOpsTradeoffPreaccumulation/inc/ConceptuallyStaticInstances.hpp"

namespace MemOpsTradeoffPreaccumulation { 

  class HeuristicParse {

  public:

    static MemOpsTradeoffPreaccumulation::ConceptuallyStaticInstances::Heuristic stringToEnum(const std::string& theHeuristic);

    static void fromString(const std::string& theHeuristicString);

  private:

  }; // end of class HeuristicParse

} // end of namespace MemOpsTradeoffPreaccumulation
                                                                     
#endif
