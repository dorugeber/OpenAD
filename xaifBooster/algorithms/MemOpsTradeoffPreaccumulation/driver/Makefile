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
ALG_OBJ_SET= \
$(addsuffix .o, $(addprefix ../src/obj/, $(basename $(notdir $(wildcard ../src/*.cpp))))) \
$(addsuffix .o, $(addprefix ../../BasicBlockPreaccumulation/src/obj/, $(basename $(notdir $(wildcard ../../BasicBlockPreaccumulation/src/*.cpp))))) \
$(addsuffix .o, $(addprefix ../../Linearization/src/obj/, $(basename $(notdir $(wildcard ../../Linearization/src/*.cpp))))) \
$(addsuffix .o, $(addprefix ../../TypeChange/src/obj/, $(basename $(notdir $(wildcard ../../TypeChange/src/*.cpp))))) \

LIBS+=../../InlinableXMLRepresentation/lib/libxaifBoosterInlinableXMLRepresentation.a
LIBS+=../../CrossCountryInterface/lib/libxaifBoosterCrossCountryInterface.a
LIBS+=../../DerivativePropagator/lib/libxaifBoosterDerivativePropagator.a

$(MAIN): $(OBJS) $(ALG_OBJ_SET) $(SYSTEM_OBJ_SET) $(LIBS)
	$(CC) $^ $(LFLAGS) -o $@

