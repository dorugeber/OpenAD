/* $Id: x_libgen.c,v 1.2 2004-02-28 21:05:05 eraxxon Exp $ */
/* -*-Mode: C;-*- */
/* * BeginRiceCopyright *****************************************************
 * 
 * ******************************************************* EndRiceCopyright */

/*************************** System Include Files ***************************/

/* #include <libgen.h> */

#include <stdlib.h> 
#include <limits.h>     /* for 'PATH_MAX' */
#include <string.h>     /* for strcpy(), etc. */

/**************************** User Include Files ****************************/

#include "x_libgen.h"

/**************************** Forward Declarations **************************/

/****************************************************************************/

/* Note: Cygwin does not yet support <libgen.h> so we provide our own
   implementation.  This code was lifted from be/be/dra_file_util.cxx. */

static char tempbuf[PATH_MAX];

extern char*
ux_basename(char *const s)
{
  register char *p;
  register char *const t = tempbuf;
  
  if (s == NULL || *s == 0) {
    return strcpy(t, ".");
  } else {
    p = strcpy(t, s);
    p += strlen(p);
    while( p != t  &&  *--p == '/' )        /* skip trailing /s */
      *p = '\0';
    while( p != t ) {
      if( *--p == '/' )
        return  ++p;
    }
    return p;
  }
}


extern char*
ux_dirname(char *const s)
{
  register char *p;
  register char *const t = tempbuf;
  
  if (s == NULL || *s == 0) {
    return strcpy(t, ".");
  } else {
    p = strcpy(t, s);
    p += strlen(p);
    while( p != t  &&  *--p == '/' )        /* skip trailing /s */
      ;
    
    if ( p == t && *p == '/' )
      return strcpy(t, "/");
    
    while( p != t ) {
      if( *--p == '/' ) {
	if ( p == t )
          return strcpy(t, "/");
	while ( *p == '/' )
          p--;
	*++p = '\0';
	return  t;
      }
    }
    return strcpy(t, ".");
  }
}

/****************************************************************************/

/* Here is yet another implementation we found in
   crayf90/sgi/cwh_pdgcs.cxx.  We provide it for reference. 

   Note that these routines make memory management impossible,
   sometimes returning a static or malloc'd string!
*/

#if 0 /* HIDE */

static char *
basename ( char * const s )
{
  char * p;
  char * last;
  char * name;
  int    size;

  if ( s == NULL || *s == '\0' )
    return ".";

  else {

    p = s + strlen ( s );

    /* skip trailing '/' */

    while ( p != s && *p == '/' )
      --p;

    last = p;

    while ( p != s ) {

      if ( *--p == '/' ) {

        ++p;
        break;
      }
    }

    size = last - p;
    name = (char *) malloc ( size + 1);
    strncpy ( name, p, size );
    name [size] = '\0';

    return name;
  }
} /* basename */

static char *
dirname ( char * const s )
{
  char * p;
  char * name;
  int    size;

  if ( s == NULL || *s == '\0' )
    return ".";

  else {

    p = s + strlen ( s );

    /* skip trailing '/' */

    while ( p != s && *p == '/' )
      --p;

    while ( p != s ) {

      if ( *--p == '/' ) {

        if ( p == s )
          return "/";

        while ( *p == '/' )
          --p;

        size = p - s + 1;
        name = (char *) malloc ( size + 1 );
        strncpy ( name, s, size );
	name [size] = '\0';
        return name;
      }
    }

    return ".";
  }
} /* dirname */

#endif /* HIDE */
