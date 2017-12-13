#ifndef _VERTEXIDENTIFICATIONLISTACTIVE_INCLUDE_
#define _VERTEXIDENTIFICATIONLISTACTIVE_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/AliasMapKey.hpp"
#include "xaifBooster/system/inc/StatementIdSetMapKey.hpp"
#include "xaifBooster/system/inc/ExpressionVertex.hpp"
#include "xaifBooster/system/inc/Variable.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/VertexIdentificationList.hpp"

using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulation { 

  /**
   * this is for the identification fo active variables   
   */
  class VertexIdentificationListActive : public VertexIdentificationList {

  public:

    VertexIdentificationListActive();

    ~VertexIdentificationListActive(){};

    virtual std::string debug() const;

    class IdentificationResult {

    public: 

      IdentificationResult(IdentificationResult_E,
                           const Assignment* aAssignment_p,
                           const ExpressionVertex* aExpressionVertex_p);

      ~IdentificationResult(){};

      IdentificationResult_E getAnswer() const;

      /**
       *this will be valid only if myAnswer!=NOT_IDENTIFIED
       */
      const Assignment&
      getAssignment() const;

      /**
       *this will be valid only if myAnswer!=NOT_IDENTIFIED
       */
      const ExpressionVertex&
      getExpressionVertex() const;

    private:

      /** 
       * no def 
       */
      IdentificationResult();

      IdentificationResult_E myAnswer;

      /**
       * this is valid only if myAnswer!=NOT_IDENTIFIED
       */
      const Assignment* myAssignment_p;

      /**
       * this is valid only if myAnswer!=NOT_IDENTIFIED
       */
      const ExpressionVertex* myExpressionVertex_p;

    };

    /**
     */
    IdentificationResult aliasIdentify(const Variable& theVariable) const;

    /**
     * this is a fall back method utilizing only alias information 
     * as opposed to the overwriting methods which look at DuUd information 
     * this method should not ever be invoked for duud based lists
     * it is overwritten in the derived classes
     */
    virtual IdentificationResult canIdentify(const Variable& theVariable) const;

    /** 
     * in case of duud information present we do nothing
     * in case of duud information absent we have use canIdentify to 
     * check and remove all list elements may aliased to theVariable
     */
    virtual void removeIfIdentifiable(const Variable& theVariable); 

    // JU: I don't believe ListItem should have to be
    // public (as opposed to be protected) but SUN CC 5.6 insists. 
    // this may have something to do with 2 stage lookup.
    //  protected:
    
    /** 
     * an entry in the list 
     */
    class ListItem: public VertexIdentificationList::ListItem { 

    public: 
      
      ListItem(const AliasMapKey& anAliasMapKey,
	       const StatementIdSetMapKey& aDuUdMapKey,
               const Assignment& aAssignment,
	       const ExpressionVertex& aExpressionVertex);

      virtual std::string debug() const;

      const Assignment&
      getAssignment() const;

      const ExpressionVertex&
      getExpressionVertex() const;

    private:

      /**
       * the pointer to an Assignment; always set; we don't own this pointer
       */ 
      const Assignment* myAssignment_p;

      /**
       * the pointer to an expression vertex;
       * always set, 
       * we don't own this pointer
       */ 
      const ExpressionVertex* myExpressionVertex_p;

    };
    
  }; // end of class VertexIdentificationListActive  
   
} // end namespace 
                                                                  
#endif
