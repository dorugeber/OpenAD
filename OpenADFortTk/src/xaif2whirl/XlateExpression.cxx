// -*-Mode: C++;-*-
// $Header: /Volumes/cvsrep/developer/OpenADFortTk/src/xaif2whirl/XlateExpression.cxx,v 1.2 2006/05/12 16:12:23 utke Exp $

#include <stdlib.h> // ANSI: cstdlib // for strtol
#include <string.h> // ANSI: cstring // for strcmp, etc.
#include <iostream>
#include <vector>
#include <algorithm>

#include "Open64IRInterface/Open64BasicTypes.h"
#include "Open64IRInterface/Open64IRInterface.hpp"
#include "Open64IRInterface/SymTab.h"
#include "Open64IRInterface/wn_attr.h"
#include "Open64IRInterface/stab_attr.h"

#include "ScalarizedRefTab.h"
#include "WhirlIDMaps.h"
#include "XAIFStrings.h"
#include "Diagnostics.h"

#include "xaif2whirl.h"
#include "Args.h"
#include "XlateExpression.h"
#include "XAIF_DOMFilters.h"
#include "XercesStrX.h"

namespace xaif2whirl { 
  extern TY_IDX ActiveTypeTyIdx;            // FIXME
  extern TY_IDX ActiveTypeInitializedTyIdx; // FIXME


  WN* XlateExpression::translateExpression(const DOMElement* elem, 
					   PUXlationContext& ctxt) {
    FORTTK_ASSERT(elem, fortTkSupport::Diagnostics::UnexpectedInput);
    // Slurp expression into a graph (DAG) and translate it
    OA::OA_ptr<OA::DGraph::DGraphImplement> g = createExpressionGraph(elem);
    // xaif2whirl::DDumpDotGraph(g);
    OA::OA_ptr<OA::DGraph::NodeInterface> root = g->getExprGraphRootNode();
    OA::OA_ptr<MyDGNode> n = root.convert<MyDGNode>();
    WN* wn = xlate_Expression(g, n, ctxt);
    return wn;
  }

  WN* XlateExpression::translateExpressionSimple(const DOMElement* elem,
						 PUXlationContext& ctxt) {
    ctxt.createXlationContext(XlationContext::EXPRSIMPLE);
    WN* wn = translateExpression(elem, ctxt);
    ctxt.deleteXlationContext();
    return wn;
  }

  WN* XlateExpression::translateVarRef(const DOMElement* elem, 
				       PUXlationContext& ctxt) {
    FORTTK_ASSERT(elem, fortTkSupport::Diagnostics::UnexpectedInput);
    // Slurp expression into a graph (DAG) and translate it
    ctxt.createXlationContext(XlationContext::VARREF);
    OA::OA_ptr<OA::DGraph::DGraphImplement> g = 
      createExpressionGraph(elem, true /* varRef */);
    // xaif2whirl::DDumpDotGraph(g);
    OA::OA_ptr<OA::DGraph::NodeInterface> root = g->getExprGraphRootNode();
    OA::OA_ptr<MyDGNode> n = root.convert<MyDGNode>();
    WN* wn = xlate_VarRef(g, n, ctxt);
    ctxt.deleteXlationContext();
    // If we are not already within another VarRef and we translated an
    // active symbol, select the appropriate portion of the active type
    if (!ctxt.currentXlationContext().isFlag(XlationContext::VARREF)
	||
	ctxt.currentXlationContext().isFlag(XlationContext::ARRAYIDX)) {
      if (ctxt.currentXlationContext().isFlag(XlationContext::ACTIVETYPE)) { 
	if (ctxt.currentXlationContext().isFlag(XlationContext::VALUESELECTOR)) {
	  wn = createValueSelector(wn); // active
	} else if (ctxt.currentXlationContext().isFlag(XlationContext::DERIVSELECTOR)) {
	  wn = createDerivSelector(wn); // deriv
	}
	ctxt.currentXlationContext().unsetFlag(XlationContext::ACTIVETYPE); // halt up-inheritance
      }
    }  
    return wn;
  }

  std::pair<ST*, WN_OFFSET> XlateExpression::translateVarRefSimple(const DOMElement* elem, 
								   PUXlationContext& ctxt) {
    FORTTK_ASSERT(elem, fortTkSupport::Diagnostics::UnexpectedInput);
    // This must be a plain XAIF symbol reference (a one-vertex graph)
    const XMLCh* nameX = elem->getNodeName();
    if ( !(XMLString::equals(nameX, XAIFStrings.elem_SymRef_x()) &&
	   GetNextSiblingElement(elem) == NULL) ) {
      FORTTK_DIE("Expected a simple var ref but found:\n" << *elem);
    }
    ctxt.createXlationContext(XlationContext::NOFLAG);
    pair<ST*, WN_OFFSET> stpair = xlate_SymbolReferenceSimple(elem, ctxt);
    ctxt.deleteXlationContext();
    return stpair;
  }

  WN* XlateExpression::patchWNExpr(WN* parent, 
				   INT kidno, 
				   PUXlationContext& ctxt) {
    // Simple Base cases
    if (!parent) { return NULL; }
    WN* wn = WN_kid(parent, kidno);
    if (!wn) { return NULL; }
    OPERATOR opr = WN_operator(wn);
    if (!OPERATOR_is_expression(opr) && !OPERATOR_is_call(opr)) { return parent; }
    // Base case: a variable reference
    if (fortTkSupport::ScalarizedRef::isRefTranslatableToXAIF(wn)) {
      if (hasActiveSymbolType(wn)) {
	WN* newwn = createValueSelector(wn);
	WN_kid(parent, kidno) = newwn;
      }
    }
    else {
      // Recursive case
      for (INT i = 0; i < WN_kid_count(wn); ++i) {
	patchWNExpr(wn, i /* kid */, ctxt);
      }
    }
    return parent;
  }

  bool XlateExpression::hasActiveSymbolType(WN* aWNp) { 
    if (WN_has_sym(aWNp)) {
      if (WN_operator(aWNp)==OPR_IMPLICIT_BND)
	return false;
      ST* st = WN_st(aWNp);
      TY_IDX tyIdx = ST_type(st);
      if (TY_kind(tyIdx) == KIND_POINTER) {
	tyIdx = TY_pointed(tyIdx);
      } 
      if (TY_kind(tyIdx) == KIND_ARRAY) {
	tyIdx = TY_etype(tyIdx);
      }
      if (tyIdx == ActiveTypeTyIdx || tyIdx == ActiveTypeInitializedTyIdx) {
	return true; 
      }
    }
    else if( WN_operator(aWNp)==OPR_STRCTFLD) { 
      TY_IDX base_ty = WN_GetBaseObjType(aWNp);
      UINT field_id = WN_field_id(aWNp);
      UINT cur_field_id=0;
      FLD_HANDLE fld = FLD_get_to_field (base_ty, field_id, cur_field_id);
      TY_IDX tyIdx = FLD_type(fld);
      if (TY_kind(tyIdx) == KIND_POINTER) {
	tyIdx = TY_pointed(tyIdx);
      } 
      if (TY_kind(tyIdx) == KIND_ARRAY) {
	tyIdx = TY_etype(tyIdx);
      }
      if (tyIdx == ActiveTypeTyIdx || tyIdx == ActiveTypeInitializedTyIdx) {
	return true; 
      }
    }
    else { 
      // Recursive case
      for (INT i = 0; i < WN_kid_count(aWNp); ++i) {
	if (hasActiveSymbolType(WN_kid(aWNp, i))) 
	  return true;
      }
    }
    return false; 
  } 

  WN* XlateExpression::translateConstant(const DOMElement* elem, 
					 PUXlationContext& ctxt) {
    FORTTK_ASSERT(elem, fortTkSupport::Diagnostics::UnexpectedInput);
    const XMLCh* typeX = elem->getAttribute(XAIFStrings.attr_type_x());
    const XMLCh* fetypeX = elem->getAttribute(XAIFStrings.attr_feType_x());
    const XMLCh* valX = elem->getAttribute(XAIFStrings.attr_value_x());
    XercesStrX type = XercesStrX(typeX);
    XercesStrX fetype = XercesStrX(fetypeX);
    TYPE_ID mtype=XAIFFETypeToWHIRLMTy(fetype.c_str());
    XercesStrX value = XercesStrX(valX);
    WN* wn = NULL;
    if ((strcmp(type.c_str(), "real") == 0) ||
	(strcmp(type.c_str(), "double") == 0)) {
      // Floating point constant
      double val = strtod(value.c_str(), (char **)NULL);
      if (mtype==MTYPE_UNKNOWN)
	mtype=Args::ourDefaultMTypeReal;
      TCON tcon = Host_To_Targ_Float(mtype, val);
      wn = Make_Const(tcon);
    } 
    else if (strcmp(type.c_str(), "integer") == 0) {
      // Integer constant: Note that we have turned off WHIRL's
      // expression simplifier, which can cause problems with our code.
      // Also note that certain special expressions need an I4INTCONST.
      // E.g.: array indices, loop updates.
      INT64 val = strtol(value.c_str(), (char **)NULL, 10);
      if (ctxt.currentXlationContext().isFlag(XlationContext::ARRAYIDX) || 
	  ctxt.currentXlationContext().isFlag(XlationContext::EXPRSIMPLE)) {
	wn = WN_CreateIntconst(OPC_I4INTCONST, (INT32)val);
      } else {
	if (mtype==MTYPE_UNKNOWN)
	  mtype=Args::ourDefaultMTypeInt;
	OPCODE opc = OPCODE_make_op(OPR_INTCONST, mtype, MTYPE_V);
	wn = WN_CreateIntconst(opc, val);
      }
    } 
    else if (strcmp(type.c_str(), "bool") == 0) {
      // Boolean constant: boolean values can be true/false or 1/0.
      unsigned int val = 1;
      const char* v = value.c_str();    
      if ((strcmp(v, "false") == 0) || (strcmp(v, "0") == 0)) {
	val = 0;
      }
      wn = CreateBoolConst(val);
    } 
    else if (strcmp(type.c_str(), "char") == 0) {
      // Character constant. Cf. cwh_stmt.cxx:349
      // wn = WN_CreateIntconst(OPC_I4INTCONST, (INT64)val);
      FORTTK_DIE(fortTkSupport::Diagnostics::Unimplemented << "creation of character constant");
    } 
    else if (strcmp(type.c_str(), "string") == 0) {
      // String constant. A string constant reference to "S" looks like:
      //   U4U1ILOAD 0 T<43,.character.,1> T<175,anon_ptr.,8>
      //     U8LDA 0 <1,596,(1_bytes)_"S"> T<127,anon_ptr.,8>
      // cf. fei_pattern_con(..) in cwh_stab.cxx
      TY_IDX ty = MTYPE_To_TY(MTYPE_STRING);
      TY_IDX ty_ptr = Stab_Pointer_To(ty);
      UINT32 len = strlen(value.c_str());
      TCON tcon = Host_To_Targ_String(MTYPE_STRING, (char*)value.c_str(), len);
      ST* st = Gen_String_Sym(&tcon, ty, FALSE);
      TYPE_ID rty = TY_mtype(ty_ptr); // Pointer_Mtype
      WN* lda = WN_CreateLda(OPR_LDA, rty, MTYPE_V, 0, ty_ptr, st, 0);
      wn = WN_CreateIload(OPR_ILOAD, MTYPE_U4, MTYPE_U1, 0, ty, ty_ptr, lda, 0);
    }
    return wn;
  }

  WN* XlateExpression::xlate_Intrinsic(OA::OA_ptr<OA::DGraph::DGraphImplement> g, 
				       OA::OA_ptr<MyDGNode> n, 
				       PUXlationContext& ctxt) {
    FORTTK_ASSERT(!g.ptrEqual(NULL) && !n.ptrEqual(NULL), fortTkSupport::Diagnostics::UnexpectedInput);
    DOMElement* elem = n->GetElem();
    const XMLCh* nmX = elem->getAttribute(XAIFStrings.attr_name_x());
    XercesStrX nm = XercesStrX(nmX);
    std::string key = GetIntrinsicKey(elem);
    bool suppressIt=(std::string("allocated")==nm.c_str() 
		     || 
		     std::string("lbound")==nm.c_str() 
		     || 
		     std::string("ubound")==nm.c_str()
		     || 
		     std::string("size")==nm.c_str());
    if (suppressIt) { 
      ctxt.currentXlationContext().setFlag(XlationContext::SUPPRESSSELECTOR);
    } 
    WN* wn = xlate_ExprOpUsingIntrinsicTable(fortTkSupport::IntrinsicXlationTable::XAIFIntrin, 
					     nm.c_str(), 
					     key.c_str(), 
					     g, 
					     n, 
					     ctxt);
    if (suppressIt) {
      ctxt.currentXlationContext().unsetFlag(XlationContext::SUPPRESSSELECTOR);
    }
    return wn;
  }

  WN* XlateExpression::xlate_Expression(OA::OA_ptr<OA::DGraph::DGraphImplement> g, 
					OA::OA_ptr<MyDGNode> n, 
					PUXlationContext& ctxt) {
    // Recursively translate the DAG (tree) rooted at this node
    DOMElement* elem = n->GetElem();
    const XMLCh* nameX = elem->getNodeName();
    XercesStrX name = XercesStrX(nameX);
    WN* wn = NULL;
    if (XMLString::equals(nameX, XAIFStrings.elem_VarRef_x())) {
      // VariableReference
      wn = xlate_VarRef(elem, ctxt);
    } 
    else if (XMLString::equals(nameX, XAIFStrings.elem_Constant_x())) {
      // Constant
      wn = translateConstant(elem, ctxt);
    } 
    else if (XMLString::equals(nameX, XAIFStrings.elem_Intrinsic_x())) {
      // Intrinsic
      wn = xlate_Intrinsic(g, n, ctxt);
    } 
    else if (XMLString::equals(nameX, XAIFStrings.elem_FuncCall_x())) {
      // FunctionCall
      wn = xlate_FunctionCall(g, n, ctxt);
    } 
    else if (XMLString::equals(nameX, XAIFStrings.elem_BoolOp_x())) {
      // BooleanOperation
      wn = xlate_BooleanOperation(g, n, ctxt);
    } 
    else {
      FORTTK_DIE("Unknown XAIF expression:\n" << *elem);
    }
    return wn;
  }

  WN* XlateExpression::xlate_VarRef(const DOMElement* elem, PUXlationContext& ctxt) {
    FORTTK_ASSERT(elem, fortTkSupport::Diagnostics::UnexpectedInput);
    // VariableReferenceType
    bool deriv = GetDerivAttr(elem);
    // skip the xaif:VariableReference node
    DOMElement* varref = GetFirstChildElement(elem);
    if (ctxt.currentXlationContext().isFlag(XlationContext::SUPPRESSSELECTOR)) { 
      ctxt.createXlationContext();
      // to make sure:
      ctxt.currentXlationContext().unsetFlag(XlationContext::DERIVSELECTOR);
      ctxt.currentXlationContext().unsetFlag(XlationContext::VALUESELECTOR);
    }  
    else 
      ctxt.createXlationContext((deriv) ? XlationContext::DERIVSELECTOR : XlationContext::VALUESELECTOR);
    WN* wn = translateVarRef(varref, ctxt);
    ctxt.deleteXlationContext();
    return wn;
  }

  WN* XlateExpression::xlate_VarRef(OA::OA_ptr<OA::DGraph::DGraphImplement> g, 
				    OA::OA_ptr<MyDGNode> n, 
				    PUXlationContext& ctxt) {
    FORTTK_ASSERT(!g.ptrEqual(NULL) && !n.ptrEqual(NULL), fortTkSupport::Diagnostics::UnexpectedInput);
    // Recursively translate the DAG (tree) rooted at this node
    DOMElement* elem = n->GetElem();
    FORTTK_ASSERT(elem, "Internal error: var-ref graph contains null DOM elem.");
    WN* wn = NULL;  
    const XMLCh* nameX = elem->getNodeName();
    if (XMLString::equals(nameX, XAIFStrings.elem_SymRef_x())) {
      // SymbolReference
      wn = xlate_SymbolReference(elem, ctxt);
    } 
    else if (XMLString::equals(nameX, XAIFStrings.elem_ArrayElemRef_x())) {
      // ArrayElementReference
      wn = xlate_ArrayElementReference(g, n, ctxt);
    } 
    else {
      FORTTK_DIE("Unknown XAIF variable reference:\n" << *elem);
    }
    return wn;
  }

  WN* XlateExpression::xlate_FunctionCall(OA::OA_ptr<OA::DGraph::DGraphImplement> g, 
					  OA::OA_ptr<MyDGNode> n,
					  PUXlationContext& ctxt) {
    FORTTK_ASSERT(!g.ptrEqual(NULL) && !n.ptrEqual(NULL), fortTkSupport::Diagnostics::UnexpectedInput);
    DOMElement* elem = n->GetElem();
    // FIXME: children are expr; find num of args (use Intrinsic function above)
    FORTTK_DIE(fortTkSupport::Diagnostics::Unimplemented << "xaif:FunctionCall");
    return NULL;
  }

  WN* XlateExpression::xlate_BooleanOperation(OA::OA_ptr<OA::DGraph::DGraphImplement> g, 
					      OA::OA_ptr<MyDGNode> n,
					      PUXlationContext& ctxt) {
    FORTTK_ASSERT(!g.ptrEqual(NULL) && !n.ptrEqual(NULL), fortTkSupport::Diagnostics::UnexpectedInput);
    DOMElement* elem = n->GetElem();
    const XMLCh* nmX = elem->getAttribute(XAIFStrings.attr_name_x());
    XercesStrX nm = XercesStrX(nmX);
    WN* wn = xlate_ExprOpUsingIntrinsicTable(fortTkSupport::IntrinsicXlationTable::XAIFBoolOp, 
					     nm.c_str(), 
					     NULL, 
					     g, 
					     n, 
					     ctxt);
    return wn;
  }

  struct XAIFEdgePositionCompare {
    // return true if e1 < e2; false otherwise
    bool operator()(const OA::OA_ptr<MyDGEdge> e1, 
		    const OA::OA_ptr<MyDGEdge> e2) const {
      unsigned int pos1 = GetPositionAttr(e1->GetElem());
      unsigned int pos2 = GetPositionAttr(e2->GetElem());
      return (pos1 < pos2);
    }
  };

  bool conversionToReal(fortTkSupport::IntrinsicXlationTable::WHIRLInfo& info) { 
    if (info.opr==OPR_CALL) {
      const std::string oprName(info.name);
      if (
	  oprName.compare("REAL")==0
	  ||
	  oprName.compare("DBLE")==0
	  || 
	  oprName.compare("FLOAT")==0
	  || 
	  oprName.compare("AIMAG")==0
	  )
	return true;
    } 
    return false; 
  } 

  WN* XlateExpression::xlate_ExprOpUsingIntrinsicTable(const fortTkSupport::IntrinsicXlationTable::XAIFOpr xopr, 
						       const char* xoprNm, 
						       const char* xIntrinKey,
						       OA::OA_ptr<OA::DGraph::DGraphImplement> g, 
						       OA::OA_ptr<MyDGNode> n, 
						       PUXlationContext& ctxt) {
    using namespace OA::DGraph;
    fortTkSupport::IntrinsicXlationTable::WHIRLInfo* aWhirlInfo_p = 
      IntrinsicTable.findWHIRLInfo(xopr, xoprNm, xIntrinKey);
    // 1. Gather the operands, sorted by the "position" attribute
    unsigned int actualArgCount(n->num_incoming());
    FORTTK_ASSERT_WARN(actualArgCount<= aWhirlInfo_p->numop && actualArgCount>= aWhirlInfo_p->numop-aWhirlInfo_p->numOptional, 
		       "Warning: get " << actualArgCount<< " intrinsic arguments for '"
		       << xoprNm  << "' expect " << aWhirlInfo_p->numop << " (have " << aWhirlInfo_p->numOptional << " optional args?)");
    OA::OA_ptr<MyDGEdge> tmp; tmp = NULL;
    vector<OA::OA_ptr<MyDGEdge> > opnd_edge(actualArgCount, tmp);
    OA::OA_ptr<EdgesIteratorInterface> itPtr 
      = n->getIncomingEdgesIterator();
    for (int i = 0; itPtr->isValid(); ++(*itPtr), ++i) {
      OA::OA_ptr<OA::DGraph::EdgeInterface> etmp = itPtr->current();
      opnd_edge[i] = etmp.convert<MyDGEdge>();
    }
    std::sort(opnd_edge.begin(), opnd_edge.end(),XAIFEdgePositionCompare()); // ascending
    // 2. Translate each operand into a WHIRL expression tree
    vector<WN*> opnd_wn(actualArgCount, NULL); 
    for (unsigned i = 0; i < actualArgCount; ++i) {
      OA::OA_ptr<NodeInterface> ntmp = opnd_edge[i]->getSource();
      OA::OA_ptr<MyDGNode> opnd = ntmp.convert<MyDGNode>();
      opnd_wn[i] = xlate_Expression(g, opnd, ctxt);
    }       
    if (aWhirlInfo_p->opr==OPR_CALL 
	&& 
	(aWhirlInfo_p->name && strcmp(aWhirlInfo_p->name,"TRANSFER")==0
	 ||
	 aWhirlInfo_p->name && strcmp(aWhirlInfo_p->name,"ASSOCIATED")==0
	 ||
	 aWhirlInfo_p->name && strcmp(aWhirlInfo_p->name,"SIZE")==0)) { 
      // leave transfer arguments untouched
      // std::cout << "XlateExpression::xlate_ExprOpUsingIntrinsicTable: skipping argument conversion for transfer" << std::endl; 
    }
    else { 
      // Here promote all arguments up to 8 bytes; we assume canonicalized
      // argument forms
      // FIXME: for now we promote reals to 8; demote ints to 4; we could
      // selectively do this...
      for (unsigned i = 0; i < opnd_wn.size(); ++i) {
	if (WN_operator(opnd_wn[i])==OPR_CALL 
	    && 
	    (strcmp(ST_name(WN_st(opnd_wn[i])),"TRANSFER")==0
	     ||
	     strcmp(ST_name(WN_st(opnd_wn[i])),"ASSOCIATED")==0
	     ||
	     strcmp(ST_name(WN_st(opnd_wn[i])),"SIZE")==0)) { 
	  // leave transfer result untouched
	  //  std::cout << "XlateExpression::xlate_ExprOpUsingIntrinsicTable: skipping result conversion for transfer" << std::endl; 
	}
	else { 
	  // FIXME: could use rtype for operator
	  TY_IDX ty = WN_Tree_Type(opnd_wn[i]);
	  TYPE_ID rty = TY_mtype(ty);      
	  // Pointers and character strings often look like integers to the
	  // test below
	  if (TY_Is_Pointer(ty) || TY_Is_Character_String(ty)) { continue; }
	  TYPE_ID newrty = MTYPE_UNKNOWN;
	  if (MTYPE_is_integral(rty)) {
	    newrty = getMType(MTYPE_CLASS_INTEGER, 4);
	  }
	  else if (MTYPE_is_float(rty)) {
	    newrty = getMType(MTYPE_CLASS_FLOAT, 8);
	  }
	  // WN_set_rtype(opnd_wn[i], newrty);
	  if (newrty != MTYPE_UNKNOWN 
	      && 
	      newrty != rty
	      && 
	      ! conversionToReal(*aWhirlInfo_p)) {
	    opnd_wn[i] = WN_Cvt(rty, newrty, opnd_wn[i]);
	  }
	}
      }
    }
    // 3. Translate into either WHIRL OPR_CALL or a WHIRL expression operator
    WN* wn = NULL;
    switch (aWhirlInfo_p->oprcl) {
    case fortTkSupport::IntrinsicXlationTable::WNCall: {
      TYPE_ID rtype = MTYPE_F8; // FIXME
      wn = CreateCallToIntrin(rtype, aWhirlInfo_p->name, opnd_wn);
      break;
    }
    case fortTkSupport::IntrinsicXlationTable::WNIntrinCall:
    case fortTkSupport::IntrinsicXlationTable::WNIntrinOp: {          
      TYPE_ID rtype = MTYPE_F8; // FIXME
      TYPE_ID dtype = MTYPE_V;  // FIXME
      INTRINSIC intrn = getWNIntrinsic(aWhirlInfo_p->name, opnd_wn, NULL);
      wn = CreateIntrinsicCall(aWhirlInfo_p->opr, intrn, rtype, dtype, opnd_wn);
      break;
    }
    case fortTkSupport::IntrinsicXlationTable::WNExpr: {
      // Find the opcode for the expression
      OPCODE opc = getWNExprOpcode(aWhirlInfo_p->opr, opnd_wn);
      // Create a WHIRL expression tree for the operator and operands
      switch (actualArgCount) {
      case 1: // unary
	wn = WN_CreateExp1(opc, opnd_wn[0]); break;
      case 2: // binary
	wn = WN_CreateExp2(opc, opnd_wn[0], opnd_wn[1]); break;
      case 3: // ternary
	wn = WN_CreateExp3(opc, opnd_wn[0], opnd_wn[1], opnd_wn[2]); break;
      default:
	FORTTK_DIE("Incorrect number of operands for WHIRL expr: " << actualArgCount);
      } 
      break;
    }
    default:
      FORTTK_DIE("Invalid WNOprClass class: " << aWhirlInfo_p->oprcl);
    }
    return wn;
  }

  // helper operator
  struct replaceIfLocal {
  public:
    ST** mySt;
    ST* myStCB;
    ST_IDX myOwnPUIdx;
    replaceIfLocal(ST** st,
		   ST* stCB,
		   ST_IDX myPUIdx):
      mySt(st),
      myStCB(stCB),
      myOwnPUIdx(myPUIdx) {
    }
    void operator()(UINT32, ST* st) const {
      if (ST_sclass(st)==SCLASS_COMMON
	  && 
	  ST_sclass(ST_base(st))==SCLASS_COMMON
	  && 
	  myOwnPUIdx==ST_st_idx(ST_base(ST_base(st)))
	  && 
	  strcmp(ST_name(ST_base(st)),ST_name(myStCB))==0
	  && 
	  strcmp(ST_name(st),ST_name(*mySt))==0)
	*mySt=st;
    }
  };

  WN* XlateExpression::xlate_SymbolReference(const DOMElement* elem, 
					     PUXlationContext& ctxt) {
    typedef std::map<ST_IDX,std::set<ST*> >  PuIdxToSTPSetMap;
    // this map is supposed to retain a representer common block (associated with another PU)
    // for references to common block variables that did not exist in the original PU
    static PuIdxToSTPSetMap globPUtoCBMap; 
    FORTTK_ASSERT(elem, fortTkSupport::Diagnostics::UnexpectedInput);
    // -------------------------------------------------------
    // 0. Setup; Possibly redirect processing
    // -------------------------------------------------------
    WN* wn = NULL;
    fortTkSupport::Symbol* sym = GetSymbol(elem, ctxt);
    if (sym->IsActive()) {
      ctxt.currentXlationContext().setFlag(XlationContext::ACTIVETYPE); // N.B. inherited up the ctxt stack
    }
    // redirect handling if access path was collapsed (includes scalarization)
    if (sym->IsPathCollapsed()) {
      WN* pathVorlage = ctxt.findWN(sym->GetPathVorlage(), true /* mustFind */);
      return xlate_SymbolReferenceCollapsedPath(elem, pathVorlage, ctxt);
    }
    ST* st = sym->GetST();
    // see what kind of symbol this is
    if (ST_sclass(st)==SCLASS_COMMON) { 
      // if it is a common block variable we need to 
      // avoid picking up duplicate common block definitions
      ST* stCB=ST_base(st);
      if (ST_sclass(stCB)!=SCLASS_COMMON) 
	FORTTK_DIE("Expect a common block name here");
      ST* stPU=ST_base(stCB);
      ST_IDX otherPUIdx=ST_st_idx(stPU);
      ST_IDX myPUIdx=Current_PU_Info->proc_sym;
      // std::cout << "doing " << ST_name(stPU) << "::" << ST_name(stCB) << "::" << ST_name(st) << " while in " << ST_name(St_Table[myPUIdx]) << std::endl;
      if (myPUIdx!=otherPUIdx) { // not the same PU
	// see if there is already a local equivalent to the common block variable 
	// used in the other PU. 
	ST* givenST_p=st;
	For_all(St_Table,GLOBAL_SYMTAB,replaceIfLocal(&st,stCB,myPUIdx));
	// std::cout << "tried for local CB:  " << ST_name(ST_base(ST_base(st))) << "::" << ST_name(ST_base(st)) << "::" << ST_name(st) << " while in " << ST_name(St_Table[myPUIdx]) << std::endl;
	// if this is replaces that means we can map it to the common block variant in this PU
	if (st==givenST_p) { // not replaced, meaning this common block did not previously exist in this PU
	  PuIdxToSTPSetMap::iterator it=globPUtoCBMap.find(myPUIdx);
	  if (it==globPUtoCBMap.end()) { // we haven't seen this common block before
	    // add it to the map and put the CB ST* into the set
	    (globPUtoCBMap[myPUIdx]).insert(stCB);
	  }
	  else {
	    std::set<ST*>& theCBset=it->second;
	    std::set<ST*>::iterator sIt;
	    for (sIt=theCBset.begin();
		 sIt!=theCBset.end();
		 ++sIt) { 
	      if (strcmp(ST_name(*sIt),ST_name(stCB))==0) {
		// got the CB already in the set
		For_all(St_Table,GLOBAL_SYMTAB,replaceIfLocal(&st,*sIt,ST_st_idx(ST_base(*sIt))));
		// std::cout << "tried for other CB: " << ST_name(ST_base(ST_base(st))) << "::" << ST_name(ST_base(st)) << "::" << ST_name(st) << " while in " << ST_name(St_Table[myPUIdx]) << std::endl;
		break;
	      }
	    }
	    if (sIt==theCBset.end()) { 
	      // wasn't in the set, add it
	      (globPUtoCBMap[myPUIdx]).insert(stCB);
	    } 
	  }
	}
      }
    }
    const char* st_name = ST_name(st);
    TY_IDX ty = ST_type(st);
    TYPE_ID rty, dty;
    WN_OFFSET oset = 0;
    // -------------------------------------------------------
    // 1. Determine which type of load to use
    // -------------------------------------------------------
    bool create_lda = false;
    // Note: Order matters in these tests
    if (ST_class(st) != CLASS_PREG) { // never create a pointer to a preg
      if (ctxt.currentXlationContext().isFlag(XlationContext::ARRAY)
	  ||
	  TY_Is_Chararray(ty)) {
	// Do not load the address of symbol that is already a pointer
	if (TY_kind(ty) != KIND_POINTER) {
	  create_lda = true;
	}
      } 
      else if (ctxt.currentXlationContext().isFlag(XlationContext::LVALUE)) {
	create_lda = true;
      } 
    }
    // -------------------------------------------------------
    // 2. Create the reference
    // -------------------------------------------------------
    if (create_lda) {
      // OPR_LDA
      TY_IDX ty_ptr = Stab_Pointer_To(ty);
      rty = TY_mtype(ty_ptr); // Pointer_Mtype
      wn = WN_CreateLda(OPR_LDA, rty, MTYPE_V, oset, ty_ptr, st, 0);
    } 
    else {
      // OPR_LDID
      rty = dty = TY_mtype(ty);
      if (TY_kind(ty) == KIND_ARRAY) { // FIXME more special cases?
	rty = dty = TY_mtype(TY_etype(ty));
      }
      // FIXME: take care of small integer types
      if (MTYPE_byte_size(dty) < 4) {
	if (MTYPE_is_unsigned(dty)) { rty = Args::ourDefaultMTypeUInt; }
	else if (MTYPE_is_signed(dty)) { rty = Args::ourDefaultMTypeInt; }
      }
      if (ST_class(st) == CLASS_PREG) {
	oset = GetPregId(elem);
      }
      wn = WN_CreateLdid(OPR_LDID, rty, dty, oset, st, ty, 0);
    } 
    return wn;
  }

  std::pair<ST*, WN_OFFSET> XlateExpression::xlate_SymbolReferenceSimple(const DOMElement* elem,
									 PUXlationContext& ctxt) {
    fortTkSupport::Symbol* sym = GetSymbol(elem, ctxt);
    ST* st = sym->GetST();
    WN_OFFSET oset = 0;
    if (ST_class(st) == CLASS_PREG) {
      oset = GetPregId(elem);
    }
    return make_pair(st, oset);
  }

  WN* XlateExpression::xlate_SymbolReferenceCollapsedPath(const DOMElement* elem, 
							  WN* pathVorlageWN,
							  PUXlationContext& ctxt) {
    OPERATOR opr = WN_operator(pathVorlageWN);
    bool create_lda = (ctxt.currentXlationContext().isFlag(XlationContext::LVALUE)); // FIXME  
    WN* wn = NULL;  
    switch (opr) {
    case OPR_STID: 
    case OPR_STBITS: {
      // Create an LDID
      TY_IDX ty_idx = WN_GetRefObjType(pathVorlageWN);
      TYPE_ID rty = TY_mtype(ty_idx); // OPCODE_rtype(WN_opcode())
      TYPE_ID dty = TY_mtype(ty_idx); // OPCODE_dtype(WN_opcode())
      ST* st = WN_st(pathVorlageWN);
      WN_OFFSET ofst = WN_offset(pathVorlageWN);
      UINT fid = WN_field_id(pathVorlageWN);
#if 0 // FIXME
      if (!TY_Is_Pointer(ty_idx)) {
	ty_idx = Stab_Pointer_To(ty_idx);
      }
      TYPE_ID rty = TY_mtype(ty_idx);
      wn = WN_CreateLda(OPR_LDA, rty, MTYPE_V, ofst, ty_idx, st, fid);
#endif
      wn = WN_CreateLdid(OPR_LDID, rty, dty, ofst, st, ty_idx, fid);
      break;
    }
    case OPR_ISTORE: 
    case OPR_ISTBITS: {
      // Create an ILOAD (copy ISTORE.kid1 to ILOAD.kid0)
      WN* addr = WN_COPY_Tree(WN_kid1(pathVorlageWN));
      TY_IDX refty = WN_GetRefObjType(pathVorlageWN);
      TY_IDX ptrty = WN_ty(pathVorlageWN);
      TYPE_ID mty = TY_mtype(refty);
      WN_OFFSET ofst = WN_offset(pathVorlageWN); 
      if (OPERATOR_has_offset(WN_operator(addr))) {
	ofst += WN_offset(addr); // ISTORE.offset + ISTORE.kid1.offset
      }
      UINT fid = WN_field_id(pathVorlageWN);
      wn = WN_CreateIload(OPR_ILOAD, mty, mty, ofst, refty, ptrty, addr, fid);
      break;
    }
    case OPR_LDA:
    case OPR_LDMA:
    case OPR_LDID:
    case OPR_LDBITS: 
    case OPR_ILOAD: 
    case OPR_ILDBITS:
    case OPR_STRCTFLD:
      wn = WN_COPY_Tree(pathVorlageWN);
      break;
    default: 
      break; // fall through
    } // switch
    FORTTK_ASSERT(wn, fortTkSupport::Diagnostics::Unimplemented << "Unable to recreate collapsed scalarized path.");
    //if (!create_lda) {
    //}
    return wn;
  }

  WN* XlateExpression::xlate_ArrayElementReference(OA::OA_ptr<OA::DGraph::DGraphImplement> g, 
						   OA::OA_ptr<MyDGNode> n, 
						   PUXlationContext& ctxt) {
    FORTTK_ASSERT(!g.ptrEqual(NULL) && !n.ptrEqual(NULL), fortTkSupport::Diagnostics::UnexpectedInput);
    DOMElement* elem = n->GetElem();
    WN* arrWN = 0;
    // ----------------------------------------------------------------------------------
    // Translate the array symbol reference so we can query the type
    OA::OA_ptr<MyDGNode> n1 = GetSuccessor(n, false /* succIsOutEdge */);
    const XMLCh* nmX = n1->GetElem()->getNodeName();
    FORTTK_ASSERT(XMLString::equals(nmX, XAIFStrings.elem_SymRef_x()),
		  "Expected " << XAIFStrings.elem_SymRef() << "; found:\n"
		  << *(n1->GetElem()));
    const XMLCh* symNmX = n1->GetElem()->getAttribute(XAIFStrings.attr_symId_x());
    XercesStrX symNm = XercesStrX(symNmX);
    ctxt.createXlationContext(XlationContext::ARRAY);
    WN* arraySym = xlate_VarRef(g, n1, ctxt);
    ctxt.deleteXlationContext();
    TY_IDX ty;
    if (WN_operator(arraySym)==OPR_STRCTFLD) { 
      ty=WN_GetRefObjType(arraySym);
    }
    else if (WN_kid0(arraySym) && WN_operator(WN_kid0(arraySym))==OPR_STRCTFLD ) {
      ty=WN_GetRefObjType(WN_kid0(arraySym));
    }
    else{
      ty = ST_type(WN_st(arraySym));
    }
    if (TY_kind(ty) == KIND_POINTER) { 
      ty = TY_pointed(ty); 
    }
    if (TY_Is_Character_String(ty)) {
      // ----------------------------------------------------------------------------------
      // treat character arrays separately because they have a special whirl format
      // there is to be one child in xaif expected to be an IndexTriplet
      FORTTK_ASSERT(GetChildElementCount(elem)==1,"Internal error: unexpected character array with multiple indices for >" <<  symNm.c_str() << "<");
      unsigned int childCount = 2; // there are two whirl child nodes
      DOMElement* dim = GetFirstChildElement(elem); 
      FORTTK_ASSERT(XMLString::equals(dim->getNodeName(), XAIFStrings.elem_IndexTriplet_x()), 
		    "Expected " << XAIFStrings.elem_IndexTriplet() << "; found:\n"
		    << *dim);
      DOMElement* tripletElementExpr = GetFirstChildElement(dim);
      UINT tripletElementCounter=0;
      vector<WN*> triplet(3);
      for (; tripletElementExpr; tripletElementExpr = GetNextSiblingElement(tripletElementExpr),++tripletElementCounter) {
	DOMElement* firstChild=GetFirstChildElement(tripletElementExpr);
	if (firstChild) { 
	  ctxt.createXlationContext(XlationContext::ARRAYIDX);
	  WN* indexExprWN = translateExpression(firstChild, ctxt);
	  ctxt.deleteXlationContext();
	  const XMLCh* tripletElementnmX = tripletElementExpr->getNodeName();
	  if (XMLString::equals(tripletElementnmX, XAIFStrings.elem_Index_x()))
	    triplet[0]=indexExprWN;
	  else if (XMLString::equals(tripletElementnmX, XAIFStrings.elem_Bound_x()))
	    triplet[1]=indexExprWN;
	  else
	    FORTTK_DIE("unexpected element :" << *tripletElementExpr);
	}
      }
      FORTTK_ASSERT(tripletElementCounter==2,"Internal error: character array has to have 2 triplet elements for >" <<  symNm.c_str() << "<");
      // Create Whirl ARRAY node 
      arrWN = WN_Create(OPR_ARRAY, MTYPE_U8, MTYPE_V, 3);
      // kid 0 is the array's base address
      WN_kid0(arrWN) = arraySym;
      WN_kid(arrWN, 1)=triplet[1];
      WN_kid(arrWN, 2)=triplet[0];
    }
    else { // is not a character array
      // ----------------------------------------------------------------------------------
      // Translate the index expression for each dimension
      unsigned int rank = GetChildElementCount(elem);
      vector<WN*> indices(rank,(WN*)0); 
      DOMElement* dim = GetFirstChildElement(elem);
      for (int i = 0; dim; dim = GetNextSiblingElement(dim), ++i) {
	const XMLCh* nmX = dim->getNodeName();
	FORTTK_ASSERT(XMLString::equals(nmX, XAIFStrings.elem_IndexTriplet_x()), 
		      "Expected " << XAIFStrings.elem_IndexTriplet() << "; found:\n"
		      << *dim);
	DOMElement* tripletElementExpr = GetFirstChildElement(dim);
	UINT tripletElementCounter=0;
	vector<WN*> triplet(3,(WN*)0);
	for (; tripletElementExpr; tripletElementExpr = GetNextSiblingElement(tripletElementExpr),++tripletElementCounter) {
	  DOMElement* firstChild=GetFirstChildElement(tripletElementExpr);
	  if (firstChild) { 
	    ctxt.createXlationContext(XlationContext::ARRAYIDX);
	    WN* indexExprWN = translateExpression(firstChild, ctxt);
	    ctxt.deleteXlationContext();
	    const XMLCh* tripletElementnmX = tripletElementExpr->getNodeName();
	    if (XMLString::equals(tripletElementnmX, XAIFStrings.elem_Index_x()))
	      triplet[0]=indexExprWN;
	    else if (XMLString::equals(tripletElementnmX, XAIFStrings.elem_Bound_x()))
	      triplet[1]=indexExprWN;
	    else if (XMLString::equals(tripletElementnmX, XAIFStrings.elem_Stride_x()))
	      triplet[2]=indexExprWN;
	    else
	      FORTTK_DIE("unexpected element :" << *tripletElementExpr);
	  }
	}
	if (tripletElementCounter==1 && triplet[0]) 
	  indices[i] = WN_Type_Conversion(triplet[0],MTYPE_I4);
	else { 
	  WN* theSrcTriplet_p=WN_Create(OPR_SRCTRIPLET,
					MTYPE_I8, 
					MTYPE_V,
					3);
	  for (int j=0; j<3; ++j) {
	    if (triplet[j]!=0) { 
	      WN_kid(theSrcTriplet_p,j)=WN_Type_Conversion(triplet[j],MTYPE_I4);
	    }
	    else { 
	      if (j!=2)
		WN_kid(theSrcTriplet_p,j)=WN_Create(OPR_IMPLICIT_BND,
						    MTYPE_V, 
						    MTYPE_V,
						    0);
	      else // the last one (stride) should default to 1
		WN_kid(theSrcTriplet_p,j)=WN_CreateIntconst(OPC_I4INTCONST, 1);
	    }
	  }
	  indices[i] = theSrcTriplet_p;
	} 
      }
      FORTTK_ASSERT(TY_AR_ndims(ty) == (INT32)rank,
		    "Internal error: mismatched array dimensions");
      // -------------------------------------------------------
      // Create Whirl ARRAY node (cf. wn_fio.cxx:1.3:7055)
      UINT nkids = (rank * 2) + 1; // 2n + 1 where (where n == rank)
      arrWN = WN_Create(OPR_ARRAY, MTYPE_U8, MTYPE_V, nkids);
      // kid 0 is the array's base address
      WN_kid0(arrWN) = arraySym;
      // kids 1 to n give size of each dimension.  We use a bogus value,
      // since we need to support only translation back to source code.
      for (unsigned i = 1; i <= rank; ++i) {
	WN_kid(arrWN, i) = WN_CreateIntconst(OPC_I4INTCONST, 0);
      }
      // kids n + 1 to 2n give index expressions for each dimension.  
      // N.B. Reverse the order of index expressions since we are
      // translating Fortran.  FIXME: should we change whirl2xaif and this
      // to not reverse the indices?
      for (unsigned i = 2*rank, j = 0; i >= (rank + 1); --i, ++j) {
	WN_kid(arrWN, i) = indices[j];
      }
      // -------------------------------------------------------
      // Wrap the ARRAY in an ILOAD
      TY_IDX ety = TY_etype(ty);
      TYPE_ID emty = TY_mtype(ety);
      TY_IDX eptrty = Stab_Pointer_To(ety);
      if (emty==0 && TY_Is_Chararray(ty)) {
	emty=MTYPE_U4;
      }
      arrWN=WN_CreateIload(OPR_ILOAD, emty, emty, 0, ety, eptrty, arrWN, 0);
    }
    return arrWN;
  }

  OA::OA_ptr<OA::DGraph::DGraphImplement> XlateExpression::createExpressionGraph(const DOMElement* elem, 
										bool varRef) {
    using namespace OA::DGraph;
    MyDGNode::resetIds();
    OA::OA_ptr<DGraphImplement> g; g = new DGraphImplement();
    VertexIdToMyDGNodeMap m;
    // Setup variables
    XMLCh* edgeStr = NULL;
    if (varRef) {
      edgeStr = XAIFStrings.elem_VarRefEdge_x();
    } else {
      edgeStr = XAIFStrings.elem_ExprEdge_x();
    }
    // -------------------------------------------------------
    // Create the graph (only examine element nodes)
    // -------------------------------------------------------
    DOMElement* e = const_cast<DOMElement*>(elem);
    do {
      const XMLCh* name = e->getNodeName();
      if (XMLString::equals(name, edgeStr)) {
	// Add an edge to the graph.  N.B.: we ignore the 'position'
	// attribute during creation of the graph.
	// Find src and target (sink) nodes. 
	const XMLCh* srcX = e->getAttribute(XAIFStrings.attr_source_x());
	const XMLCh* targX = e->getAttribute(XAIFStrings.attr_target_x());
	XercesStrX src = XercesStrX(srcX);
	XercesStrX targ = XercesStrX(targX);
	OA::OA_ptr<MyDGNode> gn1 = m[std::string(src.c_str())];  // source
	OA::OA_ptr<MyDGNode> gn2 = m[std::string(targ.c_str())]; // target
	FORTTK_ASSERT(!gn1.ptrEqual(NULL) && !gn2.ptrEqual(NULL), "Invalid edge in expression graph:\n" << *e);
	OA::OA_ptr<MyDGEdge> ge; ge = new MyDGEdge(gn1, gn2, e); // src, targ
	g->addEdge(ge);
      } 
      else {
	// Add a vertex to the graph
	const XMLCh* vidX = e->getAttribute(XAIFStrings.attr_Vid_x());
	XercesStrX vid = XercesStrX(vidX);
	FORTTK_ASSERT(strlen(vid.c_str()) > 0, 
		      "Invalid vertex in expression graph:\n" << *e);
	OA::OA_ptr<MyDGNode> gn; gn = new MyDGNode(e);
	g->addNode(gn);
	m[std::string(vid.c_str())] = gn;
      } 
    } while ( (e = GetNextSiblingElement(e)) );
    return g;
  }

  WN* XlateExpression::createValueSelector(WN* wn) {
    TYPE_ID rty = getRType(wn);
    WN* callWN = CreateCallToIntrin(rty, "__value__", 1);
    WN_actual(callWN, 0) = CreateParm(wn, WN_PARM_BY_VALUE);
    return callWN;
  }

  WN* XlateExpression::createDerivSelector(WN* wn) {
    TYPE_ID rty = getRType(wn);
    WN* callWN = CreateCallToIntrin(rty, "__deriv__", 1);
    WN_actual(callWN, 0) = CreateParm(wn, WN_PARM_BY_VALUE);
    return callWN;
  }

  TYPE_ID XlateExpression::getRType(WN* wn) {
    TY_IDX ty_idx = WN_Tree_Type(wn);
    TYPE_ID rty = MTYPE_UNKNOWN;
    if (TY_kind(ty_idx) == KIND_POINTER) { 
      ty_idx=TY_pointed(ty_idx);
    }
    if (TY_kind(ty_idx) == KIND_ARRAY) { 
      rty=TY_mtype(TY_etype(ty_idx));
    }
    else if (TY_kind(ty_idx) == KIND_STRUCT) {
      rty = MTYPE_M;
    } else {
      rty = TY_mtype(ty_idx);
    }
    // FIXME: pointer types
    FORTTK_ASSERT(rty != MTYPE_UNKNOWN, "Error finding rtype of WN expr"); 
    return rty;
  }

  TYPE_ID XlateExpression::getRTypeFromOpands(vector<WN*>& opands) {
    int opands_num = opands.size();
    FORTTK_ASSERT(opands_num > 0, fortTkSupport::Diagnostics::UnexpectedInput);
    // 1. Gather types for operands
    vector<TY_IDX> opands_ty(opands_num);
    vector<TYPE_ID> opands_mty(opands_num);
    for (int i = 0; i < opands_num; ++i) {
      TY_IDX ty = WN_Tree_Type(opands[i]);
      opands_ty[i] = ty;
      if (TY_kind(ty) == KIND_ARRAY) 
	opands_mty[i] = TY_mtype(TY_etype(ty));
      else if (TY_kind(ty) == KIND_POINTER) 
	opands_mty[i] = TY_mtype(TY_pointed(ty));
      else 
	opands_mty[i] = TY_mtype(ty);
    }
    // 2. Find an appropriate mtype for operands
    TYPE_ID mty = opands_mty[0];
    for (int i = 1; i < opands_num; ++i) {
      mty = getRTypeFromOpands(mty, opands_mty[i]);
    }
    return mty;
  }

  TYPE_ID XlateExpression::getRTypeFromOpands(TYPE_ID ty1, TYPE_ID ty2) {
    // -------------------------------------------------------
    // 1. If both types are same, the answer is easy
    // -------------------------------------------------------
    if (ty1 == ty2) {
      return ty1;
    } 
    // -------------------------------------------------------
    // 2. We have different types.
    // -------------------------------------------------------
    // 2a. If we have different, but compatible classes --> class promotion
    unsigned int cl1 = MTYPE_type_class(ty1);
    unsigned int cl2 = MTYPE_type_class(ty2);
    unsigned int cl = 0; // the new class
    if (cl1 == cl2) {
      cl = cl1;
    }
    // (u)int, float --> float
    if ( ((cl1 == MTYPE_CLASS_INTEGER || cl1 == MTYPE_CLASS_UNSIGNED_INTEGER) 
	  && (cl2 == MTYPE_CLASS_FLOAT))
	 ||
	 ((cl2 == MTYPE_CLASS_INTEGER || cl2 == MTYPE_CLASS_UNSIGNED_INTEGER)
	  && (cl1 == MTYPE_CLASS_FLOAT)) ) {
      cl = MTYPE_CLASS_FLOAT;
    }
    // int, uint --> [error]
    // !complex_float, complex_float --> [error]
    // !str, str --> [error]
    if (cl == 0) {
      return ty1; // FIXME: what to do here?
    }
    // 2b. If we have different sizes --> size promotion (choose larger)
    unsigned int sz1 = MTYPE_byte_size(ty1);
    unsigned int sz2 = MTYPE_byte_size(ty2);
    unsigned int sz = MAX(sz1, sz2); // the new size
    // 2c. Combine class and size information. 
    TYPE_ID ty = getMType(cl, sz);
    return ty;
  }

  TYPE_ID XlateExpression::getMType(unsigned int cl, unsigned int bytesz) {
    TYPE_ID ty = MTYPE_UNKNOWN;
    for (TYPE_ID i = MTYPE_FIRST; i <= MTYPE_LAST; ++i) {
      if ((MTYPE_type_class(i) == cl) && (MTYPE_byte_size(i) == bytesz)) {
	ty = i;
	break;
      }
    }
    return ty;
  }

  OPCODE XlateExpression::getWNExprOpcode(OPERATOR opr, vector<WN*>& opands) {
    // 1. Find mtype suggested from operands
    TYPE_ID mty = getRTypeFromOpands(opands);
    // 2. Find a dtype (operator dependent) FIXME/FIXME
    TYPE_ID dty = MTYPE_V; // typical dtype for intrinsics
    if (opr == OPR_TRUNC || 
	opr == OPR_EQ || opr == OPR_NE || opr == OPR_GT || opr == OPR_GE ||
	opr == OPR_LT || opr == OPR_LE) {
      dty = mty;
    } 
    // 3. Find a rtype (operator dependent)
    // FIXME: we need a better way; do we need a cvt? FIXME
    // Is_Valid_Opcode, Is_Valid_Opcode_Parts
    TYPE_ID rty = mty;
    if (opr == OPR_SQRT && MTYPE_is_integral(rty)) {
      // sqrt: f, z
      rty = getMType(MTYPE_CLASS_FLOAT, MTYPE_byte_size(rty));
    } 
    else if (opr == OPR_TRUNC || opr == OPR_MOD || opr == OPR_REM) {
      // trunc, mod, rem: i
      rty = getMType(MTYPE_CLASS_INTEGER, MTYPE_byte_size(rty));
    } 
    else if (opr == OPR_EQ || opr == OPR_NE || opr == OPR_GT || opr == OPR_GE ||
	     opr == OPR_LT || opr == OPR_LE || opr == OPR_LNOT ) {
      rty = getMType(MTYPE_CLASS_INTEGER, 4);
    }
    else if (opr == OPR_COMPLEX ) {
      rty = getMType(MTYPE_CLASS_COMPLEX_FLOAT,MTYPE_byte_size(mty));
    }
    OPCODE opc = OPCODE_make_op(opr, rty, dty);
    return opc;
  }

  INTRINSIC XlateExpression::getWNIntrinsic(const char* intrnNm, vector<WN*>& opands, TYPE_ID* dtype) {
    // 1. Find dtype suggested from operands
    TYPE_ID mty = getRTypeFromOpands(opands);
    // per Nathan the situation is that there is 
    // a method to translate the whirl intrinsic enumerations 
    // into a  name but whirl doesn't have a function to 
    // translate the name back for all of the intrinsic names
    // because often the intrinsic name known in 
    // get_intrinsic_from_name 
    // includes the machine type 
    // Because we don't include the machine type 
    // we hardcode the intrinsic here for the following cases.
    INTRINSIC intrn = INTRINSIC_INVALID;
    if (strcmp(intrnNm, "EXPEXPR") == 0) {
      intrn = INTRN_F8EXPEXPR;
      if (dtype) { *dtype = MTYPE_F8; }
    }
    else if (strcmp(intrnNm, "AMOD") == 0) {
      intrn = INTRN_F4MOD;
      if (dtype) { *dtype = MTYPE_F4; }
    }
    else if (strcmp(intrnNm, "DMOD") == 0) {
      intrn = INTRN_F4MOD;
      if (dtype) { *dtype = MTYPE_F8; }
    }
    else if (strcmp(intrnNm, "LEN") == 0) {
      intrn = INTRN_I4CLEN;
      if (dtype) { *dtype = MTYPE_I4; }
    }
    else { 
      intrn=get_intrinsic_from_name(intrnNm);
    }
    FORTTK_ASSERT(intrn != INTRINSIC_INVALID, 
		  "Unknown intrinsic '" << intrnNm << "'");
    return intrn;
  }
}
