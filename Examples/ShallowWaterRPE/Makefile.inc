FC          	= gfortran
FFLAGS      	= -fdefault-real-8 -fdefault-double-8 -g -O0
F90C        	= $(FC)
F90FLAGS    	=
MODPATH         =
MODPATHFLAG     = -I
FIXEDFORMAT 	= -ffixed-form 
CC 		= gcc
CFLAGS  	= -O0 -g
FPP     	= cpp
FPPFLAGS 	= -traditional-cpp
ifdef NETCDFROOT
LFLAGS 	        = -L${NETCDFROOT}/lib
else
LFLAGS          =
endif
IPATH   	= -I. -I./Inc
LIBS    	= -lnetcdff -lnetcdf

ifndef OPENADROOT
$(error environment variable OPENADROOT is undefined, this example requires it)
endif

# OpenAD runtime support files 
OAD_RTS = \
w2f__types.f90 \
OAD_active.f90 \
OAD_cp.f90 \
OAD_tape.f90 \
OAD_rev.f90 \
iaddr.c \
timeRatio.c  
