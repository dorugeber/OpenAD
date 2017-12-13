#ifndef _STATEMENTIDLIST_INCLUDE_
#define _STATEMENTIDLIST_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include <list>

#include "xaifBooster/utils/inc/ObjectWithId.hpp"

namespace xaifBooster{ 

  class StatementIdList : public std::list<ObjectWithId::Id>, 
			  public Debuggable { 
  public :
    std::string debug() const ;
  }; 

} 

#endif
