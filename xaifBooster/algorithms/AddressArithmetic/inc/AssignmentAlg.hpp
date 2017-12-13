#ifndef _XAIFBOOSTERADDRESSARITHMETIC_ASSIGNMENTALG_INCLUDE_
#define _XAIFBOOSTERADDRESSARITHMETIC_ASSIGNMENTALG_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/algorithms/BasicBlockPreaccumulationTapeAdjoint/inc/AssignmentAlg.hpp"

using namespace xaifBooster;

namespace xaifBoosterAddressArithmetic {  

  /** 
   * class to pull together 
   * the taping and the adjoining 
   * view per Assignment; 
   * we just need to reimplement printing
   */
  class AssignmentAlg : public xaifBoosterBasicBlockPreaccumulationTapeAdjoint::AssignmentAlg {

  public:
    
    AssignmentAlg(Assignment& theContaining);

    virtual ~AssignmentAlg() {};

    virtual void printXMLHierarchy(std::ostream& os) const;

    virtual std::string debug() const ;

    virtual void traverseToChildren(const GenericAction::GenericAction_E anAction_c);

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

  };
 
} // end of namespace 
                                                                     
#endif
