ifndef MPIROOT
 $(error "Error:  environment variable MPIROOT not defined!")
endif

ifndef OPENADROOT
 $(error "Error:  environment variable OPENADROOT not defined!")
endif

F90C     = $(MPIROOT)/bin/mpif90
CC       = $(MPIROOT)/bin/mpicc
FFLAGS   = -g -O0
CFLAGS   = -g -O0

all: driver driverAdm

driver: driver.o compute.o init.o
	$(F90C) -o $@ $^ $(LDFLAGS) $(LIBS)

%.o : %.f90
	$(F90C) $(FFLAGS) -c $<

%.o : %.f
	$(F90C) $(FFLAGS) -c $<

%.o : %.c
	$(CC) $(CFLAGS) -c $<

%.f90 : %.F90 OADmpi.inc
	$(F90C) $(FFLAGS) -E $<  > $@

RTSUPP=w2f__types OAD_active OAD_cp OAD_tape OAD_rev 
driverAdm: $(addsuffix .o, $(RTSUPP) iaddr OADmpiBK) driverAdm.o compute.xb.x2w.w2f.pp.o init.o
	$(F90C) -o $@ $^ $(LDFLAGS) $(LIBS)

%.f90: 
	ln -sf ${OPENADROOT}/runTimeSupport/simple/$@ ./

OAD_active.f90: 
	ln -sf  ${OPENADROOT}/runTimeSupport/scalar/$@ ./

iaddr.c: 
	ln -sf  ${OPENADROOT}/runTimeSupport/all/$@ ./

w2f__types.f90: 
	ln -sf  ${OPENADROOT}/runTimeSupport/all/$@ ./

# F -> WHIRL
compute.B: compute.F90 OADmpi.inc
	${OPEN64ROOT}/crayf90/sgi/mfef90 -DOAD_TRANS -z -I$(MPIROOT)/include -F -N132 $<

# WHIRL -> XAIF
compute.xaif : compute.B 
	${OPENADFORTTKROOT}/bin/whirl2xaif -o $@ $<

# XAIF -> XAIF'
compute.xb.xaif : compute.xaif
	${XAIFBOOSTERROOT}/xaifBooster/algorithms/BasicBlockPreaccumulationReverse/driver/oadDriver -i $< -c ${XAIFSCHEMAROOT}/schema/examples/inlinable_intrinsics.xaif -s ${XAIFSCHEMAROOT}/schema -o $@ -U

# XAIF' -> WHIRL'
compute.xb.x2w.B : compute.xb.xaif  
	${OPENADFORTTKROOT}/bin/xaif2whirl --structured compute.B $<

# WHIRL' -> F'
compute.xb.x2w.w2f.f: compute.xb.x2w.B
	${OPEN64ROOT}/whirl2f/whirl2f -openad $<

# postprocess F'
compute.xb.x2w.w2f.pp.f: compute.xb.x2w.w2f.f 
	${OPENADFORTTK_BASE}/tools/SourceProcessing/postProcess.py -t ${OPENADROOT}/runTimeSupport/simple/ad_template.split.f -i ${OPENADROOT}/runTimeSupport/simple/ad_inline.f --mode=r $< -o $@ 

clean:
	rm -f *.o driver driverAdm compute.x* compute.B compute.f90 *.mod *.mod-whirl OAD_* *.py w2f_* iaddr.c 

.PHONY: all clean

