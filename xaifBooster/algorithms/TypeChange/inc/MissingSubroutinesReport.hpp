#ifndef _XAIFBOOSTERTYPECHANGE_MISSINGSUBROUTINESREPORT_INCLUDE_
#define _XAIFBOOSTERTYPECHANGE_MISSINGSUBROUTINESREPORT_INCLUDE_
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

#include "xaifBooster/system/inc/SubroutineNotFoundException.hpp"

using namespace xaifBooster; 

namespace xaifBoosterTypeChange { 

  class MissingSubroutinesReport { 

  public: 

    static void report(const  SubroutineNotFoundException& e);
    static void reportConversion(const SymbolReference& srName);

  private:
    
    typedef std::list<SymbolReference*> SymbolReferencePList;

    static SymbolReferencePList ourReportedList;
 
    static SymbolReferencePList ourConversionReportedList;

  }; // end of class MissingSubroutinesReport

}

#endif
