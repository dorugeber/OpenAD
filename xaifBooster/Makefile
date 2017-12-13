# ========== begin copyright notice ===================
# This file is part of
# ---------------
# xaifBooster
# ---------------
# which is distributed under the BSD license.
# The full COPYRIGHT notice can be found in the top
# level directory of the xaifBooster distribution.
# ========== end copyright notice =====================
objs driver clean: 
	cd utils && $(MAKE) $@
	cd system && $(MAKE) $@
	cd algorithms && $(MAKE) $@

doc : 
	doxygen

ifndef INST_DIR
INST_DIR=/opt/xaifBooster
endif

install: uninstall objs
	mkdir -p ${INST_DIR}/algorithms/TypeChange/driver 
	cp -f ${XAIFBOOSTERROOT}/xaifBooster/algorithms/TypeChange/driver/oadDriver ${INST_DIR}/algorithms/TypeChange/driver
	chmod a+rx ${INST_DIR}/algorithms/TypeChange/driver/oadDriver
	strip ${INST_DIR}/algorithms/TypeChange/driver/oadDriver
	mkdir -p ${INST_DIR}/algorithms/BasicBlockPreaccumulation/driver 
	cp -f ${XAIFBOOSTERROOT}/xaifBooster/algorithms/BasicBlockPreaccumulation/driver/oadDriver ${INST_DIR}/algorithms/BasicBlockPreaccumulation/driver
	chmod a+rx ${INST_DIR}/algorithms/BasicBlockPreaccumulation/driver/oadDriver
	strip ${INST_DIR}/algorithms/BasicBlockPreaccumulation/driver/oadDriver
	mkdir -p ${INST_DIR}/algorithms/BasicBlockPreaccumulationReverse/driver 
	cp -f ${XAIFBOOSTERROOT}/xaifBooster/algorithms/BasicBlockPreaccumulationReverse/driver/oadDriver ${INST_DIR}/algorithms/BasicBlockPreaccumulationReverse/driver
	chmod a+rx ${INST_DIR}/algorithms/BasicBlockPreaccumulationReverse/driver/oadDriver
	strip ${INST_DIR}/algorithms/BasicBlockPreaccumulationReverse/driver/oadDriver
	mkdir -p ${INST_DIR}/algorithms/TraceDiff/driver 
	cp -f ${XAIFBOOSTERROOT}/xaifBooster/algorithms/TraceDiff/driver/oadDriver ${INST_DIR}/algorithms/TraceDiff/driver
	chmod a+rx ${INST_DIR}/algorithms/TraceDiff/driver/oadDriver
	strip ${INST_DIR}/algorithms/TraceDiff/driver/oadDriver

uninstall:
	@if [ -d ${INST_DIR} ]; then \
	  echo "about to uninstall ${INST_DIR}"; \
	  rm -rI ${INST_DIR}; \
        fi

.PHONY: objs driver clean doc codegen
