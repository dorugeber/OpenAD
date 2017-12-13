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
	AdjustInterfaces.h AdjustInterfaces.cxx \
	AdjustFunctions.h AdjustFunctions.cxx \
	InterfaceData.h InterfaceData.cxx \
	\
	xaif2whirl.h xaif2whirl.cxx \
	XlateStmt.h XlateStmt.cxx \
	XlateExpression.h XlateExpression.cxx \
	XlationContext.h XlationContext.cxx \
	PUXlationContext.h PUXlationContext.cxx \
	\
	XAIF_DOMErrorHandler.h XAIF_DOMErrorHandler.cxx \
	XAIF_SAXErrorHandler.h XAIF_SAXErrorHandler.cxx \
	XAIF_DOMFilters.h XAIF_DOMFilters.cxx \
	XAIF_SAXHandler.h XAIF_SAXHandler.cxx \
	XercesStrX.h XercesStrX.cxx

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

bin_PROGRAMS = xaif2whirl

xaif2whirl_SOURCES  = $(MYSOURCES)
xaif2whirl_CFLAGS   = $(MYCFLAGS)
xaif2whirl_CXXFLAGS = $(MYCXXFLAGS)
xaif2whirl_LDFLAGS  = $(MYLDFLAGS)
xaif2whirl_LDADD    = $(MYLDADD)

MOSTLYCLEANFILES = $(MYCLEAN)

#############################################################################
# Common rules
#############################################################################

include $(top_srcdir)/src/Makeinclude.rules

