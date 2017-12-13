// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/PrivateLinearizedComputationalGraphVertexAlgFactory.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/PrivateLinearizedComputationalGraphVertex.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/BasicBlockAlg.hpp"

namespace xaifBoosterBasicBlockPreaccumulation {
  PrivateLinearizedComputationalGraphVertexAlgFactory* PrivateLinearizedComputationalGraphVertexAlgFactory::ourPrivateLinearizedComputationalGraphVertexAlgFactoryInstance_p(0);
  xaifBoosterBasicBlockPreaccumulation ::PrivateLinearizedComputationalGraphVertexAlgFactory* PrivateLinearizedComputationalGraphVertexAlgFactory::instance () {
    if (!ourPrivateLinearizedComputationalGraphVertexAlgFactoryInstance_p)
      ourPrivateLinearizedComputationalGraphVertexAlgFactoryInstance_p=new PrivateLinearizedComputationalGraphVertexAlgFactory();
    return ourPrivateLinearizedComputationalGraphVertexAlgFactoryInstance_p;
  }

  std::string PrivateLinearizedComputationalGraphVertexAlgFactory::debug() const {
    std::ostringstream out;
    out << "xaifBoosterBasicBlockPreaccumulation::"
	<< "PrivateLinearizedComputationalGraphVertex"
	<< "AlgFactory["
	<< this
	<<"]"<<std::ends;
    return out.str();  
  }
}

