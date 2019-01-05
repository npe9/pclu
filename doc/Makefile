# Makefile for ~CLU/doc

.SILENT:

RM    = /bin/rm -f
FLUFF = *~ *.old *.bak .*~ .*.old .*.bak

all:
	cd driver; make
	pdflatex pclu
	pdflatex pclu
	pdflatex mini-pclu
	pdflatex mini-pclu

clean:
	cd driver; make clean
	${RM} *.aux *.dvi *.log .log ${FLUFF}

veryclean: clean
