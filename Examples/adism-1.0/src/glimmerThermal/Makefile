# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# +                                                            +
# + Makefile for GLIMMER thermal code, when built within ADISM +
# +                                                            +
# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# 
# Copyright (C) 2013 ADISM contributors - see COPYRIGHT file 
# for list of contributors.
#
# This program is free software; you can redistribute it and/or 
# modify it under the terms of the GNU General Public License as 
# published by the Free Software Foundation; either version 2 of 
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
# Compiler
ifndef F90C
F90C=gfortran
endif

OBJS = glide_tempFullSoln.o  glimmer_physcon.o glimmer_log.o \
       glimmer_pmpt.o glimmer_deriv.o glimmer_mask.o glimmer_utils.o \
       glimmer_global.o glide_velo.o glimmer_pdd.o glimmer_config.o

LIBRARY = libglimtherm.a

${LIBRARY}: ${OBJS}
	ar -r $@ $^

%.o: %.F90
	$(F90C) -c -g $(F90CFLAGS) $< 


.PHONY: clean distclean

clean:
	rm -f *.o *.mod ${LIBRARY}

distclean: clean
	rm -f *.o *.mod $(EXE)     # removes object and executables

include f90_dep.mak

