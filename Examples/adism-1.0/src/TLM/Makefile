# Extra variable definitions (from environment)

GLIMMER_LIB_DIR=../glimmerThermal

ifndef UMFPACK_LIB_DIR
UMFPACK_LIB_DIR=../../UMFPACK2.2
endif
ifndef BLAS_LIB_DIR
BLAS_LIB_DIR=../../BLAS
endif
# NetCDF
ifndef NCDF_LIB
NCDF_LIB = /usr/lib
endif
ifndef NCDF_INC
NCDF_INC = /usr/include
endif
ifndef NCDFL
NCDFL = -lnetcdff
endif

# Compiler
ifndef FC
FC=gfortran
endif

RTSUPP=w2f__types OAD_active OAD_cp OAD_tape OAD_rev revStats revolve

adism_TLM: $(addsuffix .o, $(RTSUPP)) ${GLIMMER_LIB_DIR}/libglimtherm.a adism_data.o \
	adism_ncdf.o adism.pre2.xb.x2w.w2f.post.stripped.o \
	adism_TLM.o StubsToActual/adism_umf.o  ${UMFPACK_LIB_DIR}/libumfpack.a
	${FC} -o $@ $^ -L${NCDF_LIB} ${NCDFL} -L${UMFPACK_LIB_DIR} -L${GLIMMER_LIB_DIR} -lglimtherm -lumfpack -L${BLAS_LIB_DIR} -lblas

# preprocess
adism.pre.f90: glimmer_global.F90 glimmer_physcon.F90 glimmer_pdd.F90 glimmer_utils.F90  glimmer_deriv.F90 \
	glimmer_pmpt.F90 glide_tempFullSoln.F90  glide_velo.F90 \
	adism_core.F90 adism_mbal.F90 adism_body.F90 adism_loop.F90 adism_body.F90 adism_wrap.F90 
	${OPENADFORTTK_BASE}/tools/SourceProcessing/preProcess.py --inputFormat=free --progress -m f -o $@ $^

# stubs not needing preprocessing
adism.pre2.f90: ../Adjoint/Stubs/adism_umfStub.f90 ../Adjoint/Stubs/adism_ncdfStub.f90  adism.pre.f90
	cat $^ | sed 's/intent(out)/intent(inout)/' > $@

# fortran -> whirl
adism.pre2.B: adism.pre2.f90 
	${OPEN64ROOT}/crayf90/sgi/mfef90 -z -F -N132 $<

# whirl -> xaif
adism.pre2.xaif : adism.pre2.B 
	${OPENADFORTTKROOT}/bin/whirl2xaif -n -o $@ $<

# xaif -> xaif'
adism.pre2.xb.xaif : adism.pre2.xaif
	${XAIFBOOSTERROOT}/xaifBooster/algorithms/BasicBlockPreaccumulation/driver/oadDriver \
	-c ${XAIFSCHEMAROOT}/schema/examples/inlinable_intrinsics.xaif \
	-s ${XAIFSCHEMAROOT}/schema  -i $< -o $@ 

# xaif' -> whirl'
adism.pre2.xb.x2w.B : adism.pre2.xb.xaif  
	${OPENADFORTTKROOT}/bin/xaif2whirl adism.pre2.B $<

# whirl' -> fortran'
adism.pre2.xb.x2w.w2f.f: adism.pre2.xb.x2w.B
	${OPEN64ROOT}/whirl2f/whirl2f -openad $<

# postprocess 
adism.pre2.xb.x2w.w2f.post.f90: adism.pre2.xb.x2w.w2f.f
	${OPENADFORTTK_BASE}/tools/SourceProcessing/postProcess.py -m f \
	--infoUnitFile=./w2f__types.f90  \
	--outputFormat=free -o $@ $<

# strip out stubs we want to replace with original code
adism.pre2.xb.x2w.w2f.post.stripped.f90 : adism.pre2.xb.x2w.w2f.post.f90
	cat $< | sed '1,/OAD_intrinsics.f90/d' > $@

# strip out umfpack for test
adism.pre3.f90: adism.pre2.f90
	cat $< | sed '1,27 d' > $@

# Get appropriate support code from OpenAD
w2f__types.f90: ${OPENADROOT}/runTimeSupport/all/w2f__types.f90
	cp -f $< $@
OAD_active.f90: ${OPENADROOT}/runTimeSupport/scalar/OAD_active.f90
	cp -f $< $@
OAD_rev.f90: ${OPENADROOT}/runTimeSupport/simple/OAD_rev.f90
	cp -f $< $@
revolve.f90: ${REVOLVEF9XROOT}/revolve.f90
	cp -f $< $@
revStats.f90: ${OPENADROOT}/Examples/ReversalSchemes/Extras/revStats.f90
	cp -f $< $@

# Pattern rule for getting support code
%.f90: ${OPENADROOT}/runTimeSupport/cpToFile/%.f90
	cp -f $< $@

# Pattern rules for building fortran objects
%.o : %.f90
	${FC} -I${NCDF_INC} -I${GLIMMER_LIB_DIR} -cpp -o $@ -c $< 
%.o : %.F90
	${FC} -I${NCDF_INC} -I${GLIMMER_LIB_DIR} -o $@ -c $< 
%.o : %.f
	${FC} -o $@ -c $< 


adism_wrap.F90: ../adism_wrap.F90
	ln -s $< ./$@
adism_body.F90: ../adism_body.F90
	ln -s $< ./$@
adism_loop.F90: ../adism_loop.F90
	ln -s $< ./$@
adism_core.F90: ../adism_core.F90
	ln -s $< ./$@
adism_ncdf.F90: ../adism_ncdf.F90 
	ln -s $< ./$@
adism_mbal.F90: ../adism_mbal.F90
	ln -s $< ./$@
adism_data.F90: ../adism_data.F90
	ln -s $< ./$@
glimmer_log.F90: ${GLIMMER_LIB_DIR}/glimmer_log.F90
	ln -s $< ./$@
glimmer_pdd.F90: ${GLIMMER_LIB_DIR}/glimmer_pdd.F90
	ln -s $< ./$@
glimmer_global.F90: ${GLIMMER_LIB_DIR}/glimmer_global.F90
	ln -s $< ./$@
glimmer_log.F90: ${GLIMMER_LIB_DIR}/glimmer_log.F90
	ln -s $< ./$@
glimmer_physcon.F90: ${GLIMMER_LIB_DIR}/glimmer_physcon.F90
	ln -s $< ./$@
glimmer_utils.F90: ${GLIMMER_LIB_DIR}/glimmer_utils.F90
	ln -s $< ./$@
glimmer_deriv.F90: ${GLIMMER_LIB_DIR}/glimmer_deriv.F90
	ln -s $< ./$@
glimmer_pmpt.F90: ${GLIMMER_LIB_DIR}/glimmer_pmpt.F90
	ln -s $< ./$@
glimmer_mask.F90: ${GLIMMER_LIB_DIR}/glimmer_mask.F90
	ln -s $< ./$@
glide_tempFullSoln.F90: ${GLIMMER_LIB_DIR}/glide_tempFullSoln.F90
	ln -s $< ./$@
glide_velo.F90: ${GLIMMER_LIB_DIR}/glide_velo.F90
	ln -s $< ./$@
adism_umf.f90: ../adism_umf.f90
	ln -s $< ./$@

clean: 
	rm -f OAD_* w2f__* oad_cp.[0-9]* data.dat adism_ncdf.* adism_mbal.F90 adism_data.F90
	rm -f StubsToActual/adism_umf.o
	rm -f adism.* *.B *.xaif *.o *.mod* revolve.f90 revStats.f90
	rm -f adism_umf.f90 glide_velo.F90 glide_tempFullSoln.F90 glimmer_mask.F90 glimmer_pmpt.F90 glimmer_deriv.F90 
	rm -f glimmer_utils.F90 glimmer_physcon.F90 glimmer_log.F90 glimmer_global.F90 adism_mbal.F90 adism_ncdf.F90 
	rm -f adism_body.F90 adism_core.F90 adism_loop.F90 adism_wrap.F90 glimmer_pdd.F90
spotless: clean
	rm -f adism_TLM
.PHONY: clean toolChain

include ${GLIMMER_LIB_DIR}/f90_dep.mak
adism_ncdf.o: adism_ncdf.F90 glimmer_global.o
adism_data.o: adism_data.F90 adism_ncdf.o
