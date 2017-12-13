#ifndef _SUBROUTINECALLALGBASE_INCLUDE_
#define _SUBROUTINECALLALGBASE_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/BasicBlockElementAlgBase.hpp"

namespace xaifBooster {  
  
  class SubroutineCall;

  class SubroutineCallAlgBase: public BasicBlockElementAlgBase { 

  public: 

    SubroutineCallAlgBase(const SubroutineCall& theContaining);

    virtual ~SubroutineCallAlgBase();

    const SubroutineCall& getContainingSubroutineCall() const;

  private: 

    /** 
     * not defined
     */
    SubroutineCallAlgBase();

    /** 
     * not defined
     */
    SubroutineCallAlgBase(const SubroutineCallAlgBase&);

    /** 
     * not defined
     */
    SubroutineCallAlgBase& operator=(const SubroutineCallAlgBase&);

  }; 

} // end of namespace 

#endif
