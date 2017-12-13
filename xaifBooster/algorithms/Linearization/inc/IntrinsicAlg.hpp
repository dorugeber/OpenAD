#ifndef _XAIFBOOSTERLINEARIZATION_INTRINSICALG_INCLUDE_
#define _XAIFBOOSTERLINEARIZATION_INTRINSICALG_INCLUDE_
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
#include "xaifBooster/algorithms/Linearization/inc/ExpressionVertexAlg.hpp"

using namespace xaifBooster; 

namespace xaifBoosterLinearization { 

  /**
   * class to implement linearization steps
   */
  class IntrinsicAlg : public IntrinsicAlgBase,
		       public ExpressionVertexAlg { 

  public:
    
    IntrinsicAlg(Intrinsic& theContainingIntrinsic);

    virtual ~IntrinsicAlg();

    virtual void printXMLHierarchy(std::ostream& os) const;

    virtual std::string debug() const ;

    virtual void traverseToChildren(const GenericAction::GenericAction_E anAction_c);

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

  } ; 

} 
                                                                     
#endif
