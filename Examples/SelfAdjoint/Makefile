ifndef F90C
F90C=gfortran
endif

TDIRS=$(sort $(dir $(wildcard */)))

TDIRS_ALL=$(addsuffix _all, $(TDIRS))

all: driver $(TDIRS_ALL)

driver: lu.o  driver.o head.o 
	${F90C} -o $@ $^
%.o : %.f90
	${F90C} -o $@ -c $< 

define generateAllDir
 $(1)_all: 
	cd $(1) && $(MAKE) 
endef   

$(foreach TDIR, $(TDIRS), $(eval $(call generateAllDir, $(TDIR))))

TDIRS_CLEAN=$(addsuffix _clean, $(TDIRS))
clean: $(TDIRS_CLEAN)
	rm -f *.o *.mod driver  

define generateCleanDir
 $(1)_clean: 
	cd $(1) && $(MAKE) clean
endef   

$(foreach TDIR, $(TDIRS), $(eval $(call generateCleanDir, $(TDIR))))

.PHONY: all $(TDIRS_ALL) clean $(TDIRS_CLEAN)

