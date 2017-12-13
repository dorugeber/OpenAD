#############################################################
# This file is part of angel released under the BSD license #
# The full COPYRIGHT notice can be found in the top         #
# level directory of the angel distribution                 #
#############################################################
ifndef BOOSTROOT
  $(error "BOOSTROOT undefined")
endif

# CXXFLAGS may be changed appropriately
#CXXFLAGS+= -O3 
CXXFLAGS+= -DNDEBUG
#CXXFLAGS+= -g


# You shouldn't need to modify anything below this line

SOURCES = $(wildcard *.cpp)
TARGETS = $(SOURCES:.cpp=.run)

ANGEL_LIB_DIR = ../../

CXXFLAGS+= -I$(ANGEL_LIB_DIR) -I$(BOOSTROOT)

ANGEL_LIB = $(ANGEL_LIB_DIR)/angel/lib/libangel.a

all:  $(ANGEL_LIB) $(TARGETS)

$(ANGEL_LIB):
	cd $(ANGEL_LIB_DIR)/angel && $(MAKE)

%.run:%.cpp 
	$(CXX) $(CXXFLAGS) $(ANGEL_LIB) $< -o $@

clean:
	rm -f $(TARGETS)

.PHONY: all clean
