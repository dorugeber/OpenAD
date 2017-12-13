#ifndef _EXPLICITJACOBIAN_INCLUDE_
#define _EXPLICITJACOBIAN_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include <string>
#include <list>

#include "xaifBooster/system/inc/ExplicitJacobianPartial.hpp"

namespace xaifBooster { 

  class ExplicitJacobian { 
    
  public: 

    ExplicitJacobian();

    ~ExplicitJacobian();

    static const std::string ourXAIFName;
    static const std::string our_myJacobian_XAIFName;

    typedef std::list<ExplicitJacobianPartial*> ExplicitJacobianPartialPList;

    ExplicitJacobianPartial& addExplicitJacobianPartial();

    void setJacobianName(const std::string & aJacobianName);

  private:
    
    /**
     * no def
     */
    ExplicitJacobian(const ExplicitJacobian&);

    /**
     * no def
     */
    ExplicitJacobian& operator=(const ExplicitJacobian&);

    std::string myJacobianName;

    ExplicitJacobianPartialPList myExplicitJacobianPartialList;


  }; 
  
} 

#endif
