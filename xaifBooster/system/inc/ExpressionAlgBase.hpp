#ifndef _EXPRESSIONALGBASE_INCLUDE_
#define _EXPRESSIONALGBASE_INCLUDE_
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

namespace xaifBooster {  
  
  class Expression;

  class ExpressionAlgBase : public AlgBase<Expression> {

  public:

    ExpressionAlgBase(const Expression& theContaining);

    virtual ~ExpressionAlgBase();

    virtual std::string debug() const;

    /// simply returns false (designed to be redefined by the classes that inherit from this)
    virtual bool hasExpression(const Expression& anExpression) const;

  private:

    /// no def
    ExpressionAlgBase();

    /// no def
    ExpressionAlgBase(const ExpressionAlgBase&);

    /// no def
    ExpressionAlgBase& operator=(const ExpressionAlgBase&);

  }; // end class ExpressionAlgBase

} // end of namespace 

#endif
