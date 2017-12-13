# ========== begin copyright notice ===================
# This file is part of
# ---------------
# xaifBooster
# ---------------
# which is distributed under the BSD license.
# The full COPYRIGHT notice can be found in the top
# level directory of the xaifBooster distribution.
# ========== end copyright notice =====================
CLEANTARGETS+=../classes/*.class

include ../Makefile.incl

all: CodeGenerator.java
	$(COMPLAIN)
	javac CodeGenerator.java 
	mv *.class ../classes

.PHONY: all 
