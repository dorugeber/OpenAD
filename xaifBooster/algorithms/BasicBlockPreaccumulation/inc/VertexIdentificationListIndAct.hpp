#ifndef _VERTEXIDENTIFICATIONLISTINDACT_INCLUDE_
#define _VERTEXIDENTIFICATIONLISTINDACT_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/BasicBlock.hpp"
#include "xaifBooster/system/inc/Variable.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/VertexIdentificationList.hpp"

using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulation { 

  class PrivateLinearizedComputationalGraphVertex;

  /**
   * this is to determine overwrites (LHS to preceeding references) 
   * of indirectly active variables (variables used in address 
   * computations of active variables). For instance
   * ... = ... a[i] ... 
   * i = ...
   * b = ...
   * the overwrite of i (for now) will trigger a split in the computational graph
   */
  class VertexIdentificationListIndAct : public VertexIdentificationList {

  public:

    VertexIdentificationListIndAct();

    ~VertexIdentificationListIndAct(){};

    virtual std::string debug() const;
     
    /**
     * test for overwrites
     * \param theLHSVariable is the variable whose value is overwritten
     * \param aStatementId of the statement in which theLHSVariable is the LHS (for debugging only)
     * \param theBasicBlock containing the statement in which theLHSVariable is the LHS (for debugging only)
     * \return true if any element in this class instance is overwritten by the assignment to theLHSVariable
     */
    bool overwrittenBy(const Variable& theLHSVariable,
		       const ObjectWithId::Id& aStatementId,
		       const BasicBlock& theBasicBlock) const;

    /** 
     * We pass in the variable and inside 
     * we check if it has indices and add 
     * them if needed. This method descends 
     * recursively.
     */
    void addElement(const Variable& theVariable,
		    const ObjectWithId::Id& aStatementId);

  };   
   
} 
                                                                  
#endif
