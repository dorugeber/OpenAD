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
#   HPC custom C++ macros for use with autoconf.
#
#############################################################################

#---------------------------------------------------------------
# HPC_CXX_LIST, HPC_CC_LIST
#---------------------------------------------------------------

# Order of preference:
# 1. Non-GNU vendor compiler (on Linux the GNU compiler is technically
#    the system compiler)
#    - Sun|SGI|Compaq/Alpha, PGI, Intel
# 2. GNU compiler

define([HPC_CXX_LIST], [g++ gcc CC  cxx  pgCC  icpc ecpc icc ecc c++])dnl

define([HPC_CC_LIST], [cc  pgcc  icc ecc  gcc])dnl


#---------------------------------------------------------------
# HPC_CLEAR_CXXFLAGS, HPC_CLEAR_CCFLAGS
#---------------------------------------------------------------

AC_DEFUN([HPC_ENSURE_DEFINED_CXXFLAGS],
  [if test -z "$CXXFLAGS" ; then
     CXXFLAGS=""
   fi
  ])dnl

AC_DEFUN([HPC_ENSURE_DEFINED_CFLAGS],
  [if test -z "$CFLAGS" ; then
     CFLAGS=""
   fi
  ])dnl


#---------------------------------------------------------------
# Check C++ standards-conforming access to C headers: #include <cheader>
#---------------------------------------------------------------

AC_DEFUN([HPC_CHECK_CXX_STDC_HEADERS],
  [AC_CACHE_CHECK(
     [whether ${CXX-CC} supports C++ style C headers (<cheader>)],
     [ac_cv_check_cxx_ctd_cheaders],
     [AC_TRY_COMPILE([
#include <cstdlib>
#include <cstddef>
#include <cstdio>
#include <cassert>
#include <cerrno>

#include <cstdarg>
#include <cstring>
#include <cmath>
#include <csetjmp>
#include <csignal>

#include <cctype>
#include <climits>
#include <cfloat>
#include <clocale>
],
       [std::strcmp("fee fi", "fo fum");],
       [ac_cv_check_cxx_ctd_cheaders="yes"],
       [ac_cv_check_cxx_ctd_cheaders="no"])])
   if test "$ac_cv_check_cxx_ctd_cheaders" = "no" ; then
     AC_DEFINE([NO_STD_CHEADERS], [], [Standard C headers])
   fi])dnl

# Here is an unreliable way of doing this...
#AC_CHECK_HEADERS([cstdlib cstdio cstring cassert],
#                 [AC_DEFINE([HAVE_STD_CHEADERS])],
#                 [AC_DEFINE([NO_STD_CHEADERS])])

#---------------------------------------------------------------
# HPCcxxcmp
#---------------------------------------------------------------

# Macro for MYcxxcmp
# args: ($1): (proposed_compiler)
define([HPCcxxcmp], [MYcxxcmp $CXX $@])dnl
define([HPCcccmp], [MYcxxcmp $CC $@])dnl


# Given the current compiler and a list of proposed compiler names,
# return 0 if there is a match, 1 otherwise
# args: ($1 $2...): (current_compiler, proposed_compiler...)
AC_DEFUN([HPC_DEF_CXXCMP],
  [MYcxxcmp()
   {
     mycxx=$[]1
     shift
  
     while test $[]# -ge 1 ; do
       if ( echo "$mycxx" | grep "$[]1\$" >/dev/null 2>&1 ); then
         return 0
       fi
       shift
     done
     return 1
   }])dnl

