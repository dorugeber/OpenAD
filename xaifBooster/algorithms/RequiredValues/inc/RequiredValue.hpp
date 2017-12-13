#ifndef _XAIFBOOSTERREQUIREDVALUES_REQUIREDVALUE_INCLUDE_
#define _XAIFBOOSTERREQUIREDVALUES_REQUIREDVALUE_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/ControlFlowGraphVertex.hpp"
#include "xaifBooster/system/inc/Expression.hpp"

using namespace xaifBooster;

namespace xaifBoosterRequiredValues {  

  /**
   * Class for representing a value that is required for the adjoint sweep,
   * along with a place that it's required.
   */
  class RequiredValue : public Debuggable {
  public:

    RequiredValue(const Expression& anExpression,
                  const ControlFlowGraphVertex& aControlFlowGraphVertex,
                  const std::string& anOriginStr);

    ~RequiredValue();

    std::string debug() const;

    const Expression& getExpression() const;

    const ControlFlowGraphVertex& getControlFlowGraphVertex() const;

    std::string getOriginStr() const;

    bool isArgument() const;

    const Argument& getArgument() const;

    void populateArgumentPList(Expression::CArgumentPList argumentListToBePopulated) const;

  private:

    /// the expression whose value is required
    const Expression* myExpression_p;

    /// the ControlFlowGraph that contains the expression
    const ControlFlowGraphVertex* myControlFlowGraphVertex_p;

    /// a string that describes where this value was determined to be required
    const std::string myOriginStr;

    /// no def
    RequiredValue();

    /// no def
    RequiredValue(const RequiredValue&);

    /// no def
    RequiredValue& operator=(const RequiredValue&);

  }; // end class RequiredValue

} // end namespace xaifBoosterRequiredValues

#endif

