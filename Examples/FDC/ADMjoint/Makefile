ifndef F90C
F90C=gfortran
endif
ifndef CC
CC=gcc
endif
RTSUPP=w2f__types OAD_active OAD_cp OAD_tape OAD_rev 
driverADMjoint: $(addsuffix .o, $(RTSUPP) iaddr) head.pre.xb.x2w.w2f.post.o driverADMjoint.o init.o
	${F90C} -o $@ $^
head.pre.xb.x2w.w2f.post.f $(addsuffix .f90, $(RTSUPP)) iaddr.c : toolChain 
toolChain : head.f
	openad -c -m rj $<

%.o : %.f90
	${F90C} -o $@ -c $< 

%.o : %.f
	${F90C} -o $@ -c $< 

%.o : %.c
	${CC} -o $@ -c $< 

init.f: 
	ln -s ../$@ ./

head.f: 
	ln -s ../$@ ./

clean: 
	rm -f ad_template* ad_inline.f OAD_* w2f__*  iaddr* 
	rm -f head.* *.B *.xaif *.o *.mod* driverADMjoint init.f *~ 
.PHONY: clean toolChain
