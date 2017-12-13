#ifndef _XAIFBOOSTERLINEARIZATION_EXPRESSIONEDGEALG_INCLUDE_
#define _XAIFBOOSTERLINEARIZATION_EXPRESSIONEDGEALG_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/utils/inc/GuardedMap.hpp"

#include "xaifBooster/system/inc/ExpressionEdgeAlgBase.hpp"
#include "xaifBooster/system/inc/PartialDerivativeKind.hpp"
#include "xaifBooster/system/inc/InlinableIntrinsicsExpression.hpp"
#include "xaifBooster/system/inc/Assignment.hpp"
#include "xaifBooster/system/inc/Constant.hpp"

#include "xaifBooster/algorithms/TypeChange/inc/TemporariesHelper.hpp"

#include "xaifBooster/algorithms/InlinableXMLRepresentation/inc/InlinableSubroutineCall.hpp"

using namespace xaifBooster; 

namespace xaifBoosterLinearization { 

  /**
   * class to implement linearization steps
   */
  class ExpressionEdgeAlg : public ExpressionEdgeAlgBase {

  public:
    
    ExpressionEdgeAlg(ExpressionEdge& theContainingExpressionEdge);

    virtual ~ExpressionEdgeAlg();

    virtual void printXMLHierarchy(std::ostream& os) const;

    virtual std::string debug() const ;

    virtual void traverseToChildren(const GenericAction::GenericAction_E anAction_c);

    bool hasPartial() const; 

    const InlinableIntrinsicsExpression& getPartial() const ;

    void setPartial(const InlinableIntrinsicsExpression& thePartial);

    void addArgumentsConcretizationPair(const ExpressionVertex& aConcreteArgument,
					const InlinableIntrinsicsExpressionVertex& anAbstractArgument);

    void makeConcretePartialAssignment();

    Assignment& getConcretePartialAssignment();

    const Assignment& getConcretePartialAssignment() const ;

    bool hasConcretePartialAssignment() const;

    void
    makePartialAllocation(xaifBoosterTypeChange::TemporariesHelper&);

    bool
    hasPartialAllocation() const;

    const xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall&
    getPartialAllocation() const;

    void setConcreteConstant(const Constant& aConstant);

    const Constant& getConcreteConstant() const;

    typedef std::pair<const ExpressionVertex*,const InlinableIntrinsicsExpressionVertex*> VertexPair;
    
    typedef std::list<VertexPair> VertexPairList;

    PartialDerivativeKind::PartialDerivativeKind_E getPartialDerivativeKind() const;

    void passivate();

    void linearate();

    const VertexPairList& getConcreteArgumentInstancesList() const;

    void mapPartialEV2OriginalEV(const ExpressionVertex&,
                                 const ExpressionVertex&);

    const ExpressionVertex& getOriginalExpressionVertex4ConcretePartialArgument(const ExpressionVertex&) const;

  private: 

    /** 
     * no def
     */
    ExpressionEdgeAlg();

    /** 
     * no def
     */
    ExpressionEdgeAlg(const ExpressionEdgeAlg&);

    /** 
     * no def
     */
    ExpressionEdgeAlg& operator=(const ExpressionEdgeAlg&);

    typedef GuardedMap<const ExpressionVertex*,
                     const ExpressionVertex*> CExpressionVertexP2CExpressionVertexPMap;

    /**
     * maps expression vertices in the partial expression (arguments and constants)
     * to the corresp. original ExpressionVertex
     */
    CExpressionVertexP2CExpressionVertexPMap myPartialEVP2OriginalEVPMap;

    /**
     * a reference to the partial derivative
     * which is being set as part of the 
     * linearization. It refers to an entity in the  
     * InlinableInstrinsicsCatalogue
     */
    const InlinableIntrinsicsExpression* myPartialDerivative_p;
    
    /**
     * this is to keep track of 
     * the actual arguments in myPartialDerivative
     * the list doesn't own any of the instances pointed to
     */
    VertexPairList myConcreteArgumentInstancesList;

    /**
     * This is initially 0, it will 
     * be set during linearization 
     * when we establish a concrete 
     * instance for the partial expression 
     * based on the abstract expression 
     * referred to by myPartialDerivative_p
     * this class owns this instance
     */
    Assignment* myConcretePartialAssignment_p;

    xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall* myPartialAllocation_p;

    /// for partials that consist of a single constant expression vertex
    /**
     * We do not want to create an assignment for constant partials
     * that consist of a single (constant) argument.
     * In such cases, we simply store a pointer to the corresponding Constant.
     * We do not own this.
     */ 
    const Constant* myConcreteConstant_p;

    /** 
     * this is initialized to NONLINEAR
     * will be reset upon setting of
     * myPartialDerivative_p to the value assumed 
     * there and can be changed to PASSIVE
     * with passivate
     */
    PartialDerivativeKind::PartialDerivativeKind_E myConcretePartialDerivativeKind;

  };
 
} 
                                                                     
#endif
