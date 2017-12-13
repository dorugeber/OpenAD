glimmer_deriv.o:		glimmer_global.o glimmer_deriv.F90
glimmer_global.o:		glimmer_global.F90
glimmer_log.o:		glimmer_global.o glimmer_log.F90
glimmer_mask.o:		glimmer_global.o glimmer_physcon.o glimmer_mask.F90
glimmer_physcon.o:		glimmer_global.o glimmer_physcon.F90
glimmer_pmpt.o:		glimmer_global.o glimmer_physcon.o glimmer_pmpt.F90
glimmer_utils.o:		glimmer_global.o glimmer_log.o glimmer_utils.F90
glide_tempFullSoln.o:           glimmer_physcon.o \
			  	glimmer_log.o         glimmer_pmpt.o \
				glimmer_deriv.o        glimmer_mask.o        glimmer_utils.o \
				glimmer_global.o

testTemp.o:                     glide_tempFullSoln.o glide_velo.o testTemp.F90
glide_velo.o:			glimmer_utils.o glimmer_global.o glimmer_physcon.o glide_velo.F90
glimmer_pdd.o:			glimmer_pdd.F90 glimmer_global.o glimmer_log.o
