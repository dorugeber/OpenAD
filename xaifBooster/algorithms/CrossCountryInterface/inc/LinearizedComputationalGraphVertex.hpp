#ifndef _LINEARIZEDCOMPUTATIONALGRAPHVERTEX_INCLUDE_
#define _LINEARIZEDCOMPUTATIONALGRAPHVERTEX_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/boostWrapper/inc/Vertex.hpp"

#include "xaifBooster/utils/inc/ObjectWithId.hpp"

#include "xaifBooster/system/inc/Variable.hpp"

#include "xaifBooster/algorithms/DerivativePropagator/inc/DerivativePropagator.hpp"

using namespace xaifBooster;

namespace xaifBoosterCrossCountryInterface { 

  class LinearizedComputationalGraphVertex : public Vertex {
  public:

    LinearizedComputationalGraphVertex() {};
    ~LinearizedComputationalGraphVertex() {};

  };
 
} 
                                                                     
#endif

