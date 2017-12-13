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
# Local settings
#############################################################################

#############################################################################
# Automake rules
#############################################################################

#############################################################################
# Other rules
#############################################################################

# NOTE: This really shouldn't be here.  See note in top-level Makefile.am.

# *-hook runs after all has completed
all-local: myopen64_libbe

############################################################
# Create a correctly named version of Open64's be.so (libbe.so)
############################################################

MYBESO_PATH    = @OPEN64TARG@/whirl2f/be.@HOST_DSOEXT@
MYLIBBESO_PATH = @OPEN64TARG@/whirl2f/libbe.@HOST_DSOEXT@

myopen64_libbe : $(MYLIBBESO_PATH)

$(MYLIBBESO_PATH): $(MYBESO_PATH)
	cp $< $@

.PHONY : myopen64_libbe

