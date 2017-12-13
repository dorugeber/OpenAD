documentation: open64.ps

open64.ps: open64.dvi
	dvips -f open64.dvi > open64.ps

open64.dvi: open64.tex whirl.tex symtab.tex
	latex open64
	latex open64
	makeindex open64
	latex open64
	latex open64

# hyperref'ed, searchable PDF version
open64A.pdf: open64A.tex whirl.tex symtab.tex
	pdflatex open64A
	pdflatex open64A
	makeindex open64A
	pdflatex open64A
	pdflatex open64A

clean:
	rm -f open64.{ind,aux,dvi,idx,ilg,toc,log} open64A.{ind,aux,dvi,idx,ilg,toc,log,lof,out}

veryclean: clean
	rm -f open64.ps open64A.pdf
