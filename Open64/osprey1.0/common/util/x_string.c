/* $Id: x_string.c,v 1.3 2004-02-28 21:05:05 eraxxon Exp $ */
/* -*-Mode: C;-*- */
/* * BeginRiceCopyright *****************************************************
 * 
 * ******************************************************* EndRiceCopyright */

/*************************** System Include Files ***************************/

#include <string.h>

/**************************** User Include Files ****************************/

#include "x_string.h"

/**************************** Forward Declarations **************************/

/****************************************************************************/

extern int ux_strcasecmp(const char *s1, const char *s2) 
{ return strcasecmp(s1, s2); }

extern int ux_strncasecmp(const char *s1, const char *s2, size_t n)
{ return strncasecmp(s1, s2, n); }


extern char *ux_strdup(const char *s1)
{ return strdup(s1); }


extern void *ux_memccpy(void * /*restrict*/ s1, 
			const void * /*restrict*/ s2, 
			int c, size_t n)
{ return memccpy(s1, s2, c, n); }
