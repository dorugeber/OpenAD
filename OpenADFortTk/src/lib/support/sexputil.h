// -*-Mode: C++;-*-
// $Header: /m_home/m_utkej/Argonne/cvs2svn/cvs/OpenADFortTk/src/lib/support/sexputil.h,v 1.4 2005-02-01 21:52:05 eraxxon Exp $

//***************************************************************************
//
// File:
//   $Source: /m_home/m_utkej/Argonne/cvs2svn/cvs/OpenADFortTk/src/lib/support/sexputil.h,v $
//
// Purpose:
//   Utilities for use with the sexp library.  Essentially, a set of
//   wrappers for accessing data.
//
// Description:
//   [The set of functions, macros, etc. defined in the file]
//
//***************************************************************************

#ifndef sexputil_h
#define sexputil_h

//************************** System Include Files ***************************

#include <inttypes.h>

//*************************** Sexp Include Files ****************************

#include <sexp.h>

//*************************** User Include Files ****************************

//************************** Forward Declarations ***************************

//***************************************************************************

namespace sexp {

// ---------------------------------------------------------
// sexp type (atom, list), 
// ---------------------------------------------------------

inline bool
is_atom(sexp_t* sx) 
{ 
  return (sx->ty == SEXP_VALUE); 
}
  
inline bool 
is_list(sexp_t* sx) 
{ 
  return (sx->ty == SEXP_LIST); 
}

inline bool 
is_null_list(sexp_t* sx) 
{ 
  return (sx->list == NULL); 
}


// ---------------------------------------------------------
// Atom/value operations
// ---------------------------------------------------------

inline bool  
is_atom_basic(sexp_t* sx) 
{ 
  return (sx->aty == SEXP_BASIC); 
}


inline char* 
get_value(sexp_t* sx) 
{ 
  return sx->val; 
}

inline long
get_value_l(sexp_t* sx, long default_val = 0)
{
  long val = default_val;
  if (sx->val) {
    val = strtol(sx->val, (char **)NULL, 0); // select correct base
  } 
  return val;
}

inline unsigned long
get_value_ul(sexp_t* sx, unsigned long default_val = 0)
{  
  unsigned long val = default_val;
  if (sx->val) {
    val = strtoul(sx->val, (char **)NULL, 0); // select correct base
  } 
  return val;
}

inline int32_t
get_value_i32(sexp_t* sx, int32_t default_val = 0)
{
  return (int32_t)get_value_l(sx, default_val);
}

inline uint32_t
get_value_ui32(sexp_t* sx, uint32_t default_val = 0)
{
  return (uint32_t)get_value_ul(sx, default_val);
}

inline int64_t
get_value_i64(sexp_t* sx, int64_t default_val = 0)
{
  int64_t val = default_val;
  if (sx->val) {
    val = (int64_t)strtoll(sx->val, (char **)NULL, 0); // select correct base
  } 
  return val;
}

inline uint64_t
get_value_ui64(sexp_t* sx, uint64_t default_val = 0)
{  
  uint64_t val = default_val;
  if (sx->val) {
    val = (uint64_t)strtoull(sx->val, (char **)NULL, 0); // select correct base
  } 
  return val;
}


// ---------------------------------------------------------
// List operations
// ---------------------------------------------------------

// get_elemx: Given a list, get the nth element in the list (beginning at 0)
inline sexp_t*
get_elem(sexp_t* sx, unsigned int n) 
{ 
  sexp_t* elm = sx->list; // first element
  for (int i = 0; elm && i < n; ++i, elm = elm->next) { }
  return elm;
}

inline sexp_t*
get_elem0(sexp_t* sx) 
{ 
  return sx->list; 
}

inline sexp_t*
get_elem1(sexp_t* sx) 
{ 
  return (sx->list) ? sx->list->next : NULL; 
}
  
inline sexp_t*
get_elem2(sexp_t* sx) 
{
  return (sx->list && sx->list->next) ? sx->list->next->next : NULL; 
}

inline sexp_t*
get_elem3(sexp_t* sx) 
{
  return get_elem(sx, 3);
}


// get_next: Given an element in a list, return its immediate sibling
inline sexp_t*
get_next(sexp_t* sx) 
{
  return sx->next; 
}

}; /* namespace sexp */

//***************************************************************************

namespace sexp {

// routines specific to S-expression WHIRL

inline sexp_t*
get_wnast_attrs(sexp_t* sx)
{
  return get_elem3(sx); // fourth element
}

inline sexp_t*
get_wnast_kid0(sexp_t* sx)
{
  return get_elem(sx, 4); // begin at fifth element
}


}; /* namespace sexp */

//***************************************************************************

#endif /* sexputil_h */
