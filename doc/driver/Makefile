# Makefile for ~CLU/doc/driver

.SILENT:

RM    = /bin/rm -f
FLUFF = *~ *.old *.bak .*~ .*.old .*.bak

all:
	pdflatex debug
	pdflatex pclu
	pdflatex pclu
clean:
	${RM} *.aux *.dvi *.log ${FLUFF}

veryclean: clean
