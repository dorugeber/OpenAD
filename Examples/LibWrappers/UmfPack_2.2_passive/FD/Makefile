ifndef F90C
F90C=gfortran
endif

ifndef UMFPACK_LIB_DIR
$(error "need UMFPACK_LIB_DIR to be set in the environment pointing to an installation of UMFPACK v 2.2")
endif

driverFD: solve.o test.o head.o driverFD.o
	${F90C} -o $@ $^ -L${UMFPACK_LIB_DIR} -lumfpack -lblas
%.o : %.f90
	${F90C} -o $@ -c $< 
%.f90: 
	ln -s ../$@ ./
clean: 
	rm -f *.o *.mod driverFD 

.PHONY: clean 
