#ifndef _XAIFBOOSTERBASICBLOCKPREACCUMULATIONREVERSE_SUBROUTINECALLALG_INCLUDE_
#define _XAIFBOOSTERBASICBLOCKPREACCUMULATIONREVERSE_SUBROUTINECALLALG_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/algorithms/BasicBlockPreaccumulationTapeAdjoint/inc/SubroutineCallAlg.hpp"

using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulationReverse {  

  /** 
   * class to pull together 
   * the taping and the adjoining 
   * view per SubroutineCall; 
   * we just need to reimplement printing
   */
  class SubroutineCallAlg : public xaifBoosterBasicBlockPreaccumulationTapeAdjoint::SubroutineCallAlg {

  public:
    
    SubroutineCallAlg(SubroutineCall& theContaining);

    virtual ~SubroutineCallAlg() {};

    virtual void printXMLHierarchy(std::ostream& os) const;

    virtual std::string debug() const ;

    virtual void traverseToChildren(const GenericAction::GenericAction_E anAction_c);

    /**
     * refering to xaifBoosterLinearization::SubroutineCallAlg::algorithm_action_1
     */
    virtual void algorithm_action_1();

  private:

    /** 
     * no def
     */
    SubroutineCallAlg();

    /** 
     * no def
     */
    SubroutineCallAlg(const SubroutineCallAlg&);

    /** 
     * no def
     */
    SubroutineCallAlg& operator=(const SubroutineCallAlg&);

  };
 
} // end of namespace 
                                                                     
#endif
