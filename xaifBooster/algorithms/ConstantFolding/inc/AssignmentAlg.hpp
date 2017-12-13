#ifndef _XAIFBOOSTERCONSTANTFOLDING_ASSIGNMENTALG_INCLUDE_
#define _XAIFBOOSTERCONSTANTFOLDING_ASSIGNMENTALG_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/AssignmentAlg.hpp"
#include "xaifBooster/system/inc/Assignment.hpp"

using namespace xaifBooster; 

namespace xaifBoosterConstantFolding { 

  /**
   * class to implement ConstantFolding steps
   * AssignmentAlgBase
   */
  class AssignmentAlg : public xaifBoosterBasicBlockPreaccumulation::AssignmentAlg {

  public:
    AssignmentAlg(Assignment& theContainingAssignment);
//    void AssignmentAlg::algorithm_action_3();
  private: 
 
  }; // end of class Assignment
 
} 
                                                                     
#endif
