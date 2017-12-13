// ##########################################################
// # This file is part of OpenADFortTk.                     #
// # The full COPYRIGHT notice can be found in the top      #
// # level directory of the OpenADFortTk source tree.       #
// # For more information visit                             #
// # http://www.mcs.anl.gov/openad                          #
// ##########################################################

#ifndef wn2xaif_expr_INCLUDED
#define wn2xaif_expr_INCLUDED

#include "Open64IRInterface/Open64BasicTypes.h"

#include "whirl2xaif.h"
#include "PUXlationContext.h"

//***************************************************************************
// Type conversion
//***************************************************************************

namespace whirl2xaif {

  extern void 
  WN2F_cvt(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);
  
  extern void 
  WN2F_cvtl(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);
  
  extern void 
  WN2F_tas(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);

}; /* namespace whirl2xaif */

//***************************************************************************
// Leaf (Other)
//***************************************************************************

namespace whirl2xaif {
  
  extern void 
  xlate_CONST(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);
  
  extern void 
  xlate_INTCONST(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);
  
}; /* namespace whirl2xaif */

//***************************************************************************
// Expression Operators: Unary Operations
//***************************************************************************

namespace whirl2xaif {

  extern void 
  xlate_UnaryOp(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);

  extern void 
  WN2F_rsqrt(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);

  extern void 
  WN2F_realpart(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);
  
  extern void 
  WN2F_imagpart(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);
  
  extern void 
  xlate_PAREN(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);
  
  extern void 
  xlate_RECIP(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);

  extern void 
  WN2F_parm(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);

  extern void 
  WN2F_alloca(xml::ostream& xos, WN *wn, PUXlationContext& ctxt) ;
  
}; /* namespace whirl2xaif */

//***************************************************************************
// Expression Operators: Binary Operations
//***************************************************************************

namespace whirl2xaif {
  
  extern void 
  xlate_BinaryOp(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);
  
  extern void 
  WN2F_complex(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);
  
  extern void 
  WN2F_bnor(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);
  
  extern void 
  WN2F_lshr(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);

}; /* namespace whirl2xaif */

//***************************************************************************
// Expression Operators: Ternary Operations
//***************************************************************************

namespace whirl2xaif {

  extern void 
  WN2F_select(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);
  
  extern void 
  WN2F_madd(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);
  
  extern void 
  WN2F_msub(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);
  
  extern void 
  WN2F_nmadd(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);
  
  extern void 
  WN2F_nmsub(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);
  
}; /* namespace whirl2xaif */

//***************************************************************************
// Expression Operators: N-ary Operations
//***************************************************************************

namespace whirl2xaif {

  extern void 
  xlate_INTRINSIC_OP(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);

  // Array expressions included in memory operations

}; /* namespace whirl2xaif */

//***************************************************************************

#endif 

