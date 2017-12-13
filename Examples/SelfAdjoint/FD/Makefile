ifndef F90C
F90C=gfortran
endif
driverFD: lu.o  driverFD.o head.o 
	${F90C} -o $@ $^
%.o : %.f90
	${F90C} -o $@ -c $< 
%.f90: 
	ln -s ../$@ ./
clean: 
	rm -f *.o *.mod driverFD 

.PHONY: clean 
