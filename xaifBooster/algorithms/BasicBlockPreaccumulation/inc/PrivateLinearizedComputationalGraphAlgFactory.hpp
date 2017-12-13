#ifndef _PRIVATELINEARIZEDCOMPUTATIONALGRAPHALGFACTORY_INCLUDE_
#define _PRIVATELINEARIZEDCOMPUTATIONALGRAPHALGFACTORY_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/PrivateLinearizedComputationalGraph.hpp"

namespace xaifBoosterBasicBlockPreaccumulation {
 
  class PrivateLinearizedComputationalGraphAlgFactory : public xaifBooster::Debuggable {

  public:
    static PrivateLinearizedComputationalGraphAlgFactory* instance();

    virtual ~PrivateLinearizedComputationalGraphAlgFactory(){};

    virtual PrivateLinearizedComputationalGraph* makeNewPrivateLinearizedComputationalGraph() {
      return new PrivateLinearizedComputationalGraph();
    };
    
    virtual std::string debug() const;
   
  protected:
    
    friend class BasicBlockAlg;

    PrivateLinearizedComputationalGraphAlgFactory(){};

  private:

    static PrivateLinearizedComputationalGraphAlgFactory* ourPrivateLinearizedComputationalGraphAlgFactoryInstance_p;

    PrivateLinearizedComputationalGraphAlgFactory (const PrivateLinearizedComputationalGraphAlgFactory&);

    PrivateLinearizedComputationalGraphAlgFactory operator= (const PrivateLinearizedComputationalGraphAlgFactory&);
  };
}

#endif
