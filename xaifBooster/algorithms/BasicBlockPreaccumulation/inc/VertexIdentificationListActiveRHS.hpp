#ifndef _VERTEXIDENTIFICATIONLISTACTIVERHS_INCLUDE_
#define _VERTEXIDENTIFICATIONLISTACTIVERHS_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/Argument.hpp"
#include "xaifBooster/system/inc/Assignment.hpp"
#include "xaifBooster/system/inc/Variable.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/VertexIdentificationListActive.hpp"

using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulation { 

  /**
   * this is for the identification fo active variables
   * RHS <-> RHS
   * in the presence of ud information
   */
  class VertexIdentificationListActiveRHS : public VertexIdentificationListActive {

  public:

    IdentificationResult canIdentify(const Variable& theVariable) const;

    /** 
     * in case of duud information present we keep adding 
     * variables, 
     * in case of duud information absent we have to maintain 
     * uniqueness, i.e. 
     * this will only work if canIdentify returns
     * NOT_IDENTIFIED 
     */
    virtual void addElement(const Argument& theArgument,
                            const Assignment& theAssignment,
                            const StatementIdList& theKnownAssignmentsList);
    
  }; // end of class VertexIdentificationListActiveRHS  
   
} // end namespace 
                                                                  
#endif
