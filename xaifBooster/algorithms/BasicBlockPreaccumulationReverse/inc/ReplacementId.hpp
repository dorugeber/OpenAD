#ifndef _XAIFBOOSTERBASICBLOCKPREACCUMULATIONREVERSE_REPLACEMENTID_INCLUDE_
#define _XAIFBOOSTERBASICBLOCKPREACCUMULATIONREVERSE_REPLACEMENTID_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include <string>
#include "xaifBooster/utils/inc/PrintingIntException.hpp"

using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulationReverse {  

  class ReplacementId { 

  public:

    ReplacementId();

    enum ReplacementId_E { 
      ORIGINAL=1,
      TAPING=2,
      ADJOINT=3,
      STOREARGUMENT=4,
      STORERESULT=5,
      RESTOREARGUMENT=6,
      RESTORERESULT=7,
      STORETIMESTEPARGUMENT=8,
      RESTORETIMESTEPARGUMENT=9,
      STRICTANONYMOUSTAPING=10,
      STRICTANONYMOUSADJOINT=11,
      STORETIMESTEPRESULT=12,
      RESTORETIMESTEPRESULT=13
    };

    static std::string toString(const ReplacementId_E& aKind) throw (PrintingIntException);

    static const ReplacementId_E fromString(const std::string& aName);

    void reset();

    bool atEnd();

    ReplacementId_E operator*() const;

    ReplacementId& operator++();

  private:

    ReplacementId_E myCurrent;

  }; // end of class ReplacementId

} // end of namespace
                                                                     
#endif
