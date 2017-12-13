# -*-Mode: makefile;-*-
# $Id: Makefile,v 1.19 2008/02/20 21:31:13 utke Exp $
## * BeginRiceCopyright *****************************************************
## 
## Copyright ((c)) 2002, Rice University 
## All rights reserved.
## 
## Redistribution and use in source and binary forms, with or without
## modification, are permitted provided that the following conditions are
## met:
## 
## * Redistributions of source code must retain the above copyright
##   notice, this list of conditions and the following disclaimer.
## 
## * Redistributions in binary form must reproduce the above copyright
##   notice, this list of conditions and the following disclaimer in the
##   documentation and/or other materials provided with the distribution.
## 
## * Neither the name of Rice University (RICE) nor the names of its
##   contributors may be used to endorse or promote products derived from
##   this software without specific prior written permission.
## 
## This software is provided by RICE and contributors "as is" and any
## express or implied warranties, including, but not limited to, the
## implied warranties of merchantability and fitness for a particular
## purpose are disclaimed. In no event shall RICE or contributors be
## liable for any direct, indirect, incidental, special, exemplary, or
## consequential damages (including, but not limited to, procurement of
## substitute goods or services; loss of use, data, or profits; or
## business interruption) however caused and on any theory of liability,
## whether in contract, strict liability, or tort (including negligence
## or otherwise) arising in any way out of the use of this software, even
## if advised of the possibility of such damage. 
## 
## ******************************************************* EndRiceCopyright *

#############################################################################
# Makefile
#############################################################################

#############################################################################

SHELL = /bin/sh
WD := $(shell pwd)

PLATFORM := $(shell cd $(WD)/config && ./hpcplatform)
ifeq ($(PLATFORM),)
  $(error "Unknown/unsupported platform") # unavailable in older gmakes
  error "Unknown/unsupported platform"    # will certainly cause an error
endif

#############################################################################

XERCESC = xerces-c-src

XERCESCROOT = $(WD)/$(XERCESC)
INSTALLDIR = $(WD)/$(PLATFORM)

# Do not export CC and CXX b/c these are passed via runConfigure 
# export CC
# export CXX
# export CFLAGS
# export CXXFLAGS 
export XERCESCROOT

#############################################################################

# Prepare extra compilation options:
# - For conveniece we use dummy vars (instead of overriding)
# - Sometimes a CXXFLAG is needed for linking, such as SGI's -64 abi.
MYCXXFLAGS := $(CXXFLAGS)
MYLDFLAGS := $(CXXFLAGS)

# - For passing to runConfigure, add '-z ' to each compilation option
# - For passing to runConfigure, add '-l ' to each linking option
MYCXXFLAGS := $(addprefix -z , $(MYCXXFLAGS))
MYLDFLAGS  := $(addprefix -l , $(MYLDFLAGS))

CONF_STATIC_ONLY=-s

# Get the platform for runConfigure
ifeq ($(PLATFORM),alpha-OSF1)
  XERCESPLATFORM = tru64
endif
ifeq ($(PLATFORM),x86-Cygwin)
  XERCESPLATFORM = cygwin
endif
ifeq ($(PLATFORM),x86-Linux)
  XERCESPLATFORM = linux
endif
ifeq ($(PLATFORM),ia64-Linux)
  XERCESPLATFORM = linux
endif
ifeq ($(PLATFORM),x86_64-Linux)
  XERCESPLATFORM = linux
endif
ifeq ($(PLATFORM),x86-MacOS)
  XERCESPLATFORM = macosx
  CONF_STATIC_ONLY=
endif
ifeq ($(PLATFORM),mips-IRIX64)
  XERCESPLATFORM = irix
endif
ifeq ($(PLATFORM),sparc-SunOS)
  XERCESPLATFORM = solaris
endif
ifndef XERCESPLATFORM
  $(error "Error: XERCESPLATFORM not set!")
endif

#############################################################################

all: configure build install

.PHONY : all

#############################################################################

configure: 
	@echo "*** Configuring ***"
	cd $(XERCESCROOT)/src/xercesc && \
	./runConfigure -p $(XERCESPLATFORM) -c $(CC) -x $(CXX) \
	  -m inmem -n socket -t native -r none $(CONF_STATIC_ONLY) -P $(INSTALLDIR) \
	  $(MYCXXFLAGS) $(MYLDFLAGS)

build:
	@echo "*** Building ***"
	cd $(XERCESCROOT)/src/xercesc && $(MAKE)

# The SGI compiler leaves an extra 450 MB of *.pch files that the
# xerces clean command does not remove...
clean:
	if [ -f $(XERCESCROOT)/src/xercesc/Makefile ]; then \
	  cd $(XERCESCROOT)/src/xercesc && $(MAKE) distclean; \
	fi
ifeq ($(PLATFORM),mips-IRIX64)
	find $(XERCESCROOT)/. -name "*.pch" -exec /bin/rm -f \{\} \;
endif

.PHONY : configure build clean

############################################################

install: uninstall
	@echo "*** Installing ***"
	cd $(XERCESCROOT)/src/xercesc && $(MAKE) install

uninstall: 
	/bin/rm -f -r $(INSTALLDIR)

.PHONY : install uninstall

#############################################################################

