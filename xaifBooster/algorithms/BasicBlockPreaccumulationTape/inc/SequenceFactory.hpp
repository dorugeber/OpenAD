#ifndef _XAIFBOOSTERBASICBLOCKPREACCUMULATIONTAPE_SEQUENCEFACTORY_INCLUDE_
#define _XAIFBOOSTERBASICBLOCKPREACCUMULATIONTAPE_SEQUENCEFACTORY_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/SequenceFactory.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulationTape/inc/Sequence.hpp"

namespace xaifBoosterBasicBlockPreaccumulationTape {
 
  class SequenceFactory : public xaifBoosterBasicBlockPreaccumulation::SequenceFactory {

  public:

    virtual Sequence* makeNewSequence();

    virtual std::string debug() const;

  protected:
    friend class AlgFactoryManager;

    SequenceFactory(){};

  private:

    /// no def
    SequenceFactory(const SequenceFactory&);

    /// no def
    SequenceFactory operator= (const SequenceFactory&);

  };

}

#endif

