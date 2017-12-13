#################################################################
# This file is part of the RegressionTests suite for OpenAD/F,	#
# which is distributed under the BSD license as detailed in the	#
# COPYRIGHT file in the top level directory of this package.	#
#################################################################

######################################################
# pick your compiler
######################################################
ifndef F90C
F90C=ifort
endif

ifeq ($(F90C),g95)
FIXEDFORMAT=-ffixed-form
FREEFORMAT=-ffree-form
ifdef OPTIMIZE
F90FLAGS=-O3
else
F90FLAGS=-g -O0
endif 
CC=gcc
endif

ifeq ($(F90C),gfortran)
FIXEDFORMAT=-ffixed-form
FREEFORMAT=-ffree-form
ifdef OPTIMIZE
F90FLAGS=-O3
else
F90FLAGS=-g -O0 -pedantic -std=f2003 -Wall -W -fbounds-check -fimplicit-none -Wno-unused
endif 
CC=gcc
endif


ifeq ($(F90C),ifort)
FIXEDFORMAT=-fixed
FREEFORMAT=-free
ifdef OPTIMIZE
F90FLAGS=-O3
else
F90FLAGS=-check all -g -O0 -traceback -fpe0
endif 
CC=icc
endif

ifeq ($(F90C),f95)
FIXEDFORMAT=-fixed
FREEFORMAT=-free -132
ifdef OPTIMIZE
F90FLAGS=-O4 -w=unused
else
F90FLAGS=-g -O0 -strict95 -C=all -gline -ieee=full
endif 
CC=gcc
endif

ifeq ($(F90C),openf95)
FIXEDFORMAT=-fixedform
FREEFORMAT=-freeform
ifdef OPTIMIZE
F90FLAGS=-O3
else
F90FLAGS=-g -O0
endif 
CC=opencc
endif

######################################################

COMPLAIN=
ifndef MAJOR_MODE
COMPLAIN=$(error "Error:  environment variable MAJOR_MODE not defined (set in testAll.py, run with -v)!")
endif
ifndef MINOR_MODE
COMPLAIN=$(error "Error:  environment variable MINOR_MODE not defined (set in testAll.py, run with -v)!")
endif
ifndef SCALAR_OR_VECTOR
COMPLAIN=$(error "Error:  environment variable SCALAR_OR_VECTOR not defined (set in testAll.py, run with -v)!")
endif

ifdef VERBOSE
preProcessVerbFlag=-v
xaifBoosterVerbFlag=-g 1
whirl2xaifVerbFlag=--debug 1
postProcessVerbFlag=-v
endif

# need to put modules in specific order:
MODULES=

ifneq ($(MAJOR_MODE),dd)
MODULES+= w2f__types.o OAD_active.o 
endif

ifeq ($(MAJOR_MODE),adm)
MODULES+=		\
OAD_cp.o		\
OAD_rev.o		\
OAD_tape.o		\
iaddr.o
endif

ifeq ($(MAJOR_MODE),trace)
MODULES+=OAD_rev.o OAD_trace.o
endif

ifeq ($(MAJOR_MODE),dd)
MODULES+=
else ifeq ($(MAJOR_MODE),mf)
MODULES+=
else
MODULES+= OAD_intrinsics.pre.xb.x2w.w2f.post.o
endif

ifeq ($(MAJOR_MODE),dd)
MODULES+= all_globals_mod.o
else ifeq ($(MAJOR_MODE),mf)
MODULES+= all_globals_mod.pre.w2f.o
else
MODULES+= all_globals_mod.pre.xb.x2w.w2f.post.o 
endif

PREPROCESS_FLAGS=
POSTPROCESS_FLAGS=
ifeq ($(MAJOR_MODE),tlm)
PREPROCESS_FLAGS=--mode=f
POSTPROCESS_FLAGS=--mode=f --infoUnitFile w2f__types.f90
endif

ifeq ($(MAJOR_MODE),adm)
PREPROCESS_FLAGS=--mode=r
POSTPROCESS_FLAGS=--mode=r --infoUnitFile w2f__types.f90
endif

CP_FLAG=
ifeq ($(MINOR_MODE),joint)
CP_FLAG=-I
endif

DRIVER_PRE_REQ=$(MODULES)
ifeq ($(MAJOR_MODE),dd)
DRIVER_PRE_REQ+=head.o
else ifeq ($(MAJOR_MODE),mf)
DRIVER_PRE_REQ+=head.pre.w2f.o
else
DRIVER_PRE_REQ+=head.pre.xb.x2w.w2f.post.o
endif 
DRIVER_PRE_REQ+=driver.o

ifndef OPENADROOT
  $(error "Error:  environment variable OPENADROOT not defined!")
endif

ifndef OPEN64ROOT
  $(error "Error:  environment variable OPEN64ROOT not defined!")
endif

ifndef OPENADFORTTK_BASE
  $(error "Error:  environment variable OPENADFORTTK_BASE not defined!")
endif

ifndef OPENADFORTTKROOT
  $(error "Error:  environment variable OPENADFORTTKROOT not defined!")
endif

ifndef XAIFSCHEMAROOT
  $(error "Error:  environment variable XAIFSCHEMAROOT not defined!")
endif

ifndef XAIFBOOSTERROOT
  $(error "Error:  environment variable XAIFBOOSTERROOT not defined!")
endif

LN=ln -sf

RM=rm -rf

######################################################
# default target
######################################################

all: driver

######################################################
# transformation
######################################################

# preprocess F
head_sf.pre.f: all_globals_mod.f head.f preProcess.py
	./preProcess.py $(PREPROCESS_FLAGS) --timing -o $@ $(preProcessVerbFlag) all_globals_mod.f head.f

# F -> WHIRL
head_sf.pre.B: head_sf.pre.f mfef90
	./mfef90 -z -F -N132 $<

# WHIRL -> XAIF
head_sf.pre.xaif : head_sf.pre.B whirl2xaif
	$(COMPLAIN)
	./whirl2xaif $(whirl2xaifVerbFlag) -N -n -o $@ $<

# XAIF -> XAIF'
head_sf.pre.xb.xaif : head_sf.pre.xaif xaif.xsd xaif_base.xsd xaif_inlinable_intrinsics.xsd xaif_derivative_propagator.xsd xaif_output.xsd xaifBooster
	$(COMPLAIN)
	./xaifBooster -v -V -i $< -c ${XAIFSCHEMAROOT}/schema/examples/inlinable_intrinsics.xaif -o $@ $(CP_FLAG) $(xaifBoosterVerbFlag)  ${EXAMPLE_SPECIFIC_XAIFBOOSTER_OPTIONS}

# XAIF' -> WHIRL'
head_sf.pre.xb.x2w.B : head_sf.pre.xb.xaif xaif2whirl
	./xaif2whirl -t OpenADTy_active head_sf.pre.B $<

# WHIRL' -> F'
head_sf.pre.xb.x2w.w2f.f: head_sf.pre.xb.x2w.B whirl2f whirl2f_be
	./whirl2f -openad -openadType OpenADTy_active  $<

# postprocess F'
OAD_intrinsics.pre.xb.x2w.w2f.post.f90 :  head_sf.pre.xb.x2w.w2f.f ad_inline.f postProcess.py w2f__types.f90
	$(COMPLAIN)
	./postProcess.py $(POSTPROCESS_FLAGS) --timing --abstractType OpenADTy_active $< --separateOutput --filenameSuffix=".pre.xb.x2w.w2f.post" --pathSuffix='./' $(postProcessVerbFlag) ${EXAMPLE_SPECIFIC_POSTPROCESS_OPTIONS}

all_globals_mod.pre.xb.x2w.w2f.post.f head.pre.xb.x2w.w2f.post.f : OAD_intrinsics.pre.xb.x2w.w2f.post.f90

# this file is needed only for the comparison to the reference output, we don't do anything else with it 
head_sf.pre.xb.x2w.w2f.post.f : OAD_intrinsics.pre.xb.x2w.w2f.post.f90 all_globals_mod.pre.xb.x2w.w2f.post.f head.pre.xb.x2w.w2f.post.f
	cat $^ > $@

# targets for passing through Pre/PostProcessor and mfef90 exclusively ###################

# WHIRL -> F
head_sf.pre.w2f.f: head_sf.pre.B whirl2f whirl2f_be
	$(COMPLAIN)
	./whirl2f -openad $<

# extract the transformed globals module so we the right order of 
# compilation with respect to the globals checkpoint module
all_globals_mod.pre.w2f.f: head_sf.pre.w2f.f
	cat $< | sed -n '/MODULE all_globals_mod/,/END MODULE/p' > $@

# remove the transformed globals module from the transformed head file
head.pre.w2f.f: head_sf.pre.w2f.f
	cat $< | sed '/MODULE all_globals_mod/,/END MODULE/d' > $@

##########################################################################################

# setup some links
%.xsd:
	@$(LN) ${XAIFSCHEMAROOT}/schema/$@ .

mfef90: 
	@$(LN) ${OPEN64ROOT}/crayf90/sgi/mfef90 .

whirl2xaif xaif2whirl whirl2sexp sexp2whirl: 
	@$(LN) ${OPENADFORTTKROOT}/bin/$@ .

%.py:
	@$(LN) ${OPENADFORTTK_BASE}/tools/SourceProcessing/$@ .

whirl2f whirl2f_be:
	@$(LN) ${OPEN64ROOT}/whirl2f/$@ .

ir_b2a: 
	@$(LN) ${OPEN64ROOT}/ir_tools/ir_b2a

w2f__types.f90 iaddr.c:
	@$(LN) ${OPENADROOT}/runTimeSupport/all/$@ .

ad_inline.f OAD_cp.f90 OAD_rev.f90 OAD_tape.f90 OAD_trace.f90:
	@$(LN) ${OPENADROOT}/runTimeSupport/simple/$@ .

# extra module dependency
OAD_cp.o: OAD_cp.f90 OAD_active.o

OAD_active.f90:
	$(COMPLAIN)
	@$(LN) ${OPENADROOT}/runTimeSupport/${SCALAR_OR_VECTOR}/$@ .

######################################################
# compilation
######################################################

# make the driver
driver: $(DRIVER_PRE_REQ) 
	$(COMPLAIN)
	$(F90C) $(F90FLAGS) $(DRIVER_PRE_REQ) -o $@ 

run: params.conf tmpOutput driver 
	./driver

%.o:%.f
	$(F90C) $(F90FLAGS) $(FIXEDFORMAT) -c $<

%.o:%.f90
	$(F90C) $(F90FLAGS) $(FREEFORMAT) -c $<

%.o:%.c
	$(CC) -c $<

######################################################
# miscellaneous
######################################################

tmpOutput: 
	@mkdir -p $@

minClean:
	@$(RM) OAD_intrinsics.* OAD_active.* head_sf.* head.* driver* params.conf all_globals_* ad_template.f *mod-whirl tmpOutput data.tmp driver.o all_globals_mod.xb.x2w.w2f.post.o head.xb.x2w.w2f.post.o

groupClean: minClean
	@$(RM) *.o

clean: groupClean
	$(RM) *.xsd 
	$(RM) mfef90 whirl2xaif whirl2sexp sexp2whirl ir_b2a xaif2whirl whirl2f_be whirl2f *Process.py xaifBooster .lastRun w2f__types.f90 iaddr.c ad_inline.f OAD_active.f90 OAD_cp.f90 OAD_rev.f90 OAD_tape.f90 OAD_trace.f90 oad_tr_*.xml
	$(RM) *.mod *.o
	$(RM) plotOutput

.PHONY: all minClean groupClean clean run
