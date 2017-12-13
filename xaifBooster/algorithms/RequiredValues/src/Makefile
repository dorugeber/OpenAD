# ========== begin copyright notice ===================
# This file is part of
# ---------------
# xaifBooster
# ---------------
# which is distributed under the BSD license.
# The full COPYRIGHT notice can be found in the top
# level directory of the xaifBooster distribution.
# ========== end copyright notice =====================

# default target
default: objs

include ../../../Makefile.incl
-include $(DEPINCL)

LIB_NAME=xaifBoosterRequiredValues
LIBDIR=../lib
THELIB=$(LIBDIR)/lib$(LIB_NAME).a

CLEANTARGETS+=$(LIBDIR)

objs: obj $(LIBDIR) $(THELIB)

$(THELIB): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

$(LIBDIR):
	mkdir $@
	
driver : ;

.PHONY: objs driver default

