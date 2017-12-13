ifndef F90C
F90C=gfortran
endif
ifndef CC
CC=gcc
endif
RTSUPP=w2f__types OAD_active OAD_cp OAD_tape OAD_rev 
driverADMsplit: $(addsuffix .o, $(RTSUPP) iaddr) lu.o numCore.pre.xb.x2w.w2f.post.o driverADMsplit.o 
	${F90C} -o $@ $^
numCore.pre.xb.x2w.w2f.post.f90 $(addsuffix .f90, $(RTSUPP)) iaddr.c : toolChain 
toolChain : numCore.f90
	openad -c -m rs $<

numCore.f90: luStubs.f90 head.f90
	cat $^ | sed 's/use LU/use LUstubs/' > $@

%.o : %.f90
	${F90C} ${F90FLAGS} -o $@ -c $< 

%.o : %.c
	${CC} -o $@ -c $< 

head.f90 lu.f90: 
	ln -s ../$@ ./

clean: 
	rm -f ad_template* ad_inline.f OAD_* w2f__*  iaddr* head.f90 lu.f90
	rm -f numCore.* *.o *.mod* driverADMsplit *~ 

.PHONY: clean toolChain
