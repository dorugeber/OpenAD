#ifndef _EXPRESSIONVERTEX_INCLUDE_
#define _EXPRESSIONVERTEX_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include <set>

#include "xaifBooster/utils/inc/ObjectWithId.hpp"
#include "xaifBooster/system/inc/ExpressionVertexAlgBase.hpp"
#include "xaifBooster/system/inc/VertexTraversable.hpp"
#include "xaifBooster/system/inc/InlinableIntrinsicsCatalogueItem.hpp"
#include "xaifBooster/system/inc/NonInlinableIntrinsicsCatalogueItem.hpp"

namespace xaifBooster { 

  /**
   * base class for various vertex types inside of expressions
   */
  class ExpressionVertex : public VertexTraversable, 
			   public ObjectWithId {

  public:

    ExpressionVertex();

    /**
     * dtor deallocates the algorithm object if present
     */
    virtual ~ExpressionVertex();

    /**
     * invoked by Expression
     */
    virtual void printXMLHierarchy(std::ostream& os) const=0;

    /** 
     * nothing implemented. this is for edges
     */
    virtual void printXMLHierarchyImpl(std::ostream& os, const Expression& theExpression) const {};

    /**
     * actual implementation for printing xaif
     * always invoked by the graph
     */
    virtual void printXMLHierarchyImpl(std::ostream& os) const {};

    std::string debug() const ;

    /**
     * access the algorithm
     */
    ExpressionVertexAlgBase& getExpressionVertexAlgBase() const;

    /**
     * leaf objects that are derived from this class
     * need to be comparable for equivalence
     * \todo remove this
     */
    virtual std::string equivalenceSignature() const =0;

    /**
     * not precisely OO but
     * want to avoid RTTI queries; rationale:
     * among the vertices with no inedges can be Argument instances,
     * the other such vertices must be Constant instances
     * @return true if this is an Argument
     */
    virtual bool isArgument() const;

    /**
     * not precisely OO but
     * want to avoid RTTI queries; rationale:
     * among the vertices with  inedges can be Intrinsic instances,
     * the other such vertices must be BooleanOperation instances
     * @return true if this is an Intrinsic
     */
    virtual bool isIntrinsic() const;

    /**
     * ExpressionVertex is base class for variety of leaf classes
     * with algorithms; those need to be able to create copy of 
     * themselves for use in algorithms
     */
    virtual ExpressionVertex& createCopyOfMyself(bool withAlgorithm=false) const=0;

    virtual const InlinableIntrinsicsCatalogueItem& getInlinableIntrinsicsCatalogueItem() const;
    virtual const NonInlinableIntrinsicsCatalogueItem& getNonInlinableIntrinsicsCatalogueItem() const;

  private: 

    /** 
     * no def 
     */ 
    ExpressionVertex(const ExpressionVertex&); 

    /** 
     * no def 
     */ 
    ExpressionVertex& operator=(const ExpressionVertex&); 

  protected:
    /**
     * if required then an algorithm object is allocated dynamically
     * by the factory and the pointer is set
     */
    ExpressionVertexAlgBase* myExpressionVertexAlgBase_p;

  }; // end of class ExpressionVertex
 
  typedef std::set<const ExpressionVertex*> CExpressionVertexPSet;

} // end of namespace xaifBooster
                                                                     
#endif
