// -*-Mode: C++;-*-
// $Header: /Volumes/cvsrep/developer/OpenADFortTk/src/xaif2whirl/XlateStmt.cxx,v 1.2 2006/05/12 16:12:24 utke Exp $
#include <stdlib.h> // ANSI: cstdlib // for strtol
#include <string.h> // ANSI: cstring // for strcmp, etc.
#include <iostream>

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/dom/DOMElement.hpp>

#include "Open64IRInterface/Open64BasicTypes.h"
#include "Open64IRInterface/Open64IRInterface.hpp"
#include "Open64IRInterface/SymTab.h"
#include "Open64IRInterface/wn_attr.h"
#include "Open64IRInterface/stab_attr.h"

#include <WhirlIDMaps.h>
#include <XAIFStrings.h>
#include "Diagnostics.h"

#include "xaif2whirl.h"
#include "XlateStmt.h"
#include "XlateExpression.h"
#include "XAIF_DOMFilters.h"
#include "XercesStrX.h"


namespace xaif2whirl { 

  WN* XlateStmt::translateStmt(const DOMElement* stmt, 
			       PUXlationContext& ctxt) {
    WN* wn = NULL;
    const XMLCh* name = stmt->getNodeName();
    if (XMLString::equals(name, XAIFStrings.elem_Assign_x())) {
      wn = xlate_Assignment(stmt, ctxt);
    } 
    else if (XMLString::equals(name, XAIFStrings.elem_LpInit_x()) ||
	     XMLString::equals(name, XAIFStrings.elem_LpUpdate_x())) {
      wn = translateAssignmentSimple(stmt, ctxt);
    } 
    else if (XMLString::equals(name, XAIFStrings.elem_SubCall_x())) {
      wn = xlate_SubroutineCall(stmt, ctxt);
    } 
    else if (XMLString::equals(name, XAIFStrings.elem_InlinableSubCall_x())) {
      wn = xlate_InlinableSubroutineCall(stmt, ctxt);
    } 
    else if (XMLString::equals(name, XAIFStrings.elem_Marker_x())) {
      // nothing
    } 
    else if (XMLString::equals(name, XAIFStrings.elem_DerivProp_x())) {
      wn = xlate_DerivativePropagator(stmt, ctxt);
    } 
    else {
      FORTTK_DIE("Unknown XAIF statement:\n" << *stmt);
    }
    return wn;
  }

  WN* XlateStmt::translateAssignmentSimple(const DOMElement* elem, 
					   PUXlationContext& ctxt) {
    DOMElement* lhs_elem = GetChildElement(elem, XAIFStrings.elem_AssignLHS_x());
    DOMElement* rhs_elem = GetChildElement(elem, XAIFStrings.elem_AssignRHS_x());
    DOMElement* lhsref = GetFirstChildElement(lhs_elem);
    std::pair<ST*, WN_OFFSET> lhspair = XlateExpression::translateVarRefSimple(lhsref, ctxt);
    ST* lhsst = lhspair.first;
    WN_OFFSET lhsoset = lhspair.second;
    ctxt.createXlationContext(XlationContext::EXPRSIMPLE);
    WN* rhs = xlate_AssignmentRHS(rhs_elem, ctxt);
    ctxt.deleteXlationContext();
    WN* wn = createAssignment(lhsst, lhsoset, rhs);
    return wn;
  }

  WN* XlateStmt::patchWNStmt(WN* wn, 
			     PUXlationContext& ctxt) {
    WN* patchedWN = NULL;
    OPERATOR opr = WN_operator(wn);
    if (opr == OPR_IO) {
      patchWN_IO(wn, ctxt);
      patchedWN = wn;
    } 
    else if (OPERATOR_is_call(opr)) {
      // FIXME: for now, no need to do anything.  all calls are active
      // and all arguments are variables
    }
    else if (OPERATOR_is_store(opr) && opr!=OPR_PSTORE && opr!=OPR_PSTID) {
      patchedWN = XlateExpression::patchWNExpr(wn, 0 /* kid */, ctxt);
    }
    return patchedWN;
  }

  WN* XlateStmt::xlate_Assignment(const DOMElement* elem, 
				  PUXlationContext& ctxt) {
    DOMElement* lhs_elem = GetChildElement(elem, XAIFStrings.elem_AssignLHS_x());
    DOMElement* rhs_elem = GetChildElement(elem, XAIFStrings.elem_AssignRHS_x());
    ctxt.createXlationContext(XlationContext::NOFLAG);
    WN* lhs = xlate_AssignmentLHS(lhs_elem, ctxt);
    WN* rhs = xlate_AssignmentRHS(rhs_elem, ctxt);
    ctxt.deleteXlationContext();
    // Special case to handle PREGS
    WN* wn = NULL;
    if (WN_operator(lhs) == OPR_LDID && ST_class(WN_st(lhs)) == CLASS_PREG) {
      wn = createAssignment(WN_st(lhs), WN_load_offset(lhs), rhs);
      WN_Delete(lhs); // not recursive
    } else {    
      wn = createAssignment(lhs, rhs);
    }
    return wn;
  }

  WN* XlateStmt::xlate_AssignmentLHS(const DOMElement* elem, 
				     PUXlationContext& ctxt) {
    // VariableReferenceType
    bool deriv = GetDerivAttr(elem);
    DOMElement* varref = GetFirstChildElement(elem);
    ctxt.createXlationContext((deriv) ? XlationContext::DERIVSELECTOR : XlationContext::VALUESELECTOR);
    ctxt.currentXlationContext().setFlag(XlationContext::LVALUE);
    WN* wn = XlateExpression::translateVarRef(varref, ctxt);
    ctxt.deleteXlationContext();
    return wn;
  }

  WN* XlateStmt::xlate_AssignmentRHS(const DOMElement* elem, 
				     PUXlationContext& ctxt) {
    // ExpressionType
    DOMElement* child = GetFirstChildElement(elem);
    ctxt.createXlationContext(XlationContext::NOFLAG);
    WN* wn = XlateExpression::translateExpression(child, ctxt);
    ctxt.deleteXlationContext();
    return wn;
  }

  WN* XlateStmt::xlate_SubroutineCall(const DOMElement* elem, 
				      PUXlationContext& ctxt) {
    // -------------------------------------------------------
    // 1. Gather the arguments, sorted by "position" attribute and
    // translate them into a WHIRL expression tree.
    // -------------------------------------------------------
    unsigned int numArgs = GetIntAttr(elem, XAIFStrings.attr_formalArgCount_x(), 0 /* default */);
    unsigned int numiArgs = 0; // implicit args
    std::vector<std::pair<WN*,fortTkSupport::SymId > > 
      args_wn(numArgs, std::pair<WN*,fortTkSupport::SymId > (NULL,NULL));
    for (DOMElement* arg = GetFirstChildElement(elem); (arg); 
	 arg = GetNextSiblingElement(arg)) { 
      // VariableReferenceType
      const XMLCh* nmX = arg->getNodeName();
      FORTTK_ASSERT(XMLString::equals(nmX, XAIFStrings.elem_Argument_x()), 
		    "Expected " << XAIFStrings.elem_Argument() << "; found:\n"
		    << *arg);
      unsigned int pos = GetPositionAttr(arg); // 1-based
      FORTTK_ASSERT(1 <= pos && pos <= numArgs, 
		    "Unexpected position attribute value:\n" << *arg << " out of range [1," << numArgs << "]");
      fortTkSupport::SymId symId = GetSymId(arg); 
      if (symId) // non-zero for a keyword Argument
	args_wn[pos-1].second=symId;
      // Note: We do *not* check the deriv flag; any active variable
      // references should be passed as is.
      DOMElement* argExpr = GetFirstChildElement(arg);
      // this one can be a VariableReference or a Constant
      // figure out which it is: 
      const XMLCh* nameArgExpr = argExpr->getNodeName();
      if (XMLString::equals(nameArgExpr, XAIFStrings.elem_VarRef_x())) {
	DOMElement* varRef = GetFirstChildElement(argExpr);
	ctxt.createXlationContext(XlationContext::DERIVSELECTOR);
	WN* varRefWN = XlateExpression::translateVarRef(varRef, ctxt);
	ctxt.deleteXlationContext();
	args_wn[pos - 1].first = varRefWN;
	// Determine whether WHIRL needs an implicit argument
	// (cf. WN2F_call() in wn2f_stmt.cxx)
	TY_IDX ty = WN_Tree_Type(varRefWN);
	if (TY_Is_Character_Reference(ty) || TY_Is_Chararray_Reference(ty)) {
	  numiArgs++;
	}
      }
      else if (XMLString::equals(nameArgExpr, XAIFStrings.elem_Constant_x())) {
	ctxt.createXlationContext(XlationContext::DERIVSELECTOR);
	WN* constWN = XlateExpression::translateConstant(argExpr, ctxt);
	ctxt.deleteXlationContext();
	args_wn[pos - 1].first = constWN;
	// Determine whether WHIRL needs an implicit argument
	// (cf. WN2F_call() in wn2f_stmt.cxx)
	TY_IDX ty = WN_Tree_Type(constWN);
	if (TY_Is_Character_Reference(ty) || TY_Is_Chararray_Reference(ty)) {
	  numiArgs++;
	}
      }
      else {
	FORTTK_DIE("Unknown XAIF subroutine call argument:\n" << *argExpr);
      }
    }
    // -------------------------------------------------------
    // 2. Gather WHIRL implicit arguments (e.g., for strings)
    // -------------------------------------------------------
    std::vector<WN*> iargs_wn(numiArgs, NULL);  
    for (unsigned i = 0; i < numiArgs; ++i) {
      // Create bogus values, knowing that we only want to unparse the WHIRL
      iargs_wn[i] = WN_CreateIntconst(OPC_I4INTCONST, 0); // a white lie
    }
    // -------------------------------------------------------
    // 3. Create function call
    // -------------------------------------------------------
    TYPE_ID rtype = MTYPE_V; // void type for subroutine call
    fortTkSupport::Symbol* sym = GetSymbol(elem, ctxt);
    WN* callWN = WN_Call(rtype, MTYPE_V, numArgs + numiArgs, sym->GetST());
    WN_Set_Call_Default_Flags(callWN); // set conservative assumptions
    for (unsigned i = 0; i < numArgs; ++i) {
      if (args_wn[i].first) { 
	// conservatively assume pass by reference
	WN_actual(callWN, i) = CreateParm(args_wn[i].first, WN_PARM_BY_REFERENCE);
	if (args_wn[i].second) { 
	  WN_actual(callWN, i)->u3.ty_fields.ty=args_wn[i].second;
	}
      }
    }
    for (unsigned i = 0, j = numArgs; i < numiArgs; ++i, ++j) {
      WN_actual(callWN, j) = CreateParm(iargs_wn[i], WN_PARM_BY_VALUE);
    }
    return callWN;
  }

  WN* XlateStmt::xlate_InlinableSubroutineCall(const DOMElement* elem, 
					       PUXlationContext& ctxt) {
    // FIXME: abstract with above code
    // -------------------------------------------------------
    // 1. Gather the arguments, sorted by "position" attribute and
    // translate them into a WHIRL expression tree.
    // -------------------------------------------------------
    unsigned int numArgs = GetChildElementCount(elem);
    std::vector<WN*> args_wn(numArgs);
    for (DOMElement* arg = GetFirstChildElement(elem); (arg); 
	 arg = GetNextSiblingElement(arg) ) {
      // VariableReferenceType
      const XMLCh* nmX = arg->getNodeName();
      FORTTK_ASSERT(XMLString::equals(nmX, XAIFStrings.elem_Argument_x()), 
		    "Expected " << XAIFStrings.elem_Argument() 
		    << "; found\n" << *arg);
      unsigned int pos = GetPositionAttr(arg); // 1-based
      FORTTK_ASSERT(1 <= pos, "Unexpected position attribute:\n" << *arg);
      if (pos > args_wn.size()) { args_wn.resize(pos); } // must resize
      // Note: We do *not* check the deriv flag; any active variable
      // references should be passed as is.
      DOMElement* argExpr = GetFirstChildElement(arg);
      // this one can be a VariableReference or a Constant
      // figure out which it is: 
      const XMLCh* nameArgExpr = argExpr->getNodeName();
      if (XMLString::equals(nameArgExpr, XAIFStrings.elem_VarRef_x())) {
	DOMElement* varRef = GetFirstChildElement(argExpr);
	ctxt.createXlationContext(XlationContext::DERIVSELECTOR);
	WN* varRefWN = XlateExpression::translateVarRef(varRef, ctxt);
	ctxt.deleteXlationContext();
	args_wn[pos - 1] = varRefWN;
      }
      else if (XMLString::equals(nameArgExpr, XAIFStrings.elem_Constant_x())) {
	ctxt.createXlationContext(XlationContext::DERIVSELECTOR);
	WN* constWN = XlateExpression::translateConstant(argExpr, ctxt);
	ctxt.deleteXlationContext();
	args_wn[pos - 1] = constWN;
      }
      else {
	FORTTK_DIE("Unknown XAIF subroutine call argument:\n" << *argExpr);
      }
    }
    // -------------------------------------------------------
    // 2. Create block containing OpenAD pragma and call
    // -------------------------------------------------------
    const XMLCh* subnameX = elem->getAttribute(XAIFStrings.attr_subname_x());
    XercesStrX subname = XercesStrX(subnameX);
    // Create OpenAD pragma (locate before creating placeholder nodes!)
    WN* comWN = createOpenADInline(subname.c_str(), args_wn);
    // Create placeholder nodes for arguments not found above
    for (unsigned int i = 0; i < args_wn.size(); ++i) {
      if (!args_wn[i]) { 
	fortTkSupport::Symbol* sym = GetOrCreateBogusTmpSymbol(ctxt);
	ST* st = sym->GetST();
	TYPE_ID rty = ST_mtype(st), dty = ST_mtype(st);
	args_wn[i] = WN_CreateLdid(OPR_LDID, rty, dty, 0, st, ST_type(st), 0);
      }
    }
    // Create call (with placeholder nodes)
    TYPE_ID rtype = MTYPE_V; // void type for subroutine call
    WN* callWN = CreateCallToIntrin(rtype, subname.c_str(), args_wn);
    WN* blkWN = WN_CreateBlock();
    WN_INSERT_BlockFirst(blkWN, comWN);
    WN_INSERT_BlockLast(blkWN, callWN);
    return blkWN;
  }

  WN* XlateStmt::xlate_DerivativePropagator(const DOMElement* elem, 
					    PUXlationContext& ctxt) {
    WN* blckWN = WN_CreateBlock();
    // Accumulate derivative propagator statements and add to block
    XAIF_DerivPropStmt filt;
    for (DOMElement* stmt = GetChildElement(elem, &filt);
	 (stmt); stmt = GetNextSiblingElement(stmt, &filt)) {
      WN* wn = NULL;
      if (XAIF_DerivPropStmt::IsSetDeriv(stmt)) {
	wn = xlate_SetDeriv(stmt, ctxt);
      } 
      else if (XAIF_DerivPropStmt::IsSetNegDeriv(stmt)) {
	wn = xlate_SetNegDeriv(stmt, ctxt);
      } 
      else if (XAIF_DerivPropStmt::IsIncDeriv(stmt)) {
	wn = xlate_IncDeriv(stmt, ctxt);
      } 
      else if (XAIF_DerivPropStmt::IsDecDeriv(stmt)) {
	wn = xlate_DecDeriv(stmt, ctxt);
      } 
      else if (XAIF_DerivPropStmt::IsZeroDeriv(stmt) ) {
	wn = xlate_ZeroDeriv(stmt, ctxt);
      } 
      else if (XAIF_DerivPropStmt::IsSax(stmt)) {
	wn = xlate_Saxpy(stmt, ctxt, false);
      } 
      else if (XAIF_DerivPropStmt::IsSaxpy(stmt) ) {
	wn = xlate_Saxpy(stmt, ctxt, true);
      } 
      else {
	FORTTK_DIE("Unknown XAIF derivative propagator statement:\n" << *stmt);
      }
      WN_INSERT_BlockLast(blckWN, wn);
    }
    // Do not return an empty block
    if (WN_first(blckWN) == NULL)  {
      WN_Delete(blckWN);
      blckWN = NULL;
    }
    return blckWN;
  }

  WN* XlateStmt::xlate_SetDeriv(const DOMElement* elem, 
				PUXlationContext& ctxt) {
    DOMElement* tgt = GetChildElement(elem, XAIFStrings.elem_Tgt_x());
    DOMElement* src = GetChildElement(elem, XAIFStrings.elem_Src_x());
    // Note: This should always be DERIVSELECTOR (FIXME)
    bool deriv = GetDerivAttr(tgt);
    ctxt.createXlationContext((deriv) ? XlationContext::DERIVSELECTOR : XlationContext::VALUESELECTOR);
    WN* tgtWN = XlateExpression::translateVarRef(GetFirstChildElement(tgt), ctxt);
    ctxt.deleteXlationContext();
    // Note: This should always be DERIVSELECTOR (FIXME)
    deriv = GetDerivAttr(src);
    ctxt.createXlationContext((deriv) ? XlationContext::DERIVSELECTOR : XlationContext::VALUESELECTOR);
    WN* srcWN = XlateExpression::translateVarRef(GetFirstChildElement(src), ctxt);
    ctxt.deleteXlationContext();
    WN* callWN = CreateCallToIntrin(MTYPE_V, "setderiv", 2);  
    WN_actual(callWN, 0) = CreateParm(tgtWN, WN_PARM_BY_REFERENCE);
    WN_actual(callWN, 1) = CreateParm(srcWN, WN_PARM_BY_VALUE);
    return callWN;
  }

  WN* XlateStmt::xlate_SetNegDeriv(const DOMElement* elem, 
                                   PUXlationContext& ctxt) {
    DOMElement* tgt = GetChildElement(elem, XAIFStrings.elem_Tgt_x());
    DOMElement* src = GetChildElement(elem, XAIFStrings.elem_Src_x());
    // Note: This should always be DERIVSELECTOR (FIXME)
    bool deriv = GetDerivAttr(tgt);
    ctxt.createXlationContext((deriv) ? XlationContext::DERIVSELECTOR : XlationContext::VALUESELECTOR);
    WN* tgtWN = XlateExpression::translateVarRef(GetFirstChildElement(tgt), ctxt);
    ctxt.deleteXlationContext();
    // Note: This should always be DERIVSELECTOR (FIXME)
    deriv = GetDerivAttr(src);
    ctxt.createXlationContext((deriv) ? XlationContext::DERIVSELECTOR : XlationContext::VALUESELECTOR);
    WN* srcWN = XlateExpression::translateVarRef(GetFirstChildElement(src), ctxt);
    ctxt.deleteXlationContext();
    WN* callWN = CreateCallToIntrin(MTYPE_V, "set_neg_deriv", 2);  
    WN_actual(callWN, 0) = CreateParm(tgtWN, WN_PARM_BY_REFERENCE);
    WN_actual(callWN, 1) = CreateParm(srcWN, WN_PARM_BY_VALUE);
    return callWN;
  }

  WN* XlateStmt::xlate_IncDeriv(const DOMElement* elem, 
                                PUXlationContext& ctxt) {
    DOMElement* tgt = GetChildElement(elem, XAIFStrings.elem_Tgt_x());
    DOMElement* src = GetChildElement(elem, XAIFStrings.elem_Src_x());
    // Note: This should always be DERIVSELECTOR (FIXME)
    bool deriv = GetDerivAttr(tgt);
    ctxt.createXlationContext((deriv) ? XlationContext::DERIVSELECTOR : XlationContext::VALUESELECTOR);
    WN* tgtWN = XlateExpression::translateVarRef(GetFirstChildElement(tgt), ctxt);
    ctxt.deleteXlationContext();
    // Note: This should always be DERIVSELECTOR (FIXME)
    deriv = GetDerivAttr(src);
    ctxt.createXlationContext((deriv) ? XlationContext::DERIVSELECTOR : XlationContext::VALUESELECTOR);
    WN* srcWN = XlateExpression::translateVarRef(GetFirstChildElement(src), ctxt);
    ctxt.deleteXlationContext();
    WN* callWN = CreateCallToIntrin(MTYPE_V, "inc_deriv", 2);  
    WN_actual(callWN, 0) = CreateParm(tgtWN, WN_PARM_BY_REFERENCE);
    WN_actual(callWN, 1) = CreateParm(srcWN, WN_PARM_BY_VALUE);
    return callWN;
  }

  WN* XlateStmt::xlate_DecDeriv(const DOMElement* elem, 
                                PUXlationContext& ctxt) {
    DOMElement* tgt = GetChildElement(elem, XAIFStrings.elem_Tgt_x());
    DOMElement* src = GetChildElement(elem, XAIFStrings.elem_Src_x());
    // Note: This should always be DERIVSELECTOR (FIXME)
    bool deriv = GetDerivAttr(tgt);
    ctxt.createXlationContext((deriv) ? XlationContext::DERIVSELECTOR : XlationContext::VALUESELECTOR);
    WN* tgtWN = XlateExpression::translateVarRef(GetFirstChildElement(tgt), ctxt);
    ctxt.deleteXlationContext();
    // Note: This should always be DERIVSELECTOR (FIXME)
    deriv = GetDerivAttr(src);
    ctxt.createXlationContext((deriv) ? XlationContext::DERIVSELECTOR : XlationContext::VALUESELECTOR);
    WN* srcWN = XlateExpression::translateVarRef(GetFirstChildElement(src), ctxt);
    ctxt.deleteXlationContext();
    WN* callWN = CreateCallToIntrin(MTYPE_V, "dec_deriv", 2);  
    WN_actual(callWN, 0) = CreateParm(tgtWN, WN_PARM_BY_REFERENCE);
    WN_actual(callWN, 1) = CreateParm(srcWN, WN_PARM_BY_VALUE);
    return callWN;
  }

  WN* XlateStmt::xlate_ZeroDeriv(const DOMElement* elem, 
				 PUXlationContext& ctxt) {
    WN* x = xlate_AssignmentLHS(elem, ctxt); // functionally equivalent
    WN* callWN = CreateCallToIntrin(MTYPE_V, "zero_deriv", 1);
    WN_actual(callWN, 0) = CreateParm(x, WN_PARM_BY_REFERENCE);
    return callWN;
  }

  WN* XlateStmt::xlate_Saxpy(const DOMElement* elem, 
			     PUXlationContext& ctxt, 
			     bool saxpy) {
    // count the children
    int elemCount=GetChildElementCount(elem);
    // create a WHIRL call 
    const char* fnm = (saxpy) ? "saxpy" : "sax";
    MTYPE rtype = MTYPE_V;
    // one child element for 'y'; all other child elements are pairs of 'a' and 'x'
    WN* callWN = CreateCallToIntrin(rtype, fnm, 2*(elemCount-1)+1);
    int parameterPosition=0;
    // get the a/x pairs
    DOMElement* aChildElem(GetFirstChildElement(elem));
    while (aChildElem) { 
      if (XAIF_ElemFilter(XAIFStrings.elem_AX_x()).acceptNode(aChildElem) == DOMNodeFilter::FILTER_ACCEPT) { 
	DOMElement* theA = GetChildElement(aChildElem, XAIFStrings.elem_A_x());
	DOMElement* theX = GetChildElement(aChildElem, XAIFStrings.elem_X_x());
	// A
	WN* a_wn = XlateExpression::translateExpression(GetFirstChildElement(theA), ctxt);
	// X
	bool deriv = GetDerivAttr(theX);
	ctxt.createXlationContext((deriv) ? XlationContext::DERIVSELECTOR : XlationContext::VALUESELECTOR);
	WN* x_wn = XlateExpression::translateVarRef(GetFirstChildElement(theX), ctxt);
	ctxt.deleteXlationContext();
	// add the parameters
	WN_actual(callWN, parameterPosition++) = CreateParm(a_wn, WN_PARM_BY_VALUE);
	WN_actual(callWN, parameterPosition++) = CreateParm(x_wn, WN_PARM_BY_VALUE);
      }
      aChildElem=GetNextSiblingElement(aChildElem);
    }
    // Y
    DOMElement* Y = GetChildElement(elem, XAIFStrings.elem_Y_x());
    bool deriv = GetDerivAttr(Y);
    ctxt.createXlationContext((deriv) ? XlationContext::DERIVSELECTOR : XlationContext::VALUESELECTOR);
    WN* y_wn = XlateExpression::translateVarRef(GetFirstChildElement(Y), ctxt);
    ctxt.deleteXlationContext();
    WN_actual(callWN, parameterPosition) = CreateParm(y_wn, WN_PARM_BY_REFERENCE);
    return callWN;
  }

  void XlateStmt::patchWN_IO(WN* wn, 
			     PUXlationContext& ctxt) {  
    // FIXME: only handle cray read/write for now
    patchWN_IO_cray(wn, ctxt);
  }

  void XlateStmt::patchWN_IO_cray(WN* wn, 
				  PUXlationContext& ctxt) {
    IOSTATEMENT iostmt = WN_io_statement(wn);
    FORTTK_ASSERT_WARN(iostmt == IOS_CR_FWF || iostmt == IOS_CR_FWU 
		       || iostmt == IOS_CR_FRF || iostmt == IOS_CR_FRU
		       || iostmt == IOS_CR_OPEN || iostmt == IOS_CR_CLOSE
		       || iostmt == IOS_INQUIRE || iostmt == IOS_CR_INQUIRE
		       || iostmt == IOS_REWIND  || iostmt == IOS_CR_REWIND,
		       fortTkSupport::Diagnostics::UnexpectedInput << IOSTATEMENT_name(iostmt));
    // Iterate over IO_ITEMs and translate IOLs (io lists)
    for (INT kidno = 0; kidno < WN_kid_count(wn); ++kidno) {
      WN* kid = WN_kid(wn, kidno);
      if (IS_IO_ITEM_IOL(kid)) {
	patchWN_IO_ITEM_list(kid, ctxt);
      }
    }
  }

  void XlateStmt::patchWN_IO_ITEM_list(WN* wn, 
				       PUXlationContext& ctxt) {
    // cf. xlate_IO_ITEM_list
    IOITEM kind = WN_io_item(wn);
    switch (kind) {
    case IOL_VAR:
    case IOL_ARRAY:
    case IOL_RECORD:
      XlateExpression::patchWNExpr(wn, 0 /* kid */, ctxt);
      break;
    case IOL_CHAR_ARRAY:
    case IOL_CHAR:       // skip
      break;
    case IOL_EXPR: // patch
      XlateExpression::patchWNExpr(wn, 0 /* kid */, ctxt);
      break;
    case IOL_IMPLIED_DO: // patch
    case IOL_IMPLIED_DO_1TRIP:
      XlateExpression::patchWNExpr(wn, 2 /* kid */, ctxt);
      for (int kid = 4; kid < WN_kid_count(wn); ++kid) {
	patchWN_IO_ITEM_list(WN_kid(wn, kid), ctxt);
      }
      break;
    case IOL_LOGICAL: // skip
    case IOL_DOPE:    // skip
      break;
    default:
      FORTTK_DIE(fortTkSupport::Diagnostics::UnexpectedOpr << IOITEM_name(kind));
      break;
    }
  }

  WN* XlateStmt::createAssignment(WN* lhs, 
				  WN* rhs) {
    // We always use ISTORE (instead of e.g. STID) for generality.  This
    // will not be an issue because the intension is for this WHIRL to
    // be translated to source code.
    TY_IDX ty = WN_Tree_Type(lhs); // should be a pointer type
    TY_IDX rhs_ty = WN_Tree_Type(rhs);
    if (TY_is_f90_pointer(rhs_ty) || TY_Is_Pointer(rhs_ty)) 
      rhs_ty=TY_pointed(rhs_ty);
    if (TY_Is_Array(rhs_ty) || TY_Is_Pointer(rhs_ty)) 
      rhs_ty=TY_etype(rhs_ty);
    TYPE_ID dtype = TY_mtype(rhs_ty);
    if (dtype == MTYPE_STR) {
      dtype = MTYPE_U1;
    }
    WN* wn = WN_Istore(dtype, 0, ty, lhs, rhs, 0);
    return wn;
  }

  WN* XlateStmt::createAssignment(ST* lhs, 
				  WN_OFFSET oset, 
				  WN* rhs) {
    // A special version of the above for situations in which WHIRL
    // requires STID assignments (e.g. loop initialization and updates).
    TY_IDX ty = WN_Tree_Type(rhs); // referenced-obj = base-obj
    WN* wn = WN_Stid(TY_mtype(ty), oset, lhs, ty, rhs, 0);
    return wn;
  }

  WN* XlateStmt::createZeroConst(TYPE_ID ty) {
    return Make_Const(Targ_Conv(ty, Host_To_Targ(MTYPE_I4, 0)));
  }

  WN* XlateStmt::createOpenADInline(const char* fname, 
				    std::vector<WN*>& args) {
    // $OpenAD$ INLINE subname(argpos1, argpos2..)
    std::string com = "$OpenAD$ INLINE ";
    com.reserve(128);
    com += fname;
    com += "(";
    for (unsigned int i = 0; i < args.size(); ++i) {
      const char* argdesc = (args[i]) ? "subst" : "nosubst";
      com += argdesc;
      if (i < (args.size() - 1)) { com += ","; }
    }
    com += ")";
    WN* comWN = WN_CreateComment((char*)com.c_str());
    return comWN;
  }
}
