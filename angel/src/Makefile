# $Id: Makefile,v 1.22 2008/02/28 14:57:32 gottschling Exp $
#############################################################
# This file is part of angel released under the BSD license #
# The full COPYRIGHT notice can be found in the top         #
# level directory of the angel distribution                 #
#############################################################


# BOOST_DIR must be defined in the environment !

# CXXFLAGS may be changed appropriately
#CXXFLAGS+= -O3 
CXXFLAGS+= -DNDEBUG
#CXXFLAGS+= -g
#CXXFLAGS+= -pg

#CPPFLAGS+= -DGNUPLOT # generate output for gnuplot (maybe part of output must be set as comment)
#CPPFLAGS+= -DWRITE_COSTS # write costs for instance in LSA.hpp
#CPPFLAGS+= -DSAME_RANDOM_VALUES # pseudo random values identical in each run, for debugging

FLIST=$(basename $(wildcard *.cpp))
OBJECTS=$(addsuffix .o, $(FLIST))

# include the dependency information 
DEPINCL = $(addsuffix .d, $(FLIST))

IDIR	= ../include
CXXFLAGS+= -c

ANGEL_ROOT = ../..
CXXFLAGS+= -I$(ANGEL_ROOT) 

# compile the objects
# as part of this we create dependency information 
# Remove old dependencies
# invoke compiler to create raw dependencies with processId attached
# appending to the dependencies:
#   all lines with the original targets removed
#   the line continuation removed
#   all empty lines removed
#   the remaining lines ended with ':'
# in order to represent all prerequisites as targets to deal with 
# removed prerequisites
%.o: %.cpp
	@set -e; rm -f $*.d; \
	   $(CXX) -M $(CPPFLAGS) $< > $*.d.$$$$; \
           sed -e 's,^[^:]*: *,,' \
	       -e 's, *\\$$,,' \
	       -e '/^$$/ d' \
	       -e 's,$$, :,' < $*.d.$$$$ >> $*.d; \
	   rm -f $*.d.$$$$
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

-include $(DEPINCL)

it:	all

all:	$(OBJECTS)

clean: 
	$(RM) $(RMFLAGS) *.o *~ *.d *.d.*

echo:
	echo "$(CPPFLAGS) : $(angel_config)"

.PHONY: it all clean echo









