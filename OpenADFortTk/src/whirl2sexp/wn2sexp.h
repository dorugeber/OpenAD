// -*-Mode: C++;-*-
// $Header: /m_home/m_utkej/Argonne/cvs2svn/cvs/OpenADFortTk/src/whirl2sexp/wn2sexp.h,v 1.8 2007-10-08 18:28:34 utke Exp $
#ifndef wn2sexp_h
#define wn2sexp_h

#include <vector> // STL
#include <list>   // STL

#include "Open64IRInterface/Open64BasicTypes.h"

#include "sexpostream.h"
#include "SexpTags.h"

#include "whirl2sexp.i"

//***************************************************************************
// Commonly used WHIRL translation functions
//***************************************************************************

namespace whirl2sexp {
    
  // Note: All routines Assume that Open64 symbol table globals are
  // already set.

  // TranslateWN: Translates the given WHIRL node, emitting output to
  // 'sos'.  If 'wn' is NULL, emits an empty list.  
  extern whirl2sexp::status 
  TranslateWN(sexp::ostream& sos, WN* wn);

  // TranslateWNChildren: Given a non-NULL WHIRL node, translates its
  // children, emitting output to 'sos'.
  extern whirl2sexp::status 
  TranslateWNChildren(sexp::ostream& sos, WN* wn);
  
}; /* namespace whirl2sexp */


//***************************************************************************
// S-expressions sexp::ostream operators
//***************************************************************************

// ---------------------------------------------------------
// Shortcut for TranslateWN()
// ---------------------------------------------------------

#if 0 // Is this going to be more of an annoyance than not?

struct TranslateWNInfo_ {
  WN* val;
};

sexp::ostream&
operator<<(std::ostream& os, const TranslateWNInfo_& x)
{
  sexp::ostream& sos = dynamic_cast<sexp::ostream&>(os);
  TranslateWN(sos, x.val);
  return sos;
}

inline TranslateWNInfo_ 
TranslateWN(WN* val)
{
  TranslateWNInfo_ x;
  x.val = val;
  return x;
}

#endif

// ---------------------------------------------------------
// GenSexp*: Generate an S-expression for a given object
// ---------------------------------------------------------

struct GenSexpSymInfo_ {
  ST_IDX val;
};
struct GenSexpTyInfo_ {
  TY_IDX val;
};

struct GenSexpWNOprInfo_ {
  WN* val;
};
struct GenSexpSymRefInfo_ {
  ST_IDX val;
};
struct GenSexpSymNmInfo_ {
  ST* val;
};
struct GenSexpTyUseInfo_ {
  TY_IDX val;
};

struct GenSexpFlgInfo_ {
  const char* val;
};
struct GenSexpOpaqueFlgInfo_ {
  UINT64 val;
};


sexp::ostream&
operator<<(std::ostream& os, const GenSexpSymInfo_& x);

inline GenSexpSymInfo_ 
GenSexpSym(ST_IDX val)
{
  GenSexpSymInfo_ x;
  x.val = val;
  return x;
}


sexp::ostream&
operator<<(std::ostream& os, const GenSexpTyInfo_& x);

inline GenSexpTyInfo_ 
GenSexpTy(TY_IDX val)
{
  GenSexpTyInfo_ x;
  x.val = val;
  return x;
}


sexp::ostream&
operator<<(std::ostream& os, const GenSexpWNOprInfo_& x);

inline GenSexpWNOprInfo_ 
GenSexpWNOpr(WN* val)
{
  GenSexpWNOprInfo_ x;
  x.val = val;
  return x;
}


sexp::ostream&
operator<<(std::ostream& os, const GenSexpSymRefInfo_& x);

inline GenSexpSymRefInfo_ 
GenSexpSymRef(ST_IDX val)
{
  GenSexpSymRefInfo_ x;
  x.val = val;
  return x;
}


sexp::ostream&
operator<<(std::ostream& os, const GenSexpSymNmInfo_& x);

inline GenSexpSymNmInfo_ 
GenSexpSymNm(ST* val)
{
  GenSexpSymNmInfo_ x;
  x.val = val;
  return x;
}


sexp::ostream&
operator<<(std::ostream& os, const GenSexpTyUseInfo_& x);

inline GenSexpTyUseInfo_ 
GenSexpTyUse(TY_IDX val)
{
  GenSexpTyUseInfo_ x;
  x.val = val;
  return x;
}


// Cf. GenSexpFlg
sexp::ostream&
operator<<(std::ostream& os, const GenSexpFlgInfo_& x);

inline GenSexpFlgInfo_
GenSexpFlg(const char* val)
{
  GenSexpFlgInfo_ x;
  x.val = val;
  return x;
}


// Cf. GenSexpOpaqueFlg
sexp::ostream&
operator<<(std::ostream& os, const GenSexpOpaqueFlgInfo_& x);

inline GenSexpOpaqueFlgInfo_
GenSexpOpaqueFlg(UINT64 val)
{
  GenSexpOpaqueFlgInfo_ x;
  x.val = val;
  return x;
}


//***************************************************************************

#endif /* wn2sexp_h */
