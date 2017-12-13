ifndef F90C
F90C=gfortran
endif

ifndef F90CFLAGS
F90CFLAGS=-O3
endif

all : example

example: example.o ../../revolve.o
	$(F90C) $(F90CFLAGS) -o $@ $^

%.o: %.f90
	$(F90C) $(F90CFLAGS) -I../../ -c -o $@ $<

clean: 
	rm -f *.o *.mod example

.PHONY: all clean
