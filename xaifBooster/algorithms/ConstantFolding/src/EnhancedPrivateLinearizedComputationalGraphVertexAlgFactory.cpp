// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/algorithms/ConstantFolding/inc/EnhancedPrivateLinearizedComputationalGraphVertexAlgFactory.hpp"
#include "xaifBooster/algorithms/ConstantFolding/inc/BasicBlockAlg.hpp"
namespace xaifBoosterConstantFolding {

  EnhancedPrivateLinearizedComputationalGraphVertexAlgFactory* EnhancedPrivateLinearizedComputationalGraphVertexAlgFactory::ourPrivateLinearizedComputationalGraphVertexAlgFactoryInstance_p=0;
  
  EnhancedPrivateLinearizedComputationalGraphVertex *EnhancedPrivateLinearizedComputationalGraphVertexAlgFactory::makeNewPrivateLinearizedComputationalGraphVertex() {
    return (new EnhancedPrivateLinearizedComputationalGraphVertex()); 
  }

  EnhancedPrivateLinearizedComputationalGraphVertexAlgFactory* EnhancedPrivateLinearizedComputationalGraphVertexAlgFactory::instance() {
    //    return BasicBlockAlg::getPrivateLinearizedComputationalGraphVertexAlgFactory();
    if (!ourPrivateLinearizedComputationalGraphVertexAlgFactoryInstance_p)
      ourPrivateLinearizedComputationalGraphVertexAlgFactoryInstance_p=new EnhancedPrivateLinearizedComputationalGraphVertexAlgFactory;
    return ourPrivateLinearizedComputationalGraphVertexAlgFactoryInstance_p;
  }
  
  std::string EnhancedPrivateLinearizedComputationalGraphVertexAlgFactory::debug() const {
    std::ostringstream out;
    out << "xaifBoosterConstantFolding"
	<< "::"
	<< "EnhancedPrivateLinearizedComputationalGraphVertex"
	<<  "AlgFactory["
	<< this
	<< "]"<<std::ends;
    return out.str();
  }

}
