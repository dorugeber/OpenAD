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
#   *Process with automake to produce Makefile.in*
#
#   Note: All local variables are prefixed with MY to prevent name
#   clashes with automatic automake variables.
#
#############################################################################

# We do not want the standard GNU files (NEWS README AUTHORS ChangeLog...)
AUTOMAKE_OPTIONS = foreign

#############################################################################
# Automake rules
#############################################################################

SUBDIRS = \
	lib \
	\
	whirl2xaif \
	xaif2whirl

if OPT_ENABLE_WHIRL2SEXP
  SUBDIRS += whirl2sexp
endif

if OPT_ENABLE_SEXP2WHIRL
  SUBDIRS += sexp2whirl
endif

if OPT_ENABLE_TESTERS
  SUBDIRS += testers
endif

#############################################################################
# Other rules
#############################################################################
