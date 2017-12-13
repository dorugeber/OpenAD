/* $Id: x_stdio.h,v 1.2 2004-02-28 21:05:05 eraxxon Exp $ */
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
 * ====================================================================
 */

#ifndef x_stdio_h
#define x_stdio_h

/*************************** System Include Files ***************************/

#include <stdio.h>

/**************************** User Include Files ****************************/

/*************************** Forward Declarations ***************************/

/****************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

  /* Unix */
  extern int ux_fileno(FILE *stream);
  
  /* Unix */
  extern FILE *ux_fdopen(int fildes, const char *mode);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif
