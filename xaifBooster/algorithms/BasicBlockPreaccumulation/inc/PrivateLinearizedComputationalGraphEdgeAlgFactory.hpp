#ifndef _PRIVATELINEARIZEDCOMPUTATIONALGRAPHEDGEALGFACTORY_INCLUDE_
#define _PRIVATELINEARIZEDCOMPUTAIIONALGRAPHEDGEALGFACTORY_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/PrivateLinearizedComputationalGraphEdge.hpp"

namespace xaifBoosterBasicBlockPreaccumulation {
  
  class PrivateLinearizedComputationalGraphEdgeAlgFactory : public xaifBooster::Debuggable {

  public:
    static  PrivateLinearizedComputationalGraphEdgeAlgFactory* instance();

    virtual ~PrivateLinearizedComputationalGraphEdgeAlgFactory(){};

    virtual PrivateLinearizedComputationalGraphEdge* makeNewPrivateLinearizedComputationalGraphEdge() {
      return new PrivateLinearizedComputationalGraphEdge();    
    };
  
    virtual std::string debug() const;

  protected:
    
    friend class BasicBlockAlg;

    PrivateLinearizedComputationalGraphEdgeAlgFactory(){};
  private:

    static PrivateLinearizedComputationalGraphEdgeAlgFactory* ourPrivateLinearizedComputationalGraphEdgeAlgFactoryInstance_p;

    PrivateLinearizedComputationalGraphEdgeAlgFactory (const PrivateLinearizedComputationalGraphEdgeAlgFactory&);

    PrivateLinearizedComputationalGraphEdgeAlgFactory operator= (const PrivateLinearizedComputationalGraphEdgeAlgFactory&);
  };
}

#endif
