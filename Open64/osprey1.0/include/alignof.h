/* $Id: alignof.h,v 1.1 2003-12-09 19:09:23 eraxxon Exp $ */
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
 * Standard interface to alignof() function.
 *
 * ====================================================================
 * ==================================================================== */

#ifndef alignof_h
#define alignof_h

/*************************** System Include Files ***************************/

/**************************** User Include Files ****************************/

/*************************** Forward Declarations ***************************/

/****************************************************************************/

/* ALIGNOF(type): return a size_t giving alignment information for type. */
#if defined(__GNUC__)

  /* GCC provides as a compiler builtin */
# define ALIGNOF(x) __alignof__(x)

#else

# if defined(__sun)
   /* Sun compiler provides as builtin */
#  define ALIGNOF(x) __alignof(x)
# elif defined(__sgi)
   /* SGI compiler provides as builtin */
#  define ALIGNOF(x) __builtin_alignof(x)
# elif defined(__alpha)
   /* Compaq compiler does not provide as a builtin */
#  include <alignof_replacement.h> /* for ALIGNOF */
# else
#  error "Please verify definition for alignof()." 
# endif

#endif


/****************************************************************************/

#endif
