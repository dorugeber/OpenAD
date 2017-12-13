ifndef F90C
F90C=gfortran
endif
ifndef CC
CC=gcc
endif
RTSUPP=w2f__types OAD_active OAD_cp OAD_tape OAD_rev revStats revolve
driverADMrevolve: $(addsuffix .o, $(RTSUPP)) numCore.pre.xb.x2w.w2f.post.o driverADMrevolve.o
	${F90C} -o $@ $^
numCore.f: all_globals_mod.f head.f
	cat $^ > $@
# preprocess
numCore.pre.f: numCore.f
	${OPENADFORTTK_BASE}/tools/SourceProcessing/preProcess.py -m r --inputFormat=fixed -o $@ $<
# fortran -> whirl
numCore.pre.B: numCore.pre.f 
	${OPEN64ROOT}/crayf90/sgi/mfef90 -z -F -N132 $<
# whirl -> xaif
numCore.pre.xaif : numCore.pre.B 
	${OPENADFORTTKROOT}/bin/whirl2xaif -n -o $@ $<
# xaif -> xaif'
numCore.pre.xb.xaif : numCore.pre.xaif
	${XAIFBOOSTERROOT}/xaifBooster/algorithms/BasicBlockPreaccumulationReverse/driver/oadDriver \
	-c ${XAIFSCHEMAROOT}/schema/examples/inlinable_intrinsics.xaif \
	-s ${XAIFSCHEMAROOT}/schema -f -i $< -o $@ 
# xaif' -> whirl'
numCore.pre.xb.x2w.B : numCore.pre.xb.xaif  
	${OPENADFORTTKROOT}/bin/xaif2whirl numCore.pre.B $<
# whirl' -> fortran'
numCore.pre.xb.x2w.w2f.f: numCore.pre.xb.x2w.B
	${OPEN64ROOT}/whirl2f/whirl2f -openad $<
# inject template directives
numCore.pre.xb.x2w.w2f.td.f: numCore.pre.xb.x2w.w2f.f injectTemplateDirects.sh
	./injectTemplateDirects.sh $< $@
# postprocess 
numCore.pre.xb.x2w.w2f.post.f90: numCore.pre.xb.x2w.w2f.td.f
	${OPENADFORTTK_BASE}/tools/SourceProcessing/postProcess.py -m r \
	--infoUnitFile=w2f__types.f90 \
	-i ${OPENADROOT}/runTimeSupport/cpToFile/ad_inline.f		\
	--outputFormat=free -o $@ $<
w2f__types.f90: ${OPENADROOT}/runTimeSupport/all/w2f__types.f90
	cp -f $< $@
OAD_active.f90: ${OPENADROOT}/runTimeSupport/scalar/OAD_active.f90
	cp -f $< $@
OAD_rev.f90: ${OPENADROOT}/runTimeSupport/simple/OAD_rev.f90
	cp -f $< $@
revolve.f90: ${REVOLVEF9XROOT}/revolve.f90
	cp -f $< $@
revStats.f90: ../Extras/revStats.f90
	cp -f $< $@
%.f90: ${OPENADROOT}/runTimeSupport/cpToFile/%.f90
	cp -f $< $@
%.o : %.f90
	${F90C} -o $@ -c $< 

%.o : %.f
	${F90C} -o $@ -c $< 

all_globals_mod.f: 
	ln -s ../$@ ./

head.f: 
	ln -s ../$@ ./

clean: 
	rm -f ad_inline.f OAD_* w2f__* oad_cp.0* revStats* revolve*
	rm -f numCore.* *.B *.xaif *.o *.mod* driverADMrevolve all_globals_mod.f head.f *~ 
.PHONY: clean toolChain
