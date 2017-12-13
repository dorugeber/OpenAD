// -*-Mode: C++;-*-
// $Header: /m_home/m_utkej/Argonne/cvs2svn/cvs/OpenADFortTk/src/whirl2xaif/wn2xaif_mem.h,v 1.9 2007-10-08 18:28:35 utke Exp $

#ifndef wn2xaif_mem_INCLUDED
#define wn2xaif_mem_INCLUDED

#include "Open64IRInterface/Open64BasicTypes.h"

#include "whirl2xaif.h"
#include "PUXlationContext.h"

// ***************************************************************************
// Memory Access (or assignment and variable references)
// ***************************************************************************

namespace whirl2xaif {

  // Loads (variable reference)
  extern void 
  xlate_LDA(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);
  
  extern void 
  xlate_LDID(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);
  
  extern void 
  xlate_ILOAD(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);

  extern void 
  xlate_ILOADX(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);
  
  extern void 
  WN2F_mload(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);
  
  
  // Stores (assignment)
  extern void 
  xlate_STID(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);
  
  extern void 
  xlate_ISTORE(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);

  extern void
  xlate_ISTOREX(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);

  extern void 
  WN2F_mstore(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);

  
  extern void
  WN2F_pstid(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);

  extern void
  WN2F_pstore(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);
  

  // ***************************************************************************
  // Array Operators (N-ary Operations)
  // ***************************************************************************

  
  extern void
  xlate_ARRAY(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);

  extern void
  WN2F_triplet(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);
  
  extern void
  WN2F_src_triplet(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);

  extern void
  WN2F_arrayexp(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);

  extern void
  WN2F_arrsection(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);

  extern void
  WN2F_where(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);
  

  // ***************************************************************************

  // FIXME
  extern void 
  WN2F_array_bounds(xml::ostream& xos, WN *wn, TY_IDX array_ty,
		    PUXlationContext& ctxt);
  extern void 
  WN2F_arrsection_bounds(xml::ostream& xos, WN *wn, TY_IDX array_ty,
			 PUXlationContext& ctxt);

  extern void 
  WN2F_String_Argument(xml::ostream&  tokens, WN *base, WN *length,
		       PUXlationContext& ctxt);

  extern BOOL 
  WN2F_Is_Address_Preg(WN * wn ,TY_IDX ptr_ty) ;

  // ***************************************************************************

}; /* namespace whirl2xaif */

#endif /* wn2xaif_mem_INCLUDED */

