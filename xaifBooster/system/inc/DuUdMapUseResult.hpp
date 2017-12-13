#ifndef _DUUDMAPUSERESULT_INCLUDE_
#define _DUUDMAPUSERESULT_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/StatementIdList.hpp"
#include "xaifBooster/system/inc/DuUdMapDefinitionResult.hpp"
#include "xaifBooster/system/inc/ActiveUseType.hpp"

namespace xaifBooster { 

  class DuUdMapUseResult : public DuUdMapDefinitionResult {
    
  public: 

    /** 
     * helper container for use queries
     */
    class StatementIdLists : public Debuggable { 

    public: 
 
     StatementIdLists(const StatementIdList& aDependentStatementIdList,
		      const StatementIdList& aPassiveStatementIdList,
		      const StatementIdList& aStatementIdList);
      /** 
       * this is to contain
       * xaifBooster::BasicBlockPreaccumulation::PrivateLinearizedComputationalGraph::myDependentStatementIdList
       */
      const StatementIdList& myDependentStatementIdList;

      /** 
       * this is to contain
       * xaifBooster::BasicBlockPreaccumulation::PrivateLinearizedComputationalGraph::myPassiveStatementIdList
       */
      const StatementIdList& myPassiveStatementIdList;

      /** 
       * this is to contain
       * xaifBooster::BasicBlockPreaccumulation::PrivateLinearizedComputationalGraph::myStatementIdList
       */
      const StatementIdList& myStatementIdList;

      std::string debug() const ;

    private:
      /** 
       * no def
       */
      StatementIdLists();
    };

    DuUdMapUseResult();
    
    ActiveUseType::ActiveUseType_E myActiveUse;

  };

} // end of namespace 
                                                                     
#endif
