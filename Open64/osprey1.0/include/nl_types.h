
/* This file intercepts any system version of <nl_types.h>.  Not all
   systems support gencat, catgets(), catgetmsg(), etc. and this file
   provides a workaround.
   
   Cygwin and MacOS are examples of systems without gencat functionality.

   See below for workaround solution.
*/

#ifndef NL_TYPES_INTERCEPTOR_H
#define NL_TYPES_INTERCEPTOR_H 1

/****************************************************************************/

/* We replace catopen(), catclose(), catgets(), catgetmsg(), etc. with
   our own version located in
     Open64/osprey1.0/libcsup/msgnew/mycat.*

  Currently we do this on *all* systems.  */

#if !defined(DISABLE_NL_TYPES_SYMBOL_RENAMING)

# define catopen    my_catopen
# define catclose   my_catclose
# define catgets    my_catgets
# define catgetmsg  my_catgetmsg
# define catmsgfmt  my_catmsgfmt

#endif

/****************************************************************************/

/* Declare/define certain things provided by <nl_types.h> */

#if !(defined(__CYGWIN__) || defined(__MACH__))

  /* Include the system's <nl_types.h> */
# include <../../usr/include/nl_types.h>

#else

  /* Must manually provide <nl_types.h> */

  /* The default message set used by the gencat program.  */
# define NL_SETD 1

  /* Value for FLAG parameter of `catgets' to say we want XPG4 compliance.  */
# define NL_CAT_LOCALE 1

  /* Message catalog descriptor type.  */
  typedef void *nl_catd;

  /* Type used by `nl_langinfo'.  */
  typedef int nl_item;


# ifdef __cplusplus
  extern "C" {
# endif
    
    extern nl_catd catopen(const char *name, int oflag);
    extern int catclose(nl_catd catd);
    extern char *catgets(nl_catd catd, int set_num, int msg_num,
			 const char *s);

    /* These stubs can be used as quick workaround. */
#if 0
#   define catopen(cat_name, flag) ((nl_catd)42)
#   define catclose(catalog)       ((int)0)
#   define catgets(catalog, set, number, string) \
      ((char*)"catgets: Bogus message.")
#endif

# ifdef __cplusplus
  }
# endif

  /* Extra declarations provided by <cray/nlcatmsg.h> in appropriate
     source files. */

#endif

/****************************************************************************/

#endif /* NL_TYPES_INTERCEPTOR_H */
