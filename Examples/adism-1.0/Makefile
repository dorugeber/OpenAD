clean all: 
	@echo "no recursive descend here"

# if the environemt has the definitions of UMFPACK_LIB_DIR, NETCDFROOT and mpi is available:

adism_run: adism_binaries
	cd testConfigurations/eismint2 && ../../bin/adism e2a-sens.config
	cd testConfigurations/eismint2 && ../../bin/adism_FD e2a-sens.config

adism_binaries: 
	cd src && $(MAKE)
	cd src/Adjoint && $(MAKE)
	mkdir -p bin
	cd bin && ln -sf ../src/adism ../src/adism_adjoint ../src/adism_FD ../src/adism_sens ../src/adism_sens_par . ; \
	ln -sf ../src/Adjoint/adism_adjoint ./adism_adjoint_redone ; \
	ln -sf ../src/Adjoint/adism_adjoint_bslide ./adism_adjoint_bslide_redone ; \

adism_clean:
	cd src && $(MAKE) distclean
	rm -rf bin

.PHONY: clean all adism_run adism_binaries adism_clean
