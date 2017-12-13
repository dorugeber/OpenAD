#ifndef _ENTRYALGBASE_INCLUDE_
#define _ENTRYALGBASE_INCLUDE_
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
  
  class Entry;

  class EntryAlgBase: public ControlFlowGraphVertexAlgBase { 

  public: 

    EntryAlgBase(const Entry& theContaining);

    virtual ~EntryAlgBase();

  private: 

    /** 
     * not defined
     */
    EntryAlgBase();

    /** 
     * not defined
     */
    EntryAlgBase(const EntryAlgBase&);

    /** 
     * not defined
     */
    EntryAlgBase& operator=(const EntryAlgBase&);

  }; 

} // end of namespace 

#endif
