#ifndef _PRIVATELINEARIZEDCOMPUTATIONALGRAPHVERTEXFACTORY_INCLUDE_
#define _PRIVATELINEARIZEDCOMPUTAIIONALGRAPHVERTEXFACTORY_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/PrivateLinearizedComputationalGraphVertex.hpp"

namespace xaifBoosterBasicBlockPreaccumulation {
  
  class PrivateLinearizedComputationalGraphVertexAlgFactory : public xaifBooster::Debuggable {

  public:
    static PrivateLinearizedComputationalGraphVertexAlgFactory* instance();

    virtual ~PrivateLinearizedComputationalGraphVertexAlgFactory(){};

    virtual PrivateLinearizedComputationalGraphVertex* makeNewPrivateLinearizedComputationalGraphVertex() {
      return new PrivateLinearizedComputationalGraphVertex(); 
    };

    virtual std::string debug() const;
   
  protected:
    
    friend class BasicBlockAlg;

    PrivateLinearizedComputationalGraphVertexAlgFactory(){};

  private:

    static PrivateLinearizedComputationalGraphVertexAlgFactory* ourPrivateLinearizedComputationalGraphVertexAlgFactoryInstance_p;

    PrivateLinearizedComputationalGraphVertexAlgFactory (const PrivateLinearizedComputationalGraphVertexAlgFactory&);

    PrivateLinearizedComputationalGraphVertexAlgFactory operator= (const PrivateLinearizedComputationalGraphVertexAlgFactory&);
  };
}

#endif

