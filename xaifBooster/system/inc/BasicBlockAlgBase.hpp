#ifndef _BASICBLOCKALGBASE_INCLUDE_
#define _BASICBLOCKALGBASE_INCLUDE_
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
  
  class BasicBlock;

  class BasicBlockAlgBase: public ControlFlowGraphVertexAlgBase { 

  public: 

    BasicBlockAlgBase(const BasicBlock& theContaining);

    virtual ~BasicBlockAlgBase();

    virtual std::string debug() const;

  private: 

    /** 
     * not defined
     */
    BasicBlockAlgBase();

    /** 
     * not defined
     */
    BasicBlockAlgBase(const BasicBlockAlgBase&);

    /** 
     * not defined
     */
    BasicBlockAlgBase& operator=(const BasicBlockAlgBase&);

  }; 

} // end of namespace 

#endif
