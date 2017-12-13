#ifndef _IFSTATEMENTALGBASE_INCLUDE_
#define _IFSTATEMENTALGBASE_INCLUDE_
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
  
  class IfStatement;

  class IfStatementAlgBase: public ControlFlowGraphVertexAlgBase { 

  public: 

    IfStatementAlgBase(const IfStatement& theContaining);

    virtual ~IfStatementAlgBase();

  private: 

    /** 
     * not defined
     */
    IfStatementAlgBase();

    /** 
     * not defined
     */
    IfStatementAlgBase(const IfStatementAlgBase&);

    /** 
     * not defined
     */
    IfStatementAlgBase& operator=(const IfStatementAlgBase&);

  }; 

} // end of namespace 

#endif
