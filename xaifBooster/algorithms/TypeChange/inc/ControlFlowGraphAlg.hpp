#ifndef _XAIFBOOSTERTYPECHANGE_CONTROLFLOWGRAPHALG_INCLUDE_
#define _XAIFBOOSTERTYPECHANGE_CONTROLFLOWGRAPHALG_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/ControlFlowGraphAlgBase.hpp"
#include "xaifBooster/system/inc/ControlFlowGraph.hpp"
#include "xaifBooster/algorithms/TypeChange/inc/SignaturePattern.hpp"

using namespace xaifBooster;

namespace xaifBoosterTypeChange {  

  /** 
   * class to implement renaming of subroutine definitions
   * if enforced
   */
  class ControlFlowGraphAlg : public ControlFlowGraphAlgBase {
  public:
    
    ControlFlowGraphAlg(const ControlFlowGraph& theContaining);

    virtual ~ControlFlowGraphAlg();

    virtual void printXMLHierarchy(std::ostream& os) const;

    virtual void algorithm_action_1();
                                                                                
    virtual std::string debug() const ;

    virtual void traverseToChildren(const GenericAction::GenericAction_E anAction_c);

    static void setForceNonExternalRenames();
    
    bool forceNonExternalRenames() const;

    SignaturePattern& getSomewhereVariablePattern(); 

    SignaturePattern& getSomewhereConstPattern(); 

  private:
    
    /** 
     * no def
     */
    ControlFlowGraphAlg();

    /** 
     * no def
     */
    ControlFlowGraphAlg(const ControlFlowGraphAlg&);

    /** 
     * no def
     */
    ControlFlowGraphAlg& operator=(const ControlFlowGraphAlg&);

    /** 
     * force renames of all subroutine calls/definitions
     * (non-external) 
     */
    static bool ourForceNonExternalRenamesFlag;

    /** 
     * We cannot simply restore values to 
     * formal parameters called somewhere with constant actuals.
     */
    SignaturePattern mySomewhereConstPattern;

    /** 
     * for arguments that are always called with constants; 
     * we can skip taping/checkpointing
     */
    SignaturePattern mySomewhereVariablePattern;

  };  

} 
                                                                     
#endif
