#ifndef _BASICBLOCKELEMENTALGBASE_INCLUDE_
#define _BASICBLOCKELEMENTALGBASE_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/AlgBase.hpp"
#include "xaifBooster/system/inc/Expression.hpp"

namespace xaifBooster {  
  
  class BasicBlockElement;

  class BasicBlockElementAlgBase : public AlgBase<BasicBlockElement> {

  public:

    BasicBlockElementAlgBase(const BasicBlockElement& theContaining);

    virtual ~BasicBlockElementAlgBase();

    /// simply returns false (designed to be redefined by the classes that inherit from this)
    virtual bool hasExpression(const Expression& anExpression) const;

  private:

    /// no def
    BasicBlockElementAlgBase();

    /// no def
    BasicBlockElementAlgBase(const BasicBlockElementAlgBase&);

    /// no def
    BasicBlockElementAlgBase& operator=(const BasicBlockElementAlgBase&);

  }; // end class BasicBlockElementAlgBase

} // end namespace xaifBooster

#endif
