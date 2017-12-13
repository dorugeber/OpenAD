#ifndef _ENHANCEDPRIVATELINEARIZEDCOMPUTATIONALGRAPHALGFACTORY_INCLUDE_
#define _ENHANCEDPRIVATELINEARIZEDCOMPUTATIONALGRAPHALGFACTORY_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/algorithms/ConstantFolding/inc/EnhancedPrivateLinearizedComputationalGraph.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/PrivateLinearizedComputationalGraphAlgFactory.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/PrivateLinearizedComputationalGraph.hpp"


using namespace xaifBooster;

namespace xaifBoosterConstantFolding {

  class EnhancedPrivateLinearizedComputationalGraphAlgFactory : public xaifBoosterBasicBlockPreaccumulation::PrivateLinearizedComputationalGraphAlgFactory {

  public:
    
    static  EnhancedPrivateLinearizedComputationalGraphAlgFactory *instance();

    virtual ~EnhancedPrivateLinearizedComputationalGraphAlgFactory(){};

    virtual EnhancedPrivateLinearizedComputationalGraph * makeNewPrivateLinearizedComputationalGraph();

    virtual std::string debug() const;

  protected:
    
    friend class BasicBlockAlg;
    
    EnhancedPrivateLinearizedComputationalGraphAlgFactory(){};

  private:

    static EnhancedPrivateLinearizedComputationalGraphAlgFactory* ourPrivateLinearizedComputationalGraphAlgFactoryInstance_p;

    EnhancedPrivateLinearizedComputationalGraphAlgFactory (const EnhancedPrivateLinearizedComputationalGraphAlgFactory&);

    EnhancedPrivateLinearizedComputationalGraphAlgFactory operator= (const EnhancedPrivateLinearizedComputationalGraphAlgFactory&);
  };
}
#endif
