# ========== begin copyright notice ==============
# This file is part of 
# ---------------
# xaifBooster
# ---------------
# Distributed under the BSD license as follows:
# Copyright (c) 2005, The University of Chicago
# All rights reserved.
#
# Redistribution and use in source and binary forms, 
# with or without modification, are permitted provided that the following conditions are met:
#
#    - Redistributions of source code must retain the above copyright notice, 
#      this list of conditions and the following disclaimer.
#    - Redistributions in binary form must reproduce the above copyright notice, 
#      this list of conditions and the following disclaimer in the documentation 
#      and/or other materials provided with the distribution.
#    - Neither the name of The University of Chicago nor the names of its contributors 
#      may be used to endorse or promote products derived from this software without 
#      specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
# SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# 
# General Information:
# xaifBooster is intended for the transformation of 
# numerical programs represented as xml files according 
# to the XAIF schema. It is part of the OpenAD framework. 
# The main application is automatic 
# differentiation, i.e. the generation of code for 
# the computation of derivatives. 
# The following people are the principal authors of the 
# current version: 
# 	Uwe Naumann
#	Jean Utke
# Additional contributors are: 
#	Andrew Lyons
#	Peter Fine
#
# For more details about xaifBooster and its use in OpenAD please visit:
#   http://www.mcs.anl.gov/openad
#
# This work is partially supported by:
# 	NSF-ITR grant OCE-0205590
# ========== end copyright notice ==============
######################################################
# pick your compiler
######################################################
#FIXEDFORMAT=-ffixed-form
#FREEFORMAT=-ffree-form
#F95=g95 -g -O0 
#CC=gcc

#FIXEDFORMAT=-ffixed-form
#FREEFORMAT=-ffree-form
#F95=gfortran -g -O0 
#CC=gcc

FIXEDFORMAT=-fixed
FREEFORMAT=-free
F95=ifort -check all -g -fpe0 -O0 -traceback 
CC=icc

#FIXEDFORMAT=-fixed
#FREEFORMAT=-free
#F95=f95 -g -w=unused
#CC=gcc
######################################################

ifdef REVERSE_MODE
MODULES=                        \
w2f__types.o                    \
active_module.o                 \
OpenAD_checkpoints.o            \
OpenAD_dct.o                    \
OpenAD_rev.o                    \
OpenAD_tape.o                   \
iaddr.o                         \
all_globals_mod.xb.x2w.w2f.pp.o 
PP_FLAGS=
ifeq ($(SUB_MODE),joint)
CP_FLAG=-I
else
CP_FLAG=
endif
else
MODULES=                        \
w2f__types.o                    \
active_module.o                 \
all_globals_mod.xb.x2w.w2f.pp.o 
PP_FLAGS=-f
endif

all: head.twice.f

ifndef OPENADROOT
  $(error "Error:  environment variable OPENADROOT not defined!")
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
# transformation part 1
######################################################

# paste the globals module into the head file 
# so we can do the single file transformation
head_sf.f : all_globals_mod.f head.f 
	cat $^ > $@

# F -> WHIRL
head_sf.B: head_sf.f mfef90
	./mfef90 -F -N132 $<

# WHIRL -> XAIF
head_sf.xaif : head_sf.B whirl2xaif
	./whirl2xaif -n $< | sed 's/prefix=\"OpenAD_\"/prefix="oad"/' > $@ 

# XAIF -> XAIF'
head_sf.xb.xaif : head_sf.xaif xaif.xsd xaif_base.xsd xaif_inlinable_intrinsics.xsd xaif_derivative_propagator.xsd xaif_output.xsd openad_tlm
	./openad_tlm -i $< -c ${XAIFSCHEMAROOT}/schema/examples/inlinable_intrinsics.xaif -o $@ 

# XAIF' -> WHIRL'
head_sf.xb.x2w.B : head_sf.xb.xaif xaif2whirl
	./xaif2whirl --structured head_sf.B $<

# WHIRL' -> F'
head_sf.xb.x2w.w2f.f: head_sf.xb.x2w.B whirl2f whirl2f_be
	./whirl2f -openad $<

# postprocess F'
head.once.f: head_sf.xb.x2w.w2f.f multi-pp1.pl
	perl multi-pp1.pl $(PP_FLAGS) $<

######################################################
# transformation part 2
######################################################

# paste in the array propagators 
head.once_sf.f : arrayProp.f head.once.f 
	cat $^ > $@

# F -> WHIRL
head.once_sf.B: head.once.f mfef90
	./mfef90 -F -N132 $<

# WHIRL -> XAIF
head.once_sf.xaif : head.once_sf.B whirl2xaif
	./whirl2xaif -n $< | sed 's/prefix=\"OpenAD_\"/prefix="oad"/' > $@ 

# XAIF -> XAIF'
head.once_sf.xb.xaif : head.once_sf.xaif xaif.xsd xaif_base.xsd xaif_inlinable_intrinsics.xsd xaif_derivative_propagator.xsd xaif_output.xsd openad_tlm
	./openad_adm -i $< -c ${XAIFSCHEMAROOT}/schema/examples/inlinable_intrinsics.xaif -o $@ 

# XAIF' -> WHIRL'
head.once_sf.xb.x2w.B : head.once_sf.xb.xaif xaif2whirl
	./xaif2whirl --structured head.once_sf.B $<

# WHIRL' -> F'
head.once_sf.xb.x2w.w2f.f: head.once_sf.xb.x2w.B whirl2f whirl2f_be
	./whirl2f -openad $<

# postprocess F'
head.once_sf.xb.x2w.w2f.pp.f: head.once_sf.xb.x2w.w2f.f multi-pp1.pl
	perl multi-pp.pl $(PP_FLAGS) $<

# extract the transformed globals module so we the right order of 
# compilation with respect to the globals checkpoint module
all_globals_mod.once_sf.f: head.once_sf.xb.x2w.w2f.pp.f
	cat $< | sed -n '/MODULE all_globals_mod/,/END MODULE/p' > $@

# remove the transformed globals module from the transformed head file
head.twice.f: head.once_sf.xb.x2w.w2f.pp.f
	cat $< | sed '/MODULE all_globals_mod/,/END MODULE/d' > $@

######################################################
# links to OpenAD tools and schemata
######################################################

%.xsd:
	$(LN) ${XAIFSCHEMAROOT}/schema/$@ .

mfef90: 
	$(LN) ${OPENADROOT}/Open64/osprey1.0/targ_ia32_ia64_linux/crayf90/sgi/mfef90 .

whirl2xaif xaif2whirl whirl2sexp sexp2whirl: 
	$(LN) ${OPENADROOT}/OpenADFortTk/OpenADFortTk-x86-Linux/bin/$@ .

%.pl: 
	$(LN) ${OPENADROOT}/OpenADFortTk/tools/multiprocess/$@ .

whirl2f whirl2f_be:
	$(LN) ${OPENADROOT}/Open64/osprey1.0/targ_ia32_ia64_linux/whirl2f/$@ .

openad_tlm:
	$(LN) ${OPENADROOT}/xaifBooster/algorithms/BasicBlockPreaccumulation/test/t $@

openad_adm:
	$(LN) ${OPENADROOT}/xaifBooster/algorithms/BasicBlockPreaccumulationReverse/test/t $@

######################################################
# compilation
######################################################

# make the driver
driver: $(MODULES) head.xb.x2w.w2f.pp.o driver.o 
	$(F95) $^ -o $@ 

numericalComparison: numericalComparison.o
	$(F95) $^ -o $@ 

run: params.conf tmpOutput driver numericalComparison 
	./driver

%.o:%.f
	$(F95) $(FIXEDFORMAT) -c $<

%.o:%.f90
	$(F95) $(FREEFORMAT) -c $<

######################################################
# miscellaneous
######################################################

tmpOutput: 
	mkdir -p $@

testAllclean:
	$(RM) head_sf.*  driver* params.conf ad_template.f *mod-whirl tmpOutput data.tmp

clean: testAllclean
	$(RM) *.xsd 
	$(RM) mfef90 whirl2xaif whirl2sexp xaif2whirl whirl2f_be whirl2f *.pl xaifBooster numericalComparison .lastRun
	$(RM) *.mod *.o

objs test : 

.PHONY: all objs test clean testAllclean run
