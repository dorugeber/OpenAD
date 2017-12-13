// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/BasicBlockElementAlgBase.hpp"
#include "xaifBooster/system/inc/BasicBlockElement.hpp"

namespace xaifBooster {

  BasicBlockElementAlgBase::BasicBlockElementAlgBase(const BasicBlockElement& theContaining) :
    AlgBase<BasicBlockElement>(theContaining) {
  } // end BasicBlockElementAlgBase::BasicBlockElementAlgBase()

  BasicBlockElementAlgBase::~BasicBlockElementAlgBase() {
  } // end BasicBlockElementAlgBase::~BasicBlockElementAlgBase()

  bool
  BasicBlockElementAlgBase::hasExpression(const Expression& anExpression) const {
    return false;
  } // end BasicBlockElementAlgBase::hasExpression()

} // end namespace xaifBooster
