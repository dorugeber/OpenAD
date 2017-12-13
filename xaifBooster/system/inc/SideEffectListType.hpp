#ifndef _SIDEEFFECTLISTTYPE_INCLUDE_
#define _SIDEEFFECTLISTTYPE_INCLUDE_
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
#include <vector>
#include "xaifBooster/utils/inc/PrintingIntException.hpp"

namespace xaifBooster { 
  class SideEffectListType { 
  public:
    enum SideEffectListType_E {MOD_LOCAL_LIST=0,
			       MOD_LIST=1,
			       READ_LOCAL_LIST=2,
			       READ_LIST=3,
                               ON_ENTRY_LIST=4,
                               DUMMY_LAST_ENTRY=5};

    static unsigned short numTypes() {return DUMMY_LAST_ENTRY;}

    static const std::list<SideEffectListType_E>& getTypeList();

    static const std::vector<std::string>& getXAIFNames();
    
    static const std::string our_Mod_XAIFName;
    static const std::string our_ModLocal_XAIFName;
    static const std::string our_Read_XAIFName;
    static const std::string our_ReadLocal_XAIFName;
    static const std::string our_OnEntry_XAIFName;
    
  private:

    static std::list<SideEffectListType_E> ourTypeList;
    
    static std::vector<std::string> ourXAIFNames;

  }; // end of class SideEffectListType

} // end of namespace xaifBooster
                                                                     
#endif
