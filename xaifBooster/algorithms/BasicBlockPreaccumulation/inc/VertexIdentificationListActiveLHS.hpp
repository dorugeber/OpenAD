#ifndef _VERTEXIDENTIFICATIONLISTACTIVELHS_INCLUDE_
#define _VERTEXIDENTIFICATIONLISTACTIVELHS_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/utils/inc/ObjectWithId.hpp"

#include "xaifBooster/system/inc/Assignment.hpp"
#include "xaifBooster/system/inc/Variable.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/VertexIdentificationListActive.hpp"

using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulation { 

  /**
   * this is for the identification of active variables
   * RHS <-> preceding LHS
   * in the presence of alias or ud information
   * the behavior changes based on the presence or absence of duud information
   */
  class VertexIdentificationListActiveLHS : public VertexIdentificationListActive {

  public:

    /** 
     * in case of duud information present this has only 
     * a meaningfull invocation for "theVariable" being 
     * a RHS Argument
     * in case of duud information absent this can also 
     * be invoked between LHS variables.
     */
    IdentificationResult canIdentify(const Variable& theVariable,
				     const ObjectWithId::Id& statementId) const;

    /** 
     * in case of duud information present we keep adding 
     * variables, 
     * in case of duud information absent we have to maintain 
     * uniqueness, i.e. 
     * this will only work if canIdentify returns
     * NOT_IDENTIFIED 
     */
    void addElement(const Assignment& aAssignment);

    virtual std::string debug() const;

  private: 

    class ListItem: public VertexIdentificationListActive::ListItem { 
      
    public: 

      ListItem(const Assignment& aAssignment);

      virtual std::string debug() const;

      /**
       * this is the statement id of the assignment in which this 
       * vertex represents the LHS, i.e. it is the statementID of 
       * the single entry in the ud chain or alternatively the 
       * result of looking at the alias map,
       * this must be set or this ListItem instance is useless
       */
      const ObjectWithId::Id& myStatementId;

    };
    
    /** 
     * retrieve the list of statement IDs collected in myList
     */
    void getStatementIdList(StatementIdList& aStatementIdList)const;

  }; // end of class VertexIdentificationListActiveLHS  
   
} // end namespace 
                                                                  
#endif
