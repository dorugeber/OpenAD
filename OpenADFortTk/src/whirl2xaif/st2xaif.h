// ##########################################################
// # This file is part of OpenADFortTk.                     #
// # The full COPYRIGHT notice can be found in the top      #
// # level directory of the OpenADFortTk source tree.       #
// # For more information visit                             #
// # http://www.mcs.anl.gov/openad                          #
// ##########################################################

#ifndef st2xaif_INCLUDED
#define st2xaif_INCLUDED
/* ====================================================================
 * ====================================================================
 *
 * Description:
 *
 *    TranslateSTUse:
 *       Translate a variable reference.
 *
 *    TranslateSTDecl:
 *       Translate a variable declaration
 *
 *    ST2F_deref_translate:
 *       Translate a pointer variable to its dereferenced version.
 *
 *    ST2F_Declare_Tempvar:
 *       Declares a tempvar with the given index in the local scope.
 *
 *    ST2F_FIRST_PARAM_IDX:
 *       An index to the first parameter to be explicitly declared in
 *       Fortran, given the type of the function and the type of the 
 *       first (implicit or explicit) argument.
 *
 * ====================================================================
 * ====================================================================
 */
#include <string>

#include "Open64IRInterface/Open64BasicTypes.h"

#include <xmlostream.h>
#include "PUXlationContext.h"

namespace whirl2xaif {

  extern void 
  xlate_SymbolTables(xml::ostream& xos, SYMTAB_IDX symtab_lvl, 
		     fortTkSupport::ScalarizedRefTab_W2X* nonscalarsymtab, 
		     PUXlationContext& ctxt);
  
  extern void 
  xlate_SYMTAB(xml::ostream& xos, 
	       SYMTAB_IDX symtab_lvl, 
	       PUXlationContext& ctxt);
  
  extern void 
  xlate_ScalarizedRefTab(xml::ostream& xos, 
			 fortTkSupport::ScalarizedRefTab_W2X* symtab, 
			 PUXlationContext& ctxt);
  
  extern void 
  xlate_ArrayBounds(xml::ostream& xos, 
		    TY_IDX ty_idx, 
		    PUXlationContext& ctxt);

  extern void 
  TranslateSTDecl(xml::ostream& xos, ST* st, PUXlationContext& ctxt);
  
  extern void 
  TranslateSTUse(xml::ostream& xos, ST* st, PUXlationContext& ctxt);
  

// FIXME/REMOVE
extern std::string 
TCON2F_hollerith(TCON tvalue);

extern std::string 
TCON2F_translate(TCON tvalue, BOOL is_logical, TY_IDX object_ty);

extern std::string 
TCON2F_translate(TCON tvalue, BOOL is_logical);


extern void 
ST2F_deref_translate(xml::ostream& xos, ST* st, PUXlationContext& ctxt);

extern void 
ST2F_Declare_Tempvar(TY_IDX ty, UINT idx);

//-------------------------------------

// FIXME: see: wn_attr.h WN_Call_First_Arg_Idx
#define ST2F_FIRST_PARAM_IDX(funtype) \
   (Func_Return_To_Param(funtype)? \
    (Func_Return_Character(funtype)? 2 : 1) : 0)

#endif /* st2xaif_INCLUDED */

}; /* namespace whirl2xaif */



