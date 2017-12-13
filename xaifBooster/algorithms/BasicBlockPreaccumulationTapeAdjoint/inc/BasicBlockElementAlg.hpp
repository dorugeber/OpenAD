#ifndef _XAIFBOOSTERBASICBLOCKPREACCUMULATIONTAPEADJOINT_BASICBLOCKELEMENTALG_INCLUDE_
#define _XAIFBOOSTERBASICBLOCKPREACCUMULATIONTAPEADJOINT_BASICBLOCKELEMENTALG_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/BasicBlock.hpp"
#include "xaifBooster/system/inc/BasicBlockElementAlgBase.hpp"

using namespace xaifBooster; 

namespace xaifBoosterBasicBlockPreaccumulationTapeAdjoint { 

  /**
   * class to implement reversals of BasicBlockElements
   */
  class BasicBlockElementAlg : public BasicBlockElementAlgBase {

  public:
    
    BasicBlockElementAlg(const BasicBlockElement& theContainingBasicBlockElement);

    virtual ~BasicBlockElementAlg();

    virtual void printXMLHierarchy(std::ostream& os) const;

    virtual std::string debug() const ;

    virtual void traverseToChildren(const GenericAction::GenericAction_E anAction_c);

    /** 
     * for this reversal scheme we are basically looking for 
     * SubRoutineCall elements to insert themselves
     */
    virtual void insertYourself(const BasicBlock& theBasicBlock) {};

  private: 

    /** 
     * no def
     */
    BasicBlockElementAlg();

    /** 
     * no def
     */
    BasicBlockElementAlg(const BasicBlockElementAlg&);

    /** 
     * no def
     */
    BasicBlockElementAlg& operator=(const BasicBlockElementAlg&);

  };
 
} 
                                                                     
#endif
