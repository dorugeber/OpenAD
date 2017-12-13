# ========== begin copyright notice ===================
# This file is part of
# ---------------
# xaifBooster
# ---------------
# which is distributed under the BSD license.
# The full COPYRIGHT notice can be found in the top
# level directory of the xaifBooster distribution.
# ========== end copyright notice =====================
include ../Makefile.incl

XMLDIR=../xml
TARGETS=\
helper_declaration.inc \
helper_definition.inc \
helper_initialization.inc \
parse_method_decl.inc \
parse_method_stmt.inc 

all: $(TARGETS)

%.inc : $(XMLDIR)/%.xml
	$(COMPLAIN)
	java -cp $(CLASSPATH) CodeGenerator $<

.PHONY: all 
