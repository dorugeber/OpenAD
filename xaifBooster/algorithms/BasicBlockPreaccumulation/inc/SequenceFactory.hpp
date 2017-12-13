#ifndef _XAIFBOOSTERBASICBLOCKPREACCUMULATION_SEQUENCEFACTORY_INCLUDE_
#define _XAIFBOOSTERBASICBLOCKPREACCUMULATION_SEQUENCEFACTORY_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/Sequence.hpp"

namespace xaifBoosterBasicBlockPreaccumulation {
 
  class SequenceFactory : public xaifBooster::Debuggable {

  public:

    virtual ~SequenceFactory(){};

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

