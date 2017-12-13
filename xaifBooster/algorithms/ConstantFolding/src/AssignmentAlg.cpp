// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include <sstream>

#include "xaifBooster/utils/inc/DbgLoggerManager.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/AssignmentAlg.hpp"

#include "xaifBooster/algorithms/ConstantFolding/inc/AssignmentAlg.hpp"
#include "xaifBooster/algorithms/ConstantFolding/inc/EnhancedPrivateLinearizedComputationalGraph.hpp"
#include "xaifBooster/algorithms/ConstantFolding/inc/BasicBlockAlg.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/PrivateLinearizedComputationalGraph.hpp"

namespace xaifBoosterConstantFolding { 

  AssignmentAlg::AssignmentAlg(Assignment& theContainingAssignment) :    
    xaifBoosterBasicBlockPreaccumulation::AssignmentAlg(theContainingAssignment) { 
  }

//  void AssignmentAlg::algorithm_action_3() {
    /*    //we need to get the BasicBlock, and then iterator thoguht the sequence and get every PrivateLinearizedComputationalGRaph and perform the opperations upon it.
    // we accomplish this by ..
    BasicBlock BB = ;//stuff
    BasicBlockAlg::Sequence seq = ;//stuff
    //Other needed Information
    for( ; //something
	 ; //something!=Something
	 ;) { //something
      PrivateLinearizedComputationalGraph p = ;//stuff
      EnhancedPrivateLinearizedComputationalGraph g=p;
      g.RunConstantCheckForwardBackward();
      g.RunTrivialCheckForward();
      g.RunTrivialCheckBackward();
      }*/
//    }
  
}
