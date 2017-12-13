ifndef F90C
F90C=gfortran
endif
driverFD:  all_globals_mod.o driverFD.o head.o
	${F90C} -o $@ $^
%.o : %.f90
	${F90C} -o $@ -c $< 
%.o : %.f
	${F90C} -o $@ -c $< 

# for all_globals_mod.f and head.f which stay the same 
%.f: 
	ln -s ../$@ ./
clean: 
	rm -f *.o *.mod driverFD all_globals_mod.f head.f 
.PHONY: clean 
