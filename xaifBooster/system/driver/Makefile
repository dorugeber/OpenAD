# ========== begin copyright notice ===================
# This file is part of
# ---------------
# xaifBooster
# ---------------
# which is distributed under the BSD license.
# The full COPYRIGHT notice can be found in the top
# level directory of the xaifBooster distribution.
# ========== end copyright notice =====================
MAIN=oadDriver

driver: obj buildStamp.hpp $(MAIN)

CLEANTARGETS+=buildStamp.hpp tmp $(MAIN)

objs : ;

.PHONY: objs driver 

include ../../Makefile.incl
-include $(DEPINCL)

$(MAIN): obj/$(MAIN).o $(SYSTEM_OBJ_SET) $(LIBS) 
	$(CC) $^ $(LFLAGS) -o $@

