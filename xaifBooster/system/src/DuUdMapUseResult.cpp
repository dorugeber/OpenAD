// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/system/inc/DuUdMapUseResult.hpp"

namespace xaifBooster { 

  DuUdMapUseResult::StatementIdLists::StatementIdLists(const StatementIdList& aDependentStatementIdList,
						       const StatementIdList& aPassiveStatementIdList,
						       const StatementIdList& aStatementIdList) :
    myDependentStatementIdList(aDependentStatementIdList),
    myPassiveStatementIdList(aPassiveStatementIdList),
    myStatementIdList(aStatementIdList) {
  }

  std::string DuUdMapUseResult::StatementIdLists::debug() const { 
    std::ostringstream out;
    out << "StatementIdLists[" << this 
	<< " myDependentStatementIdList=" << myDependentStatementIdList.debug().c_str() 
	<< ",myPassiveStatementIdList=" << myPassiveStatementIdList.debug().c_str()
	<< ",myStatementIdList=" << myStatementIdList.debug().c_str()
	<< "]" << std::ends;  
    return out.str();
  } // end of Variable::debug


  DuUdMapUseResult::DuUdMapUseResult() { 
    myActiveUse=ActiveUseType::UNDEFINEDUSE;
  }
    
} // end of namespace  
