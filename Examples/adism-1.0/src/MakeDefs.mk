# Library locations
# UMFPACK 2.2
ifndef UMFPACK_LIB_DIR
$(error "need UMFPACK_LIB_DIR to be set in the environment pointing to an installation of UMFPACK v 2.2")
endif

# NetCDF (for Ubuntu/Linux Mint)
ifndef NETCDFROOT
NETCDFROOT = /usr
endif
# these depend on how netcdf was configured: 
NETCDFLIBS = -lnetcdff -lnetcdf
NETCDFLIBPATH = -L$(NETCDFROOT)/lib
IPATH+= -I$(NETCDFROOT)/include

# Compiler
ifndef F90C
export F90C=gfortran
endif

ifndef F90CFLAGS
export F90CFLAGS=-g -c $(IPATH)
endif

ifndef MPIFC
MPIFC=mpif90
endif

GLIMMER = $(TOPDIR)/glimmerThermal
GLIMLIB = $(GLIMMER)/libglimtherm.a
IPATH+= -I$(GLIMMER)

LD=$(F90C)
LDFLAGS=-L${UMFPACK_LIB_DIR} -lumfpack  -lblas ${NETCDFLIBPATH} $(NETCDFLIBS)
