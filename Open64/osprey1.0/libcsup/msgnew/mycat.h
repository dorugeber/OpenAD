/* $Id: mycat.h,v 1.1 2003-11-07 15:20:41 eraxxon Exp $ */
/* -*-Mode: C;-*- */
/* * BeginRiceCopyright *****************************************************
 * 
 * ******************************************************* EndRiceCopyright */

#ifndef MY_CAT_H
#define MY_CAT_H 1

/*************************** System Include Files ***************************/

#define DISABLE_NL_TYPES_SYMBOL_RENAMING 1 /* nl_types workaround */
#include <nl_types.h> 
#undef DISABLE_NL_TYPES_SYMBOL_RENAMING

/**************************** User Include Files ****************************/

/*************************** Forward Declarations ***************************/

/****************************************************************************/

/* This implements a replacement of the gencat, catopen(), catclose(),
   catgets(), catgetmsg() functionality provided by <nl_types.h> and
   libc.  The motivation is that neither Cygwin nor MacOS provide a
   gencat or catgetmsg(), and we need a quick workaround.
   
   Caveats: 
     - While a full replacement w.r.t. Open64's (English) messages,
     this is not a full scale replacement of the message catalogue
     system.  For example, the messages here are hard coded and there
     is no support for other languages.
     - Another important difference between this implementation and a
     real version is that hard coding the message strings means that
     they are compiled into the binary and reside in core during
     runtime (consuming about 170 Kb).  A real version would allow for
     the messages to be stored on disk.
     
   Data comes from crayf90/fe90/cf90.i which was produced from
     crayf90/fe90/cf90.msgs.

   cf. libcsup/msgnew/catgetmsg.c
   cf. libcsup/msgnew/catmsgfmt.c

*/

#ifdef __cplusplus
extern "C" {
#endif

  extern nl_catd my_catopen(const char *name, int oflag);
  extern int my_catclose(nl_catd catd);

  extern char *my_catgets(nl_catd catd,
			  int set_num,
			  int msg_num,
			  const char *s);
  
  extern char* my_catgetmsg(nl_catd catd, 
			    int set_num, 
			    int msg_num, 
			    char *buf,   
			    int buflen);
  
  extern char* my_catmsgfmt(const char *cmdname,
			    const char *groupcode,
			    int msgnum,
			    const char *severity,
			    const char *msgtext,
			    char *buf,
			    int buflen,
			    const char *position,
			    const char *debug);

#ifdef __cplusplus
}
#endif

/****************************************************************************/

#endif /* MY_CAT_H */
