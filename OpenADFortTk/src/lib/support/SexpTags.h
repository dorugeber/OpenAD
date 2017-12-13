// -*-Mode: C++;-*-
// $Header: /m_home/m_utkej/Argonne/cvs2svn/cvs/OpenADFortTk/src/lib/support/SexpTags.h,v 1.5 2005-02-01 22:02:28 eraxxon Exp $

//***************************************************************************
//
// File:
//   $Source: /m_home/m_utkej/Argonne/cvs2svn/cvs/OpenADFortTk/src/lib/support/SexpTags.h,v $
//
// Purpose:
//   [The purpose of this file]
//
// Description:
//   [The set of functions, macros, etc. defined in the file]
//
//***************************************************************************

#ifndef SexpTags_h
#define SexpTags_h

//************************** System Include Files ***************************

//*************************** User Include Files ****************************

//************************** Forward Declarations ***************************

//***************************************************************************

namespace SexpTags {

  const char* 
  get_grammar_summary();
  
  const char* 
  get_grammar();

  extern const char* WHIRL;
  extern const char* PU_FOREST;
  extern const char* PU_TREE;
  extern const char* PU;
  
  extern const char* GBL_SYMTAB;
  extern const char* PU_SYMTAB;
  
  extern const char* FILE_INFO;
  extern const char* ST_TAB;
  extern const char* TY_TAB;
  extern const char* PU_TAB;
  extern const char* FLD_TAB;
  extern const char* ARB_TAB;
  extern const char* TYLIST_TAB;
  extern const char* TCON_TAB;
  extern const char* TCON_STR_TAB;
  extern const char* INITO_TAB;
  extern const char* INITV_TAB;
  extern const char* BLK_TAB;
  extern const char* ST_ATTR_TAB;
  extern const char* STR_TAB;
  extern const char* PREG_TAB;
  extern const char* LABEL_TAB;

  extern const char* ST;
  extern const char* TY;
  extern const char* FLG;
  extern const char* OFLG;

};

//***************************************************************************

#endif /* SexpTags_h */
