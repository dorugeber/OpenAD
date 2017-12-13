# preprocess
func.pre.f90: func.f90
	${OPENADFORTTK_BASE}/tools/SourceProcessing/preProcess.py \
	--inputFormat=free -o $@ $<

clean: 
	rm -f func.pre.f90

.PHONY: clean
