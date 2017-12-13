// -*-Mode: C++;-*-
// $Header: /m_home/m_utkej/Argonne/cvs2svn/cvs/OpenADFortTk/src/sexp2whirl/sexp2wn.h,v 1.6 2007-10-08 18:28:34 utke Exp $

//***************************************************************************
//
// File:
//   $Source: /m_home/m_utkej/Argonne/cvs2svn/cvs/OpenADFortTk/src/sexp2whirl/sexp2wn.h,v $
//
// Purpose:
//   [The purpose of this file]
//
// Description:
//   [The set of functions, macros, etc. defined in the file]
//
//***************************************************************************

#ifndef sexp2wn_h
#define sexp2wn_h

//************************** System Include Files ***************************

#include <vector> // STL
#include <list>   // STL

//*************************** Sexp Include Files ****************************

#include <sexp.h>

//************************** Open64 Include Files ***************************

#include "Open64IRInterface/Open64BasicTypes.h"

//*************************** User Include Files ****************************

#include "sexp2whirl.i"

//************************** Forward Declarations ***************************

//***************************************************************************
// Commonly used WHIRL translation functions
//***************************************************************************

namespace sexp2whirl {
    
  // Note: All routines Assume that Open64 symbol table globals are
  // already set.

  // TranslateWN: Translate the given S-expression WHIRL-AST node and
  // return a WN*.  If 'sx' is NULL, returns NULL.
  extern WN* 
  TranslateWN(sexp_t* sx);

  // TranslateWNChildren: Given a non-NULL S-expression WHIRL-AST
  // node, translate its children and return them in a WN-vector.
  extern std::vector<WN*>
  TranslateWNChildren(sexp_t* sx);
  
}; /* namespace sexp2whirl */


//***************************************************************************
// Commonly used routines for extracting information from S-expressions
//***************************************************************************

namespace sexp2whirl {
  
  ST_IDX 
  GetWhirlSym(sexp_t* sx);
  
  TY_IDX
  GetWhirlTy(sexp_t* sx);
  
  OPERATOR
  GetWhirlOpr(sexp_t* sx);
  
  OPCODE
  GetWhirlOpc(sexp_t* sx);
  
  ST_IDX
  GetWhirlSymRef(sexp_t* sx);
  
  TY_IDX
  GetWhirlTyUse(sexp_t* sx);
  
  const char*
  GetWhirlFlg(sexp_t* sx);

  UINT64
  GetWhirlOpaqueFlg(sexp_t* sx);
  
}; /* namespace sexp2whirl */


//***************************************************************************

#endif /* sexp2wn_h */
