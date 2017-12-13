ifndef OPENADROOT
 $(error "Error:  environment variable OPENADROOT not defined!")
endif

ifndef F90C    
F90C  = gfortran
endif
FFLAGS   = -g -O0
LDFLAGS  = 
LIBS     =
TARGETS=driver driverFd driverAdm

all: $(TARGETS)

driver: driver.o compute.o init.o
	$(F90C) -o $@ $^ $(LDFLAGS) $(LIBS)

driverFd: driverFd.o compute.o init.o
	$(F90C) -o $@ $^ $(LDFLAGS) $(LIBS)

%.o : %.f90
	$(F90C) $(FFLAGS) -c $<

%.o : %.f
	$(F90C) $(FFLAGS) -c $<

RTSUPP=w2f__types OAD_active OAD_cp OAD_tape OAD_rev 
driverAdm: $(addsuffix .o, $(RTSUPP) iaddr) compute.xb.x2w.w2f.pp.o driverAdm.o init.o
	${F90C} -o $@ $^

compute.xb.x2w.w2f.pp.f $(addsuffix .f90, $(RTSUPP)) iaddr.c : toolChain 

toolChain : compute.f90
	openad -c -m rs $<

clean:
	rm -f $(TARGETS) *.o compute.x* compute.B *.mod *.mod-whirl *.log~
	rm -f $(addsuffix .f90, $(RTSUPP)) iaddr.c ad_inline.f ad_template.f

.PHONY: all clean toolChain
