// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/algorithms/ConstantFolding/inc/EnhancedPrivateLinearizedComputationalGraphAlgFactory.hpp"
#include "xaifBooster/algorithms/ConstantFolding/inc/BasicBlockAlg.hpp"
namespace xaifBoosterConstantFolding {

  EnhancedPrivateLinearizedComputationalGraphAlgFactory* EnhancedPrivateLinearizedComputationalGraphAlgFactory::ourPrivateLinearizedComputationalGraphAlgFactoryInstance_p=0;  

  EnhancedPrivateLinearizedComputationalGraph   *EnhancedPrivateLinearizedComputationalGraphAlgFactory::makeNewPrivateLinearizedComputationalGraph() {
    return (new EnhancedPrivateLinearizedComputationalGraph()); 
  }

  EnhancedPrivateLinearizedComputationalGraphAlgFactory* EnhancedPrivateLinearizedComputationalGraphAlgFactory::instance() {
    //  return BasicBlockAlg::getPrivateLinearizedComputationalGraphAlgFactory();
    if (!(ourPrivateLinearizedComputationalGraphAlgFactoryInstance_p))
      ourPrivateLinearizedComputationalGraphAlgFactoryInstance_p=new EnhancedPrivateLinearizedComputationalGraphAlgFactory;
    return ourPrivateLinearizedComputationalGraphAlgFactoryInstance_p;
  }
  
  std::string EnhancedPrivateLinearizedComputationalGraphAlgFactory::debug() const {
    std::ostringstream out;
    out << "xaifBoosterConstantFolding"
	<< "::"
	<< "EnhancedPrivateLinearizedComputationalGraph"
	<<  "AlgFactory["
	<< this
	<< "]"<<std::ends;
    return out.str();
  }

}
