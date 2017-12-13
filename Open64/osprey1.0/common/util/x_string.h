/* $Id: x_string.h,v 1.2 2004-02-28 21:05:05 eraxxon Exp $ */
/* -*-Mode: C;-*- */
/* * BeginRiceCopyright *****************************************************
 * 
 * ******************************************************* EndRiceCopyright */

/* ====================================================================
 * ====================================================================
 *
 *
 * Nathan Tallent.
 *
 * Description:
 *
 * Header file extensions.
 *
 * The ANSI/ISO C++ standard does not include every routine that is
 * legally available in ANSI/ISO C.  Moreover, neither C++ nor C
 * include every routine within the standard UNIX specification
 * (OpenGroup).  This file provides that extra functionality for
 * ANSI/ISO C++ and C code.
 *
 * Routines available in C are prefixed by c_.
 * Routines available in Unix are prefixed by ux_
 *
 * ====================================================================
 * ==================================================================== */

#ifndef x_string_h
#define x_string_h

/*************************** System Include Files ***************************/

#include <string.h>

/**************************** User Include Files ****************************/

/*************************** Forward Declarations ***************************/

/****************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif
  
  /* Unix */
  extern int ux_strcasecmp(const char *s1, const char *s2);
  extern int ux_strncasecmp(const char *s1, const char *s2, size_t n);

  /* Unix */
  extern char *ux_strdup(const char *s1);

  /* Unix */
  extern void *ux_memccpy(void * /*restrict*/ s1, 
			  const void * /*restrict*/ s2, 
			  int c, size_t n);
  
#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif
