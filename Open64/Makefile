#############################################################################

# WARNING: exporting these causes problems with the Open64 build system
#export CC
#export CXX
#export CFLAGS
#export CXXFLAGS

#############################################################################

ifndef OPEN64ROOT
  $(error "Error: OPEN64ROOT not set!")
endif 

ifndef OPEN64TARG
  $(error "Error: OPEN64TARG not set!")
endif

all: open64_fe_build open64_be_build open64_tools_build

open64_fe_build: 
	cd $(OPEN64ROOT)/crayf90/sgi && $(MAKE) 

open64_be_build: 
	cd $(OPEN64ROOT)/whirl2f && $(MAKE)

open64_tools_build: 
	cd $(OPEN64ROOT)/ir_tools && $(MAKE)


clean: 
	cd $(OPEN64ROOT) && $(MAKE) clean 
	cd osprey1.0/tests && $(MAKE) clean

veryclean: clean
	cd $(OPEN64ROOT) && $(MAKE) clobber 

ifndef INST_DIR
INST_DIR=/opt/Open64
endif
INST_EXT=${INST_DIR}/osprey1.0/${OPEN64TARG}

install: uninstall all
	mkdir -p ${INST_EXT}/crayf90/sgi/
	cp -f ${OPEN64ROOT}/crayf90/sgi/mfef90 ${INST_EXT}/crayf90/sgi/
	strip ${INST_EXT}/crayf90/sgi/mfef90
	chmod a+rx ${INST_EXT}/crayf90/sgi/mfef90
	mkdir -p ${INST_EXT}/whirl2f/
	cp -f ${OPEN64ROOT}/whirl2f/whirl2f ${INST_EXT}/whirl2f/
	strip ${INST_EXT}/whirl2f/whirl2f
	chmod a+rx ${INST_EXT}/whirl2f/whirl2f
	cp -f ${OPEN64ROOT}/whirl2f/whirl2f_be ${INST_EXT}/whirl2f/
	strip ${INST_EXT}/whirl2f/whirl2f_be
	chmod a+rx ${INST_EXT}/whirl2f/whirl2f_be
	cp -f ${OPEN64ROOT}/whirl2f/be.so ${INST_EXT}/whirl2f/
	strip ${INST_EXT}/whirl2f/be.so
	chmod a+r ${INST_EXT}/whirl2f/be.so
	cd ${INST_EXT}/whirl2f && ln -sf be.so libbe.so
	cp -f ${OPEN64ROOT}/whirl2f/whirl2f.so ${INST_EXT}/whirl2f/
	strip ${INST_EXT}/whirl2f/whirl2f.so
	chmod a+r ${INST_EXT}/whirl2f/whirl2f.so

uninstall:
	@if [ -d ${INST_DIR} ]; then \
	  echo "about to uninstall ${INST_DIR}"; \
	  rm -rI ${INST_DIR}; \
        fi

.PHONY : open64_fe_build open64_be_build open64_tools_build clean veryclean install uninstall
