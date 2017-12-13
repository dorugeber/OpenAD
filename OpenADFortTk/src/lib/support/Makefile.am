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
Open64IRInterface/Open64BasicTypes.h \
Open64IRInterface/Open64IRInterface.cpp Open64IRInterface/Open64IRInterface.hpp \
Open64IRInterface/IntrinsicInfo.cpp Open64IRInterface/IntrinsicInfo.h \
Open64IRInterface/SymTab.cpp Open64IRInterface/SymTab.h \
Open64IRInterface/WhirlGlobalStateUtils.cpp Open64IRInterface/WhirlGlobalStateUtils.h \
Open64IRInterface/WhirlIO.cpp Open64IRInterface/WhirlIO.h \
Open64IRInterface/CleanUpWhirl.cpp Open64IRInterface/CleanUpWhirl.h \
Open64IRInterface/IFDiagnostics.cpp Open64IRInterface/IFDiagnostics.h \
Open64IRInterface/stab_attr.cpp Open64IRInterface/stab_attr.h \
Open64IRInterface/wn_attr.cpp Open64IRInterface/wn_attr.h \
\
BaseMap.h \
CmdLineParser.h CmdLineParser.cxx \
Exception.h Exception.cxx \
Diagnostics.h Diagnostics.cxx  \
IntrinsicXlationTable.h IntrinsicXlationTable.cxx \
OAMaps.h OAMaps.cxx \
quad.h \
ScalarizedRef.h ScalarizedRef.cxx \
ScalarizedRefTab.h ScalarizedRefTab.cxx \
sexpostream.h sexpostream.cxx \
SexpTags.h SexpTags.cxx \
sexputil.h sexputil.cxx \
Symbol.h Symbol.cxx \
WhirlIDMaps.h WhirlIDMaps.cxx \
WhirlParentize.h WhirlParentize.cxx \
XAIFStrings.h XAIFStrings.cxx \
XAIFSymToSymbolMap.h  XAIFSymToSymbolMap.cxx \
xmlostream.h xmlostream.cxx 

MYCFLAGS   = $(OPENADFORTTK_IFLAGS) \
	@OPEN64_IFLAGS@ @OPEN64_CFLAGS@ \
	@OA_IFLAGS@ @XERCES_IFLAGS@ @OPT_SEXP_IFLAGS@ \
	@HOST_CFLAGS@

MYCXXFLAGS = $(OPENADFORTTK_IFLAGS) \
	@OPEN64_IFLAGS@ @OPEN64_CXXFLAGS@ \
	@OA_IFLAGS@ @XERCES_IFLAGS@ @OPT_SEXP_IFLAGS@ \
	@HOST_CXXFLAGS@

if IS_HOST_AR
  MYAR = @HOST_AR@
else
  MYAR = $(AR) cru
endif

MYLIBADD = @HOST_LIBTREPOSITORY@

MYCLEAN = @HOST_LIBTREPOSITORY@

#############################################################################
# Automake rules
#############################################################################

noinst_LIBRARIES = libsupport.a

libsupport_a_SOURCES  = $(MYSOURCES)
libsupport_a_CFLAGS   = $(MYCFLAGS)
libsupport_a_CXXFLAGS = $(MYCXXFLAGS)
libsupport_a_AR       = $(MYAR)
libsupport_a_LIBADD   = $(MYLIBADD)

MOSTLYCLEANFILES = $(MYCLEAN)

#############################################################################
# Common rules
#############################################################################

include $(top_srcdir)/src/Makeinclude.rules

