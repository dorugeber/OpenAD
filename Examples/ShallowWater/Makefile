include Makefile.inc

# pick up run time support mod files
MODPATH+=$(MODPATHFLAG)./OADrts

# modules to push through OpenAD
SRCMODS = 	\
adsize.f90 	\
size.f90 	\
parms.f90 	\
vars.f90 	\
pfields.f90 	\
force.f90 	\
data.f90 	\
weights.f90 	\
mini.f90 	\
size_small.f90

# code to push through OpenAD
# but we filter out  I/O logic
SRCFILES = 			\
inifields.F 			\
readparms.F 			\
iowrapper.F 			\
read_data.F 			\
make_weights.F 			\
map_control_vector.F 		\
small_routines_split.F 		\
small_routines_joint.F 		\
cost_function.F 		\
cost_depth.F 			\
loop_body_wrapper_inner.F 	\
loop_body_wrapper_outer.F 	\
forward_model.F 

default: oad_rts dataLinks Run/shallow_water_ad


dataLinks: Run
	ln -sf ../Data/data Run/data
	ln -sf ../Data/data.orig Run/data.orig
	ln -sf ../Data/restart_long.nc Run/restart_long.nc
	ln -sf ../Data/weights_depth_diff.nc Run/weights_depth_diff.nc
	ln -sf ../Data/weights_depth.nc Run/weights_depth.nc

Run : 
	mkdir Run

###################################################################
# shallow_water_ad
###################################################################

OAD_RTS_DIR=OADrts

oad_rts: 
	cd $(OAD_RTS_DIR) && $(MAKE)

# build the binary
Run/shallow_water_ad: $(addprefix $(OAD_RTS_DIR)/,$(addsuffix .o, $(basename $(OAD_RTS)))) \
	              numericalCore.xb.x2w.w2f.pp.o \
		      ionetcdf.o \
                      main.prepped.o
	$(FC) $(LFLAGS) -o $@ $^ $(LIBS)  

# the stages of the tool pipeline: 

# concatenate all sources into a single file
numericalCore.f : $(SRCMODS) $(SRCFILES:.F=.f)
	cat $^ > $@
# fortran -> whirl
numericalCore.B: numericalCore.f 
	${OPEN64ROOT}/crayf90/sgi/mfef90 -z -F -N132 -r8 $<
# whirl -> xaif
numericalCore.xaif : numericalCore.B 
	${OPENADFORTTKROOT}/bin/whirl2xaif -o $@ $<
# xaif -> xaif'
numericalCore.xb.xaif : numericalCore.xaif
	${XAIFBOOSTERROOT}/xaifBooster/algorithms/BasicBlockPreaccumulationReverse/driver/oadDriver -i $< -c ${XAIFSCHEMAROOT}/schema/examples/inlinable_intrinsics.xaif -s ${XAIFSCHEMAROOT}/schema -o $@ -U
# xaif' -> whirl'
numericalCore.xb.x2w.B : numericalCore.xb.xaif  
	${OPENADFORTTKROOT}/bin/xaif2whirl numericalCore.B $<
# whirl' -> fortran'
numericalCore.xb.x2w.w2f.f: numericalCore.xb.x2w.B
	${OPEN64ROOT}/whirl2f/whirl2f -openad $<
# postprocess 
numericalCore.xb.x2w.w2f.pp.f: numericalCore.xb.x2w.w2f.f
	${OPENADROOT}/OpenADFortTk/tools/SourceProcessing/postProcess.py -m r \
	--infoUnitFile=${OAD_RTS_DIR}/w2f__types.f90 \
	-i ${OAD_RTS_DIR}/ad_inline.f -t ${OAD_RTS_DIR}/ad_template.split.f \
	-o $@ $< 

# compile this file
numericalCore.xb.x2w.w2f.pp.o: numericalCore.xb.x2w.w2f.pp.f
	$(F90C) $(FIXEDFORMAT) $(F90FLAGS) $(FFLAGS) $(MODPATH) -c  -o $@ $<

###################################################################
# generic things 
###################################################################

# cancel the built in rule: 
%.o:%.F 

# F77 preprocessor rule: 
%.f:%.F
	$(FPP) $(FPPFLAGS) $(IPATH) -o $@ $< 

# F77 compile rule
%.o:%.f 
	$(FC) $(FIXEDFORMAT) $(FFLAGS) $(MODPATH) -c  -o $@ $< 

# F90 compile rule
%.o:%.f90 
	$(F90C) $(F90FLAGS) $(FFLAGS) $(MODPATH) -c -o $@ $< 

# C compile rule
%.o:%.c 
	$(CC) -c $(CFLAGS) -o $@ $< 

clean:	
	cd $(OAD_RTS_DIR) && $(MAKE) clean
	rm -f numericalCore* 
	rm -f *.f *.o *.mod *.mod-whirl 
	rm -rf Run

.SUFFIXES: .f90 .F .f .o 

all: default Run/shallow_water

.PHONY : default all oad_rtsLinks dataLinks clean 

###################################################################
# original shallow_water 
###################################################################

Run/shallow_water: $(SRCMODS:.f90=.o) $(SRCFILES:.F=.o) ionetcdf.o main.o 
	$(FC) $(LFLAGS) -o $@ $^ $(LIBS)

