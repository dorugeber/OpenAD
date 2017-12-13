#ifndef _XAIFBOOSTERTRACEDIFF_INTRINSICALG_INCLUDE_
#define _XAIFBOOSTERTRACEDIFF_INTRINSICALG_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/IntrinsicAlgBase.hpp"

using namespace xaifBooster; 

namespace xaifBoosterTraceDiff { 

  /**
   * class to implement tracing
   */
  class IntrinsicAlg : public IntrinsicAlgBase { 

  public:
    
    IntrinsicAlg(Intrinsic& theContainingIntrinsic);

    virtual ~IntrinsicAlg();

    virtual void printXMLHierarchy(std::ostream& os) const;

    virtual std::string debug() const ;

    virtual void traverseToChildren(const GenericAction::GenericAction_E anAction_c);

    virtual void algorithm_action_2(); 

  private: 

    /** 
     * no def
     */
    IntrinsicAlg();

    /** 
     * no def
     */
    IntrinsicAlg(const IntrinsicAlg&);

    /** 
     * no def
     */
    IntrinsicAlg& operator=(const IntrinsicAlg&);

    /**
     * flag to rename this to call a tracing-enabled wrapper for 
     * the intrinsic
     */
    bool myRenameFlag;

  } ; 

} 
                                                                     
#endif
