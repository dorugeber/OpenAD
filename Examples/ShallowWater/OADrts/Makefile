include ../Makefile.inc

build: $(OAD_RTS) $(addsuffix .o, $(basename $(OAD_RTS)))

# pick up the support files from the OpenAD installation
OAD_active.f90:
	ln -sf ${OPENADROOT}/runTimeSupport/scalar/$@ ./
w2f__types.f90 iaddr.c:
	ln -sf ${OPENADROOT}/runTimeSupport/all/$@ ./
timeRatio.c:
	ln -sf ${OPENADROOT}/runTimeSupport/metrics/$@ ./


# specialized F90 compile here is the reason for the separate makefile
# we do not want impose the default bit size on reals required for 
# the other source code 
%.o:%.f90 
	$(F90C) -c $(F90FLAGS) -o $@ $< 

clean:
	rm -f *.o *.mod *.MOD
	-find . -type l -exec rm {} \;

.PHONY : build clean 


