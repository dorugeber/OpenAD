ifndef F90C
F90C=gfortran
endif
RTSUPP=w2f__types OAD_active
driverTLM: $(addsuffix .o, $(RTSUPP)) head.pre.xb.x2w.w2f.post.o driverTLM.o init.o
	${F90C} -o $@ $^

head.pre.xb.x2w.w2f.post.f $(addsuffix .f90, $(RTSUPP)) : toolChain 

toolChain : head.f
	openad -c -m f $<

%.o : %.f90
	${F90C} -o $@ -c $< 

%.o : %.f
	${F90C} -o $@ -c $< 

all_globals_mod.f: 
	ln -s ../$@ ./

head.f: 
	ln -s ../$@ ./

init.f:
	ln -s ../$@ ./

clean: 
	rm -f ad_template* OAD_* w2f__*  iaddr* 
	rm -f head.* *.B *.xaif *.o *.mod* driverTLM all_globals_mod.f head.f init.f *~ 
.PHONY: clean toolChain
