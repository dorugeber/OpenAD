// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================


#include "xaifBooster/utils/inc/DbgLoggerManager.hpp"
#include "xaifBooster/system/inc/SymbolReference.hpp"
#include "xaifBooster/algorithms/TypeChange/inc/MissingSubroutinesReport.hpp"

namespace xaifBoosterTypeChange { 

  MissingSubroutinesReport::SymbolReferencePList MissingSubroutinesReport::ourReportedList;
  MissingSubroutinesReport::SymbolReferencePList MissingSubroutinesReport::ourConversionReportedList;

  void MissingSubroutinesReport::report(const  SubroutineNotFoundException& e) { 
    const SymbolReference& s(e.getSymbolReference());
    SymbolReferencePList::iterator i;
    for (i=ourReportedList.begin();
	 i!=ourReportedList.end();
	 ++i) { 
      if (&((*i)->getScope())==&(s.getScope()) && &((*i)->getSymbol())==&(s.getSymbol()))
	break;
    }
    if (i==ourReportedList.end()) { 
      ourReportedList.push_back(new SymbolReference(s.getSymbol(),
						    s.getScope()));
      DBG_MACRO(DbgGroup::WARNING, 
		"cannot find subroutine " 
		<< s.getSymbol().plainName().c_str() 
		<< " (xaif name " 
		<< s.getSymbol().getId().c_str() 
		<< ") which means the analysis may net be able to track activity through these calls. Consider providing stubs for external routines.");
    }
  } 

  void MissingSubroutinesReport::reportConversion(const SymbolReference& srName) { 
    SymbolReferencePList::iterator i;
    for (i=ourConversionReportedList.begin();
	 i!=ourConversionReportedList.end();
	 ++i) { 
      if (&((*i)->getScope())==&(srName.getScope()) && &((*i)->getSymbol())==&(srName.getSymbol()))
	break;
    }
    if (i==ourConversionReportedList.end()) { 
      ourConversionReportedList.push_back(new SymbolReference(srName.getSymbol(),
							      srName.getScope()));
      DBG_MACRO(DbgGroup::WARNING, 
		"black box subroutine " 
		<< srName.getSymbol().plainName().c_str() 
		<< " (xaif name " 
		<< srName.getSymbol().getId().c_str() 
		<< ") is called with active arguments which are being converted to passive.");
    }
  } 
  
}

