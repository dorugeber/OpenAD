/* $Id: x_stdio.c,v 1.2 2004-02-28 21:05:05 eraxxon Exp $ */
/* -*-Mode: C;-*- */
/* * BeginRiceCopyright *****************************************************
 * 
 * ******************************************************* EndRiceCopyright */

/*************************** System Include Files ***************************/

#include <stdio.h>

/**************************** User Include Files ****************************/

#include "x_stdio.h"

/**************************** Forward Declarations **************************/

/****************************************************************************/

extern int ux_fileno(FILE *stream)
{ return fileno(stream); }

extern FILE *ux_fdopen(int fildes, const char *mode)
{ return fdopen(fildes, mode); }
