#ifndef _XAIFBOOSTERLINEARIZATION_CONSTANTALG_INCLUDE_
#define _XAIFBOOSTERLINEARIZATION_CONSTANTALG_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/ConstantAlgBase.hpp"
#include "xaifBooster/algorithms/Linearization/inc/ExpressionVertexAlg.hpp"

using namespace xaifBooster; 

namespace xaifBoosterLinearization { 

  /**
   * class to implement linearization steps
   */
  class ConstantAlg : public ConstantAlgBase,
		      public ExpressionVertexAlg { 

  public:
    
    ConstantAlg(Constant& theContainingConstant);

    virtual ~ConstantAlg();

    virtual void printXMLHierarchy(std::ostream& os) const;

    virtual std::string debug() const ;

    virtual void traverseToChildren(const GenericAction::GenericAction_E anAction_c);

  private: 

    /** 
     * no def
     */
    ConstantAlg();

    /** 
     * no def
     */
    ConstantAlg(const ConstantAlg&);

    /** 
     * no def
     */
    ConstantAlg& operator=(const ConstantAlg&);

  } ; 

} 
                                                                     
#endif
