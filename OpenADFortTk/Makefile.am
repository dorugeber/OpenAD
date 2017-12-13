
#############################################################################
#
# File:
#   $Source: /Volumes/cvsrep/developer/OpenADFortTk/Makefile.am,v $
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
# Local settings
#############################################################################

# A list of {file ! installation directory} pairs.
MYFILEPAIRS = \
	Sourceme-csh! \
	Sourceme-sh! 

MYINSTALLDIR = $(DESTDIR)$(prefix)

#############################################################################
# Automake rules
#############################################################################

SUBDIRS = \
	doc \
	src 

#############################################################################
# Other rules
#############################################################################

############################################################
# Open64 preparation
############################################################

# NOTE: The 'all-local' target in doc creates a correctly named
# version of Open64's be.so.  We put it there because 1) it should be
# here but 2) automake doesn't have any way of adding a hook that runs
# before all other prerequisites.

############################################################
# Install
############################################################

# *-hook runs after install has completed
install-data-local: myinstall-files

myinstall-files:
	@for pair in $(MYFILEPAIRS); do \
	  src=`echo $${pair} | awk -F'!' '{print $$1}'` ; \
	  dir=`echo $${pair} | awk -F'!' '{print $$2}'` ; \
	  destdir=$(MYINSTALLDIR)/$${dir} ; \
	  $(mkinstalldirs) $${destdir} ; \
	  $(INSTALL_DATA) $(srcdir)/$${src} $${destdir} ; \
	done

.PHONY : install-data-local myinstall-files 
