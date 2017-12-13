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

driver: obj $(MAIN) quickRegression.bash

$(MAIN).cpp: buildStamp.hpp

CLEANTARGETS+=buildStamp.hpp tmp $(MAIN) quickRegression.bash

objs : ;

.PHONY: objs driver 

quickRegression.bash: 
	ln -sf $(XAIFBOOSTERROOT)/$(ROOTDIR)/system/driver/quickRegression.bash

include ../../../Makefile.incl
-include $(DEPINCL)

# pick up all algorithm specific obj files
ALG_OBJ_SET=$(addsuffix .o, $(addprefix ../src/obj/, $(basename $(notdir $(wildcard ../src/*.cpp)))))

LIBS+=../../InlinableXMLRepresentation/lib/libxaifBoosterInlinableXMLRepresentation.a
LIBS+=../../AdjointUtils/lib/libxaifBoosterAdjointUtils.a
LIBS+=../../CodeReplacement/lib/libxaifBoosterCodeReplacement.a

# setup for ANGEL
ifndef ANGELROOT
  $(error "Error:  environment variable ANGELROOT not defined!")
endif

LIBS+=$(ANGELROOT)/lib/libangel.a

$(MAIN): $(OBJS) $(ALG_OBJ_SET) $(SYSTEM_OBJ_SET) $(LIBS) 
	$(CC) $^ $(LFLAGS) -o $@


