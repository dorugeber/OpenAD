// ##########################################################
// # This file is part of OpenADFortTk.                     #
// # The full COPYRIGHT notice can be found in the top      #
// # level directory of the OpenADFortTk source tree.       #
// # For more information visit                             #
// # http://www.mcs.anl.gov/openad                          #
// ##########################################################

#ifndef wn2xaif_stmt_INCLUDED
#define wn2xaif_stmt_INCLUDED

#include "Open64IRInterface/Open64BasicTypes.h"

#include "whirl2xaif.h"
#include "PUXlationContext.h"

//***************************************************************************
// Passive Statements
//***************************************************************************

namespace whirl2xaif {

  extern void 
  xlate_PassiveStmt(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);

}; /* namespace whirl2xaif */


//***************************************************************************
// Structured Control Flow Statements
//***************************************************************************

namespace whirl2xaif {
  
  extern void 
  xlate_BLOCK(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);
  
  extern void 
  WN2F_region(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);
  
  extern void 
  xlate_DO_LOOP(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);
  
  extern void 
  xlate_DO_WHILE(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);
  
  extern void 
  xlate_WHILE_DO(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);
  
  extern void 
  xlate_IF(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);
  
}; /* namespace whirl2xaif */


//***************************************************************************
// Unstructured Control Flow Statements
//***************************************************************************

namespace whirl2xaif {

  extern void 
  WN2F_implied_do(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);

  extern void
  WN2F_noio_implied_do(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);
  
  extern void 
  xlate_GOTO(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);
  
  extern void
  xlate_SWITCH(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);
  
  extern void
  WN2F_casegoto(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);
  
  extern void 
  WN2F_compgoto(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);
  
  extern void 
  WN2F_agoto(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);
  
  extern void 
  xlate_condBR(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);
  
  extern void 
  xlate_RETURN(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);
  
  extern void 
  xlate_RETURN_VAL(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);
  
  extern void 
  xlate_LABEL(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);
  
}; /* namespace whirl2xaif */


//***************************************************************************
// Calls
//***************************************************************************

namespace whirl2xaif {

  extern void 
  xlate_CALL(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);
  
  extern void 
  xlate_INTRINSIC_CALL(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);
  
}; /* namespace whirl2xaif */


//***************************************************************************
// Other Statements
//***************************************************************************

namespace whirl2xaif {

  extern void 
  WN2F_eval(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);
  
  extern void 
  xlate_PRAGMA(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);
  
  extern void 
  xlate_PREFETCH(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);
  
  extern void 
  xlate_COMMENT(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);

  extern void 
  WN2F_dealloca(xml::ostream& xos, WN *wn, PUXlationContext& ctxt) ;
  
  
  extern void
  xlate_USE(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);  

  extern void
  WN2F_namelist_stmt(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);  
  
  extern void
  WN2F_implicit_bnd(xml::ostream& xos, WN *wn, PUXlationContext& ctxt); 
  
  extern void
  WN2F_nullify_stmt(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);

  extern void
  WN2F_interface_blk(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);
  
  extern void
  WN2F_ar_construct(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);
  
}; /* namespace whirl2xaif */

//***************************************************************************

#endif /* wn2xaif_stmt_INCLUDED */
