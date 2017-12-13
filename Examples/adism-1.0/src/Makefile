# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# +                                                               +
# + ADISM makefile                                                +
# +                                                               +
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# 
# Copyright (C) 2013 ADISM contributors - see COPYRIGHT file 
# for list of contributors.
#
# This program is free software; you can redistribute it and/or 
# modify it under the terms of the GNU General Public License as 
# published by the Free Software Foundation; either version 3 of 
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, 
# but WITHOUT ANY WARRANTY; without even the implied warranty of 
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License 
# along with this program; if not, write to the Free Software 
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 
# 02111-1307 USA
#
# ADISM is maintained by:
#
# Ian Rutt, Swansea University (i.c.rutt@swansea.ac.uk)
# Jonathan McGovern, LGGE, France
#
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TOPDIR=.
include MakeDefs.mk

OBJS=adism_ncdf.o adism_mbal.o adism_umf.o adism_wrap.o adism_body.o adism_loop.o adism_core.o adism_mbal.o adism_umf.o adism_data.o
RTSUPP = w2f__types OAD_active OAD_tape OAD_cp OAD_rev revolve
ADSDIR = adsupport

EXE = adism adism_FD adism_sens adism_sens_par adism_adjoint

all: $(GLIMLIB) $(EXE)

$(GLIMLIB): 
	cd $(GLIMMER) && $(MAKE)

adism: adism.o $(OBJS)  $(GLIMLIB) 
	$(LD) -o $@ $^ $(LDFLAGS)

adism_FD: adism_FD.o $(OBJS) $(GLIMLIB) 
	$(LD) -o $@ $^ $(LDFLAGS)

adism_sens: adism_sens.o $(OBJS) $(GLIMLIB)
	$(LD) -o $@ $^ $(LDFLAGS)

adism_sens_par: adism_sens_par.o $(OBJS) $(GLIMLIB)
	${MPIFC} -o $@ $^ $(LDFLAGS) 

adism_adjoint: $(addsuffix .o, $(RTSUPP)) \
	adism_data.o adism_ncdf.o adism_adcore.o adism_adjoint.o \
	cpSupport.o $(GLIMLIB) ${UMFPACK_LIB_DIR}/libumfpack.a
	$(LD) -o $@ $^ $(LDFLAGS)

%.o: %.F90
	$(F90C) $(F90CFLAGS) -o $@ $< 

%.o: %.f90
	$(F90C) $(F90CFLAGS) -o $@ $< 

%.o: $(ADSDIR)/%.f90
	$(F90C) $(F90CFLAGS) -o $@ $< 

.PHONY: clean distclean

clean:
	cd $(GLIMMER) && $(MAKE) $@
	cd Adjoint && $(MAKE) $@
	rm -f *.o *.mod head.f *~  data.dat

distclean: clean
	cd $(GLIMMER) && $(MAKE) $@
	cd Adjoint && $(MAKE) $@
	rm -f *.o *.mod head.f *~  data.dat $(EXE)

adism_wrap.o: adism_wrap.F90 adism_body.o adism_core.o adism_loop.o adism_mbal.o adism_umf.o
adism_body.o: adism_body.F90 adism_core.o adism_ncdf.o adism_mbal.o
adism_ncdf.o: adism_ncdf.F90
adism_mbal.o: adism_mbal.F90 adism_core.o
adism_umf.o:  adism_umf.f90
adism_core.o: adism_core.F90
adism_loop.o: adism_loop.F90
adism.o:      adism.F90 adism_wrap.o adism_data.o
adism_FD.o:   adism_FD.F90 adism_wrap.o adism_data.o
adism_sens.o: adism_sens.F90 adism_wrap.o adism_data.o
adism_sens_par.o: adism_sens_par.F90 adism_wrap.o adism_data.o
	$(MPIFC) -g -c $(F90CFLAGS) $< 
adism_data.o: adism_data.F90 adism_core.o
adism_adcore.o: adism_adcore.f90 OAD_tape.o

w2f__types.o: $(ADSDIR)/w2f__types.f90
OAD_active.o: $(ADSDIR)/OAD_active.f90
OAD_cp.o: $(ADSDIR)/OAD_cp.f90
OAD_rev.o: $(ADSDIR)/OAD_rev.f90
OAD_tape.o: $(ADSDIR)/OAD_tape.f90
revolve.o: $(ADSDIR)/revolve.f90
