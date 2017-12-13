#ifndef _SUBROUTINENOTFOUNDEXCEPTION_INCLUDE_
#define _SUBROUTINENOTFOUNDEXCEPTION_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/utils/inc/BaseException.hpp"

namespace xaifBooster { 

  class SymbolReference;

  /** 
   * as the name says we have some problem finding 
   * a certain subroutine 
   * which may be a non-fatal error, hence the
   * separate exception class 
   */
  class SubroutineNotFoundException : public BaseException { 

  public:

    SubroutineNotFoundException(const SymbolReference& aSymbolReference);

    ~SubroutineNotFoundException(){};

    const SymbolReference& getSymbolReference()const;

  private: 

    const SymbolReference& mySymbolReference;

  }; 
  
} // end of namespace xaifBooster
                                                                     
#endif
