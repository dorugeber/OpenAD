// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/utils/inc/LogicException.hpp"
#include "xaifBooster/system/inc/SideEffectListType.hpp"

namespace xaifBooster { 
  
  const std::string SideEffectListType::our_Mod_XAIFName("xaif:Mod");
  const std::string SideEffectListType::our_ModLocal_XAIFName("xaif:ModLocal");
  const std::string SideEffectListType::our_Read_XAIFName("xaif:Read");
  const std::string SideEffectListType::our_ReadLocal_XAIFName("xaif:ReadLocal");
  const std::string SideEffectListType::our_OnEntry_XAIFName("xaif:OnEntry");

  std::list<SideEffectListType::SideEffectListType_E> SideEffectListType::ourTypeList;
  
  std::vector<std::string> SideEffectListType::ourXAIFNames;

  const std::list<SideEffectListType::SideEffectListType_E>& SideEffectListType::getTypeList() { 
    if (ourTypeList.empty()) { 
      ourTypeList.push_back(MOD_LOCAL_LIST);
      ourTypeList.push_back(MOD_LIST);
      ourTypeList.push_back(READ_LOCAL_LIST);
      ourTypeList.push_back(READ_LIST);
      ourTypeList.push_back(ON_ENTRY_LIST);
    }
    return ourTypeList;
  }
  
  const std::vector<std::string>& SideEffectListType::getXAIFNames() {
    if (ourXAIFNames.empty()) { 
      ourXAIFNames.push_back(const_cast<std::string&>(our_ModLocal_XAIFName));
      ourXAIFNames.push_back(const_cast<std::string&>(our_Mod_XAIFName));
      ourXAIFNames.push_back(const_cast<std::string&>(our_ReadLocal_XAIFName));
      ourXAIFNames.push_back(const_cast<std::string&>(our_Read_XAIFName));
      ourXAIFNames.push_back(const_cast<std::string&>(our_OnEntry_XAIFName));
    }
    return ourXAIFNames;
  }
  
} // end of namespace xaifBooster
