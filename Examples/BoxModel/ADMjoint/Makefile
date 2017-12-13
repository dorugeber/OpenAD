ifndef F90C
F90C=gfortran
endif
ifndef CC
CC=gcc
endif
RTSUPP=w2f__types OAD_active OAD_cp OAD_tape OAD_rev 
driverADMjoint: $(addsuffix .o, $(RTSUPP) iaddr) numCore.pre.xb.x2w.w2f.post.o driverADMjoint.o
	${F90C} -o $@ $^
numCore.pre.xb.x2w.w2f.post.f $(addsuffix .f90, $(RTSUPP)) iaddr.c : toolChain 
toolChain : numCore.f
	openad -c -m rj $<

numCore.f: all_globals_mod.f head.f
	cat $^ > $@

%.o : %.f90
	${F90C} -o $@ -c $< 

%.o : %.f
	${F90C} -o $@ -c $< 

%.o : %.c
	${CC} -o $@ -c $< 

all_globals_mod.f: 
	ln -s ../$@ ./

head.f: 
	ln -s ../$@ ./

clean: 
	rm -f ad_template* ad_inline.f OAD_* w2f__*  iaddr* 
	rm -f numCore.* *.B *.xaif *.o *.mod* driverADMjoint all_globals_mod.f head.f *~ 
.PHONY: clean toolChain
