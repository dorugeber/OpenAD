// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/system/inc/SubroutineNotFoundException.hpp"
#include "xaifBooster/system/inc/SymbolReference.hpp"

namespace xaifBooster { 

  SubroutineNotFoundException::SubroutineNotFoundException(const SymbolReference& aSymbolReference) :
    mySymbolReference(aSymbolReference) { 
    std::ostringstream reason;
    reason << "SubroutineNotFoundException: " << aSymbolReference.debug().c_str();
    convertToOneLineReason(reason);
  } // end of SubroutineNotFoundException::SubroutineNotFoundException

  const SymbolReference& SubroutineNotFoundException::getSymbolReference()const { 
    return mySymbolReference;
  } 
  
} // end of namespace xaifBooster
                                                                     


