// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/utils/inc/LogicException.hpp"

#include "xaifBooster/system/inc/FindDefinitionResult.hpp"
#include "xaifBooster/system/inc/Assignment.hpp"

namespace xaifBooster { 

  FindDefinitionResult::FindDefinitionResult(const BasicBlockElement* aDefinitionStatement_p) :
    myDefinitionStatement_p(aDefinitionStatement_p) {
  } 
    
  bool FindDefinitionResult::foundIt() { 
    return myDefinitionStatement_p!=0;
  }
  
  bool FindDefinitionResult::foundAssignment() { 
    return (foundIt() && dynamic_cast<const Assignment*>(myDefinitionStatement_p)!=0);
  }

  const Assignment& FindDefinitionResult::getAssignment() {
    const Assignment* assignment_p(dynamic_cast<const Assignment*>(myDefinitionStatement_p));
    if (!assignment_p)
      THROW_LOGICEXCEPTION_MACRO("FindDefinitionResult::getAssignment: no assignment found");
    return *assignment_p;
  }

} 
