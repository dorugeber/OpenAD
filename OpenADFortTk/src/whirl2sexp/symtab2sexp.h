// -*-Mode: C++;-*-
// $Header: /m_home/m_utkej/Argonne/cvs2svn/cvs/OpenADFortTk/src/whirl2sexp/symtab2sexp.h,v 1.9 2007-10-08 18:28:34 utke Exp $
#ifndef symtab2sexp_h
#define symtab2sexp_h

#include "Open64IRInterface/Open64BasicTypes.h"

#include "sexpostream.h"

//***************************************************************************
// Translate symbol tables
//***************************************************************************

namespace whirl2sexp {

  extern void 
  TranslateGlobalSymbolTables(sexp::ostream& sos, int flags = 0);
  
  extern void 
  TranslateLocalSymbolTables(sexp::ostream& sos, SYMTAB_IDX stab_lvl,
			     int flags = 0);

}; /* namespace whirl2sexp */


//***************************************************************************
// Translate individual tables
//***************************************************************************

namespace whirl2sexp {

  // Translate every entry of the given table at the given lexical level
  // ('stab_lvl'). 

  void 
  xlate_FILE_INFO(sexp::ostream& sos);

  void 
  xlate_ST_TAB(sexp::ostream& sos, SYMTAB_IDX stab_lvl);
  void
  xlate_ST_TAB(sexp::ostream& sos, const SCOPE& scope);

  void 
  xlate_ST_ATTR_TAB(sexp::ostream& sos, SYMTAB_IDX stab_lvl);

  void 
  xlate_PU_TAB(sexp::ostream& sos);

  void 
  xlate_TY_TAB(sexp::ostream& sos);

  void 
  xlate_FLD_TAB(sexp::ostream& sos);

  void 
  xlate_ARB_TAB(sexp::ostream& sos);

  void 
  xlate_TYLIST_TAB(sexp::ostream& sos);
  
  void 
  xlate_TCON_TAB(sexp::ostream& sos);

  void 
  xlate_TCON_STR_TAB(sexp::ostream& sos);
  
  void 
  xlate_INITO_TAB(sexp::ostream& sos, SYMTAB_IDX stab_lvl);

  void 
  xlate_INITV_TAB(sexp::ostream& sos);

  void 
  xlate_BLK_TAB(sexp::ostream& sos);
  
  void 
  xlate_STR_TAB(sexp::ostream& sos);

  
  void 
  xlate_LABEL_TAB(sexp::ostream& sos, SYMTAB_IDX stab_lvl);

  void 
  xlate_PREG_TAB(sexp::ostream& sos, SYMTAB_IDX stab_lvl);
      
}; /* namespace whirl2sexp */


//***************************************************************************
// Functions and functors to translate individual table entries
//***************************************************************************

namespace whirl2sexp {
  
  void
  xlate_SYMTAB_entry(sexp::ostream& sos, UINT32 idx, ST* st);
  
  void
  xlate_SYMTAB_entry(sexp::ostream& sos, UINT32 idx, ST_ATTR* sta);

  void
  xlate_SYMTAB_entry(sexp::ostream& sos, UINT32 idx, PU* pu);

  void
  xlate_SYMTAB_entry(sexp::ostream& sos, UINT32 idx, TY* ty);
  
  void
  xlate_SYMTAB_entry(sexp::ostream& sos, UINT32 idx, FLD* fld);

  void
  xlate_SYMTAB_entry(sexp::ostream& sos, UINT32 idx, ARB* arb);
 
  void
  xlate_SYMTAB_entry(sexp::ostream& sos, UINT32 idx, TYLIST* tyl);
 
  void
  xlate_SYMTAB_entry(sexp::ostream& sos, UINT32 idx, TCON* tcon);

  void
  xlate_SYMTAB_entry(sexp::ostream& sos, UINT32 idx, INITO* inito);

  void
  xlate_SYMTAB_entry(sexp::ostream& sos, UINT32 idx, INITV* initv);

  void
  xlate_SYMTAB_entry(sexp::ostream& sos, UINT32 idx, BLK* blk);
    
  void
  xlate_SYMTAB_entry(sexp::ostream& sos, UINT32 idx, LABEL* lbl);

  void
  xlate_SYMTAB_entry(sexp::ostream& sos, UINT32 idx, PREG* preg);

  void 
  xlate_SYMTAB_entry_TCON_STR(sexp::ostream& sos, UINT32 idx);

  void
  xlate_SYMTAB_entry_STR(sexp::ostream& sos, UINT32 idx);

  
  // xlate_SYMTAB_entry_op: a templated functor that simply calls the
  // above (overloaded) routines.
  template <class T>
  class xlate_SYMTAB_entry_op {
  public:
    xlate_SYMTAB_entry_op(sexp::ostream& sos_) : sos(sos_), tblsz(0) { }
    ~xlate_SYMTAB_entry_op() { }
    
    void operator()(UINT32 idx, T* entry) const { 
      xlate_SYMTAB_entry(sos, idx, entry);
      sos << sexp::EndLine;
    }
    
  private:
    sexp::ostream& sos;
    UINT32 tblsz; // could use to conditionally generate EndLines
  };


}; /* namespace whirl2sexp */


//***************************************************************************

#endif /* symtab2sexp_h */

