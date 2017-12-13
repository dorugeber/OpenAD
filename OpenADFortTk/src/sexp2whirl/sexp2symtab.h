// -*-Mode: C++;-*-
// $Header: /m_home/m_utkej/Argonne/cvs2svn/cvs/OpenADFortTk/src/sexp2whirl/sexp2symtab.h,v 1.9 2007-10-08 18:28:33 utke Exp $
#ifndef sexp2symtab_h
#define sexp2symtab_h

#include <sexp.h>

#include "Open64IRInterface/Open64BasicTypes.h"

#include "Diagnostics.h"

namespace sexp2whirl {

  extern void 
  TranslateGlobalSymbolTables(sexp_t* gbl_symtab, int flags = 0);

  extern void 
  TranslateLocalSymbolTables(sexp_t* pu_symtab, SYMTAB_IDX stab_lvl,
			     int flags = 0);

}; /* namespace sexp2whirl */


//***************************************************************************
// Translate individual tables
//***************************************************************************

namespace sexp2whirl {

  // Translate every entry of the given table at the given lexical level
  // ('stab_lvl'). 

  void 
  xlate_FILE_INFO(sexp_t* file_info);

  void 
  xlate_ST_TAB(sexp_t* st_tab, SYMTAB_IDX stab_lvl);
  void
  xlate_ST_TAB(sexp_t* st_tab, const SCOPE& scope);

  void 
  xlate_ST_ATTR_TAB(sexp_t* st_attr_tab, SYMTAB_IDX stab_lvl);

  void 
  xlate_PU_TAB(sexp_t* pu_tab);

  void 
  xlate_TY_TAB(sexp_t* ty_tab);

  void 
  xlate_FLD_TAB(sexp_t* fld_tab);

  void 
  xlate_ARB_TAB(sexp_t* arb_tab);

  void 
  xlate_TYLIST_TAB(sexp_t* tylist_tab);
  
  void 
  xlate_TCON_TAB(sexp_t* tcon_tab);

  void 
  xlate_TCON_STR_TAB(sexp_t* str_tab);
  
  void 
  xlate_INITO_TAB(sexp_t* inito_tab, SYMTAB_IDX stab_lvl);

  void 
  xlate_INITV_TAB(sexp_t* initv_tab);

  void 
  xlate_BLK_TAB(sexp_t* blk_tab);
  
  void 
  xlate_STR_TAB(sexp_t* str_tab);

  
  void 
  xlate_LABEL_TAB(sexp_t* label_tab, SYMTAB_IDX stab_lvl);
  
  void 
  xlate_PREG_TAB(sexp_t* preg_tab, SYMTAB_IDX stab_lvl);
      
}; /* namespace sexp2whirl */


//***************************************************************************
// Functions to translate individual table entries
//***************************************************************************

namespace sexp2whirl {
  
  ST*
  xlate_ST_TAB_entry(sexp_t* sx);
  
  ST_ATTR*
  xlate_ST_ATTR_TAB_entry(sexp_t* sx);

  PU*
  xlate_PU_TAB_entry(sexp_t* sx);

  TY*
  xlate_TY_TAB_entry(sexp_t* sx);

  FLD*
  xlate_FLD_TAB_entry(sexp_t* sx);

  ARB*
  xlate_ARB_TAB_entry(sexp_t* sx);
  
  TYLIST*
  xlate_TYLIST_TAB_entry(sexp_t* sx);

  TCON*
  xlate_TCON_TAB_entry(sexp_t* sx);

  INITO*
  xlate_INITO_TAB_entry(sexp_t* sx);

  INITV*
  xlate_INITV_TAB_entry(sexp_t* sx);

  BLK*
  xlate_BLK_TAB_entry(sexp_t* sx);
  
  LABEL*
  xlate_LABEL_TAB_entry(sexp_t* sx);

  PREG*
  xlate_PREG_TAB_entry(sexp_t* sx);


  UINT32
  xlate_TCON_STR_TAB_entry(sexp_t* sx, std::string& buf);

  UINT32
  xlate_STR_TAB_entry(sexp_t* sx, std::string& buf);
  

  // xlate_SYMTAB_entry: Templated access to translation routines
  template <typename T>
  T*
  xlate_SYMTAB_entry(sexp_t* sx)
  { FORTTK_DIE("Must not be called (template specialization must exist)"); }
  
  // Specializations of xlate_SYMTAB_entry
  template <> 
  inline ST* 
  xlate_SYMTAB_entry<ST>(sexp_t* sx) { return xlate_ST_TAB_entry(sx); }
  
  template <>
  inline ST_ATTR* 
  xlate_SYMTAB_entry<ST_ATTR>(sexp_t* sx) { return xlate_ST_ATTR_TAB_entry(sx); }
  
  template <>
  inline PU* 
  xlate_SYMTAB_entry<PU>(sexp_t* sx) { return xlate_PU_TAB_entry(sx); }

  template <>
  inline TY* 
  xlate_SYMTAB_entry<TY>(sexp_t* sx) { return xlate_TY_TAB_entry(sx); }
  
  template <>
  inline FLD* 
  xlate_SYMTAB_entry<FLD>(sexp_t* sx) { return xlate_FLD_TAB_entry(sx); }
  
  template <>
  inline ARB* 
  xlate_SYMTAB_entry<ARB>(sexp_t* sx) { return xlate_ARB_TAB_entry(sx); }
  
  template <>
  inline TYLIST* 
  xlate_SYMTAB_entry<TYLIST>(sexp_t* sx) { return xlate_TYLIST_TAB_entry(sx); }
  
  template <>
  inline TCON* 
  xlate_SYMTAB_entry<TCON>(sexp_t* sx) { return xlate_TCON_TAB_entry(sx); }
  
  template <>
  inline INITO* 
  xlate_SYMTAB_entry<INITO>(sexp_t* sx) { return xlate_INITO_TAB_entry(sx); }
  
  template <>
  inline INITV* 
  xlate_SYMTAB_entry<INITV>(sexp_t* sx) { return xlate_INITV_TAB_entry(sx); }
  
  template <>
  inline BLK* 
  xlate_SYMTAB_entry<BLK>(sexp_t* sx) { return xlate_BLK_TAB_entry(sx); }
    
  template <>
  inline LABEL* 
  xlate_SYMTAB_entry<LABEL>(sexp_t* sx) { return xlate_LABEL_TAB_entry(sx); }
  
  template <>
  inline PREG* 
  xlate_SYMTAB_entry<PREG>(sexp_t* sx) { return xlate_PREG_TAB_entry(sx); }
  
}; /* namespace sexp2whirl */


//***************************************************************************

#endif /* sexp2symtab_h */

