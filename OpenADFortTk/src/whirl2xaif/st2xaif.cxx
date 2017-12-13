// ##########################################################
// # This file is part of OpenADFortTk.                     #
// # The full COPYRIGHT notice can be found in the top      #
// # level directory of the OpenADFortTk source tree.       #
// # For more information visit                             #
// # http://www.mcs.anl.gov/openad                          #
// ##########################################################

/* ====================================================================
 * ====================================================================
 *
 * Revision history:
 *  07-May-95 - Original Version
 *
 * Description: FIXME
 *
 *    See st2f.h for a description of the exported functions and 
 *    variables.  This module translates ST nodes into variable and
 *    function declarations (TranslateSTDecl), and gets the 
 *    lvalue for a variable or function when directly referenced in
 *    an expression (TranslateSTUse).  We provide a special 
 *    interface to deal with pseudo registers (pregs), but some 
 *    symbols must be handled by the context in which they appear,
 *    since this context uniquely determines the reference (e.g. 
 *    labels has label-numbers in the WN tree).
 *
 *    Possibly necessary TODO: sym_consts are only partially
 *    supported at the moment.
 *
 *    Fortran pointers are represented by two declarations, where
 *    one declares the pointer object (which is allocated memory)
 *    and one denotes the pointer dereference which also serves to
 *    specify the type of object to which is pointed:
 *
 *        INTEGER*4 a(12)
 *        POINTER(p, a)
 *
 *    Only "p" occurs in the WHIRL symbol table.  We have to derive
 *    "a" from "p" (with a name derived from "p").  The w2cf_symtab.h
 *    facilities coordinates this for us.
 *
 *    It is crucial that names with external linkage are generated 
 *    with the same name between compilation units.  For this reason
 *    we give file-scope variables precedence in name-ownership (i.e.
 *    they are entered first into the symbol-table).  If, despite this
 *    effort, there are clashes between names with static and external 
 *    linkage, the generated code may not be compilable or correctly
 *    executable.  TODO: Emit warning about this.
 * 
 * ====================================================================
 * ====================================================================
 */

#include <ctype.h>
#include <sstream> 
#include <alloca.h>

#include "Open64IRInterface/Open64BasicTypes.h"

#include "wn2xaif.h"
#include "st2xaif.h"
#include "ty2xaif.h"
#include "Args.h"

#include "Open64IRInterface/SymTab.h"


namespace whirl2xaif { 



  /*------- Fwd refs for miscellaneous utilities ------------------------*/
  /*---------------------------------------------------------------------*/

  static BOOL ST2F_Is_Dummy_Procedure(ST *st) ;
  static void ST2F_Declare_Return_Type(xml::ostream& xos,TY_IDX return_ty, 
				       PUXlationContext& ctxt);

  // Symbol declarations and uses

  static void 
  xlate_ST_ignore(xml::ostream& xos, ST *st, PUXlationContext& ctxt);

  static void 
  xlate_STDecl_error(xml::ostream& xos, ST *st, PUXlationContext& ctxt);
  static void 
  xlate_STDecl_VAR(xml::ostream& xos, ST *st, PUXlationContext& ctxt);
  static void 
  xlate_STDecl_FUNC(xml::ostream& xos, ST *st, PUXlationContext& ctxt);
  static void 
  xlate_STDecl_CONST(xml::ostream& xos, ST *st, PUXlationContext& ctxt);
  static void 
  xlate_STDecl_PREG(xml::ostream& xos, ST *st, PUXlationContext& ctxt);
  static void 
  xlate_STDecl_BLOCK(xml::ostream& xos, ST *st, PUXlationContext& ctxt);
  static void 
  xlate_STDecl_NAME(xml::ostream& xos, ST *st, PUXlationContext& ctxt);
  static void 
  xlate_STDecl_TYPE(xml::ostream& xos, ST *st, PUXlationContext& ctxt);

  static void 
  xlate_STUse_error(xml::ostream& xos, ST *st, PUXlationContext& ctxt);
  static void 
  xlate_STUse_VAR(xml::ostream& xos, ST *st, PUXlationContext& ctxt);
  static void 
  xlate_STUse_CONST(xml::ostream& xos, ST *st, PUXlationContext& ctxt);
  static void 
  xlate_STUse_BLOCK(xml::ostream& xos, ST *st, PUXlationContext& ctxt);

  static FLD_HANDLE
  TY_Lookup_FLD(TY_IDX struct_ty, TY_IDX ref_ty, UINT64 ref_ofst, unsigned short eqInst=1);

  // ***************************************************************************


  // identical copy from xaif2whirl.cxx
  static FLD_HANDLE 
  TY_Lookup_FLD(TY_IDX struct_ty, TY_IDX ref_ty, UINT64 ref_ofst,unsigned short eqInst)
  {
    FLD_ITER fld_iter = Make_fld_iter(TY_fld(struct_ty));
    unsigned short foundInst=0;
    do {
      FLD_HANDLE fld(fld_iter);
      UINT64 ofst = FLD_ofst(fld);
      TY_IDX ty   = FLD_type(fld);
      if (ofst == ref_ofst) {
	++foundInst;
	if (ref_ty == 0) {
	  if (eqInst==foundInst)
	    return fld;
	}
	else {
	  if (Stab_Identical_Types(ref_ty, ty, FALSE /* check_quals */,
				   FALSE /* check_scalars */, TRUE)) {
	    return fld;
	  }
	}
      }
    } while (!FLD_last_field(fld_iter++));
    return FLD_HANDLE(); // null field
  }

  std::string
  TCON2F_hollerith(TCON tvalue)
  {
    /* Translates the given Hollerith constant into Fortran representation.
     * A hollerith constant cannot be split into substrings.
     */
    const char *strbase;
    char       *str;
    INT32       strlen;
  
    FORTTK_ASSERT_WARN(TCON_ty(tvalue) == MTYPE_STR,
		       "Unexpected type " << MTYPE_name(TCON_ty(tvalue)));
  
    strlen = Targ_String_Length(tvalue);
    strbase = Targ_String_Address(tvalue);
    str = (char *) alloca(strlen + 16);
    sprintf(str, "%dH%s", strlen, strbase);
  
    return std::string(str);
  } /* TCON2F_hollerith */

   
  std::string
  TCON2F_translate(TCON tvalue, BOOL is_logical, TY_IDX object_ty)
  {
    // Note: It would be nice to simply use C++ stream formatting
    // instead of Targ_Print(..), but a TCON is a (complex) union whose
    // interpretation technically depends on the target.

    // Since Targ_Print(..) prints floating points with Fortran
    // exponents (i.e. the exponent of a double is preceeded by 'd'
    // instead of an 'e'), we massage its output.

    // FIXME: for now we use this hack to return a string
    std::ostringstream sstr;
    char* floatstr = NULL;
  
    if (is_logical && MTYPE_type_class(TCON_ty(tvalue)) & MTYPE_CLASS_INTEGER) {
      // Treat it as regular integral constant, unless it has value 0 or 1.
      if (Targ_To_Host(tvalue) == 0LL)
	sstr << "false";
      else if  (Targ_To_Host(tvalue) == 1LL)
	sstr << "true";
      else
	is_logical = FALSE;
    } else { /* Only integral values can be treated as boolean */
      is_logical = FALSE; 
    }
  
    if (!is_logical) {
      switch (TCON_ty(tvalue)) {

      case MTYPE_STR: {
	// To be entirely safe, we do not assume the string contains
	// NULL terminator.
	INT32 len = Targ_String_Length(tvalue);
	const char* str = Targ_String_Address(tvalue);      
	for (int i = 0; i < len; ++i)
	  sstr << str[i];
      }
	break;
      
      case MTYPE_I1:
      case MTYPE_I2:
      case MTYPE_I4:
	sstr << Targ_Print("%1d", tvalue);
	break;
      
      case MTYPE_I8:
	sstr << Targ_Print("%1lld", tvalue);
	break;
      
      case MTYPE_U1:
      case MTYPE_U2:
      case MTYPE_U4:
	sstr << Targ_Print("%1u", tvalue);
	break;
      
      case MTYPE_U8:
	sstr << Targ_Print("%1llu", tvalue);
	break;
      
      case MTYPE_F4:
	floatstr = Targ_Print("%.10e", tvalue);
	break;
      
      case MTYPE_F8:
	floatstr = Targ_Print("%.20e", tvalue);
	break;
      
      case MTYPE_FQ:
	floatstr = Targ_Print(NULL, tvalue);
	break;
      
      case MTYPE_C4:
      case MTYPE_C8:
      case MTYPE_CQ:
	sstr << '(' << TCON2F_translate(Extract_Complex_Real(tvalue), FALSE)
	     << ',' << TCON2F_translate(Extract_Complex_Imag(tvalue), FALSE)
	     << ')';
	break;
      
      default:
	/* Only expression nodes should be handled here */
	FORTTK_DIE("Unexpected type " << MTYPE_name(TCON_ty(tvalue)));
	break;
      }
    }

    if (floatstr) {
      char* exp = NULL;
      if (exp = strchr(floatstr, 'd')) {
	*exp = 'e';
      }
      sstr << floatstr;
    }
  
    return sstr.str();

  } /* TCON2F_translate */

  std::string
  TCON2F_translate(TCON tvalue, BOOL is_logical)
  {
    return TCON2F_translate(tvalue, is_logical, (TY_IDX)NULL);
  } 


  // ***************************************************************************

  // usage: xlate_SymbolTables(xos, CURRENT_SYMTAB, symtab, ctxt);

  void 
  xlate_SymbolTables(xml::ostream& xos, SYMTAB_IDX symtab_lvl, 
		     fortTkSupport::ScalarizedRefTab_W2X* nonscalarsymtab, 
		     PUXlationContext& ctxt)
  {
    xos << xml::BegElem("xaif:SymbolTable") << xml::EndAttrs;

    xlate_SYMTAB(xos, symtab_lvl, ctxt);
    xlate_ScalarizedRefTab(xos, nonscalarsymtab, ctxt);
  
    xos << xml::EndElem;
  }

  void 
  xlate_ArrayBounds(xml::ostream& xos, 
		    TY_IDX ty_idx, 
		    PUXlationContext& ctxt) { 
    if (TY_kind(ty_idx) == KIND_ARRAY) {
      if (! TY_is_character(ty_idx) &&  ! TY_is_f90_assumed_size(ty_idx)) { 
	bool assumeBoundsAllConst=false;
	// figure out if all bounds are constant
	for (int i=0; i<TY_AR_ndims(ty_idx); i++) {
	  if (TY_AR_const_lbnd(ty_idx,i) && TY_AR_const_ubnd(ty_idx,i)) {
	    if (!assumeBoundsAllConst && i==0) { 
	      assumeBoundsAllConst=true;
	    }
	  }
	  else 
	    if (assumeBoundsAllConst)
	      assumeBoundsAllConst=false;
	}
	// only if all of them are constant we  
	// specify the dimensions in xaif.  
	// If they are not constant or only 
	// some are constant we don't bother and 
	// assume everything is variable
	if (assumeBoundsAllConst) { 
	  for (int i=0; i<TY_AR_ndims(ty_idx); i++) {
	    xos << xml::BegElem("xaif:DimensionBounds") 
		<< xml::Attr("lower", TY_AR_lbnd_val(ty_idx,i))
		<< xml::Attr("upper", TY_AR_ubnd_val(ty_idx,i))
		<< xml::EndElem;
	  }
	}
      }
    } 
  } 

  // FIXME: move to xlateSYMTAB.cxx
  class xlate_ST_TAB {
  public:
    xlate_ST_TAB(xml::ostream& xos_, SYMTAB_IDX symtab_, PUXlationContext& ctxt_) 
      : xos(xos_), symtab(symtab_), ctxt(ctxt_)
    { } 

    // A function object applied to every entry of a ST_TAB
    void operator()(UINT32 idx, ST* st) const 
    { 
      TranslateSTDecl(xos, st, ctxt);
    }
  
  private:
    xml::ostream&   xos;
    SYMTAB_IDX      symtab;
    PUXlationContext& ctxt;  
  };


  // FIXME: move to xlateSYMTAB.cxx
  // xlate_SYMTAB: Translate a WHIRL SYMTAB (a collection of tables) to
  // an XAIF symbol table.  'symtab_lvl' is an index (lexical level) in
  // the current 'Scope_tab[]'.
  void 
  xlate_SYMTAB(xml::ostream& xos, SYMTAB_IDX symtab_lvl,
	       PUXlationContext& ctxt)
  {
    // 'For_all' applies 'operator()' to every entry of St_Table.
    For_all(St_Table, symtab_lvl, xlate_ST_TAB(xos, symtab_lvl, ctxt));
  }

#if 0
  void 
  xlate_PREGTAB(xml::ostream& xos, SYMTAB_IDX symtab_lvl,
		PUXlationContext& ctxt)
  {
    // 'For_all' applies 'operator()' to every entry of Preg_Table.
    For_all(Preg_Table, symtab_lvl, xlate_PREG_TAB(xos, symtab_lvl, ctxt));
  }
#endif


  void
  xlate_ScalarizedRefTab(xml::ostream& xos, 
		         fortTkSupport::ScalarizedRefTab_W2X* symtab, 
			 PUXlationContext& ctxt)
  {
    if (!symtab) { return; }
  
    for (fortTkSupport::ScalarizedRefTab_W2X::ScalarizedRefPoolTy::iterator it 
	   = symtab->RefPoolBegin(); 
	 it != symtab->RefPoolEnd(); ++it) {
      fortTkSupport::ScalarizedRef* sym = (*it);
    
      WN* wn = sym->getWN();
      TY_IDX ty = WN_GetRefObjType(wn);
      const char* ty_str = TranslateTYToSymType(ty);
      if (!ty_str) { ty_str = "***"; }
      if (WN_operator(wn)==OPR_ILOAD)
	ty=WN_Tree_Type(WN_kid0(wn));
      else
	ty = WN_Tree_Type(wn);
      bool isPointer = TY_Is_Pointer(ty) || TY_is_f90_pointer(ty);
      bool isFnPointer = isPointer && (TY_kind(TY_pointed(ty)) == KIND_FUNCTION);
      const char* shape_str = (isPointer && (!isFnPointer)) ? TranslateTYToSymShape(TY_pointed(ty))
                                        : TranslateTYToSymShape(ty);
    
      int active = (strcmp(ty_str, "real") == 0 // FIXME: see xlate_STDecl_VAR
		    || strcmp(ty_str, "complex") == 0) ? 1 : 0; 

      xos << xml::BegElem("xaif:Symbol") << xml::Attr("symbol_id", sym->getName().c_str());    
      if (isFnPointer) {
	xos << xml::Attr("kind", "pointer") << xml::Attr("type", ty_str);
      }
      else {
	xos << xml::Attr("kind", "variable") << xml::Attr("type", ty_str);
      }
      xos << xml::Attr("shape", shape_str) << WhirlIdAnnot(ctxt.findWNId(wn))
	  << xml::Attr("active", active) << xml::EndElem;
    }
  }


  // ***************************************************************************

  typedef void (*XlateSTHandlerFunc)(xml::ostream&, ST*, PUXlationContext&);

  static const XlateSTHandlerFunc XlateSTDecl_HandlerTable[CLASS_COUNT] =
    {
      &xlate_ST_ignore,     /* CLASS_UNK    == 0 */
      &xlate_STDecl_VAR,    /* CLASS_VAR    == 1 */
      &xlate_STDecl_FUNC,   /* CLASS_FUNC   == 2 */
      &xlate_STDecl_CONST,  /* CLASS_CONST  == 3 */
      &xlate_STDecl_PREG,   /* CLASS_PREG   == 4 */
      &xlate_STDecl_BLOCK,  /* CLASS_BLOCK  == 5 */
      &xlate_STDecl_NAME,   /* CLASS_NAME   == 6 */
      &xlate_STDecl_error,  /* CLASS_MODULE == 7 */
      &xlate_STDecl_TYPE,   /* CLASS_TYPE   == 8 */
      &xlate_STDecl_CONST,  /* CLASS_PARAMETER  == 9 */
    };

  static const XlateSTHandlerFunc XlateSTUse_HandlerTable[CLASS_COUNT] =
    {
      &xlate_ST_ignore,     /* CLASS_UNK   == 0 */
      &xlate_STUse_VAR,     /* CLASS_VAR   == 1 */
      &xlate_STUse_error,   /* CLASS_FUNC  == 2 */
      &xlate_STUse_CONST,   /* CLASS_CONST == 3 */
      &xlate_STUse_error,   /* CLASS_PREG  == 4 */
      &xlate_STUse_BLOCK,   /* CLASS_BLOCK == 5 */
      &xlate_STUse_error,   /* CLASS_NAME  == 6 */
      &xlate_STDecl_error,  /* CLASS_MODULE == 7 */
      &xlate_STDecl_error,  /* CLASS_TYPE   == 8 */
      &xlate_STDecl_error,  /* CLASS_PARAMETER  == 9 */
    };

  // ***************************************************************************

  void 
  TranslateSTDecl(xml::ostream& xos, ST* st, PUXlationContext& ctxt)
  { 
    XlateSTDecl_HandlerTable[ST_sym_class(st)](xos, st, ctxt);
  } 

  void 
  TranslateSTUse(xml::ostream& xos, ST* st, PUXlationContext& ctxt)
  { 
    XlateSTUse_HandlerTable[ST_sym_class(st)](xos, st, ctxt);
  }

  static void 
  xlate_ST_ignore(xml::ostream& xos, ST *st, PUXlationContext& ctxt)
  {
  }

  static void 
  xlate_STDecl_error(xml::ostream& xos, ST *st, PUXlationContext& ctxt)
  {
    FORTTK_DIE("Unknown ST_CLASS " << ST_class(st));
  }

  static bool equivalencedToActive(ST* st,
				   PUXlationContext& ctxt) { 
    // find the symbols we are equivalence to:
    //    std::cout << "JU: equivalencedToActive: looking at " << ST_name(st) << std::endl;  
    TY_IDX baseTypeIndex = ST_type(ST_base(st));
    mUINT64 offset = ST_ofst(st); // offset into base symbol
    // find field with correct offset or symbol
    FLD_HANDLE fld = TY_Lookup_FLD(baseTypeIndex, 0, offset);
    ST_IDX fldStIdx=FLD_st(fld);
    //    std::cout << "\tJU: equivalencedToActive: looking at fld " << 1 <<  ":" << fld.Idx() << ":" << FLD_name(fld) << ":" << offset;  
    if (!fldStIdx) { 
      //      std::cout << "\tJU: equivalencedToActive: no FLD st"  << std::endl;
      ;
    }
    else {  
      //      std::cout << "\tJU: equivalencedToActive: have FLD st"  << std::endl;
      ST* fldSt_p=ST_ptr(fldStIdx);
      if (ctxt.isActiveSym(fldSt_p)) 
	return true;
    } 
    // if not the first one retrieve other fields with the same offset
    unsigned short eqInst=2;
    fld = TY_Lookup_FLD(baseTypeIndex, 0, offset,eqInst);
    while (!fld.Is_Null()) { 
      ST_IDX fldStIdx=FLD_st(fld);
      if (!fldStIdx) {  // this happens e.g. for local variable equivalenced to common block variable  
	//	std::cout << "\tJU: equivalencedToActive: skipping fld " << eqInst <<  ":" << fld.Idx() << ":" << FLD_name(fld) << ":" << offset << std::endl;  
	;
      }
      else { 
	ST* fldSt_p=ST_ptr(fldStIdx);
	//	std::cout << "\tJU: equivalencedToActive: looking at fld " << eqInst <<  ":" << fld.Idx() << ":" << ST_name(fldSt_p) << ":" << offset << std::endl;  
	if (ctxt.isActiveSym(fldSt_p)) 
	  return true;
      } 
      fld = TY_Lookup_FLD(baseTypeIndex, 0, offset,++eqInst);
    }
    return false; 
  } 

  static bool equivalencedToActiveBlock(ST* st,
					PUXlationContext& ctxt) { 
    // find what we are equivalence to:
    TY_IDX baseTypeIndex = ST_type(ST_base(st));
    mUINT64 offset = ST_ofst(st); // offset into base symbol
    // find field with correct offset or symbol
    FLD_HANDLE fld = TY_Lookup_FLD(baseTypeIndex, 0, offset);
    ST_IDX fldStIdx=FLD_st(fld);
    //    std::cout << "\tJU: equivalencedToActiveBlock: looking at fld " << 1 <<  ":" << fld.Idx() << ":" << FLD_name(fld) << ":" << offset;  
    if (!fldStIdx) { 
      //      std::cout << "\tJU: equivalencedToActiveBlock: no FLD st"  << std::endl;
    }
    else {  
      //      std::cout << "\tJU: equivalencedToActiveBlock: have FLD st"  << std::endl;
      ST* fldSt_p=ST_ptr(fldStIdx);
      if (ctxt.isActiveSym(fldSt_p)) 
	return true;
    } 
    return false; 
  } 

  static bool activeInCommon(ST* st,
			     PUXlationContext& ctxt) { 
    // see if there is anything else 
    // active in this common block 
    TY_IDX tyIdx = ST_type(ST_base(st));
    TY& ty = Ty_Table[tyIdx];
    FLD_HANDLE fldlist=TY_flist(ty);
    FLD_ITER fld_iter = Make_fld_iter(fldlist);
    do {
      FLD_HANDLE fld (fld_iter);
      ST_IDX fldStIdx=FLD_st(fld);
      //      std::cout << "\tJU: activeInCommon: looking at fld " << 1 <<  ":" << fld.Idx() << ":" << FLD_name(fld) << ":" << FLD_ofst(fld);  
      if (!fldStIdx) { 
	//	std::cout << "\tJU: activeInCommon: no FLD st"  << std::endl;
	;
      }
      else {  
	//	std::cout << "\tJU: activeInCommon: have FLD st"  << std::endl;
	ST* fldSt_p=ST_ptr(fldStIdx);
	if (ctxt.isActiveSym(fldSt_p)) 
	  return true; 
      }
    } while (!FLD_last_field (fld_iter++)) ;
    return false;
  }

  static void 
  xlate_STDecl_VAR(xml::ostream& xos, ST *st, PUXlationContext& ctxt) {  
    FORTTK_ASSERT(ST_class(st) == CLASS_VAR, fortTkSupport::Diagnostics::UnexpectedInput);
    const char* st_name = ST_name(st);
    ST* base = ST_base(st);
    TY_IDX ty = ST_type(st);
    bool translatenow = false;
    if (Stab_Is_Common_Block(st)) {
      TY2F_Translate_Common(xos, st_name, ty); 
    } 
    else if (ST_sclass(st) == SCLASS_FORMAL && !ST_is_value_parm(st)) {
      // A procedure parameter (we expect a pointer TY to counteract the
      // Fortran call-by-reference semantics)
      FORTTK_ASSERT((TY_Is_Pointer(ty)|| TY_is_f90_pointer(ty)), "Unexpected type " << TY_kind(ty) << " for " << st_name);
    
      TY_IDX base_ty = TY_pointed(ty);
      if (TY_Is_Pointer(base_ty) && TY_ptr_as_array(Ty_Table[base_ty])) {
	/* FIXME: Handle ptr as array parameters */ 
	ty = Stab_Array_Of(TY_pointed(base_ty), 0/*size*/);
      } 
      else {
	ty = base_ty;
      }
      translatenow = true;
    } 
    else {
      translatenow = true;
    }
    if (translatenow) { // FIXME
      bool isPointer = TY_Is_Pointer(ty) || TY_is_f90_pointer(ty);
      bool isFnPointer = isPointer && (TY_kind(TY_pointed(ty)) == KIND_FUNCTION);
      const char* ty_str = (isPointer && (!isFnPointer)) ? TranslateTYToSymType(TY_pointed(ty))
                                     : TranslateTYToSymType(ty);
      if (!ty_str) { ty_str = "***"; }
      const char* shape_str = (isPointer && (!isFnPointer)) ? TranslateTYToSymShape(TY_pointed(ty))
                                        : TranslateTYToSymShape(ty);
      if (!shape_str) { shape_str = "***"; }
      int active = (ctxt.isActiveSym(st)) ? 1 : 0;
      if (Args::ourUniformCBactFlag && !active && Stab_Is_Valid_Base(st)) { 
	if (ST_is_equivalenced(st)) { 
	  active=equivalencedToActive(st,ctxt);
	} 
	if (!active && Stab_Is_Equivalence_Block(ST_base(st))) {
	  active=equivalencedToActiveBlock(st,ctxt);
	}
	if (!active && Stab_Is_Common_Block(ST_base(st))) {
	  active=activeInCommon(st,ctxt);
	}
	if (active) { 
	  if ((strcmp(ty_str, "integer") == 0
	       ||
	       strcmp(ty_str, "string") == 0)) { 
	    static const char* txt1 = "cannot activate equivalenced of common block symbol >";
	    static const char* txt2 = "< of type ";
	    if (CURRENT_SYMTAB == GLOBAL_SYMTAB) {
	      FORTTK_MSG(0, "warning: within global scope: " << txt1 << ST_name(st) << txt2 << ty_str);
	    }
	    else {
	      ST_IDX pu_st = PU_Info_proc_sym(Current_PU_Info);
	      FORTTK_MSG(0, "warning: within " << ST_name(pu_st) << ": " << txt1 << ST_name(st) << txt2  << ty_str);
	    }
	  }
	  else {
	    static const char* txt1 = "activating symbol >";
	    static const char* txt2 = "< of type ";
	    if (CURRENT_SYMTAB == GLOBAL_SYMTAB) {
	      FORTTK_MSG(1, "within global scope: " << txt1 << ST_name(st) << txt2 << ty_str);
	    }
	    else {
	      ST_IDX pu_st = PU_Info_proc_sym(Current_PU_Info);
	      FORTTK_MSG(1, "within " << ST_name(pu_st) << ": " << txt1 << ST_name(st) << txt2  << ty_str);
	    }
	  }
	}
      } 
      if (Args::ourAllActiveFlag 
	  && 
	  !active 
	  && 
	  ((strcmp(ty_str, "real") == 0
	    ||
	    strcmp(ty_str, "complex") == 0))
	  &&  
	  !(ST_sclass(st) == SCLASS_PSTATIC 
	    &&
	    ST_is_initialized(st))
	  ) {
	active=true;
      }
      if (active 
	  && 
	  (strcmp(ty_str, "integer") == 0
	   ||
	   strcmp(ty_str, "string") == 0)) {
	active = false;
	static const char* txt1 = "deactivating symbol >";
	static const char* txt2 = "< of type ";
	if (CURRENT_SYMTAB == GLOBAL_SYMTAB) {
	  FORTTK_MSG(0, "warning: within global scope: " << txt1 << ST_name(st) << txt2 << ty_str);
	}
	else {
	  ST_IDX pu_st = PU_Info_proc_sym(Current_PU_Info);
	  FORTTK_MSG(0, "warning: within " << ST_name(pu_st) << ": " << txt1 << ST_name(st) << txt2  << ty_str);
	}
      }
      if (active 
	  && 
	  (ST_sclass(st) == SCLASS_PSTATIC 
	    &&
	   ST_is_initialized(st))) { 
	static const char* txt1 = "explicit initialization of active symbol >";
	static const char* txt2 = "< implies the active type may not use default initialization";
	if (CURRENT_SYMTAB == GLOBAL_SYMTAB) {
	  FORTTK_MSG(0, "warning: within global scope: " << txt1 << ST_name(st) << txt2 );
	}
	else {
	  ST_IDX pu_st = PU_Info_proc_sym(Current_PU_Info);
	  FORTTK_MSG(0, "warning: within " << ST_name(pu_st) << ": " << txt1 << ST_name(st) << txt2 );
	}
      }
      fortTkSupport::SymId st_id = (fortTkSupport::SymId)ST_index(st);

      xos << xml::BegElem("xaif:Symbol") << AttrSymId(st);
      if (isFnPointer) {
	xos << xml::Attr("kind", "pointer");
      } 
      else {
	xos << xml::Attr("kind", "variable");
      }
      xos << xml::Attr("type", ty_str)
	  << xml::Attr("feType",TranslateTYToMType(ty))
	  << xml::Attr("shape", shape_str) 
	  << SymIdAnnot(st_id)
	  << xml::Attr("active", active);
      if (isPointer) {
	xos << xml::Attr("pointer", isPointer);
      }
      xos << xml::EndAttrs;
      xlate_ArrayBounds(xos, ty, ctxt);
      xos << xml::EndElem;
    }
  }

  static void 
  xlate_STDecl_FUNC(xml::ostream& xos, ST* st, PUXlationContext& ctxt)
  {
    // This only makes sense for "external" functions in Fortran,
    // while we should not do anything for other functions.
    FORTTK_ASSERT(ST_class(st) == CLASS_FUNC, fortTkSupport::Diagnostics::UnexpectedInput);

    fortTkSupport::SymId st_id = (fortTkSupport::SymId)ST_index(st);
    xos << xml::BegElem("xaif:Symbol") << AttrSymId(st)
	<< xml::Attr("kind", "subroutine") << xml::Attr("type", "void")
	<< SymIdAnnot(st_id) << xml::EndElem;

  }

  static void 
  xlate_STDecl_CONST(xml::ostream& xos, ST *st, PUXlationContext& ctxt)
  {
    //xos << BegComment << "const id=" << (UINT)ST_index(st) << EndComment;
  }

  static void 
  xlate_STDecl_PREG(xml::ostream& xos, ST *st, PUXlationContext& ctxt)
  {
    TY_IDX ty = ST_type(st);
    const char* ty_str = TranslateTYToSymType(ty);
    if (!ty_str) { 
      return; // skip [FIXME -- better hope this is not used!]
    }
  
    fortTkSupport::SymId st_id = (fortTkSupport::SymId)ST_index(st);
    xos << xml::BegElem("xaif:Symbol") << AttrSymId(st)
	<< xml::Attr("kind", "variable") << xml::Attr("type", ty_str)
	<< xml::Attr("shape", "scalar") << SymIdAnnot(st_id)
	<< xml::Attr("active", 0) << xml::EndElem;
  }

  static void 
  xlate_STDecl_BLOCK(xml::ostream& xos, ST *st, PUXlationContext& ctxt)
  {
    //xos << BegComment << "block id=" << (UINT)ST_index(st) << EndComment;
  }

  static void 
  xlate_STDecl_NAME(xml::ostream& xos, ST *st, PUXlationContext& ctxt)
  {
    //xos << BegComment << "name id=" << (UINT)ST_index(st) << EndComment;
  }

  static void 
  xlate_STDecl_TYPE(xml::ostream& xos, ST *st, PUXlationContext& ctxt)
  {
    FORTTK_ASSERT(ST_class(st) == CLASS_TYPE, fortTkSupport::Diagnostics::UnexpectedInput);

    const char  *st_name = ST_name(st);
    TY_IDX       ty_rt = ST_type(st);
 
#if 0 // FIXME 
    xos << BegComment << "type id=" << (UINT)ST_index(st) << EndComment; 
    TY2F_translate(xos, ST_type(st), 1, ctxt);
#endif
  }

  // ***************************************************************************
  // 
  // ***************************************************************************

  static void 
  xlate_STUse_error(xml::ostream& xos, ST *st, PUXlationContext& ctxt)
  {
    FORTTK_DIE("Unknown ST_CLASS " << ST_class(st));
  }

  static void 
  xlate_STUse_VAR(xml::ostream& xos, ST *st, PUXlationContext& ctxt)
  {
    FORTTK_ASSERT(ST_class(st) == CLASS_VAR, fortTkSupport::Diagnostics::UnexpectedInput);

    // Note: for functions, check that st is a return var using
    //   ST_is_return_var(st)) (cf. whirl2f)

#if 0 // FIXME xlate_SymRef moves from 'base' to 'field' (cannot reciprocate)
    if (Stab_Is_Based_At_Common_Or_Equivalence(st)) {
      /* Reference the corresponding field in the common block (we do this
       * only to ensure that the name referenced matches the one used for
       * the member of the common-block at the place of declaration).  Note
       * that will full splitting, the original common block can be found
       * at ST_full(ST_base(st)).
       */
      xlate_SymRef(xos, ST_base(st) /*base-symbol*/,
		   Stab_Pointer_To(ST_type(ST_base(st))), /*base-type*/
		   ST_type(st) /*object-type*/, 
		   ST_ofst(st) /*object-ofst*/, ctxt);
    }
    //else {
#endif
  
    // FIXME: abstract
    ST_TAB* sttab = Scope_tab[ST_level(st)].st_tab;
   fortTkSupport::SymTabId scopeid = ctxt.findSymTabId(sttab);
  
    xos << xml::BegElem("xaif:SymbolReference") 
	<< xml::Attr("vertex_id", ctxt.currentXlationContext().getNewVertexId())
	<< xml::Attr("scope_id", scopeid) << AttrSymId(st) << xml::EndElem;
  }


  static void 
  xlate_STUse_CONST(xml::ostream& xos, ST *st, PUXlationContext& ctxt)
  {
    FORTTK_ASSERT(ST_class(st) == CLASS_CONST, fortTkSupport::Diagnostics::UnexpectedInput);
  
    // A CLASS_CONST symbol never has a name, so just emit the value.
    TY_IDX ty_idx = ST_type(st);
    TY& ty = Ty_Table[ty_idx];
  
    std::string val;
    if (TY_mtype(ty) == MTYPE_STR && TY_align(ty_idx) > 1) {
      val = TCON2F_hollerith(STC_val(st)); // must be a hollerith constant
    } else {
      val = TCON2F_translate(STC_val(st), TY_is_logical(ty));
    }
  
    const char* ty_str = TranslateTYToSymType(ty_idx);
    if (!ty_str) { ty_str = "***"; }  

    xos << xml::BegElem("xaif:Constant") 
	<< xml::Attr("vertex_id", ctxt.currentXlationContext().getNewVertexId()) 
	<< xml::Attr("type", ty_str) 
	<< xml::Attr("feType",TranslateTYToMType(ty_idx))
	<< xml::Attr("value", val) << xml::EndElem;
  }


  static void 
  xlate_STUse_BLOCK(xml::ostream& xos, ST *st, PUXlationContext& ctxt)
  {
    /* with f90 at -O2, CLASS_BLOCK can appear on LDAs etc. in IO */
    /* put out something, so whirlbrowser doesn't fall over       */
    FORTTK_ASSERT(ST_class(st) == CLASS_BLOCK, fortTkSupport::Diagnostics::UnexpectedInput);
  
    xos << xml::BegElem("***use_block") << xml::Attr("id", ctxt.currentXlationContext().getNewVertexId()) 
	<< xml::Attr("_type", -1) << xml::Attr("value", ST_name(st)) << xml::EndElem;
  } 


  // ***************************************************************************
  // 
  // ***************************************************************************

  /*------------------------ exported routines --------------------------*/

  void 
  ST2F_deref_translate(xml::ostream& xos, ST *st, PUXlationContext& ctxt)
  {
    FORTTK_ASSERT(ST_sym_class(st)==CLASS_VAR && 
		  TY_Is_Pointer(ST_type(st)) &&
		  !Stab_Is_Based_At_Common_Or_Equivalence(st), 
		  fortTkSupport::Diagnostics::UnexpectedInput << ST_class(st));
  
    /* reference to the pointer value; cf. W2CF_Symtab_Nameof_St_Pointee */
    xos << "{deref***} " << "deref_" << ST_name(st);
  }


  void 
  ST2F_Declare_Tempvar(TY_IDX ty, UINT idx)
  {
#if 0 // FIXME
    xml::ostream& tmp_tokens = New_Token_Buffer();
    UINT         current_indent = 0; //FIXME Current_Indentation();

    Append_F77_Indented_Newline(PUinfo_local_decls, 1, NULL/*label*/);
    if (TY_Is_Pointer(ty))
      {
	/* Assume we never need to dereference the pointer, or else we
	 * need to maintain a map from tmp_idx->pointee_idx (new temporary
	 * for pointee_idx), so declare this temporary variable to be of
	 * an integral type suitable for a pointer value.
	 */
	ty = Stab_Mtype_To_Ty(Pointer_Mtype);
      }
    tmp_tokens << "tmp" << idx; /* name */
    TY2F_translate(tmp_tokens, ty);                                   /* type */
    if (ST_is_in_module(Scope_tab[Current_scope].st) &&
	!PU_is_nested_func(Pu_Table[ST_pu(Scope_tab[Current_scope].st)]))
      {
	Append_F77_Indented_Newline(tmp_tokens, 1, NULL/*label*/);
	Append_Token_String(tmp_tokens,"PRIVATE ");
	tmp_tokens << "tmp" << idx;
      }

    Append_And_Reclaim_Token_List(PUinfo_local_decls, &tmp_tokens);
    Set_Current_Indentation(current_indent);
#endif
  } /* ST2F_Declare_Tempvar */


  static BOOL
  ST2F_Is_Dummy_Procedure(ST *st)
  {
    /* Does this ST represent a dummy procedure ? */
    BOOL dummy = FALSE;
    if (ST_sclass(st) == SCLASS_FORMAL && ST_is_value_parm(st)) {
      TY_IDX ty = ST_type(st);
      if (TY_kind(ty) == KIND_POINTER
	  && TY_kind(TY_pointed(ty)) == KIND_FUNCTION) {
	dummy = TRUE;
      }
    }
    return dummy;
  }


  static void
  ST2F_Declare_Return_Type(xml::ostream& xos, TY_IDX return_ty, 
			   PUXlationContext& ctxt)
  {
    // The TY represents a dummy procedure or a function return type
    if ( (return_ty != (TY_IDX)0) && (TY_kind(return_ty) != KIND_VOID) ) {
      /* Use integral type for pointer returns */
      if (TY_Is_Pointer(return_ty))
	TY2F_translate(xos, Stab_Mtype_To_Ty(TY_mtype(return_ty)), ctxt);
      else
	TY2F_translate(xos, return_ty, ctxt);
    }
  }



}
