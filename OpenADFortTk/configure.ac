##########################################################
# This file is part of OpenADFortTk.                     #
# The full COPYRIGHT notice can be found in the top      #
# level directory of the OpenADFortTk source tree.       #
# For more information visit                             #
# http://www.mcs.anl.gov/openad                          #
##########################################################

#############################################################################
#
# Description:
#   autoconf input file for OpenADFortTk.
#   *Process with autoconf to produce configure*
#
# Modification history:
#   2005/06/09 - Nathan Tallent
#
#############################################################################

include(config/hpc-cxxutils.m4)

#----------------------------------------------------------------------------
# Initialization: 
# Require recent autoconf, automake
#----------------------------------------------------------------------------

AC_INIT([OpenADFortTk], [1.0], [utke@mcs.anl.gov], [OpenADFortTk])
AC_CONFIG_AUX_DIR([config])

AC_PREREQ(2.59)
AM_INIT_AUTOMAKE(1.9.5)

AC_CANONICAL_HOST

AC_MSG_NOTICE([INFO: On entry: CXX=${CXX} CXXFLAGS=${CXXFLAGS}])
AC_MSG_NOTICE([INFO: On entry: CC=${CC} CFLAGS=${CFLAGS}])

#----------------------------------------------------------------------------
# Setup
#----------------------------------------------------------------------------

# ${srcdir}, ${ac_top_srcdir}, ${ac_top_builddir}
openadforttk_top=`cd ${srcdir} && pwd`
OPENADFORTTK_PLATFORM=`cd ${openadforttk_top}/config && ./hpcplatform`
AC_SUBST([OPENADFORTTK_PLATFORM])

#----------------------------------------------------------------------------
# Prerequisites: programs, libraries
#----------------------------------------------------------------------------

# AC_PROG_CXX will set CXXFLAGS to something like -g -O2 if not
# already defined.  We do not want this!
HPC_ENSURE_DEFINED_CXXFLAGS
HPC_ENSURE_DEFINED_CFLAGS

# Check for programs.
AC_PROG_CXX(HPC_CXX_LIST)
AC_PROG_CC(HPC_CC_LIST)
AC_PROG_CXXCPP
AC_PROG_RANLIB
AC_PROG_INSTALL

AC_MSG_NOTICE([INFO: After check: CXX=${CXX} CXXFLAGS=${CXXFLAGS}])
AC_MSG_NOTICE([INFO: After check: CC=${CC} CFLAGS=${CFLAGS}])

# Use C++ compiler for all checks
AC_LANG(C++)

# Check for header files
AC_HEADER_STDC
AC_CHECK_HEADERS(unistd.h)
AC_CHECK_HEADERS(inttypes.h)
HPC_CHECK_CXX_STDC_HEADERS

# Check for typedefs, structures, and compiler characteristics.
# -none thus far-

# Check for library functions.
# -none thus far-


#-------------------------------------------------
# Check for Open64
#-------------------------------------------------

# NOTE: we should allow the locations of these libraries to optionally
# be given on configure line or in the environment.

AC_MSG_CHECKING([for Open64 base])
OPEN64BASE="${OPEN64_BASE}/osprey1.0"
if test ! -d "${OPEN64BASE}" ; then 
  AC_MSG_ERROR([Invalid Open64 base path ${OPEN64BASE}])
fi
AC_MSG_RESULT([${OPEN64BASE}])

AC_MSG_CHECKING([for Open64 installation])
OPEN64TARG="${OPEN64ROOT}"
if test ! -d "${OPEN64TARG}" ; then 
  AC_MSG_ERROR([Invalid Open64 installation path ${OPEN64TARG}])
fi
AC_MSG_RESULT([${OPEN64TARG}])

open64_defines="-DIs_True_On -D_LEGO_CLONER -DBACK_END -DMONGOOSE_BE -Dlonglong -D_NEW_SYMTAB -DBUILD_WHIRL2F"

OPEN64_IFLAGS="${open64_defines} -I${OPEN64BASE}/include -I${OPEN64BASE}/common/com -I${OPEN64BASE}/common/com/ia64 -I${OPEN64BASE}/common/util -I${OPEN64BASE}/be/com -I${OPEN64BASE}/be/be -I${OPEN64TARG}/include"

OPEN64_CFLAGS="-D_LANGUAGE_C"
OPEN64_CXXFLAGS="-D_LANGUAGE_C_PLUS_PLUS"
OPEN64_LDFLAGS="-L${OPEN64TARG}/whirl2f -lbe"

AC_SUBST([OPEN64BASE])
AC_SUBST([OPEN64TARG])
AC_SUBST([OPEN64_IFLAGS])
AC_SUBST([OPEN64_CFLAGS])
AC_SUBST([OPEN64_CXXFLAGS])
AC_SUBST([OPEN64_LDFLAGS])

#-------------------------------------------------
# Check for Open Analysis (CFG builder, etc.)
#-------------------------------------------------

AC_MSG_CHECKING([for OpenAnalysis])

OA="${OPENANALYSISROOT}"
if test ! -d "${OA}" ; then 
  AC_MSG_ERROR([Invalid OpenAnalysis installation path ${OA}])
fi

OA_IFLAGS="-DOA_IRHANDLETYPE_UL -I${OA}/include"
OA_LDFLAGS="-L${OA}/lib -lOAul"

AC_MSG_RESULT([${OA}])
AC_SUBST([OA_IFLAGS])
AC_SUBST([OA_LDFLAGS])


#-------------------------------------------------
# Check for Xerces-C (XML parser)
#-------------------------------------------------

AC_MSG_CHECKING([for Xerces-C])

XERCES="${XERCESCROOT}"
if test ! -d "${XERCES}" ; then 
  AC_MSG_ERROR([Invalid Xerces-C installation path ${XERCES}])
fi

XERCES_IFLAGS="-I${XERCES}/include"
XERCES_LDFLAGS="-L${XERCES}/lib -lxerces-c"

AC_MSG_RESULT([${XERCES}])
AC_SUBST([XERCES])
AC_SUBST([XERCES_IFLAGS])
AC_SUBST([XERCES_LDFLAGS])


#----------------------------------------------------------------------------
# Options
#----------------------------------------------------------------------------

#-------------------------------------------------
# with-sexp (S-expression parser)
#-------------------------------------------------

AC_MSG_CHECKING([whether configured with S-expression library])

OPT_HAVE_SEXP=no
OPT_SEXP=
OPT_SEXP_IFLAGS=
OPT_SEXP_LDFLAGS=

AC_ARG_WITH([sexp],
  AC_HELP_STRING([--with-sexp=<sexp_install_path>],
                 [use given sexp installation (absolute path) (default is NO)]),
  [if test $withval != no; then
     OPT_HAVE_SEXP=yes
     if ( echo "${withval}" | grep -v "^/" >/dev/null 2>&1 ); then
       AC_MSG_ERROR([--with-sexp requires absolute path as argument; given '${withval}'])
     fi
     OPT_SEXP=${withval}
   fi],
  [if test -d "${OPENADFORTTK_SEXP}" ; then 
    OPT_HAVE_SEXP=yes
    OPT_SEXP=${OPENADFORTTK_SEXP}
  fi])

if test "${OPT_HAVE_SEXP}" = "yes"; then 
  OPT_SEXP_IFLAGS="-I${OPT_SEXP}"
  OPT_SEXP_LDFLAGS="-L${OPT_SEXP} -lsexp"
  AC_MSG_RESULT([yes (${OPT_SEXP})])
else
  AC_MSG_RESULT([no])
fi

AM_CONDITIONAL(OPT_HAVE_SEXP, test "${OPT_HAVE_SEXP}" = "yes")
AC_SUBST([OPT_SEXP_IFLAGS])
AC_SUBST([OPT_SEXP_LDFLAGS])


#-------------------------------------------------
# enable-develop
#-------------------------------------------------

AC_MSG_CHECKING([whether DEVELOP mode is enabled])

OPT_ENABLE_DEVELOP=no

AC_ARG_ENABLE([develop],
  AC_HELP_STRING([--enable-develop],
                 [Build development version (enable debugging)]),
  [case "${enableval}" in
     yes) OPT_ENABLE_DEVELOP="yes" ;;
     no)  OPT_ENABLE_DEVELOP="no" ;;
     *) AC_MSG_ERROR([bad value ${enableval} for --enable-develop]) ;;
   esac],
  [OPT_ENABLE_DEVELOP=no])

AC_MSG_RESULT([${OPT_ENABLE_DEVELOP}])
AM_CONDITIONAL(OPT_ENABLE_DEVELOP, test "${OPT_ENABLE_DEVELOP}" = "yes")


#-------------------------------------------------
# enable-sexp2whirl: Only build sexp2whirl if sexp library is present
#-------------------------------------------------

AC_MSG_CHECKING([whether to build sexp2whirl])

OPT_ENABLE_SEXP2WHIRL="no"
if test "${OPT_HAVE_SEXP}" = "yes" ; then
  OPT_ENABLE_SEXP2WHIRL="yes"
fi

AC_MSG_RESULT([${OPT_ENABLE_SEXP2WHIRL}])
AM_CONDITIONAL(OPT_ENABLE_WHIRL2SEXP, test "yes" = "yes")
AM_CONDITIONAL(OPT_ENABLE_SEXP2WHIRL, test "${OPT_ENABLE_SEXP2WHIRL}" = "yes")

#-------------------------------------------------
# enable-testers: Only build in develop mode
#-------------------------------------------------

AC_MSG_CHECKING([whether to build testers])

OPT_ENABLE_TESTERS="no"
if test "${OPT_ENABLE_DEVELOP}" = "yes" ; then
  OPT_ENABLE_TESTERS="yes"
fi

AC_MSG_RESULT([${OPT_ENABLE_TESTERS}])
AM_CONDITIONAL(OPT_ENABLE_TESTERS, test "${OPT_ENABLE_TESTERS}" = "yes")

#----------------------------------------------------------------------------
# Host-dependent configuration
# $host: $host_cpu-$host_vendor-$host_os
#----------------------------------------------------------------------------

HPC_DEF_CXXCMP

#-------------------------------------------------
# Host-specific settings for OpenADFortTk
#-------------------------------------------------

# General settings for internal libraries
HOST_CFLAGS=""
HOST_CXXFLAGS=""
HOST_AR=""
HOST_LIBTREPOSITORY=""
HOST_DSOEXT="so"

# Specific settings for programs
HOST_OPEN64_IFLAGS=""
#HOST_WHIRLXAIF_LDFLAGS=""
#HOST_WHIRLSEXP_LDFLAGS=""
#HOST_TESTER_LDFLAGS=""

# Options
if test "$OPT_ENABLE_DEVELOP" = "yes" ; then
  my_opt_cflags="-g -O0"
  my_opt_cxxflags="-g -O0"
  HOST_OPEN64_IFLAGS="${HOST_OPEN64_IFLAGS} -D_DEBUG"
else 
  # On some compilers, -g implies -O0.  Make sure -O2 takes precedence!
  my_opt_cflags="-g -O2"
  my_opt_cxxflags="-g -O2"
fi

case ${host} in

  i386*-*-linux* | i686*-*-linux* | i386*-*-cygwin* | i686*-*-cygwin*)
    if HPCcxxcmp([g++ gcc]); then
      HOST_OPEN64_IFLAGS="${HOST_OPEN64_IFLAGS} -fPIC -D__GNU_BUG_WORKAROUND"
    else
      HOST_OPEN64_IFLAGS="${HOST_OPEN64_IFLAGS} -fPIC"
    fi

    HOST_OPEN64_IFLAGS="${HOST_OPEN64_IFLAGS} -D_LINUX_LINUX -D_SGI_SOURCE -D_SVR4_SOURCE -DTARG_IA64 -DCOMPILE_UPC -D_MIPSEL -D_LONGLONG -D_MIPS_SZINT=32 -D_MIPS_SZPTR=32 -D_MIPS_SZLONG=32 -D__MIPS_AND_IA64_ELF_H"
    ;;

  x86_64*-*-linux* | ia64*-*-linux*)
    ;;

  sparc*-*-solaris*)
    if HPCcxxcmp([CC]); then
      # Sun Forte/ONE
      HOST_OPEN64_IFLAGS="${HOST_OPEN64_IFLAGS} -KPIC -features=rtti"
      HOST_AR="$CXX $CXXFLAGS -xar -o"
      if test "$OPT_ENABLE_DEVELOP" = "yes" ; then
        my_opt_cflags="-g"
        my_opt_cxxflags="-g"
      else 
        my_opt_cflags="-O"
        my_opt_cxxflags="-O"
      fi
    else
      HOST_OPEN64_IFLAGS="${HOST_OPEN64_IFLAGS} -fPIC -D__GNU_BUG_WORKAROUND"
    fi

    HOST_OPEN64_IFLAGS="${HOST_OPEN64_IFLAGS} -D_SOLARIS_SOLARIS -D_SOLARIS -D_SGI_SOURCE -D_SVR4_SOURCE -DTARG_SOLARIS -DCOMPILE_UPC -D_NEW_SYMTAB -D_LONGLONG -Dlonglong -D_LIBELF_XTND_64"
    ;;

  *)
    ;;
esac

 

case ${host} in
  *-*-cygwin* | *-*-cygwin*)
    HOST_DSOEXT="dll"
    ;;
esac

# Grab optimization flags
HOST_CFLAGS="${HOST_CFLAGS} ${my_opt_cflags} ${HOST_OPEN64_IFLAGS}"
HOST_CXXFLAGS="${HOST_CXXFLAGS} ${my_opt_cxxflags} ${HOST_OPEN64_IFLAGS}"

#-------------------------------------------------
# Distribute these settings into makefiles
#-------------------------------------------------
AC_SUBST([HOST_CFLAGS])
AC_SUBST([HOST_CXXFLAGS])
AC_SUBST([HOST_AR])
AC_SUBST([HOST_LIBTREPOSITORY])
AC_SUBST([HOST_DSOEXT])
AM_CONDITIONAL(IS_HOST_AR, test "${HOST_AR}" != "")

#----------------------------------------------------------------------------
# Makefiles to create
#----------------------------------------------------------------------------

AC_OUTPUT([ \
  Makefile \
  doc/Makefile \
  \
  src/Makefile \
  src/lib/Makefile \
  src/lib/support/Makefile \
  src/whirl2sexp/Makefile \
  src/sexp2whirl/Makefile \
  src/whirl2xaif/Makefile \
  src/xaif2whirl/Makefile \
  src/testers/Makefile \
])

