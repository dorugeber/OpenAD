#ifndef _XAIFBOOSTERLINEARIZATION_EXPRESSIONVERTEXALG_INCLUDE_
#define _XAIFBOOSTERLINEARIZATION_EXPRESSIONVERTEXALG_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/ExpressionVertexAlgBase.hpp"
#include "xaifBooster/system/inc/PartialDerivativeKind.hpp"
#include "xaifBooster/system/inc/Symbol.hpp"
#include "xaifBooster/system/inc/Scope.hpp"
#include "xaifBooster/system/inc/Variable.hpp"
#include "xaifBooster/system/inc/Assignment.hpp"

#include "xaifBooster/algorithms/TypeChange/inc/TemporariesHelper.hpp"

#include "xaifBooster/algorithms/InlinableXMLRepresentation/inc/InlinableSubroutineCall.hpp"

using namespace xaifBooster; 

namespace xaifBoosterLinearization { 

  /**
   * class to implement linearization steps
   */
  class ExpressionVertexAlg : public virtual ExpressionVertexAlgBase {

  public:
    
    ExpressionVertexAlg(ExpressionVertex& theContainingExpressionVertex);

    virtual ~ExpressionVertexAlg();

    virtual void printXMLHierarchy(std::ostream& os) const;

    virtual std::string debug() const ;

    virtual void traverseToChildren(const GenericAction::GenericAction_E anAction_c);

    void makeAuxiliaryVariable(const Symbol& aSymbol,
				const Scope& aScope);

    bool hasAuxiliaryVariable() const ;

    const Variable& getAuxiliaryVariable() const ;

    void
    copyAuxiliaryInto(Argument&) const;

    void
    makeAuxiliaryAllocation(xaifBoosterTypeChange::TemporariesHelper&);

    bool
    hasAuxiliaryAllocation() const;

    const xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall&
    getAuxiliaryAllocation() const;

    /** 
     * is this vertex active?
     */
    bool isActive() const;

    /**
     * during clipping of passive subexpressions
     * we may passivate vertices
     */
    void passivate() const;

  private: 

    /** 
     * no def
     */
    ExpressionVertexAlg();

    /** 
     * no def
     */
    ExpressionVertexAlg(const ExpressionVertexAlg&);

    /** 
     * no def
     */
    ExpressionVertexAlg& operator=(const ExpressionVertexAlg&);

    /**
     * this is used for static single assignment code 
     * when we assign an intermediate result to an 
     * auxiliary variable to avoid recomputations
     * for use in partial derivatives
     * this will be set during linearization
     * this is owned by this class 
     * by means of calling makeAuxiliaryVariable;
     * deleted in the dtor
     */
    Variable* myAuxiliaryVariable_p; 

    xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall* myAuxiliaryAllocation_p;

    /**
     * we maintain a flag indicating activity; 
     * the initialization is always to active;
     * the flag may change from active to 
     * passive using passivate;
     */
    mutable
    bool myActiveFlag;

  };
 
} 
                                                                     
#endif
