INPUT = Diff_Ctrl_IOC_User_Guide.tex

PDF  = $(INPUT:.tex=.pdf)
ALL  = $(PDF)

all: $(ALL)

%.pdf: %.tex
	pdflatex -dSubsetFonts=true -dEmbedAllFonts=true $^
	pdflatex -dSubsetFonts=true -dEmbedAllFonts=true $^

.PHONY: clean realclean mrproper distclean

clean :
	rm -f *.eps *.dat *.log *.out *.aux *.dvi *.ps *.toc version.tex

realclean: clean
	rm -f *.pdf

mrproper: realclean

distclean: realclean
