/* $Id: x_stdlib.c,v 1.2 2004-02-28 21:05:05 eraxxon Exp $ */
/* -*-Mode: C;-*- */
/* * BeginRiceCopyright *****************************************************
 * 
 * ******************************************************* EndRiceCopyright */

/*************************** System Include Files ***************************/

#include <stdlib.h>

/**************************** User Include Files ****************************/

#include "x_stdlib.h"

/**************************** Forward Declarations **************************/

/****************************************************************************/

extern int ux_mkstemp(char *strtemplate)
{ return mkstemp(strtemplate); }

