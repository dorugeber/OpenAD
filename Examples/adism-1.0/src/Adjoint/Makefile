TOPDIR=..

include $(TOPDIR)/MakeDefs.mk

ifdef DEBUG
PPDEBUG=-v
else 
PPDEBUG=
endif
# NetCDF

EXE = adism_adjoint adism_adjoint_bslide 

RTSUPP=w2f__types OAD_active OAD_cp OAD_tape OAD_rev revolve

ALL: ${GLIMLIB} $(EXE)

adism_adjoint: $(addsuffix .o, $(RTSUPP))  \
	adism_data.o adism_ncdf.o \
	adism.pre2.xb.x2w.w2f.post.stripped.o \
	adism_adjoint.o CP/cpSupport.o 
	$(LD) -o $@ $^ $(LDFLAGS) ${GLIMLIB}

adism_adjoint_bslide: $(addsuffix .o, $(RTSUPP)) \
	adism_data.o  adism_ncdf.o \
	adism_bslide.pre2.xb.x2w.w2f.post.stripped.o \
	adism_adjoint_bslide.o CP/cpSupport.o 
	$(LD) -o $@ $^ $(LDFLAGS) ${GLIMLIB}

# preprocess
adism.pre.f90: glimmer_global.F90  glimmer_physcon.F90  glimmer_utils.F90  glimmer_deriv.F90 \
	glimmer_pmpt.F90 glide_tempFullSoln.F90 glide_velo.F90 glimmer_pdd.F90\
	adism_core.F90 adism_mbal.F90 adism_body.F90 adism_loop.F90 adism_body.F90 adism_wrap.F90	
	${OPENADFORTTK_BASE}/tools/SourceProcessing/preProcess.py --inputFormat=free --progress -m f -o $@ $^

adism_bslide.pre.f90: glimmer_global.F90  glimmer_physcon.F90  glimmer_utils.F90  glimmer_deriv.F90 \
	glimmer_pmpt.F90 glide_tempFullSoln.F90 glide_velo.F90 glimmer_pdd.F90\
	adism_core.F90 adism_mbal.F90 adism_body.F90 adism_loop.F90 adism_body.F90 adism_wrap_bslide.F90	
	${OPENADFORTTK_BASE}/tools/SourceProcessing/preProcess.py --inputFormat=free --progress -m f -o $@ $^

# stubs not needing preprocessing
adism.pre2.f90: Stubs/adism_umfStub.f90 Stubs/adism_ncdfStub.f90  adism.pre.f90
	cat $^ > $@

adism_bslide.pre2.f90: Stubs/adism_umfStub.f90 Stubs/adism_ncdfStub.f90  adism_bslide.pre.f90
	cat $^ > $@

# fortran -> whirl
adism.pre2.B: adism.pre2.f90 
	${OPEN64ROOT}/crayf90/sgi/mfef90 -z -F -N132 $<

adism_bslide.pre2.B: adism_bslide.pre2.f90 
	${OPEN64ROOT}/crayf90/sgi/mfef90 -z -F -N132 $<

# whirl -> xaif
adism.pre2.xaif : adism.pre2.B 
	${OPENADFORTTKROOT}/bin/whirl2xaif -n -o $@ $<

adism_bslide.pre2.xaif : adism_bslide.pre2.B 
	${OPENADFORTTKROOT}/bin/whirl2xaif -n -o $@ $<

# xaif -> xaif'
adism.pre2.xb.xaif : adism.pre2.xaif
	${XAIFBOOSTERROOT}/xaifBooster/algorithms/BasicBlockPreaccumulationReverse/driver/oadDriver \
	-c ${XAIFSCHEMAROOT}/schema/examples/inlinable_intrinsics.xaif \
	-s ${XAIFSCHEMAROOT}/schema -f -I -i $< -o $@ 

adism_bslide.pre2.xb.xaif : adism_bslide.pre2.xaif
	${XAIFBOOSTERROOT}/xaifBooster/algorithms/BasicBlockPreaccumulationReverse/driver/oadDriver \
	-c ${XAIFSCHEMAROOT}/schema/examples/inlinable_intrinsics.xaif \
	-s ${XAIFSCHEMAROOT}/schema -f -I -i $< -o $@ 

# xaif' -> whirl'
adism.pre2.xb.x2w.B : adism.pre2.xb.xaif  
	${OPENADFORTTKROOT}/bin/xaif2whirl adism.pre2.B $<

adism_bslide.pre2.xb.x2w.B : adism_bslide.pre2.xb.xaif  
	${OPENADFORTTKROOT}/bin/xaif2whirl adism_bslide.pre2.B $<

# whirl' -> fortran'
adism.pre2.xb.x2w.w2f.f: adism.pre2.xb.x2w.B
	${OPEN64ROOT}/whirl2f/whirl2f -openad $<

adism_bslide.pre2.xb.x2w.w2f.f: adism_bslide.pre2.xb.x2w.B
	${OPEN64ROOT}/whirl2f/whirl2f -openad $<

# postprocess 
adism.pre2.xb.x2w.w2f.post.f90: adism.pre2.xb.x2w.w2f.f loop_template.revolve.f90  body_template.split.f90 umfpack_template.split.f90
	${OPENADFORTTK_BASE}/tools/SourceProcessing/postProcess.py -m r $(PPDEBUG) \
	-i ${OPENADROOT}/runTimeSupport/cpToFile/ad_inline.f \
	-t ${OPENADROOT}/runTimeSupport/simple/ad_template.split.f \
        --infoUnitFile=./w2f__types.f90  \
	--outputFormat=free  -o $@ $<

adism_bslide.pre2.xb.x2w.w2f.post.f90: adism_bslide.pre2.xb.x2w.w2f.f loop_template.revolve.f90  body_template.split.f90 umfpack_template.split.f90
	${OPENADFORTTK_BASE}/tools/SourceProcessing/postProcess.py -m r $(PPDEBUG) \
	-i ${OPENADROOT}/runTimeSupport/cpToFile/ad_inline.f \
	-t ${OPENADROOT}/runTimeSupport/simple/ad_template.split.f \
        --infoUnitFile=./w2f__types.f90  \
	--outputFormat=free  -o $@ $<

# strip out stubs we want to replace with original code
adism.pre2.xb.x2w.w2f.post.stripped.f90 : adism.pre2.xb.x2w.w2f.post.f90 
	cat $< \
	| sed '/MODULE adism_ncdf/,/end subroutine NETCDF_CLOSE/d' \
	| sed '/end subroutine adism_umf/{n;n;d;}' \
	> $@

adism_bslide.pre2.xb.x2w.w2f.post.stripped.f90 : adism_bslide.pre2.xb.x2w.w2f.post.f90 
	cat $< \
	| sed '/MODULE adism_ncdf/,/end subroutine NETCDF_CLOSE/d' \
	| sed '/end subroutine adism_umf/{n;n;d;}' \
	> $@

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
OAD_cp.f90: ${OPENADROOT}/runTimeSupport/cpToFile/OAD_cp.f90
	cp -f $< $@
revolve.f90: ${REVOLVEF9XROOT}/revolve.f90
	cp -f $< $@

# Pattern rule for getting support code
%.f90: ${OPENADROOT}/runTimeSupport/cpToFile/%.f90
	cp -f $< $@

# Pattern rules for building fortran objects
%.o : %.f90
	$(F90C) $(F90CFLAGS) -o $@ $< 
%.o : %.F90
	$(F90C) $(F90CFLAGS) -o $@ $< 
%.o : %.f
	$(F90C) $(F90CFLAGS) -o $@ $< 

adism_wrap.F90: ../adism_wrap.F90
	ln -s $< ./$@
adism_wrap_bslide.F90: ../adism_wrap_bslide.F90
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
glimmer_log.F90: ${GLIMMER}/glimmer_log.F90
	ln -s $< ./$@
glimmer_pdd.F90: ${GLIMMER}/glimmer_pdd.F90
	ln -s $< ./$@
glimmer_global.F90: ${GLIMMER}/glimmer_global.F90
	ln -s $< ./$@
glimmer_physcon.F90: ${GLIMMER}/glimmer_physcon.F90
	ln -s $< ./$@
glimmer_utils.F90: ${GLIMMER}/glimmer_utils.F90
	ln -s $< ./$@
glimmer_deriv.F90: ${GLIMMER}/glimmer_deriv.F90
	ln -s $< ./$@
glimmer_pmpt.F90: ${GLIMMER}/glimmer_pmpt.F90
	ln -s $< ./$@
glimmer_mask.F90: ${GLIMMER}/glimmer_mask.F90
	ln -s $< ./$@
glide_tempFullSoln.F90: ${GLIMMER}/glide_tempFullSoln.F90
	ln -s $< ./$@
glide_velo.F90: ${GLIMMER}/glide_velo.F90
	ln -s $< ./$@
adism_umf.f90: ../adism_umf.f90
	ln -s $< ./$@
adism_adjoint.f90: ../adism_adjoint.f90
	ln -s $< ./$@

clean: 
	rm -f OAD_* w2f__* oad_cp.[0-9]* oad_cp.tmp data.dat adism_ncdf.* adism_mbal.F90 
	rm -f CP/*.o
	rm -f adism_core.F90 adism_loop.F90 adism_wrap.F90 adism_wrap_bslide.F90 revolve.f90 adism_body.F90 adism_data.F90
	rm -f adism.pre* *.B *.xaif *.o *.mod* 
	rm -f adism_umf.f90 glide_velo.F90 glide_tempFullSoln.F90 glimmer_mask.F90 glimmer_pmpt.F90 glimmer_deriv.F90 
	rm -f glimmer_utils.F90 glimmer_physcon.F90 glimmer_log.F90 glimmer_global.F90  adism_ncdf.F90 glimmer_pdd.F90
	rm -f adism_bslide.pre* adism_G.pre* adism_adjoint.f90

distclean: clean
	rm -f adism_adjoint_bslide adism_adjoint

.PHONY: clean distclean 

include ${GLIMMER}/f90_dep.mak
adism_ncdf.o: adism_ncdf.F90 glimmer_global.o
adism_data.o: adism_data.F90 adism_ncdf.o
