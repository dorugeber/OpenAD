#ifndef _XAIFBOOSTERTRACEDIFF_SUBROUTINECALLALG_INCLUDE_
#define _XAIFBOOSTERTRACEDIFF_SUBROUTINECALLALG_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/algorithms/TypeChange/inc/SubroutineCallAlg.hpp"

using namespace xaifBooster;

namespace xaifBooster { 
  class SubroutineCall;
}

namespace xaifBoosterTraceDiff {  

  /** 
   * class to implement tracing for certain subroutine calls
   */
  class SubroutineCallAlg : virtual public xaifBoosterTypeChange::SubroutineCallAlg {

  public:
    
    SubroutineCallAlg(const SubroutineCall& theContainingSubroutineCall);

    virtual ~SubroutineCallAlg();

    virtual std::string debug() const ;

    virtual void traverseToChildren(const GenericAction::GenericAction_E anAction_c);

    void printXMLHierarchy(std::ostream& os) const;
    
    virtual void algorithm_action_2(); 

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

    void handleArrayAccessIndices(const ConcreteArgument& theConcreteArgument); 

    /** 
     * call traces
     */
    PlainBasicBlock::BasicBlockElementList myCallTraces;

    /** 
     * return traces
     */
    PlainBasicBlock::BasicBlockElementList myReturnTraces;

  }; // end of class SubroutineCallAlg
 
} 
                                                                     
#endif
