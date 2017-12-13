// -*-Mode: C++;-*-
// $Header: /Volumes/cvsrep/developer/OpenADFortTk/src/xaif2whirl/XlateStmt.h,v 1.2 2006/05/12 16:12:24 utke Exp $

#ifndef XlateStmt_INCLUDED_h
#define XlateStmt_INCLUDED_h

#include <iostream>

#include "xercesc/dom/DOMDocument.hpp"

#include "Open64IRInterface/Open64BasicTypes.h"

#include "PUXlationContext.h"

namespace xaif2whirl {

  class XlateStmt { 

  public: 

    static WN* translateStmt(const DOMElement* stmt, 
			     PUXlationContext& ctxt); 

    /** 
     * TranslateAssignmentSimple: Translates certain XAIF constructs into
     * special WHIRL assignment statements in order to conform to WHIRL
     * requirements.
     */
    static WN* translateAssignmentSimple(const DOMElement* stmt, 
					 PUXlationContext& ctxt);

    static WN* patchWNStmt(WN* wn, PUXlationContext& ctxt);

  private: 

    static WN* xlate_Assignment(const DOMElement* elem, 
				PUXlationContext& ctxt);

    static WN* xlate_AssignmentLHS(const DOMElement* elem, 
				   PUXlationContext& ctxt);

    static WN* xlate_AssignmentRHS(const DOMElement* elem, 
				   PUXlationContext& ctxt);

    static WN* xlate_SubroutineCall(const DOMElement* elem, 
				    PUXlationContext& ctxt);

    static WN* xlate_InlinableSubroutineCall(const DOMElement* elem, 
					     PUXlationContext& ctxt);

    /** 
     *  Must not return an empty block
     */
    static WN* xlate_DerivativePropagator(const DOMElement* elem, 
					  PUXlationContext& ctxt);

    static WN* xlate_SetDeriv(const DOMElement* elem, 
			      PUXlationContext& ctxt);

    static WN* xlate_SetNegDeriv(const DOMElement* elem,
                                 PUXlationContext& ctxt);

    static WN* xlate_IncDeriv(const DOMElement* elem,
                              PUXlationContext& ctxt);

    static WN* xlate_DecDeriv(const DOMElement* elem,
                              PUXlationContext& ctxt);

    static WN* xlate_ZeroDeriv(const DOMElement* elem, 
			       PUXlationContext& ctxt);
    /** 
     * xlate_Saxpy: handles calls to both
     *   saxpy(a,x,y): Y=A1*X1+A2*X2+...+Y
     *   sax(a,x,y)  : Y=A1*X1+A2*X2+...
     */
    static WN* xlate_Saxpy(const DOMElement* elem,
			   PUXlationContext& ctxt, 
			   bool saxpy);

    static void patchWN_IO(WN* wn, 
			   PUXlationContext& ctxt);

    static void patchWN_IO_cray(WN* wn, 
				PUXlationContext& ctxt);

    static void patchWN_IO_ITEM_list(WN* wn, 
				     PUXlationContext& ctxt);

    static WN* createAssignment(WN* lhs, WN* rhs);

    static WN* createAssignment(ST* lhs, WN_OFFSET oset, WN* rhs);

    static WN* createZeroConst(TYPE_ID ty);

    static WN* createOpenADInline(const char* fname, std::vector<WN*>& args);

  }; 
}

#endif 
