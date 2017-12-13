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
	whirl2xaif.h whirl2xaif.i whirl2xaif.cxx \
	wn2xaif.h wn2xaif.i wn2xaif.cxx \
	wn2xaif_stmt.h wn2xaif_stmt.cxx \
	wn2xaif_expr.h wn2xaif_expr.cxx \
	wn2xaif_mem.h wn2xaif_mem.cxx \
	wn2xaif_io.h wn2xaif_io.cxx \
	st2xaif.h st2xaif.cxx \
	ty2xaif.h ty2xaif.cxx \
	XlationContext.h XlationContext.cxx \
	PUXlationContext.h PUXlationContext.cxx

MYCFLAGS   = $(OPENADFORTTK_IFLAGS) \
	@OPEN64_IFLAGS@ @OPEN64_CFLAGS@ @OA_IFLAGS@ @XERCES_IFLAGS@ \
	@HOST_CFLAGS@

MYCXXFLAGS = $(OPENADFORTTK_IFLAGS) \
	@OPEN64_IFLAGS@ @OPEN64_CXXFLAGS@ @OA_IFLAGS@ @XERCES_IFLAGS@ \
	@HOST_CXXFLAGS@

MYLDADD = \
	@HOST_LIBTREPOSITORY@ \
	$(OPENADFORTTK_LIBSUPPORT) \
	@OPEN64_LDFLAGS@ @OA_LDFLAGS@ @XERCES_LDFLAGS@ \
	-lm

# @HOST_WHIRLXAIF_LDFLAGS@

MYCLEAN = @HOST_LIBTREPOSITORY@

#############################################################################
# Automake rules
#############################################################################

bin_PROGRAMS = whirl2xaif

whirl2xaif_SOURCES  = $(MYSOURCES)
whirl2xaif_CFLAGS   = $(MYCFLAGS)
whirl2xaif_CXXFLAGS = $(MYCXXFLAGS)
whirl2xaif_LDFLAGS  = $(MYLDFLAGS)
whirl2xaif_LDADD    = $(MYLDADD)

MOSTLYCLEANFILES = $(MYCLEAN)

#############################################################################
# Common rules
#############################################################################

include $(top_srcdir)/src/Makeinclude.rules

