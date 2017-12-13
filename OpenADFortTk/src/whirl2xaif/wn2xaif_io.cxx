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
 * Description:
 *
 *   Translate an OPC_IO subtree into the appropriate Fortran constructs.
 *   Recursive translation of WN nodes should only use WN2F_Translate(),
 *   with exception of IO_ITEMS which are handled locally!
 *
 *   The Fortran I/O statements have the following syntax:
 *
 *      <statement_keyword> <control_list> <I/O list>
 *
 *   where
 *
 *      <statement_keyword> ::= <IOS enumeration as defined in wio.h>
 *      <control_list>      ::= <IOU, IOF, and IOC items from wio.h>
 *      <I/O list>          ::= <IOL items from wio.h>
 *
 * ====================================================================
 * ====================================================================
 */

#include "Open64IRInterface/Open64BasicTypes.h"

#include "wn2xaif.h"
#include "wn2xaif_mem.h"
#include "wn2xaif_io.h"
#include "st2xaif.h"
#include "ty2xaif.h"

using namespace whirl2xaif;
using namespace xml; // for xml::ostream, etc

// xlate_IO_IOS_BACKSPACE
static void 
WN2F_ios_backspace(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);

static void 
WN2F_ios_close(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);

static void 
WN2F_ios_definefile(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);

static void 
WN2F_ios_delete(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);

static void 
WN2F_ios_endfile(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);

static void 
WN2F_ios_find(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);

static void 
WN2F_ios_inquire(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);

static void 
WN2F_ios_namelist(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);

static void 
WN2F_ios_open(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);

static void 
WN2F_ios_rewind(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);

static void 
WN2F_ios_unlock(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);

static void 
WN2F_ios_accept(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);

static void
WN2F_ios_decode(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);

static void 
WN2F_ios_encode(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);

static void 
WN2F_ios_print(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);

static void 
WN2F_ios_read(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);

static void 
WN2F_ios_rewrite(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);

static void 
WN2F_ios_type(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);

static void 
WN2F_ios_write(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);

static void 
WN2F_ios_cr(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);


static BOOL
xlate_IO_ITEM_unit(xml::ostream& xos, WN *item, PUXlationContext& ctxt);

static BOOL
xlate_IO_ITEM_format(xml::ostream& xos, WN* item, PUXlationContext& ctxt);

static BOOL
xlate_IO_ITEM_control(xml::ostream& xos, WN* item, PUXlationContext& ctxt);

static BOOL 
xlate_IO_ITEM_list(xml::ostream& xos, WN *item, PUXlationContext& ctxt);

// ************************** Forward Declarations ***************************

// FIXME: REMOVE
/* A rather special IOC item to replace an IOF_LABEL item.  This
 * value is only valid when ORIGFMT_IOCTRL is TRUE! */
static UINT32       Origfmt_Ioctrl_Label;

// ***************************************************************************

typedef void (*XlateWNHandlerFunc)(xml::ostream&, WN *, PUXlationContext&);
static XlateWNHandlerFunc XlateWNio_HandlerTable[IOSTATEMENT_LAST+1];
static bool HandlerTableInitialized = false;

static void 
WN2F_Io_initialize(void)
{
  XlateWNio_HandlerTable[IOS_BACKSPACE] = &WN2F_ios_backspace;
  XlateWNio_HandlerTable[IOS_CLOSE] = &WN2F_ios_close;
  XlateWNio_HandlerTable[IOS_DEFINEFILE] = &WN2F_ios_definefile;
  XlateWNio_HandlerTable[IOS_DELETE] = &WN2F_ios_delete;
  XlateWNio_HandlerTable[IOS_ENDFILE] = &WN2F_ios_endfile;
  XlateWNio_HandlerTable[IOS_FIND] = &WN2F_ios_find;
  XlateWNio_HandlerTable[IOS_INQUIRE] = &WN2F_ios_inquire;
  XlateWNio_HandlerTable[IOS_NAMELIST] = &WN2F_ios_namelist;
  XlateWNio_HandlerTable[IOS_OPEN] = &WN2F_ios_open;
  XlateWNio_HandlerTable[IOS_REWIND] = &WN2F_ios_rewind;
  XlateWNio_HandlerTable[IOS_UNLOCK] = &WN2F_ios_unlock;
  XlateWNio_HandlerTable[IOS_ACCEPT] = &WN2F_ios_accept;
  XlateWNio_HandlerTable[IOS_DECODE] = &WN2F_ios_decode;
  XlateWNio_HandlerTable[IOS_ENCODE] = &WN2F_ios_encode;
  XlateWNio_HandlerTable[IOS_PRINT] = &WN2F_ios_print;
  XlateWNio_HandlerTable[IOS_READ] = &WN2F_ios_read;
  XlateWNio_HandlerTable[IOS_REWRITE] = &WN2F_ios_rewrite;
  XlateWNio_HandlerTable[IOS_TYPE] = &WN2F_ios_type;
  XlateWNio_HandlerTable[IOS_WRITE] = &WN2F_ios_write;
  XlateWNio_HandlerTable[IOS_CR_FWF]   = &WN2F_ios_cr;
  XlateWNio_HandlerTable[IOS_CR_FRN]   = &WN2F_ios_read;  
  XlateWNio_HandlerTable[IOS_CR_FWN]   = &WN2F_ios_write;  
  XlateWNio_HandlerTable[IOS_CR_FWU]   = &WN2F_ios_cr;
  XlateWNio_HandlerTable[IOS_CR_FRF]   = &WN2F_ios_cr;
  XlateWNio_HandlerTable[IOS_CR_FRU]   = &WN2F_ios_cr;
  XlateWNio_HandlerTable[IOS_CR_OPEN]  = &WN2F_ios_open;
  XlateWNio_HandlerTable[IOS_CR_CLOSE] = &WN2F_ios_close;
  XlateWNio_HandlerTable[IOS_CR_REWIND] = &WN2F_ios_rewind;
  XlateWNio_HandlerTable[IOS_CR_INQUIRE] = &WN2F_ios_inquire;
  XlateWNio_HandlerTable[IOS_CR_ENDFILE] = &WN2F_ios_endfile;
  XlateWNio_HandlerTable[IOS_CR_BACKSPACE] = &WN2F_ios_backspace;
  
} /* WN2F_Io_initialize */


static BOOL
Is_Cray_IO(IOSTATEMENT ios)
{
  BOOL res ;
  res = (ios == IOS_CR_FWF) || (ios == IOS_CR_FWU) || (ios == IOS_CR_FRF) 
    || (ios == IOS_CR_FRU) || (ios == IOS_CR_OPEN) || (ios == IOS_CR_CLOSE)
    || (ios == IOS_CR_REWIND) || (ios == IOS_CR_INQUIRE) 
    || (ios == IOS_CR_ENDFILE) || (ios == IOS_CR_FRN) || (ios == IOS_CR_FWN)
    || (ios == IOS_CR_BACKSPACE);
  return res ;
}

// ***************************************************************************

void
whirl2xaif::xlate_IO(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  FORTTK_ASSERT(WN_operator(wn) == OPR_IO, fortTkSupport::Diagnostics::UnexpectedInput);

  static UINT32 unique_label = 99999U;
  
  // Initialize table on demand for now
  if (!HandlerTableInitialized) { // FIXME
    WN2F_Io_initialize();
    HandlerTableInitialized = true;
  }
  
  // FIXME Should we use the string given by an IOC_VARFMT_ORIGFMT for a
  // IOF_LABEL?
  INT ios_kid = 0;
  for ( ; (ios_kid < WN_kid_count(wn) &&
	   WN_io_item(WN_kid(wn, ios_kid)) != IOC_VARFMT_ORIGFMT); 
	ios_kid++);
  if (ios_kid < WN_kid_count(wn)) {
    ctxt.currentXlationContext().setFlag(XlationContext::ORIGFMT_IOCTRL);
    Origfmt_Ioctrl_Label = unique_label--; // cf. W2CF_Symtab_Unique_Label
  }
  
  /* FIXME Now dispatch to the appropriate handler routine for each kind of
   * IO statement, after beginning the statement on a new line and
   * setting the appropriate ctxt flags.  */
  ctxt.currentXlationContext().setFlag(XlationContext::IO_STMT);
  //ctxt.currentXlationContext().setFlag(no_newline);
  
  const IOSTATEMENT ios = WN_IOSTMT(wn);
  FORTTK_ASSERT(XlateWNio_HandlerTable[ios] != NULL,
		fortTkSupport::Diagnostics::UnexpectedOpr << get_iostatement_name(ios));
  
  if (Is_Cray_IO(ios)) 
    ctxt.currentXlationContext().setFlag(XlationContext::CRAY_IO);
  XlateWNio_HandlerTable[ios](xos, wn, ctxt);
  ctxt.currentXlationContext().unsetFlag(XlationContext::CRAY_IO);
  
}


BOOL
whirl2xaif::xlate_IO_ITEM(xml::ostream& xos, WN* item, PUXlationContext& ctxt)
{
  // Returns TRUE when something (anything) was emitted for this item.
  BOOL emitted = FALSE;

  /* Any pointer-argument to an io-item should be dereferenced.  Most
   * notably, this applies for an LDA of a character string in a FMT
   * specifier, but it also applies for a variety of other arguments.
   * Such dereferences are specified within the xlate_IO routines.
   */
  if (ctxt.currentXlationContext().isFlag(XlationContext::DEREF_IO_ITEM))
    ctxt.currentXlationContext().setFlag(XlationContext::DEREF_ADDR);
  else
    ctxt.currentXlationContext().unsetFlag(XlationContext::DEREF_ADDR);
  
  if (IS_IO_ITEM_IOU(item))
    emitted = xlate_IO_ITEM_unit(xos, item, ctxt);
  else if (IS_IO_ITEM_IOF(item))
    emitted = xlate_IO_ITEM_format(xos, item, ctxt);
  else if (IS_IO_ITEM_IOC(item))
    emitted = xlate_IO_ITEM_control(xos, item, ctxt);
  else if (IS_IO_ITEM_IOL(item))
    emitted = xlate_IO_ITEM_list(xos, item, ctxt);
  else 
    FORTTK_DIE(fortTkSupport::Diagnostics::UnexpectedOpr 
	       << (IOITEM_name(WN_IOITEM(item)) != NULL ? 
		   IOITEM_name(WN_IOITEM(item)) : "unknown_name")); 
  
  return emitted;
} /* xlate_IO_ITEM */


// ***************************************************************************
// 
// ***************************************************************************

// FIXME: merge with next function
static void 
xlate_IOControlList(xml::ostream& xos, WN *ios,
		    INT from_kid, INT to_kid, PUXlationContext& ctxt)
{
  // Emit an IO control list (IOU, IOF, and IOC)
  for (INT ios_kid = from_kid; ios_kid <= to_kid; ++ios_kid) {
    xos << BegElem("xaif:Argument") << Attr("position", ios_kid+1); //FIXME
    ctxt.createXlationContext();
    xlate_IO_ITEM(xos, WN_kid(ios, ios_kid), ctxt);
    ctxt.deleteXlationContext();
    xos << EndElem;
  }
}

static void 
xlate_IOList(xml::ostream& xos, WN *ios, INT from_kid, PUXlationContext& ctxt)
{
  /* Emit an IOL list, starting at the given kid index and
   * continuing to the last kid. */
  for (INT ios_kid = from_kid; ios_kid < WN_kid_count(ios); ++ios_kid) {
    xos << BegElem("xaif:Argument") << Attr("position", ios_kid+1); // FIXME
    ctxt.createXlationContext();
    xlate_IO_ITEM(xos, WN_kid(ios, ios_kid), ctxt);
    ctxt.deleteXlationContext();
    xos << EndElem;
  }
}


/*---------------------- IO Item Handler-routines ---------------------*/
/*---------------------------------------------------------------------*/

static BOOL
xlate_IO_ITEM_unit(xml::ostream& xos, WN *item, PUXlationContext& ctxt)
{
  /* Any arguments assumed to be by reference unless otherwise noted */
  ctxt.currentXlationContext().setFlag(XlationContext::DEREF_ADDR);
  
  switch (WN_io_item(item)) {
  case IOU_NONE: 
    xos << Comment("unit=*");
    break;
    
  case IOU_DEFAULT: /* asterisk or double astrisk */
    xos << BegComment << "unit=" 
	<< ((WN_const_val(WN_kid0(item)) == 0) ? "**" : "*") << EndComment;
    break;
    
  case IOU_EXTERNAL:    /* unit number */
  case IOU_DOPE:
  case IOU_INTERNAL:    /* substring or array reference */
    xos << "unit***=";
    TranslateWN(xos, WN_kid0(item), ctxt); // FIXME
    break;
    
  default:
    FORTTK_DIE(fortTkSupport::Diagnostics::UnexpectedOpr << IOITEM_name(WN_IOITEM(item)));
    xos << BegComment << "unit=" << IOITEM_name(WN_IOITEM(item)) 
	<< EndComment;
    break;
  } /*switch*/
  
  return TRUE; // FIXME
}


static BOOL
xlate_IO_ITEM_format(xml::ostream& xos, WN* item, PUXlationContext& ctxt)
{
  /* Any arguments assumed to be by reference unless otherwise noted */
  ctxt.currentXlationContext().setFlag(XlationContext::DEREF_ADDR);
  
  switch (WN_io_item(item))
    {
    case IOF_NONE:
      if (ctxt.currentXlationContext().isFlag(XlationContext::CRAY_IO) 
	  && 
	  ctxt.currentXlationContext().isFlag(XlationContext::FMT_IO)) {
	xos << Comment("fmt=*");
      } 
      break;

    case IOF_ASSIGNED_VAR:
      /* The front-end should never generate these, since they are converted
       * into IOF_CHAR_EXPR items.  Should we for any reason start regenerating
       * these, this is the place where it should occur.
       */
      FORTTK_ASSERT_WARN(FALSE, 
			 fortTkSupport::Diagnostics::UnexpectedOpr << IOITEM_name(WN_IOITEM(item)));
      xos << BegComment << "fmt=" << IOITEM_name(WN_IOITEM(item)) << EndComment;
#if 0
      TranslateWN(xos, WN_kid0(item), ctxt);
#endif
      break;

    case IOF_LABEL:
      /* a FORMAT label or character-string expression */
      if (ctxt.currentXlationContext().isFlag(XlationContext::ORIGFMT_IOCTRL)) {
	xos << BegComment << "fmt=" << Num2Str(Origfmt_Ioctrl_Label, "%lld") 
	    << EndComment;
      } else {
	xos << "fmt***";
	TranslateWN(xos, WN_kid0(item), ctxt);
      }
      break;
     
    case IOF_CHAR_EXPR:
      /* a character-substring expression */
      xos << BegComment << "fmt=";
      WN2F_String_Argument(xos, WN_kid0(item), /* base */
			   WN_kid1(item), /* length */ ctxt);
      xos << EndComment;
      break;
     
    case IOF_LIST_DIRECTED:
      xos << Comment("fmt=*");
      break;

    case IOF_NAMELIST_DIRECTED:
      xos << Comment("NML=*");
      TranslateWN(xos, WN_kid(item,WN_kid_count(item)-1), ctxt);
      Clear_BE_ST_w2fc_referenced(WN_st(WN_kid(item,WN_kid_count(item)-1)));
      /* don't dump out Namelist name and corresponding type
       * in *.w2f.f file.When call TranslateWN,the namelist name(st entry) will
       * be set "referenced".Clear the flag.--------fzhao
       */
      break;

    case IOF_UNFORMATTED:
      break;

    case IOF_CR_PARSFMT:
      break;
       
    case IOF_CR_FMTSRC:
    case IOF_CR_FMTSRC_DOPE:
      {
	xos << Comment("fmt***");
	WN * kid0 = WN_kid0(item);
	if (!IS_IO_NULL_OPR(kid0))
	  TranslateWN(xos, kid0, ctxt);
	break;
      }
    default:
      FORTTK_DIE(fortTkSupport::Diagnostics::UnexpectedOpr << IOITEM_name(WN_IOITEM(item)));
      xos << BegComment << "fmt=" << IOITEM_name(WN_IOITEM(item)) 
	  << EndComment;
      break;
    } /*switch*/

  return TRUE; // FIXME
}


static BOOL
xlate_IO_ITEM_control(xml::ostream& xos, WN* item, PUXlationContext& ctxt)
{
  /* Return TRUE if a control specifier was emitted. */
  const IOITEM item_kind = WN_IOITEM(item);
  
  switch (item_kind) {
  case IOC_KEY:
    /* TODO: associate this with IOC_KEY */
    FORTTK_DEVMSG(0, fortTkSupport::Diagnostics::Unimplemented << "IOC" << IOITEM_name(item_kind));
    xos << BegComment << "ctr=" << IOITEM_name(item_kind) << "=< ??? >"
	<< EndComment;
    break;
    
  case IOC_KEY_START:
  case IOC_KEY_END:
  case IOC_KEY_CHARACTER:
  case IOC_KEY_INTEGER: 
  case IOC_NML: /* TODO: remove from IOC enumeration! It is redundant */
    FORTTK_DEVMSG(0, fortTkSupport::Diagnostics::Unimplemented << "IOC" << IOITEM_name(item_kind));
    xos << BegComment << "ctr=" << IOITEM_name(item_kind) << EndComment;
    break;
    
  case IOC_EXIST:
  case IOC_NAMED:
  case IOC_OPENED:
    /* LOGICAL argument */
    FORTTK_ASSERT(WN_kid_count(item) >= 1,
		  "Unexpected kid count " << WN_kid_count(item));
    xos << IOITEM_name(item_kind) << "***=";
    ctxt.currentXlationContext().setFlag(XlationContext::HAS_LOGICAL_ARG);
    if (TY_kind(WN_Tree_Type(WN_kid0(item))) != KIND_SCALAR)
      ctxt.currentXlationContext().setFlag(XlationContext::DEREF_ADDR);
    TranslateWN(xos, WN_kid0(item), ctxt);
    ctxt.currentXlationContext().unsetFlag(XlationContext::HAS_LOGICAL_ARG);
    break;
    
  case IOC_READONLY:
  case IOC_SHARED:
  case IOC_U:
    xos << BegComment << "ctr=" << IOITEM_name(item_kind) << EndComment;
    break;
    
  case IOC_VARFMT:
    break;
    
  case IOC_VARFMT_ORIGFMT:
    /* We assume a label-number already has been assigned for
     * this FORMAT statement into Origfmt_Ioctrl_Label.  See
     * also IOF_LABEL.  Expect the string argument to be an
     * LDA of a string-constant (ST of class CONST).  Note that
     * a string-constant always is '\0' terminated.
     */
    FORTTK_ASSERT_WARN((WN_operator(WN_kid0(item)) == OPR_LDA &&
			ST_class(WN_st(WN_kid0(item))) == CLASS_CONST),
		       fortTkSupport::Diagnostics::UnexpectedOpr << IOITEM_name(WN_IOITEM(item)));
    //ctxt.currentXlationContext().unsetFlag(no_newline);
    xos << "FORMAT " << Num2Str(Origfmt_Ioctrl_Label, "%lld")
	<< Targ_String_Address(STC_val(WN_st(WN_kid0(item))));
    break;
    
  case IOC_ERR:
  case IOC_EOR:
  case IOC_END:
    xos << IOITEM_name(item_kind);
    FORTTK_ASSERT_WARN((WN_operator(WN_kid0(item)) == OPR_GOTO),
		       fortTkSupport::Diagnostics::UnexpectedOpr << WN_operator(item));
    xos << WN_label_number(WN_kid0(item));
    break;
    
  case IOC_CR_FLFLAG:
  case IOC_CR_EDFLAG:
  case IOC_ERRFLAG:
  case IOC_CR_EEEFLAG:
    break;
    
  default:
    /* The usual case; an arbitrary non-logic expression argument,
     * which is usually an integral value, an array, or a character
     * string.
     */
    xos << IOITEM_name(item_kind) << "=";
    if (TY_kind(WN_Tree_Type(WN_kid0(item))) != KIND_SCALAR)
      ctxt.currentXlationContext().setFlag(XlationContext::DEREF_ADDR);
    
    /* if kid count > 1, then it's a character object */
    if (WN_kid_count(item) == 1)
      TranslateWN(xos, WN_kid0(item), ctxt);
    else
      WN2F_String_Argument(xos,WN_kid0(item),WN_kid1(item),ctxt);
    break;
  }
  
  return TRUE;
}

namespace whirl2xaif {

  void
  WN2F_implied_do(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);

}; /* namespace whirl2xaif */

static BOOL
xlate_IO_ITEM_list(xml::ostream& xos, WN *item, PUXlationContext& ctxt)
{
  const IOITEM item_kind = WN_IOITEM(item);

  switch (item_kind) {
  case IOL_VAR:
  case IOL_ARRAY:
  case IOL_CHAR_ARRAY:
  case IOL_RECORD:
    if (WN_operator(WN_kid0(item)) == OPR_LDID       &&
	ST_sclass(WN_st(WN_kid0(item))) == SCLASS_FORMAL &&
	TY_Is_Pointer(WN_ty(WN_kid0(item)))              &&
	TY_Is_Pointer(TY_pointed(WN_ty(WN_kid0(item))))) {
      /* Work around a f77 bug */
      WN_set_ty(WN_kid0(item), TY_pointed(WN_ty(WN_kid0(item))));
    }
    ctxt.currentXlationContext().setFlag(XlationContext::DEREF_ADDR); /* Assume pass-by-reference */
    TranslateWN(xos, WN_kid0(item), ctxt);
    break;
    
  case IOL_CHAR: {
    WN * len = WN_kid1(item);
    if (ctxt.currentXlationContext().isFlag(XlationContext::CRAY_IO))   /* typecode is kid1 */
      len = WN_kid2(item);
    
    WN2F_String_Argument(xos, WN_kid0(item), /* base */
			 len /* length */, ctxt);
  }
    break;

  case IOL_EXPR:
    ctxt.currentXlationContext().unsetFlag(XlationContext::DEREF_ADDR); /* Assume pass-by-value */
    TranslateWN(xos, WN_kid0(item), ctxt);
    break;
    
  case IOL_IMPLIED_DO:
  case IOL_IMPLIED_DO_1TRIP:
    ctxt.currentXlationContext().unsetFlag(XlationContext::DEREF_ADDR); /* Handled specially */
    WN2F_implied_do(xos, item, ctxt); /* Defined in WN2F_stmt.c */
    break;
    
  case IOL_LOGICAL:
    ctxt.currentXlationContext().setFlag(XlationContext::HAS_LOGICAL_ARG);
    ctxt.currentXlationContext().unsetFlag(XlationContext::DEREF_ADDR); /* Assume pass-by-value */
    TranslateWN(xos, WN_kid0(item), ctxt);
    break;
    
  case IOL_DOPE: {
    INT32 kids = WN_kid_count(item);
    
    /* base address */ 
    ctxt.currentXlationContext().setFlag(XlationContext::DEREF_ADDR); 
    TranslateWN(xos, WN_kid0(item), ctxt);
#if 0 //August 
    if (kids > 2) {  /* implied do? */
      /* Generate the subscript list - part may be in dope address, */
      /* set up several stmts ago. may just get INTCONST(0) here    */
      INT32 i = 2 ;
      while (i < kids) {
	TranslateWN(xos, WN_kid(item,i), ctxt);
      }
    }
#endif
  }
    break;
  
  default:
    FORTTK_DIE(fortTkSupport::Diagnostics::UnexpectedOpr << IOITEM_name(WN_IOITEM(item)));
    xos << IOITEM_name(WN_IOITEM(item));
    break;
  } /* switch */
  
  return TRUE;
}


// ***************************************************************************
// SGI IO
// ***************************************************************************

static void 
WN2F_ios_backspace(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  /* The kids should be an IOU, followed a sequence of IOCs.  Always
   * use the explicit UNIT keyword, unless there is exactly one kid an
   * it is an IOU.
   */
  FORTTK_ASSERT(WN_IOSTMT(wn) == IOS_BACKSPACE 
		|| WN_IOSTMT(wn) == IOS_CR_BACKSPACE, 
		fortTkSupport::Diagnostics::UnexpectedOpr << IOSTATEMENT_name(WN_IOSTMT(wn)));
  
  xos << "BACKSPACE";
  if (WN_kid_count(wn) == 1 && IS_IO_ITEM_IOU(WN_kid0(wn)))
    xlate_IO_ITEM(xos, WN_kid0(wn), ctxt);
  else
    xlate_IOControlList(xos, wn,
			0 /* from kid*/, WN_kid_count(wn)-1 /* to kid*/,
			ctxt);

} /* WN2F_ios_backspace */


static void 
WN2F_ios_close(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  /* The kids should be an IOU, followed a sequence of IOCs.  Always
   * use the explicit UNIT keyword.
   */
  FORTTK_ASSERT(WN_IOSTMT(wn) == IOS_CLOSE||WN_IOSTMT(wn) == IOS_CR_CLOSE,
		fortTkSupport::Diagnostics::UnexpectedOpr << IOSTATEMENT_name(WN_IOSTMT(wn)));

  fortTkSupport::WNId stmtid = ctxt.findWNId(wn);
  xos << BegElem(XAIFStrings.elem_Marker())
      << Attr("statement_id", stmtid)
      << BegAttr("annotation") << WhirlIdAnnotVal(stmtid)
      << " [CLOSE***]" << EndAttr
      << EndElem;

#if 0 // FIXME
  xos << "CLOSE";
  xlate_IOControlList(xos, wn,
		      0 /* from kid*/, WN_kid_count(wn)-1 /* to kid*/,
		      ctxt);
#endif
} /* WN2F_ios_close */


static void 
WN2F_ios_definefile(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  /* The kids should be an IOU, followed a sequence of IOCs.  Always
   * use the explicit UNIT keyword.
   */
  FORTTK_ASSERT(WN_io_statement(wn) == IOS_DEFINEFILE   &&
		WN_kid_count(wn) == 5                   &&
		WN_io_item(WN_kid(wn, 1)) == IOC_MAXREC &&
		WN_io_item(WN_kid(wn, 2)) == IOC_RECL   &&
		WN_io_item(WN_kid(wn, 3)) == IOC_U      &&
		WN_io_item(WN_kid(wn, 4)) == IOC_ASSOCIATEVARIABLE,
		fortTkSupport::Diagnostics::UnexpectedOpr << IOSTATEMENT_name(WN_IOSTMT(wn)));

  xos << "DEFINE FILE";

  xlate_IO_ITEM_unit(xos, WN_kid(wn, 0), ctxt);
  xos << "(";

  if (TY_kind(WN_Tree_Type(WN_kid0(WN_kid(wn, 1)))) != KIND_SCALAR)
    ctxt.currentXlationContext().setFlag(XlationContext::DEREF_ADDR);
  TranslateWN(xos, WN_kid0(WN_kid(wn, 1)), ctxt);
  ctxt.currentXlationContext().unsetFlag(XlationContext::DEREF_ADDR);
  xos << ",";

  if (TY_kind(WN_Tree_Type(WN_kid0(WN_kid(wn, 2)))) != KIND_SCALAR)
    ctxt.currentXlationContext().setFlag(XlationContext::DEREF_ADDR);
  TranslateWN(xos, WN_kid0(WN_kid(wn, 2)), ctxt);
  ctxt.currentXlationContext().unsetFlag(XlationContext::DEREF_ADDR);
  xos << ",";

  xos << "U";
  xos << ",";

  if (TY_kind(WN_Tree_Type(WN_kid0(WN_kid(wn, 4)))) != KIND_SCALAR)
    ctxt.currentXlationContext().setFlag(XlationContext::DEREF_ADDR);
  TranslateWN(xos, WN_kid0(WN_kid(wn, 4)), ctxt);
  xos << ")";
   
} /* WN2F_ios_definefile */


static void 
WN2F_ios_delete(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  /* The kids should be an IOU, followed a sequence of IOCs.  Always
   * use the explicit UNIT keyword.
   */
  FORTTK_ASSERT(WN_IOSTMT(wn) == IOS_DELETE, 
		fortTkSupport::Diagnostics::UnexpectedOpr << IOSTATEMENT_name(WN_IOSTMT(wn)));
  
  xos << "DELETE";
  xlate_IOControlList(xos, wn,
		      0 /* from kid*/, WN_kid_count(wn)-1 /* to kid*/, ctxt);
} /* WN2F_ios_delete */


static void 
WN2F_ios_endfile(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  /* The kids should be an IOU, followed a sequence of IOCs.  Always
   * use the explicit UNIT keyword, unless there is exactly one kid an
   * it is an IOU.
   */
  FORTTK_ASSERT(WN_IOSTMT(wn) == IOS_ENDFILE 
		|| WN_IOSTMT(wn) == IOS_CR_ENDFILE, 
		fortTkSupport::Diagnostics::UnexpectedOpr << IOSTATEMENT_name(WN_IOSTMT(wn)));
  
  xos << "ENDFILE";
  if (WN_kid_count(wn) == 1 && IS_IO_ITEM_IOU(WN_kid0(wn)))
    xlate_IO_ITEM(xos, WN_kid0(wn), ctxt);
  else
    xlate_IOControlList(xos, wn,
			0 /* from kid*/, WN_kid_count(wn)-1 /* to kid*/, ctxt);
  
} /* WN2F_ios_endfile */


static void 
WN2F_ios_find(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  /* The kids should be an IOU, followed a sequence of IOCs.  Always
   * use the explicit UNIT keyword.
   */
  FORTTK_ASSERT(WN_IOSTMT(wn) == IOS_FIND, 
		fortTkSupport::Diagnostics::UnexpectedOpr << IOSTATEMENT_name(WN_IOSTMT(wn)));
  
  xos << "FIND";
  xlate_IOControlList(xos, wn,
		      0 /* from kid*/, WN_kid_count(wn)-1 /* to kid*/, ctxt);
} /* WN2F_ios_find */


static void 
WN2F_ios_inquire(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  /* The kids should be an optional IOU, followed a sequence of IOCs.
   * Always use the explicit UNIT keyword when the IOU is present.
   */
  FORTTK_ASSERT(WN_IOSTMT(wn) == IOS_INQUIRE || 
		WN_IOSTMT(wn) == IOS_CR_INQUIRE,
		fortTkSupport::Diagnostics::UnexpectedOpr << IOSTATEMENT_name(WN_IOSTMT(wn)));
  
  xos << BegElem(XAIFStrings.elem_Marker())
      << Attr("statement_id", ctxt.currentXlationContext().getNewVertexId())
      << BegAttr("annotation") << WhirlIdAnnotVal(ctxt.findWNId(wn))
      << " [INQUIRE***]" << EndAttr
      << EndElem;

#if 0
  xos << "INQUIRE";
  xlate_IOControlList(xos, wn,
		      0,                  /* from kid*/
		      WN_kid_count(wn)-1, /* to kid*/
		      ctxt);
#endif 

} /* WN2F_ios_inquire */


static void 
WN2F_ios_namelist(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  FORTTK_ASSERT(WN_IOSTMT(wn) == IOS_NAMELIST, 
		fortTkSupport::Diagnostics::UnexpectedOpr << IOSTATEMENT_name(WN_IOSTMT(wn)));
  
  xos << "NAMELIST/";
  xlate_IO_ITEM(xos, WN_kid1(wn), ctxt);
  xos << "/";
  
  if (WN_kid_count(wn) > 2)
    xlate_IOList(xos, wn, 2, ctxt);

} /* WN2F_ios_namelist */


static void 
WN2F_ios_open(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  /* The kids should be an IOU, followed a sequence of IOCs.  Always
   * use the explicit UNIT keyword.
   */
  FORTTK_ASSERT(WN_IOSTMT(wn) == IOS_OPEN || WN_IOSTMT(wn) == IOS_CR_OPEN,
		fortTkSupport::Diagnostics::UnexpectedOpr << IOSTATEMENT_name(WN_IOSTMT(wn)));
  
  fortTkSupport::WNId stmtid = ctxt.findWNId(wn);
  xos << BegElem(XAIFStrings.elem_Marker())
      << Attr("statement_id", stmtid)
      << BegAttr("annotation") << WhirlIdAnnotVal(stmtid)
      << " [OPEN***]" << EndAttr
      << EndElem;
} /* WN2F_ios_open */


static void 
WN2F_ios_rewind(xml::ostream& xos, WN *wn, PUXlationContext& ctxt) {
  FORTTK_ASSERT(WN_IOSTMT(wn) == IOS_REWIND || WN_IOSTMT(wn) == IOS_CR_REWIND, 
		fortTkSupport::Diagnostics::UnexpectedOpr << IOSTATEMENT_name(WN_IOSTMT(wn)));
  fortTkSupport::WNId stmtid = ctxt.findWNId(wn);
  xos << BegElem(XAIFStrings.elem_Marker())
      << Attr("statement_id", stmtid)
      << BegAttr("annotation") << WhirlIdAnnotVal(stmtid)
      << " [REWIND***]" << EndAttr; 
  xos << EndElem;
} /* WN2F_ios_rewind */


static void 
WN2F_ios_unlock(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  /* The kids should be an IOU, followed a sequence of IOCs.  Always
   * use the explicit UNIT keyword, unless there is exactly one kid an
   * it is an IOU.
   */
  FORTTK_ASSERT(WN_IOSTMT(wn) == IOS_UNLOCK, 
		fortTkSupport::Diagnostics::UnexpectedOpr << IOSTATEMENT_name(WN_IOSTMT(wn)));
  
  xos << "UNLOCK";
  if (WN_kid_count(wn) == 1 && IS_IO_ITEM_IOU(WN_kid0(wn)))
    xlate_IO_ITEM(xos, WN_kid0(wn), ctxt);
  else
    xlate_IOControlList(xos, wn,
			0 /* from kid*/, WN_kid_count(wn)-1 /* to kid*/, ctxt);
  
} /* WN2F_ios_unlock */


static void 
WN2F_ios_accept(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  /* The kids should be an IOF, followed a sequence of IOLs.
   */
  FORTTK_ASSERT(WN_IOSTMT(wn) == IOS_ACCEPT, 
		fortTkSupport::Diagnostics::UnexpectedOpr << IOSTATEMENT_name(WN_IOSTMT(wn)));
  
  xos << "ACCEPT";
  xlate_IO_ITEM(xos, WN_kid0(wn), ctxt);
  if (WN_kid_count(wn) > 1) {
    xlate_IOList(xos, wn, 1, ctxt);
  }
} /* WN2F_ios_accept */


static void
WN2F_ios_decode(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  /* The kids should be an IOU, followed by an IOF, followed by a
   * sequence of IOCs and a sequence of IOLs.  Use keywords only
   * when the IOC list is non-empty.  Note that the IOU contains
   * both the integer expression (c) and the scalar or array 
   * reference (a) in "ENCODE (c, f, a [,IOSTAT=ios][,ERR=s]) iolist".
   */
  INT  iol_kid;
  
  FORTTK_ASSERT(WN_IOSTMT(wn) == IOS_DECODE, 
		fortTkSupport::Diagnostics::UnexpectedOpr << IOSTATEMENT_name(WN_IOSTMT(wn)));
  
  /* This is needed for the translation of the number of characters and
   * the buffer we decode characters from.
   */
  ctxt.currentXlationContext().setFlag(XlationContext::DEREF_ADDR);
  
  xos << "DECODE(";
  
  /* Translate the number of characters */
  FORTTK_ASSERT_WARN(WN_IOITEM(WN_kid0(wn)) == IOU_INTERNAL &&
		     WN_kid_count(WN_kid0(wn)) >= 2,
		     fortTkSupport::Diagnostics::UnexpectedOpr << IOITEM_name(WN_IOITEM(WN_kid0(wn))));
  TranslateWN(xos, WN_kid1(WN_kid0(wn)), ctxt);
  
  /* Translate the format */
  xos << ",";
  xlate_IO_ITEM(xos, WN_kid1(wn), ctxt);
  
  /* Translate the buffer we decode from */
  xos << ",";
  TranslateWN(xos, WN_kid0(WN_kid0(wn)), ctxt);
  
  /* Translate the EOSTAT and the ERR items */
  iol_kid = 2;
  if (WN_kid_count(wn) > 2 && IS_IO_ITEM_IOC(WN_kid(wn, 2))) {
    iol_kid = 3;
    xos << ",";
    xlate_IO_ITEM(xos, WN_kid(wn, 2), ctxt);
  }
  if (WN_kid_count(wn) > 3 && IS_IO_ITEM_IOC(WN_kid(wn, 3))) {
    iol_kid = 4;
    xos << ",";
    xlate_IO_ITEM(xos, WN_kid(wn, 3), ctxt);
  }
  xos << ")";
  
  /* Get the io_list */
  if (WN_kid_count(wn) > iol_kid)
    xlate_IOList(xos, wn, iol_kid, ctxt);
  
} /* WN2F_ios_decode */


static void 
WN2F_ios_encode(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  /* The kids should be an IOU, followed by an IOF, followed by a
   * sequence of IOCs and a sequence of IOLs.  Use keywords only
   * when the IOC list is non-empty.  Note that the IOU contains
   * both the integer expression (c) and the scalar or array 
   * reference (a) in "ENCODE (c, f, a [,IOSTAT=ios][,ERR=s]) iolist".
   */
  INT  iol_kid;
  
  FORTTK_ASSERT(WN_IOSTMT(wn) == IOS_ENCODE, 
		fortTkSupport::Diagnostics::UnexpectedOpr << IOSTATEMENT_name(WN_IOSTMT(wn)));
  
  /* This is needed for the translation of the number of characters and
   * the buffer we encode characters from.
   */
  ctxt.currentXlationContext().setFlag(XlationContext::DEREF_ADDR);
  
  xos << "ENCODE(";
  
  /* Translate the number of characters */
  FORTTK_ASSERT_WARN(WN_IOITEM(WN_kid0(wn)) == IOU_INTERNAL &&
		     WN_kid_count(WN_kid0(wn)) >= 2,
		     fortTkSupport::Diagnostics::UnexpectedOpr << IOITEM_name(WN_IOITEM(WN_kid0(wn))));
  TranslateWN(xos, WN_kid1(WN_kid0(wn)), ctxt);
  
  /* Translate the format */
  xos << ",";
  xlate_IO_ITEM(xos, WN_kid1(wn), ctxt);
  
  /* Translate the buffer we encode from */
  xos << ",";
  TranslateWN(xos, WN_kid0(WN_kid0(wn)), ctxt);
  
  /* Translate the EOSTAT and the ERR items */
  iol_kid = 2;
  if (WN_kid_count(wn) > 2 && IS_IO_ITEM_IOC(WN_kid(wn, 2))) {
    iol_kid = 3;
    xos << ",";
    xlate_IO_ITEM(xos, WN_kid(wn, 2), ctxt);
  }
  if (WN_kid_count(wn) > 3 && IS_IO_ITEM_IOC(WN_kid(wn, 3))) {
    iol_kid = 4;
    xos << ",";
    xlate_IO_ITEM(xos, WN_kid(wn, 3), ctxt);
  }
  xos << ")";
  
  /* Get the io_list */
  if (WN_kid_count(wn) > iol_kid)
    xlate_IOList(xos, wn, iol_kid, ctxt);
  
} /* WN2F_ios_encode */


static void 
WN2F_ios_print(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  INT iol_kid;
  
  /* The kids should be an IOF, followed a sequence of IOLs.
   */
  FORTTK_ASSERT(WN_IOSTMT(wn) == IOS_PRINT, 
		fortTkSupport::Diagnostics::UnexpectedOpr << IOSTATEMENT_name(WN_IOSTMT(wn)));
  
  xos << "PRINT";
  
  //ctxt.currentXlationContext().setFlag(issue_ioc_asterisk);
  
  /* We do not really expect to have a unit specification for a "PRINT"
   * statement, but just in the case one occurs anyway, we skip it here.
   */
  if (IS_IO_ITEM_IOU(WN_kid0(wn)))
    iol_kid = 2; /* kid 1 must be the format */
  else
    iol_kid = 1; /* Missing unit number */
  xlate_IO_ITEM(xos, WN_kid(wn, iol_kid-1), ctxt);
  if (WN_kid_count(wn) > iol_kid) {
    xlate_IOList(xos, wn, iol_kid, ctxt);
  }
  
  //ctxt.currentXlationContext().unsetFlag(issue_ioc_asterisk);
  
} /* WN2F_ios_print */


static void 
WN2F_ios_read(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  /* The kids should be an IOU, followed by an IOF, followed by a
   * sequence of IOCs and a sequence of IOLs.  Use keywords only
   * when the IOC list is non-empty.  The IOL should be a sequence
   * of addresses into which the values read should be put.  In
   * the whirl2f output these addresses must be dereferenced.
   */
  INT  iol_kid;
  BOOL use_keyword;
  
  
  xos << "READ";
  //ctxt.currentXlationContext().setFlag(issue_ioc_asterisk);
  
  /* Determine whether or not we have the "READ f [,iolist]" format.
   */
  if (WN_IOITEM(WN_kid0(wn)) == IOU_DEFAULT           && 
      IS_IO_ITEM_IOF(WN_kid1(wn))                         &&
      WN_IOITEM(WN_kid1(wn)) != IOF_NAMELIST_DIRECTED &&
      (WN_kid_count(wn) == 2 || IS_IO_ITEM_IOL(WN_kid(wn, 2)))) {
    xlate_IO_ITEM(xos, WN_kid0(wn), ctxt);
    iol_kid = 2;
    if (WN_kid_count(wn) > 2)
      xos << ",";
  } else {
    for (iol_kid = 0; 
	 (iol_kid < WN_kid_count(wn)) && !IS_IO_ITEM_IOL(WN_kid(wn, iol_kid));
	 iol_kid++);
    
    /* Get the IOU, IOF, and IOC items */
    xlate_IOControlList(xos, wn,
			0,           /* from kid*/
			iol_kid-1,   /* to kid*/
			ctxt);
  }
  
  /* Get the io_list */
  if (iol_kid < WN_kid_count(wn)) {
    ctxt.currentXlationContext().setFlag(XlationContext::DEREF_IO_ITEM); /* Assume pass-by-reference */
    xlate_IOList(xos, wn, iol_kid, ctxt);
  }
  
  //ctxt.currentXlationContext().unsetFlag(issue_ioc_asterisk);
  
} /* WN2F_ios_read */


static void 
WN2F_ios_rewrite(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  /* The kids should be an IOU, followed by an IOF, followed by a
   * sequence of IOCs and a sequence of IOLs.  Use keywords only
   * when the IOC list is non-empty.
   */
  INT  iol_kid;
  
  FORTTK_ASSERT(WN_IOSTMT(wn) == IOS_REWRITE, 
		fortTkSupport::Diagnostics::UnexpectedOpr << IOSTATEMENT_name(WN_IOSTMT(wn)));
  
  xos << "REWRITE";
  //ctxt.currentXlationContext().setFlag(issue_ioc_asterisk);
  
  for (iol_kid = 0; 
       (iol_kid < WN_kid_count(wn)) && !IS_IO_ITEM_IOL(WN_kid(wn, iol_kid));
       iol_kid++);
  
  /* Get the IOU, IOF, and IOC items */
  xlate_IOControlList(xos, wn,
		      0,           /* from kid*/
		      iol_kid-1,     /* to kid*/
		      ctxt);
  
  /* Get the io_list */
  if (iol_kid < WN_kid_count(wn))
    xlate_IOList(xos, wn, iol_kid, ctxt);
  
  //ctxt.currentXlationContext().unsetFlag(issue_ioc_asterisk);
  
} /* WN2F_ios_rewrite */


static void 
WN2F_ios_type(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  /* The kids should be an IOF, followed a sequence of IOLs.
   */
  FORTTK_ASSERT(WN_IOSTMT(wn) == IOS_TYPE, 
		fortTkSupport::Diagnostics::UnexpectedOpr << IOSTATEMENT_name(WN_IOSTMT(wn)));
  
  xos << "TYPE";
  xlate_IO_ITEM(xos, WN_kid0(wn), ctxt);
  if (WN_kid_count(wn) > 1) {
    xlate_IOList(xos, wn, 1, ctxt);
  }
} /* WN2F_ios_type */


static void 
WN2F_ios_write(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  /* The kids should be an IOU, followed by an IOF, followed by a
   * sequence of IOCs and a sequence of IOLs.  Use keywords only
   * when the IOC list is non-empty.
   */
  fortTkSupport::WNId stmtid = ctxt.findWNId(wn);
  xos << BegElem(XAIFStrings.elem_Marker())
      << Attr("statement_id", stmtid)
      << BegAttr("annotation") << WhirlIdAnnotVal(stmtid)
      << " [WRITE***]" << EndAttr;
  
#if 0 // FIXME: comment out for now
  //ctxt.currentXlationContext().setFlag(issue_ioc_asterisk);
  
  INT  iol_kid;
  for (iol_kid = 0; 
       (iol_kid < WN_kid_count(wn)) && !IS_IO_ITEM_IOL(WN_kid(wn, iol_kid));
       iol_kid++);
  
  /* Get the IOU, IOF, and IOC items */
  xlate_IOControlList(xos, wn,
		      0 /* from kid */,  iol_kid-1 /* to kid */, ctxt);
  
  /* Get the io_list */
  if (iol_kid < WN_kid_count(wn))
    xlate_IOList(xos, wn, iol_kid, ctxt);
  
  //ctxt.currentXlationContext().unsetFlag(issue_ioc_asterisk);
#endif
  xos << EndElem;
  
} /* WN2F_ios_write */


// ***************************************************************************
// Cray IO
// ***************************************************************************

static void 
WN2F_ios_cr(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
{
  // Craylibs IO - write/read The kids should be an IOS, with kids of IO_ITEMS
  IOSTATEMENT iostmt = WN_io_statement(wn);
  FORTTK_ASSERT(iostmt == IOS_CR_FWF || iostmt == IOS_CR_FWU 
		|| iostmt == IOS_CR_FRF || iostmt == IOS_CR_FRU,
		fortTkSupport::Diagnostics::UnexpectedOpr << IOSTATEMENT_name(iostmt));
  
  //ctxt.currentXlationContext().setFlag(issue_ioc_asterisk);
  
  /* decide if read/write formatted/unformatted */
  if (iostmt == IOS_CR_FWF || iostmt == IOS_CR_FRF)
    ctxt.currentXlationContext().setFlag(XlationContext::FMT_IO) ;

  const char* io_op = "WRITE***";
  if (iostmt == IOS_CR_FRF || iostmt == IOS_CR_FRU) {
    io_op = "READ***" ;
  }

  fortTkSupport::WNId stmtid = ctxt.findWNId(wn);
  xos << BegElem(XAIFStrings.elem_Marker())
      << Attr("statement_id", stmtid)
      << BegAttr("annotation") << WhirlIdAnnotVal(stmtid)
      << " [" << io_op << "]" << EndAttr;

#if 0 // FIXME: comment out for now
  /* count items in control list */
  INT iol_kid;
  for (iol_kid = 0; 
       (iol_kid < WN_kid_count(wn)) && !IS_IO_ITEM_IOL(WN_kid(wn, iol_kid));
       iol_kid++);
  
  /* Get the IOU, IOF, and IOC items */
  xlate_IOControlList(xos, wn,
		      0 /* from kid */, iol_kid-1 /* to kid */, ctxt);
  
  /* Get the IOL (io_list) */
  if (iol_kid < WN_kid_count(wn))
    xlate_IOList(xos, wn, iol_kid, ctxt);
  
  //reset_PUXlationContex_issue_ioc_asterisk(ctxt);
#endif
  
  xos << EndElem;
} 

