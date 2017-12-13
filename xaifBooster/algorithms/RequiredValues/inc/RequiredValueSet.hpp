#ifndef _XAIFBOOSTERREQUIREDVALUES_REQUIREDVALUESET_INCLUDE_
#define _XAIFBOOSTERREQUIREDVALUES_REQUIREDVALUESET_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include <list>

#include "xaifBooster/system/inc/ControlFlowGraphVertex.hpp"
#include "xaifBooster/system/inc/Expression.hpp"

#include "xaifBooster/algorithms/RequiredValues/inc/RequiredValue.hpp"

using namespace xaifBooster;

namespace xaifBoosterRequiredValues {  

  class RequiredValueSet {
  public:
    RequiredValueSet();
    ~RequiredValueSet();

    enum ComparisonResult_E {LESSTHAN,
                             GREATERTHAN,
                             EQUAL,
                             INCOMPARABLE};

    static std::string
    comparisonResultToString(ComparisonResult_E aComparisonResult);

    /**
     * 
     */
    void addValueToRequiredSet(const Expression& anExpression,
                               const ControlFlowGraphVertex& aControlFlowGraphVertex,
                               const std::string anOriginStr);

    std::string debug() const;

    typedef std::list<const RequiredValue*> RequiredValuePList;

    const RequiredValuePList& getRequiredValuesPList() const;

  private:

    /**
     * The set of required values.  We own the contents of this list (deleted in dtor)
     */
    RequiredValuePList myRequiredValuesPList;

    /// no def
    RequiredValueSet(const RequiredValueSet&);

    /// no def
    RequiredValueSet& operator=(const RequiredValueSet&);

  };  // end class RequiredValueSet

} // end namespace xaifBoosterRequiredValues

#endif

