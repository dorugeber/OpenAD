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
# Common settings
#############################################################################

include $(top_srcdir)/src/Makeinclude.config

#############################################################################
# Local settings
#############################################################################

MYSOURCES = \
	main.cxx \
	Args.h Args.cxx \
	\
	tester.h tester.cxx

MYIFLAGS = -I@OPEN64BASE@/be/whirl2f -I@OPEN64BASE@/be/whirl2c
MYLDFLAGS = @OPEN64TARG@/whirl2f/whirl2f.@HOST_DSOEXT@

MYCFLAGS   = $(OPENADFORTTK_IFLAGS) \
	@OPEN64_IFLAGS@ $(MYIFLAGS) @OPEN64_CFLAGS@ @OA_IFLAGS@ \
	@HOST_CFLAGS@

MYCXXFLAGS = $(OPENADFORTTK_IFLAGS) \
	@OPEN64_IFLAGS@ $(MYIFLAGS) @OPEN64_CXXFLAGS@ @OA_IFLAGS@ \
	@HOST_CXXFLAGS@

MYLDADD = \
	@HOST_LIBTREPOSITORY@ \
	$(OPENADFORTTK_LIBSUPPORT) \
	@OPEN64_LDFLAGS@ $(MYLDFLAGS) @OA_LDFLAGS@ \
	-lm

# @HOST_WHIRLTESTER_LDFLAGS@

MYCLEAN = @HOST_LIBTREPOSITORY@

#############################################################################
# Automake rules
#############################################################################

bin_PROGRAMS = whirltester

whirltester_SOURCES  = $(MYSOURCES)
whirltester_CFLAGS   = $(MYCFLAGS)
whirltester_CXXFLAGS = $(MYCXXFLAGS)
whirltester_LDFLAGS  = $(MYLDFLAGS)
whirltester_LDADD    = $(MYLDADD)

MOSTLYCLEANFILES = $(MYCLEAN)

#############################################################################
# Common rules
#############################################################################

include $(top_srcdir)/src/Makeinclude.rules

