ifndef F90C
F90C=gfortran -g -O0
endif
ifndef CC
CC=gcc
endif
RTSUPP=w2f__types OAD_active  

all: driverR driverF

driverR: $(addsuffix .o, $(RTSUPP) OAD_cp OAD_tape OAD_rev iaddr) revolve.o driver.o numCoreR.xb.x2w.w2f.pp.o
	${F90C} -o $@ $^

driverF: $(addsuffix .o, $(RTSUPP) iaddr OAD_tape) driver.o numCoreF.xb.x2w.w2f.pp.o
	${F90C} -o $@ $^

### Reverse
# xaif -> xaif'
numCoreR.xb.xaif : numCore.xaif
	${XAIFBOOSTERROOT}/xaifBooster/algorithms/BasicBlockPreaccumulationReverse/driver/oadDriver -P -f \
	-c ${XAIFSCHEMAROOT}/schema/examples/inlinable_intrinsics.xaif \
	-s ${XAIFSCHEMAROOT}/schema -i $< -o $@ 
# xaif' -> whirl'
numCoreR.xb.x2w.B : numCoreR.xb.xaif  
	${OPENADFORTTKROOT}/bin/xaif2whirl numCore.B $<
# whirl' -> fortran'
numCoreR.xb.x2w.w2f.f: numCoreR.xb.x2w.B
	${OPEN64ROOT}/whirl2f/whirl2f -openad $<
# postprocess 
numCoreR.xb.x2w.w2f.pp.f: numCoreR.xb.x2w.w2f.f
	${OPENADFORTTK_BASE}/tools/SourceProcessing/postProcess.py -m r \
	--infoUnitFile=w2f__types.f90 \
	-i ${OPENADROOT}/runTimeSupport/cpToFile/ad_inline.f \
	-t ${OPENADROOT}/runTimeSupport/simple/ad_template.split.f \
	-o $@ $<

### Forward
# xaif -> xaif'
numCoreF.xb.xaif : numCore.xaif
	${XAIFBOOSTERROOT}/xaifBooster/algorithms/BasicBlockPreaccumulation/driver/oadDriver \
	-c ${XAIFSCHEMAROOT}/schema/examples/inlinable_intrinsics.xaif \
	-s ${XAIFSCHEMAROOT}/schema -i $< -o $@ 
# xaif' -> whirl'
numCoreF.xb.x2w.B : numCoreF.xb.xaif  
	${OPENADFORTTKROOT}/bin/xaif2whirl numCore.B $<
# whirl' -> fortran'
numCoreF.xb.x2w.w2f.f: numCoreF.xb.x2w.B
	${OPEN64ROOT}/whirl2f/whirl2f -openad $<
# postprocess 
numCoreF.xb.x2w.w2f.pp.f: numCoreF.xb.x2w.w2f.f
	${OPENADFORTTK_BASE}/tools/SourceProcessing/postProcess.py -m f \
	--infoUnitFile=w2f__types.f90 \
	-o $@ $<


### Generic
numCore.f90: loopWrapper.f90 loopBody.f90
	cat $^ > $@

# fortran -> whirl
numCore.B: numCore.f90 
	${OPEN64ROOT}/crayf90/sgi/mfef90 -z -F -N132 $<
# whirl -> xaif
numCore.xaif : numCore.B 
	${OPENADFORTTKROOT}/bin/whirl2xaif -n -o $@ $<

w2f__types.f90: ${OPENADROOT}/runTimeSupport/all/w2f__types.f90
	cp -f $< $@

iaddr.c: ${OPENADROOT}/runTimeSupport/all/iaddr.c
	cp -f $< $@

OAD_cp.f90 : ${OPENADROOT}/runTimeSupport/cpToFile/OAD_cp.f90
	cp -f $< $@

OAD_tape.f90 : ${OPENADROOT}/runTimeSupport/cpToFile/OAD_tape.f90
	cp -f $< $@

%.f90: ${OPENADROOT}/runTimeSupport/scalar/%.f90
	cp -f $< $@

%.f90: ${OPENADROOT}/runTimeSupport/simple/%.f90
	cp -f $< $@

revolve.f90: ${REVOLVEF9XROOT}/revolve.f90
	cp -f $< $@

%.o : %.f90
	${F90C} -o $@ -c $< 
%.o : %.f
	${F90C} -o $@ -c $< 
%.o : %.c
	${CC} -o $@ -c $< 
clean: 
	rm -f ad_template* ad_inline.f OAD_* w2f__*  iaddr* oad_cp* revolve*
	rm -f numCore* *.B *.xaif *.o *.mod driver driver? *~ 

.PHONY: all clean 
