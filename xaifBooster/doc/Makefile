# ========== begin copyright notice ===================
# This file is part of
# ---------------
# xaifBooster
# ---------------
# which is distributed under the BSD license.
# The full COPYRIGHT notice can be found in the top
# level directory of the xaifBooster distribution.
# ========== end copyright notice =====================
FLIST=$(wildcard *.tex *.eps)
xaifbooster_doc.ps: $(FLIST) xaifbooster_doc.bbl
	latex xaifbooster_doc
	dvips -t letter xaifbooster_doc -o

xaifbooster_doc.bbl: literature.bib
	latex xaifbooster_doc
	bibtex xaifbooster_doc

clean: 
	rm -f *~ *bak xaifbooster_doc.ps xaifbooster_doc.dvi xaifbooster_doc.log xaifbooster_doc.aux xaifbooster_doc.bbl xaifbooster_doc.blg  xaifbooster_doc.idx xaifbooster_doc.toc

.PHONY: clean
