ifndef F90C
F90C=gfortran
endif

ifndef UMFPACK_LIB_DIR
$(error "need UMFPACK_LIB_DIR to be set in the environment pointing to an installation of UMFPACK v 2.2")
endif

RTSUPP=w2f__types OAD_active OAD_tape OAD_rev 
driverADMsplit: $(addsuffix .o, $(RTSUPP)) solve.o test.o numCore.pre.xb.x2w.w2f.post.o driverADMsplit.o 
	${F90C} -o $@ $^ -L${UMFPACK_LIB_DIR} -lumfpack -lblas

# preprocess
numCore.pre.f90: head.f90 Stubs/solve.f90
	${OPENADFORTTK_BASE}/tools/SourceProcessing/preProcess.py -m r -o $@ $^
# fortran -> whirl
numCore.pre.B: numCore.pre.f90 
	${OPEN64ROOT}/crayf90/sgi/mfef90 -z -F -N132 $<
# whirl -> xaif
numCore.pre.xaif : numCore.pre.B 
	${OPENADFORTTKROOT}/bin/whirl2xaif -n -o $@ $<
# xaif -> xaif'
numCore.pre.xb.xaif : numCore.pre.xaif
	${XAIFBOOSTERROOT}/xaifBooster/algorithms/BasicBlockPreaccumulationReverse/driver/oadDriver \
	-c ${XAIFSCHEMAROOT}/schema/examples/inlinable_intrinsics.xaif \
	-s ${XAIFSCHEMAROOT}/schema -f -r -i $< -o $@ 
# xaif' -> whirl'
numCore.pre.xb.x2w.B : numCore.pre.xb.xaif  
	${OPENADFORTTKROOT}/bin/xaif2whirl numCore.pre.B $<
# whirl' -> fortran'
numCore.pre.xb.x2w.w2f.f: numCore.pre.xb.x2w.B
	${OPEN64ROOT}/whirl2f/whirl2f -openad $<
# postprocess 
numCore.pre.xb.x2w.w2f.post.f90: numCore.pre.xb.x2w.w2f.f
	${OPENADFORTTK_BASE}/tools/SourceProcessing/postProcess.py -m r \
	--infoUnitFile=w2f__types.f90 \
	-i ${OPENADROOT}/runTimeSupport/simple/ad_inline.f		\
	-t ${OPENADROOT}/runTimeSupport/simple/ad_template.split.f      \
	--outputFormat=free -o $@ $<
w2f__types.f90: ${OPENADROOT}/runTimeSupport/all/w2f__types.f90
	cp -f $< $@
OAD_active.f90: ${OPENADROOT}/runTimeSupport/scalar/OAD_active.f90
	cp -f $< $@
OAD_tape.f90: ${OPENADROOT}/runTimeSupport/simple/OAD_tape.f90
	cp -f $< $@
OAD_rev.f90: ${OPENADROOT}/runTimeSupport/simple/OAD_rev.f90
	cp -f $< $@
revStats.f90: ../Extras/revStats.f90
	cp -f $< $@

%.o : %.f90
	${F90C} ${F90FLAGS} -o $@ -c $< 

%.o : %.c
	${CC} -o $@ -c $< 

head.f90 solve.f90 test.f90: 
	ln -s ../$@ ./

clean: 
	rm -f ad_template* ad_inline.f OAD_* w2f__*  iaddr* head.f90 solve.f90 test.f90
	rm -f numCore.* *.o *.mod* driverADMsplit *~ 

.PHONY: clean toolChain
