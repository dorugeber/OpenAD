#ifndef XlateExpression_INCLUDED_h
#define XlateExpression_INCLUDED_h

#include <iostream>

#include "xercesc/dom/DOMDocument.hpp"
#include "xercesc/dom/DOMElement.hpp"
#include "xercesc/dom/DOMNode.hpp"

#include "Open64IRInterface/Open64BasicTypes.h"

#include "IntrinsicXlationTable.h"

#include "PUXlationContext.h"

#include "OpenAnalysis/Utils/DGraph/DGraphInterface.hpp"
#include "OpenAnalysis/Utils/DGraph/DGraphImplement.hpp"



namespace xaif2whirl {

  class MyDGNode;

  class XlateExpression { 

  public: 

    /** 
     * the first node in an expression graph...  
     */
    static WN* translateExpression(const xercesc::DOMElement* elem, 
				   PUXlationContext& ctxt);
  
    /** 
     * TranslateExpressionSimple: Translates certain XAIF expressions into
     * special WHIRL expressions in order to conform to WHIRL
     * requirements.
     */
    static WN* translateExpressionSimple(const xercesc::DOMElement* elem, 
					 PUXlationContext& ctxt);
    /** 
     * TranslateVarRef: Given the first node in a variable reference
     * graph, create a variable reference.
     *
     * If the LVALUE flag of 'ctxt' is set an address expression (lvalue)
     * is returned; otherwise a rvalue is returned.  If this is the
     * outermost part of an active varref, creates a value/deriv selector.
     */
    static WN* translateVarRef(const xercesc::DOMElement* elem, 
			       PUXlationContext& ctxt);
  
    /** TranslateSimpleVarRef: Given the first node in a simple variable
     * reference graph, create a variable reference.  No value/deriv
     * selector can be created.
     */
    static std::pair<ST*, WN_OFFSET> translateVarRefSimple(const xercesc::DOMElement* elem, 
							   PUXlationContext& ctxt);
  
    /** 
     * FIXME: this can use the parent map that is in ctxt now. The new
     * signature could be:
     *  PatchWNExpr(WN* wn, PUXlationContext& ctxt)
     
     * PatchWNExpr: Descends through the expression rooted at
     * WN_kid(parent, kidno), modifying variable references with a
     * value-component selector.
     *
     * This function purposefully only operates on expressions (as
     * opposed to arbitrary WHIRL trees) because statements have to be
     * processed specially.
     */
    static WN* patchWNExpr(WN* parent, 
			   INT kidno, 
			   PUXlationContext& ctxt);


    static WN* translateConstant(const xercesc::DOMElement* elem, 
				 PUXlationContext& ctxt);

    /** 
     * createValueSelector: Select the value portion of 'wn', by wrapping
     * a dummy intrinsic call around it
     * N.B.: This creates a OPR_CALL node, which is not an expression.
     */
    static WN* createValueSelector(WN* wn);

  private:

    static bool hasActiveSymbolType(WN* aWNp);

    static WN* xlate_Expression(OA::OA_ptr<OA::DGraph::DGraphImplement> g, 
				OA::OA_ptr<MyDGNode> n, 
				PUXlationContext& ctxt);

    static WN* xlate_VarRef(const xercesc::DOMElement* elem, 
			    PUXlationContext& ctxt);

    static WN* xlate_VarRef(OA::OA_ptr<OA::DGraph::DGraphImplement> g, 
			    OA::OA_ptr<MyDGNode> n, 
			    PUXlationContext& ctxt);

    static WN* xlate_Intrinsic(OA::OA_ptr<OA::DGraph::DGraphImplement> g, 
			       OA::OA_ptr<MyDGNode> n,
			       PUXlationContext& ctxt);

    static WN* xlate_FunctionCall(OA::OA_ptr<OA::DGraph::DGraphImplement> g, 
				  OA::OA_ptr<MyDGNode> n, 
				  PUXlationContext& ctxt);

    static WN* xlate_BooleanOperation(OA::OA_ptr<OA::DGraph::DGraphImplement> g, 
				      OA::OA_ptr<MyDGNode> n, 
				      PUXlationContext& ctxt);

    /**
     *xlate_ExprOpUsingIntrinsicTable: abstract similarities betweeen
     * translation of XAIF Intrinsic, FunctionCall, BooleanOperation.  The
     * XAIF operator will become either a special WHIRL node or some type
     * of WHIRL call. WN*
     */
    static WN* xlate_ExprOpUsingIntrinsicTable(fortTkSupport::IntrinsicXlationTable::XAIFOpr xopr, 
					       const char* xoprNm, const char* xIntrinKey,
					       OA::OA_ptr<OA::DGraph::DGraphImplement> g, 
					       OA::OA_ptr<MyDGNode> n, 
					       PUXlationContext& ctxt);

    /**
     * xlate_SymbolReferenceSimple: Translate a symbol reference.  May an
     * active flag in 'ctxt' that is inherited *up* the context stack.
     * N.B.: For PREGS we *do not* create a LDA
     */
    static WN* xlate_SymbolReference(const xercesc::DOMElement* elem, 
				     PUXlationContext& ctxt);

    /**
     * xlate_SymbolReferenceSimple: Translate a simple symbol reference.
     * For pregs, also return the PREG_IDX as an WN_OFFSET.
     * Do not set an active flag.
     */
    static pair<ST*, WN_OFFSET> xlate_SymbolReferenceSimple(const xercesc::DOMElement* elem, 
							    PUXlationContext& ctxt);

    static WN* xlate_SymbolReferenceCollapsedPath(const xercesc::DOMElement* elem, WN* pathVorlageWN,
						  PUXlationContext& ctxt);

    static WN* xlate_ArrayElementReference(OA::OA_ptr<OA::DGraph::DGraphImplement> g, 
					   OA::OA_ptr<MyDGNode> n, 
					   PUXlationContext& ctxt);


    /**
     * createExpressionGraph: Given the first element in an XAIF
     * expression or variable reference graph, returns a DGraph where
     * where graph nodes point to nodes in the DOM tree.  Variable
     * reference graphs are special cases of an expression graph.
     *
     * When walking a top-down graph from root to children, one descends
     * *incoming* edges.  Here is an example expression:
     *
     *     mult    <==   x * (y + z)
     *     ^ ^
     *    /   \
     *   x   plus
     *       ^ ^
     *      /   \
     *     y     z
     *
     * Here is an example variable reference:
     *  
     *  ArrayElementReference        <==   A[1, i]
     *    Index: Constant: 1
     *    Index: SymbolReference: i
     *        ^ 
     *        |
     *  SymbolReference: A
     *
     */    
    static OA::OA_ptr<OA::DGraph::DGraphImplement> createExpressionGraph(const xercesc::DOMElement* elem, 
									 bool varRef = false);
    
    /**
     * createDerivSelector: Select the deriv portion of 'wn', by wrapping
     * a dummy intrinsic call around it
     * N.B.: This creates a OPR_CALL node, which is not an expression.
     */
    static WN* createDerivSelector(WN* wn);

    static TYPE_ID getRType(WN* wn);

    static TYPE_ID getRTypeFromOpands(vector<WN*>& opands);

    static TYPE_ID getRTypeFromOpands(TYPE_ID ty1, TYPE_ID ty2);

    /**
     * Scan Machine_Types table for the right type.
     */
    static TYPE_ID getMType(unsigned int cl, unsigned int bytesz);

    static OPCODE getWNExprOpcode(OPERATOR opr, vector<WN*>& opands);

    static INTRINSIC getWNIntrinsic(const char* intrnNm, vector<WN*>& opands, TYPE_ID* dtype);

  };
  
}

#endif
