#ifndef _LABELALGBASE_INCLUDE_
#define _LABELALGBASE_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/ControlFlowGraphVertexAlgBase.hpp"

namespace xaifBooster {  
  
  class Label;

  class LabelAlgBase: public ControlFlowGraphVertexAlgBase { 

  public: 

    LabelAlgBase(const Label& theContaining);

    virtual ~LabelAlgBase();

  private: 

    /** 
     * not defined
     */
    LabelAlgBase();

    /** 
     * not defined
     */
    LabelAlgBase(const LabelAlgBase&);

    /** 
     * not defined
     */
    LabelAlgBase& operator=(const LabelAlgBase&);

  }; 

} // end of namespace 

#endif
