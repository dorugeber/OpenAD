ifndef F90C
F90C=gfortran
endif
RTSUPP=w2f__types OAD_active
driverTLM: $(addsuffix .o, $(RTSUPP)) numCore.pre.xb.x2w.w2f.post.o driverTLM.o 
	${F90C} -o $@ $^

numCore.pre.xb.x2w.w2f.post.f $(addsuffix .f90, $(RTSUPP)) : toolChain 

toolChain : numCore.f
	openad -c -m f $<

numCore.f: all_globals_mod.f head.f
	cat $^ > $@

%.o : %.f90
	${F90C} -o $@ -c $< 

%.o : %.f
	${F90C} -o $@ -c $< 

all_globals_mod.f: 
	ln -s ../$@ ./

head.f: 
	ln -s ../$@ ./

clean: 
	rm -f ad_template* OAD_* w2f__*  iaddr* 
	rm -f numCore.* *.B *.xaif *.o *.mod* driverTLM all_globals_mod.f head.f *~ 
.PHONY: clean toolChain
