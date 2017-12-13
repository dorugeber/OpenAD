// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/utils/inc/PrintManager.hpp"

namespace xaifBooster { 

  PrintManager* PrintManager::ourPrintManagerInstance_p;
  const unsigned int PrintManager::ourDefaultIndentation=2;
  bool PrintManager::ourVerbosity=false;
  
  PrintManager&
  PrintManager::getInstance() { 
    if (!ourPrintManagerInstance_p)
      ourPrintManagerInstance_p=new PrintManager();
    ourPrintManagerInstance_p->my_indentation+=ourDefaultIndentation;
    return *ourPrintManagerInstance_p;
  } // end of PrintManager::getInstance

  PrintManager::PrintManager() :
    my_indentation(0){
  }
  
  void PrintManager::releaseInstance() { 
    my_indentation-=ourDefaultIndentation;
  } // end of PrintManager::releaseInstance

  std::string 
  PrintManager::indent() { 
    return std::string(my_indentation,' ');
  } // end of indent

  std::string 
  PrintManager::indent(unsigned int additionalIndent) { 
    return std::string(my_indentation+additionalIndent,' ');
  } // end of indent

  void PrintManager::setVerbose() { 
    ourVerbosity=true;
  } 

  bool PrintManager::isVerbose() { 
    return ourVerbosity;
  }

} // end of namespace 
