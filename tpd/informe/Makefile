#!/bin/bash
doc:
	pdflatex doc
	if [ -x /usr/bin/evince ]; then evince doc.pdf; else open doc.pdf; fi &

clean:
	rm -f *.aux *.log *.out *.blg *.bbl

build:
	pdflatex doc
	bibtex doc
	pdflatex doc
	pdflatex doc
