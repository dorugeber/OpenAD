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

include ../../Makefile.incl
-include $(DEPINCL)

THELIB=$(UTILS_LIB_DIR)/lib$(UTILS_LIB_NAME).a

CLEANTARGETS+=$(UTILS_LIB_DIR)

objs: obj $(UTILS_LIB_DIR) $(THELIB)

$(THELIB): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

$(UTILS_LIB_DIR):
	mkdir $@

driver : ;

.PHONY: objs driver default

