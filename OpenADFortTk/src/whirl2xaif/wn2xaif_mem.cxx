// -*-Mode: C++;-*-
// $Header: /Volumes/cvsrep/developer/OpenADFortTk/src/whirl2xaif/wn2xaif_mem.cxx,v 1.39 2006/05/12 16:12:23 utke Exp $


#include <sstream> //FIXME
#include <cassert> //FIXME
#include <strings.h>


#include "Open64IRInterface/Open64BasicTypes.h"
#include "pf_cg.h"


#include "wn2xaif.h"
#include "wn2xaif_mem.h"
#include "st2xaif.h"
#include "ty2xaif.h"

namespace whirl2xaif { 


  // ************************** Forward Declarations ***************************

  static void
  DumpVarRefEdge(xml::ostream& xos, UINT eid, UINT srcid, UINT targid);

  static void 
  WN2F_Block(xml::ostream& xos, ST * st, STAB_OFFSET off, PUXlationContext& ctxt);

  static WN *WN2F_ZeroInt_Ptr = NULL;
  static WN *WN2F_OneInt_Ptr = NULL;

#define WN2F_INTCONST_ZERO						\
  (WN2F_ZeroInt_Ptr == NULL? WN2F_ZeroInt_Ptr = WN2F_Initiate_ZeroInt() \
   : WN2F_ZeroInt_Ptr)
#define WN2F_INTCONST_ONE						\
  (WN2F_OneInt_Ptr == NULL? WN2F_OneInt_Ptr = WN2F_Initiate_OneInt()	\
   : WN2F_OneInt_Ptr)

  static void 
  WN2F_Arrsection_Slots(xml::ostream& xos, WN* wn, PUXlationContext& ctxt);

  static void 
  xlate_ArrayIndices(xml::ostream& xos, WN* wn, PUXlationContext& ctxt);

  // ***************************************************************************

  /*------------------------- Utility Functions ------------------------*/
  /*--------------------------------------------------------------------*/

  static WN *
  WN2F_Initiate_ZeroInt(void)
  {
    static char ZeroInt [sizeof (WN)];
    WN       *wn = (WN*) &ZeroInt;
    OPCODE    opcode = OPCODE_make_op(OPR_INTCONST, MTYPE_I4, MTYPE_V);
  
    bzero(wn, sizeof(WN));
    WN_set_opcode(wn, opcode);
    WN_set_kid_count(wn, 0);
    WN_map_id(wn) =  WN_MAP_UNDEFINED;
    WN_const_val(wn) = 0LL;
    return wn;
  } /* WN2F_Initiate_ZeroInt */

  static WN *
  WN2F_Initiate_OneInt(void)
  {
    static char OneInt [sizeof (WN)];
    WN       *wn = (WN*) &OneInt;
    OPCODE    opcode = OPCODE_make_op(OPR_INTCONST, MTYPE_I4, MTYPE_V);
  
    bzero(wn, sizeof(WN));
    WN_set_opcode(wn, opcode);
    WN_set_kid_count(wn, 0);
    WN_map_id(wn) =  WN_MAP_UNDEFINED;
    WN_const_val(wn) = 1LL;
    return wn;
  } /* WN2F_Initiate_ZeroInt */


    /** 
     * translating string slices
     *  \warning{The whirl format for substrings has in 
     *  the first child the substring size 
     *  and in the second the offset from the base. 
     *  We could make proper lower and upper bound 
     *  expression from this format for xaif. 
     *  Because we need to backtranslate to whirl 
     *  we would then have to undo this - 
     *  so instead we just dump the current expression 
     *  into the index/bound positions and leave 
     *  them as they are even though they look misleading in xaif.}
    */
  static void
  WN2F_Substring(xml::ostream& xos, 
		 INT64        string_size,
		 WN          *lower_bnd,
		 WN          *substring_size,
		 PUXlationContext& ctxt) {
    if (WN_operator(lower_bnd) == OPR_INTCONST   
	&&
	WN_const_val(lower_bnd) == 0                    
	&&
	WN_operator(substring_size) == OPR_INTCONST 
	&&
	WN_const_val(substring_size) == string_size) {
	FORTTK_DIE("should not invoke this");
    }
    xos << xml::BegElem(XAIFStrings.elem_ArrayElemRef())
	<< xml::Attr("vertex_id", ctxt.currentXlationContext().getNewVertexId());
    xos << xml::BegElem(XAIFStrings.elem_IndexTriplet());
    xos << xml::BegElem(XAIFStrings.elem_Index());
    ctxt.createXlationContext(); 
    ctxt.currentXlationContext().unsetFlag(XlationContext::VARREF); // elem_Index() contains ExpressionType
    TranslateWN(xos, lower_bnd, ctxt);
    ctxt.deleteXlationContext();
    xos << xml::EndElem;
    xos << xml::BegElem(XAIFStrings.elem_Bound());
    ctxt.createXlationContext(); 
    ctxt.currentXlationContext().unsetFlag(XlationContext::VARREF); // elem_Index() contains ExpressionType
    TranslateWN(xos, substring_size, ctxt);
    ctxt.deleteXlationContext();
    xos << xml::EndElem;
    xos << xml::EndElem;
    xos << xml::EndElem;
  }


  static void
  WN2F_Get_Substring_Info(WN **base,        /* Possibly OPR_ARRAY node (in/out)*/
			  TY_IDX *string_ty,/* The string type (out) */
			  WN **lower_bnd,   /* The lower bound index (out) */
			  WN **length )
  {
    /* There are two possibilities concerning the array base expressions.
     * It can be a pointer to a complete character-string (array) or it
     * can be a pointer to a character within a character-string (single
     * character).  In the first instance, the offset off the base of 
     * string is zero.  In the latter case, the offset is given by the
     * array indexing operation.
     */
    TY_IDX ptr_ty = WN_Tree_Type(*base);

    // the WN_Tree_Type logic has been changed to 
    // return the actual result type rather than a 
    // low level compiler internal "void pointer" 
    // therefore this may not actually be a pointer type. 
    if ((TY_kind(ptr_ty) == KIND_POINTER) &&
	(TY_kind(TY_pointed(ptr_ty)) != KIND_FUNCTION))
      *string_ty = TY_pointed(ptr_ty);
    else
      *string_ty=ptr_ty;

    if (TY_size(*string_ty) == 1 && !TY_Is_Array(*string_ty)
	&& WN_operator(*base) == OPR_ARRAY) {
      /* Let the base of the string be denoted as the base of the array
       * expression.
       */
      *string_ty = TY_pointed(WN_Tree_Type(WN_kid0(*base)));
      *lower_bnd = WN_array_index(*base, 0);
      *length    = WN_kid1(*base);
      *base = WN_kid0(*base);
    }
    else if (WN_operator(*base) == OPR_ARRAY &&
	     TY_Is_Array(*string_ty)             &&
	     TY_AR_ndims(*string_ty) == 1        &&
	     TY_Is_Character_String(*string_ty)  &&
	     !TY_ptr_as_array(Ty_Table[ptr_ty])) {
      /* Presumably, the lower bound is given by the array operator */
      *lower_bnd = WN_array_index(*base, 0);
      *length    = WN_kid1(*base);
      *base = WN_kid0(*base);
    }
    else {
      *lower_bnd = WN2F_INTCONST_ZERO;
      *length    = WN2F_INTCONST_ZERO;
    }
  } /* WN2F_Get_Substring_Info */

  static WN *
  WN2F_Find_Base(WN *addr)
  {
    /* utility to find base of address tree */
    WN *res = addr;

    switch (WN_operator(addr)) {
    case OPR_ARRAY: 
    case OPR_ILOAD:
      res=WN_kid0(addr);
      break;

    case OPR_ADD:
      if (WN_operator(WN_kid0(addr)) == OPR_INTCONST)
	res = WN2F_Find_Base(WN_kid1(addr));
      else
	res = WN2F_Find_Base(WN_kid0(addr));
      break;

    default:
      res = addr;
      break;
    }
    return res;
  }

  BOOL
  WN2F_Is_Address_Preg(WN * ad ,TY_IDX ptr_ty)
  {
    /* Does this look like a preg or variable being used as an address ? */
    /* These are propagated by opt/pfa                                   */

    BOOL is_somewhat_address_like = TY_kind(ptr_ty) == KIND_POINTER;
  
    if (TY_kind(ptr_ty) == KIND_SCALAR) {
      TYPE_ID tid = TY_mtype(ptr_ty);
      is_somewhat_address_like |= (MTYPE_is_pointer(tid)) || (tid == MTYPE_I8) || (tid == MTYPE_I4) ;
    }

    if (is_somewhat_address_like) {
      WN * wn = WN2F_Find_Base(ad);
      if (WN_operator(wn) == OPR_LDID) {
	ST * st = WN_st(wn) ;
	if (ST_class(st) == CLASS_PREG)
	  return TRUE ;
      
	if (ST_class(st) == CLASS_VAR) {
	  if (TY_kind(ptr_ty) == KIND_SCALAR)
	    return TRUE;
	
	  if (TY_kind(WN_ty(wn)) == KIND_SCALAR) {
	    TYPE_ID wtid = TY_mtype(WN_ty(wn));
	  
	    /* Looks like a Cray pointer (I4/I8) ? */
	    if ((wtid == MTYPE_I8)|| (wtid == MTYPE_I4))
	      if (ad != wn)
		return TRUE ;
	  
	    /* Looks like a VAR with a U4/U8? used  */
	    /* only with offsets, or FORMALs would  */
	    /* qualify, if intrinsic mtype          */
	    if (MTYPE_is_pointer(wtid))
	      if (TY_kind(ST_type(st)) != KIND_SCALAR)
		return TRUE;
	  }
	}
      }
    }
    return FALSE;
  }


  // ***************************************************************************
  // Loads (In WHIRL, loads are expressions.)
  // ***************************************************************************

  void 
  xlate_LDA(xml::ostream& xos, WN* wn, PUXlationContext& ctxt)
  {
    FORTTK_ASSERT(WN_operator(wn) == OPR_LDA, fortTkSupport::Diagnostics::UnexpectedInput);
    FORTTK_ASSERT(ST_class(WN_st(wn)) != CLASS_PREG, "Cannot LDA a PREG");
  
    // Base and referenced (some offset, possibly 0, from base) objects
    ST* base_st = WN_st(wn); // symbol for base object
    TY_IDX base_ty = WN_GetBaseObjType(wn);
    TY_IDX baseptr_ty = Stab_Pointer_To(base_ty);
    TY_IDX ref_ty = WN_GetRefObjType(wn); // a pointer type
  
    // Implicit dereference (Note: sometimes we need to deal with buggy
    // WHIRL code, where ref_ty is not a pointer type.  In this case we
    // guess a type.
    ref_ty = (TY_Is_Pointer(ref_ty)) ? TY_pointed(ref_ty) : base_ty;
  
    ctxt.currentXlationContext().setWN(wn);
    ctxt.currentXlationContext().unsetFlag(XlationContext::DEREF_ADDR);
    ctxt.currentXlationContext().setFlag(XlationContext::HAS_NO_ARR_ELMT);
    xlate_SymRef(xos, base_st, baseptr_ty, ref_ty, WN_lda_offset(wn), ctxt);
    ctxt.currentXlationContext().unsetFlag(XlationContext::HAS_NO_ARR_ELMT);
  
    
  }


  void 
  xlate_LDID(xml::ostream& xos, WN* wn, PUXlationContext& ctxt)
  {
    FORTTK_ASSERT(WN_operator(wn) == OPR_LDID, fortTkSupport::Diagnostics::UnexpectedInput);

    // Base and referenced (some offset, possibly 0, from base) objects
    TY_IDX base_ty = WN_GetBaseObjType(wn);
    TY_IDX baseptr_ty;
    TY_IDX ref_ty = WN_GetRefObjType(wn);
  
    if (ST_class(WN_st(wn)) == CLASS_PREG) {
      // Note: WN_load_offset() is the PREG_IDX
      ctxt.currentXlationContext().setWN(wn);
      xlate_PregRef(xos, WN_st(wn), base_ty, WN_load_offset(wn), ctxt);
    } 
    else {

      // FIXME: Stab_Pointer_To, et. al. create types!!!
      if (ctxt.currentXlationContext().isFlag(XlationContext::DEREF_ADDR) && TY_Is_Pointer(base_ty)) {
	// Expect the loaded type to be a pointer to the type of object
	// to be dereferenced.  The only place (besides declaration sites)
	// where we expect to specially handle ptr_as_array objects.
	if (TY_ptr_as_array(Ty_Table[ref_ty])) {
	  ref_ty = Stab_Array_Of(TY_pointed(ref_ty), 0/*size*/);
	} else {
	  ref_ty = TY_pointed(ref_ty);
	}
      
	// There are two possibilities for the base type: A regular 
	// pointer or a pointer to be treated as a pointer to an array.
	// In either case, 'baseptr_ty' is a pointer to the 
	// derefenced base type. 
	//
	// Note that this does not handle a pointer to a struct to be
	// treated as an array of structs, where the object type and
	// offset denote a member of the struct, since xlate_SymRef() 
	// cannot access a struct member through an array access.
	if (TY_ptr_as_array(Ty_Table[base_ty])) {
	  base_ty = Stab_Array_Of(TY_pointed(base_ty), 0/*size*/);
	} else {
	  base_ty = TY_pointed(base_ty); // baseptr_ty = base_ty;
	}
      } 
      baseptr_ty = Stab_Pointer_To(base_ty);
    
      ctxt.currentXlationContext().setWN(wn);
      ctxt.currentXlationContext().setFlag(XlationContext::HAS_NO_ARR_ELMT); // FIXME why?
      xlate_SymRef(xos, WN_st(wn), baseptr_ty, ref_ty, WN_load_offset(wn), ctxt);
      ctxt.currentXlationContext().unsetFlag(XlationContext::HAS_NO_ARR_ELMT);
    }

    
  } 


  void 
  xlate_ILOAD(xml::ostream& xos, WN* wn, PUXlationContext& ctxt)
  {
    // Note that we handle this just like we do the lhs of an ISTORE.
    FORTTK_ASSERT(WN_operator(wn) == OPR_ILOAD, fortTkSupport::Diagnostics::UnexpectedInput);
  
    // Base and referenced (some offset, possibly 0, from base) objects
    WN* baseptr = WN_kid0(wn); // address expression as WN
    TY_IDX base_ty = WN_GetBaseObjType(wn);
    TY_IDX baseptr_ty = Stab_Pointer_To(base_ty);
    TY_IDX ref_ty = WN_GetRefObjType(wn);
  
    // Translate into a reference
    ctxt.currentXlationContext().setWN(wn);
    if (WN_operator(baseptr) == OPR_LDA || WN_operator(baseptr) == OPR_LDID) {
      ctxt.currentXlationContext().setFlag(XlationContext::HAS_NO_ARR_ELMT); // FIXME
    }
  
    xlate_MemRef(xos, baseptr, baseptr_ty, ref_ty, WN_load_offset(wn), ctxt);
  
    ctxt.currentXlationContext().unsetFlag(XlationContext::HAS_NO_ARR_ELMT);

    
  }

  void 
  xlate_ILOADX(xml::ostream& xos, WN* wn, PUXlationContext& ctxt)
  {
    FORTTK_DIE(fortTkSupport::Diagnostics::Unimplemented);
    xos << OPCODE_name(WN_opcode(wn));
    
  }


  void 
  WN2F_mload(xml::ostream& xos, WN* wn, PUXlationContext& ctxt)
  {
    // This should only appear the as the rhs of an ISTORE.  Treat
    // it just like an ILOAD.
    FORTTK_ASSERT(WN_operator(wn) == OPR_MLOAD, fortTkSupport::Diagnostics::UnexpectedInput);

    // FIXME:
    
    /* Get the type of the base from which we are loading */
    TY_IDX base_ty = WN_Tree_Type(WN_kid0(wn));
    if (!TY_Is_Pointer(base_ty))
      base_ty = WN_ty(wn);
  
    /* Get the object to be loaded */
    xlate_MemRef(xos, WN_kid0(wn), /* base-symbol */
		 base_ty, /* base-type */
		 TY_pointed(WN_ty(wn)), /* object-type */
		 WN_load_offset(wn), /* object-ofst */ ctxt);

    
  }


  // ***************************************************************************
  // Stores (In WHIRL, stores are statements.)
  // ***************************************************************************

  // xlate_STID: Translate a WHIRL STID node to an XAIF assignment
  void 
  xlate_STID(xml::ostream& xos, WN* wn, PUXlationContext& ctxt)
  {
    FORTTK_ASSERT(WN_operator(wn) == OPR_STID, fortTkSupport::Diagnostics::UnexpectedInput);

    // LHS base and referenced (some offset, possibly 0, from base) objects
    ST* base_st = WN_st(wn); // symbol for base object
    TY_IDX base_ty = WN_GetBaseObjType(wn);
    TY_IDX baseptr_ty = Stab_Pointer_To(base_ty);
    TY_IDX ref_ty = WN_GetRefObjType(wn);
  
    // Assignment
    if (!ctxt.currentXlationContext().isFlag(XlationContext::ASSIGN)) {
      USRCPOS srcpos;
      USRCPOS_srcpos(srcpos) = WN_Get_Linenum(wn);
      xos << xml::BegElem(XAIFStrings.elem_Assign())
	  << xml::Attr("statement_id", ctxt.findWNId(wn))
	  << xml::Attr("lineNumber",USRCPOS_linenum(srcpos))	
	  << xml::Attr("do_chain", ctxt.findDoChainId(wn));
    }
  
    // LHS of assignment
    WN* lhs = wn; // OPR_STID represents the LHS of the assignment
    xos << xml::BegElem(XAIFStrings.elem_AssignLHS()) 
	<< xml::Attr("du_ud", ctxt.findUDDUChainId(lhs))
	<< xml::Attr("alias", ctxt.getAliasMapKey(lhs))
	<< xml::EndAttrs;
    ctxt.createXlationContext(XlationContext::VARREF, wn); // implicit for LHS
  
    if (ST_class(base_st) == CLASS_PREG) { // FIXME
      // Note: WN_load_offset() is the PREG_IDX
      xlate_PregRef(xos, base_st, base_ty, WN_store_offset(wn), ctxt);
    } 
    else {
      xlate_SymRef(xos, base_st, baseptr_ty, ref_ty, WN_store_offset(wn), ctxt);
    }
  
    ctxt.deleteXlationContext();
    xos << xml::EndElem;
  
    // RHS of assignment
    BOOL logical = TY_is_logical(Ty_Table[ref_ty]); // FIXME
    xos << xml::BegElem(XAIFStrings.elem_AssignRHS()) << xml::EndAttrs;
    ctxt.createXlationContext(XlationContext::NOFLAG, wn);
    if (logical) { 
      ctxt.currentXlationContext().setFlag(XlationContext::HAS_LOGICAL_ARG); 
    } // FIXME
    TranslateWN(xos, WN_kid0(wn), ctxt);
    if (logical) { 
      ctxt.currentXlationContext().unsetFlag(XlationContext::HAS_LOGICAL_ARG); 
    } // FIXME
    ctxt.deleteXlationContext();
    xos << xml::EndElem;

    if (!ctxt.currentXlationContext().isFlag(XlationContext::ASSIGN)) {
      xos << xml::EndElem /* elem_Assign() */;
    }
  
    
  }

  void 
  xlate_ISTORE(xml::ostream& xos, WN* wn, PUXlationContext& ctxt)
  {
    FORTTK_ASSERT(WN_operator(wn) == OPR_ISTORE, fortTkSupport::Diagnostics::UnexpectedInput);

    // LHS base and referenced (some offset, possibly 0, from base) objects
    WN* baseptr = WN_kid1(wn); // address expression as WN
    TY_IDX base_ty = WN_GetBaseObjType(wn);
    TY_IDX baseptr_ty = Stab_Pointer_To(base_ty);
    TY_IDX ref_ty = WN_GetRefObjType(wn);
  
    // Assignment
    if (!ctxt.currentXlationContext().isFlag(XlationContext::ASSIGN)) {
      USRCPOS srcpos;
      USRCPOS_srcpos(srcpos) = WN_Get_Linenum(wn);
      xos << xml::BegElem(XAIFStrings.elem_Assign())
	  << xml::Attr("statement_id", ctxt.findWNId(wn))
	  << xml::Attr("lineNumber",USRCPOS_linenum(srcpos))	
	  << xml::Attr("do_chain", ctxt.findDoChainId(wn));
    }
  
    // LHS of assignment (dereference address)
    WN* lhs = baseptr;
    xos << xml::BegElem(XAIFStrings.elem_AssignLHS()) 
	<< xml::Attr("du_ud", ctxt.findUDDUChainId(lhs))
	<< xml::Attr("alias", ctxt.getAliasMapKey(lhs))
	<< xml::EndAttrs;
    ctxt.createXlationContext(XlationContext::VARREF, wn); // implicit for LHS
  
    if (WN_operator(baseptr) == OPR_LDA || WN_operator(baseptr) == OPR_LDID) {
      ctxt.currentXlationContext().setFlag(XlationContext::HAS_NO_ARR_ELMT);
    }
    xlate_MemRef(xos, baseptr, baseptr_ty, ref_ty, WN_store_offset(wn), ctxt);
    ctxt.currentXlationContext().unsetFlag(XlationContext::HAS_NO_ARR_ELMT); 

    ctxt.deleteXlationContext();
    xos << xml::EndElem;

    // RHS of assignment
    xos << xml::BegElem(XAIFStrings.elem_AssignRHS()) << xml::EndAttrs;
    ctxt.createXlationContext(XlationContext::NOFLAG, wn);
    TranslateWN(xos, WN_kid0(wn), ctxt);
    ctxt.deleteXlationContext();
    xos << xml::EndElem;

    if (!ctxt.currentXlationContext().isFlag(XlationContext::ASSIGN)) {
      xos << xml::EndElem /* elem_Assign() */;
    }
  
    
  }

  void 
  xlate_ISTOREX(xml::ostream& xos, WN* wn, PUXlationContext& ctxt)
  {
    FORTTK_DIE(fortTkSupport::Diagnostics::Unimplemented);
    xos << std::endl << OPCODE_name(WN_opcode(wn));
    
  }


  void 
  WN2F_mstore(xml::ostream& xos, WN* wn, PUXlationContext& ctxt)
  {
    /* Note that we make the assumption that this is just like an 
     * ISTORE, and handle it as though it were.  We do not handle
     * specially assignment-forms where the lhs is incompatible with
     * the rhs, since we assume this will never happen for Fortran
     * and we cannot easily get around this like we do in C (i.e.
     * with cast expressions. (FIXME)
     */
    FORTTK_ASSERT(WN_operator(wn) == OPR_MSTORE, fortTkSupport::Diagnostics::UnexpectedInput);
#if 0
    FORTTK_ASSERT_WARN(WN_operator(WN_kid0(wn)) == OPR_MLOAD,
		       fortTkSupport::Diagnostics::UnexpectedOpr << "rhs of WN2F_mstore");
  
    //TODO: scalar expression allowed, but array/structure assignment assumed
    // with constant ie: should put out doloop?... call OFFSET_Memref?
#endif

    /* Get the base address into which we are storing a value */
    TY_IDX base_ty = WN_Tree_Type(WN_kid1(wn));
    if (!TY_Is_Pointer(base_ty))
      base_ty = WN_ty(wn);
  
    /* Get the lhs of the assignment (dereference address) */
    xos << std::endl; 
    xlate_MemRef(xos, WN_kid1(wn),      /* base-symbol */
		 base_ty,               /* base-type */ 
		 TY_pointed(WN_ty(wn)), /* object-type */
		 WN_store_offset(wn),   /* object-ofst */ 
		 ctxt);
  
    // Assign the rhs to the lhs.
    xos << "mstore=" << std::endl;
  
    /* The rhs */
    TranslateWN(xos, WN_kid0(wn), ctxt);

    
  }


  void
  WN2F_pstid(xml::ostream& xos, WN* wn, PUXlationContext& ctxt)
  {
    FORTTK_ASSERT(WN_operator(wn) == OPR_PSTID, fortTkSupport::Diagnostics::UnexpectedInput);
  
    /* Get the lhs of the assignment */
    xos << std::endl;
    if (ST_class(WN_st(wn)) == CLASS_PREG) {
      xlate_PregRef(xos, WN_st(wn), ST_type(WN_st(wn)), WN_store_offset(wn), 
		    ctxt);
    } 
    else {
      xlate_SymRef(xos, WN_st(wn),                      /* base-symbol */
		   Stab_Pointer_To(ST_type(WN_st(wn))), /* base-type */
		   WN_ty(wn),                           /* object-type */
		   WN_store_offset(wn),                 /* object-ofst */
		   ctxt);
    }
  
    // Assign the rhs to the lhs.
    xos << "pstid=>";
  
    /* The rhs */
    if (TY_is_logical(Ty_Table[WN_ty(wn)])) {
      ctxt.currentXlationContext().setFlag(XlationContext::HAS_LOGICAL_ARG);
      TranslateWN(xos, WN_kid0(wn), ctxt);
      ctxt.currentXlationContext().unsetFlag(XlationContext::HAS_LOGICAL_ARG);
    } else
      TranslateWN(xos, WN_kid0(wn), ctxt);
  
    
  } /* WN2F_pstid */


  void
  WN2F_pstore(xml::ostream& xos, WN* wn, PUXlationContext& ctxt)
  {
    TY_IDX        base_ty;
    FORTTK_ASSERT(WN_operator(wn) == OPR_PSTORE, fortTkSupport::Diagnostics::UnexpectedInput);
  
    /* Get the base address into which we are storing a value */
    base_ty = WN_Tree_Type(WN_kid1(wn));
    if (!TY_Is_Pointer(base_ty))
      base_ty = WN_ty(wn);
  
    /* Get the lhs of the assignment (dereference address) */
    xos << std::endl;
    ctxt.currentXlationContext().setFlag(XlationContext::HAS_NO_ARR_ELMT);
  
    xlate_MemRef(xos,
		 WN_kid1(wn),           /* base-symbol */
		 base_ty,               /* base-type */
		 TY_pointed(WN_ty(wn)), /* object-type */
		 WN_store_offset(wn),   /* object-ofst */
		 ctxt);
    ctxt.currentXlationContext().unsetFlag(XlationContext::HAS_NO_ARR_ELMT);
  
    // Assign the rhs to the lhs.
    xos << "pstore=>";
  
    /* The rhs */
    if (TY_is_logical(Ty_Table[TY_pointed(WN_ty(wn))])) {
      ctxt.currentXlationContext().setFlag(XlationContext::HAS_LOGICAL_ARG);
      TranslateWN(xos, WN_kid0(wn), ctxt);
      ctxt.currentXlationContext().unsetFlag(XlationContext::HAS_LOGICAL_ARG);
    } 
    else {
      TranslateWN(xos, WN_kid0(wn), ctxt);
    }
  
    
  } /* WN2F_pstore */

  // ***************************************************************************
  // Array Operators (N-ary Operations)
  // ***************************************************************************

  void
  xlate_ARRAY(xml::ostream& xos, WN* wn, PUXlationContext& ctxt)
  {
    // N.B.: WHIRL indices are 0-based and memory layout is row-major
    // (right-most index represents contiguous elements).  
    // In contrast, Fortran indices are 1-based and memory layout is
    // column-major (left-most index represents contiguous elements).
    // To convert WHIRL indices into a Fortran index expression, reverse
    // their order and denormalize to base 1.
  
    /* Note that array indices have been normalized to assume the
     * array is based at index zero.  Since a base at index 1 is
     * the default for Fortran, we denormalize to base 1 here. */
    FORTTK_ASSERT(WN_operator(wn) == OPR_ARRAY, fortTkSupport::Diagnostics::UnexpectedInput);

    BOOL deref = ctxt.currentXlationContext().isFlag(XlationContext::DEREF_ADDR);
  
    // Only allow taking the address of an array element for F90!
#if 0
    FORTK_ASSERT_WARN(deref, "taking the address of an array element");
#endif

    bool newContext = false; // FIXME: abstract (symref, memref)
    if (!ctxt.currentXlationContext().isFlag(XlationContext::VARREF)) {
      xos << xml::BegElem(XAIFStrings.elem_VarRef())
	  << xml::Attr("vertex_id", ctxt.currentXlationContext().getNewVertexId())
	  << xml::Attr("du_ud", ctxt.findUDDUChainId(wn))
	  << xml::Attr("alias", ctxt.getAliasMapKey(wn));
      ctxt.createXlationContext(XlationContext::VARREF, wn); // FIXME: do we need wn?
      newContext = true; 
    }

    /* Get the array or, for ptr-as-array types, the element type */  
    WN* kid = WN_kid0(wn);
    TY_IDX ptr_ty = WN_Tree_Type(kid);
  
    if (WN2F_Is_Address_Preg(kid, ptr_ty)) {
      /* a preg or sym has been used as an address, usually after
	 optimization don't know base type, or anything else so use
	 OPR_ARRAY to generate bounds */
      TranslateWN(xos, kid, ctxt);
      xlate_ArrayIndices(xos, wn, ctxt);
      // FIXME
    } else {
      TY_IDX array_ty = TY_pointed(ptr_ty); // base of OPR_ARRAY
    
      if (WN_operator(kid) == OPR_LDID 
	  && ST_sclass(WN_st(kid)) == SCLASS_FORMAL 
	  && !ST_is_value_parm(WN_st(kid))
	  && WN_element_size(wn) == (INT64)TY_size(array_ty)
	  && WN_num_dim(wn) == 1
	  && WN_operator(WN_array_index(wn, 0)) == OPR_INTCONST 
	  && WN_const_val(WN_array_index(wn, 0)) == 0 
	  && !TY_ptr_as_array(Ty_Table[WN_ty(kid)])
	  && (!TY_Is_Array(array_ty) 
	      || TY_size(TY_AR_etype(array_ty)) < TY_size(array_ty))) {
	// This array access is just a weird representation for an implicit
	// reference parameter dereference.  Ignore the array indexing.
	TranslateWN(xos, kid, ctxt);

      } else if (!TY_ptr_as_array(Ty_Table[ptr_ty]) 
		 && TY_Is_Character_String(array_ty) ) {
	/* We assume that substring accesses are treated in the handling
	 * of intrinsic functions, except when the substrings are to be
	 * handled as integral types and thus are encountered here. */
	WN2F_String_Argument(xos, wn, WN2F_INTCONST_ONE, ctxt);
      } else { 
	// A regular array access

	// Array base
	UINT srcid = ctxt.currentXlationContext().peekVertexId();
	TranslateWN(xos, kid, ctxt); // still use ctxt.currentXlationContext().isFlag(XlationContext::DEREF_ADDR)
	ctxt.currentXlationContext().unsetFlag(XlationContext::DEREF_ADDR);

	// Array indexing
	UINT targid = ctxt.currentXlationContext().peekVertexId();
	WN2F_array_bounds(xos, wn, array_ty, ctxt);
      
	DumpVarRefEdge(xos, ctxt.currentXlationContext().getNewEdgeId(), srcid, targid);
      }
    }
  
    if (newContext) {
      ctxt.deleteXlationContext();
      xos << xml::EndElem /* elem_VarRef() */;
    }
  
    
  } /* xlate_ARRAY */


  /*
    |*                                                                           *|
    |* for array section triplet node,kid0 is lower bound,it should plus 1LL for *|
    |* adjusted bound,upper bound=kid0+k1*k2                                     *|
    |* kid0 evaluates to the starting integer value of the progression.	     *|
    |* kid1 evaluates to an integer value that gives the stride in the           *|
    |*   progression                                                             *|
    |* kid2 evaluates to the number of values in the progression       	     *|
    |*                                                                           *|
  */
  void
  WN2F_triplet(xml::ostream& xos, WN* wn, PUXlationContext& ctxt)
  {
    WN      *kid0;
    WN      *kid1;
    WN      *kid2;
    kid0=WN_kid0(wn);
    kid1=WN_kid1(wn);
    kid2=WN_kid2(wn);
    TranslateWN(xos, kid0, ctxt);
    if ((WN_operator(kid2) == OPR_INTCONST) &&
	(WN_const_val(kid2)==INT_MIN) )
      xos << ":";
    else  {
      xos << ":";
      if (WN_operator(kid0) == OPR_INTCONST &&
	  WN_operator(kid1) == OPR_INTCONST &&
	  WN_operator(kid2) == OPR_INTCONST) {
	std::string val;
	if ((WN_const_val(kid0)+WN_const_val(kid2)*WN_const_val(kid1))>=INT_MAX)
	  val = TCON2F_translate(Host_To_Targ(MTYPE_I8, WN_const_val(kid0)+
					      WN_const_val(kid2)*
					      WN_const_val(kid1)),
				 FALSE);
	else    
	  val = TCON2F_translate(Host_To_Targ(MTYPE_I4, WN_const_val(kid0)+
					      WN_const_val(kid2)*
					      WN_const_val(kid1)),
				 FALSE);
	xos << val;
      
      } 
      else 
	if (WN_operator(kid0) == OPR_INTCONST &&
	    WN_operator(kid1) == OPR_INTCONST ) {
	  if (WN_const_val(kid1)==1) {
	    if (WN_const_val(kid0)== 0) {
	      TranslateWN(xos, kid2, ctxt);
	    }
	    else {
	      TranslateWN(xos, kid1, ctxt);
	      xos << "+";
	      TranslateWN(xos, kid2, ctxt); }
	  }
	  else {
	    if (WN_const_val(kid0)== 0){
	      TranslateWN(xos, kid1, ctxt);
	      xos << "*";
	      TranslateWN(xos, kid2, ctxt); }
	    else {
	      TranslateWN(xos, kid0, ctxt);
	      xos << "+";
	      TranslateWN(xos, kid1, ctxt);
	      xos << "*";
	      TranslateWN(xos, kid2, ctxt); }
	  }
	}
	else 
	  if (WN_operator(kid1) == OPR_INTCONST &&
	      WN_operator(kid2) == OPR_INTCONST) {
	    TranslateWN(xos, kid0, ctxt);
	    xos << "+";
	  
	    std::string val;
	    if ((WN_const_val(kid1)*WN_const_val(kid2))>=INT_MAX)
	      val = TCON2F_translate(Host_To_Targ(MTYPE_I8,   
						  WN_const_val(kid1)*
						  WN_const_val(kid2)),
				     FALSE);
	    else 
	      val = TCON2F_translate(Host_To_Targ(MTYPE_I4,   
						  WN_const_val(kid1)*
						  WN_const_val(kid2)),
				     FALSE);
	  }
	  else 
	    if (WN_operator(kid0) == OPR_INTCONST &&
		WN_operator(kid2) == OPR_INTCONST) {
	      if (WN_const_val(kid2)==1) {
		if (WN_const_val(kid0)== 0) {
		  TranslateWN(xos, kid1, ctxt);
		}
		else {
		  TranslateWN(xos, kid0, ctxt);
		  xos << "+";
		  TranslateWN(xos, kid1, ctxt); 
		}
	      }
	      else {
		if (WN_const_val(kid0)== 0){
		  TranslateWN(xos, kid2, ctxt);
		  xos << "*";
		  TranslateWN(xos, kid1, ctxt); }
		else {
		  TranslateWN(xos, kid0, ctxt);
		  xos << "+";
		  TranslateWN(xos, kid1, ctxt);
		  xos << "*";
		  TranslateWN(xos, kid2, ctxt); }
	      }
	    }
	    else 
	      if (WN_operator(kid0) == OPR_INTCONST){ 
		if (WN_const_val(kid0)==0) {
		  TranslateWN(xos, kid1, ctxt);
		  xos << "*";
		  TranslateWN(xos, kid2, ctxt);}
		else {
		  TranslateWN(xos, kid0, ctxt);
		  xos << "+";
		  TranslateWN(xos, kid1, ctxt);
		  xos << "*";
		  TranslateWN(xos, kid2, ctxt);
		}
	      }
	      else 
		if (WN_operator(kid1) == OPR_INTCONST){
		  TranslateWN(xos, kid0, ctxt);
		  xos << "+";
		  if (WN_const_val(kid1)==1){
		    TranslateWN(xos, kid2, ctxt);}
		  else {
		    TranslateWN(xos, kid1, ctxt);
		    xos << "*";
		    TranslateWN(xos, kid2, ctxt);
		  }
		}
		else
		  if (WN_operator(kid2) == OPR_INTCONST) {
		    TranslateWN(xos, kid0, ctxt);
		    xos << "+";
		    if (WN_const_val(kid2)==1)
		      TranslateWN(xos, kid1, ctxt);
		    else
		      {
			TranslateWN(xos, kid2, ctxt);
			xos << "*";
			TranslateWN(xos, kid1, ctxt);
		      }
		  }
      if ((WN_operator(kid1) == OPR_INTCONST) && 
	  (WN_const_val(kid1)==1))  {
      } else {
	xos << ":";
	TranslateWN(xos, kid1, ctxt);
      } 
    }  
    
  
  }

  void
  WN2F_src_triplet(xml::ostream& xos, WN* wn, PUXlationContext& ctxt) {
    if (WN_operator(WN_kid0(wn))!= OPR_IMPLICIT_BND) {
      xos << xml::BegElem(XAIFStrings.elem_Index());
      ctxt.createXlationContext(); 
      ctxt.currentXlationContext().unsetFlag(XlationContext::VARREF); // elem_Index() contains ExpressionType
      TranslateWN(xos, WN_kid0(wn), ctxt);
      ctxt.deleteXlationContext();
      xos << xml::EndElem;
    }
    if (WN_operator(WN_kid1(wn))!= OPR_IMPLICIT_BND) { 
      xos << xml::BegElem(XAIFStrings.elem_Bound());
      ctxt.createXlationContext(); 
      ctxt.currentXlationContext().unsetFlag(XlationContext::VARREF); // elem_Bound() contains ExpressionType
      TranslateWN(xos, WN_kid1(wn), ctxt); 
      ctxt.deleteXlationContext();
      xos << xml::EndElem;
    }
    xos << xml::BegElem(XAIFStrings.elem_Stride());
    ctxt.createXlationContext(); 
    ctxt.currentXlationContext().unsetFlag(XlationContext::VARREF); // elem_Stride() contains ExpressionType
    TranslateWN(xos, WN_kid2(wn), ctxt); 
    ctxt.deleteXlationContext();
    xos << xml::EndElem;
  }

  void
  WN2F_arrayexp(xml::ostream& xos, WN* wn, PUXlationContext& ctxt)
  {
    TranslateWN(xos, WN_kid0(wn), ctxt);
    
  }
  

  void
  WN2F_arrsection(xml::ostream& xos, WN* wn, PUXlationContext& ctxt)
  {
    /* Note that array indices have been normalized to assume the
     * array is based at index zero.  Since a base at index 1 is
     * the default for Fortran, we denormalize to base 1 here.
     */
    FORTTK_ASSERT(WN_operator(wn) == OPR_ARRSECTION, fortTkSupport::Diagnostics::UnexpectedInput);

    BOOL  deref = ctxt.currentXlationContext().isFlag(XlationContext::DEREF_ADDR);
    WN    * kid;
    TY_IDX ptr_ty;
    TY_IDX array_ty;

    /* Only allow taking the address of an array element for F90!
     *
     */
#if 0
    FORTTK_ASSERT_WARN(deref, "taking the address of an array element");
#endif

    /* Get the array or, for ptr-as-array types, the element type */
    kid    = WN_kid0(wn);
    ptr_ty = WN_Tree_Type(kid);

    if (WN2F_Is_Address_Preg(kid,ptr_ty) or !TY_Is_Pointer(ptr_ty)) {
      /* a preg or sym has been used as an address, usually after
	 optimization don't know base type, or anything else so use
	 OPR_ARRAY to generate bounds */

      UINT srcid = ctxt.currentXlationContext().peekVertexId();
      TranslateWN(xos, kid, ctxt);
      // Array indexing
      UINT targid = ctxt.currentXlationContext().peekVertexId();
      WN2F_Arrsection_Slots(xos,wn,ctxt);
      DumpVarRefEdge(xos, ctxt.currentXlationContext().getNewEdgeId(), srcid, targid);
    }
    else {
      array_ty = TY_pointed(ptr_ty); // base of OPR_ARRAY
    
      if (WN_operator(kid) == OPR_LDID       &&

	  ST_sclass(WN_st(kid)) == SCLASS_FORMAL &&
	  !ST_is_value_parm(WN_st(kid))          &&
	  WN_element_size(wn) == (INT64)TY_size(array_ty)       &&
	  WN_num_dim(wn) == 1                            &&
	  WN_operator(WN_array_index(wn, 0)) == OPR_INTCONST &&
	  WN_const_val(WN_array_index(wn, 0)) == 0       &&
	  !TY_ptr_as_array(Ty_Table[WN_ty(kid)])           &&
	  (!TY_Is_Array(array_ty) ||
	   TY_size(TY_AR_etype(array_ty)) < TY_size(array_ty))) {
	/* This array access is just a weird representation for an implicit
	 * reference parameter dereference.  Ignore the array indexing.
	 */

	TranslateWN(xos, kid, ctxt);
      }
      else if (!TY_ptr_as_array(Ty_Table[ptr_ty]) 
	       && TY_Is_Character_String(array_ty)) {
	/* We assume that substring accesses are treated in the handling
	 * of intrinsic functions, except when the substrings are to be
	 * handled as integral types and thus are encountered here.
	 */
	WN2F_String_Argument(xos, wn, WN2F_INTCONST_ONE, ctxt);
      }
      else { /* A regular array access */
	/* Get the base of the object to be indexed into, still using
	 * ctxt.currentXlationContext().isFlag(XlationContext::DEREF_ADDR).
	 */
	bool newContext=false;
	if (!ctxt.currentXlationContext().isFlag(XlationContext::VARREF)) {
	  xos << xml::BegElem(XAIFStrings.elem_VarRef())
	      << xml::Attr("vertex_id", ctxt.currentXlationContext().getNewVertexId())
	      << xml::Attr("du_ud", ctxt.findUDDUChainId(wn))
	      << xml::Attr("alias", ctxt.getAliasMapKey(wn));
	  ctxt.createXlationContext(XlationContext::VARREF, wn);
	  newContext=true;
	}
	UINT srcid = ctxt.currentXlationContext().peekVertexId();
	TranslateWN(xos, kid, ctxt);
	ctxt.currentXlationContext().unsetFlag(XlationContext::DEREF_ADDR);
       
	if ( ctxt.currentXlationContext().isFlag(XlationContext::HAS_NO_ARR_ELMT))
	  ;
	else { 
	  UINT targid = ctxt.currentXlationContext().peekVertexId();
	  WN2F_arrsection_bounds(xos,wn,array_ty,ctxt);
	  DumpVarRefEdge(xos, ctxt.currentXlationContext().getNewEdgeId(), srcid, targid);
	}
	if (newContext) {
	  ctxt.deleteXlationContext();
	  xos << xml::EndElem /* elem_VarRef() */;
	}
      }
    }
    
  } /* WN2F_arrsection */


  void 
  WN2F_where(xml::ostream& xos, 
	     WN* wn, 
	     PUXlationContext& ctxt) {
    xos << "WHERE(";
    TranslateWN(xos, WN_kid0(wn), ctxt);
    xos << ")";
    TranslateWN(xos, WN_kid1(wn), ctxt);
    xos << "END WHERE";
    TranslateWN(xos, WN_kid2(wn), ctxt);
  }


  void
  WN2F_Arrsection_Slots(xml::ostream& xos, WN* wn, PUXlationContext& ctxt) {
    INT32 dim;
    INT32 array_dim;
    TY_IDX ttyy;
    ARB_HANDLE arb_base;
    WN* kid;
    /* Gets bounds from the slots of an OPC_ARRSECTION node  */
    /* Append the "denormalized" indexing expressions in reverse order
     * of the way they occur in the indexing expression, since Fortran
     * employs column-major array layout, meaning the leftmost indexing
     * expression represents array elements laid out in contiguous
     * memory locations.
     */
    WN* kid0 = WN_kid0(wn);
    if (WN_operator(kid0)==OPR_ILOAD && WN_operator(WN_kid0(kid0)) == OPR_STRCTFLD) { 
      kid0 = WN_kid0(kid0);
    }
    if (WN_operator(kid0)==OPR_STRCTFLD) { 
      ttyy=WN_GetRefObjType(kid0);
    }
    else { 
      ttyy = ST_type(WN_st(kid0));
    }
    if (TY_Is_Pointer(ttyy))  //Sept temp use
      ttyy=TY_pointed(ttyy);
    if (TY_is_f90_pointer(ttyy))
      ttyy = TY_pointed(ttyy);
    arb_base = TY_arb(ttyy);
    array_dim =  ARB_dimension(arb_base);
    if (array_dim>0) {
      xos << xml::BegElem(XAIFStrings.elem_ArrayElemRef())
	  << xml::Attr("vertex_id", ctxt.currentXlationContext().getNewVertexId());
      for (dim = WN_num_dim(wn)-1; dim >= 0; --dim) {
	xos << xml::BegElem(XAIFStrings.elem_IndexTriplet());
	if (WN_operator(WN_array_index(wn, dim))==OPR_SRCTRIPLET) {
	  TranslateWN(xos, WN_array_index(wn, dim), ctxt);
	} else {
	  xos << xml::BegElem(XAIFStrings.elem_Index());
	  ctxt.createXlationContext(); 
	  ctxt.currentXlationContext().unsetFlag(XlationContext::VARREF); // elem_Index() contains ExpressionType
	  TranslateWN(xos, WN_array_index(wn, dim), ctxt);
	  ctxt.deleteXlationContext();
	  xos << xml::EndElem;
	}
	xos << xml::EndElem;
      }
      xos << xml::EndElem;
    }
  }

  void
  xlate_ArrayIndices(xml::ostream& xos, WN* wn, PUXlationContext& ctxt)
  {
    // FIXME: do not handle co dimentions
  
    /* get array's rank and co_rank information from kid0 of wn
     * kid0 should be OPR_LDA */
    INT32 array_dim;
    WN* kid = WN_kid0(wn);
    if (WN_operator(kid) == OPR_LDA) {
      ST* st = WN_st(kid);
      TY_IDX ty = ST_type(st);
    
      if (TY_Is_Pointer(ty))
	ty = TY_pointed(ty);
      if (TY_is_f90_pointer(ty))
	ty = TY_pointed(ty);
    
      ARB_HANDLE arb_base = TY_arb(ty);
      array_dim = ARB_dimension(arb_base);
    } else {
      array_dim = WN_num_dim(wn);
    }
  
    /* Append indexing expressions in reverse order of the way they
     * occur in the indexing expression, since Fortran employs
     * column-major array layout, meaning the leftmost indexing
     * expression represents array elements laid out in contiguous
     * memory locations. */
    xos << xml::BegElem(XAIFStrings.elem_ArrayElemRef()) 
	<< xml::Attr("vertex_id", ctxt.currentXlationContext().getNewVertexId());
    for (INT32 dim = array_dim - 1; dim >= 0; --dim) {
      xos << xml::BegElem(XAIFStrings.elem_IndexTriplet());
      xos << xml::BegElem(XAIFStrings.elem_Index());
      ctxt.createXlationContext(); 
      ctxt.currentXlationContext().unsetFlag(XlationContext::VARREF); // elem_Index() contains ExpressionType
      TranslateWN(xos, WN_array_index(wn, dim), ctxt);
      ctxt.deleteXlationContext();
      xos << xml::EndElem;
      xos << xml::EndElem;
    }
    xos << xml::EndElem;
  }

  void
  WN2F_array_bounds(xml::ostream& xos, WN* wn, TY_IDX array_ty,
		    PUXlationContext& ctxt)
  {
    // FIXME: referenced in ty2xaif.cxx

    if (TY_is_f90_pointer(array_ty)) // FIXME (should this be moved up?)
      array_ty = TY_pointed(array_ty); //Sept
  
    // (TY_Is_Array(array_ty) && TY_AR_ndims(array_ty) >= WN_num_dim(wn)) 
    FORTTK_ASSERT_WARN((TY_AR_ndims(array_ty) == WN_num_dim(wn)),
		       "array dimension mismatch");
    FORTTK_ASSERT_WARN(((INT64)TY_size(TY_AR_etype(array_ty)) 
		        == WN_element_size(wn)) 
		       || WN_element_size(wn) < 0 
		       || TY_size(TY_AR_etype(array_ty)) == 0,
		       "access/declaration mismatch in array element size");
  
    xlate_ArrayIndices(xos, wn, ctxt);
  }

  void
  WN2F_arrsection_bounds(xml::ostream& xos, WN* wn, TY_IDX array_ty,
			 PUXlationContext& ctxt)
  {
    /* This prints the array subscript expression. It was part of
     * xlate_ARRAY, but was split so it could be used for bounds
     * of structure components.
     */
    if (TY_is_f90_pointer(array_ty))
      array_ty = TY_pointed(array_ty);//Sept
  
    if (TY_Is_Array(array_ty) && TY_AR_ndims(array_ty) >= WN_num_dim(wn)) {
      /* Cannot currently handle differing element sizes at place of
       * array declaration versus place of array access (TODO?). */
      FORTTK_ASSERT_WARN(((INT64)TY_size(TY_AR_etype(array_ty)) 
			  == WN_element_size(wn))
			 || WN_element_size(wn) < 0 
			 || TY_size(TY_AR_etype(array_ty)) == 0,
			 "access/declaration mismatch in array element size");
    
      WN2F_Arrsection_Slots(xos,wn,ctxt);
    
    } else { /* Normalize array access to assume a single dimension */
      FORTTK_DIE(fortTkSupport::Diagnostics::Unimplemented);
      FORTTK_ASSERT_WARN(!TY_Is_Array(array_ty) || TY_AR_ndims(array_ty) == 1,
			 "access/declaration mismatch in array dimensions");
      //FIXME: WN2F_Normalize_Idx_To_Onedim(xos, wn, ctxt);
    }
  
  }

  void
  WN2F_String_Argument(xml::ostream& xos, WN* base_parm, WN* length,
		       PUXlationContext& ctxt)
  {
    /* Append the xos denoting the substring expression represented
     * by the base-expression.
     *
     * There are two possibilities concerning the array base 
     * expressions.  It can be a pointer to a complete character-
     * string (array) or it can be a pointer to a character within 
     * a character-string (single character).  In the first instance,
     * the offset off the base of string is zero.  In the latter 
     * case, the offset is given by the array indexing operation.
     *
     * NOTE: In some cases (notably for IO_ITEMs), we may try to 
     * derive a substring off an OPC_VINTRINSIC_CALL node or a
     * VCALL node.  This should only happend when the returned value
     * is the first argument and the length is the second argument.
     */
    WN   *base = WN_Skip_Parm(base_parm);
    WN   *base1 = WN_Skip_Parm(base_parm);
    WN   *lower_bnd;
    WN   *length_new;
    WN   *arg_expr;
    TY_IDX str_ty;
    INT64 str_length;
  
    /* Skip any INTR_ADRTMP and INTR_VALTMP nodes */
    if (WN_operator(base) == OPR_INTRINSIC_OP &&
	(INTR_is_adrtmp(WN_intrinsic(base)) || 
	 INTR_is_valtmp(WN_intrinsic(base)))) {
      base = WN_kid0(base);
    }
  
    if (WN_operator(base) == OPR_CVTL) {
      /* probably CHAR(INT) within IO stmt. convert via CHAR & process
	 rest elsewhere */
      xos << "(char";
      TranslateWN(xos,WN_kid0(base),ctxt);
      xos << ')';
      return;
    }
  
  
    /* Handle VCALLs specially, since the string information is given
     * by the first two arguments to the call.  Note that we can 
     * always assume a lower bound of zero for these, as we never 
     * generate code for the return-address.  This should only occur
     * within an IO stmt.  Note that the type of VCALLs must be 
     * accessed in the ctxt of an ADRTMP or VALTMP.
     */
    if (WN_opcode(base) == OPC_VCALL ||
	WN_opcode(base) == OPC_VINTRINSIC_CALL) {
      arg_expr  = WN_Skip_Parm(WN_kid1(base));
      lower_bnd = WN2F_INTCONST_ZERO;
    
      /* fixed size string? */
      if (WN_operator(arg_expr) == OPR_INTCONST)
	str_length = WN_const_val(arg_expr);
      else
	str_length = -1 ;  
    
      ctxt.currentXlationContext().setFlag(XlationContext::DEREF_ADDR);
      TranslateWN(xos, base, ctxt);
      ctxt.currentXlationContext().unsetFlag(XlationContext::DEREF_ADDR);
    
    } else {
      /* A regular address expression as base */    
      WN2F_Get_Substring_Info(&base, &str_ty, &lower_bnd, &length_new);
    
      /* Was this a character component of an array of derived type? */
      /* eg: vvv(2)%ccc(:)(1:5) - offset to ccc is added above base, */
      /* ADD(8,ARRAY(2,LDA VVV)) with array section for CCC on top   */
      /* of the ADD, and the substring above the array section. Take */
      /* the substring off the top, and process the rest             */    
      if (TY_kind(str_ty) == KIND_STRUCT) {
	FLD_PATH_INFO *fld_path ;
	FLD_HANDLE fld;
	TY_IDX  ty_idx ; 
      
	TY & ty = New_TY(ty_idx);
      
	TY_Init (ty, 1, KIND_SCALAR, MTYPE_U1, Save_Str(".w2fch."));
	Set_TY_is_character(ty);
      
	fld_path = TY2F_Get_Fld_Path(str_ty, 
				     ty_idx,
				     WN2F_Sum_Offsets(base));
      
	fld = TY2F_Last_Fld(fld_path);
	TY2F_Free_Fld_Path(fld_path);
      
	/* call memref for FLD offset, otherwise the ADD is */
	/* just another binary op                           */
	xlate_MemRef(xos, WN_kid0(base), WN_Tree_Type(base),
		     FLD_type(fld), 0, ctxt);
      } else {
	str_length = TY_size(str_ty);
      
	/* with optimization, may not have useful address TY 
	 * when TreeType will return array of U1 from SubstringInfo */
      
	FORTTK_ASSERT(TY_Is_Character_String(str_ty) 
		      || TY_Is_Array_Of_UChars(str_ty),
		      "Unexpected conversion from pointer to character string");

	/* Get the string base and substring notation for the argument.  */
	if ((WN_operator(lower_bnd) != OPR_INTCONST   
	     ||
	     WN_const_val(lower_bnd) != 0                    
	     ||
	     WN_operator(length_new) != OPR_INTCONST 
	     ||
	     WN_const_val(length_new) != str_length )
	    && 
	    !ctxt.currentXlationContext().isFlag(XlationContext::HAS_NO_ARR_ELMT)
	    && 
	    WN_class(base)!=CLASS_CONST
	    && 
	    WN_class(base)!=CLASS_FUNC) { 
	  xos << xml::BegElem(XAIFStrings.elem_VarRef())
	      << xml::Attr("vertex_id", ctxt.currentXlationContext().getNewVertexId())
	      << xml::Attr("du_ud", ctxt.findUDDUChainId(base))
	      << xml::Attr("alias", ctxt.getAliasMapKey(base));
	  ctxt.createXlationContext(XlationContext::VARREF, base); 
	  UINT srcid = ctxt.currentXlationContext().peekVertexId();
	  TranslateWN(xos, base, ctxt);
	  UINT targid = ctxt.currentXlationContext().peekVertexId();
	  WN2F_Substring(xos, 
			 str_length, 
			 lower_bnd,
			 length_new, 
			 ctxt);
	  DumpVarRefEdge(xos, ctxt.currentXlationContext().getNewEdgeId(), srcid, targid);
	  xos << xml::EndElem /* elem_VarRef() */;
	  ctxt.deleteXlationContext();
	}
	else {
	  ctxt.currentXlationContext().setFlag(XlationContext::DEREF_ADDR);
	  TranslateWN(xos, base, ctxt);
	  ctxt.currentXlationContext().unsetFlag(XlationContext::DEREF_ADDR);
	}
	return;
      }
    } /* WN2F_String_Argument */
  }

  static void
  DumpVarRefEdge(xml::ostream& xos, UINT eid, UINT srcid, UINT targid)
  {
    xos << xml::BegElem(XAIFStrings.elem_VarRefEdge()) << xml::Attr("edge_id", eid)
	<< xml::Attr("source", srcid) << xml::Attr("target", targid)
	<< xml::EndElem;
  }

}
