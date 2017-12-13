ifndef F90C
F90C=gfortran
endif

ifndef F90CFLAGS
F90CFLAGS=-O3
endif

all : revolve.o 

%.o: %.f90
	$(F90C) $(F90CFLAGS) -c -o $@ $<

clean: 
	cd Examples && $(MAKE) clean
	rm -f *.o *.mod example

.PHONY: all clean
