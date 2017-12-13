// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/utils/inc/PrintManager.hpp"

#include "xaifBooster/system/inc/Assignment.hpp"

#include "xaifBooster/algorithms/TraceDiff/inc/BasicBlockAlg.hpp"
#include "xaifBooster/algorithms/TraceDiff/inc/AssignmentAlg.hpp"

using namespace xaifBooster;

namespace xaifBoosterTraceDiff { 

  BasicBlockAlg::BasicBlockAlg(BasicBlock& theContaining) : 
    BasicBlockAlgBase(theContaining),
    xaifBoosterControlFlowReversal::BasicBlockAlg(theContaining) {
  }

  std::string BasicBlockAlg::debug () const { 
    std::ostringstream out;
    out << "xaifBoosterTraceDiff::BasicBlockAlg[" << this
 	<< "]" << std::ends;  
    return out.str();
  } // end of BasicBlockAlg::debug

  void BasicBlockAlg::traverseToChildren(const GenericAction::GenericAction_E anAction_c) { 
  } 

  void BasicBlockAlg::algorithm_action_2() { 
    DBG_MACRO(DbgGroup::CALLSTACK, "xaifBoosterTraceDiff::BasicBlockAlg::algorithm_action_2()");
    // for every assignment check the RHS
    const PlainBasicBlock::BasicBlockElementList& theContainingBBEelemtList(dynamic_cast<const BasicBlock&>(BasicBlockAlgBase::getContaining()).getBasicBlockElementList());
    for (PlainBasicBlock::BasicBlockElementList::const_iterator li=theContainingBBEelemtList.begin();
	   li!=theContainingBBEelemtList.end();
	   li++) {
      // see if this is an assignment
      const Assignment* anAssignment_p=dynamic_cast<const Assignment*>(*li);
      if (anAssignment_p) { 
        dynamic_cast<AssignmentAlg&>(anAssignment_p->getAssignmentAlgBase()).trace();
      }
    } 
  } // end BasicBlockAlg::algorithm_action_2()
  
} // end of namespace
