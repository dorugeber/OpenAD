#ifndef _EXPRESSION_INCLUDE_
#define _EXPRESSION_INCLUDE_
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
#include "xaifBooster/utils/inc/XMLPrintable.hpp"
#include "xaifBooster/system/inc/ExpressionVertex.hpp"
#include "xaifBooster/system/inc/ExpressionEdge.hpp"
#include "xaifBooster/system/inc/GraphWrapperTraversable.hpp"
#include "xaifBooster/system/inc/ExpressionAlgBase.hpp"
#include "xaifBooster/system/inc/Variable.hpp"
#include "xaifBooster/system/inc/Argument.hpp"
#include "xaifBooster/system/inc/Constant.hpp"

namespace xaifBooster { 

  /**
   * an expression, usually 
   * numeric but can contain boolean operations 
   */
  class Expression : 
    public GraphWrapperTraversable<ExpressionVertex, ExpressionEdge>, 
    public XMLPrintable {

  public:

    /**
     * ctor stes pointer to dynamically allocated algorithm
     * object if required
     */
    Expression(bool hasAlgorithm=true);

    /**
     * dtor deallocates algorithm if present
     */
    ~Expression();

    /**
     * virtual in XMLPrintable
     * decision on what is to be printed is taken in
     * algorithm (if present, otherwise printXMLHierarchyImpl
     * is called
     */
    void printXMLHierarchy(std::ostream& os) const;

    /**
     * actual implementation for printing xaif
     */
    void printXMLHierarchyImpl(std::ostream& os) const;

    /**
     * is virtual in Debuggable inherited via XMLPrintable
     */
    std::string debug() const ;

    /**
     * calls GraphvizDisplay::show
     */
    void show(const std::string& outputName) const;

    /**
     * \param theTarget where we deep copy the contents of this instance to
     * \param withNewId indicates if the graph 
     * elements will have their own Id's  
     * created from getNexVertex/EdgeId()
     * \param withAlgorithm indicates if the graph 
     * elements will have their algorithm objects 
     * created.
     * returns the vertex in theTarget that is equivalent to the max vertex of self
     */
    ExpressionVertex& copyMyselfInto(Expression& theTarget,
				     bool withNewId,
				     bool withAlgorithm) const;  

    /**
     * similar to copyMyselfInto
     * perform a deep copy of the subexpression  contents 
     * starting with theTopVertex into theTarget
     * \param theTarget is the Expression into which we copy
     * \param theTopVertex is vertex of this instance
     * \param withNewId if true it  receives a new id, if false it inherits the original id
     * \param withAlgorithm if true a new algorithm instance is made
     * \return the copy of theTopVertex in theTarget
     */
    ExpressionVertex& copySubExpressionInto(Expression& theTarget,
					    const ExpressionVertex& theTopVertex,
					    bool withNewId,
					    bool withAlgorithm) const;
    
    /** 
     * algorithm access where the Expression may 
     * be const but in difference to the 
     * internal representation (wich is always 
     * const for the algorithms) the algorithm 
     * instances will always be modifiable.
     */
    ExpressionAlgBase& getExpressionAlgBase() const;

    /**
     * virtual in GenericTraverseInvoke
     * used by generic traversal
     * child is myExpressionAlgBase_p (if set)
     */
    void traverseToChildren(const GenericAction::GenericAction_E anAction_c);

    /**
     * finds top vertex of positional subexpression
     */
    const ExpressionVertex& findPositionalSubExpressionOf(const ExpressionVertex& aVertex,
							  unsigned int aPosition) const;


    /**
     * for variable replacements in an expression: 
     * in the pair the first is the variable to be replaced, 
     * the second the variable by which the first is replaced
     */
    typedef std::pair<const Variable*,VariableSymbolReference*> VariablePVariableSRPPair;
    typedef std::list<VariablePVariableSRPPair> VariablePVariableSRPPairList; 

    /** 
     * this is generic utility for replacing Variables in a given 
     * Expression.  These Expression instances have to be non-constant, 
     * i.e. attributes of some algorithm class
     */
    void replaceVariables(const VariablePVariableSRPPairList& replacementList); 

    /**
     * the boolean indicates if the argument in question is used as non-value inquiry
     * based on the settings in an InlinableIntrinsicsCatalogueItem instance where applicable
     */
    typedef std::list<std::pair<Argument*,bool> > ArgumentPList;

    void appendArguments(ArgumentPList& listToBeAppended);

    /**
     * the boolean indicates if the argument in question is used as non-value inquiry
     * based on the settings in an InlinableIntrinsicsCatalogueItem instance where applicable
     */
    typedef std::list<std::pair<const Argument*,bool> > CArgumentPList;

    void appendArguments(CArgumentPList& listToBeAppended) const;

    /** 
     * true if the expression is constant
     * \todo for the time being assumes the value is contained in a single constant vertex
     */
    bool isConstant() const; 

    /**
     *  if this is a constant integer expression evaluate it
     */
    int constIntEval() const;

    void appendActiveArguments(CArgumentPList& listToBeAppended) const;

    /**
     * add an intrinsic node and edges conntecting it
     * \param opName is the name the intrinsic
     * \param arg1TopVertex is the maximal vertex of the subexpression used as first argument
     * \param arg2TopVertex is the maximal vertex of the subexpression used as second argument
     * \return the new intrinsic vertex
     */
    ExpressionVertex& addBinaryOpByName(const std::string& opName,
					const ExpressionVertex& arg1TopVertex,
					const ExpressionVertex& arg2TopVertex);

    /**
     * This expression is considered to contain \p anExpression if and only if
     * they are the same or \p anExpression is contained in one of our arguments.
     * If \p anExpression is not found in any of these places,
     * the respective algorithm (if any) is subsequently checked
     */
    bool hasExpression(const Expression& anExpression) const;

    /**
     * simple wrapper class so we don't pass around pointers
     */
    template<class Vertex>
    class ReturnOfSingleVertex {
    private:
    	Vertex* myVertex_p;
    	/**
    	 * no def
    	 */
    	ReturnOfSingleVertex();
    public:
    	bool hasIt()const {return myVertex_p!=0;}
    	Vertex& getIt() const {
    		if (!myVertex_p) {
    			THROW_LOGICEXCEPTION_MACRO("ReturnOfSingleVertex::getIt: do not have it");
    		}
    		return *myVertex_p;
    	}
    	ReturnOfSingleVertex(Vertex* theVertex_p) : myVertex_p(theVertex_p){}
    };
    typedef ReturnOfSingleVertex<const Variable> ReturnOfSingleVariable;

    ReturnOfSingleVariable singleVariable() const ;

    typedef ReturnOfSingleVertex<const Constant> ReturnOfSingleConstant;

    ReturnOfSingleConstant singleConstant() const;

  private:

    /** 
     * the class owns this instance, 
     * it will be set 
     * during construction and deleted during 
     * destruction
     */
    ExpressionAlgBase* myExpressionAlgBase_p;

    typedef std::list<const ExpressionVertex*> ExpressionVertexPList;

    /** 
     * recursively invoked implementation for public copySubExpressionInto
     */
    void copySubExpressionInto(Expression& theTarget,
			       const ExpressionVertex& theTopVertex,
			       ExpressionVertex& theTopCopy,
			       bool withNewId,
			       bool withAlgorithm,
			       ExpressionVertexPList theList) const;

    /**
     * internal helper to recursively evaluate 
     * constant integer expressions
     */
    int constIntEvalR(const ExpressionVertex& theTopVertex) const;

    /**
     * internal helper to recursively determine
     * if an expression is constant
     */
    bool isConstantR(const ExpressionVertex& theTopVertex) const;

  }; 

} // end of namespace xaifBooster
                                                                     
#endif
