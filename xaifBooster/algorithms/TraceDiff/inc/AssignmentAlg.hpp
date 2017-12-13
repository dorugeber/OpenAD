#ifndef _XAIFBOOSTERTRACEDIFF_ASSIGNMENTALG_INCLUDE_
#define _XAIFBOOSTERTRACEDIFF_ASSIGNMENTALG_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/AssignmentAlgBase.hpp"
#include "xaifBooster/system/inc/Assignment.hpp"
#include "xaifBooster/system/inc/PlainBasicBlock.hpp"

using namespace xaifBooster; 

namespace xaifBoosterTraceDiff { 

  /**
   * class to implement TraceDiff steps
   * AssignmentAlgBase
   */
  class AssignmentAlg : public AssignmentAlgBase {

  public:
    
    AssignmentAlg(Assignment& theContainingAssignment);

    virtual ~AssignmentAlg(){};

    virtual void printXMLHierarchy(std::ostream& os) const;

    virtual std::string debug() const ;

    virtual void traverseToChildren(const GenericAction::GenericAction_E anAction_c);

    void trace();

  private: 

    /** 
     * no def
     */
    AssignmentAlg();

    /** 
     * no def
     */
    AssignmentAlg(const AssignmentAlg&);

    /** 
     * no def
     */
    AssignmentAlg& operator=(const AssignmentAlg&);

    PlainBasicBlock::BasicBlockElementList myTracingCalls;

  }; // end of class Assignment
 
} 
                                                                     
#endif
